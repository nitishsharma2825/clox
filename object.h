#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "chunk.h"
#include "value.h"
#include "table.h"

#define OBJ_TYPE(value)       (AS_OBJ(value)->type)

#define IS_STRING(value)      isObjType(value, OBJ_STRING)
#define AS_STRING(value)      ((ObjString*)AS_OBJ(value))
#define IS_NATIVE(value)      isObjType(value, OBJ_NATIVE)
#define AS_CSTRING(value)     (((ObjString*)AS_OBJ(value))->chars)
#define IS_CLOSURE(value)     isObjType(value, OBJ_CLOSURE)
#define AS_CLOSURE(value)     ((ObjClosure*)AS_OBJ(value))
#define IS_FUNCTION(value)    isObjType(value, OBJ_FUNCTION)
#define AS_FUNCTION(value)    ((ObjFunction*)AS_OBJ(value))
#define AS_NATIVE(value)      (((ObjNative*)AS_OBJ(value))->function)
#define IS_CLASS(value)       isObjType(value, OBJ_CLASS)
#define AS_CLASS(value)       ((ObjClass*)AS_OBJ(value))
#define IS_INSTANCE(value)    isObjType(value, OBJ_INSTANCE)
#define AS_INSTANCE(value)    ((ObjInstance*)AS_OBJ(value))

typedef enum
{
    OBJ_STRING,
    OBJ_NATIVE,
    OBJ_FUNCTION,
    OBJ_CLOSURE,
    OBJ_UPVALUE,
    OBJ_CLASS,
    OBJ_INSTANCE
} ObjType;

// state common across all object types
struct Obj
{
    ObjType type;
    bool isMarked; // for garbage collection
    struct Obj* next; // for linked list of all objects, intrusive list
};

typedef struct
{
    Obj obj;
    int arity;
    int upvalueCount;
    Chunk chunk;
    ObjString* name;
} ObjFunction;

typedef Value (*NativeFn)(int argCount, Value* args);

typedef struct
{
    Obj obj;
    NativeFn function;
} ObjNative;

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

typedef struct ObjUpvalue
{
    Obj obj;
    Value* location;
    Value closed; // when closed, the value is moved here from the stack
    struct ObjUpvalue* next; // for linked list of open upvalues
} ObjUpvalue;

typedef struct
{
    Obj obj;
    ObjFunction* function;
    ObjUpvalue** upvalues;
    int upvalueCount;
} ObjClosure;

typedef struct
{
    Obj obj;
    ObjString* name;
} ObjClass;

typedef struct
{
    Obj obj;
    ObjClass* klass;
    Table fields;
} ObjInstance;

ObjClosure* newClosure(ObjFunction* function);
ObjFunction* newFunction();
ObjClass* newClass(ObjString* name);
ObjInstance* newInstance(ObjClass* klass);

ObjNative* newNative(NativeFn function);

ObjString* takeString(char* chars, int length);
ObjString* copyString(const char* chars, int length);
ObjUpvalue* newUpvalue(Value* slot);
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