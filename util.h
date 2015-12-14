#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

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
 * file_contents. Sets fsize to the size of the file.
 * All files will be null-terminated; however, fsize doesn't take
 * the null-terminating character into account. This way, image files
 * for instance simply ignore the null, and strlen will still
 * work on contents of text files.
 * Returns non-zero on error.
 */
int get_file_contents(char *file_name, char **file_contents,
		      size_t *fsize);

int is_number(char *str);

/*
 * Concatenates path1 with path2, and places the result
 * in result_path. result_path's memory must be allocated,
 * and be the sum of both path sizes (including null terminating char).
 * It is assumed path1 and path2 are valid paths, which can
 * be checked with ispath(1).
 * Returns non-zero on error.
 */
int pathcat(char *path1, char *path2, char *result_path);

/*
 * Returns true if path follows valid path syntax. False otherwise.
 */
int ispath(char *path);

/*
 * Returns a pointer of the extension within path, or empty string
 * if path has no extension.
 */
char *get_ext(char *path);

/*
 * Returns a string literal of the mime type describing
 * the extension ext. Returns text/plain for unsupported extensions.
 */
char *get_mimetype(char *ext);

#endif
