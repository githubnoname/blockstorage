BlockStorage
============

Test task is about storing and organizing data blocks in blobs. Good point to start code exploration is the test, see `src/test_blob.cpp`.

Requirements
------------

C++11, was tested with gcc-4.8.5.
Unix-like OS, was tested on Centos 7.

Build & check
-------------

        make && make check

bin/storage
-----------

        The CLI utility. Can be used to play with blobs and blocks.

        # init new blob in the file 'data1'. Set limit to 3 blocks.
        $ bin/storage -f data1 --init 3

        # put first block, id=5, data=hello.
        $ bin/storage -f data1 --put 5 hello

        # second one
        $ bin/storage -f data1 --put 7 world

        # third one. make a reference to alread y exists block. use hexdump -C data1
        $ bin/storage -f data1 --put 2 hello

        # over limit
        $ bin/storage -f data1 --put 10 hello
        Can't insert data

        # wrong id
        $ bin/storage -f data1 --get 1
        Can't get data

        # get blocks
        $ bin/storage -f data1 --get 2
        2 => 'hello'
        $ bin/storage -f data1 --get 5
        5 => 'hello'
        $ bin/storage -f data1 --get 7
        7 => 'world'
