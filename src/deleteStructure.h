#ifndef DROGI_DELETESTRUCTURE_H
#define DROGI_DELETESTRUCTURE_H

#include "structures.h"
#include "map.h"

/** @brief Usuwa wszystkie drogi i miasta ze struktury map.
 * @param[in] map - wskaźnik na strukturę mapy.
 * @param[in] cityList - wskaźnik na listę miast.
 */
void deleteCitiesRoads(Map *map, CityList *cityList);
/** @brief Usuwa pozostałe w kolejce elementy.
 * @param[in] queue - wskaźnik na wskaźnik na kolejkę.
 */
void cleanQueue(Queue **queue);
/** @brief Usuwa drogi ze struktury mapa.
 * @param[in] routeList - wskaźnik na tablicę dróg.
 */
void deleteRoutes(Route* routeList[]);
/** @brief Usuwa listę dróg ze struktury mapa.
 * @param[in] cityList - wskaźnik na listę miast.
 */
void deleteCityList(CityList* cityList);
/** @brief Usuwa drogę krajową.
 * @param[in] route - wskaźnik na drogę krajową.
 */
void deleteRoute(Route* route);
/** @brief Usuwa drogę ze struktury.
 * Usuwa także elementy listy dróg wskazujące na drogę.
 * @param[in] road - wskaźnik na drogę.
 */
void deleteRoadAndTwoRoadLists(Road *road);

void deleteLine(char* line);




#endif //DROGI_DELETESTRUCTURE_H
