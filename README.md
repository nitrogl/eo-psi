HashBuckets Library
===================

Dependencies
------------

This library makes use of other very popular libraries.

*   GMP
*   NTL
*   CryptoPP


Compile from source
-------------------

You need CMake to compile this library from source.

Clone the repository; then,

    $ cd haskbuckets
    $ mkdir -p build
    $ cd build
    $ cmake ../
    $ make
    $ make doc # (optional) if you want to build the documentation too
    # make install # (optional) if you want to install the library

where `$` at the beginning indicates that you should not need root permissions, while `#` at the beginning means that you may need root permissions to perform the operation.


Running some tests
------------------

The compilation produces three test binary files:

*   `zzrndgen`
*   `hbtest`
*   `bptest`
*   `eopsisrv`
*   `eopsicli`

In order to use them correctly, just run them with the argument `-h` and read the syntax.

If you do not want or can not install the library into your system, you may need to allow binary files to find the library files.
Depending on your system, there are several and different ways you can easily find surfing the web.

