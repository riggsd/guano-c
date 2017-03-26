#ifndef FILE_GUANO_H
#define FILE_GUANO_H

struct guanofield {
  char* ns;
  char* key;
  char* val;
};

typedef struct guanofield GuanoField;


/**
 * Initialize a 16-bit mono .WAV file.
 */
int initialize_wav(const char* filename, unsigned int samplerate);

int write_guano(const char field[], const char value[]);

int write_guano_int(const char field[], const int value);

int write_guano_float(char field[], double value, int precision);

int write_guano_bool(char field[], int value);

int write_wav_data(int nframes, int data[]);

int finalize_wav();

#endif
