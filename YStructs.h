/* YStructs.h
   Semantics struct definitions and utility functions
 */

#ifndef YSTRUCTS_H
#define YSTRUCTS_H

#include "SymTab.h"
#include "IOMngr.h"

static char * BaseTypeNames[4] = { "int", "chr", "bool", "void"};
static char * TypeNames[3] = { "unkn", "prim", "func"};

enum AttrKinds { VOID_KIND = -1, INT_KIND, STRING_KIND, STRUCT_KIND };

/* Declaration of semantic record data types used in grammar.y %union */
struct IdList {
  struct SymEntry * entry;
  struct Span span;
  struct IdList * next;
};

enum BaseTypes { IntBaseType, ChrBaseType, BoolBaseType, VoidBaseType };

struct FuncDesc {
  enum BaseTypes returnType;
  struct InstrSeq * funcCode;
};

struct PrimDesc {
  enum BaseTypes type;
  int initialValue;           // only meaningful with desc of identifier
};

enum DeclTypes { UnknownType, PrimType, FuncType };

struct TypeDesc {
  enum DeclTypes declType;
  union {
    struct PrimDesc primDesc;
    struct FuncDesc funcDesc;
  };
};

 struct LiteralDesc {
   enum BaseTypes baseType;
   int value;
 };

 // Symbol Table Structures
 struct Attr {
   struct TypeDesc * typeDesc;
   char * reference;
   struct Span declSpan;
};

struct ExprResult {
     struct InstrSeq * exprCode;
     int resultRegister;
     char operator;
     enum BaseTypes resType;
     struct LiteralDesc * desc;
};

// Make and Free Structs
struct IdList *        MakeIdList(struct SymEntry * entry, struct Span span);
struct TypeDesc *      MakePrimDesc(enum BaseTypes type, int initialValue);
struct TypeDesc *      MakeFuncDesc(enum BaseTypes returnType);
struct LiteralDesc *   MakeLiteralDesc(char * text, enum BaseTypes type);
struct Attr *          MakeAttr(struct TypeDesc * typeDesc, char * reference, struct Span span);
struct ExprResult *    MakeExprResult(struct InstrSeq * exprCode, int resultRegister, char operator, enum BaseTypes resType, struct LiteralDesc * desc);

void                   FreeIdList(struct IdList * item);
void                   FreeTypeDesc(struct TypeDesc * typeDesc);
void                   FreeLiteralDesc(struct LiteralDesc * literalDesc);
void                   FreeAttr(struct Attr * attr);

// Supporting Functions
char *                AppendStr(const char * str1, const char * str2);

// Message Posting Function
void PostMessageAndExit(struct Span span, char * message);
void CondPostMessageAndExit(bool cond, struct Span span, char * message);

struct Span LastTokenSpan();

// Symbol Table Functions
void DisplaySymbolTable(struct SymTab * table);

#endif
