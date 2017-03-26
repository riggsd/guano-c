#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "guano.h"


#define RIFF_ID       "RIFF"
#define WAVE_RIFF_ID  "WAVE"
#define FMT_CHUNK_ID  "fmt "
#define DATA_CHUNK_ID "data"
#define GUAN_CHUNK_ID "guan"
#define GUAN_CHUNK_OFFSET 0x24

static FILE* file;
static unsigned int guano_bytes;
static unsigned int data_bytes;


static int write_le(unsigned int word, int nbytes) {
  extern FILE* file;
  unsigned int buf;
  int nwritten = 0;
  while (nbytes > 0) {
    buf = word & 0xff;
    nwritten += fwrite(&buf, 1, 1, file);
    nbytes--;
    word >>= 8;
  }
  return nwritten;
}


/**
 * Initialize a 16-bit mono .WAV file.
 */
int initialize_wav(const char* filename, unsigned int samplerate) {
  extern FILE* file;
  extern unsigned int guano_bytes;
  extern unsigned int data_bytes;
  unsigned int nchannels = 1;
  unsigned int bytes_per_sample = 2;  /* 16 bit */
  unsigned int byte_rate = samplerate * nchannels * bytes_per_sample;

  file = fopen(filename, "w");
  assert(file != NULL);

  guano_bytes = 0;  /* GUANO subchunk size counter */
  data_bytes = 0;   /* data subchunk size counter */

  /* RIFF header */
  fwrite("RIFF", 1, 4, file);
  write_le(0, 4);  /* RIFF container size, fill this in upon `close()` */
  fwrite("WAVE", 1, 4, file);

  /* Format subchunk */
  fwrite("fmt ", 1, 4, file);
  write_le(2+2+4+4+2+2, 4);  /* fmt subchunk size */
  write_le(1, 2);            /* PCM format */
  write_le(nchannels, 2);
  write_le(samplerate, 4);
  write_le(byte_rate, 4);
  write_le(nchannels * bytes_per_sample, 2);  /* block align */
  write_le(8 * bytes_per_sample, 2);          /* bits per sample */

  /* GUANO subchunk */
  fwrite(GUAN_CHUNK_ID, 1, 4, file);
  write_le(0, 4);   /* GUANO subchunk size, fill this in upon `close()`, address GUAN_CHUNK_OFFSET+4 */
  write_guano("GUANO|Version", "1.0 ");

  return 0;
}

int write_guano(const char* field, const char* value) {
  extern FILE* file;
  extern unsigned int guano_bytes;
  int nbytes;
  nbytes = fprintf(file, "%s: %s\n", field, value);
  guano_bytes += nbytes;
  return nbytes;
}

int write_guano_int(const char* field, int value) {
  return 0;  /* TODO */
}

int write_guano_float(char field[], double value, int precision) {
  return 0;  /* TODO */
}

int write_guano_bool(char field[], int value) {
  return 0;  /* TODO */
}

int write_wav_data(int nframes, int data[]) {
  return 0;  /* TODO */
}

int finalize_wav(void) {
  extern FILE* file;
  extern unsigned int guano_bytes;
  extern unsigned int data_bytes;
  
  if (data_bytes == 0) {
    if (guano_bytes % 2) {
      guano_bytes += fwrite('\0', 1, 1, file);
    }
    fwrite(DATA_CHUNK_ID, 1, 4, file);
    write_le(0, 4);   /* empty subchunk - .WAV is invalid without 'data'  */
  }

  fseek(file, GUAN_CHUNK_OFFSET+4, SEEK_SET);
  write_le(guano_bytes, 4);

  fclose(file);
  return 0;
}
