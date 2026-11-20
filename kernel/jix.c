#include "../include/jix.h"

char compareNames(const char* name,const char* cmpr) {
    for (unsigned int i = 0; i < PropertysNameFixed; i++) {
        if (name[i] != cmpr[i])
            return name[i] - cmpr[i];

        if (name[i] == '\0')
            return 0;
    }

    return 0;
}
void SetProperty(
    METATABLE* table,
    const char* entryName,
    METAPROP typeofValue,
    METATHING value
) {
    if (!table || !entryName)
        return;

    unsigned int i;

    // Reject names exceeding the fixed size.
    for (i = 0; i < PropertysNameFixed; i++) {
        if (entryName[i] == '\0')
            break;
    }

    if (i == PropertysNameFixed)
        return;

    METAENTRY* entry = GetProperty(table, entryName);

    if (entry) {
        entry->typeofValue = typeofValue;
        entry->anyThing = value;
        return;
    }

    unsigned int newLen = table->length + 1;

    METAENTRY* newEntrys =
        allocateMemory(sizeof(METAENTRY) * newLen);

    if (!newEntrys)
        return;

    // Copy existing entries.
    if (table->entrys) {
        for (i = 0; i < table->length; i++)
            newEntrys[i] = table->entrys[i];
    }

    // Initialize new entry.
    METAENTRY* newEntry = &newEntrys[table->length];

    for (i = 0; i < PropertysNameFixed; i++)
        newEntry->entryName[i] = entryName[i];

    newEntry->typeofValue = typeofValue;
    newEntry->anyThing = value;

    // Replace old storage.
    METAENTRY* oldEntrys = table->entrys;

    table->entrys = newEntrys;
    table->length = newLen;

    if (oldEntrys)
        freeMemory(oldEntrys);
} // The property setter
METAENTRY* GetProperty(
    METATABLE* table,
    const METANAMER* entryName
) {
    if (!table) return (METAENTRY*)0;
    unsigned int item = 0;
    while (item < table->length) {
        METAENTRY* entry = &table->entrys[item];
        if (!compareNames(entry->entryName, entryName))
            return entry;
        item++;
    }
    return (METAENTRY*)0;
} // Entry name
METATABLE* JixTable(void)
{
    METATABLE* table = allocateMemory(sizeof(METATABLE));

    if (!table)
        return 0;

    table->length = 0;
    table->entrys = 0;

    return table;
} // Jix table maker