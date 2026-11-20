// Jarrye Is not Unix (JIX) kernel trait
// for testbench or implementators
#ifndef JixKernelStyleTrait
#define JixKernelStyleTrait

#define PropertysNameFixed 16

typedef struct _metatable_t METATABLE;
typedef struct _metaentry_t METAENTRY;
typedef char METANAMER;
typedef enum {
    META_POINTER,
    META_UNSIGNED,
    META_SIGNED
} METAPROP; // Metatable typeof
typedef union {
    void*        pointer;
    unsigned int numberUnsigned;
    int          numberSigned;
} METATHING; // Metatable property content

void*   allocateMemory(unsigned int size);
void    freeMemory(void* buff);

struct _metaentry_t {
    METANAMER entryName[PropertysNameFixed];
    METAPROP typeofValue;
    METATHING anyThing;
}; // Metatable entry
struct _metatable_t {
    unsigned int length;
    METAENTRY* entrys;
}; // Metatable

void SetProperty(
    METATABLE* table, 
    const METANAMER* entryName,
    METAPROP typeofValue,
    METATHING value
); // The property setter
METAENTRY* GetProperty(
    METATABLE* table,
    const METANAMER* entryName
); // Entry name
METATABLE* JixTable(
); // Jix table maker

#endif