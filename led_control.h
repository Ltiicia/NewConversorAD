#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Inicializa os LEDs configurando os pinos e o PWM
 */
void led_init();

/**
 * @brief Define a cor dos LEDs RGB
 *
 * @param r Valor do canal vermelho (0-255)
 * @param g Valor do canal verde (0-255)
 * @param b Valor do canal azul (0-255)
 */
void led_set_color(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Define o brilho dos LEDs
 *
 * @param brightness Valor do brilho (0-255)
 */
void led_set_brightness(uint8_t brightness);

/**
 * @brief Controla os LEDs com base nos valores do joystick
 *
 * @param x Valor do eixo X do joystick
 * @param y Valor do eixo Y do joystick
 */
void led_control(int x, int y);

/**
 * @brief Alterna o estado do LED verde
 */
void toggle_green_led();

/**
 * @brief Liga ou desliga os LEDs
 *
 * @param state Estado desejado dos LEDs (true para ligar, false para desligar)
 */
void led_on_off(bool state);

#endif // LED_CONTROL_H