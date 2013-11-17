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
    hid_t vls_t;
    herr_t status;

    h5md_element pos;

    int i, j;
    double r[10][3];
    int species[10], dims[1];

    char *vls_data;

    vls_data = "Custom parameter value";

    H5open();

    file = h5md_create_file("hop.h5", "Pierre de Buyl", NULL, "use_h5md", "N/A");

    grp = H5Gcreate(file, "particles", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    b_grp = H5Gcreate(grp, "beads", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    o_grp = H5Gcreate(file, "observables", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    p_grp = H5Gcreate(file, "parameters", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    vls_t = H5Tcopy(H5T_C_S1);
    H5Tset_size(vls_t, H5T_VARIABLE);

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

    h5md_create_fixed_data_scalar(p_grp, "custom_parameter", vls_t, &vls_data);

    // Update r in a loop and write data to the file.
    for (j=0;j<2;j++) {
      for (i=0;i<10;i++) r[i][0] += 0.1*i*i;
      h5md_append(pos, r, j+1, (double) j+1);
    }

    status = H5Gclose(grp);
    status = H5Gclose(b_grp);
    status = H5Gclose(o_grp);
    status = H5Gclose(p_grp);
    status = H5Tclose(vls_t);

    H5Fclose(file);

    H5close();

    return 0;

}
