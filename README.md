# Logo
# Sumario

# Geral
Nesse projeto foi desenvolvido uma biblioteca em assembly ARMv7 para a utilização da GPU desenvolvida por Gabriel Sá Barreto, para o seu desenvolvimento foi necessario aparender sobre como a GPU funciona, como se comunicar com ela, e adaptar o jogo de Tetriz desenvolvido no problema anterior para a nova plataforma.

# Requisitos
"Falar aqui o que precisa para rodar os testes e a biblioteca"

A biblioteca produzida teve que cumprir os seguintes requisitos:

- [x] Desenhar sprites 20x20 na tela
- [x] Definir a cor base do background
- [x] Modificar os sprites armazenados na memoria
- [x] Desenhar quadrados e triangulos 20x20 até 160x160 aumentando numa escala de 10x10
- [x] Pintar quadrados 8x8 na tela
- [x] Limpar a tela
- [x] Printar telas inteiras

Além dos seguintes requisitos, foi solicitado que as instruções fossem feitas em Assembly e extendem-se para C.

# Desenvolvimento

# Assembly

O processador consiste num 800MHz Dual-core ARM Cortex-A9 MPCore processor com Assembly ARMv7, e foi desenvolvido nele as seguintes funções em Assembly:

![Header do Assembly](ReadMe_files/header.png)

Vale lembrar que todas as funções após iniciarem a sua execução salvam o contexto dos registradores utilizados para sua execução e restauram esse contexto logo antes do retorno da função para evitar conflitos na execução do problema.

As funções que são instruções da GPU funcionam todas de forma muito semelhante onde os parametros são passados para o buffer Data_B e o opcode e endereço de memoria são enviados para o buffer Data_A, após isso elas aguardam poderem escrever checkando a variavel wrfull == 0 e é enviado um pulso para wrreg permitindo a escrita. Esse processo se repete para as funções wbr_bg, wbr_sp, wbm, wsm, dp_triangle e dp_square




