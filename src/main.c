#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void print_usage(char *argv[]) {
  printf("Usage: %s -n -f <database file>\n", argv[0]);
  printf("\t-n - crate database file\n");
  printf("\t-f - (required) path to database file\n");
  return;
}

int main(int argc, char *argv[]) {
  char *filepath = NULL;
  bool newfile = false;
  int c;
  int dbfd = -1;
  struct dbheader_t *dbhdr_t = NULL;

  while ((c = getopt(argc, argv, "nf:")) != -1) {
    switch (c) {
      case 'n':
        newfile = true;
        break;
      case 'f':
        filepath = optarg;
        break;
      case '?':
        print_usage(argv);
        break;
      default:
        return -1;
    }
  }

  if (filepath == NULL) {
    printf("Filepath is a required argument.\n");
    print_usage(argv);
    return 0;
  }

  if (newfile) {
    dbfd = create_db_file(filepath);
    if (dbfd == STATUS_ERROR) {
      printf("Unable to create database file\n");
      return -1;
    }

    if (create_db_header(dbfd, &dbhdr_t) == STATUS_ERROR) {
      printf("Failed to create db header\n");
      return -1;
    }
  } else {
    dbfd = open_db_file(filepath);

    if (dbfd == -1) {
      printf("Unable to open database file\n");
      return -1;
    }

    if (validate_db_header(dbfd, &dbhdr_t) == STATUS_ERROR) {
      printf("Failed to validate db file\n");
      return -1;
    }

  }

  output_file(dbfd, dbhdr_t);

  return 0;
}
