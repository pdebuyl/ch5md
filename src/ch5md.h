/* Copyright (C) 2013 Pierre de Buyl
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 */

#ifndef CH5MD_H
#define CH5MD_H
#include "hdf5.h"
#include <stdbool.h>

#define CH5MD_RANK_ERROR -10

typedef struct {
  hid_t group;
  hid_t step;
  hid_t time;
  hid_t value;
  hid_t datatype;
  int is_time;
} h5md_element;

hid_t h5md_create_file (const char *filename, const char *author, const char *author_email, const char *creator, const char *creator_version);
h5md_element h5md_create_time_data(hid_t loc, const char *name, int N, int D, hid_t datatype);
h5md_element h5md_create_fixed_data_simple(hid_t loc, const char *name, int rank, int int_dims[], hid_t datatype, void *data);
h5md_element h5md_create_fixed_data_scalar(hid_t loc, const char *name, hid_t datatype, void *data);
int h5md_append(h5md_element e, void *data, int step, double time);

#endif
