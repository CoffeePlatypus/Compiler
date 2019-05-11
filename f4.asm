                    .text                                                # FinishSemantics
                    .globl      __start                                  # FinishSemantics
__start:                                                                 # FinishSemantics
                    jal         _main                                    # FinishSemantics
                    li          $v0         10                           # FinishSemantics
                    syscall                                              # FinishSemantics
_main:                                                                   # func entry
                    subu        $sp         $sp         4                # ProcFuncBody
                    sw          $ra         0($sp)                       # ProcFuncBody
L7:                                                                      # ProcLoop
                    li          $v0         4                              
                    la          $a0         L1                             
                    syscall                                                
L4:                                                                      # ProcLoop
                    li          $v0         4                              
                    la          $a0         L2                             
                    syscall                                                
                    b           L3                                       # ProcBreak
                    b           L4                                       # ProcLoop
L3:                                                                      # ProcLoop
                    lw          $t0         _i                             
                    li          $t1         0                              
                    bne         $t0         $t1         L5                 
                    li          $t0         10                             
                    li          $v0         11                             
                    move        $a0         $t0                            
                    syscall                                                
                    b           L6                                       # ProcBreak
L5:                                                                        
                    lw          $t1         _i                             
                    li          $t0         1                              
                    sub         $t2         $t1         $t0                
                    sw          $t2         _i                             
                    b           L7                                       # ProcLoop
L6:                                                                      # ProcLoop
_main_ret:                                                                 
                    lw          $ra         0($sp)                       # ProcFuncBody
                    addu        $sp         $sp         4                # ProcFuncBody
                    jr          $ra                                      # func return
                    .data                                                # FinishSemantics
_i:                 .word       3                                          
L1:                 .asciiz     "meow "                                    
L2:                 .asciiz     "tomato \n"                                
