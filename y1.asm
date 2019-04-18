                    .text                                                # FinishSemantics
                    .globl      __start                                  # FinishSemantics
__start:                                                                 # FinishSemantics
                    jal         _main                                    # FinishSemantics
                    li          $v0         10                           # FinishSemantics
                    syscall                                              # FinishSemantics
_main:                                                                   # func entry
                    jr          $ra                                      # func return
                    .data                                                # FinishSemantics
_cde			.word	:                                                             
_fgh			.word	:                                                             
_ij			.word	:                                                              
_x			.word	:                                                               
_klmn			.word	:                                                            
_xy			.word	:                                                              
_xyz			.word	:                                                             
_abcd			.word	:                                                            
_abc			.word	:                                                             
_ab			.word	:                                                              
_bcd			.word	:                                                             
