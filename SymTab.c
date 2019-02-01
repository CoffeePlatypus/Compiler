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
     // malloc symbol table
     struct SymTab * table = malloc(sizeof(struct SymTab));
     table->scopeName = strdup(scopeName);
     table->parent = parentTable // This might not be right
     table->size = 0;
     table->count = 0;
     table->contents = malloc(sizeof(* struct SymEntry) * size); // contets size = size of SymEntry pointers mult size?
}

struct SymTab *
DestroySymTab(struct SymTab *aTable) {
     free(aTable->scopeName);
     // TODO add more free, like for contents probably
     free(aTable);
}

unsigned int
HashName(int size, const char *name) {
     int hashy = 0;
     for(int i = 0; i < size; i++) {
          hash += name[i];
     }
     return hashy;
}

// googled strcmp to be sure it works how I thought
struct SymEntry *
FindHashedName(struct SymTab *aTable, int hashValue, const char *name) {
     struct SymEntry * temp = aTable->contents[hashValue];
     while(temp) {
          if(!strcmp(temp->name, name)) {
               return temp;
          }
     }
     return NULL;
}

struct SymEntry *
LookupName(struct SymTab *aTable, const char * name) {
     struct SymEntry * temp = FindHashedName(aTable, HashName(strlen(name), name), name);
     if(temp) {
          return temp;
     }else if(aTable->parent) {
          return LookupName(aTable->parent, name);
     }else{
          return NULL;
     }
}

struct SymEntry *
EnterName(struct SymTab *aTable, const char *name) {
     int hashy = HashName(strlen(name);
     struct SymEntry * temp = FindHashedName(aTable, hashy, name), name);
     if(temp) {
          return temp;
     }
     
}

void
SetAttr(struct SymEntry *anEntry, int kind, void *attributes) {
     anEntry->attributes = attributes;
     anEntry->kind = kind;
}

int
GetAttrKind(const struct SymEntry *anEntry) {
     return anEntry? anEntry->kind : NULL;
}

void *
GetAttr(const struct SymEntry *anEntry) {
     return anEntry? anEntry->attributes : NULL;
}

const char *
GetName(const struct SymEntry *anEntry) {
     return anEntry? anEntry->name : NULL;
}

struct SymTab *
GetTable(const struct SymEntry *anEntry) {
     return anEntry? anEntry->table : NULL;
}

const char *
GetScopeName(const struct SymTab *aTable) {
     return anEntry? aTable->ScopeName : NULL;
}

char *
GetScopeNamePath(const struct SymTab *aTable) {
     // I think I may run into a problem with strcat here
     return aTable? strcat(aTable->name, GetScopeName(aTable->parent)) : '/0';
}

struct SymTab *
GetParentTable(const struct SymTab *aTable) {
     return aTable? aTable->parent : NULL;
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
