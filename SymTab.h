/* SymTab.h

  This file defines the interface to a simple symbol table.
  For the purposes of this module a symbol table is abstractly
  viewed as a collection of entries each associating a name
  with a single attribute which is taken to be a (void *).
  This allows the user to associate a name with an arbitrary
  structure of attribute information which the user creates
  and maintains. A symbol table can optionally reference a
  parent symbol table to support names in nested scopes.

*/

#ifndef SYMTAB_H
#define SYMTAB_H

#include <stdbool.h>

/* The symbol table entry structure. Contains name and attribute information.
   Definition is in the implementation file.
*/
struct SymEntry;

/* The symbol table structure proper. Definition is in the implementation
   file.
*/
struct SymTab;

/* CreateSymTab     Create and return a reference to a symbol table of
                    approximately Size many entries. ScopeName is a name to
                    describe the scope of the table and maybe NULL. If
                    parentTable is not NULL, the new table will reference it.
                    Returns a pointer to the table or NULL if unable to create
                    the table.

   DestroySymTab    Destroy all storage associated with a Symbol Table which
                    is under the table's control. This function can only free
                    storage that has been allocated by this module. Any
                    storage allocated for attributes associated with an entry
                    must be freed before the table is destroyed. See SymTabDriver
                    for an example of how this can be done. This function returns
                    the parent table reference. 
*/
struct SymTab *     CreateSymTab(int size, char * scopeName, struct SymTab * parentTable);
struct SymTab *     DestroySymTab(struct SymTab *aTable);

/* LookupName       Search aTable, and if necessary parent tables, for an
                    entry for aName. Returns the first entry found or NULL.

   EnterName        Create an new entry for aName in aTable if it does not
                    exist in aTable. In either case return a reference to the
                    entry.
*/
struct SymEntry *   LookupName(struct SymTab *aTable, const char * aName);
struct SymEntry *   EnterName(struct SymTab *aTable, const char * aName);

/* SetAttr          Set the attribute and "kind" associated with anEntry. The kind
                    is a user defined integer which can be used to distinguish
                    the type of data. Attributes is a "void *" and so may be
                    an address or a long int.
   GetAttrKind      Return the "kind" of the attribute or null if anEntry == NULL.
   GetAttr          Return the attribute associated with anEntry or NULL if
                    anEntry == NULL.
   GetName          Return the name string associated with anEntry or NULL if
                    anEntry == NULL.
   GetTable         Return the table that contains anEntry or NULL if
                    anEntry == NULL.
   GetScopeName     Return the scope name of aTable or NULL if anEntry == NULL.
   GetScopeNamePath Return a new string combining the scope name of aTable
                    and all parent tables. The caller is responsible for
                    freeing the returned string.
   GetParentTable   Return the parent of aTable or NULL if aTable == NULL.
*/
void                SetAttr(struct SymEntry *anEntry, int kind, void *attributes);
int                 GetAttrKind(const struct SymEntry *anEntry);
void *              GetAttr(const struct SymEntry *anEntry);
const char *        GetName(const struct SymEntry *anEntry);
struct SymTab *     GetTable(const struct SymEntry *anEntry);
const char *        GetScopeName(const struct SymTab *aTable);
char *              GetScopeNamePath(const struct SymTab *aTable);
struct SymTab *     GetParentTable(const struct SymTab *aTable);

/* GetEntries       Return an array of SymEntry pointers for all entires in
                    aTable, optionally including parent tables, which satisfy
                    the textFunc which is of type entryTestFunc. An entryTestFunc
                    takes an entry reference and tests it for some property and
                    returns a boolean result. If textFunc is NULL all entries
                    are included in the list.
*/

typedef bool (*entryTestFunc)(struct SymEntry * entry);

struct SymEntry ** GetEntries(struct SymTab *aTable, bool includeParentTable,
                      entryTestFunc testFunc);

/* Statistics       Return a structure containing statistics about aTable.
                    - minLen is the length of the shortest chain in the hash table
                    - maxLen is the length of the longest chain in the hash table
                    - avgLen is the average chain length
                    - entryCnt is the sum of the chain lengths
*/
struct Stats { int minLen;
               int maxLen;
               int avgLen;
               int entryCnt;
};
struct Stats *      Statistics(struct SymTab *aTable);

#endif
