# What is the qom
The qom is the object model of [qemu](https://github.com/qemu/qemu)(a.k.a. quick emulator). With the qom, we can 
use **c** language to write code in **OOP(object-oriented programming)** way.

# Usage
- download the repository
```bash
git clone https://github.com/Gyumeijie/qemu-object-model.git
```
- emit the **make** command
```bash
make
```
- run the code
```bash
./main
```

# Guide
In qom the root object is object, defined in [object.h](https://github.com/Gyumeijie/qemu-object-model/blob/master/qom/object.h).
all the newly-created object must inherit some object.And in qom a class is represented by two structs, one struct is called
**instance struct**, the other is called **class struct**.
For example, given a class Base, who has a data memeber named ***greeting***, and a ***say*** member method,then
the two structs are the following:
```c
typedef struct Base {
  Object parent;

  char *greeting;
} Base;


typedef struct BaseClass {
   ObjectClass parent_class;

   void (*say)(void*); 
} BaseClass;
```

And we also need define the following three macro.
```c
#define BASE_GET_CLASS(obj) \
        OBJECT_GET_CLASS(BaseClass, obj, TYPE_BASE)

#define BASE_CLASS(klass) \
        OBJECT_CLASS_CHECK(BaseClass, klass, TYPE_BASE)

#define BASE(obj) \
        OBJECT_CHECK(Base, obj, TYPE_BASE)
```

In OO language like c++, we can use **new** keyword to create a instance, similarly we use class-prefix new to instantiate
a class, say we can use ```Base_new()```to create an instance of tpye Base.

We can use the marocs above the acess members, supposed that we already have an instance obj of type Base, we can call the method
by using the following code:
```c
BASE_GET_CLASS(obj)->say(obj);
```
In qom, the creation of an instance can be finished by two steps:
- allocate memery 
- initialize the data member
qom supply us with an **instance init hook function**, which acts as  **constructor** in OO language. The following
shows the instance init hook in this example:
```c
static void instance_init(Object *obj)
{
    Base *This = BASE(obj);
    This->greeting = "I am base";
}
```
Similarly, the class struct also need an init hook:
```c
static void class_init(ObjectClass *oc, void *data)
{
    BaseClass *base = BASE_CLASS(oc);
    base->say = say;
}
```

The last thing to do is to fill the **TypeInfo** struct, which descripts some basic infomation of the type:
```c
static const TypeInfo type_info = {
    .name = TYPE_BASE,
    .parent = TYPE_OBJECT,
    .instance_size = sizeof(Base),
    .abstract = false,
    .class_size = sizeof(BaseClass),
    .instance_init = instance_init,
    .class_init = class_init,
};

```
and then register it by calling ```type_register_static(&type_info)```.

 # Resources
- There is a project named [OBS-Framework](https://github.com/Gyumeijie/OBS-Framework) athoured by me, heavily 
using qom model, you can visit it for more information.

- In some way, the qom is an simiplified version of [Gobject](https://developer.gnome.org/gobject/stable/), which
is also a type of object model and is the object system of Glib.
