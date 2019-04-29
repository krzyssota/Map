#ifndef DROGI_ADDITIONALFUNCTIONS_H
#define DROGI_ADDITIONALFUNCTIONS_H

#include <stdbool.h>
#include "map.h"

/** @brief Wyciaga nazwe miasta.
 * @param[in] name – wskaźnik na napis reprezentujący nazwę miasta;
 * @return wskaznik na zaalokowany adres w pamieci z nazwą miasta.
 */
char* getName(const char *name);
/** @brief Sprawdza czy id drogi krajowej jest poprawny.
 * @param[in] id - id drogi krajowej.
 * @return true jeśli id jest poprawne. false w przeciwnym przypadku.
 */
bool correctId(unsigned id);
/** @brief Sprawdza czy nazwa miasta jest poprawna.
 * @param[in] cityName - wskaźnik na nazwę miasta.
 * @return true jeśli nazwa jest poprawne. false w przeciwnym przypadku.
 */
bool correctName(const char* cityName);
/** @brief Liczy libcze znaków, którą zajmnie opis drogi krajowej.
 * @param[in] route - wskaźnik na drogę krajową.
 * @return Liczbę znaków.
 */
int count(Route* route);
/** @brief Zwraca opis drogi krajowej.
 * @param[in] route - wskaźnik na drogę krajową.
 * @param[in] length - liczba znaków potrzebna na opis drogi krajowej.
 * @return Wskaźnik na opis drogi krajowej.
 */
char* getDescription(Route* route, int length);

#endif //DROGI_ADDITIONALFUNCTIONS_H
