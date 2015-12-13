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

/*
 * Reads file with file_name, allocates appropriate memory and sets
 * file_contents. 
 * Returns non-zero on error.
 */
int get_file_contents(char *file_name, char **file_contents);

int is_number(char *str);

/*
 * Concatenates path1 with path2, and places the result
 * in result_path. result_path's memory must be allocated,
 * and is the sum of both paths sizes (including null terminating char).
 * It is assumed path1 and path2 are valid paths, which can
 * be checked with ispath(1).
 * Returns non-zero on error.
 */
int pathcat(char *path1, char *path2, char *result_path);

/*
 * Returns true if path follows valid path syntax. False otherwise.
 */
int ispath(char *path);

#endif
