#include "util.h"
#include <ctype.h>
#include <string.h>

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
