CREATE TYPE intlist;

CREATE FUNCTION intlist_in(cstring) RETURNS intlist AS 'MODULE_PATHNAME' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION intlist_out(intlist) RETURNS cstring AS 'MODULE_PATHNAME' LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE intlist (
    internallength = -1,
    input = intlist_in,
    output = intlist_out,
    storage = external
);

CREATE CAST (intlist as varchar) WITH INOUT AS ASSIGNMENT;
CREATE CAST (varchar AS intlist) WITH INOUT AS ASSIGNMENT;
CREATE CAST (text as intlist) WITH INOUT AS ASSIGNMENT;
CREATE CAST (intlist as text) WITH INOUT AS ASSIGNMENT;
CREATE CAST (intlist as bytea) WITHOUT FUNCTION;
CREATE CAST (bytea as intlist) WITHOUT FUNCTION;
