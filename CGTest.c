#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "YCodeGen.h"

int
main(int argc, char * argv[]) {

  InitCodeGen("CGTest.asm");

  int aNum = AvailTmpReg();
  char * aName = TmpRegName(aNum);
  printf("register %d has name %s\n",aNum,aName);
  int bNum = AvailTmpReg();
  char * bName = TmpRegName(bNum);
  printf("register %d has name %s\n",bNum,bName);
  int cNum = AvailTmpReg();
  char * cName = TmpRegName(cNum);
  printf("register %d has name %s\n",cNum,cName);

  char * aLabel = NewLabel();
  char * bLabel = NewLabel();
  printf("label %s\n",aLabel);
  printf("label %s\n",bLabel);

  struct InstrSeq * code = GenOp(".text");
  code = AppendSeq(code,GenInstr(aLabel,"li",Imm(5),aName,NULL,NULL));
  code = AppendSeq(code,GenOp2("lw","_x",bName));
  code = AppendSeq(code,GenOp3("add",aName,bName,cName));

  ReleaseTmpReg(aNum);
  ReleaseTmpReg(bNum);

  code = AppendSeq(code,GenLabel(bLabel));
  code = AppendSeq(code,GenOp(".data"));
  code = AppendSeq(code,GenLabel("_x"));
  code = AppendSeq(code,GenOp1(".word",Imm(0)));

  ReleaseTmpReg(aNum);
  ReleaseTmpReg(bNum);

  WriteSeq(code);
  FreeSeq(code);

  CloseCodeGen();
}
