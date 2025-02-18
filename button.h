#ifndef BUTTON_A_H
#define BUTTON_A_H

/**
 * @brief Inicializa o botão A configurando o pino e a interrupção
 */
void button_a_init();

/**
 * @brief Verifica se o botão A foi pressionado
 *
 * @return true se o botão foi pressionado, false caso contrário
 */
bool button_a_is_pressed();

#endif // BUTTON_A_H