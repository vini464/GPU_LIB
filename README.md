<p align="center">
  <img src="ReadMe_files/inicio.gif" alt="Descrição do GIF" width="50%">
</p>

# Sumário

## Geral
Este projeto trata-se do desenvolvimento de uma biblioteca com funções gráficas para a GPU projetada e implementada por Gabriel Sá Barreto no kit de desenvolvimento DE1-SoC. Para realização do projeto, foi necessário aprender como a GPU funciona, como realizar a comunicação entre o HPS e a FPGA, e adaptar o jogo [Tetriz](https://github.com/vini464/TETRIS_ON_DE1-SOC) para a nova plataforma.

## Requisitos

A biblioteca produzida teve que cumprir os seguintes requisitos:

- Conter todas as funções do processador gráfico, são elas:
    - Desenhar sprites na tela;
    - Modificar/Inserir sprites na memória da GPU;
    - Mudar a cor base do background;
    - Desenhar quadrados de tamanho fixo (8x8 pixels) na tela;
    - Desenhar polígonos (quadrados ou triângulos) de tamanhos variáveis na tela.
- Todo o código da biblioteca deverá ser escrito na linguagem assembly.
- Implementar a parte gráfica do jogo [Tetriz](https://github.com/vini464/TETRIS_ON_DE1-SOC) utilizando a biblioteca desenvolvida.

## Biblioteca

O processador consiste num 800MHz Dual-core ARM Cortex-A9 MPCore processor com Assembly ARMv7, e foi desenvolvido nele as seguintes funções em Assembly:

<p align="center">
  <img src="ReadMe_files/header.png" alt="Descrição do GIF" width="50%">
</p>


Vale lembrar que todas as funções após iniciarem a sua execução salvam o contexto dos registradores utilizados para sua execução e restauram esse contexto logo antes do retorno da função para evitar conflitos na execução do problema.

As funções que são instruções da GPU funcionam todas de forma muito semelhante onde os parametros são passados para o buffer Data_B e o opcode e endereço de memoria são enviados para o buffer Data_A, após isso elas aguardam poderem escrever checkando a variavel wrfull == 0 e é enviado um pulso para wrreg permitindo a escrita. Esse processo se repete para as funções wbr_bg, wbr_sp, wbm, wsm, dp_triangle e dp_square

# C

Em C foram desenvolvidos algumas funções ** Fala aqui sobre C Everton **

# Tetriz

** Fala aqui sobre o que precisou enfiar pro jogo funcionar **

# Como executar

Para rodarmos o código, precisamos cumprir alguns requisitos que são:

1. Uma Placa DE1-SoC com a GPU de Gabriel Sá Barreto instalada.
2. Linux instalados.
3. A biblioteca do IntelFPGAUP Accelerometer instalada.
4. Um monitor com entrada VGA.
5. Um cabo de Ethernet
6. Um computador com terminal para acessar a placa via protocolo SSH.

Com todos os requisitos cumpridos basta transferir os arquivos desse repositório para a placa. E então entrar na pasta do projeto e rodar o comando `make game`.

Caso sua placa tenha acesso a internet, e o git instalado, você pode digitar o seguinte comando: 
Clone o repositório:
```
git clone https://github.com/vini464/GPU_LIB.git && cd GPU_LIB
make game
```
> **Obs: Você precisa de privilégio de administrador para rodar o jogo.**


# Testes

Para reproduzir os testes você precisará do script jarvis.c.

### Teste de BackGround

Esse teste consiste em chamar a função wbr_bg e definir uma cor para ser mostrada, seu resultado deve se parecer com isso:

<p align="center">
  <img src="ReadMe_files/Background.gif" alt="Descrição do GIF" width="25%">
</p>

### Teste de Sprite

Esse teste consiste em chamar a função para colocar todos os sprites na tela, o seu resultado deve se parecer com isso:

<p align="center">
  <img src="ReadMe_files/wbr_sp.gif" alt="Descrição do GIF" width="25%">
</p>

### Teste de Adicionar Sprite

Esse teste consiste em adicionar um sprite de uma cobra pre-feita na placa, o seu resultado deve se parecer com isso:

<p align="center">
  <img src="ReadMe_files/wsm.gif" alt="Descrição do GIF" width="25%">
</p>

### Teste de Triangulo e Quadrado

Esse teste consiste em desenhar um triangulo e um quadrado na tela, caso passe paramentros errados, a GPU tentará printar e resultará em uma imagem parecendo um "post-it", caso coloque parametros validos, o resultado será esse:

<p align="center">
  <img src="ReadMe_files/dp_triangle.gif" alt="Descrição do GIF" width="25%">
  <img src="ReadMe_files/dp_square.gif" alt="Descrição do GIF" width="25%">
</p>

### Teste de Background Block

Esse teste consiste em desenhar um background block na tela, o seu resultado deve se parecer com isso:

<p align="center">
  <img src="ReadMe_files/wbm.gif" alt="Descrição do GIF" width="25%">
</p>

### Teste de 7 Segmentos

Esse teste consiste em passar um número para o 7 segmentos e após isso, deve ser impresso esse número no 7 segmentos:

<p align="center">
  <img src="ReadMe_files/7_seg.gif" alt="Descrição do GIF" width="25%">
</p>

### Teste de Botão

Esse teste consiste em ficar continuamente lendo a palavra que está associada aos botões, cada botão pode somar +1,+2,+4,+8 na exibição enquanto estiver sendo pressionado, o seu resultado deve se parecer com isso:

<p align="center">
  <img src="ReadMe_files/botao.gif" alt="Descrição do GIF" width="25%">
</p>

# Tecnologias utilizadas:

Para o desenvolvimento desse projeto, utilizamos as seguintes tecnologias e ferramentas:

Editores de texto:
<img src="https://img.shields.io/badge/Editor-VSCode-blue?logo=visual-studio-code&logoColor=white"/>
<img src="https://img.shields.io/badge/Editor-Neovim-green?logo=neovim&logoColor=white"/>

Linguagem:
<img src="https://img.shields.io/badge/Linguagem-C-blue?logo=c&logoColor=white"/>

Ferramentas auxiliares:
<img src="https://img.shields.io/badge/Software-Piskel-blue?style=flat" alt="Piskel Badge" height="25">
<img src="ReadMe_files/piskel_logo.png" alt="Piskel Logo" height="25">

# Conclusão

Foi desenvolvido toda a biblioteca para uso da GPU com todos os comandos em Assembly e mais algumas funções em C, portanto, os principais objetivos do PBL que eram, conseguir entender como funciona a GPU de Gabriel Sá Barreto e aprender e obter experiencia com códigos em Assembly ARM foi conquistado tendo em vista o sucesso do desenvolvimento do problema.



