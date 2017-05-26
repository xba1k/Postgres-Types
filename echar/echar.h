#ifndef _ECHAR_H_
#define _ECHAR_H_

#include "postgres.h"
#include "mb/pg_wchar.h"
#include "access/gist.h"
#include "access/itup.h"
#include "access/skey.h"
#include "access/hash.h"
#include "catalog/pg_type.h"
#include "utils/array.h"
#include "utils/builtins.h"
#include "storage/bufpage.h"
#include "lib/stringinfo.h"
#include "libpq/pqformat.h"
#include "utils/inet.h"
#include "sys/socket.h"
#include "utils/portal.h"
#include "utils/guc.h"
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <string.h>

void processErrors(void);
int aes_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext);
int aes_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext);

Datum echar_in(PG_FUNCTION_ARGS);
Datum echar_out(PG_FUNCTION_ARGS);
Datum echar_bytea(PG_FUNCTION_ARGS);
Datum bytea_echar(PG_FUNCTION_ARGS);
Datum echar_le(PG_FUNCTION_ARGS);
Datum echar_lt(PG_FUNCTION_ARGS);
Datum echar_ge(PG_FUNCTION_ARGS);
Datum echar_gt(PG_FUNCTION_ARGS);
Datum echar_ne(PG_FUNCTION_ARGS);
Datum echar_eq(PG_FUNCTION_ARGS);
Datum echar_cmp(PG_FUNCTION_ARGS);

#endif
