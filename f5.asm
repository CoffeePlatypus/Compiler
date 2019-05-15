                    .text                                                # FinishSemantics
                    .globl      __start                                  # FinishSemantics
__start:                                                                 # FinishSemantics
                    jal         _main                                    # FinishSemantics
                    li          $v0         10                           # FinishSemantics
                    syscall                                              # FinishSemantics
_main:                                                                   # func entry
                    subu        $sp         $sp         4                # ProcFuncBody
                    sw          $ra         0($sp)                       # ProcFuncBody
                    la          $t0         _arr                         # ProcDeclArray
                    li          $t1         0                              
                    sw          $t1         _i                             
L4:                                                                      # ProcFor
                    lw          $t2         _i                             
                    li          $t3         6                              
                    bge         $t2         $t3         L1                 
                    lw          $t2         _i                             
                    li          $v0         1                              
                    move        $a0         $t2                            
                    syscall                                                
                    sll         $t2         $t2         2                # i*4
                    add         $t2         $t2         $t0              # addr A[i]
                    sw          $t3         0($t2)                       # ProcAssignArray
                    li          $v0         4                              
                    la          $a0         L2                             
                    syscall                                                
                    lw          $t4         _i                             
                    sll         $t4         $t4         2                # ProcAccessArray
                    add         $t4         $t4         $t0              # ProcAccessArray
                    lw          $t4         0($t4)                       # ProcAccessArray
                    li          $v0         1                              
                    move        $a0         $t4                            
                    syscall                                                
                    li          $v0         4                              
                    la          $a0         L3                             
                    syscall                                                
                    lw          $t5         _i                             
                    li          $t4         1                              
                    add         $t6         $t5         $t4                
                    sw          $t6         _i                             
                    b           L4                                       # ProcFor
L1:                                                                      # ProcFor
                    li          $v0         4                              
                    la          $a0         L5                             
                    syscall                                                
_main_ret:                                                                 
                    lw          $ra         0($sp)                       # ProcFuncBody
                    addu        $sp         $sp         4                # ProcFuncBody
                    jr          $ra                                      # func return
                    .data                                                # FinishSemantics
_i:                 .word       0                                          
_arr:               .space      24                                         
                    .align      2                                        # align? This is how we did it in 270
L2:                 .asciiz     ":"                                        
L3:                 .asciiz     "\n"                                       
L5:                 .asciiz     "done"                                     
