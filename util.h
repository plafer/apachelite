#ifndef UTIL_H
#define UTIL_H

/*
 * Searches arr to find if it can find target.
 * Returns 1 on success, 0 otherwise.
 */
int str_ismemberof(const char *const *arr,
		   int size, char *target, int target_size);

/*
 * Lowers string. Optional length parameter;
 * 0 to lower whole string (i.e. until '\0').
 */
void str_tolower(char *str, int len);

int is_number(char *str);

#endif
