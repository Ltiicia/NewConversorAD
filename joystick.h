#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Inicializa o joystick configurando os pinos e o ADC
 */
void joystick_init();

/**
 * @brief Lê o valor do eixo X do joystick
 *
 * @return Valor lido do eixo X
 */
uint16_t joystick_read_x();

/**
 * @brief Lê o valor do eixo Y do joystick
 *
 * @return Valor lido do eixo Y
 */
uint16_t joystick_read_y();

/**
 * @brief Obtém a posição do joystick nos eixos X e Y
 *
 * @param x Ponteiro para armazenar o valor do eixo X
 * @param y Ponteiro para armazenar o valor do eixo Y
 */
void joystick_get_position(uint16_t *x, uint16_t *y);

/**
 * @brief Verifica se o botão do joystick foi pressionado
 *
 * @return true se o botão foi pressionado, false caso contrário
 */
bool joystick_button_is_pressed();

#endif // JOYSTICK_H