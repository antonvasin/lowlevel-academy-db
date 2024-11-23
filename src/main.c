#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void print_usage(char *argv[]) {
  printf("Usage: %s -n -f <database file>\n", argv[0]);
  printf("\t-n - create database file\n");
  printf("\t-f - (required) path to database file\n");
  printf("\t-a - add record to database\n");
  printf("\t-l - list records\n");
  printf("\t-r - remove record by employee name\n");
  printf("\t-u - (name,hours) update employee's hours\n");
  return;
}

int main(int argc, char *argv[]) {
  char *filepath = NULL;
  char *addstring = NULL;
  char *removestring = NULL;
  char *updatestring = NULL;
  bool newfile = false;
  bool list = false;
  int c;
  int dbfd = -1;
  struct dbheader_t *dbhdr = NULL;
  struct employee_t *employees = NULL;

  while ((c = getopt(argc, argv, "nf:a:lr:u:")) != -1) {
    switch (c) {
      case 'n':
        newfile = true;
        break;
      case 'f':
        filepath = optarg;
        break;
      case 'a':
        addstring = optarg;
        break;
      case 'l':
        list = true;
        break;
      case 'r':
        removestring = optarg;
        break;
      case 'u':
        updatestring = optarg;
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

    if (create_db_header(dbfd, &dbhdr) == STATUS_ERROR) {
      printf("Failed to create db header\n");
      return -1;
    }
  } else {
    dbfd = open_db_file(filepath);

    if (dbfd == -1) {
      printf("Unable to open database file\n");
      return -1;
    }

    if (validate_db_header(dbfd, &dbhdr) == STATUS_ERROR) {
      printf("Failed to validate db file\n");
      return -1;
    }

  }

  if (read_employees(dbfd, dbhdr, &employees) != STATUS_SUCCESS) {
    printf("Failed to read employees\n");
    return 0;
  }

  if (addstring) {
    dbhdr->count++;
    employees = realloc(employees, dbhdr->count*(sizeof(struct employee_t)));
    add_employee(dbhdr, employees, addstring);
  }

  if (updatestring) {
    update_employee(dbhdr, employees, updatestring);
  }

  if (removestring) {
    remove_employee(dbhdr, employees, removestring);
  }

  if (list) {
    list_employees(dbhdr, employees);
  }

  output_file(dbfd, dbhdr, employees);

  return 0;
}
