/* Copyright (C) 2013 Pierre de Buyl
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 */

#include "hdf5.h"
#include "ch5md.h"

#define MIN_CHUNK 10
#define MAX_CHUNK 256

hid_t h5md_create_file (const char *filename, const char *author, const char *creator, const char *creator_version)
{
  hid_t file;
  hid_t g, g1;
  hid_t a, s, t;
  herr_t status;

  file = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  g = H5Gcreate(file, "h5md", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  s = H5Screate(H5S_SCALAR);
  t = H5Tcopy(H5T_C_S1);
  status = H5Tset_size(t, H5T_VARIABLE);

  g1 = H5Gcreate(g, "author", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  a = H5Acreate(g1, "name", t, s, H5P_DEFAULT, H5P_DEFAULT);
  status = H5Awrite(a, t, &author);
  status = H5Aclose(a);
  status = H5Gclose(g1);

  g1 = H5Gcreate(g, "creator", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  a = H5Acreate(g1, "name", t, s, H5P_DEFAULT, H5P_DEFAULT);
  status = H5Awrite(a, t, &creator);
  status = H5Aclose(a);
  a = H5Acreate(g1, "version", t, s, H5P_DEFAULT, H5P_DEFAULT);
  status = H5Awrite(a, t, &creator_version);
  status = H5Aclose(a);
  status = H5Gclose(g1);


  return file;
}

h5md_element h5md_create_time_data (hid_t loc, const char *name, int N, int D, hid_t datatype)
{

  h5md_element td;

  hid_t spc, plist;
  hsize_t dims[3], max_dims[3], chunks[3];
  herr_t status;

  int tmp;

  dims[0] = 0 ;
  dims[1] = N ;
  dims[2] = D ;
  max_dims[0] = H5S_UNLIMITED ;
  max_dims[1] = N ;
  max_dims[2] = D ;
  chunks[0] = 10 ;
  tmp = 256;
  if (MAX_CHUNK<N/4) {
    chunks[1] = MAX_CHUNK;
  } else {
    if (N/4>MIN_CHUNK) {
      chunks[1] = N/4;
    } else {
      chunks[1] = MIN_CHUNK;
    }
  }
  chunks[2] = D ;

  td.group = H5Gcreate(loc, name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  spc = H5Screate_simple( 1 , dims, max_dims);
  plist = H5Pcreate(H5P_DATASET_CREATE);
  status = H5Pset_chunk(plist, 1, chunks);
  td.time = H5Dcreate(td.group, "time", H5T_NATIVE_DOUBLE, spc, H5P_DEFAULT, plist, H5P_DEFAULT);
  td.step = H5Dcreate(td.group, "step", H5T_NATIVE_INT, spc, H5P_DEFAULT, plist, H5P_DEFAULT);
  H5Pclose(plist);
  H5Sclose(spc);

  spc = H5Screate_simple( 3 , dims, max_dims) ;
  plist = H5Pcreate(H5P_DATASET_CREATE);
  status = H5Pset_chunk(plist, 3, chunks);
  td.value = H5Dcreate(td.group, "value", datatype, spc, H5P_DEFAULT, plist, H5P_DEFAULT);
  H5Pclose(plist);
  status = H5Sclose(spc);

  td.datatype = datatype;
  td.is_time = true;

  return td;

}

int h5md_extend_by_one(hid_t dset, hsize_t *dims) {

  hid_t file_space;
  int rank;
  hsize_t maxdims[H5S_MAX_RANK];

  // Get dataset information
  file_space = H5Dget_space(dset);
  rank = H5Sget_simple_extent_ndims(file_space);
  if (rank > H5S_MAX_RANK) {
    return CH5MD_RANK_ERROR;
  }
  H5Sget_simple_extent_dims(file_space, dims, maxdims);
  H5Sclose(file_space);

  // Extend dimensions by one
  dims[0] = dims[0]+1;
  H5Dset_extent(dset, dims);

  return 0;

}

int h5md_append(h5md_element e, void *data, int step, double time) {

  hid_t mem_space, file_space;
  int i, rank;
  hsize_t dims[H5S_MAX_RANK], maxdims[H5S_MAX_RANK];
  hsize_t start[H5S_MAX_RANK], count[H5S_MAX_RANK];

  h5md_extend_by_one(e.step, dims);

  // Define hyperslab selection
  start[0] = dims[0]-1;
  count[0] = 1;

  // Select the hyperslab
  file_space = H5Dget_space(e.step);
  rank = H5Sget_simple_extent_ndims(file_space);
  mem_space = H5Screate_simple(rank-1, dims+1, NULL);
  // Define hyperslab selection
  start[0] = dims[0]-1;
  count[0] = 1;
  for (i=1 ; i<rank ; i++) {
    start[i] = 0;
    count[i] = dims[i];
  }
  H5Sselect_hyperslab(file_space, H5S_SELECT_SET, start, NULL, count, NULL);
  H5Dwrite(e.step, H5T_NATIVE_INT, mem_space, file_space, H5P_DEFAULT, (void *)&step);
  H5Sclose(file_space);
  H5Sclose(mem_space);

  h5md_extend_by_one(e.time, dims);

  // Define hyperslab selection
  start[0] = dims[0]-1;
  count[0] = 1;

  // Select the hyperslab
  file_space = H5Dget_space(e.time);
  rank = H5Sget_simple_extent_ndims(file_space);
  mem_space = H5Screate_simple(rank-1, dims+1, NULL);
  // Define hyperslab selection
  start[0] = dims[0]-1;
  count[0] = 1;
  for (i=1 ; i<rank ; i++) {
    start[i] = 0;
    count[i] = dims[i];
  }
  H5Sselect_hyperslab(file_space, H5S_SELECT_SET, start, NULL, count, NULL);
  H5Dwrite(e.time, H5T_NATIVE_DOUBLE, mem_space, file_space, H5P_DEFAULT, (void *)&time);
  H5Sclose(file_space);
  H5Sclose(mem_space);

  h5md_extend_by_one(e.value, dims);

  // Select the hyperslab
  file_space = H5Dget_space(e.value);
  rank = H5Sget_simple_extent_ndims(file_space);
  mem_space = H5Screate_simple(rank-1, dims+1, NULL);

  // Define hyperslab selection
  start[0] = dims[0]-1;
  count[0] = 1;
  for (i=1 ; i<rank ; i++) {
    start[i] = 0;
    count[i] = dims[i];
  }
  H5Sselect_hyperslab(file_space, H5S_SELECT_SET, start, NULL, count, NULL);
  H5Dwrite(e.value, e.datatype, mem_space, file_space, H5P_DEFAULT, data);
  H5Sclose(file_space);
  H5Sclose(mem_space);

  return 0;
}
