#ifndef _INTLIST_H_
#define _INTLIST_H_

extern "C" {

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

}

extern "C" {

Datum intlist_in(PG_FUNCTION_ARGS);
Datum intlist_out(PG_FUNCTION_ARGS);

}

#endif
