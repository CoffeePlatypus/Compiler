/* CodeGen.h
   Routines to support the generation of assembly code.

   Code is maintained as linked list of struct InstrSeq.

   InitCodeGen    - does module initialization and sets filename for writing
                    out assembly code
   GenInstr       - create a struct InstrSeq node from supplied fields
   AppendSeq      - concatenates two instruction sequences into one
   WriteSeq       - write instruction sequence to output file

   GenLabel       - return string of form "L####" to use as label

   AvailTmpReg    - return the index of an available register
   TmpRegName     - returns string to use for temporary register RegNum, string
                    is of the form "$t#"
   ReleaseTmpReg  - marks register as being available
   ResetAllTmpReg - marks all registers as being available

   SaveSeq        - creates the instruction sequence to save all in use
                    registers prior to a function call
   RestoreSeq     - creates the instruction sequence to restore all in use
                    registers after a function call

   Imm            - string to use as the immediate of value
   RegOff         - string to use for register plus offset, of the form
                    "%d(%s)"
*/

#include <stdio.h>

struct InstrSeq {
  struct InstrSeqNode * head;
  struct InstrSeqNode * tail;
};

void							 InitCodeGen(char *AFilename);
void               CloseCodeGen();

// Convenience Macros
#define GenLabel(Label)                                  GenInstr(Label,NULL,NULL,NULL,NULL,NULL)
#define GenLabelX(Label)                                 GenInstr(Label,NULL,NULL,NULL,NULL,__func__)
#define GenLabelC(Label,Comment)                         GenInstr(Label,NULL,NULL,NULL,NULL,Comment)
#define GenOp(Op)                                        GenInstr(NULL,Op,NULL,NULL,NULL,NULL)
#define GenOpX(Op)                                       GenInstr(NULL,Op,NULL,NULL,NULL,__func__)
#define GenOpC(Op,Comment)                               GenInstr(NULL,Op,NULL,NULL,NULL,Comment)
#define GenOp1(Op,Oprnd1)                                GenInstr(NULL,Op,Oprnd1,NULL,NULL,NULL)
#define GenOp1X(Op,Oprnd1)                               GenInstr(NULL,Op,Oprnd1,NULL,NULL,__func__)
#define GenOp1C(Op,Oprnd1,Comment)                       GenInstr(NULL,Op,Oprnd1,NULL,NULL,Comment)
#define GenOp2(Op,Oprnd1,Oprnd2)                         GenInstr(NULL,Op,Oprnd1,Oprnd2,NULL,NULL)
#define GenOp2X(Op,Oprnd1,Oprnd2)                        GenInstr(NULL,Op,Oprnd1,Oprnd2,NULL,__func__)
#define GenOp2C(Op,Oprnd1,Oprnd2,Comment)                GenInstr(NULL,Op,Oprnd1,Oprnd2,NULL,Comment)
#define GenOp3(Op,Oprnd1,Oprnd2,Oprnd3)                  GenInstr(NULL,Op,Oprnd1,Oprnd2,Oprnd3,NULL)
#define GenOp3X(Op,Oprnd1,Oprnd2,Oprnd3)                 GenInstr(NULL,Op,Oprnd1,Oprnd2,Oprnd3,__func__)
#define GenOp3C(Op,Oprnd1,Oprnd2,Oprnd3,Comment)         GenInstr(NULL,Op,Oprnd1,Oprnd2,Oprnd3,Comment)
#define GenComment(Comment)                              GenInstr(NULL,NULL,NULL,NULL,NULL,Comment)

struct InstrSeq *  GenInstr(const char * Label, const char * OpCode,
                            const char * Oprnd1, const char * Oprnd2, const char * Oprnd3,
                            const char * Comment);

struct InstrSeq *  AppendSeq(struct InstrSeq *Seq1,
                             struct InstrSeq *Seq2);
int                SeqLength(struct InstrSeq * Seq1);
void               FreeSeq(struct InstrSeq * Seq);
void 						   WriteSeq(struct InstrSeq *ASeq);

// returned value must be freed
char *						 NewLabel();

// functions for working with registers
int  						   AvailTmpReg();
char *						 TmpRegName(int RegNum);
void							 ReleaseTmpReg(int ANum);
void 	             ResetAllTmpReg();
void               DebugRegs();

// must be strdup'd immediately since private buffer is returned
// should these be const?
const char *						 Imm(int Val);
const char *						 RegOff(int Offset, char * Reg);
const char *             IntLit(int val);

// function calling conventions
//int                FrameSize();
struct InstrSeq *  CalleeProlog();
struct InstrSeq *  CalleeEpilog();
