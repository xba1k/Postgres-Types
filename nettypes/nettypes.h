#ifndef _NETTYPES_H_
#define _NETTYPES_H_

#include "postgres.h"

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

#ifndef PG_GETARG_UINT8
#define PG_GETARG_UINT8(n)  DatumGetUInt8(PG_GETARG_DATUM(n))
#endif

#ifndef PG_RETURN_UINT8
#define PG_RETURN_UINT8(x)  return DatumGetUInt8(x)
#endif

#ifndef PG_RETURN_UINT16
#define PG_RETURN_UINT16(x)  return DatumGetUInt16(x)
#endif

#ifndef INET_STRUCT_DATA
#define INET_STRUCT_DATA(is_) ((inet_struct *)VARDATA_ANY(is_))
#endif

#ifndef INET_IPADDR
#define INET_IPADDR ipaddr
#endif

char *u32a(uint32_t value);
uint32_t au32(char *str, char *err);

#endif
