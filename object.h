#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "chunk.h"
#include "value.h"

#define OBJ_TYPE(value)       (AS_OBJ(value)->type)

#define IS_STRING(value)      isObjType(value, OBJ_STRING)
#define AS_STRING(value)      ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)     (((ObjString*)AS_OBJ(value))->chars)
#define IS_FUNCTION(value)    isObjType(value, OBJ_FUNCTION)
#define AS_FUNCTION(value)    ((ObjFunction*)AS_OBJ(value))

typedef enum
{
    OBJ_STRING,
    OBJ_FUNCTION,
} ObjType;

// state common across all object types
struct Obj
{
    ObjType type;
    struct Obj* next; // for linked list of all objects, intrusive list
};

typedef struct
{
    Obj obj;
    int arity;
    Chunk chunk;
    ObjString* name;
} ObjFunction;

// struct inheriting from Obj
// pointer to a struct is actually pointer to its 1st field
// this allows to safely cast between Obj* and ObjString*
struct ObjString
{
    Obj obj;
    int length;
    char* chars;
    uint32_t hash;
};

ObjFunction* newFunction();

ObjString* takeString(char* chars, int length);
ObjString* copyString(const char* chars, int length);
void printObject(Value value);

static inline bool isObjType(Value value, ObjType type)
{
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif

// Note for ObjString:
// Value is in stack, which contains the Obj* pointer
// ObjString is in heap
// character of strings are also in heap, but its pointer is in ObjString which owns it
// 2 levels of indirection, 2 structures in heap + 1 in stack