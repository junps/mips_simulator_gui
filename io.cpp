#include "io.h"

#include <QDebug>

#define MAX_N_WORDS 4096
extern fi_union sld_words[];
extern unsigned sld_n_words;
extern unsigned sld_index;

/*****************************************************************************
 * SLD Reader : convert the input SLD text file into a binary format
 ****************************************************************************/

/*-----------------------------------------------------------------------------
 * read a float in the SLD file and append it to the array sld_words.
 * fp : input SLD file stream
 * RETURN value : the float read from the file
 */
static float read_float(FILE* fp)
{
  float f;

  if(sld_n_words >= MAX_N_WORDS){
    perror("read_float : too many sld words.\n");
    exit(1);
  }

  if(fscanf(fp, "%f", &f) != 1){
    perror("failed to read a float\n");
    exit(1);
  }

  return (sld_words[sld_n_words++].f = f);
}

/*-----------------------------------------------------------------------------
 * read an integer in the SLD file and append it to the array sld_words.
 * fp : input SLD file stream
 * RETURN value : the integer read from the file
 */
static int read_int(FILE* fp)
{
  int i;
  if(sld_n_words >= MAX_N_WORDS){
    perror("read_int : too many sld words.\n");
    exit(1);
  }

  if(fscanf(fp, "%d", &i) != 1){
    perror("failed to read an int\n");
    exit(1);
  }

  return (sld_words[sld_n_words++].i = i);
}

/*-----------------------------------------------------------------------------
 * read a 3D float vector and append it to the array sld_words.
 * fp : input SLD file stream
 */
static void read_vec3(FILE* fp)
{
  read_float(fp);
  read_float(fp);
  read_float(fp);
}

/*-----------------------------------------------------------------------------
 * read the scene environments
 * fp : input SLD file stream
 */
static void read_sld_env(FILE* fp)
{
  /* screen pos */
  read_vec3(fp);
  /* screen rotation */
  read_float(fp);  read_float(fp);
  /* n_lights : Actually, it should be an int value ! */
  read_float(fp);
  /* light rotation */
  read_float(fp);  read_float(fp);
  /* beam  */
  read_float(fp);
}

/*-----------------------------------------------------------------------------
 * read all the objects
 * fp : input SLD file stream
 */
static void read_objects(FILE* fp)
{

  while (read_int(fp) != -1) {  /* texture : -1 -> end */
    /* form */
    read_int(fp);
    /* refltype */
    read_int(fp);
    /* isrot_p*/
    int is_rot = read_int(fp);
    /* abc */
    read_vec3(fp);
    /* xyz */
    read_vec3(fp);
    /* is_invert */
    read_float(fp);
    /* refl_param */
    read_float(fp); read_float(fp);
    /* color */
    read_vec3(fp);
    /* rot */
    if(is_rot){
      read_vec3(fp);
    }
  }
}

/*-----------------------------------------------------------------------------
 * read the AND-network
 * fp : input SLD file stream
 */
static void read_and_network(FILE* fp)
{
  while(read_int(fp) != -1){
    while(read_int(fp) != -1);
  }
}

/*-----------------------------------------------------------------------------
 * read the OR-network
 * fp : input SLD file stream
 */
static void read_or_network(FILE* fp)
{
  while(read_int(fp) != -1){
    while(read_int(fp) != -1);
  }
}

/*-----------------------------------------------------------------------------
 * read the SLD file
 * fp : input SLD file stream
 */
static void read_sld(FILE* fp)
{
  read_sld_env(fp);
  read_objects(fp);
  read_and_network(fp);
  read_or_network(fp);
}

/*****************************************************************************
 * Each step of the server
 *****************************************************************************/

/*-----------------------------------------------------------------------------
 * read the SLD file and convert into a binary format.
 * sld_file_name : name of the SLD data file
 */
void load_sld_file(const char* sld_file_name)
{
  FILE* fp = sld_file_name ? fopen(sld_file_name, "rb") : stdin;
  if(fp == NULL) {
    perror("cannot open SLD file");
    exit(1);
  }
  read_sld(fp);

  fclose(fp);
}

fi_union read_a_sld_word() {
    sld_index += 1;
    return sld_words[sld_index-1];
}

fi_union read_a_sld_byte() {
    fi_union ret;
    switch(sld_index % 4) {
        case 0:
            ret.i = sld_words[sld_index / 4].i & 0xff;
            break;
        case 1:
            ret.i = (sld_words[sld_index / 4].i >> 8) & 0xff;
            break;
        case 2:
            ret.i = (sld_words[sld_index / 4].i >> 16) & 0xff;
            break;
        case 3:
            ret.i = (sld_words[sld_index / 4].i >> 24) & 0xff;
            break;
    }
    sld_index++;
    return ret;
}
