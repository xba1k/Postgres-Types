#include "zchar.h"
#include <sys/types.h>
#include <unistd.h>
#include <zlib.h>

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(zchar_in);

Datum zchar_in(PG_FUNCTION_ARGS) {

    char	*s = PG_GETARG_CSTRING(0);
    bytea	*result;
    uLong ucompSize = strlen(s);
    uLong compSize = compressBound(ucompSize);

    result = palloc(ucompSize + VARHDRSZ);
    compress((Bytef *)VARDATA(result), &compSize, (Bytef *)s, ucompSize);
    SET_VARSIZE(result, compSize + VARHDRSZ);

    PG_RETURN_BYTEA_P(result);

}

PG_FUNCTION_INFO_V1(zchar_out);

Datum zchar_out(PG_FUNCTION_ARGS) {

    bytea	*vlena = PG_GETARG_BYTEA_PP(0);
    char	*result;

    uLong compSize = VARSIZE_ANY_EXHDR(vlena);
    uLong ucompSize = compSize * 4;

    result = palloc(ucompSize);

    uncompress((Bytef *)result, &ucompSize, (Bytef *)VARDATA_ANY(vlena), compSize);
    result[ucompSize]='\0';

    PG_RETURN_CSTRING(result);

}

Datum zchar_bytea(PG_FUNCTION_ARGS) {
    PG_RETURN_BYTEA_P(PG_GETARG_BYTEA_PP(0));
}

Datum bytea_zchar(PG_FUNCTION_ARGS) {
    PG_RETURN_BYTEA_P(PG_GETARG_BYTEA_PP(0));
}
