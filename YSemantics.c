/* Semantics.c
 Support and semantic action routines.
    Author:      Julia Froegel
    Created:     04/08/2019
    Resources:

 */


#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "Scanner.h"
#include "YStructs.h"
#include "YSemantics.h"

// Shared Data

struct SymTab * IdentifierTable;
int sem_debug = false;

// corresponds to enum Operators
char * Ops[] = { "add", "sub", "mul", "div"};

// enum Comparisons { LtCmp, LteCmp, GtCmp, GteCmp, EqCmp, NeqCmp };
// comparison set instructions, in order with enum Comparisons
char * CmpSetReg[] = { "slt", "sle", "sgt", "sge", "seq", "sne" };

void
InitSemantics() {
  IdentifierTable = CreateSymTab(100,"global",NULL);
}

bool
selectPrimType(struct SymEntry * entry) {
  struct Attr * attr = GetAttr(entry);
  return attr->typeDesc->declType == PrimType;
}

void
processGlobalIdentifier(void * dataCode) {
  struct SymEntry ** p = GetEntries(IdentifierTable,false,selectPrimType);
  while (*p) {
    const struct SymEntry * entry = *p;
    struct Attr * attr = GetAttr(entry);
    // gen code here
    p++;
  }
}

bool
selectFuncType(struct SymEntry * entry) {
  struct Attr * attr = GetAttr(entry);
  return attr->typeDesc->declType == FuncType;
}

void
processFunctions(void * textCode) {
  // need to implement

}

// Semantics Actions

void
FinishSemantics() {
     // DisplaySymbolTable(IdentifierTable);
  // build text segment
  // struct InstrSeq * textCode = GenOpX(".text");
  //
  // // form module preamble
  // struct SymEntry * mainEntry = LookupName(IdentifierTable,"main");
  // CondPostMessageAndExit(!mainEntry,MakeEOFSpan(),"no main function for module");

  // should make sure main takes no arguments
  // struct Attr * mainAttr = GetAttr(mainEntry);

  // boilerplate for spim
  // AppendSeq(textCode,GenOp1X(".globl","__start"));
  // AppendSeq(textCode,GenLabelX("__start"));
  //
  // AppendSeq(textCode,GenOp1X("jal",mainAttr->reference));
  // AppendSeq(textCode,GenOp2X("li","$v0","10"));
  // AppendSeq(textCode,GenOpX("syscall"));

  // append code for all Functions
 // struct SymEntry ** functionList = GetEntries(IdentifierTable, false, selectFuncType);
 // // to do add from List
 // while(*functionList) {
 //      struct SymEntry * temp = *functionList;
 //      struct Attr * attr = GetAttr(temp);
 //      AppendSeq(textCode, attr->typeDesc->funcDesc.funcCode);
 //      functionList++;
 //  }

  // struct InstrSeq * dataCode = GenOpX(".data");
  //todo get data
  // struct SymEntry ** dataList = GetEntries(IdentifierTable, false, selectPrimType);
  // while(*dataList) {
  //      struct SymEntry * temp = *dataList;
  //      struct Attr * attr = GetAttr(temp);
  //      // char * data = AppendStr(attr->reference, "\t\t\t.word\t");
  //      // data = AppendStr(data, itoa(attr->typeDesc->primDesc.initialValue ));
  //      char * retType = malloc(sizeof(char)*10);
  //      sprintf(retType, "%d", attr->typeDesc->primDesc.initialValue);
  //      if (sem_debug) printf("%s\n",retType);
  //      AppendSeq(dataCode, GenInstr(attr->reference, ".word", retType, NULL, NULL, NULL));
  //      dataList++;
  //  }

  // combine and write
  // struct InstrSeq * moduleCode = AppendSeq(textCode,dataCode);
  // WriteSeq(moduleCode);
  // //
  // // // free code
  // FreeSeq(moduleCode);
  // CloseCodeGen();

  CloseSource();

  DisplaySymbolTable(IdentifierTable);
}

void
ProcDecl(struct IdList * idList, enum BaseTypes baseType, int initialValue) {
  // walk idList item entries
  // get entry attributes
  // check required conditions
  // make and assign primitive type descriptor
  // make and assign id reference string
  while(idList) {
       struct SymEntry * d = LookupName(IdentifierTable, GetName(idList->entry));
       if (sem_debug) printf("Entered %s = %d : %d\n", GetName(idList->entry),initialValue, baseType);
       struct Attr * attr = GetAttr(d);
       attr->typeDesc = MakePrimDesc(baseType, initialValue);
       attr->reference = AppendStr("_",attr->reference);
       SetAttr(d,STRUCT_KIND, attr );
       idList = idList->next;
 }
}

void
ProcDeclFunc(struct IdList * idList, enum BaseTypes type) {
     // printf("declare funct\n");
  // walk idList item entries
  // get entry attributes
  // check required conditions
  // make and assign function descriptor
  // make and assign id reference string

  // can there be multiple declared on a line? if not then it doesn't need loop
  while(idList) {
       struct SymEntry * d = LookupName(IdentifierTable, GetName(idList->entry));
       // d = EnterName(IdentifierTable, GetName(idList->entry));
       struct Attr * attr = GetAttr(d);
       attr->typeDesc = MakeFuncDesc(type);
       attr->reference = AppendStr("_",GetName(idList->entry));
       SetAttr(d,STRUCT_KIND, attr );
       idList = idList->next;
 }
}

void
ProcFuncBody(struct IdList * idItem, struct InstrSeq * codeBlock) {
     printf("fun\n");
  // check single item //InstrSeq in YCodeGen
  // check has typeDesc, if not create one indicating void return
  // wrap codeBlock in function entry label and jump return, assign to typeDesc
  // prepend entry lable and append jr $ra

  struct SymEntry * d = LookupName(IdentifierTable, GetName(idItem->entry));
  // printf("name %s\n", GetName(idItem->entry));
  struct Attr * attr = GetAttr(d);

  if(! attr->typeDesc) {
       attr->typeDesc = MakeFuncDesc(3);
       // scared that this suddenly works
       printf("here %d\n", attr->typeDesc->funcDesc.returnType);
  }
  attr->reference = AppendStr("_",GetName(idItem->entry));
  // printf("atr %s\n", attr->reference);
  SetAttr(d,STRUCT_KIND, attr );
  //figrue out how
  // printf("ref %s", attr->reference);
  codeBlock = AppendSeq(GenLabelC(attr->reference, "func entry"), codeBlock);
  AppendSeq(codeBlock, GenOp1C("jr","$ra","func return"));

  attr->typeDesc->funcDesc.funcCode = codeBlock;

}

struct IdList *
AppendIdList(struct IdList * item, struct IdList * list) {
  // chain item to list, return item
  // printf("append\n");
  if(!list) {
       // printf("-%s- \n",GetName(item->entry));
       return item;
 }
  struct IdList * temp = list;
  while(temp->next){
       // printf("-%s",GetName(temp->entry));
       temp = temp->next;
 }
 temp->next = item;
 // printf("-%s",GetName(temp->entry));
 // printf("-%s- \n",GetName(item->entry));
 return list;
}

struct IdList *
ProcName(char * tokenText, struct Span span) {
  // look up token text
  // check conditions
  // enter token text
  // create struct IdList * item
  // create and init attribute struct
     struct SymEntry * dup = LookupName(IdentifierTable, tokenText);
     if(!dup) {
          dup = EnterName(IdentifierTable, tokenText);
          SetAttr(dup, 0,MakeAttr(NULL, tokenText, span));
          return MakeIdList(dup, span);
     }else{
          //todo maybe problem
           return NULL;
     }
}

struct InstrSeq *
ProcAssign(char * id, struct ExprResult * res){
     printf("proc assing");
     struct SymEntry * d = LookupName(IdentifierTable, id);
     struct Attr * attr = GetAttr(d);
     ReleaseTmpReg(res->resultRegister);
     return AppendSeq(res->exprCode, GenOp2("sw", TmpRegName(AvailTmpReg()), attr->reference));
}

struct InstrSeq *
Put(struct ExprResult * expr){
     printf("put");
     // syscall print output

     int reg = AvailTmpReg();
     int lval = expr->desc->value;

     if(expr->desc->baseType == BoolBaseType) {
          lval = lval ? 84 : 70;
     }
     char * val = malloc(sizeof(char)*5);
     sprintf(val, "%d", lval);
     struct InstrSeq * ins = GenOp2("li",TmpRegName(reg), val);
     switch(expr->desc->baseType->baseType) {
          case IntBaseType:
            AppendSeq(ins, GenOp2("li", "$v0", "1"));
            break;
          case ChrBaseType:
          case BoolBaseType:
            AppendSeq(ins, GenOp2("li", "$v0", "11"));
     }
     AppendSeq(ins, GenOp2("move", "$a0", TmpRegName(reg)));
     AppendSeq(ins, GenOp("syscall"));
     ReleaseTmpReg(reg);
     return AppendSeq(expr->exprCode, ins) ;
}

struct ExprResult *
GetInt(){
     // todo generate syscall to get int
     // int reg
     printf("get int");
     struct InstrSeq * ins = AppendSeq(GenOp2("li", "$v0", "5"), GenOp("syscall"));
     int reg = AvailTmpReg();
     AppendSeq(ins, GenOp2("move", TmpRegName(reg), "$v0"));
     // WriteSeq(ins);
     return MakeExprResult(ins, reg, 'g', IntBaseType, NULL);
}

///// Expr Code! //////////

struct ExprResult *
ProcOp(struct ExprResult * oprnd1, struct ExprResult * oprnd2, int opNum) {
     int reg = AvailTmpReg();
     struct InstrSeq * instr = GenOp3(Ops[opNum], TmpRegName(reg), TmpRegName(oprnd1->resultRegister),  TmpRegName(oprnd2->resultRegister));
     // WriteSeq(instr);
     ReleaseTmpReg(oprnd1->resultRegister);
     ReleaseTmpReg(oprnd2->resultRegister);
     return MakeExprResult(AppendSeq(AppendSeq(oprnd1->exprCode, oprnd2->exprCode), instr), reg, opNum, IntBaseType, NULL );
}

struct ExprResult *
ProcUmin(struct ExprResult * oprnd1) {
     int reg = AvailTmpReg();
     struct InstrSeq * instr = GenOp3("sub", TmpRegName(reg), "0", TmpRegName(oprnd1->resultRegister));
     // WriteSeq(instr);
     ReleaseTmpReg(oprnd1->resultRegister);
     return MakeExprResult(AppendSeq(oprnd1->exprCode, instr), reg, '-', IntBaseType, NULL );
}

struct ExprResult *
ProcLit(char * val, enum BaseTypes type) {
     // IntBaseType
     int reg = AvailTmpReg();
     return MakeExprResult(GenOp2("li", TmpRegName(reg), val), reg, 'l', type, MakeLiteralDesc(val, type));
}

struct ExprResult *
ProcLoadVar(char * id) {
     struct SymEntry * d = LookupName(IdentifierTable, id);
     struct Attr * attr = GetAttr(d);
     // attr->typeDesc->initialValue;
     int reg = AvailTmpReg();
     struct InstrSeq * instr = GenOp2("lw", TmpRegName(reg), id);
     // WriteSeq(instr);
     return MakeExprResult(instr, reg, 'l', attr->typeDesc->primDesc.type, MakeLiteralDesc(attr->typeDesc->primDesc.initialValue, attr->typeDesc->primDesc.type));
}
