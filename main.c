#include "base.h"
#include <stdio.h>

// used in error.c
Error *error_fatal;
Error *error_abort;
int errno;

int main()
{
   object_type_register();
   Base_register();

   Base *obj = Base_new();
   BASE_GET_CLASS(obj)->say(obj);

   return 0;
}
