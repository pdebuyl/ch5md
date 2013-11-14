
#include "hdf5.h"
#include "ch5md.h"

int main(void) {
    hid_t    file;
    hid_t    grp, b_grp, p_grp;
    herr_t   status;

    h5md_element pos;

    int i, j;
    double r[10][3];

    file = h5md_create_file("hop.h5", "Pierre de Buyl", "use_h5md", "N/A");

    grp = H5Gcreate(file, "particles", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    b_grp = H5Gcreate(grp, "beads", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    
    pos = h5md_create_time_data(b_grp, "position", 10, 3, H5T_NATIVE_DOUBLE);

    for (i=0;i<10;i++) {
      r[i][0] = i;
    }

    for (j=0;j<2;j++) {
      for (i=0;i<10;i++) r[i][0] += 0.1*i*i;
      h5md_append(pos, r, j+1, (double) j+1);
    }

    status = H5Gclose(grp);
    status = H5Gclose(b_grp);

    H5Fclose(file);

    return 0;

}
