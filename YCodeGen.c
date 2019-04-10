//#define INFINITE_REGISTERS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "YCodeGen.h"

struct InstrSeqNode {
  char * Label;
  char * OpCode;
  char * Oprnd1;
  char * Oprnd2;
  char * Oprnd3;
  char * Comment;
  struct InstrSeqNode * next;
};

char *AssmFilename;
FILE *AssmFile;

int NextLabel;

struct TmpReg {
  bool free;
  bool hasBeenUsed;
  char * name;
} Registers[]
  = { {true, false, "$t0"},  {true, false, "$t1"},  {true, false, "$t2"},
      {true, false, "$t3"},  {true, false, "$t4"},  {true, false, "$t5"},
      {true, false, "$t6"},  {true, false, "$t7"},  {true, false, "$t8"},
      {true, false, "$t9"},  {true, false, "$s0"},  {true, false, "$s1"},
      {true, false, "$s2"},  {true, false, "$s3"},  {true, false, "$s4"},
    };
#define MAX_REG 15

char Buf[16];

void
InitCodeGen(char * AFilename) {
  AssmFilename = malloc(strlen(AFilename)+1);
  strcpy(AssmFilename,AFilename);
  AssmFile = fopen(AssmFilename,"w");
  //AssmFile = stdout;

  NextLabel = 1;
}

void
CloseCodeGen() {
  free(AssmFilename);
}

char *
CopyStr(const char * AStr) {
  return (AStr) ? strdup(AStr) : NULL;
}

struct InstrSeq *
GenInstr(const char * Label, const char * OpCode, const char * Oprnd1, const char * Oprnd2, const char * Oprnd3, const char * Comment) {
  struct InstrSeqNode * node = malloc(sizeof(struct InstrSeqNode));
  node->Label = CopyStr(Label);
  node->OpCode = CopyStr(OpCode);
  node->Oprnd1 = CopyStr(Oprnd1);
  node->Oprnd2 = CopyStr(Oprnd2);
  node->Oprnd3 = CopyStr(Oprnd3);
  node->Comment = CopyStr(Comment);
	node->next = NULL;

  struct InstrSeq * seq = malloc(sizeof(struct InstrSeq));
  seq->head = node;
  seq->tail = node;

  return seq;
}


extern struct InstrSeq *
AppendSeq(struct InstrSeq * seq1, struct InstrSeq * seq2) {
  if (!seq1 && seq2) return seq2;
  if (!seq2 && seq1) return seq1;
  if (!seq1 && !seq2) return NULL;

  seq1->tail->next = seq2->head;
  seq1->tail = seq2->tail;
  free(seq2);

  return seq1;
}

int
SeqLength(struct InstrSeq * seq1) {
  int cnt = 0;
  if (!seq1) return 0;
  struct InstrSeqNode * p = seq1->head;
  while (p) {
    cnt++;
    p = p->next;
  }
  return cnt;
}

void
FreeSeq(struct InstrSeq * seq) {
  struct InstrSeqNode * node = seq->head;
  while (node) {
    struct InstrSeqNode * next = node->next;
    free(node);
    node = next;
  }
  free(seq);
}

char *
SafeString(char * str) {
  return str ? str : "";
}

void
WriteSeq(struct InstrSeq *ASeq) {
  struct InstrSeqNode * node = ASeq->head;
  while (node) {
    char * label = SafeString(node->Label);
    int labelPadding = strlen(label) < 20 ? 20 - strlen(label) : 0;
    fprintf(AssmFile,"%s%-*s%-12s%-12s%-12s%-12s     %c %s\n",
            SafeString(label),
            node->Label ? labelPadding : 20,
            node->Label ? ":" : "",
            SafeString(node->OpCode),
            SafeString(node->Oprnd1),
            SafeString(node->Oprnd2),
            SafeString(node->Oprnd3),
            node->Comment ? '#' : ' ',
            SafeString(node->Comment)
          );
    node = node->next;
  }
  fclose(AssmFile);
}

char *
NewLabel(){
  char *label = (char *) malloc(8);
  sprintf(label,"L%d",NextLabel++);

  return label;
}

#ifndef INFINITE_REGISTERS

int
AvailTmpReg() {
  for (int i = 0; i < MAX_REG; i++) {
    if (Registers[i].free) {
      Registers[i].free = false;
      Registers[i].hasBeenUsed = true;
      return i;
    }
  }

  return -1;
}

char *
TmpRegName(int RegNum) {
  if ((RegNum >= 0) && ( RegNum < MAX_REG)) {
    return Registers[RegNum].name;
  }
  else {
    return NULL;
  }
}

void
ReleaseTmpReg(int ANum) {
  if ((ANum >= 0) && ( ANum < MAX_REG)) {
    Registers[ANum].free = true;
  }
  return;
}

void
ResetAllTmpReg() {
  for (int i = 0; i < MAX_REG; i++) {
    Registers[i].free = true;
    Registers[i].hasBeenUsed = false;
  }
  return;
}

void
DebugRegs() {
  for (int i = 0; i < MAX_REG; i++) {
    fprintf(stderr,"%d ",Registers[i].free);
  }
  fprintf(stderr,"\n");
}

#else

int nextRegNum = 0;

int
AvailTmpReg() {
  return nextRegNum++;
}

// this will leak memory
char *
TmpRegName(int RegNum) {
  char * n = malloc(16);
  sprintf(n,"$r%d",RegNum);
  return n;
}

void
ReleaseTmpReg(int ANum) {

}

void
ResetAllTmpReg() {
  nextRegNum = 0;
}

void
DebugRegs() {
  fprintf(stderr,"nextRegNum %d\n",nextRegNum);
}

#endif

const char *
Imm(int Val) {
  sprintf(Buf,"%d",Val);
  return Buf;
}

const char *
RegOff(int Offset, char * Reg) {
  sprintf(Buf,"%d(%s)",Offset,Reg);
  return Buf;
}

const char *
IntLit(int val) {
  sprintf(Buf,"%d",val);
  return Buf;
}
//int
//FrameSize() {
//  int cnt = 0;
//  for (int i = 0; i < MAX_REG; i++) {
//    if (Registers[i].hasBeenUsed) cnt++;
//  }
//  return 4*(cnt+3);
//}

// struct InstrSeq *
// CalleeProlog() {
//   int cnt = 0;
//   struct InstrSeq * saveSeq = GenInstr(NULL,"sw","$ra",RegOff(cnt*4,"$sp"),NULL);
//   cnt++;
//   AppendSeq(saveSeq,GenInstr(NULL,"sw","$fp",RegOff(cnt*4,"$sp"),NULL));
//   cnt++;
//   // use $s7 as pointer to start of local args
//   // this will avoid having to know how many registers are being saved,
//   // and consequently the frame size, prior to completing the func code
//   AppendSeq(saveSeq,GenInstr(NULL,"sw","$s7",RegOff(cnt*4,"$sp"),NULL));
//   cnt++;
//
//   for (int i = 0; i < MAX_REG; i++) {
//     if (Registers[i].hasBeenUsed) {
//       AppendSeq(saveSeq,GenInstr(NULL,"sw",TmpRegName(i),RegOff(cnt*4,"$sp"),NULL));
//       cnt++;
//     }
//   }
//   saveSeq = AppendSeq(GenInstr(NULL,"addiu","$sp","$sp",Imm(-cnt*4)),saveSeq);
//   AppendSeq(saveSeq,GenInstr(NULL,"addiu","$fp","$sp",Imm((cnt-1)*4)));
//   AppendSeq(saveSeq,GenInstr(NULL,"addiu","$s7","$sp",Imm(-4)));
//
//   return saveSeq;
// }
//
// struct InstrSeq *
// CalleeEpilog() {
//   int cnt = 0;
//   struct InstrSeq * restoreSeq = GenInstr(NULL,"lw","$ra",RegOff(cnt*4,"$sp"),NULL);
//   cnt++;
//   AppendSeq(restoreSeq,GenInstr(NULL,"lw","$fp",RegOff(cnt*4,"$sp"),NULL));
//   cnt++;
//   AppendSeq(restoreSeq,GenInstr(NULL,"lw","$s7",RegOff(cnt*4,"$sp"),NULL));
//   cnt++;
//
//   for (int i = 0; i < MAX_REG; i++) {
//     if (Registers[i].hasBeenUsed) {
//       AppendSeq(restoreSeq,GenInstr(NULL,"lw",TmpRegName(i),RegOff(cnt*4,"$sp"),NULL));
//       cnt++;
//     }
//   }
//   AppendSeq(restoreSeq,GenInstr(NULL,"addiu","$sp","$sp",Imm(cnt*4)));
//
//   return restoreSeq;
// }
