.global _start

_start:
  @ abrir /dev/mem
  ldr r0, =devmem
  mov r1, #2
  mov r2, #0
  mov r7, #5
  svc 0

  ldr r9, =fpga_bridge
  ldr r5, [r9]
  mov r4, r0
  
  mov r0, #0
  mov r1, #4096 @ Tamanho da página
  mov r2, #3    @ Leitura e escrita
  mov r3, #1    @ map_shared
  mov r7, #192  @ syscall mmap2
  svc 0
  
  cmp r0, #0
  bgt error

  mov r10, r0

  mov r1, #0
  strb r1, [r10, #0xc0]


  
  mov r1, #0b000111
  strb r1, [r10, #0x70] 
  
  mov r1, #0b1110010
  str r1, [r10, #0x90]

  mov r1, #1
  strb r1, [r10, #0xc0]
  
  mov r0, #1
  ldr r1, =success
  mov r2, #8
  mov r7, #4
  svc 0
  
  mov r7, #1
  mov r0, #0
  svc 0

error:
  mov r0, #1
  ldr r1, =failed
  mov r2, #7
  mov r7, #4
  svc 0

  mov r7, #1
  mov r0, #1
  svc 0





.data
  devmem:      .asciz "/dev/mem"
  success:     .ascii "success\n"
  failed:      .ascii "failed\n"
  fpga_bridge: .word 0xff200
