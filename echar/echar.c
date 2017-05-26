#include "echar.h"
#include <sys/types.h>
#include <unistd.h>

PG_MODULE_MAGIC;

void processErrors(void) {

    unsigned long errCode;

    while((errCode = ERR_get_error())) {
        char *err = ERR_error_string(errCode, NULL);
        elog(NOTICE, "%s\n", err);
    }

}

#define cleanup() processErrors(); EVP_CIPHER_CTX_free(ctx); return -1;

int aes_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {

    EVP_CIPHER_CTX *ctx = NULL;
    int len = 0, ciphertext_len = 0;

    if(!(ctx = EVP_CIPHER_CTX_new())) {
	cleanup();
    }

    if(!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
	cleanup();
    }

    if(plaintext) {
        if(!EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) {
	    cleanup();
	}
        ciphertext_len = len;
    }

    if(EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1) {
	cleanup();
    }
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int aes_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext) {

    EVP_CIPHER_CTX *ctx = NULL;
    int len = 0, plaintext_len = 0, ret;

    if(!(ctx = EVP_CIPHER_CTX_new())) {
	cleanup();
    }

    if(!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        cleanup();
    }

    if(ciphertext) {
        if(!EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) {
            cleanup();
	}

        plaintext_len = len;
    }

    ret = EVP_DecryptFinal_ex(ctx, plaintext + len, &len);

    EVP_CIPHER_CTX_free(ctx);

    if(ret > 0) {
        plaintext_len += len;
        return plaintext_len;
    } else  {
        return -1;
    }

}


unsigned char **initKeyAndIv(void);
void cleanupKeyAndIv(unsigned char **ki);

unsigned char **initKeyAndIv(void) {

    unsigned char **result;
    Portal portal;
    const char *encr_key = NULL;
    const char *encr_iv = NULL;
    char *comment, *pos;

    result = palloc(sizeof(char *) * 2);
    portal = GetPortalByName("");

    if(portal) {

        pos = strstr(portal->sourceText, "/*");

        if(pos) {

            comment = pstrdup(pos+2);
            pos = strstr(comment, "*/");
            *pos = '\0';

            encr_key = strstr(comment, "key=");

            if(encr_key) {
                encr_key+=4;
            }

            encr_iv = strstr(comment,"iv=");

            if(encr_iv) {
                encr_iv += 3;
            }

        }

    }

    if(!encr_key) {
        encr_key = GetConfigOption("encryption.key", true, false);
    }

    if(!encr_iv) {
        encr_iv = GetConfigOption("encryption.iv", true, false);
    }

    if(encr_key) {
        result[0] = palloc(33);
        bzero(result[0], 33);
        strncpy((char*)result[0], encr_key, 32);
    } else {
        result[0] = NULL;
    }

    if(encr_iv) {
        result[1] = palloc(17);
        bzero(result[1], 17);
        strncpy((char*)result[1], encr_iv, 16);
    } else {
        result[1] = NULL;
    }

    return result;

}

void cleanupKeyAndIv(unsigned char **ki) {

    if(ki) {

        pfree(ki[0]);
        pfree(ki[1]);
        pfree(ki);

    }

}

PG_FUNCTION_INFO_V1(echar_in);
Datum echar_in(PG_FUNCTION_ARGS) {

    char    *s = PG_GETARG_CSTRING(0);
    bytea   *result;
    unsigned char **keyAndIv;
    unsigned unencrSize, encrSize;

    keyAndIv = initKeyAndIv();

    if(!keyAndIv[0]) {
        elog(ERROR, "[SET encryption.key TO value] must be called prior to invoking this function");
    }

    if(!keyAndIv[1]) {
        elog(ERROR, "[SET encryption.iv TO value] must be called prior to invoking this function");
    }

    unencrSize = strlen(s);
    result = palloc((unencrSize / 16 + 1) * 16 + VARHDRSZ);

    encrSize = aes_encrypt((unsigned char*)s, unencrSize, keyAndIv[0], keyAndIv[1], (unsigned char *)VARDATA(result));

    cleanupKeyAndIv(keyAndIv);

    SET_VARSIZE(result, encrSize + VARHDRSZ);
    PG_RETURN_BYTEA_P(result);

}

PG_FUNCTION_INFO_V1(echar_out);
Datum echar_out(PG_FUNCTION_ARGS) {

    bytea   *echar1 = PG_GETARG_BYTEA_PP(0);
    unsigned char *result;
    unsigned char **keyAndIv;
    int encrSize, unencrSize;


    keyAndIv = initKeyAndIv();

    if(!keyAndIv[0]) {
        elog(ERROR, "[SET encryption.key TO value] must be called prior to invoking this function");
    }

    if(!keyAndIv[1]) {
        elog(ERROR, "[SET encryption.iv TO value] must be called prior to invoking this function");
    }

    encrSize = VARSIZE_ANY_EXHDR(echar1);
    result = palloc0(encrSize+1); // encrypted size is always >= unencrypted due to padding

    unencrSize = aes_decrypt((unsigned char*)VARDATA_ANY(echar1), encrSize, keyAndIv[0], keyAndIv[1], result);

    if(unencrSize < 0)  {

        elog(ERROR, "Decryption failed. Invalid key/IV ?");

    }

    result[unencrSize]='\0';

    cleanupKeyAndIv(keyAndIv);

    PG_RETURN_CSTRING(result);

}

PG_FUNCTION_INFO_V1(echar_bytea);
Datum echar_bytea(PG_FUNCTION_ARGS) {
    PG_RETURN_BYTEA_P(PG_GETARG_BYTEA_PP(0));
}

PG_FUNCTION_INFO_V1(bytea_echar);
Datum bytea_echar(PG_FUNCTION_ARGS) {
    PG_RETURN_BYTEA_P(PG_GETARG_BYTEA_PP(0));
}

PG_FUNCTION_INFO_V1(echar_cmp);
Datum echar_cmp(PG_FUNCTION_ARGS) {

    bytea *echar1 = PG_GETARG_BYTEA_PP(0);
    bytea *echar2 = PG_GETARG_BYTEA_PP(1);
    int res = 0;
    unsigned char *result1 = NULL, *result2 = NULL;
    unsigned char **keyAndIv;
    int encrSize, unencrSize;

    keyAndIv = initKeyAndIv();

    if(!keyAndIv[0]) {
        elog(ERROR, "[SET encryption.key TO value] must be called prior to invoking this function");
    }

    if(!keyAndIv[1]) {
        elog(ERROR, "[SET encryption.iv TO value] must be called prior to invoking this function");
    }

    encrSize = VARSIZE_ANY_EXHDR(echar1);

    result1 = palloc0(encrSize+1);

    unencrSize = aes_decrypt((unsigned char*)VARDATA_ANY(echar1), encrSize, keyAndIv[0], keyAndIv[1], result1);

    if(unencrSize < 0)  {

        elog(ERROR, "Decryption failed. Invalid key/IV ?");

    }

    result1[unencrSize]='\0';

    encrSize = VARSIZE_ANY_EXHDR(echar2);

    result2 = palloc0(encrSize+1);

    unencrSize = aes_decrypt((unsigned char*)VARDATA_ANY(echar2), encrSize, keyAndIv[0], keyAndIv[1], result2);

    if(unencrSize < 0)  {

        elog(ERROR, "Decryption failed. Invalid key/IV ?");

    }

    result2[unencrSize]='\0';

    res = strcmp((char*)result1, (char*)result2);

    pfree(result1);
    pfree(result2);

    cleanupKeyAndIv(keyAndIv);

    PG_RETURN_INT32(res);

}

PG_FUNCTION_INFO_V1(echar_eq);
Datum echar_eq(PG_FUNCTION_ARGS) {
    int res = DatumGetInt32(DirectFunctionCall2(echar_cmp, PG_GETARG_DATUM(0), PG_GETARG_DATUM(1)));
    PG_RETURN_BOOL(res == 0);
}

PG_FUNCTION_INFO_V1(echar_ne);
Datum echar_ne(PG_FUNCTION_ARGS) {
    int res = DatumGetInt32(DirectFunctionCall2(echar_cmp,PG_GETARG_DATUM(0),PG_GETARG_DATUM(1)));
    PG_RETURN_BOOL(res != 0);
}

PG_FUNCTION_INFO_V1(echar_gt);
Datum echar_gt(PG_FUNCTION_ARGS) {
    int res = DatumGetInt32(DirectFunctionCall2(echar_cmp, PG_GETARG_DATUM(0), PG_GETARG_DATUM(1)));
    PG_RETURN_BOOL(res > 0);
}

PG_FUNCTION_INFO_V1(echar_ge);
Datum echar_ge(PG_FUNCTION_ARGS) {
    int res = DatumGetInt32(DirectFunctionCall2(echar_cmp, PG_GETARG_DATUM(0), PG_GETARG_DATUM(1)));
    PG_RETURN_BOOL(res >= 0);
}

PG_FUNCTION_INFO_V1(echar_lt);
Datum echar_lt(PG_FUNCTION_ARGS) {
    int res = DatumGetInt32(DirectFunctionCall2(echar_cmp,PG_GETARG_DATUM(0),PG_GETARG_DATUM(1)));
    PG_RETURN_BOOL(res < 0);
}

PG_FUNCTION_INFO_V1(echar_le);
Datum echar_le(PG_FUNCTION_ARGS) {
    int res = DatumGetInt32(DirectFunctionCall2(echar_cmp,PG_GETARG_DATUM(0),PG_GETARG_DATUM(1)));
    PG_RETURN_BOOL(res <= 0);
}
