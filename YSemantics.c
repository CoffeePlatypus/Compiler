/* Semantics.c
 Support and semantic action routines.
    Author:      Julia Froegel
    Created:     04/08/2019
    Resources:

    ¯\_(ツ)_/¯

 */


#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "Scanner.h"
#include "YStructs.h"
#include "YSemantics.h"

// Shared Data

struct SymTab * IdentifierTable;
struct SymTab * StringTable;
int sem_debug = false;

// corresponds to enum Operators
char * Ops[] = { "add", "sub", "mul", "div"};

// enum Comparisons { LtCmp, LteCmp, GtCmp, GteCmp, EqCmp, NeqCmp };
// comparison set instructions, in order with enum Comparisons
char * CmpSetReg[] = { "slt", "sle", "sgt", "sge", "seq", "sne" };

void
InitSemantics() {
  IdentifierTable = CreateSymTab(100,"global",NULL);
  StringTable = CreateSymTab(100, "strings", NULL);
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
    char * retType = malloc(sizeof(char)*10);
    sprintf(retType, "%d", attr->typeDesc->primDesc.initialValue);
    if (sem_debug) printf("%s\n",retType);
    AppendSeq(dataCode, GenInstr(attr->reference, ".word", retType, NULL, NULL, NULL));
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
  struct SymEntry ** functionList = GetEntries(IdentifierTable, false, selectFuncType);

  while(*functionList) {
       struct SymEntry * temp = *functionList;
       struct Attr * attr = GetAttr(temp);
       AppendSeq(textCode, attr->typeDesc->funcDesc.funcCode);
       functionList++;
   }
}

void
processStrings(void * dataCode){
     struct SymEntry **  asciiz = GetEntries(StringTable, false, NULL);
     while (*asciiz) {
          const struct SymEntry * entry = *asciiz;
          struct StrAttr * attr = GetAttr(entry);
          AppendSeq(dataCode, GenInstr(attr->label, ".asciiz", attr->text, NULL, NULL, NULL));
          asciiz++;
     }
}

// Semantics Actions

void
FinishSemantics() {
     // DisplaySymbolTable(IdentifierTable);
  // build text segment
  struct InstrSeq * textCode = GenOpX(".text");

  // form module preamble
  struct SymEntry * mainEntry = LookupName(IdentifierTable,"main");
  CondPostMessageAndExit(!mainEntry,MakeEOFSpan(),"no main function for module");

  // should make sure main takes no arguments
  struct Attr * mainAttr = GetAttr(mainEntry);

  // boilerplate for spim
  AppendSeq(textCode,GenOp1X(".globl","__start"));
  AppendSeq(textCode,GenLabelX("__start"));

  AppendSeq(textCode,GenOp1X("jal",mainAttr->reference));
  AppendSeq(textCode,GenOp2X("li","$v0","10"));
  AppendSeq(textCode,GenOpX("syscall"));

  // append code for all Functions
  processFunctions(textCode);

  struct InstrSeq * dataCode = GenOpX(".data");
  processGlobalIdentifier(dataCode);
  processStrings(dataCode);

  // combine and write
  struct InstrSeq * moduleCode = AppendSeq(textCode,dataCode);
  WriteSeq(moduleCode);
  //
  // // free code
  FreeSeq(moduleCode);
  CloseCodeGen();

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
     // printf("fun\n");
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
       // printf("here %d\n", attr->typeDesc->funcDesc.returnType);
  }
  attr->reference = AppendStr("_",GetName(idItem->entry));
  // printf("atr %s\n", attr->reference);
  SetAttr(d,STRUCT_KIND, attr );
  //figrue out how
  // printf("ref %s", attr->reference);
  struct InstrSeq * header = AppendSeq(GenLabelC(attr->reference, "func entry"), GenOp3X("subu", "$sp", "$sp","4"));
  AppendSeq(header, GenOp2X("sw", "$ra", "0($sp)"));
  codeBlock = AppendSeq( header, codeBlock);

  struct InstrSeq * exit = AppendSeq(GenLabel(AppendStr(attr->reference,"_ret")), GenOp2X("lw","$ra", "0($sp)"));
  AppendSeq(exit, GenOp3X("addu", "$sp", "$sp", "4"));
  AppendSeq(exit, GenOp1C("jr","$ra","func return"));
  AppendSeq(codeBlock, exit);

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
     }
     return MakeIdList(dup, span);
}

struct InstrSeq *
ProcAssign(char * id, struct ExprResult * res){
     if (!res) return NULL;
     // printf("proc ass : %s\n", id);
     struct SymEntry * d = LookupName(IdentifierTable, id);
     struct Attr * attr = GetAttr(d);
     // if (res->resultRegister == -1) {
     //
     // }
     ReleaseTmpReg(res->resultRegister);
     // int reg = AvailTmpReg();

     return AppendSeq(res->exprCode, GenOp2("sw", TmpRegName(res->resultRegister), attr->reference));
}

struct InstrSeq *
Put(struct ExprResult * expr){
     // printf("Put %s\n", expr->desc->value);
     // syscall print output
     if (! expr) {
          printf("put null\n");
          return NULL;
     } // :[
     // printf("put \n");//, expr->desc->value);
     int reg = expr->operator == 'w'? expr->resultRegister : AvailTmpReg();
     int lval = expr->desc->value;

     if(expr->desc->baseType == BoolBaseType) {
          lval = lval ? 84 : 70;
     }
     char * val = malloc(sizeof(char)*5);
     sprintf(val, "%d", lval);
     struct InstrSeq * ins = expr->operator == 'w'? expr->exprCode :GenOp2("li",TmpRegName(reg), val);
     switch(expr->desc->baseType) {
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
     return ins ;
}

struct InstrSeq *
PutString(char * str){
     // StingTable
     // dup = EnterName(IdentifierTable, tokenText);
     // printf("put: %s\n",str);
     char * l = NewLabel();
     struct SymEntry * ent = EnterName(StringTable, l);
     SetAttr(ent,STRUCT_KIND, MakeStrAttr(l,str));
     struct InstrSeq * ins = GenOp2("li", "$v0", "4");
     AppendSeq(ins, GenOp2("la", "$a0", l));
     AppendSeq(ins, GenOp("syscall"));
     return ins;
}

struct ExprResult *
GetInt(){
     // todo generate syscall to get int
     // int reg
     // printf("get int\n");
     struct InstrSeq * ins = AppendSeq(GenOp2C("li", "$v0", "5","read int syscall"), GenOp("syscall"));
     int reg = AvailTmpReg();
     AppendSeq(ins, GenOp2("move", TmpRegName(reg), "$v0"));
     // WriteSeq(ins);
     return MakeExprResult(ins, reg, 'g', IntBaseType, NULL);
}

///// Expr Code! //////////

struct ExprResult *
ProcOp(struct ExprResult * oprnd1, struct ExprResult * oprnd2, int opNum) {
     if ( oprnd1==NULL || oprnd2==NULL) {
          if(oprnd1 == NULL) printf("\t\t\t\t\top1 == null");
          if(oprnd2 == NULL) printf("\t\t\t\t\top2 == null");
          printf("\n");
          return NULL;
     }
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
     // printf("proc Lit %s : %d\n",val, type);

     struct LiteralDesc * litDesc = MakeLiteralDesc(val, type);
     // printf("litDesc %d", litDesc->value);
     if (type == IntBaseType ) {
          int reg = AvailTmpReg();
          struct InstrSeq * ins = GenOp2("li", TmpRegName(reg), val);
          return MakeExprResult(ins, reg, 'l', type, litDesc );
     }else if( type == BoolBaseType){
          int reg = AvailTmpReg();
          struct InstrSeq * ins = GenOp2("li", TmpRegName(reg),  strcmp(val, "true") ? "0" : "1");
          return MakeExprResult(ins, reg, 'l', type, litDesc );
     }
     return MakeExprResult(NULL, -1, 'l', type, litDesc );
}

struct ExprResult *
ProcLoadVar(char * id) {
     // printf("load id %s\n",id);
     // DisplaySymbolTable(IdentifierTable);
     // printf("lw %s\n",id);
     struct SymEntry * d = LookupName(IdentifierTable, id);
     if(d == NULL) {
          printf("\tid not found: %s\n",id);
          return NULL;
     }
     // printf(" ~~~id %s\n", id);
     struct Attr * attr = GetAttr(d);
     // attr->typeDesc->initialValue;
     int reg = AvailTmpReg();
     struct InstrSeq * instr = GenOp2("lw", TmpRegName(reg), attr->reference);
     // WriteSeq(instr);
     char * stringy = malloc(sizeof(char)*10);
     sprintf(stringy, "%d", attr->typeDesc->primDesc.initialValue); // This is gross and stupid - Should probably not do it this way
     return MakeExprResult(instr, reg, 'w', attr->typeDesc->primDesc.type, MakeLiteralDesc(stringy, attr->typeDesc->primDesc.type));
}

//////// Cond /////////
struct CondResult *
ProcCond(struct ExprResult * x, char * op, struct ExprResult * y) {

     struct InstrSeq * ins = AppendSeq(x->exprCode, y->exprCode);
     // printf(NewLabel());
     struct CondResult * res = MakeCondResult(ins, 0, NewLabel());
     AppendSeq(res->exprCode, GenOp3(op,TmpRegName(x->resultRegister), TmpRegName(y->resultRegister), res->label));
     ReleaseTmpReg(x->resultRegister);
     ReleaseTmpReg(y->resultRegister);
     // printf("finished cond %s\n", res->label);
     return res;
}

struct InstrSeq *
ProcIf(struct CondResult * res, struct InstrSeq * ins) {
     // printf("proc if\n");
     ins = AppendSeq(res->exprCode, ins);
     AppendSeq(ins, GenLabel(res->label));
     return ins;
}

struct InstrSeq *
ProcIfElse(struct CondResult * res, struct InstrSeq * ins1, struct InstrSeq * ins2){
     // printf("proc if else\n");
     char * l = NewLabel();
     // WriteSeq(ins2);
     AppendSeq(ins1, GenOp1X("b", l));
     AppendSeq(ins1, GenLabelX(res->label));
     AppendSeq(ins2, GenLabelX(l));
     // printf("1 %s \n2 %s\n", res->label, l);
     return AppendSeq( AppendSeq(res->exprCode, ins1), ins2);
}

struct InstrSeq *
ProcWhile(struct CondResult * cond, struct InstrSeq * ins){
     // printf("Proc while");
     // WriteSeq(ins);
     char * l = NewLabel();
     struct InstrSeq * res =  AppendSeq(GenLabelX(l), cond->exprCode);
     AppendSeq(res, ins);
     AppendSeq(res, GenOp1X("b",l));
     AppendSeq(res, GenLabelX(cond->label));
     return res;
}

struct InstrSeq *
ProcFuncCall(char * id){
     return GenOp1C("jal", AppendStr("_", id), "CallFunc");
}
