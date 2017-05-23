#include "nettypes.h"
#include <sys/types.h>
#include <unistd.h>

char *u32a(uint32_t value) {

    register char *p, *p2;
    char *buf = palloc(11);
    char tmp;

    p = p2 = buf;

    do {
	*p++ = value % 10 + '0';
    } while ((value /= 10) > 0);

    *p-- = '\0';

    while(p>p2)
	tmp=*p, *p--=*p2, *p2++=tmp;

    return buf;

}

#define u16a(X) u32a((uint32_t)X)
#define u8a(X) u32a((uint32_t)X)

uint32_t au32(char *str, char *err) {

    uint32_t result = 0;
    int i;

    for (i = 0; str[i] >= '0' && str[i] <= '9'; ++i)
	 result = 10 * result + (str[i] - '0');

    if(i == 0 || str[i] != '\0')
	*err = 1;
    
    return result;

}

#define au16(X, Y) (uint16_t)au32(X, Y)
#define au8(X, Y) (uint8_t)au32(X, Y)

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(inetaddr_in);

Datum inetaddr_in(PG_FUNCTION_ARGS) {

    char *ip = PG_GETARG_CSTRING(0);
    unsigned char *ina = palloc(4);
    
    if(sscanf(ip, "%hhu.%hhu.%hhu.%hhu", &ina[0], &ina[1], &ina[2], &ina[3]) != 4)
	ereport(ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION), errmsg("invalid input syntax for inetaddr: \"%s\"", ip)));
	
    PG_RETURN_POINTER(ina);

}

PG_FUNCTION_INFO_V1(inetaddr_out);

Datum inetaddr_out(PG_FUNCTION_ARGS) {

    char *ina = PG_GETARG_POINTER(0);
    char *result = (char *) palloc(16);
    
    sprintf(result, "%hhu.%hhu.%hhu.%hhu", ina[0], ina[1], ina[2], ina[3]);
    
    PG_RETURN_CSTRING(result);

}

PG_FUNCTION_INFO_V1(inetaddr_recv);

Datum inetaddr_recv(PG_FUNCTION_ARGS) {

    StringInfo buf = (StringInfo) PG_GETARG_POINTER(0);
    
    PG_RETURN_POINTER(pq_getmsgbytes(buf, 4));
    
}

PG_FUNCTION_INFO_V1(inetaddr_send);

Datum inetaddr_send(PG_FUNCTION_ARGS) {

    char *ina = PG_GETARG_POINTER(0);
    StringInfoData buf;

    pq_begintypsend(&buf);
    pq_sendbytes(&buf, ina, 4);
    PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
    
}

PG_FUNCTION_INFO_V1(inetaddr_text);

Datum inetaddr_text(PG_FUNCTION_ARGS) {

    char *ina = PG_GETARG_POINTER(0);
    char buf[20];
    int len = sprintf(buf, "%hhu.%hhu.%hhu.%hhu", ina[0], ina[1], ina[2], ina[3]);
    
    text *out = palloc(len + VARHDRSZ);
    SET_VARSIZE(out, len + VARHDRSZ);
    
    memcpy(VARDATA(out), buf, len);
    
    PG_RETURN_TEXT_P(out);

}

PG_FUNCTION_INFO_V1(text_inetaddr);

Datum text_inetaddr(PG_FUNCTION_ARGS) {

    text *txt = PG_GETARG_TEXT_P(0);
    int tlen = VARSIZE(txt) - VARHDRSZ;
    char buf[20];
    
    if (tlen < sizeof(buf)) {
    
	char ip[4];
	unsigned char *ina;
    
	memcpy(buf, VARDATA(txt), tlen);
	buf[tlen] = 0;

	ina = palloc(4);
    
	if(sscanf(ip, "%hhu.%hhu.%hhu.%hhu", &ina[0], &ina[1], &ina[2], &ina[3]) != 4)
	    ereport(ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION), errmsg("invalid input syntax for inetaddr: \"%s\"", ip)));
	
	PG_RETURN_POINTER(ina);
	
    
    }

    ereport(NOTICE, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION), errmsg("invalid input length for inetaddr: \"%u\"", tlen)));
    PG_RETURN_NULL();

}

PG_FUNCTION_INFO_V1(inet_inetaddr);

Datum inet_inetaddr(PG_FUNCTION_ARGS) {

    inet *inetptr = PG_GETARG_INET_P(0);
    inet_struct *in = INET_STRUCT_DATA(inetptr);
    
    if (in->family == PGSQL_AF_INET) {
    
	unsigned char *p = in->INET_IPADDR;
	char *ip = palloc(4);
	memcpy(ip, p, 4);
	
	PG_RETURN_POINTER(ip);
    
    }

    ereport(NOTICE, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION), errmsg("invalid adress family for inet: \"%hu\", expected \"%hu\"", in->family, PGSQL_AF_INET)));
    PG_RETURN_NULL();

}

PG_FUNCTION_INFO_V1(inetaddr_inet);

Datum inetaddr_inet(PG_FUNCTION_ARGS) {

    char *ina = PG_GETARG_POINTER(0);
    inet *res = palloc0(VARHDRSZ + sizeof(inet_struct));
    inet_struct *in;
    unsigned char *p;

    SET_VARSIZE(res, VARHDRSZ + offsetof(inet_struct, INET_IPADDR) + 4);

    in = ((inet_struct *)VARDATA(res));

    in->bits = 32;
    in->family = PGSQL_AF_INET;

    p = in->INET_IPADDR;
    memcpy(p, ina, 4);

    PG_RETURN_INET_P(res);

}

/* USIGNED 1 BYTE */

PG_FUNCTION_INFO_V1(uint8_t_in);

Datum uint8_t_in(PG_FUNCTION_ARGS) {

    uint8_t x;
    char err = 0;
    
    x = au8(PG_GETARG_CSTRING(0), &err);
    
    if(err)
	ereport(ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION), errmsg("invalid input syntax for uint8_t: \"%s\"", PG_GETARG_CSTRING(0))));

    PG_RETURN_UINT8(x);
    
}

PG_FUNCTION_INFO_V1(uint8_t_out);

Datum uint8_t_out(PG_FUNCTION_ARGS) {

    PG_RETURN_CSTRING(u8a(PG_GETARG_UINT8(0)));
    
}

PG_FUNCTION_INFO_V1(uint8_t_recv);

Datum uint8_t_recv(PG_FUNCTION_ARGS) {

    StringInfo buf = (StringInfo) PG_GETARG_POINTER(0);

    PG_RETURN_UINT8((uint8_t)pq_getmsgint(buf, sizeof(uint8_t)));
    
}

PG_FUNCTION_INFO_V1(uint8_t_send);

Datum uint8_t_send(PG_FUNCTION_ARGS) {

    uint8_t arg1 = PG_GETARG_UINT8(0);
    StringInfoData buf;

    pq_begintypsend(&buf);
    pq_sendint(&buf, arg1, sizeof(uint8_t));
    PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
    
}

// CONVERT FROM uint8_t TO SMALLINT

PG_FUNCTION_INFO_V1(uint8_t_smallint);

Datum uint8_t_smallint(PG_FUNCTION_ARGS) {

    PG_RETURN_INT16((int16)PG_GETARG_UINT8(0));

}

// CONVERT FROM SMALLINT TO uint8_t

PG_FUNCTION_INFO_V1(smallint_uint8_t);

Datum smallint_uint8_t(PG_FUNCTION_ARGS) {

    PG_RETURN_UINT8((uint8_t)PG_GETARG_INT16(0));

}

// CONVERT FROM uint8_t TO INTEGER

PG_FUNCTION_INFO_V1(uint8_t_integer);

Datum uint8_t_integer(PG_FUNCTION_ARGS) {

    uint8_t x = PG_GETARG_UINT8(0);
    
    PG_RETURN_INT32((int32)x);

}

// CONVERT FROM INTEGER TO uint8_t

PG_FUNCTION_INFO_V1(integer_uint8_t);

Datum integer_uint8_t(PG_FUNCTION_ARGS) {

    int32 x = PG_GETARG_INT32(0);
    
    PG_RETURN_UINT8((uint8_t)x);

}

// CONVERT FROM uint8_t TO BIGINT

PG_FUNCTION_INFO_V1(uint8_t_bigint);

Datum uint8_t_bigint(PG_FUNCTION_ARGS) {

    uint8_t x = PG_GETARG_UINT8(0);

    PG_RETURN_INT64((int64)x);

}

// CONVERT FROM BIGINT TO uint8_t

PG_FUNCTION_INFO_V1(bigint_uint8_t);

Datum bigint_uint8_t(PG_FUNCTION_ARGS) {

    int64 x = PG_GETARG_INT64(0);
    
    PG_RETURN_UINT8((uint8_t)x);

}

/* UNSIGNED 2 BYTES */

PG_FUNCTION_INFO_V1(uint16_t_in);

Datum uint16_t_in(PG_FUNCTION_ARGS) {

    uint16_t x;
    char err = 0;

    x = au16(PG_GETARG_CSTRING(0), &err);
    
    if(err)
	ereport(ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION), errmsg("invalid input syntax for uint16_t: \"%s\"", PG_GETARG_CSTRING(0))));

    PG_RETURN_UINT16(x);
    
}

PG_FUNCTION_INFO_V1(uint16_t_out);

Datum uint16_t_out(PG_FUNCTION_ARGS) {

    PG_RETURN_CSTRING(u16a(PG_GETARG_UINT16(0)));
    
}

PG_FUNCTION_INFO_V1(uint16_t_recv);

Datum uint16_t_recv(PG_FUNCTION_ARGS) {

    StringInfo buf = (StringInfo) PG_GETARG_POINTER(0);

    PG_RETURN_UINT16((uint16_t) pq_getmsgint(buf, sizeof(uint16_t)));
    
}

PG_FUNCTION_INFO_V1(uint16_t_send);

Datum uint16_t_send(PG_FUNCTION_ARGS) {

    uint16_t arg1 = PG_GETARG_UINT16(0);
    StringInfoData buf;

    pq_begintypsend(&buf);
    pq_sendint(&buf, arg1, sizeof(uint16_t));
    PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
    
}

// CONVERT uint16_t TO SMALLINT

PG_FUNCTION_INFO_V1(uint16_t_smallint);

Datum uint16_t_smallint(PG_FUNCTION_ARGS) {

    uint16_t x = PG_GETARG_UINT16(0);
    
    PG_RETURN_INT16((int16)x);

}

// CONVERT SMALLINT TO uint16_t

PG_FUNCTION_INFO_V1(smallint_uint16_t);

Datum smallint_uint16_t(PG_FUNCTION_ARGS) {

    int16 x = PG_GETARG_INT16(0);
    
    PG_RETURN_UINT16((uint16_t)x);

}

// CONVERT uint16_t TO INTEGER

PG_FUNCTION_INFO_V1(uint16_t_integer);

Datum uint16_t_integer(PG_FUNCTION_ARGS) {

    uint16_t x = PG_GETARG_UINT16(0);
    
    PG_RETURN_INT32((int32)x);

}

// CONVERT INTEGER TO uint16_t

PG_FUNCTION_INFO_V1(integer_uint16_t);

Datum integer_uint16_t(PG_FUNCTION_ARGS) {

    int32_t x = PG_GETARG_INT32(0);
    
    PG_RETURN_UINT16((uint16_t)x);

}

// CONVERT uint16_t TO BIGINT

PG_FUNCTION_INFO_V1(uint16_t_bigint);

Datum uint16_t_bigint(PG_FUNCTION_ARGS) {

    uint16_t x = PG_GETARG_UINT16(0);
    
    PG_RETURN_INT64((int64)x);

}

// CONVERT BIGINT TO uint16_t

PG_FUNCTION_INFO_V1(bigint_uint16_t);

Datum bigint_uint16_t(PG_FUNCTION_ARGS) {

    int64 x = PG_GETARG_INT64(0);
    
    PG_RETURN_UINT16((uint16_t)x);

}

/* UNSIGNED 4 BYTES */

PG_FUNCTION_INFO_V1(uint32_t_in);

Datum uint32_t_in(PG_FUNCTION_ARGS) {

    char err = 0;
    
    uint32_t x = au32(PG_GETARG_CSTRING(0), &err);

    if(err)
	ereport(ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION), errmsg("invalid input syntax for uint32_t: \"%s\"", PG_GETARG_CSTRING(0))));
		
    PG_RETURN_UINT32(x);

}

PG_FUNCTION_INFO_V1(uint32_t_out);

Datum uint32_t_out(PG_FUNCTION_ARGS) {

    PG_RETURN_CSTRING(u32a(PG_GETARG_UINT32(0)));

}

PG_FUNCTION_INFO_V1(uint32_t_recv);

Datum uint32_t_recv(PG_FUNCTION_ARGS) {

    StringInfo  buf = (StringInfo) PG_GETARG_POINTER(0);

    PG_RETURN_UINT32(pq_getmsgint(buf, sizeof(uint32_t)));

}

PG_FUNCTION_INFO_V1(uint32_t_send);

Datum uint32_t_send(PG_FUNCTION_ARGS) {

    uint32_t arg1 = PG_GETARG_UINT32(0);
    StringInfoData buf;

    pq_begintypsend(&buf);
    pq_sendint(&buf, arg1, sizeof(uint32_t));

    PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
    
}

// CONVERT FROM uint32_t TO SMALLINT

PG_FUNCTION_INFO_V1(uint32_t_smallint);

Datum uint32_t_smallint(PG_FUNCTION_ARGS) {

    uint32_t x = PG_GETARG_UINT32(0);
    
    PG_RETURN_INT16((int16)x);

}

// CONVERT FROM SMALLINT TO uint32_t

PG_FUNCTION_INFO_V1(smallint_uint32_t);

Datum smallint_uint32_t(PG_FUNCTION_ARGS) {

    int16 x = PG_GETARG_INT16(0);
    
    PG_RETURN_UINT32((uint32_t)x);

}

// CONVERT FROM uint32_t TO INTEGER

PG_FUNCTION_INFO_V1(uint32_t_integer);

Datum uint32_t_integer(PG_FUNCTION_ARGS) {

    uint32_t x = PG_GETARG_UINT32(0);
    
    PG_RETURN_INT32((int32)x);

}

// CONVERT FROM INTEGER TO uint32_t

PG_FUNCTION_INFO_V1(integer_uint32_t);

Datum integer_uint32_t(PG_FUNCTION_ARGS) {

    int32 x = PG_GETARG_INT32(0);
    
    PG_RETURN_UINT32((uint32_t)x);

}

// CONVERT FROM uint32_t TO BIGINT

PG_FUNCTION_INFO_V1(uint32_t_bigint);

Datum uint32_t_bigint(PG_FUNCTION_ARGS) {

    uint32_t x = PG_GETARG_UINT32(0);
    
    PG_RETURN_INT64((int64)x);

}

// CONVERT FROM BIGINT TO uint32_t

PG_FUNCTION_INFO_V1(bigint_uint32_t);

Datum bigint_uint32_t(PG_FUNCTION_ARGS) {

    int64 x = PG_GETARG_INT64(0);
    
    PG_RETURN_UINT32((uint32_t)x);

}


// INDEX related functions

PG_FUNCTION_INFO_V1(uint32_t_cmp);

Datum uint32_t_cmp(PG_FUNCTION_ARGS) {

    uint32_t a = PG_GETARG_UINT32(0);
    uint32_t b = PG_GETARG_UINT32(1);
    
    if(a < b)
	PG_RETURN_INT32(-1);
	
    if(a == b)
	PG_RETURN_INT32(0);

    PG_RETURN_INT32(1);
    
}

PG_FUNCTION_INFO_V1(uint32_t_hash);

Datum uint32_t_hash(PG_FUNCTION_ARGS) {

    uint32_t a = PG_GETARG_UINT32(0);
    
    return hash_any((unsigned char *)&a, sizeof(uint32_t));

}

PG_FUNCTION_INFO_V1(inetaddr_cmp);

Datum inetaddr_cmp(PG_FUNCTION_ARGS) {

    int result = memcmp(PG_GETARG_POINTER(0), PG_GETARG_POINTER(1), 4);

    if(result < 0)
	PG_RETURN_INT32(-1);
	
    if(result == 0)
	PG_RETURN_INT32(0);
	
    PG_RETURN_INT32(1);
    
}

PG_FUNCTION_INFO_V1(inetaddr_hash);

Datum inetaddr_hash(PG_FUNCTION_ARGS) {

    return hash_any((unsigned char*)PG_GETARG_POINTER(0), 4);

}

PG_FUNCTION_INFO_V1(uint32_t_eq);

Datum uint32_t_eq(PG_FUNCTION_ARGS) {

    uint32_t a = PG_GETARG_UINT32(0);
    uint32_t b = PG_GETARG_UINT32(1);
    
    PG_RETURN_BOOL(a == b);

}

PG_FUNCTION_INFO_V1(inetaddr_eq);

Datum inetaddr_eq(PG_FUNCTION_ARGS) {

    PG_RETURN_BOOL(memcmp(PG_GETARG_POINTER(0), PG_GETARG_POINTER(1), 4) == 0 ? 1 : 0);

}

PG_FUNCTION_INFO_V1(inetaddr_neq);

Datum inetaddr_neq(PG_FUNCTION_ARGS) {

    PG_RETURN_BOOL(memcmp(PG_GETARG_POINTER(0), PG_GETARG_POINTER(1), 4) != 0 ? 1 : 0);

}

PG_FUNCTION_INFO_V1(inetaddr_ge);

Datum inetaddr_ge(PG_FUNCTION_ARGS) {

    PG_RETURN_BOOL(memcmp(PG_GETARG_POINTER(0), PG_GETARG_POINTER(1), 4) >= 0 ? 1 : 0);

}

PG_FUNCTION_INFO_V1(inetaddr_le);

Datum inetaddr_le(PG_FUNCTION_ARGS) {

    PG_RETURN_BOOL(memcmp(PG_GETARG_POINTER(0), PG_GETARG_POINTER(1), 4) <= 0 ? 1 : 0);

}

PG_FUNCTION_INFO_V1(inetaddr_gt);

Datum inetaddr_gt(PG_FUNCTION_ARGS) {

    PG_RETURN_BOOL(memcmp(PG_GETARG_POINTER(0), PG_GETARG_POINTER(1), 4) > 0 ? 1 : 0);

}

PG_FUNCTION_INFO_V1(inetaddr_lt);

Datum inetaddr_lt(PG_FUNCTION_ARGS) {

    PG_RETURN_BOOL(memcmp(PG_GETARG_POINTER(0), PG_GETARG_POINTER(1), 4) < 0 ? 1 : 0);

}

PG_FUNCTION_INFO_V1(uint32_t_eq_int4);

Datum uint32_t_eq_int4(PG_FUNCTION_ARGS) {

    uint32_t a = PG_GETARG_UINT32(0);
    int32 b = PG_GETARG_INT32(1);

    PG_RETURN_BOOL(a == b);

}

PG_FUNCTION_INFO_V1(uint32_t_eq_int8);

Datum uint32_t_eq_int8(PG_FUNCTION_ARGS) {

    uint32_t a = PG_GETARG_UINT32(0);
    int8 b = PG_GETARG_INT32(1);

    PG_RETURN_BOOL(a == b);

}

PG_FUNCTION_INFO_V1(uint32_t_neq);

Datum uint32_t_neq(PG_FUNCTION_ARGS) {

    uint32_t a = PG_GETARG_UINT32(0);
    uint32_t b = PG_GETARG_UINT32(1);

    PG_RETURN_BOOL(a != b);

}

PG_FUNCTION_INFO_V1(uint32_t_neq_int4);

Datum uint32_t_neq_int4(PG_FUNCTION_ARGS) {

    uint32_t a = PG_GETARG_UINT32(0);
    int32 b = PG_GETARG_INT32(1);

    PG_RETURN_BOOL(a != b);

}

PG_FUNCTION_INFO_V1(uint32_t_le);

Datum uint32_t_le(PG_FUNCTION_ARGS) {

    uint32_t a = PG_GETARG_UINT32(0);
    uint32_t b = PG_GETARG_UINT32(1);

    PG_RETURN_BOOL(a <= b);

}

PG_FUNCTION_INFO_V1(uint32_t_le_int4);

Datum uint32_t_le_int4(PG_FUNCTION_ARGS) {

    uint32_t a = PG_GETARG_UINT32(0);
    int32 b = PG_GETARG_INT32(1);

    PG_RETURN_BOOL(a <= b);

}

PG_FUNCTION_INFO_V1(uint32_t_le_int8);

Datum uint32_t_le_int8(PG_FUNCTION_ARGS) {

    uint32_t a = PG_GETARG_UINT32(0);
    int8 b = PG_GETARG_INT32(1);

    PG_RETURN_BOOL(a <= b);

}

PG_FUNCTION_INFO_V1(uint32_t_ge);

Datum uint32_t_ge(PG_FUNCTION_ARGS) {

    uint32_t a = PG_GETARG_UINT32(0);
    uint32_t b = PG_GETARG_UINT32(1);

    PG_RETURN_BOOL(a >= b);

}

PG_FUNCTION_INFO_V1(uint32_t_ge_int4);

Datum uint32_t_ge_int4(PG_FUNCTION_ARGS) {

    uint32_t a = PG_GETARG_UINT32(0);
    int32 b = PG_GETARG_INT32(1);

    PG_RETURN_BOOL(a >= b ? 1 : 0);

}

PG_FUNCTION_INFO_V1(uint32_t_lt);

Datum uint32_t_lt(PG_FUNCTION_ARGS) {

    uint32_t a = PG_GETARG_UINT32(0);
    uint32_t b = PG_GETARG_UINT32(1);

    PG_RETURN_BOOL(a < b);

}

PG_FUNCTION_INFO_V1(uint32_t_lt_int4);

Datum uint32_t_lt_int4(PG_FUNCTION_ARGS) {

    uint32_t a = PG_GETARG_UINT32(0);
    int32 b = PG_GETARG_INT32(1);

    PG_RETURN_BOOL(a < b);

}

PG_FUNCTION_INFO_V1(uint32_t_lt_int8);

Datum uint32_t_lt_int8(PG_FUNCTION_ARGS) {

    uint32_t a = PG_GETARG_UINT32(0);
    int8 b = PG_GETARG_INT64(1);

    PG_RETURN_BOOL(a < b);

}

PG_FUNCTION_INFO_V1(uint32_t_gt);

Datum uint32_t_gt(PG_FUNCTION_ARGS) {

    uint32_t a = PG_GETARG_UINT32(0);
    uint32_t b = PG_GETARG_UINT32(1);

    PG_RETURN_BOOL(a > b);

}

PG_FUNCTION_INFO_V1(uint32_t_gt_int4);

Datum uint32_t_gt_int4(PG_FUNCTION_ARGS) {

    uint32_t a = PG_GETARG_UINT32(0);
    int32 b = PG_GETARG_INT32(1);

    PG_RETURN_BOOL(a > b ? 1 : 0);

}

PG_FUNCTION_INFO_V1(uint32_t_gt_int8);

Datum uint32_t_gt_int8(PG_FUNCTION_ARGS) {

    uint32_t a = PG_GETARG_UINT32(0);
    int64 b = PG_GETARG_INT64(1);

    PG_RETURN_BOOL(a > b ? 1 : 0);

}

PG_FUNCTION_INFO_V1(uint32_t_ge_int8);

Datum uint32_t_ge_int8(PG_FUNCTION_ARGS) {

    uint32_t a = PG_GETARG_UINT32(0);
    int64 b = PG_GETARG_INT64(1);

    PG_RETURN_BOOL(a >= b ? 1 : 0);

}

PG_FUNCTION_INFO_V1(uint32_t_neq_int8);

Datum uint32_t_neq_int8(PG_FUNCTION_ARGS) {

    uint32_t a = PG_GETARG_UINT32(0);
    int64 b = PG_GETARG_INT64(1);

    PG_RETURN_BOOL(a != b ? 1 : 0);

}
