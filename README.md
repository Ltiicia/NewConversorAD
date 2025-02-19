# Atividade - Controle de LED RGB e Display OLED com Joystick

**Desenvolvido por:** Letícia Gonçalves Souza

Este projeto foi desenvolvido como parte do programa **EmbarcaTech**, com o objetivo de aplicar conceitos de **ADC**, **PWM**, **I2C** e **interrupções com debouncing**. A atividade visa consolidar o entendimento prático desses conceitos através da implementação de uma interface interativa na placa **BitDogLab** com o microcontrolador **RP2040**.

---

## Sobre o Projeto

O projeto consiste na implementação de um sistema que integra diversas funcionalidades:
- Leitura dos valores analógicos de um **joystick** utilizando o ADC do RP2040.
- Controle de intensidade dos LEDs RGB via PWM, onde:
  - **LED Azul:** Brilho ajustado conforme o eixo Y do joystick.
  - **LED Vermelho:** Brilho ajustado conforme o eixo X do joystick.
  - **LED Verde:** Alterna seu estado a cada acionamento do botão do joystick.
- Exibição gráfica em um **display OLED SSD1306** (conectado via I2C), onde um quadrado de 8x8 pixels se move proporcionalmente aos valores do joystick.
- Uso de botões com rotinas de interrupção para:
  - Ativar/desativar os LEDs PWM (Botão A).
  - Alternar o estado do LED verde e modificar a borda do display (Botão do Joystick).


## Vídeo de Demonstração

[Assista ao vídeo de demonstração aqui](https://youtu.be/V21F1PUWwRI)



## Funcionalidades

1. **Leitura do Joystick e Controle dos LEDs:**  
   - O **joystick** fornece valores dos eixos X e Y que são convertidos via ADC.
   - O **LED Azul** tem seu brilho ajustado conforme o eixo Y, e o **LED Vermelho** conforme o eixo X, utilizando PWM.
   - O **LED Verde** alterna seu estado a cada pressionamento do botão do joystick.

2. **Interface Gráfica no Display OLED:**  
   - Exibição de um quadrado de 8x8 pixels que se move de acordo com a posição do joystick.
   - Alteração do estilo da borda do display ao pressionar o botão do joystick.

3. **Controle dos Botões com Interrupções:**  
   - **Botão A:** Ativa ou desativa os LEDs controlados por PWM.
   - **Botão do Joystick:** Realiza o toggle do LED verde e altera a borda do display.
   - Implementação de **debouncing** via software para garantir a confiabilidade dos acionamentos.

---

## Tecnologias e Ferramentas Utilizadas

- **Hardware:**
  - **RP2040/BitDogLab**
  - **LED RGB**
  - **Joystick** (com leitura analógica dos eixos X e Y)
  - **Botão do Joystick**
  - **Botão A**
  - **Display OLED SSD1306** (conexão I2C)

- **Software:**
  - Linguagem de programação: **C**
  - Ambiente de Desenvolvimento: **Visual Studio Code**
  - **Pico SDK** para controle de hardware

---

## Como Executar o Projeto

1. **Configuração do Ambiente:**
   - Certifique-se de ter o **Visual Studio Code** instalado e configurado para desenvolvimento em C.
   - Instale e configure o **Pico SDK** para trabalhar com o RP2040.
   - Conecte a placa BitDogLab ao computador.

2. **Clonar o Repositório:**

   ```bash
   git clone https://github.com/Ltiicia/NewConversorAD.git
   cd NewConversorAD
