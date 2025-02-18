#include "led_control.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/pwm.h"
#include <stdlib.h>

#define LED_COUNT 3 // Number of RGB LEDs
#define PWM_SLICE 0 // PWM slice for controlling LEDs
#define PWM_MAX 50  // Maximum PWM value

static uint LED_PINS[LED_COUNT] = {11, 13, 12}; // GPIO pins for RGB LEDs
static bool green_led_state = false;            // State of the green LED

/**
 * @brief Inicializa os LEDs configurando os pinos e o PWM
 */
void led_init()
{
    for (int i = 0; i < LED_COUNT; i++)
    {
        gpio_set_function(LED_PINS[i], GPIO_FUNC_PWM);
        uint slice_num = pwm_gpio_to_slice_num(LED_PINS[i]);
        pwm_set_wrap(slice_num, PWM_MAX); // Set PWM range to 0-PWM_MAX
        pwm_set_enabled(slice_num, true); // Enable PWM
    }
}

/**
 * @brief Liga ou desliga os LEDs
 *
 * @param state Estado desejado dos LEDs (true para ligar, false para desligar)
 */
void led_on_off(bool state)
{
    uint8_t level = state ? PWM_MAX : 0; // Define o nível PWM para ligar ou desligar
    for (int i = 0; i < LED_COUNT; i++)
    {
        pwm_set_gpio_level(LED_PINS[i], level);
    }
    printf("LEDs %s\n", state ? "ligados" : "desligados");
}

/**
 * @brief Define a cor dos LEDs RGB
 *
 * @param r Valor do canal vermelho (0-255)
 * @param g Valor do canal verde (0-255)
 * @param b Valor do canal azul (0-255)
 */
void led_set_color(uint8_t r, uint8_t g, uint8_t b)
{
    printf("Setting LED color: R=%d, G=%d, B=%d\n", r, g, b);
    pwm_set_gpio_level(LED_PINS[0], g); // Set green channel
    pwm_set_gpio_level(LED_PINS[1], r); // Set red channel
    pwm_set_gpio_level(LED_PINS[2], b); // Set blue channel
}

/**
 * @brief Controla os LEDs com base nos valores do joystick
 *
 * @param x Valor do eixo X do joystick
 * @param y Valor do eixo Y do joystick
 */
void led_control(int x, int y)
{
    // Map joystick values to LED brightness (0-255)
    uint8_t brightness_x = abs(x - 2048) * 255 / 2048; // Map x from 0-4095 to 0-255
    uint8_t brightness_y = abs(y - 2048) * 255 / 2048; // Map y from 0-4095 to 0-255

    // Calculate LED colors based on brightness
    uint8_t r = brightness_x;             // Use brightness_x directly for red channel
    uint8_t b = brightness_y;             // Use brightness_y directly for blue channel
    uint8_t g = green_led_state ? 50 : 0; // Green LED state

    printf("Joystick values: x=%d, y=%d -> LED brightness: R=%d, G=%d, B=%d\n", x, y, r, g, b);
    led_set_color(r, g, b); // Set LED color based on joystick values
}

/**
 * @brief Alterna o estado do LED verde
 */
void toggle_green_led()
{
    green_led_state = !green_led_state;
    printf("Green LED state: %s\n", green_led_state ? "ON" : "OFF");
}