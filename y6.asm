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
                    la          $a0         L6                             
                    syscall                                                
                    li          $v0         5                            # read int syscall
                    syscall                                                
                    move        $t0         $v0                            
                    sw          $t0         _nn                            
L14:                                                                     # ProcWhile
                    lw          $t0         _nn                            
                    li          $t1         0                              
                    beq         $t0         $t1         L7                 
                    jal         _isPrime                                   
                    li          $v0         4                              
                    la          $a0         L8                             
                    syscall                                                
                    lw          $t0         _ret                           
                    bne         $t0                     L9                 
                    li          $v0         4                              
                    la          $a0         L10                            
                    syscall                                                
                    b           L12                                      # ProcIfElse
L9:                                                                      # ProcIfElse
                    li          $v0         4                              
                    la          $a0         L11                            
                    syscall                                                
L12:                                                                     # ProcIfElse
                    li          $v0         4                              
                    la          $a0         L13                            
                    syscall                                                
                    li          $v0         5                            # read int syscall
                    syscall                                                
                    move        $t0         $v0                            
                    sw          $t0         _nn                            
                    b           L14                                      # ProcWhile
L7:                                                                      # ProcWhile
                    li          $v0         4                              
                    la          $a0         L15                            
                    syscall                                                
_main_ret:                                                                 
                    lw          $ra         0($sp)                       # ProcFuncBody
                    addu        $sp         $sp         4                # ProcFuncBody
                    jr          $ra                                      # func return
_isPrime:                                                                # func entry
                    subu        $sp         $sp         4                # ProcFuncBody
                    sw          $ra         0($sp)                       # ProcFuncBody
                    li          $t0         -1                             
                    sw          $t0         _r                             
                    li          $t0         2                              
                    sw          $t0         _d                             
                    li          $t0         0                              
                    sw          $t0         _c                             
                    lw          $t0         _nn                            
                    li          $t1         2                              
                    div         $t2         $t0         $t1                
                    sw          $t2         _n                             
L3:                                                                      # ProcWhile
                    lw          $t0         _d                             
                    lw          $t1         _n                             
                    bgt         $t0         $t1         L1                 
                    lw          $t0         _nn                            
                    lw          $t1         _d                             
                    div         $t2         $t0         $t1                
                    lw          $t0         _d                             
                    mul         $t1         $t2         $t0                
                    sw          $t1         _a                             
                    lw          $t0         _a                             
                    lw          $t1         _nn                            
                    bne         $t0         $t1         L2                 
                    lw          $t0         _c                             
                    li          $t1         1                              
                    add         $t2         $t0         $t1                
                    sw          $t2         _c                             
L2:                                                                        
                    lw          $t0         _d                             
                    li          $t1         1                              
                    add         $t2         $t0         $t1                
                    sw          $t2         _d                             
                    b           L3                                       # ProcWhile
L1:                                                                      # ProcWhile
                    lw          $t0         _c                             
                    li          $t1         0                              
                    bne         $t0         $t1         L4                 
                    sw                      _ret                           
                    b           L5                                       # ProcIfElse
L4:                                                                      # ProcIfElse
                    sw                      _ret                           
L5:                                                                      # ProcIfElse
_isPrime_ret:                                                              
                    lw          $ra         0($sp)                       # ProcFuncBody
                    addu        $sp         $sp         4                # ProcFuncBody
                    jr          $ra                                      # func return
                    .data                                                # FinishSemantics
_d:                 .word       0                                          
_n:                 .word       0                                          
_r:                 .word       0                                          
_nn:                .word       0                                          
_ret:               .word       0                                          
_a:                 .word       0                                          
_c:                 .word       0                                          
L6:                 .asciiz     "> "                                       
L8:                 .asciiz     "number ") put(nn) put(" is "                               
L10:                .asciiz     "prime.\n"                                 
L11:                .asciiz     "not prime. It has ") put(c) put(" divisors.\n"                               
L13:                .asciiz     "> "                                       
L15:                .asciiz     "done\n"                                   
