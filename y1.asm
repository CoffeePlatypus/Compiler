                    .text                                                # FinishSemantics
                    .globl      __start                                  # FinishSemantics
__start:                                                                 # FinishSemantics
                    jal         _main                                    # FinishSemantics
                    li          $v0         10                           # FinishSemantics
                    syscall                                              # FinishSemantics
_main:                                                                   # func entry
                    subu        $sp         $sp         4                # ProcFuncBody
                    sw          $ra         0($sp)                       # ProcFuncBody
_main_ret:                                                                 
                    lw          $ra         0($sp)                       # ProcFuncBody
                    addu        $sp         $sp         4                # ProcFuncBody
                    jr          $ra                                      # func return
                    .data                                                # FinishSemantics
_cde:               .word       1                                          
_fgh:               .word       32                                         
_ij:                .word       32                                         
_x:                 .word       0                                          
_klmn:              .word       1                                          
_xy:                .word       0                                          
_xyz:               .word       0                                          
_abcd:              .word       0                                          
_abc:               .word       0                                          
_ab:                .word       1                                          
_bcd:               .word       0                                          
