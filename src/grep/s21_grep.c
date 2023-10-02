#include "s21_grep.h"

int main(int argc, char **argv) {
  opts flags = {false};
  data info = {0};
  const char *short_opts = "e:ivclnhsf:o";
  if (argc > 2) {
    if (reading_flags(argc, argv, &flags, &info, short_opts) == false) {
      if (info.various_e == 0) {
        sprintf(info.pattern, "%s", argv[optind]);
        optind++;
      }
      if (optind + 1 < argc) info.count_files = 1;
      read_files(argc, argv, &flags, &info);
    }
  }
  return 0;
}

bool reading_flags(int argc, char **argv, opts *flags, data *info,
                   const char *short_opts) {
  int flag = 0;
  bool flag_error = false;
  while ((flag = (getopt_long(argc, argv, short_opts, NULL, NULL))) != -1)
    reading_flags_switch(argv, &flag, flags, info, &flag_error);
  return flag_error;
}

void reading_flags_switch(char **argv, int *flag, opts *flags, data *info,
                          bool *flag_error) {
  switch (*flag) {
    case 'e':
      flags->e = true;
      reading_flags_switch_case_e(info);
      break;
    case 'i':
      flags->i = true;
      break;
    case 'v':
      flags->v = true;
      break;
    case 'c':
      flags->c = true;
      break;
    case 'l':
      flags->l = true;
      break;
    case 'n':
      flags->n = true;
      break;
    case 'h':
      flags->h = true;
      break;
    case 's':
      flags->s = true;
      break;
    case 'f':
      flags->f = true;
      reading_flags_switch_case_f(argv, info);
      break;
    case 'o':
      flags->o = true;
      break;
    default:
      *flag_error = true;
      break;
  }
}

void reading_flags_switch_case_e(data *info) {
  if (info->various_e > 0) {
    strcat(info->pattern, "|");
  }
  strcat(info->pattern, optarg);
  info->various_e++;
}

void reading_flags_switch_case_f(char **argv, data *info) {
  char *file_name = argv[optind - 1];
  if (existing_file(file_name)) {
    char reading_symbol = 0;
    unsigned int len_pattern = 0, cnt_empty_lines = 0;
    FILE *reading_file = NULL;
    reading_file = fopen(file_name, "r");
    if (info->various_e > 0) strcat(info->pattern, "|");
    while ((reading_symbol = fgetc(reading_file)) != EOF) {
      len_pattern = strlen(info->pattern);
      if ((reading_symbol == '\0') || (reading_symbol == '\n')) {
        if (cnt_empty_lines == 0) {
          strcat(info->pattern, "|");
          cnt_empty_lines = 1;
        } else {
          strcat(info->pattern, ".");
        }
      } else {
        info->pattern[len_pattern] = reading_symbol;
      }
    }
    info->various_e++;
    fclose(reading_file);
  } else {
    fprintf(stderr, "grep: %s: No such file or directory\n", argv[optind]);
  }
}

void read_files(int argc, char **argv, opts *flags, data *info) {
  while (optind < argc) {
    char *file_name = argv[optind];
    if (existing_file(file_name)) {
      FILE *reading_file = fopen(file_name, "r");
      procc_data(flags, info, reading_file, argv, file_name);
      fclose(reading_file);
    } else {
      fprintf(stderr, "grep: %s: No such file or directory\n", argv[optind]);
    }
    optind++;
  }
}

bool existing_file(char *file_name) {
  bool exist = true;
  FILE *check_file = fopen(file_name, "r");
  if (check_file == NULL) {
    exist = false;
  }
  fclose(check_file);
  return exist;
}

void procc_data(opts *flags, data *info, FILE *reading_file, char **argv,
                char *file_name) {
  regex_t regex;
  size_t len;
  char *temp_line = NULL;
  int cflags = REG_EXTENDED, ordinate = 0, count_match = 0;
  if (reading_file) {
    if (flags->i) cflags = REG_EXTENDED | REG_ICASE;
    if (regcomp(&regex, info->pattern, cflags) == 0) {
      while (getline(&temp_line, &len, reading_file) != -1) {
        ordinate++;
        if (flags->o && flags->v == false && flags->c == false &&
            flags->l == false)
          proc_o(argv, temp_line, flags, info, ordinate, regex);  // 0
        else {
          if (temp_line) {
            int res_regexec = regexec(&regex, temp_line, 0, NULL, 0);
            if ((res_regexec == 0 && flags->v == false) ||
                (res_regexec == 1 && flags->v == true)) {
              if (flags->c == false) {
                if (info->count_files > 1) {
                  printf("%s:", file_name);
                }
                if (flags->l && proc_l(argv)) break;
                if (flags->h) proc_h(argv, info);
                if (flags->n) proc_n(&ordinate);
                int iterator = 0;

                proc_v(&iterator, temp_line);
                printf("\n");
              }
              count_match++;
            }
          }
        }
      }
      free(temp_line);
    }
    regfree(&regex);
  } else {
    if (flags->s == false)
      fprintf(stderr, "grep: %s: No such file or directory\n", argv[optind]);
  }
  if (flags->c) proc_c(argv, count_match, flags, info);
}

bool proc_flags_for_o(opts *flags) {
  bool result = false;
  if (flags->v == false && flags->c == false && flags->l == false)
    result = true;
  return result;
}

bool proc_l(char **argv) {
  printf("%s\n", argv[optind]);
  return true;
}

bool proc_h(char **argv, data *info) {
  bool result = false;
  if (info->count_files == 1) {
    printf("%s:", argv[optind]);
    result = true;
  }
  return result;
}

bool proc_n(int *ordinate) {
  printf("%d:", *ordinate);
  return true;
}

bool proc_v(int *iterator, char *temp_line) {
  bool result = false;

  while (temp_line[*iterator]) {
    if (temp_line[*iterator] != '\n') printf("%c", temp_line[*iterator]);
    *iterator += 1;
    result = true;
  }
  return result;
}

void proc_c(char **argv, int count_match, opts *flags, data *info) {
  if (count_match == 0) {
    if (info->count_files) {
      if (flags->h)
        printf("%d\n", count_match);
      else
        printf("%s:%d\n", argv[optind], count_match);
    } else {
      if (flags->l == true) printf("%d\n", count_match);
    }
  } else if (count_match > 0) {
    if (info->count_files) {
      if (flags->l) {
        if (flags->h)
          printf("1\n%s\n", argv[optind]);
        else
          printf("%s:1\n%s\n", argv[optind], argv[optind]);
      } else if (flags->h)
        printf("%d\n", count_match);
      else
        printf("%s:%d\n", argv[optind], count_match);
    } else {
      if (flags->l == true)
        printf("1\n%s\n", argv[optind]);
      else if (flags->l == false)
        printf("%d\n", count_match);
    }
  }
}

void proc_o(char **argv, char *temp_line, opts *flags, data *info, int ordinate,
            regex_t regex) {
  regmatch_t pmatch[1];
  int point = 0;
  while (regexec(&regex, temp_line, 1, pmatch, 0) != REG_NOMATCH) {
    if (point == 0) {
      if (info->count_files == 1) {
        if ((flags->h == 0) && (flags->n == 0)) printf("%s:", argv[optind]);
        if ((flags->h == 0) && (flags->n == 1)) printf("%s:", argv[optind]);
        if ((flags->h == 1) && (flags->n == 1)) printf("%d:", ordinate);
        if ((flags->h == 0) && (flags->n == 1)) printf("%d:", ordinate);
      } else {
        if (flags->n == 1) printf("%d:", ordinate);
      }
    }
    for (int i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++) {
      printf("%c", temp_line[i]);
      point++;
    }
    printf("\n");
    temp_line = temp_line + pmatch[0].rm_eo;
  }
}