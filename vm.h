#ifndef clox_vm_h
#define clox_vm_h

#include "object.h"
#include "table.h"
#include "value.h"

#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)

// A call frame represents a single ongoing function call
typedef struct 
{
    ObjClosure* closure;
    uint8_t* ip;    // return address after function end
    Value* slots; // points into the VM's value stack at the first slot that this function can use
} CallFrame;

// think about alignment when declaring fields, not thought here for simplicity
typedef struct
{
    CallFrame frames[FRAMES_MAX];
    int frameCount;
    Value stack[STACK_MAX];
    Value* stackTop;
    Table globals;
    Table strings; // string interning, used as set to keep only unique strings
    ObjUpvalue* openUpvalues; // linked list of all open upvalues
    Obj* objects; // pointer to head of list of objects to free

    int grayCount;
    int grayCapacity;
    Obj** grayStack;

    size_t bytesAllocated;
    size_t nextGC;
} VM;

typedef enum
{
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

extern VM vm;

void initVM();
void freeVM();
InterpretResult interpret(const char* source);
void push(Value value);
Value pop();

#endif