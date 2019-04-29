#ifndef DROGI_DIJKSTRA_H
#define DROGI_DIJKSTRA_H


#include "structures.h"
#include "map.h"

/** @brief Zwraca element kolejki priorytetowej, z którego można odzywskać optymalną drogę.
 *  @param[in,out] map – wskaźnik na strukturę przechowującą mapę dróg;
  * @param[in] routeA – wskaźnik na drogę krajową.
  * @param[in] routeB – wskaźnik na drogę krajową.
 * @param[in] cityA – wskaźnik na miasto;
 * @param[in] cityB – wskaźnik na miasto.
 * @param[in] road – wskaźnik na usuwaną drogę. Jeśli funkcja została wywołana poza removeRoad, zmienna dummy == NULL
 * @return Wartość @p true, jeśli odcinek drogi został usunięty.
 */
QueueElement* Dijkstra(Map* map, Route* routeA, Route* routeB, City* cityA, City* cityB, Road* roadRemoved);
/** @brief Zwraca element kolejki z najwyższym priorytetem.
 * @param[in] queue - wskaźnik na wskaźnik na kolejkę priorytetową.
 * @return Element kolejki z najwyższym priorytetem.
 */
QueueElement* pop(Queue** queue);

#endif //DROGI_DIJKSTRA_H
