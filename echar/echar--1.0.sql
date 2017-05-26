CREATE TYPE echar;

CREATE FUNCTION echar_in(cstring) RETURNS echar AS 'MODULE_PATHNAME' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION echar_out(echar) RETURNS cstring AS 'MODULE_PATHNAME' LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE echar (
    internallength = -1,
    input = echar_in,
    output = echar_out
);

CREATE FUNCTION echar_bytea(echar) RETURNS bytea AS 'MODULE_PATHNAME' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION bytea_echar(bytea) RETURNS echar AS 'MODULE_PATHNAME' LANGUAGE C IMMUTABLE STRICT;
CREATE CAST ( bytea AS echar) WITHOUT FUNCTION;
CREATE CAST ( echar AS bytea) WITHOUT FUNCTION;
CREATE CAST (echar as varchar) WITH INOUT AS ASSIGNMENT;
CREATE CAST (varchar AS echar) WITH INOUT AS ASSIGNMENT;

CREATE FUNCTION echar_eq(echar,echar) RETURNS boolean AS 'MODULE_PATHNAME','echar_eq' LANGUAGE C STRICT IMMUTABLE;
CREATE FUNCTION echar_ne(echar,echar) RETURNS boolean AS 'MODULE_PATHNAME','echar_ne' LANGUAGE C STRICT IMMUTABLE;
CREATE FUNCTION echar_gt(echar,echar) RETURNS boolean AS 'MODULE_PATHNAME','echar_gt' LANGUAGE C STRICT IMMUTABLE;
CREATE FUNCTION echar_ge(echar,echar) RETURNS boolean AS 'MODULE_PATHNAME','echar_ge' LANGUAGE C STRICT IMMUTABLE;
CREATE FUNCTION echar_lt(echar,echar) RETURNS boolean AS 'MODULE_PATHNAME','echar_lt' LANGUAGE C STRICT IMMUTABLE;
CREATE FUNCTION echar_le(echar,echar) RETURNS boolean AS 'MODULE_PATHNAME','echar_le' LANGUAGE C STRICT IMMUTABLE;
CREATE FUNCTION echar_cmp(echar,echar) RETURNS integer AS 'MODULE_PATHNAME','echar_cmp' LANGUAGE C STRICT IMMUTABLE;

CREATE OPERATOR = (
       LEFTARG = echar,
       RIGHTARG = echar,
       PROCEDURE = echar_eq,
       COMMUTATOR = =,
       NEGATOR = <>,
       RESTRICT = eqsel,
       JOIN = eqjoinsel,
       MERGES,
       HASHES
);
CREATE OPERATOR <> (
       LEFTARG = echar,
       RIGHTARG = echar,
       PROCEDURE = echar_ne,
       COMMUTATOR = <>,
       NEGATOR = =,
       RESTRICT = neqsel,
       JOIN = neqjoinsel
);

CREATE OPERATOR #<# (
       LEFTARG = echar,
       RIGHTARG = echar,
       PROCEDURE = echar_lt,
       COMMUTATOR = #>#,
       NEGATOR = #>=#,
       RESTRICT = scalarltsel,
       JOIN = scalarltjoinsel
);
CREATE OPERATOR #<=# (
       LEFTARG = echar,
       RIGHTARG = echar,
       PROCEDURE = echar_le,
       COMMUTATOR = #>=#,
       NEGATOR = #>#,
       RESTRICT = scalarltsel,
       JOIN = scalarltjoinsel
);
CREATE OPERATOR #># (
       LEFTARG = echar,
       RIGHTARG = echar,
       PROCEDURE = echar_gt,
       COMMUTATOR = #<#,
       NEGATOR = #<=#,
       RESTRICT = scalargtsel,
       JOIN = scalargtjoinsel
);
CREATE OPERATOR #>=# (
       LEFTARG = echar,
       RIGHTARG = echar,
       PROCEDURE = echar_ge,
       COMMUTATOR = #<=#,
       NEGATOR = #<#,
       RESTRICT = scalargtsel,
       JOIN = scalargtjoinsel
);

CREATE OPERATOR CLASS btree_echar_ops
DEFAULT FOR TYPE echar USING btree
AS
        OPERATOR        1       #<# ,
        OPERATOR        2       #<=# ,
        OPERATOR        3       = ,
        OPERATOR        4       #>=# ,
        OPERATOR        5       #># ,
        FUNCTION        1       echar_cmp(echar,echar);
