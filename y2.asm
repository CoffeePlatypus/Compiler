                    .text                                                # FinishSemantics
                    .globl      __start                                  # FinishSemantics
__start:                                                                 # FinishSemantics
                    jal         main                                     # FinishSemantics
                    li          $v0         10                           # FinishSemantics
                    syscall                                              # FinishSemantics
                    .data                                                # FinishSemantics
