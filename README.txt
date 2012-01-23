libipaca README
===============

libipaca is a C++ library for the calculation of isotope patterns given a
stoichiometry. The implementation is work in progress.

The current master branch features:

* an implementation of Rockwood and Haimi's Mercury7 (sparse binary 
  convolution) algorithm.
* support for arbitrary, user-defined stoichiometry and spectrum types
* a straightforward, easy-to-use interface:

    MyStoichiometry s;
    // do something with the stoichiometry
    // ...
    // now get the isotope pattern
    ipaca::Mercury7<MyStoichiometry, MySpectrum> mercury7;
    MySpectrum spectrum = mercury7(stoichiometry);
    // ...

