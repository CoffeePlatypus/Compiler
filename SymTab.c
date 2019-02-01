#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "SymTab.h"

/* The symbol table entry structure proper.
*/
struct SymEntry {
  struct SymTab * table;
  char * name;
  int attrKind;
  void * attributes;
  struct SymEntry * next;
};

/* The symbol table structure proper. The hash table array Contents
   is dynamically allocated according to size
*/
struct SymTab {
  struct SymTab * parent;
  char * scopeName;
  int size;
  int count;
  struct SymEntry ** contents;
};

struct SymTab *
CreateSymTab(int size, char * scopeName, struct SymTab * parentTable) {
}

struct SymTab *
DestroySymTab(struct SymTab *aTable) {
}

unsigned int
HashName(int size, const char *name) {
}

struct SymEntry *
FindHashedName(struct SymTab *aTable, int hashValue, const char *name) {
}

struct SymEntry *
LookupName(struct SymTab *aTable, const char * name) {
}

struct SymEntry *
EnterName(struct SymTab *aTable, const char *name) {
}

void
SetAttr(struct SymEntry *anEntry, int kind, void *attributes) {
}

int
GetAttrKind(const struct SymEntry *anEntry) {
}

void *
GetAttr(const struct SymEntry *anEntry) {
}

const char *
GetName(const struct SymEntry *anEntry) {
}

struct SymTab *
GetTable(const struct SymEntry *anEntry) {
}

const char *
GetScopeName(const struct SymTab *aTable) {
}

char *
GetScopeNamePath(const struct SymTab *aTable) {
}

struct SymTab *
GetParentTable(const struct SymTab *aTable) {
}

struct SymEntry ** entryArray = NULL;
int entryArraySize = 0;

void ProvisionArray(struct SymTab * aTable, bool includeParents) {
  // sum table sizes to get requested size
  if (entryArraySize < reqSize) {
    entryArray = realloc(entryArray,reqSize * sizeof(struct SymEntry *));
  }
}

void
CollectEntries(struct SymTab * aTable, bool includeParents, entryTestFunc testFunc) {
  // enumerate table collecting SymEntry pointers, if testFunc provided used to
  // select entries, null terminate the array
}

struct SymEntry **
GetEntries(struct SymTab * aTable, bool includeParents, entryTestFunc testFunc) {
}

struct Stats *
Statistics(struct SymTab *aTable) {
}
