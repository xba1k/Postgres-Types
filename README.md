# Postgres-Types
Various useful datatypes for PostgreSQL powered data warehouses

* Zchar

Simple data type to store strings as Zlib-compressed blobs. This type should support most varchar operators via assignment.

* Build/Install

Copy the types to contrib directory of Postgres source and run make install. Follow up with "create extension."
