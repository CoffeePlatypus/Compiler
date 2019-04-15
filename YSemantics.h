/* Semantics.h
 The action and supporting routines for performing semantics processing.
 */

#include "SymTab.h"
#include "IOMngr.h"
#include "YCodeGen.h"
#include "YStructs.h"

// Supporting Routines

void InitSemantics();
void FinishSemantics();

// Semantics Actions

// Step 1
void                    ProcDecl(struct IdList * idList, enum BaseTypes baseType, int initialValue);
void                    ProcDeclFunc(struct IdList * idList, enum BaseTypes type);
void                    ProcFuncBody(struct IdList * idItem, struct InstrSeq * codeBlock);
struct IdList *         AppendIdList(struct IdList * item, struct IdList * list);
struct IdList *         ProcName(char * tokenText, struct Span span);
