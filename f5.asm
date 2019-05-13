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
                    sll         $t1         $t1         2                # i*4
                    add         $t1         $t1         $t0              # addr A[i]
                    sw          $t2         0($t1)                       # ProcAssignArray
                    li          $v0         4                              
                    la          $a0         L1                             
                    syscall                                                
_main_ret:                                                                 
                    lw          $ra         0($sp)                       # ProcFuncBody
                    addu        $sp         $sp         4                # ProcFuncBody
                    jr          $ra                                      # func return
                    .data                                                # FinishSemantics
_i:                 .word       0                                          
L1:                 .asciiz     "done"                                     
_arr:               .space      20                                         
