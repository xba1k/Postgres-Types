CREATE TYPE inetaddr;

CREATE FUNCTION inetaddr_in(cstring) RETURNS inetaddr AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION inetaddr_out(inetaddr) RETURNS cstring AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION inetaddr_recv(internal) RETURNS inetaddr AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION inetaddr_send(inetaddr) RETURNS bytea AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;


CREATE TYPE inetaddr (
    internallength = 4,
    input = inetaddr_in,
    output = inetaddr_out,
    receive = inetaddr_recv,
    send = inetaddr_send,
    alignment = int4
);

CREATE FUNCTION inetaddr_inet(inetaddr) RETURNS inet AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION inet_inetaddr(inet) RETURNS inetaddr AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION inetaddr_text(inetaddr) RETURNS text AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION text_inetaddr(text) RETURNS inetaddr AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (text as inetaddr) WITH FUNCTION text_inetaddr(text);
CREATE CAST (inet as inetaddr) WITH FUNCTION inet_inetaddr(inet) AS ASSIGNMENT;
CREATE CAST (inetaddr as inet) WITH FUNCTION inetaddr_inet(inetaddr) AS ASSIGNMENT;
CREATE CAST (inetaddr as text) WITH FUNCTION inetaddr_text(inetaddr);


CREATE TYPE uint8_t;

CREATE FUNCTION uint8_t_in(cstring) RETURNS uint8_t AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint8_t_out(uint8_t) RETURNS cstring AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint8_t_recv(internal) RETURNS uint8_t AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint8_t_send(uint8_t) RETURNS bytea AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE uint8_t (
    internallength = 1,
    input = uint8_t_in,
    output = uint8_t_out,
    receive = uint8_t_recv,
    send = uint8_t_send,
    alignment = char,
    passedbyvalue
);

CREATE FUNCTION uint8_t_smallint(uint8_t) RETURNS smallint AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint8_t_integer(uint8_t) RETURNS integer AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint8_t_bigint(uint8_t) RETURNS bigint AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION smallint_uint8_t(smallint) RETURNS uint8_t AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION integer_uint8_t(integer) RETURNS uint8_t AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION bigint_uint8_t(bigint) RETURNS uint8_t AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE uint16_t;

CREATE FUNCTION uint16_t_in(cstring) RETURNS uint16_t AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint16_t_out(uint16_t) RETURNS cstring AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint16_t_recv(internal) RETURNS uint16_t AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint16_t_send(uint16_t) RETURNS bytea AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE uint16_t (
    internallength = 2,
    input = uint16_t_in,
    output = uint16_t_out,
    receive = uint16_t_recv,
    send = uint16_t_send,
    alignment = int2,
    passedbyvalue
);

CREATE FUNCTION uint16_t_smallint(uint16_t) RETURNS smallint AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint16_t_integer(uint16_t) RETURNS integer AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint16_t_bigint(uint16_t) RETURNS bigint AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION smallint_uint16_t(smallint) RETURNS uint16_t AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION integer_uint16_t(integer) RETURNS uint16_t AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION bigint_uint16_t(bigint) RETURNS uint16_t AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE uint32_t;

CREATE FUNCTION uint32_t_in(cstring) RETURNS uint32_t AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint32_t_out(uint32_t) RETURNS cstring AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint32_t_recv(internal) RETURNS uint32_t AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint32_t_send(uint32_t) RETURNS bytea AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE uint32_t (
    internallength = 4,
    input = uint32_t_in,
    output = uint32_t_out,
    receive = uint32_t_recv,
    send = uint32_t_send,
    alignment = int4,
    passedbyvalue
);

CREATE FUNCTION uint32_t_smallint(uint32_t) RETURNS smallint AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint32_t_integer(uint32_t) RETURNS integer AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint32_t_bigint(uint32_t) RETURNS bigint AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION smallint_uint32_t(smallint) RETURNS uint32_t AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION integer_uint32_t(integer) RETURNS uint32_t AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION bigint_uint32_t(bigint) RETURNS uint32_t AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION uint32_t_cmp(uint32_t, uint32_t) RETURNS integer AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint32_t_hash(uint32_t) RETURNS integer AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION uint32_t_gt(uint32_t, uint32_t) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint32_t_ge(uint32_t, uint32_t) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint32_t_lt(uint32_t, uint32_t) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint32_t_le(uint32_t, uint32_t) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint32_t_eq(uint32_t, uint32_t) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint32_t_neq(uint32_t, uint32_t) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION uint32_t_gt_int8(uint32_t, int8) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint32_t_ge_int8(uint32_t, int8) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint32_t_lt_int8(uint32_t, int8) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint32_t_le_int8(uint32_t, int8) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint32_t_eq_int8(uint32_t, int8) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint32_t_neq_int8(uint32_t, int8) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION inetaddr_cmp(inetaddr, inetaddr) RETURNS integer AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION inetaddr_hash(inetaddr) RETURNS integer AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION uint32_t_gt_int4(uint32_t, int4) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint32_t_ge_int4(uint32_t, int4) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint32_t_lt_int4(uint32_t, int4) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint32_t_le_int4(uint32_t, int4) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint32_t_eq_int4(uint32_t, int4) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION uint32_t_neq_int4(uint32_t, int4) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION inetaddr_gt(inetaddr, inetaddr) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION inetaddr_ge(inetaddr, inetaddr) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION inetaddr_lt(inetaddr, inetaddr) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION inetaddr_le(inetaddr, inetaddr) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION inetaddr_eq(inetaddr, inetaddr) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION inetaddr_neq(inetaddr, inetaddr) RETURNS bool AS 'MODULE_PATH' LANGUAGE C IMMUTABLE STRICT;

CREATE CAST ( uint8_t AS smallint) with function uint8_t_smallint(uint8_t) AS IMPLICIT;
CREATE CAST ( smallint AS uint8_t) with function smallint_uint8_t(smallint) AS IMPLICIT;
CREATE CAST ( uint16_t AS smallint) with function uint16_t_smallint(uint16_t) AS IMPLICIT;
CREATE CAST ( smallint AS uint16_t) with function smallint_uint16_t(smallint) AS IMPLICIT;
CREATE CAST ( uint32_t AS smallint) with function uint32_t_smallint(uint32_t) AS IMPLICIT;
CREATE CAST ( smallint AS uint32_t) with function smallint_uint32_t(smallint) AS IMPLICIT;

CREATE CAST ( uint8_t AS integer) with function uint8_t_integer(uint8_t) AS IMPLICIT;
CREATE CAST ( integer AS uint8_t) with function integer_uint8_t(integer) AS IMPLICIT;
CREATE CAST ( uint16_t AS integer) with function uint16_t_integer(uint16_t) AS IMPLICIT;
CREATE CAST ( integer AS uint16_t) with function integer_uint16_t(integer) AS IMPLICIT;
CREATE CAST ( uint32_t AS integer) with function uint32_t_integer(uint32_t) AS IMPLICIT;
CREATE CAST ( integer AS uint32_t) with function integer_uint32_t(integer) AS IMPLICIT;

CREATE CAST ( uint8_t AS bigint) with function uint8_t_bigint(uint8_t) AS IMPLICIT;
CREATE CAST ( bigint AS uint8_t) with function bigint_uint8_t(bigint) AS IMPLICIT;
CREATE CAST ( uint16_t AS bigint) with function uint16_t_bigint(uint16_t) AS IMPLICIT;
CREATE CAST ( bigint AS uint16_t) with function bigint_uint16_t(bigint) AS IMPLICIT;
CREATE CAST ( uint32_t AS bigint) with function uint32_t_bigint(uint32_t) AS IMPLICIT;
CREATE CAST ( bigint AS uint32_t) with function bigint_uint32_t(bigint) AS IMPLICIT;

CREATE OPERATOR < (

    LEFTARG = uint32_t,
    RIGHTARG = uint32_t,
    PROCEDURE = uint32_t_lt,
    COMMUTATOR = '>',
    NEGATOR = '>=',
    RESTRICT = scalarltsel,
    JOIN = scalarltjoinsel

);

CREATE OPERATOR < (

    LEFTARG = uint32_t,
    RIGHTARG = int8,
    PROCEDURE = uint32_t_lt_int8,
    COMMUTATOR = '>',
    NEGATOR = '>=',
    RESTRICT = scalarltsel,
    JOIN = scalarltjoinsel

);

CREATE OPERATOR < (

    LEFTARG = uint32_t,
    RIGHTARG = int4,
    PROCEDURE = uint32_t_lt_int4,
    COMMUTATOR = '>',
    NEGATOR = '>=',
    RESTRICT = scalarltsel,
    JOIN = scalarltjoinsel

);

CREATE OPERATOR <= (

    LEFTARG = uint32_t,
    RIGHTARG = uint32_t,
    PROCEDURE = uint32_t_le,
    COMMUTATOR = '>=',
    NEGATOR = '>',
    RESTRICT = scalarltsel,
    JOIN = scalarltjoinsel

);

CREATE OPERATOR > (

    LEFTARG = uint32_t,
    RIGHTARG = uint32_t,
    PROCEDURE = uint32_t_gt,
    COMMUTATOR = '<',
    NEGATOR = '<=',
    RESTRICT = scalargtsel,
    JOIN = scalargtjoinsel

);

CREATE OPERATOR > (

    LEFTARG = uint32_t,
    RIGHTARG = int8,
    PROCEDURE = uint32_t_gt_int8,
    COMMUTATOR = '<',
    NEGATOR = '<=',
    RESTRICT = scalargtsel,
    JOIN = scalargtjoinsel

);

CREATE OPERATOR > (

    LEFTARG = uint32_t,
    RIGHTARG = int4,
    PROCEDURE = uint32_t_gt_int4,
    COMMUTATOR = '<',
    NEGATOR = '<=',
    RESTRICT = scalargtsel,
    JOIN = scalargtjoinsel

);

CREATE OPERATOR >= (

    LEFTARG = uint32_t,
    RIGHTARG = uint32_t,
    PROCEDURE = uint32_t_ge,
    COMMUTATOR = '<=',
    NEGATOR = '<',
    RESTRICT = scalargtsel,
    JOIN = scalargtjoinsel

);

CREATE OPERATOR >= (

    LEFTARG = uint32_t,
    RIGHTARG = int8,
    PROCEDURE = uint32_t_ge_int8,
    COMMUTATOR = '<=',
    NEGATOR = '<',
    RESTRICT = scalargtsel,
    JOIN = scalargtjoinsel

);

CREATE OPERATOR >= (

    LEFTARG = uint32_t,
    RIGHTARG = int4,
    PROCEDURE = uint32_t_ge_int4,
    COMMUTATOR = '<=',
    NEGATOR = '<',
    RESTRICT = scalargtsel,
    JOIN = scalargtjoinsel

);

CREATE OPERATOR = (

    LEFTARG = uint32_t,
    RIGHTARG = uint32_t,
    PROCEDURE = uint32_t_eq,
    COMMUTATOR = '=',
    NEGATOR = '<>',
    RESTRICT = eqsel,
    JOIN = eqjoinsel,
    SORT1 = '<',
    SORT2 = '<',
    HASHES

);

CREATE OPERATOR = (

    LEFTARG = uint32_t,
    RIGHTARG = int8,
    PROCEDURE = uint32_t_eq_int8,
    COMMUTATOR = '=',
    NEGATOR = '<>',
    RESTRICT = eqsel,
    JOIN = eqjoinsel,
    SORT1 = '<',
    SORT2 = '<',
    HASHES

);

CREATE OPERATOR = (

    LEFTARG = uint32_t,
    RIGHTARG = int4,
    PROCEDURE = uint32_t_eq_int4,
    COMMUTATOR = '=',
    NEGATOR = '<>',
    RESTRICT = eqsel,
    JOIN = eqjoinsel,
    SORT1 = '<',
    SORT2 = '<',
    HASHES

);

CREATE OPERATOR <> (

    LEFTARG = uint32_t,
    RIGHTARG = uint32_t,
    PROCEDURE = uint32_t_neq,
    COMMUTATOR = '<>',
    NEGATOR = '=',
    RESTRICT = neqsel,
    JOIN = neqjoinsel

);

CREATE OPERATOR <> (

    LEFTARG = uint32_t,
    RIGHTARG = int8,
    PROCEDURE = uint32_t_neq_int8,
    COMMUTATOR = '<>',
    NEGATOR = '=',
    RESTRICT = neqsel,
    JOIN = neqjoinsel

);

CREATE OPERATOR <> (

    LEFTARG = uint32_t,
    RIGHTARG = int4,
    PROCEDURE = uint32_t_neq_int4,
    COMMUTATOR = '<>',
    NEGATOR = '=',
    RESTRICT = neqsel,
    JOIN = neqjoinsel

);

CREATE OPERATOR <= (

    LEFTARG = uint32_t,
    RIGHTARG = int4,
    PROCEDURE = uint32_t_le_int4,
    COMMUTATOR = '>=',
    NEGATOR = '>',
    RESTRICT = scalarltsel,
    JOIN = scalarltjoinsel

);

CREATE OPERATOR <= (

    LEFTARG = uint32_t,
    RIGHTARG = int8,
    PROCEDURE = uint32_t_le_int8,
    COMMUTATOR = '>=',
    NEGATOR = '>',
    RESTRICT = scalarltsel,
    JOIN = scalarltjoinsel

);

CREATE OPERATOR < (

    LEFTARG = inetaddr,
    RIGHTARG = inetaddr,
    PROCEDURE = inetaddr_lt,
    COMMUTATOR = '>',
    NEGATOR = '>=',
    RESTRICT = scalarltsel,
    JOIN = scalarltjoinsel

);

CREATE OPERATOR > (

    LEFTARG = inetaddr,
    RIGHTARG = inetaddr,
    PROCEDURE = inetaddr_gt,
    COMMUTATOR = '<',
    NEGATOR = '<=',
    RESTRICT = scalargtsel,
    JOIN = scalargtjoinsel

);

CREATE OPERATOR <= (

    LEFTARG = inetaddr,
    RIGHTARG = inetaddr,
    PROCEDURE = inetaddr_le,
    COMMUTATOR = '>=',
    NEGATOR = '>',
    RESTRICT = scalarltsel,
    JOIN = scalarltjoinsel

);

CREATE OPERATOR = (

    LEFTARG = inetaddr,
    RIGHTARG = inetaddr,
    PROCEDURE = inetaddr_eq,
    COMMUTATOR = '=',
    NEGATOR = '<>',
    RESTRICT = eqsel,
    JOIN = eqjoinsel,
    SORT1 = '<',
    SORT2 = '<',
    HASHES

);

CREATE OPERATOR >= (

    LEFTARG = inetaddr,
    RIGHTARG = inetaddr,
    PROCEDURE = inetaddr_ge,
    COMMUTATOR = '<',
    NEGATOR = '<=',
    RESTRICT = scalargtsel,
    JOIN = scalargtjoinsel

);

CREATE OPERATOR <> (

    LEFTARG = inetaddr,
    RIGHTARG = inetaddr,
    PROCEDURE = inetaddr_neq,
    COMMUTATOR = '<>',
    NEGATOR = '=',
    RESTRICT = neqsel,
    JOIN = neqjoinsel

);

CREATE OPERATOR CLASS btree_uint32_t_ops
DEFAULT FOR TYPE uint32_t USING btree
AS
    OPERATOR        1       < ,
    OPERATOR        2       <= ,
    OPERATOR        3       = ,
    OPERATOR        4       >= ,
    OPERATOR        5       > ,
    FUNCTION        1       uint32_t_cmp(uint32_t, uint32_t);

CREATE OPERATOR CLASS hash_uint32_t_ops
DEFAULT FOR TYPE uint32_t USING hash
AS
    OPERATOR        1       = ,
    FUNCTION        1       uint32_t_hash(uint32_t);



CREATE OPERATOR CLASS btree_inetaddr_ops
DEFAULT FOR TYPE inetaddr USING btree
AS
    OPERATOR        1       < ,
    OPERATOR        2       <= ,
    OPERATOR        3       = ,
    OPERATOR        4       >= ,
    OPERATOR        5       > ,
    FUNCTION        1       inetaddr_cmp(inetaddr, inetaddr);

CREATE OPERATOR CLASS hash_inetaddr_ops
DEFAULT FOR TYPE inetaddr USING hash
AS
    OPERATOR        1       = ,
    FUNCTION        1       inetaddr_hash(inetaddr);
