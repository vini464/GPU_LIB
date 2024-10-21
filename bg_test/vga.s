.global setSpr
.type setSpr, %function
.extern wsm
.extern wbm
.extern wbr_sp
.extern wbr_bg

 
@ Receives the mapped fpga_bridge in R0, and the bitmap vector address in R1
@ Also recieve the memory offset in R2, this is a number less or equal to 32
@ The bitmap vector should be 400 position long with 9 bits on each position
setSpr:
          sub sp, sp, #24
          str r1, [sp, #0]
          str r2, [sp, #4]
          str r3, [sp, #8]
          str r4, [sp, #12]
          str r5, [sp, #16]
          str r6, [sp, #20]
          
          mov r3, r1      @ saving bitmap vector address
          mov r4, r2      @ saving memory offset
          mov r5, #400

          mul r1, r4, r5  @ primeira posição do sprite 
          mov r6, #0
          mov r5, #800
getColor: CMP r6, r5
          beq endSetSpr
          ldr r2, [r3, r6] @ pegando a primeira cor
          @ note que em r0 está a fpga_bridge
          @ em r1 está o endereço do primeiro pixel do sprite
          @ e em r2 está a cor do primeiro pixel
          bl wsm
          add r6, #2 @ próxima posição do vetor de bitmap *pode dar erro aqui*
          add r1, #1 @ proxima posição a ser escrita
          b getColor
endSetSpr: 

          ldr r1, [sp, #0]
          ldr r2, [sp, #4]
          ldr r3, [sp, #8]
          ldr r4, [sp, #12]
          ldr r5, [sp, #16]
          ldr r6, [sp, #20]
          add sp, sp, #24

          bx lr
          
.global showSpr
.type showSpr, %function
@ r0 -> fpga_bridge
@ r1 -> pos_x
@ r2 -> pos_y
@ r3 -> Offset (posição na memoria ID)
@ r4 -> layer < 32
showSpr:
    sub sp, sp, #4    
    str r0, [sp, #0] 
    mov r0, #1
    bl wbr_sp
    
    ldr r0, [sp, #0]
    add sp, sp, #4
    bx lr

    
.global hideSpr
.type hideSpr, %function
@ r0 -> fpga_bridge
@ r1 -> pos_x
@ r2 -> pos_y
@ r3 -> Offset
@ r4 -> layer < 32
hideSpr:
    sub sp, sp, #4    
    str r0, [sp, #0] 
    mov r0, #0
    bl wbr_sp
    
    ldr r0, [sp, #0]
    add sp, sp, #4
    bx lr




.global setBgImg
.type setBgImg, %function



@ Argumentos:
@ R0 -> fpga_bridge
@ R1 -> vetor bitmap (é um vetor de short int (2Bytes cada elemento))
setBgImg:
   @ A tela é dividida em quadrados de 8x8, 80x60(linhas por colunas) total de 4800 blocos 
    sub sp, sp, #28
    str r0, [sp, #0]
    str r1, [sp, #4]
    str r2, [sp, #8]
    str r3, [sp, #12]
    str r4, [sp, #16]
    str r5, [sp, #20]
    str r6, [sp, #24]
   
    mov r5, #2
    mov r3, r1 
    mov r2, #0
    ldr r4, =last_block_address
    ldr r4, [r4]
    
getNewblock:    
    cmp r2, r4
    beq endSetBgImg
    mul r6, r2, r5 
    ldr r1, [r3, r6] @ cor

    @ r0 ok
    @ r1 ok
    @ r2 ok
    bl wbm
    
    add r2, #1
    b getNewblock 
    
endSetBgImg: 
    ldr r0, [sp, #0]
    ldr r1, [sp, #4]
    ldr r2, [sp, #8]
    ldr r3, [sp, #12]
    ldr r4, [sp, #16]
    ldr r5, [sp, #20]
    ldr r6, [sp, #24]
    add sp, sp, #28
  bx lr


.data 
last_block_address: .word 4800
