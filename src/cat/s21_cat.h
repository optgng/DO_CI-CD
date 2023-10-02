#ifndef SRC_CAT_S21_CAT_H
#define SRC_CAT_S21_CAT_H

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define no_args 0
#define no_flag 0

typedef struct CatOpts {
  bool b;
  bool e;
  bool n;
  bool s;
  bool t;
  bool v;
} opts;

const struct option long_opts[] = {{"number-nonblank", no_args, no_flag, 'b'},
                                   {"number", no_args, no_flag, 'n'},
                                   {"squeeze-blank", no_args, no_flag, 's'},
                                   {NULL, 0, NULL, 0}};

void reading_flags(int argc, char **argv, opts *flags, const char *short_opts);
void reading_flags_switch(int *flag, opts *flags);
void print_data(opts *flags, FILE *reading_file);
bool existing_file(char *file_name);
void proc_b(char prev_symbol, int *cnt_all_line, char curr_symbol);
void proc_e(char curr_symbol);
void proc_n(int *cnt_all_line, char curr_symbol);
int proc_s(char prev_symbol, char curr_symbol, int *cnt_empty_line);
int proc_T(char *curr_symbol, char *prev_symbol);
void proc_v(char *curr_symbol);

#endif  // SRC_CAT_S21_CAT_H