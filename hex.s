.global _start

_start:
  ldr r0, =devmem
  mov r1, #2 @ leitura e escrita
  mov r2, #0 @ file mode, necessário apenas se for criar o arquivo

 @ ldr r9, =openat
 @ ldr r7, [r9] 
  mov r7, #5
  svc 0
  
  @ saving fd
  mov r10, r0
  
  ldr r9, =fpga_bridge

  @ mapping memory
  mov r0, #0 
  mov r1, #4096
  mov r2, #3 @ read and write
  mov r3, #1 @ map_shared
  mov r4, r10
  ldr r5, [r9] 
  mov r7, #192 @ syscall mmap2
  svc 0

  cmp r0, #-1
  beq error

  mov r11, r0
  
  mov r1, #0
  strb r1, [r0, #0x10]
  
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
devmem: .asciz "/dev/mem"
success: .ascii "success\n"
failed: .ascii "failed\n"
openat: .word 322
fpga_bridge: .word 0xff200
hex1: .word 0xff200010
