/**
 * @file
 * Moduł przetwarza wejście standardowe.
 * @author Krzysztof Sota
 * @date 18.05.2019
 * */

#ifndef DROGI_INPUT_H
#define DROGI_INPUT_H

#include "map.h"

/**@brief Zczytuje i wykonuje komendy z wejścia standardowego.
 * @param[in,out] map – wskaźnik na strukturę przechowującą mapę dróg.
 */
void readExecuteInput(Map *map);

#endif //DROGI_INPUT_H
