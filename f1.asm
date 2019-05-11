                    .text                                                # FinishSemantics
                    .globl      __start                                  # FinishSemantics
__start:                                                                 # FinishSemantics
                    jal         _main                                    # FinishSemantics
                    li          $v0         10                           # FinishSemantics
                    syscall                                              # FinishSemantics
_main:                                                                   # func entry
                    subu        $sp         $sp         4                # ProcFuncBody
                    sw          $ra         0($sp)                       # ProcFuncBody
L2:                                                                      # ProcWhile
                    lw          $t0         _i                             
                    li          $t1         5                              
                    bge         $t0         $t1         L1                 
                    lw          $t0         _i                             
                    li          $v0         1                              
                    move        $a0         $t0                            
                    syscall                                                
                    li          $t0         10                             
                    li          $v0         11                             
                    move        $a0         $t0                            
                    syscall                                                
                    lw          $t1         _i                             
                    li          $t0         1                              
                    add         $t2         $t1         $t0                
                    sw          $t2         _i                             
                    b           L2                                       # ProcWhile
L1:                                                                      # ProcWhile
                    lw          $t0         _i                             
                    li          $v0         1                              
                    move        $a0         $t0                            
                    syscall                                                
                    li          $t0         10                             
                    li          $v0         11                             
                    move        $a0         $t0                            
                    syscall                                                
_main_ret:                                                                 
                    lw          $ra         0($sp)                       # ProcFuncBody
                    addu        $sp         $sp         4                # ProcFuncBody
                    jr          $ra                                      # func return
                    .data                                                # FinishSemantics
_i:                 .word       0                                          
_b:                 .word       1                                          
