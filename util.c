#include "util.h"
#include <ctype.h>
#include <string.h>

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
