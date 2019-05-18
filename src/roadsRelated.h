/**
 * @file
 * Moduł udostępnia funcję wykorzystywane przy operacjach na drogach.
 * @author Krzysztof Sota
 * @date 18.05.2019
 * */
#ifndef DROGI_ROADSRELATED_H
#define DROGI_ROADSRELATED_H

#include "map.h"
#include "structures.h"
#include "stdlib.h"
#include "stdbool.h"

/** @brief Stwierdzą, która z podanych dróg jest starsza.
 * @param[in] roadA - wskaźnik na pierwszą drogę.
 * @param[in] roadB - wskaźnik na drugą drogę.
 * @return Zwraca wskaźnik na starszą drogę.
 */
Road* olderRoad(Road* roadA, Road* roadB);
/** @brief Znajduję element listy dróg wskazującej na drogę.
 *  @param[in] roadList - wskaźnik na listę dróg;
  * @param[in] road - wskaźnik na drogę.
 * @return Zwraca element listy dróg wskazującej na drogę, lub NULL jeśli lista nie zawiera elementu wskazującego na drogę.
 */
RoadList* findRoadListElement(RoadList *roadList, Road *road);

/** @brief Znajduję miasto po nazwie.
 * @param[in] cityList - wskaźnik na listę miast.
  * @param[in] cityName - wskaźnik na szukaną nazwę.
 * @return Zwraca szukane miasto, lub NULL jeśli w strukturze nie ma miasta o podanej nazwie.
 */
City* findCityByName(CityList *cityList, const char *cityName) ;

/** @brief Dodaje drogę do miasta.
 *  @param[in] city – wskaźnik na miasto.
  * @param[in] roadList - wskaźnik na listę dróg;
  * @param[in] newRoad - wskaźnik na drogę.
 * @return Jeśli @p true udało się dodać drogę do miasta. W przeciwnym wypadku @p false.
 */
bool addRoadToCity(City *city, RoadList* roadList, Road *newRoad);

/** @brief Dodaje miasto do mapy.
 *  @param[in,out] map – wskaźnik na strukturę przechowującą mapę dróg;
 *  @param[in] city – wskaźnik na miasto.
 * @return Jeśli @p true udało się dodać miasto do mapy. W przeciwnym wypadku @p false.
 */
bool addCity(Map* map, City* city);

/** @brief Znajduję drogę prowadzącą z jednego do drugiego miasta.
 * @param[in] cityA – wskaźnik na miasto;
 * @param[in] cityB – wskaźnik na miasto.
 * @return Zwraca wskaźnik na znalezioną drogę. Jeśli droga nie zostałą znaleziona zwraca NULL.
 */
Road* findRoad(City* cityA, City* cityB);

/** @brief Stwierdzą czy droga krajowa zawiera drogę.
  * @param[in] route – wskaźnik na drogę krajową.
 * @param[in] city – wskaźnik na miasto;
 * @return Wartość @p true, jeśli droga krajowa zawiera drogę. W przeciwnym wypadku @p false.
 */
bool routeContainsCity(Route* route, City* city);

#endif //DROGI_ROADSRELATED_H
