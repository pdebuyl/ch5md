/* Copyright (C) 2013 Pierre de Buyl
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 */

#include "hdf5.h"
#include "ch5md.h"

int main(void) {
    hid_t file;
    hid_t grp, b_grp, p_grp;
    hid_t o_grp;
    herr_t status;

    h5md_element pos;

    int i, j;
    double r[10][3];
    int species[10], dims[1];

    file = h5md_create_file("hop.h5", "Pierre de Buyl", "use_h5md", "N/A");

    grp = H5Gcreate(file, "particles", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    b_grp = H5Gcreate(grp, "beads", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    o_grp = H5Gcreate(file, "observables", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    
    // Create a time-dependent dataset
    // There is no data yet in "pos"
    pos = h5md_create_time_data(b_grp, "position", 10, 3, H5T_NATIVE_DOUBLE);

    for (i=0;i<10;i++) {
      r[i][0] = i;
      species[i] = 1;
    }
    dims[0] = 10;

    // Create a simple fixed-in-time dataset
    // Data is written immediately here
    h5md_create_fixed_data_simple(b_grp, "species", 1, dims, H5T_NATIVE_INT, species);

    // Create a scalar fixed-in-time dataset
    // Data is written immediately here
    i = 12345;
    h5md_create_fixed_data_scalar(o_grp, "scalar_dset", H5T_NATIVE_INT, &i);

    // Update r in a loop and write data to the file.
    for (j=0;j<2;j++) {
      for (i=0;i<10;i++) r[i][0] += 0.1*i*i;
      h5md_append(pos, r, j+1, (double) j+1);
    }

    status = H5Gclose(grp);
    status = H5Gclose(b_grp);
    status = H5Gclose(o_grp);

    H5Fclose(file);

    return 0;

}
