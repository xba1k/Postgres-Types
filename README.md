# Postgres-Types
Various useful datatypes for PostgreSQL powered data warehouses

## zchar

Simple data type to store strings as Zlib-compressed blobs. This type should support most varchar operators via assignment. 

## echar

This datatype implements storing of strings in AES encrypted form. It is specifically intended for encryption-at-rest in multi-tenant databases where rows in the same table could belong to different tenants. It does support indexing, but due to its "fail on bad key" behavior multi-tenant tables should be partitioned with each tenant's rows being grouped in a separate parition thus also allowing index seggregation. See included README.txt for simple example on usage of that type.

## nettypes

Implements a set of datatypes useful for storage of network data.

1. inetaddr - store IPv4 addresses as 32-bits instead of bulkier built-in inet type.
2. uint8_t - unsigned 8 bit integer (for protocol numbers).
3. uint16_t - unsigned 16 bit integer (for port numbers, packet lengths).
4. uint32_t - unsigned 32 bit integer (TCP sequence numbers).

## Build/Install

Copy the types to contrib directory of Postgres source and run make install. Follow up with "create extension."
