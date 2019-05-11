                    .text                                                # FinishSemantics
                    .globl      __start                                  # FinishSemantics
__start:                                                                 # FinishSemantics
                    jal         _main                                    # FinishSemantics
                    li          $v0         10                           # FinishSemantics
                    syscall                                              # FinishSemantics
_main:                                                                   # func entry
                    subu        $sp         $sp         4                # ProcFuncBody
                    sw          $ra         0($sp)                       # ProcFuncBody
                    li          $t0         0                              
                    sw          $t0         _i                             
L2:                                                                      # ProcFor
                    lw          $t1         _i                             
                    li          $t2         5                              
                    bge         $t1         $t2         L1                 
                    lw          $t1         _i                             
                    li          $v0         1                              
                    move        $a0         $t1                            
                    syscall                                                
                    li          $t1         10                             
                    li          $v0         11                             
                    move        $a0         $t1                            
                    syscall                                                
                    lw          $t2         _i                             
                    li          $t1         1                              
                    add         $t3         $t2         $t1                
                    sw          $t3         _i                             
                    b           L2                                       # ProcFor
L1:                                                                      # ProcFor
_main_ret:                                                                 
                    lw          $ra         0($sp)                       # ProcFuncBody
                    addu        $sp         $sp         4                # ProcFuncBody
                    jr          $ra                                      # func return
                    .data                                                # FinishSemantics
_i:                 .word       0                                          
