.global map_memory
.type map_memory, %function
.global unmap_memory
.type   unmap_memory, %function

@ retorna o endereço de memória mapeado, ou 0 em caso de erro
@ também guarda o fd na pilha para retornar
map_memory:
    
    sub sp, sp, #28
    str r1, [sp, #24]
    str r2, [sp, #20]
    str r3, [sp, #16]
    str r4, [sp, #12]
    str r5, [sp, #8]
    str r7, [sp, #4]
    
    ldr r0, =devmem
    mov r1, #2 @ leitura e escrita
    mov r2, #0 @ file mode, necessário apenas se for criar o arquivo
    mov r7, #5 @ system call: open
    svc 0

    str r0, [sp, #0]
    
    mov r4, r0    @ fd
    mov r0, #0    @ base_address
    mov r1, #4096 @ page_size
    mov r2, #3    @ read and write
    mov r3, #1    @ map_shared
    ldr r5, =fpga_bridge @ offset
    ldr r5, [r5]
    mov r7, #192  @ system call: mmap2
    svc 0
    
    ldr r1, [sp, #24]
    ldr r2, [sp, #20]
    ldr r3, [sp, #16]
    ldr r4, [sp, #12]
    ldr r5, [sp, #8]
    ldr r7, [sp, #4]

    add sp, sp, #24
    
    bx lr


@ recebe como argumento o fd e o endereço de memória mapeado
@ r0 -> Endereço mapeado
@ r1 -> fd
unmap_memory:

  sub sp, sp, #16
  str r0, [sp, #12]
  str r1, [sp, #8]
  str r2, [sp, #4]
  str r7, [sp, #0]

  mov r2, r1
  mov r1, #4096 @ tamanho da página mapeada
  mov r7, #91   @ system call: munmap
  svc 0

  mov r0, r2   @ fd
  mov r7, #6   @ system call: close
  svc 0
  
  ldr r0, [sp, #12]
  ldr r1, [sp, #8]
  ldr r2, [sp, #4]
  ldr r7, [sp, #0]

  add sp, sp, #16

  bx lr

.data
  devmem: .asciz "/dev/mem"
  fpga_bridge: .word 0xff200 @ já dividido por 4Kb

