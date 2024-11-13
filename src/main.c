#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>

// #include "common.h"
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

  return 0;
}
