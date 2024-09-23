#ifndef SPEC_9
#define SPEC_9

#include <stdbool.h>

bool str_contains(char * line, char *target);
int is_alias_or_function(char *arg);
void get_alias(char *arg, char *dest);
void get_function(char *arg, char *dest);

#endif