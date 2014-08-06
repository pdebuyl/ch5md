/* Copyright (C) 2013-2014 Pierre de Buyl
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 */

#include "hdf5.h"
#include "ch5md.h"

#define NPART 16

int main(void) {
    h5md_file file;
    h5md_particles_group beads;
    hid_t vls_t;
    herr_t status;

    int i, j;
    double r[NPART][3];
    int species[NPART], dims[1];

    char *vls_data;

    vls_data = "Custom parameter value";

    H5open();

    file = h5md_create_file("data_use_ch5md.h5", "Pierre de Buyl", NULL, "use_h5md", "N/A");

    beads = h5md_create_particles_group(file, "beads");
    
    vls_t = H5Tcopy(H5T_C_S1);
    H5Tset_size(vls_t, H5T_VARIABLE);

    // Create a time-dependent dataset
    // There is no data yet in "pos"
    beads.position = h5md_create_time_data(beads.group, "position", NPART, 3, H5T_NATIVE_DOUBLE);

    for (i=0;i<NPART;i++) {
      r[i][0] = i;
      species[i] = 1;
    }
    dims[0] = NPART;

    // Create a simple fixed-in-time dataset
    // Data is written immediately here
    beads.species = h5md_create_fixed_data_simple(beads.group, "species", 1, dims, H5T_NATIVE_INT, species);

    // Create a scalar fixed-in-time dataset
    // Data is written immediately here
    i = 12345;
    h5md_create_fixed_data_scalar(file.observables, "scalar_dset", H5T_NATIVE_INT, &i);

    h5md_create_fixed_data_scalar(file.parameters, "custom_parameter", vls_t, &vls_data);

    // Update r in a loop and write data to the file.
    for (j=0;j<2;j++) {
      for (i=0;i<NPART;i++) r[i][0] += 0.1*i*i;
      h5md_append(beads.position, r, j+1, (double) j+1);
    }

    status = H5Tclose(vls_t);

    h5md_close_time_data(beads.position);
    h5md_close_file(file);

    H5close();

    return 0;

}
