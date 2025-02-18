#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "joystick.h"

#define JOYSTICK_X_PIN 27
#define JOYSTICK_Y_PIN 26
#define JOYSTICK_BUTTON_PIN 22 // Define the GPIO pin for the joystick button

/**
 * @brief Inicializa o joystick configurando os pinos e o ADC
 */
void joystick_init()
{
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);
    gpio_init(JOYSTICK_BUTTON_PIN);
    gpio_set_dir(JOYSTICK_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_BUTTON_PIN);
}

/**
 * @brief Lê o valor do eixo X do joystick
 *
 * @return Valor lido do eixo X
 */
uint16_t joystick_read_x()
{
    adc_select_input(1); // Ensure the correct ADC channel is selected for X
    return adc_read();
}

/**
 * @brief Lê o valor do eixo Y do joystick
 *
 * @return Valor lido do eixo Y
 */
uint16_t joystick_read_y()
{
    adc_select_input(0); // Ensure the correct ADC channel is selected for Y
    return adc_read();
}

/**
 * @brief Obtém a posição do joystick nos eixos X e Y
 *
 * @param x Ponteiro para armazenar o valor do eixo X
 * @param y Ponteiro para armazenar o valor do eixo Y
 */
void joystick_get_position(uint16_t *x, uint16_t *y)
{
    *x = joystick_read_x();
    *y = joystick_read_y();
}

/**
 * @brief Verifica se o botão do joystick foi pressionado
 *
 * @return true se o botão foi pressionado, false caso contrário
 */
bool joystick_button_is_pressed()
{
    return !gpio_get(JOYSTICK_BUTTON_PIN);
}