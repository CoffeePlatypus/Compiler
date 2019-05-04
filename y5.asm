                    .text                                                # FinishSemantics
                    .globl      __start                                  # FinishSemantics
__start:                                                                 # FinishSemantics
                    jal         _main                                    # FinishSemantics
                    li          $v0         10                           # FinishSemantics
                    syscall                                              # FinishSemantics
_main:                                                                   # func entry
                    subu        $sp         $sp         4                # ProcFuncBody
                    sw          $ra         0($sp)                       # ProcFuncBody
                    li          $v0         4                              
                    la          $a0         L1                             
                    syscall                                                
                    li          $v0         5                            # read int syscall
                    syscall                                                
                    move        $t0         $v0                            
                    sw          $t0         _p                             
L11:                                                                     # ProcWhile
                    lw          $t0         _p                             
                    li          $t1         1                              
                    ble         $t0         $t1         L2                 
                    li          $v0         4                              
                    la          $a0         L3                             
                    syscall                                                
                    lw          $t0         _p                             
                    li          $v0         1                              
                    move        $a0         $t0                            
                    syscall                                                
                    li          $v0         4                              
                    la          $a0         L4                             
                    syscall                                                
                    li          $t0         2                              
                    sw          $t0         _d                             
L8:                                                                      # ProcWhile
                    lw          $t0         _d                             
                    lw          $t1         _p                             
                    li          $t2         2                              
                    div         $t3         $t1         $t2                
                    bgt         $t0         $t3         L5                 
                    lw          $t0         _p                             
                    lw          $t1         _d                             
                    div         $t2         $t0         $t1                
                    sw          $t2         _q                             
                    lw          $t0         _q                             
                    lw          $t1         _d                             
                    mul         $t2         $t0         $t1                
                    lw          $t0         _p                             
                    bne         $t2         $t0         L6                 
                    lw          $t0         _d                             
                    li          $v0         1                              
                    move        $a0         $t0                            
                    syscall                                                
                    li          $v0         4                              
                    la          $a0         L7                             
                    syscall                                                
L6:                                                                        
                    lw          $t0         _d                             
                    li          $t1         1                              
                    add         $t2         $t0         $t1                
                    sw          $t2         _d                             
                    b           L8                                       # ProcWhile
L5:                                                                      # ProcWhile
                    li          $v0         4                              
                    la          $a0         L9                             
                    syscall                                                
                    li          $v0         4                              
                    la          $a0         L10                            
                    syscall                                                
                    li          $v0         5                            # read int syscall
                    syscall                                                
                    move        $t0         $v0                            
                    sw          $t0         _p                             
                    b           L11                                      # ProcWhile
L2:                                                                      # ProcWhile
_main_ret:                                                                 
                    lw          $ra         0($sp)                       # ProcFuncBody
                    addu        $sp         $sp         4                # ProcFuncBody
                    jr          $ra                                      # func return
                    .data                                                # FinishSemantics
_d:                 .word       0                                          
_p:                 .word       0                                          
_q:                 .word       0                                          
L1:                 .asciiz     "> "                                       
L3:                 .asciiz     "factors of "                               
L4:                 .asciiz     " are "                                    
L7:                 .asciiz     " "                                        
L9:                 .asciiz     "\n"                                       
L10:                .asciiz     "> "                                       
