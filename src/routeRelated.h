/**
 * @file
 * Moduł udostępnia funcję wykorzystywane przy operacjach na drogach krajowych.
 * @author Krzysztof Sota
 * @date 18.05.2019
 * */

#include "structures.h"
#include "map.h"

#ifndef DROGI_ROUTERELATED_H
#define DROGI_ROUTERELATED_H

/** @brief Zwraca drugie miasto z drogi.
 *  @param[in] road - wskaźnik na drogę.
 *  @param[in] city - wskaźnik na miasto.
 * @return Zwraca drugie miasto z drogi, lub NULL jeśli droga nie zawiera miasta.
 */
City *getOtherCity(Road *road, City *city);

/** @brief Znajduje najkrotsza sciezke pomiedzy miastem A i miastem B.
 * Rozwiazanie nie zawiera miast z drogi krajowej A, drogi krajowej B, ani drogi usuwanej.
 * @param[in,out] map      - wskaźnik na mape.
 * @param[in] routeA      - wskaźnik na droge krajowa.
 * @param[in] routeB      - wskaźnik na droge krajowa.
 * @param[in] cityA      - wskaźnik na miasto.
 * @param[in] cityB      - wskaźnik na miasto.
 * @param[in] roadRemoved   -  droga usuwana w funkcji removeRoad. Dla innych wywolan wartosc NULL.
 * @return Wskaznik na liste miast z ktorych sklada sie sciezka jesli sciezka zostala znaleziona.
 * Wartość NULL, jeśli nie zostala znaleziona.
 */
ShortestPathResult* findShortestPath(Map *map, Route *routeA, Route *routeB, City *cityA, City *cityB, Road* roadRemoved);

/** @brief W drogach skladajacych sie na droge krajowa zapisuje o tym informacje.
 * * @param[in] route - wskaźnik na drogę krajową.
 */
void addRouteInfoToRoads(Route *route);

/** @brief W drogach skladajacych sie na droge krajowa usuwa o tym informacje.
 * * @param[in] route - wskaźnik na drogę krajową.
 */
void deleteRouteInfoFromRoad(Route* route);

/** @brief Czy droga krajowa zawiera drogę.
 * * @param[in] route - wskaźnik na drogę krajową.
 * @param[in] road - wskaźnik na drogę.
 * @return true jeśli drogą krajowa zawiera drogę. false w przeciwnym przypadku.
 */
bool routeContainsRoad(Route *route, Road *road);

/**@brief Rozważa, która ścieżka jest lepsza. Wywołanie w extendRoute.
 * @param res1 - wskaźnik na opakowanie najlepszej proponowanej drogi krajowej od miasta do początku oryginalnej drogi krajowej.
 * @param res2 - wskaźnik na opakowanie najlepszej proponowanej drogi krajowej od końca oryginalnej drogi krajowej do miasta.
* @return 1 jeśli pierwsza drogą jest lepsza. 2 jeśli druga jest lepsza. 0 jeśli obie są równie dobre w myśl kryteriów podanych
        * w zadaniu.
 */
int betterPath(ShortestPathResult* res1, ShortestPathResult* res2);

/** @brief Wklada liste miast ze sciezki do drogi krajowej.
 * @param[in] path      - wskaźnik na sciezke.
 * @param[in] route      - wskaźnik na droge krajowa.
 * @param[in] cityA      - wskaźnik na miasto.
 * @param[in] cityB      - wskaźnik na miasto.
 */
void insertPathIntoRoute(CityList* path, Route* route, City* cityA, City* cityB);

/**@brief Tworzy drogę krajową na podstawie wzoru routeParam.
 * @param[in,out] map - wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeParam - wskaźnik na strukturę przechowującą
 * @return Wartość @p true jeśli operacją powiodłą się, @p false w przeciwnym razie.
 */
bool newRouteFromRouteParam(Map* map, RouteParam* routeParam);

City* occurrenceInRoute(int x, Route *route, City *cityA, City *cityB);

#endif //DROGI_ROUTERELATED_H
