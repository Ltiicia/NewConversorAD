#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Ajuste se seu display tiver outro endereço
#define OLED_ADDR 0x3C

#define OLED_CMD 0x00
#define OLED_DATA 0x40

// Buffer para 128x64 (1 bit/pixel = 1024 bytes)
static uint8_t buffer[1024];

// Variável global para armazenar a instância de I2C usada
static i2c_inst_t *oled_i2c = NULL;

/**
 * @brief Atualiza o display com o conteúdo do buffer
 */
void oled_update(void)
{
  for (uint8_t page = 0; page < 8; page++)
  {
    // Seleciona a página
    i2c_write_blocking(oled_i2c, OLED_ADDR,
                       (uint8_t[]){OLED_CMD, (uint8_t)(0xB0 + page)}, 2, false);

    // Endereço de coluna (parte baixa e alta)
    i2c_write_blocking(oled_i2c, OLED_ADDR,
                       (uint8_t[]){OLED_CMD, 0x00}, 2, false);
    i2c_write_blocking(oled_i2c, OLED_ADDR,
                       (uint8_t[]){OLED_CMD, 0x10}, 2, false);

    // Envia os 128 bytes daquela página
    // (usamos OLED_DATA = 0x40 somente no primeiro byte se quisermos,
    //  mas como está enviando blocos, normalmente já está configurado)
    // Para maior compatibilidade, pode-se enviar 1 byte de comando 0x40 + 128 de dados
    // Mas aqui faremos de forma simples:
    uint8_t temp[129];
    temp[0] = OLED_DATA; // Indica que os próximos são dados
    memcpy(&temp[1], &buffer[page * 128], 128);
    i2c_write_blocking(oled_i2c, OLED_ADDR, temp, 129, false);
  }
}

/**
 * @brief Inicializa o display e limpa o buffer
 */
void oled_init(i2c_inst_t *i2c)
{
  // Armazena a instância para uso em todo o módulo
  oled_i2c = i2c;

  // Sequência de inicialização para 128x64
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xAE}, 2, false); // Display off
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xD5, 0x80}, 3, false); // Display clock
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xA8, 0x3F}, 3, false); // Multiplex = 0x3F (64 linhas)
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xD3, 0x00}, 3, false); // Display offset = 0
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0x40}, 2, false); // Start line = 0
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0x8D, 0x14}, 3, false); // Charge pump ON
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0x20, 0x00}, 3, false); // Memory addressing mode (Horizontal)
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xA1}, 2, false); // Segment remap
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xC8}, 2, false); // COM scan direction
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xDA, 0x12}, 3, false); // COM pins
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0x81, 0x7F}, 3, false); // Contrast
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xD9, 0xF1}, 3, false); // Pre-charge
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xDB, 0x40}, 3, false); // VCOMH deselect
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xA4}, 2, false); // Output follows RAM
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xA6}, 2, false); // Normal display (A7 = invert)
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xAF}, 2, false); // Display ON

  // Limpa o buffer e atualiza
  memset(buffer, 0, sizeof(buffer));
  oled_update();
}

/**
 * @brief Limpa todo o buffer e atualiza
 */
void oled_clear(void)
{
  memset(buffer, 0, sizeof(buffer));
  oled_update();
}

/**
 * @brief Desenha (ou limpa) um pixel no buffer
 *
 * @param x Coordenada X do pixel
 * @param y Coordenada Y do pixel
 * @param color Cor do pixel (1 para acender, 0 para apagar)
 */
void oled_draw_pixel(uint8_t x, uint8_t y, uint8_t color)
{
  if (x >= 128 || y >= 64)
    return; // Fora dos limites
  uint16_t index = (y / 8) * 128 + x;
  uint8_t bit = 1 << (y % 8);

  if (color)
    buffer[index] |= bit; // Acende pixel
  else
    buffer[index] &= ~bit; // Apaga pixel
}

/**
 * @brief Desenha uma string (você precisa implementar a lógica de fonte)
 *
 * @param x Coordenada X inicial
 * @param y Coordenada Y inicial
 * @param str String a ser desenhada
 */
void oled_draw_string(uint8_t x, uint8_t y, const char *str)
{
  while (*str)
  {
    // Exemplo simplificado: avançar X e ignorar a lógica de fonte
    // Para desenhar de verdade, precisa de bitmap de caracteres.
    // Este placeholder apenas avança x para cada caractere:
    x += 6; // Largura de caractere 5 + 1 de espaço
    str++;
  }
  // Atualiza se quiser imediatamente
  oled_update();
}

/**
 * @brief Escreve uma mensagem simples na posição (0,0)
 *
 * @param message Mensagem a ser exibida
 */
void oled_display_message(const char *message)
{
  oled_clear();
  oled_draw_string(0, 0, message);
  oled_update();
}

/**
 * @brief Desenha um quadrado cheio
 *
 * @param x Coordenada X inicial
 * @param y Coordenada Y inicial
 * @param width Largura do quadrado
 * @param height Altura do quadrado
 */
void oled_draw_square(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
  for (uint8_t i = 0; i < width; i++)
  {
    for (uint8_t j = 0; j < height; j++)
    {
      oled_draw_pixel(x + i, y + j, 1);
    }
  }
  oled_update();
}

/**
 * @brief Desenha uma borda (contorno) na tela inteira sem limpar o buffer
 *
 * @param style Estilo da borda (true para sólida, false para pontilhada)
 */
void oled_draw_border(bool style)
{
  if (style)
  {
    // Borda sólida
    for (uint8_t i = 0; i < 128; i++)
    {
      oled_draw_pixel(i, 0, 1);  // Topo
      oled_draw_pixel(i, 63, 1); // Base
    }
    for (uint8_t i = 0; i < 64; i++)
    {
      oled_draw_pixel(0, i, 1);   // Esquerda
      oled_draw_pixel(127, i, 1); // Direita
    }
  }
  else
  {
    // Borda pontilhada
    for (uint8_t i = 0; i < 128; i += 2)
    {
      oled_draw_pixel(i, 0, 1);
      oled_draw_pixel(i, 63, 1);
    }
    for (uint8_t i = 0; i < 64; i += 2)
    {
      oled_draw_pixel(0, i, 1);
      oled_draw_pixel(127, i, 1);
    }
  }
}

/**
 * @brief Desenha uma borda (contorno) na tela inteira
 *
 * @param style Estilo da borda (true para sólida, false para pontilhada)
 */
void oled_set_border(bool style)
{
  oled_clear();
  oled_draw_border(style);
  oled_update();
}