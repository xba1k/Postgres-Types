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

## intlist

This was a proof of concept demonstrating a datatype that could store comma-seperated lists of integers (i.e. 100,3,423,4) more
efficiently than just text while being transparent to the application and not suffering from compression related performance penalties. In addition to parsing the string into an array of integers it utilizes bits from https://github.com/lemire/FastPFor project to perform extremely fast delta encoding and bit packing operations on these integers thus reducing the on-disk column footprint. It has demonstrated size reduction from 24GB down to 7GB using a real production dataset. Lists stored will be sorted in order to facilitate delta encoding. This extension depends on libsnappy.

## Build/Install

Copy the types to contrib directory of Postgres source and run make install. Follow up with "create extension."
