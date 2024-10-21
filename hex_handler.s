.global hex_handler
.type hex_handler, %function


@ Receives a decimal number in R0 the returns this number 
@ in 8 bit number representable in hex display form, also in R0
dec_to_hex:
  sub sp, sp, #4
  str r1, [sp, #0]
  
  ldr r1, =hex_numbers1
  ldr r0, [r1, r0]

  ldr r1, [sp, #0]
  add sp, sp, #4
  bx lr

@ Receives the mapped address in R0 and 
hex_handler:


.data
@ checar qual desses funciona na placa, não sei a ordem dos segmentos
@ ATENÇÃO: não mudar a ordem desses valores, ela faz parte da lógica da função dec_to_hex
hex_numbers1: .word 0b01000000, 0b01111001, 0b00100100, 0b00110000, 0b00011001, 0b00010010, 0b00000010, 0b00111000, 0b00000000, 0b00010000
hex_numbers0: .word 0b10111111, 0b10000110, 0b11011011, 0b11001111, 0b11100110, 0b11101101, 0b11111101, 0b11000111, 0b11111111, 0b11101111
