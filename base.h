#ifndef BASE_H
#define BASE_H

#include "./qom/object.h"


#define TYPE_BASE "base"

void Base_register(void);

typedef struct Base {
  Object parent;

  char *greeting;
} Base;

typedef struct BaseClass {
   ObjectClass parent_class;

   void (*say)(void*); 
} BaseClass;

Base* Base_new(void);

#define BASE_GET_CLASS(obj) \
        OBJECT_GET_CLASS(BaseClass, obj, TYPE_BASE)

#define BASE_CLASS(klass) \
        OBJECT_CLASS_CHECK(BaseClass, klass, TYPE_BASE)

#define BASE(obj) \
        OBJECT_CHECK(Base, obj, TYPE_BASE)

#endif
