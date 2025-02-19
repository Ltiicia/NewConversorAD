#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "led_control.h"
#include "joystick.h"
#include "button.h"
#include "oled_display.h"

#define I2C_PORT i2c1 // Usando i2c1 pois GPIO14 e 15 são do I2C1
#define I2C_SDA_PIN 14
#define I2C_SCL_PIN 15

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define SQUARE_SIZE 8

static bool leds_enabled = true;
static bool border_style = false;

/**
 * @brief Configura os periféricos e inicializa os componentes
 */
void setup()
{
  stdio_init_all();
  printf("Inicializando ADC...\n");
  adc_init();
  printf("Inicializando I2C...\n");
  i2c_init(I2C_PORT, 400 * 1000); // Initialize I2C at 400kHz
  gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(I2C_SDA_PIN);
  gpio_pull_up(I2C_SCL_PIN);

  printf("Inicializando LEDs...\n");
  led_init();
  printf("Inicializando joystick...\n");
  joystick_init();
  printf("Inicializando botão...\n");
  button_a_init();
  printf("Inicializando display OLED...\n");
  oled_init(I2C_PORT); // Pass the I2C port to oled_init
  printf("Setup concluído.\n");
}

/**
 * @brief Atualiza o display OLED com a posição do joystick
 *
 * @param x Valor do eixo X do joystick
 * @param y Valor do eixo Y do joystick
 */
void update_display(int x, int y)
{
  int display_x = (x * (DISPLAY_WIDTH - SQUARE_SIZE)) / 4096;           // Map joystick X value to display width (0-120)
  int display_y = ((4095 - y) * (DISPLAY_HEIGHT - SQUARE_SIZE)) / 4096; // Map joystick Y value to display height (0-56) and invert

  oled_clear(); // Evitar flickering chamando apenas quando necessário
  oled_draw_square(display_x, display_y, SQUARE_SIZE, SQUARE_SIZE);
  
  if (border_style)
  {
    oled_draw_border(true); // Desenha a borda sólida
  }

  oled_update();
}

/**
 * @brief Função principal que executa o loop de controle
 *
 * @return int Código de retorno
 */
int main()
{
  setup();

  while (true)
  {
    printf("Lendo valores do joystick...\n");
    int joystick_x = joystick_read_x();
    int joystick_y = joystick_read_y();
    printf("Joystick: x=%d, y=%d\n", joystick_x, joystick_y);

    if (leds_enabled)
    {
      led_control(joystick_x, joystick_y);
    }

    update_display(joystick_x, joystick_y);

    printf("Verificando estado do botão A...\n");
    if (button_a_is_pressed())
    {
      printf("Botão A pressionado!\n");
      leds_enabled = !leds_enabled;
      printf("LEDs %s\n", leds_enabled ? "ligados" : "desligados");
      led_on_off(leds_enabled); // Liga ou desliga os LEDs
      oled_display_message(leds_enabled ? "LEDs ON" : "LEDs OFF");
    }

    if (joystick_button_is_pressed())
    {
      printf("Botão do joystick pressionado!\n");
      toggle_green_led();
      border_style = !border_style;
      oled_set_border(border_style);
    }

    sleep_ms(100);
  }
}