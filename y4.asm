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
                    sw          $t0         _x                             
                    li          $v0         4                              
                    la          $a0         L2                             
                    syscall                                                
                    lw          $t0         _x                             
                    li          $v0         1                              
                    move        $a0         $t0                            
                    syscall                                                
                    li          $v0         4                              
                    la          $a0         L3                             
                    syscall                                                
                    li          $t0         1                              
                    sw          $t0         _fac                           
L5:                                                                      # ProcWhile
                    lw          $t0         _x                             
                    li          $t1         0                              
                    ble         $t0         $t1         L4                 
                    lw          $t0         _fac                           
                    lw          $t1         _x                             
                    mul         $t2         $t0         $t1                
                    sw          $t2         _fac                           
                    lw          $t0         _x                             
                    li          $t1         1                              
                    sub         $t2         $t0         $t1                
                    sw          $t2         _x                             
                    b           L5                                       # ProcWhile
L4:                                                                      # ProcWhile
                    li          $v0         4                              
                    la          $a0         L6                             
                    syscall                                                
                    lw          $t0         _fac                           
                    li          $v0         1                              
                    move        $a0         $t0                            
                    syscall                                                
                    li          $v0         4                              
                    la          $a0         L7                             
                    syscall                                                
_main_ret:                                                                 
                    lw          $ra         0($sp)                       # ProcFuncBody
                    addu        $sp         $sp         4                # ProcFuncBody
                    jr          $ra                                      # func return
                    .data                                                # FinishSemantics
_x:                 .word       0                                          
_fac:               .word       0                                          
L1:                 .asciiz     "> "                                       
L2:                 .asciiz     "x = "                                     
L3:                 .asciiz     "\n"                                       
L6:                 .asciiz     "factorial = "                               
L7:                 .asciiz     "\n"                                       
