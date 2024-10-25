.global gpu_open
.type gpu_open, %function
.global gpu_close
.type   gpu_close, %function
.global wbm
.type wbm, %function
.global wbr_bg
.type wbr_bg, %function
.global wbr_sp
.type wbr_sp, %function
.global wsm
.type wsm, %function
.global dp_triangle
.type dp_triangle, %function
.global dp_square
.type dp_square, %function
.global set_hex
.type set_hex, %function
.global read_keys
.type read_keys, %function

@ Constants
.equ data_b, 0x70
.equ data_a, 0x80
.equ wrreg,  0xc0

@ Mapeia a memória e sava seu endereço
gpu_open:
    sub sp, sp, #28
    str r1, [sp, #24]
    str r2, [sp, #20]
    str r3, [sp, #16]
    str r4, [sp, #12]
    str r5, [sp, #8]
    str r7, [sp, #4]
    str r0, [sp, #0]
    
    ldr r0, =devmem
    mov r1, #2 @ leitura e escrita
    mov r2, #0 @ file mode, necessário apenas se for criar o arquivo
    mov r7, #5 @ system call: open
    svc 0
    
    ldr r1, =fd
    str r0, [r1]
    
    mov r4, r0    @ fd
    mov r0, #0    @ base_address
    mov r1, #4096 @ page_size
    mov r2, #3    @ read and write
    mov r3, #1    @ map_shared
    ldr r5, =fpga_bridge @ offset
    ldr r5, [r5]
    mov r7, #192  @ system call: mmap2
    svc 0

    ldr r1, =mapped_address
    str r0, [r1]
    
    ldr r1, [sp, #24]
    ldr r2, [sp, #20]
    ldr r3, [sp, #16]
    ldr r4, [sp, #12]
    ldr r5, [sp, #8]
    ldr r7, [sp, #4]
    ldr r0, [sp, #0]

    add sp, sp, #28
    
    bx lr


@ desmapeia a memoria e fecha o /dev/mem
gpu_close:

  sub sp, sp, #12
  str r0, [sp, #8]
  str r1, [sp, #4]
  str r7, [sp, #0]

  ldr r0, =mapped_address
  ldr r0, [r0]
  mov r1, #4096 @ tamanho da página mapeada
  mov r7, #91   @ system call: munmap
  svc 0
  
  ldr r0, =fd
  ldr r0, [r0]
  mov r7, #6   @ system call: close
  svc 0
  
  ldr r0, [sp, #8]
  ldr r1, [sp, #4]
  ldr r7, [sp, #0]

  add sp, sp, #12

  bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@ GPU INSTRUCTIONS @@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@ checa se a fila está cheia
check_fifo:  
  sub sp, sp, #8 
  str r0, [sp, #4]
  str r1, [sp, #0]
  
  ldr r0, =mapped_address
  ldr r0, [r0]
checking:
  ldr r1, [r0, #0xb0] 
  CMP r1, #1
  beq checking

  ldr r0, [sp, #4]
  ldr r1, [sp, #0]
  add sp, sp, #8 
  bx lr
@----------------------------------------------@

@ Manda a instrução wbm para a gpu
@ Argumentos:
@ r0 -> bgr (blue, green, red) code
@ r1 -> Endereço do bloco 14 bits
wbm:
  sub sp, sp, #12
  str r0, [sp, #8]
  str r1, [sp, #4]
  str r2, [sp, #0]
    
  ldr r2, =mapped_address
  ldr r2, [r2]
  
  lsl r1, r1, #4
  add r1, r1, #0b0010

  str r0, [r2, #data_b] @ inserindo bgr no dataB
  str r2, [r2, #data_a] @ inserindo memoria e upcode no dataA
  
  sub sp, sp, #4
  str lr, [sp, #0]
  bl check_fifo
  ldr lr, [sp, #0]
  add sp, sp, #4

  mov r1, #1
  str r1, [r2, #wrreg] @ ativando wrreg
  mov r1, #0
  str r1, [r2, #wrreg] @ desativando wrreg

  ldr r0, [sp, #8]
  ldr r1, [sp, #4]
  ldr r2, [sp, #0]
  add sp, sp, #12 

  bx lr

@-----------------------------------------------------------@
@-----------------------------------------------------------@
@ Argumentos:
@ r0 -> Valor BGR
wbr_bg:
  sub sp, sp, #8 
  str r0, [sp, #4]
  str r1, [sp, #0]

  ldr r1, =mapped_address
  ldr r1, [r1]

  str r0, [r1, #data_b]
  mov r1, #0 @ upcode + registrador (ambos são 0)
  str r1, [r0, #data_a]
  mov r1, #1

  sub sp, sp, #4
  str lr, [sp, #0]
  bl check_fifo
  ldr lr, [sp, #0]
  add sp, sp, #4

  str r1, [r1, #wrreg]
  mov r1, #0
  str r1, [r1, #wrreg]

  ldr r0, [sp, #4]
  ldr r1, [sp, #0]
  add sp, sp, #4

  bx lr

@ Mostra um sprite na tela
@ Argumentos:
@ r0, sp (habilita o sprite) 1 bit
@ r1, pos_x_y 30 bits (só da pra passar até o r3 como parâmetro)
@ r2, offset (escolha do sprite na memoria) 9 bits
@ r3, registrador (layer de exibição) 5 bits quanto mais próximo de 0 mais alto
wbr_sp:

  sub sp, sp, #24
  str r0, [sp, #20]
  str r1, [sp, #16]
  str r2, [sp, #12]
  str r3, [sp, #8]
  str r4, [sp, #4]

  ldr r4, =mapped_address
  ldr r4, [r4]
  
  lsl r0, r0, #20 @ adicionando pos x_y
  add r0, r0, r1
  lsl r0, r0, #9 @ adicionando offset
  add r0, r0, r2
  str r0, [r4, #data_b]
  
  lsl r0, r3, #4 @ adicionando upcode
  str r0, [r4, #data_a]
  
  sub sp, sp, #4
  str lr, [sp, #0]
  bl check_fifo
  ldr lr, [sp, #0]
  add sp, sp, #4

  mov r0, #1
  str r0, [r4, #wrreg]
  mov r0, #0
  str r0, [r4, #wrreg]
  
  ldr r0, [sp, #20]
  ldr r1, [sp, #16]
  ldr r2, [sp, #12]
  ldr r3, [sp, #8]
  ldr r4, [sp, #4]
  add sp, sp, #24

  bx lr

@ ------------------------------------------------------ @
@ ------------------------------------------------------ @

@ Altera um pixel de um sprite na memória
@ Argumentos:
@ r0 -> Endereço a ser acessado
@ r1 -> Valor BGR
wsm:

  sub sp, sp, #12
  str r2, [sp, #8]
  str r1, [sp, #4]
  str r0, [sp, #0]
  
  ldr r2, =mapped_address
  ldr r2, [r2]

  lsl r0, r0, #4
  add r0, r0, #0b0001
  str r0, [r2, #data_a]

  str r1, [r2, #data_b]

  sub sp, sp, #4
  str lr, [sp, #0]
  bl check_fifo
  ldr lr, [sp, #0]
  add sp, sp, #4

  mov r1, #1
  str r1, [r2, #wrreg]
  mov r1, #0
  str r1, [r2, #wrreg]

  ldr r2, [sp, #8]
  ldr r1, [sp, #4]
  ldr r0, [sp, #0]
  add sp, sp, #12

  bx lr

@ ------------------------------------------------------ @
@ ------------------------------------------------------ @
@ Argumentos:
@ r0 -> Código BGR
@ r1 -> Tamanho
@ r2 -> Ponto de referência Y_X
@ r3 -> endereço de destino
dp_triangle:
  sub sp, sp, #24
  str r0, [sp, #20]
  str r1, [sp, #16]
  str r2, [sp, #12]
  str r3, [sp, #8]
  str r4, [sp, #4]
  
  mov r4, #1 @ adicionando forma

  lsl r4, r4, #9 
  add r0, r4, r0 @ adicionando cor 

  lsl r0, r0, #4
  add r0, r0, r1 @ adicionando o tamanho

  lsl r0, r0, #18
  add r0, r0, r3 @ adicionando ref_point y_x

  ldr r4, =mapped_address 
  ldr r4, [r4]
  str r0, [r4, #data_b]
  
  lsl r0, r3, #4 
  add r0, r0, #0b0011
  str r0, [r4, #data_a]
  
  sub sp, sp, #4
  str lr, [sp, #0]
  bl check_fifo
  ldr lr, [sp, #0]
  add sp, sp, #4

  mov r0, #1
  str r0, [r4, #wrreg]
  mov r0, #0
  str r0, [r4, #wrreg]

  ldr r0, [sp, #20]
  ldr r1, [sp, #16]
  ldr r2, [sp, #12]
  ldr r3, [sp, #8]
  ldr r4, [sp, #4]
  add sp, sp, #24

  bx lr


@ ------------------------------------------------------ @
@ Argumentos:
@ r0 -> Código BGR
@ r1 -> Tamanho
@ r2 -> Ponto de referência Y_X
@ r3 -> endereço de destino da instrução, quanto mais proximos a 0 mais abaixo ficam
dp_square:
  sub sp, sp, #24
  str r0, [sp, #20]
  str r1, [sp, #16]
  str r2, [sp, #12]
  str r3, [sp, #8]
  str r4, [sp, #4]
  
  mov r4, #0 @ adicionando forma

  lsl r4, r4, #9 
  add r0, r4, r0 @ adicionando cor 

  lsl r0, r0, #4
  add r0, r0, r1 @ adicionando o tamanho

  lsl r0, r0, #18
  add r0, r0, r3 @ adicionando ref_point y_x

  ldr r4, =mapped_address 
  ldr r4, [r4]
  str r0, [r4, #data_b]
  
  lsl r0, r3, #4
  add r0, r0, #0b0011
  str r0, [r4, #data_a]
  
  sub sp, sp, #4
  str lr, [sp, #0]
  bl check_fifo
  ldr lr, [sp, #0]
  add sp, sp, #4

  mov r0, #1
  str r0, [r4, #wrreg]
  mov r0, #0
  str r0, [r4, #wrreg]

  ldr r0, [sp, #20]
  ldr r1, [sp, #16]
  ldr r2, [sp, #12]
  ldr r3, [sp, #8]
  ldr r4, [sp, #4]
  add sp, sp, #24

  bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@ Periféricos @@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@ Mostra valores no display de 7 segmentos
@ argumentos:
@ r0 -> 16bit d5 e d4
@ r1 -> 32bits d3, d2, d1, d0
set_hex:
  sub sp, sp, #12
  str r0, [sp, #8]
  str r1, [sp, #4]
  str r2, [sp, #0]

  ldr r2, =mapped_address
  ldr r2, [r2]
  strb r1, [r2, #0x60]
  lsr r1, r1, #8
  strb r1, [r2, #0x50]
  lsr r1, r1, #8
  strb r1, [r2, #0x40]
  lsr r1, r1, #8
  strb r1, [r2, #0x30]
  
  strb r0, [r2, #0x20]
  lsr r0, r0, #8
  strb r0, [r2, #0x20]

  ldr r0, [sp, #8]
  ldr r1, [sp, #4]
  ldr r2, [sp, #0]
  add sp, sp, #12
  bx lr


@ lê os botões pressionados e retorna em r0  
read_keys:
  sub sp, sp, #4
  str r1, [sp, #0]

  ldr r1, =mapped_address
  ldr r1, [r1]
  ldr r0, [r1, #0x0]

  ldr r1, [sp, #0]
  add sp, sp, #4
  
  bx lr


.data
  devmem: .asciz "/dev/mem"
  fpga_bridge: .word 0xff200 @ já dividido por 4Kb
  mapped_address: .space 4
  fd: .space 4

