EO-PSI Protocol
===============

Efficient protocol to both privately outsource set data to the cloud and privately outsource secure computation of the Private Set Intersection of two clients.

Dependencies
------------

This library makes use of other very popular libraries.

*   GMP
*   NTL
*   Crypto++


Compile from source
-------------------

You can use CMake to compile this library from source.
Notice that for some distros you need to install *-dev packages too.

Clone the repository; then,

    $ cd eo-psi
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
*   `eopsisimul`
*   `cherpars`

In order to use them correctly, just run them with the argument `-h` and read the syntax.

If you do not want or can not install the library into your system, you may need to allow binary files to find the library files.
Depending on your system, there are several and different ways you can easily find surfing the web.

Publication
-----------
[Efficient Delegated Private Set Intersection on Outsourced Private Datasets](http://ieeexplore.ieee.org/document/7934388/