Below is the short demonstration for the usage of this datatype, note the intentional
error cases :

postgres=# create extension echar;
CREATE EXTENSION
postgres=# create table echar_test(stuff echar);
CREATE TABLE
postgres=# insert into echar_test values('stuff');
ERROR:  [SET encryption.key TO value] must be called prior to invoking this function
LINE 1: insert into echar_test values('stuff');
                                      ^
postgres=# set encryption.key to 'encryption key';
SET
postgres=# insert into echar_test values('stuff');
ERROR:  [SET encryption.iv TO value] must be called prior to invoking this function
LINE 1: insert into echar_test values('stuff');
                                      ^
postgres=# set encryption.iv to 'encryption iv';
SET
postgres=# insert into echar_test values('stuff');
INSERT 0 1
postgres=# select * from echar_test;
 stuff 
-------
 stuff
(1 row)

postgres=# set encryption.key to 'another key';
SET
postgres=# insert into echar_test values('more stuff');
INSERT 0 1
postgres=# select * from echar_test;
ERROR:  Decryption failed. Invalid key/IV ?
postgres=# select * from echar_test limit 1 offset 1;
   stuff    
------------
 more stuff
(1 row)

postgres=# select * from echar_test limit 1 offset 0;
ERROR:  Decryption failed. Invalid key/IV ?
postgres=# set encryption.key to 'encryption key';
SET
postgres=# select * from echar_test limit 1 offset 0;
 stuff 
-------
 stuff
(1 row)

postgres=# select * from echar_test limit 1 offset 1;
ERROR:  Decryption failed. Invalid key/IV ?
postgres=# 


There is also a trivial implementation of passing KEY/IV pairs via comments in this fashion :

insert /* key=abcdefhskrhsl3dj iv=shridlrkewjewlkek */ into test_echar values('stuff');

but it's too simple and should be improved quite a bit before use.
