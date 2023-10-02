#include "s21_cat.h"

int main(int argc, char **argv) {
  opts flags = {false};
  const char *short_opts = "+benstvTE";
  reading_flags(argc, argv, &flags, short_opts);
  for (int i = optind; i < argc; i++) {
    char *file_name = argv[i];
    if (existing_file(file_name)) {
      FILE *reading_file = fopen(argv[i], "r");
      print_data(&flags, reading_file);
      fclose(reading_file);
    } else {
      perror(argv[i]);
    }
  }
  return 0;
}

void reading_flags(int argc, char **argv, opts *flags, const char *short_opts) {
  int flag = 0;
  int opt_index = 0;
  while ((flag = (getopt_long(argc, argv, short_opts, long_opts,
                              &opt_index))) != -1)
    reading_flags_switch(&flag, flags);
  if (flags->b) flags->n = false;
}

void reading_flags_switch(int *flag, opts *flags) {
  switch (*flag) {
    case 'b':
      flags->b = true;
      break;
    case 'e':
      flags->e = true;
      flags->v = true;
      break;
    case 'n':
      flags->n = true;
      break;
    case 's':
      flags->s = true;
      break;
    case 't':
      flags->t = true;
      flags->v = true;
      break;
    case 'v':
      flags->v = true;
      break;
    case 'T':
      flags->t = true;
      break;
    case 'E':
      flags->e = true;
      break;
    default:
      printf("usage: s21_cat [-benstvTE] [file ...]\n");
      exit(EXIT_FAILURE);
      break;
  };
}

void print_data(opts *flags, FILE *reading_file) {
  char prev_symbol = '\n', curr_symbol = 0;
  int cnt_empty_line = 0, cnt_all_line = 0;
  while ((curr_symbol = getc(reading_file)) != EOF) {
    if (flags->s && proc_s(prev_symbol, curr_symbol, &cnt_empty_line) == 1 &&
        cnt_empty_line > 1)
      continue;
    if (flags->b) proc_b(prev_symbol, &cnt_all_line, curr_symbol);
    if (flags->n) proc_n(&cnt_all_line, prev_symbol);
    if (flags->e) proc_e(curr_symbol);
    if (flags->t && proc_T(&curr_symbol, &prev_symbol) == 1) continue;
    if (flags->v) proc_v(&curr_symbol);

    printf("%c", curr_symbol);
    prev_symbol = curr_symbol;
  }
}

bool existing_file(char *file_name) {
  bool exist = true;
  FILE *check_file = NULL;
  if ((check_file = fopen(file_name, "r")) == NULL) {
    exist = false;
  } else {
    fclose(check_file);
  }
  return exist;
}

void proc_b(char prev_symbol, int *cnt_all_line, char curr_symbol) {
  if (prev_symbol == '\n' && curr_symbol != '\n') {
    *cnt_all_line += 1;
    printf("%6d\t", *cnt_all_line);
  }
}

void proc_e(char curr_symbol) {
  if (curr_symbol == '\n') printf("$");
}

void proc_n(int *cnt_all_line, char prev_symbol) {
  if (prev_symbol == '\n') {
    *cnt_all_line += 1;
    printf("%6d\t", *cnt_all_line);
  }
}

int proc_s(char prev_symbol, char curr_symbol, int *cnt_empty_line) {
  int result = 0;
  if ((curr_symbol == '\n') && (prev_symbol == '\n')) {
    *cnt_empty_line += 1;
    result = 1;
  } else {
    *cnt_empty_line = 0;
  }
  return result;
}

int proc_T(char *curr_symbol, char *prev_symbol) {
  int result = 0;
  if (*curr_symbol == '\t') {
    printf("^I");
    *prev_symbol = *curr_symbol;

    result = 1;
  }
  return result;
}

void proc_v(char *curr_symbol) {
  if (((*curr_symbol <= 8)) || ((*curr_symbol >= 11) && (*curr_symbol <= 31))) {
    *curr_symbol += 64;
    printf("^");
  } else if (*curr_symbol >= 127) {
    printf("^");
    *curr_symbol = '?';
  }
}
