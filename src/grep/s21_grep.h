#ifndef SRC_GREP_S21_GREP_H
#define SRC_GREP_S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 8192
typedef struct GrepOpts {
  bool e;
  bool i;
  bool v;
  bool c;
  bool l;
  bool n;
  bool h;
  bool s;
  bool f;
  bool o;
} opts;

typedef struct GrepData {
  char pattern[BUF_SIZE];
  int count_files;
  int various_e;
} data;

bool reading_flags(int argc, char **argv, opts *flags, data *info,
                   const char *short_opts);
void reading_flags_switch(char **argv, int *flag, opts *flags, data *info,
                          bool *flag_error);
void reading_flags_switch_case_e(data *info);
void reading_flags_switch_case_f(char **argv, data *info);
void read_files(int argc, char **argv, opts *flags, data *info);
void procc_data(opts *flags, data *info, FILE *reading_file, char **argv,
                char *file_name);
bool existing_file(char *file_name);
bool proc_flags_for_o(opts *flags);
bool proc_l(char **argv);
bool proc_h(char **argv, data *info);
bool proc_n(int *ordinate);
bool proc_v(int *iterator, char *temp_line);
void proc_c(char **argv, int count_match, opts *flags, data *info);
void proc_o(char **argv, char *temp_line, opts *flags, data *info, int ordinate,
            regex_t regex);
#endif  // SRC_GREP_S21_GREP_H