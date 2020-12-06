#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

using namespace std;

/* for SLD tata stream */
typedef union {
  int i;
  float f;
} fi_union;

/* Application parameters */
typedef struct{
  const char* ppm_filename;
  const char* sld_filename;

} app_settings;

/* the binary image of the input SLD data */


void load_sld_file(const char* sld_file_name);
fi_union read_a_sld_word(void);


#endif // IO_H
