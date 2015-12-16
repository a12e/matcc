MatC is a subset of the C language that aims to make matrix computation easier. matcc a compiler for this language
written as part of a school project.

Building matcc
===============

These are the dependencies of matcc :

  * a C compiler
  * cmake
  * flex
  * bison
  * spim

On Ubuntu, you can run `apt-get install build-essential cmake flex bison spim`

Once you installed these dependencies you can build matcc like any other CMake project :
```
mkdir build
cd build
cmake .. && make
```

Testing matcc
=============

After you built matcc feel free to run all tests to check if everything is correct :
```
cd build
ctest
```

To see what tests are available run `ctest -N`

To run a single test do `ctest -R 62` where 62 is a regexp choosing the test(s)

