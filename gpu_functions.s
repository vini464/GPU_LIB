.global wbm
.type wbm, %function

.global wbr_bg
.type wbr_bg, %function

.global wbr_sp
.type wbr_sp, %function

.global wsm
.type wsm, %function

.global dp
.type dp, %function

@ Constants
.equ data_b, 0x70
.equ data_a, 0x80
.equ wrreg,  0xc0


@ argumentos:
@ r0 -> Endereço de memoria (mapeado) da fpga_bridge
@ r1 -> bgr (blue, green, red) code 
@ r2 -> edereço do bloco  14 bits
@
wbm:
  sub sp, sp, #12
  str r0, [sp, #8]
  str r1, [sp, #4]
  str r2, [sp, #0]
  
  lsl r2, r2, #4
  add r2, r2, #0b0010

  str r1, [r0, #data_b] @ inserindo bgr no dataB
  str r2, [r0, #data_a] @ inserindo memoria e upcode no dataA

  mov r1, #1
  str r1, [r0, #wrreg] @ ativando wrreg
  mov r1, #0
  str r1, [r0, #wrreg] @ desativando wrreg

  ldr r0, [sp, #8]
  ldr r1, [sp, #4]
  ldr r2, [sp, #0]
  add sp, sp, #12 

  bx lr

@ ------------------------------------------------------ @

@ Argumentos:
@ r0 -> Endereço de memória mapeado
@ r1 -> Valor BGR
wbr_bg:
  sub sp, sp, #4 
  str r1, [sp, #0]

  ldr r1, [r0, #data_b]
  mov r1, #0 @ upcode + registrador (ambos são 0)
  ldr r1, [r0, #data_a]
  mov r1, #1
  ldr r1, [r0, #wrreg]
  mov r1, #0
  ldr r1, [r0, #wrreg]

  ldr r1, [sp, #0]
  add sp, sp, #4

  bx lr

@ ------------------------------------------------------ @

@ Argumentos: 
@ r0 -> Sp
@ r1 -> Coordenada x
@ r2 -> Coordenada y
@ r3 -> Offset
@ r4 -> Registrador
@ sp -> Endereço mapeado
@ já libera o espaço da pilha usado pelo endereço mapeado
wbr_sp:

  sub sp, sp, #8
  str r0, [sp, #4]
  str r5, [sp, #0]

  ldr r5, [sp, #8]
  
  lsl r0, r0, #10
  add r0, r0, r1
  lsl r0, r0, #10
  add r0, r0, r2
  lsl r0, r0, #9
  add r0, r0, r3
  str r0, [r5, #data_b]
  
  lsl r0, r4, #4
  str r0, [r5, #data_a]

  mov r0, #1
  str r0, [r5, #wrreg]
  mov r0, #0
  str r0, [r5, #wrreg]
  
  ldr r0, [sp, #4]
  ldr r5, [sp, #0]
  sub sp, sp, #12 

  bx lr

@ ------------------------------------------------------ @

@ Argumentos:
@ r0 -> Endereço de memória mapeado
@ r1 -> Endereço a ser acessado
@ r2 -> Valor BGR
wsm:

  sub sp, sp, #4
  str r1, [sp, #0]

  lsl r1, r1, #4
  add r1, r1, #0b0001
  str r1, [r0, #data_a]

  str r2, [r0, #data_b]

  mov r1, #1
  str r1, [r0, #wrreg]
  mov r1, #0
  str r1, [r0, #wrreg]

  ldr r1, [sp, #0]
  add sp, sp, #4

  bx lr

@ ------------------------------------------------------ @
  
  

@ Argumentos:
@ r0 -> Forma
@ r1 -> Código BGR
@ r2 -> Tamanho
@ r3 -> Ponto de referência X
@ r4 -> Ponto de referência Y
@ sp -> endereço de destino -> endereço mapeado
@ a pilha é limpa durante a execução
dp:
  sub sp, sp, #12
  str r0, [sp, #8]
  str r5, [sp, #4]
  str r6, [sp, #0]

  ldr r5, [sp, #12]
  ldr r6, [sp, #16]

  lsl r0, r0, #9
  add r0, r0, r1 @ adicionando a cor
  lsl r0, r0, #4
  add r0, r0, r2 @ adicionando o tamanho
  lsl r0, r0, #9
  add r0, r0, r3 @ adicionando ref_point x
  lsl r0, r0, #9
  add r0, r0, r4 @ adicionando ref_point y
  str r0, [r6, #data_b]
  
  lsl r0, r6, #4
  add r0, r0, #0b0011
  str r0, [r6, #data_a]
  
  mov r0, #1
  str r0, [r6, #wrreg]
  mov r0, #0
  str r0, [r6, #wrreg]

  ldr r0, [sp, #8]
  ldr r5, [sp, #4]
  ldr r6, [sp, #0]
  sub sp, sp, #20
  
  bx lr
