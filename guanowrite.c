#include <stdio.h>

#include "guano.h"


int main(int argc, char* argv[]) {
  char* fname = "test.wav";
  printf("Writing file %s ...\n", fname);
  initialize_wav(fname, 250000);

  write_guano("Make", "Myotisoft");
  write_guano("Model", "guanowrite");
  write_guano_int("TE", 10);
  write_guano_float("Length", 4.2, 2);

  write_wav_data(4, (int[]){0,0,0,0});  /* fake */

  finalize_wav();
  printf("Done.\n");
  return 0;
}
