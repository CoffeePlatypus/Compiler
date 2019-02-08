#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "SymTab.h"
bool debug = 0;

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

void printTable(struct SymTab * table) {
     if(table) {
          printf("Print Table\n");
          if(table->scopeName) printf("Scope Name: %s\n", table->scopeName);
          printf("Size %d\n", table->size);
          printf("Count %d\n\n",table->count);
          if(1) {
               for(int i = 0; i<table->size; i++) {
                    struct SymEntry * temp = table->contents[i];
                    printf("[%d]\n", i);
                    while(temp) {
                         printf("\t %s \n", temp->name);
                         temp = temp->next;
                    }
               }
          }
     }else{
          printf("NULL table\n");
     }
}

struct SymTab *
CreateSymTab(int size, char * scopeName, struct SymTab * parentTable) {
     // malloc symbol table
     if (debug) printf("create sym tab\n");
     struct SymTab * table = malloc(sizeof(struct SymTab));
     table->scopeName = scopeName ? strdup(scopeName) : scopeName;
     table->parent = parentTable; // This might not be right
     table->size = size;
     table->count = 0;
     table->contents = malloc(size * sizeof(struct SymEntry *)); // contets size = size of SymEntry pointers mult size?
     for(int i = 0; i<size; i++) { //idk
          table->contents[i] = NULL;
     }
     if (debug) printTable(table);
     return table;
}

struct SymTab *
DestroySymTab(struct SymTab *aTable) {
     if (debug) printf("free sym tab\n");
     struct SymTab *temp = aTable->parent;
     free(aTable->scopeName);
     for(int i = 0; i<aTable->size; i++) {
          struct SymEntry * temp = aTable->contents[i];
          while(temp) {
               struct SymEntry * next = temp->next;
               free(temp->name);
               free(temp);
               temp = next;
          }
     }
     free(aTable->contents);
     free(aTable);
     if (debug) printf("freed sym tab\n");
     return temp;
}

unsigned int
HashName(int size, const char *name) {
     int hashy = 0;
     for(int i = 0; i < strlen(name); i++) {
          hashy += name[i];
     }
     return hashy % size;
}

// googled strcmp to be sure it works how I thought
struct SymEntry *
FindHashedName(struct SymTab *aTable, int hashValue, const char *name) {
     if(!aTable) return NULL;
     if (debug) printf("find hashed name at : %d\n", hashValue);
     struct SymEntry * temp = aTable->contents[hashValue];
     while(temp) {
          if(!strcmp(temp->name, name)) {
               if (debug) printf("found hashed name\n\n");
               return temp;
          }
          temp = temp->next;
     }
     if (debug) printf("no hashed name\n");
     return NULL;
}

struct SymEntry *
LookupName(struct SymTab *aTable, const char * name) {
     if(!(aTable && name)) return NULL;
     if (debug) printf("lookup name : %s\n", name);
     struct SymEntry * temp = FindHashedName(aTable, HashName(aTable->size, name), name);
     if(temp) {
          return temp;
     }else if(aTable->parent) {
          if (debug) printf("lookup in parent\n");
          return LookupName(aTable->parent, name);
     }else{
          if (debug) printf("no name\n");
          return NULL;
     }
}

struct SymEntry *
EnterName(struct SymTab *aTable, const char *name) {
     if(!(aTable && name)) return NULL;
     if (debug) {
          printf("enter name : %s\n", name);
          // printTable(aTable);
     }
     int hashy = HashName(aTable->size, name);
     struct SymEntry * temp = FindHashedName(aTable, hashy, name);
     if(temp) {
          return temp;
     }
     if (debug) printf("create entry to add\n");
     struct SymEntry * newEntry = malloc(sizeof(struct SymEntry));
     newEntry->name = strdup(name);
     newEntry->next = aTable->contents[hashy];
     newEntry->table = aTable;
     newEntry->attributes = NULL;
     newEntry->attrKind = 0;
     aTable->contents[hashy] = newEntry;
     aTable->count++;
     if (debug) {
          printf("added name\n\n");
          // Statistics(aTable);
          printTable(aTable);
     }
     return newEntry;
}

void
SetAttr(struct SymEntry *anEntry, int kind, void *attributes) {
     anEntry->attributes = attributes;
     anEntry->attrKind = kind;
}

int
GetAttrKind(const struct SymEntry *anEntry) {
     return anEntry? anEntry->attrKind : 0;
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
     return aTable? aTable->scopeName : NULL;
}

char *
GetScopeNamePath(const struct SymTab *aTable) {
     // I think I may run into a problem with strcat here
     // return aTable? strcat(aTable->scopeName, GetScopeName(aTable->parent)) : '/0'; <- no work
     if(!aTable) {
          // printf("\tno table\n");
          return strdup("");
     }else if(!aTable->parent) {
          // printf("\tno parent: %s\n",aTable->scopeName);
          return strdup(aTable->scopeName);
     }else{

          char * parentString = GetScopeNamePath(aTable->parent);
          //printf("\t%s has parent %s with len %d\n", aTable->scopeName, parentString,strlen(parentString));
          char * dest = malloc( (strlen(parentString)+strlen(aTable->scopeName)+2) * sizeof(char));
          dest[0] = '\0';
          //printf("\t\t%s\n",dest);
          strcat(dest, parentString);
          //printf("\t\t%s\n",dest);
          free(parentString);
          strcat(dest, ">");
          //printf("\t\t%s\n",dest);
          strcat(dest, aTable->scopeName);
          //printf("\t\t%s\n",dest);
          return dest;
     }
     return strdup(aTable->scopeName);

}

struct SymTab *
GetParentTable(const struct SymTab *aTable) {
     return aTable? aTable->parent : NULL;
}

/// why are these scoped outside the methods?
struct SymEntry ** entryArray = NULL;
int entryArraySize = 0;
int entIndex = 0;

void ProvisionArray(struct SymTab * aTable, bool includeParents) {
     // sum table sizes to get requested size
     // TODO sum counts in aTable and maybe parents. lol what that said ^
     int reqSize = aTable->count;
     if(includeParents) {
          struct SymTab * temp = aTable->parent;
          while(temp) {
               reqSize += temp->count;
               temp = temp->parent;
          }
     }
     reqSize ++;
     if (entryArraySize < reqSize) {
          entryArray = realloc(entryArray,reqSize * sizeof(struct SymEntry *));
     }
}

void
CollectEntries(struct SymTab * aTable, bool includeParents, entryTestFunc testFunc) {
  // enumerate table collecting SymEntry pointers, if testFunc provided used to
  // select entries, null terminate the array
  if(debug) printf("collect\n");
//   if(!entryTestFunct) { // if entry exists
//      entryTestFunct(entry); // if keep entry
// }
     if(!aTable) return;
     if(debug) printTable(aTable);

     for(int i = 0; i < aTable->size; i++) {
          struct SymEntry * temp = aTable->contents[i];
          while(temp) {
               if(!testFunc || testFunc(temp)) { // much better than before
                     entryArray[entIndex++] = temp;
                }
               temp = temp->next;
          }
     }
     if(includeParents) {
          CollectEntries(aTable->parent, includeParents, testFunc);
     }
     entryArray[entIndex] = NULL;
     // todo add parents;
}

void printEntries() {
     printf("Printin entries\n");
     int i = 0;
     struct SymEntry * temp = entryArray[i++];
     while(temp) {
          printf("\t%s\n", temp->name);
          temp = entryArray[i++];
     }
}

struct SymEntry **
GetEntries(struct SymTab * aTable, bool includeParents, entryTestFunc testFunc) {
     if(debug) printf("get entries\n");
     // ProvisionArray
     ProvisionArray(aTable, includeParents);
     // collect
     entIndex = 0;
     CollectEntries(aTable, includeParents, testFunc);
     // return?
     if(debug) printEntries();
     return entryArray;
}

struct Stats *
Statistics(struct SymTab *aTable) {
     if(debug) printf("\n printing stats\n");
     struct Stats * stats = malloc(sizeof(struct Stats));
     struct SymEntry * temp = aTable->contents[0];
     int count = 0;
     while(temp) {
          count++;
          temp = temp->next;
     }
     if (debug) printf("hash [0] : %d\n", count);
     stats->minLen = count;
     stats->maxLen = count;
     stats->entryCnt = count;
     for(int i = 1; i < aTable->size; i++) {
          temp = aTable->contents[i];
          count = 0;
          while(temp) {
               count++;
               temp = temp->next;
          }
          stats->entryCnt += count;
          if(count < stats->minLen) {
               stats->minLen = count;
          }else if(count > stats->maxLen) {
               stats->maxLen = count;
          }
          if (debug) printf("hash [%d] : %d\n", i, count);
     }
     if(debug) printf("\n");
     stats->avgLen = stats->entryCnt / aTable->size;
     return stats;
}
