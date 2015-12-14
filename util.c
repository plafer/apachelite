#include "util.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int str_ismemberof(const char *const *arr,
		   int size, char *target, int target_size)
{
  while (size-- > 0)
    {
      if (strncmp(*arr, target, target_size) == 0)
	{
	  return 1;
	}
      arr++;
    }

  return 0;
}

void str_tolower(char *str, int len)
{
  if (len <= 0)
    {
      len = strlen(str);
    }
  
  while (len-- > 0)
    {
      *str = tolower(*str);
      str++;
    }
}

int is_number(char *str)
{
  if (str == NULL || strlen(str) == 0)
    {
      return 0;
    }

  while (*str != '\0')
    {
      if (!isdigit(*str))
	{
	  return 0;
	}
      
      str++;
    }
  
  return 1;
}

int get_file_contents(char *file_name, char **file_contents)
{
  FILE *target_file;
  if ((target_file = fopen(file_name, "r")) == NULL)
    {
      return 1;
    }

  if (fseek(target_file, 0L, SEEK_END) == -1)
    {
      fclose(target_file);
      return 1;
    }

  long fsize = 0L;
  if ((fsize = ftell(target_file)) == -1)
    {
      fclose(target_file);
      return 1;
    }
  rewind(target_file);

  char *file_contents_temp;
  if ((file_contents_temp = malloc(fsize + 1)) == NULL)
    {
      fclose(target_file);
      return 1;
    }
  memset(file_contents_temp, '\0', fsize + 1);

  fread(file_contents_temp, 1, fsize, target_file);
  if (ferror(target_file))
    {
      free(file_contents_temp);
      fclose(target_file);
      return 1;
    }

  file_contents_temp[fsize] = '\0';

  *file_contents = file_contents_temp;
  fclose(target_file);
  return 0;
}

int pathcat(char *path1, char *path2, char *result_path)
{
  size_t path1_len = strlen(path1);
  strncpy(result_path, path1, path1_len);
  result_path += path1_len;

  if (path1[path1_len - 1] == '/' && path2[0] == '/')
    {
      result_path--;
    }
  else if (path1[path1_len - 1] != '/' && path2[0] != '/')
    {
      *result_path++ = '/';
    }
  
  size_t path2_len = strlen(path2);
  strncpy(result_path, path2, path2_len);
  
  result_path += path2_len;
  *result_path = '\0';
  return 0;
}

int ispath(char *path)
{
  if (path == NULL || strlen(path) == 0)
    {
      return 0;
    }
  
  while (*path != '\0')
    {
      if (*path == '/' && *(path + 1) == '/')
	{
	  return 0;
	}
      // Please make a table of allowed characters
      if (!isalpha(*path) && *path != '/' && *path != '?'
	  && *path != '.' && *path != '%' && *path != ':'
	  && *path != '|' && *path != '<' && *path != '>'
	  && *path != ' ' && *path != '!')
	{
	  return 0;
	}
      path++;
    }
  return 1;
}

char *get_ext(char *path)
{
  if (path == NULL || *path == '\0')
    {
      return "";
    }

  // path + 1 to avoid names like .git
  // the second strchr is to avoid paths like /foo.s/bleh
  char *dot;
  if ((dot = strrchr(path + 1, '.')) == NULL ||
      strchr(dot, '/') != NULL)
    {
      return "";
    }
  return dot + 1;
}

char *get_mimetype(char *ext)
{
  if (ext == NULL || *ext == '\0')
    {
      return "text/plain";
    }

  int ext_len = strlen(ext);
  char *local_ext = malloc(ext_len + 1);
  strncpy(local_ext, ext, ext_len);
  local_ext[ext_len] = '\0';
  str_tolower(local_ext, ext_len);

  char *mime = NULL;
  if (strncmp(local_ext, "html", ext_len) == 0)
    {
      mime = "text/html";
    }
  else if (strncmp(local_ext, "gif", ext_len) == 0)
    {
      mime = "image/gif";
    }
  else if (strncmp(local_ext, "jpg", ext_len) == 0)
    {
      mime = "image/jpeg";
    }
  else if (strncmp(local_ext, "png", ext_len) == 0)
    {
      mime = "image/png";
    }
  else
    {
      mime = "text/plain";
    }
  free(local_ext);
  return mime;
}
