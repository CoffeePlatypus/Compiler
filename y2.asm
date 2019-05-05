                    .text                                                # FinishSemantics
                    .globl      __start                                  # FinishSemantics
__start:                                                                 # FinishSemantics
                    jal         _main                                    # FinishSemantics
                    li          $v0         10                           # FinishSemantics
                    syscall                                              # FinishSemantics
_main:                                                                   # func entry
                    subu        $sp         $sp         4                # ProcFuncBody
                    sw          $ra         0($sp)                       # ProcFuncBody
                    li          $t0         62                             
                    li          $v0         11                             
                    move        $a0         $t0                            
                    syscall                                                
                    li          $v0         5                            # read int syscall
                    syscall                                                
                    move        $t0         $v0                            
                    sw          $t0         _x                             
                    li          $t0         62                             
                    li          $v0         11                             
                    move        $a0         $t0                            
                    syscall                                                
                    li          $v0         5                            # read int syscall
                    syscall                                                
                    move        $t0         $v0                            
                    sw          $t0         _y                             
                    lw          $t0         _y                             
                    lw          $t1         _x                             
                    lw          $t2         _x                             
                    mul         $t3         $t1         $t2                
                    add         $t1         $t0         $t3                
                    sw          $t1         _r                             
                    li          $t0         114                            
                    li          $v0         11                             
                    move        $a0         $t0                            
                    syscall                                                
                    li          $t0         61                             
                    li          $v0         11                             
                    move        $a0         $t0                            
                    syscall                                                
                    lw          $t0         _r                             
                    li          $v0         1                              
                    move        $a0         $t0                            
                    syscall                                                
                    li          $t0         10                             
                    li          $v0         11                             
                    move        $a0         $t0                            
                    syscall                                                
                    lw          $t0         _y                             
                    lw          $t1         _x                             
                    add         $t2         $t0         $t1                
                    lw          $t0         _x                             
                    mul         $t1         $t2         $t0                
                    sw          $t1         _r                             
                    li          $t0         114                            
                    li          $v0         11                             
                    move        $a0         $t0                            
                    syscall                                                
                    li          $t0         61                             
                    li          $v0         11                             
                    move        $a0         $t0                            
                    syscall                                                
                    lw          $t0         _r                             
                    li          $v0         1                              
                    move        $a0         $t0                            
                    syscall                                                
                    li          $t0         10                             
                    li          $v0         11                             
                    move        $a0         $t0                            
                    syscall                                                
                    lw          $t0         _y                             
                    lw          $t1         _x                             
                    lw          $t2         _x                             
                    mul         $t3         $t1         $t2                
                    add         $t1         $t0         $t3                
                    sw          $t1         _r                             
                    li          $t0         114                            
                    li          $v0         11                             
                    move        $a0         $t0                            
                    syscall                                                
                    li          $t0         61                             
                    li          $v0         11                             
                    move        $a0         $t0                            
                    syscall                                                
                    lw          $t0         _r                             
                    li          $v0         1                              
                    move        $a0         $t0                            
                    syscall                                                
                    li          $t0         10                             
                    li          $v0         11                             
                    move        $a0         $t0                            
                    syscall                                                
                    lw          $t0         _x                             
                    lw          $t1         _x                             
                    mul         $t2         $t0         $t1                
                    lw          $t0         _y                             
                    add         $t1         $t2         $t0                
                    sw          $t1         _r                             
                    li          $t0         114                            
                    li          $v0         11                             
                    move        $a0         $t0                            
                    syscall                                                
                    li          $t0         61                             
                    li          $v0         11                             
                    move        $a0         $t0                            
                    syscall                                                
                    lw          $t0         _r                             
                    li          $v0         1                              
                    move        $a0         $t0                            
                    syscall                                                
                    li          $t0         10                             
                    li          $v0         11                             
                    move        $a0         $t0                            
                    syscall                                                
                    li          $t1         84                             
                    li          $v0         11                             
                    move        $a0         $t1                            
                    syscall                                                
                    li          $t1         32                             
                    li          $v0         11                             
                    move        $a0         $t1                            
                    syscall                                                
                    li          $t2         70                             
                    li          $v0         11                             
                    move        $a0         $t2                            
                    syscall                                                
                    li          $t2         10                             
                    li          $v0         11                             
                    move        $a0         $t2                            
                    syscall                                                
_main_ret:                                                                 
                    lw          $ra         0($sp)                       # ProcFuncBody
                    addu        $sp         $sp         4                # ProcFuncBody
                    jr          $ra                                      # func return
                    .data                                                # FinishSemantics
_r:                 .word       0                                          
_x:                 .word       0                                          
_y:                 .word       0                                          
