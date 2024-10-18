.global wbm
.type wbm, %function

@ argumentos:
@ r0 -> bgr (blue, green, red) code 
@ r1 -> linha  (deve estar entre 0 e 59)
@ r2 -> coluna (deve estar entre 0 e 79)
@ Endereço de memoria (mapeado) da fpga_bridge pela pilha
wbm:
  sub sp, sp, #20
  str r0, [sp, #16]
  str r1, [sp, #12]
  str r2, [sp, #8]
  str r3, [sp, #4]
  str r4, [sp, #0]

  ldr r3, [sp, #20] @ fpga_bridge

  @ find block memory
  mul r4, r1, #80   
  add r4, r4, r2

  @ add upcode
  lsl r4, r4, #4
  add r4, r4, #0b0010

  str r0, [r3, 0x70] @ inserindo bgr no dataB
  str r4, [r3, 0x80] @ inserindo memoria e upcode no dataA
  mov r1, #1
  str r1, [r3, 0xc0] @ ativando wrreg
  mov r1, #0
  str r1, [r3, 0xc0] @ desativando wrreg

  ldr r0, [sp, #16]
  ldr r1, [sp, #12]
  ldr r2, [sp, #8]
  ldr r3, [sp, #4]
  ldr r4, [sp, #0]
  add sp, sp, #24 @ retirando fpga_bridge da pilha

  bx lr

