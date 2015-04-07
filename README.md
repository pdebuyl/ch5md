ch5md : Read and write H5MD files in C
======================================

Copyright (C) 2013-2015 Pierre de Buyl

ch5md is a set of C routines to manipulate H5MD files. H5MD is a file format
specification based on [HDF5](http://www.hdfgroup.org/HDF5/) for storing
molecular data, whose development is found at <http://nongnu.org/h5md/>.

ch5md is developed by Pierre de Buyl and is released under the 3-clause BSD
license that can be found in the file LICENSE.

The example file `use_ch5md.c` can be used as a starting point to use the
library.

    git clone https://github.com/pdebuyl/ch5md.git
    cd ch5md
    make -C examples

The executable `use_h5md` produces a H5MD file: `data_use_ch5md.h5`.

To generate the doxygen-based documentation:

    make -C doc
