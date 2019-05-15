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
L6:                                                                      # ProcFor
                    lw          $t1         _i                             
                    li          $t2         5                              
                    bge         $t1         $t2         L1                 
                    li          $v0         5                            # read int syscall
                    syscall                                                
                    move        $t1         $v0                            
                    sw          $t1         _x                             
                    li          $v0         4                              
                    la          $a0         L2                             
                    syscall                                                
                    lw          $t1         _x                             
                    li          $v0         1                              
                    move        $a0         $t1                            
                    syscall                                                
                    li          $v0         4                              
                    la          $a0         L3                             
                    syscall                                                
                    lw          $t1         _x                             
                    li          $t2         0                              
                    bge         $t1         $t2         L4                 
                    li          $t1         -1                             
                    lw          $t2         _x                             
                    mul         $t3         $t1         $t2                
                    sw          $t3         _x                             
                    b           L5                                       # ProcTuri
L4:                                                                      # ProcTuri
                    lw          $t1         _x                             
                    sw          $t1         _x                             
L5:                                                                      # ProcTuri
                    lw          $t1         _x                             
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
                    b           L6                                       # ProcFor
L1:                                                                      # ProcFor
_main_ret:                                                                 
                    lw          $ra         0($sp)                       # ProcFuncBody
                    addu        $sp         $sp         4                # ProcFuncBody
                    jr          $ra                                      # func return
                    .data                                                # FinishSemantics
_i:                 .word       0                                          
_x:                 .word       0                                          
L2:                 .asciiz     "|"                                        
                    .align      2                                          
L3:                 .asciiz     "| = "                                     
                    .align      2                                          
