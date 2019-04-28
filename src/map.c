#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "structures.h"
#include "additionalFunctions.h"
#include "roadsRelated.h"
#include "deleteStructure.h"
#include "routeRelated.h"


/** @brief Tworzy nową strukturę.
 * Tworzy nową, pustą strukturę niezawierającą żadnych miast, odcinków dróg ani
 * dróg krajowych.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Map* newMap(void){

    Map* map = malloc(sizeof(Map));
    if(map == NULL){
        return NULL;
    }

    map->cityList = NULL;

    /*map->routes = malloc(sizeof(Route*) * 1000);*/
    /*if(map->routes == NULL){
        return NULL;
    }*/

    for(int i = 0; i <= 999; i++) {
        map->routes[i] = NULL;
    }

    return map;
}


/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p map.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] map        – wskaźnik na usuwaną strukturę.
 */
void deleteMap(Map *map){
    if(map != NULL) {
        deleteCitiesRoads(map, map->cityList);
        deleteRouteList(map->routes);
        free(map);
    }
}

/** @brief Dodaje do mapy odcinek drogi między dwoma różnymi miastami.
 * Jeśli któreś z podanych miast nie istnieje, to dodaje go do mapy, a następnie
 * dodaje do mapy odcinek drogi między tymi miastami.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] length     – długość w km odcinka drogi;
 * @param[in] builtYear  – rok budowy odcinka drogi.
 * @return Wartość @p true, jeśli odcinek drogi został dodany.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, obie podane nazwy miast są identyczne, odcinek drogi między tymi
 * miastami już istnieje lub nie udało się zaalokować pamięci.
 */
bool addRoad(Map *map, const char *city1, const char *city2, unsigned length, int builtYear){

    if(!correctName(city1) || !correctName(city2) || builtYear == 0){
        return false; //któryś z parametrów ma niepoprawną wartość,
    }

    if(strcmp(city1, city2) == 0){ //obie podane nazwy miast są identyczne
        return false;
    }

    Road* newRoad = createNewRoad();
    if(newRoad == NULL){
        return false; //nie udało się zaalokować pamięci.
    }

    City* cityA = findCityByName(map->cityList, city1);
    if(cityA == NULL){
        cityA = newCity(city1);
        if(cityA == NULL){
            return false;
        }

        if(!addCity(map, cityA)){
            return false;
        }
    }
    newRoad->cityA = cityA;

    City* cityB = findCityByName(map->cityList, city2);
    if(cityB == NULL){
        cityB = newCity(city2);
        if(cityB == NULL){
            return false;
        }

        if(!addCity(map, cityB)){
            return false;
        }
    }
    newRoad->cityB = cityB;

    newRoad->length = length;
    newRoad->year = builtYear;



    if(!addRoadToCity(cityA, cityA->roadList, newRoad)){ // (miasta istnialy) odcinek drogi między tymi miastami już istnieje
        free(newRoad); //todo pewnie tutaj
        return false;
    }
    addRoadToCity(cityB, cityB->roadList, newRoad);

    return true;
}


/** @brief Modyfikuje rok ostatniego remontu odcinka drogi.
 * Dla odcinka drogi między dwoma miastami zmienia rok jego ostatniego remontu
 * lub ustawia ten rok, jeśli odcinek nie był jeszcze remontowany.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] repairYear – rok ostatniego remontu odcinka drogi.
 * @return Wartość @p true, jeśli modyfikacja się powiodła.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, któreś z podanych miast nie istnieje, nie ma odcinka drogi między
 * podanymi miastami, podany rok jest wcześniejszy niż zapisany dla tego odcinka
 * drogi rok budowy lub ostatniego remontu.
 */
bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear) {

    if(!correctName(city1) || !correctName(city2) || repairYear == 0){
        return false; // któryś z parametrów ma niepoprawną wartość
    }

    City* cityA = findCityByName(map->cityList, city1); //któreś z podanych miast nie istnieje,
    if(cityA == NULL){
        return false;
    }
    City* cityB = findCityByName(map->cityList, city2);
    if(cityB == NULL){
        return false;
    }

    Road* road = findRoad(cityA, cityB);
    if(road == NULL){ //nie ma odcinka drogi między podanymi miastami,
        return false;
    }
    if(repairYear >= road->year){ // podany rok jest wcześniejszy niz zapisany dla tego odcinka rok
        road->year = repairYear;
        return true;
    }
    return false;
}
// --------------------------------------------------------------------------------
// --------------------------------------

/** @brief Łączy dwa różne miasta drogą krajową.
 * Tworzy drogę krajową pomiędzy dwoma miastami i nadaje jej podany numer.
 * Wśród istniejących odcinków dróg wyszukuje najkrótszą drogę. Jeśli jest
 * więcej niż jeden sposób takiego wyboru, to dla każdego wariantu wyznacza
 * wśród wybranych w nim odcinków dróg ten, który był najdawniej wybudowany lub
 * remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli droga krajowa została utworzona.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, istnieje już droga krajowa o podanym numerze, któreś z podanych
 * miast nie istnieje, obie podane nazwy miast są identyczne, nie można
 * jednoznacznie wyznaczyć drogi krajowej między podanymi miastami lub nie udało
 * się zaalokować pamięci.
 */
bool newRoute(Map *map, unsigned routeId, const char *city1, const char *city2){

    if(!correctName(city1) || !correctName(city2) || !correctId(routeId)){
        return false;
    }

    if(strcmp(city1, city2) == 0){
        return false;
    }

    if(map->routes[routeId] != NULL){
        return false;
    }

    City* cityA = findCityByName(map->cityList, city1);
    City* cityB = findCityByName(map->cityList, city2);

    if(cityA == NULL || cityB == NULL){
        return false;
    }

    Route* newRoute = createNewRoute(routeId);
    if(newRoute == NULL){
        return false;
    }

    /*CityList* start = newCityList();
    if(start == NULL){
        return false;
    }
    start->city = cityA;

    CityList* end = newCityList();
    if(end == NULL){
        return false;
    }
    end->city = cityB;*/

    int dummy;
    CityList* shortestPath = findShortestPath(map, newRoute, cityA, cityB, &dummy);
    if(shortestPath == NULL){
        return false;
    }

    newRoute->cityList = shortestPath;

    addRouteInfoToRoads(newRoute);

    /*map->routes[newRoute->routeId] = malloc(sizeof(Route*));*/

    /*if(map->routes[newRoute->routeId] == NULL) {
        while (shortestPath != NULL){
            CityList* tmp = shortestPath;
            shortestPath = shortestPath->next;
            free(tmp);
        }
    }*/
    map->routes[newRoute->routeId] = newRoute;

    return true;
}

/** @brief Wydłuża drogę krajową do podanego miasta.
 * Dodaje do drogi krajowej nowe odcinki dróg do podanego miasta w taki sposób,
 * aby nowy fragment drogi krajowej był najkrótszy. Jeśli jest więcej niż jeden
 * sposób takiego wydłużenia, to dla każdego wariantu wyznacza wśród dodawanych
 * odcinków dróg ten, który był najdawniej wybudowany lub remontowany i wybiera
 * wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] city       – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli droga krajowa została wydłużona.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * nazwę, nie istnieje droga krajowa o podanym numerze, nie ma miasta o podanej
 * nazwie, przez podane miasto już przechodzi droga krajowa o podanym numerze,
 * podana droga krajowa kończy się w podanym mieście, nie można jednoznacznie
 * wyznaczyć nowego fragmentu drogi krajowej lub nie udało się zaalokować
 * pamięci.
 */
bool extendRoute(Map *map, unsigned routeId, const char *city){ // TODO dla route A-B, extend C, wybiera min(C-A-B, A-B-C)

    if(!correctName(city) || !correctId(routeId)){
        return false;
    }

    if(map->routes[routeId] == NULL){
        return false;
    }

    City* additionalCity = findCityByName(map->cityList, city); //nie ma miasta o podanej nazwie
    if(additionalCity == NULL){
        return false;
    }

    if(routeContainsCity(map->routes[routeId], additionalCity)){ //przez miasto już przechodzi droga krajowa o podanym numerze
        return false;                                            // podana droga krajowa kończy się w podanym mieście,
    }

    CityList* tmp = map->routes[routeId]->cityList;

    int firstOldestRoadYear = INT_MIN;
    CityList* firstPath = findShortestPath(map, map->routes[routeId], additionalCity, tmp->city, &firstOldestRoadYear);
    if(firstPath == NULL){
        return false;
    }
    unsigned firstLength = calculateLength(firstPath);

    while(tmp->next != NULL){
        tmp = tmp->next;
    }
    int secondOldestRoadYear = INT_MIN;
    CityList* secondPath = findShortestPath(map, map->routes[routeId], tmp->city, additionalCity, &secondOldestRoadYear);
    if(secondPath == NULL){
        return false;
    }
    unsigned secondLength = calculateLength(secondPath);


    switch(betterPath(firstOldestRoadYear, firstLength, secondOldestRoadYear, secondLength)){
        case 1: {
            CityList *endOfPath = firstPath;
            while (endOfPath->next != NULL) {
                endOfPath = endOfPath->next;
            }

            endOfPath->next = map->routes[routeId]->cityList->next;
            (map->routes[routeId]->cityList->next)->prev = endOfPath;

            map->routes[routeId]->cityList = firstPath;
            //TODO free secondPath
            deleteCityList(secondPath);
            break;
        }
        case 2: {

            CityList *endOfRoute = map->routes[routeId]->cityList;
            while (endOfRoute->next != NULL) {
                endOfRoute = endOfRoute->next;
            }

            endOfRoute->next = secondPath->next;
            (secondPath->next)->prev = endOfRoute;
            //TODO free firstpath
            deleteCityList(firstPath);

            break;
        }
        case 0: {
            //TODO free memory
            deleteCityList(firstPath);
            deleteCityList(secondPath);
            return false;
        }
        default: {
            return false;
        }
    }

    return true;
}

/** @brief Usuwa odcinek drogi między dwoma różnymi miastami.
 * Usuwa odcinek drogi między dwoma miastami. Jeśli usunięcie tego odcinka drogi
 * powoduje przerwanie ciągu jakiejś drogi krajowej, to uzupełnia ją
 * istniejącymi odcinkami dróg w taki sposób, aby była najkrótsza. Jeśli jest
 * więcej niż jeden sposób takiego uzupełnienia, to dla każdego wariantu
 * wyznacza wśród dodawanych odcinków drogi ten, który był najdawniej wybudowany
 * lub remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli odcinek drogi został usunięty.
 * Wartość @p false, jeśli z powodu błędu nie można usunąć tego odcinka drogi:
 * któryś z parametrów ma niepoprawną wartość, nie ma któregoś z podanych miast,
 * nie istnieje droga między podanymi miastami, nie da się jednoznacznie
 * uzupełnić przerwanego ciągu drogi krajowej lub nie udało się zaalokować
 * pamięci.
 */
bool removeRoad(Map *map, const char *city1, const char *city2){
    return false;
}

/** @brief Udostępnia informacje o drodze krajowej.
 * Zwraca wskaźnik na napis, który zawiera informacje o drodze krajowej. Alokuje
 * pamięć na ten napis. Zwraca pusty napis, jeśli nie istnieje droga krajowa
 * o podanym numerze. Zaalokowaną pamięć trzeba zwolnić za pomocą funkcji free.
 * Informacje wypisywane są w formacie:
 * numer drogi krajowej;nazwa miasta;długość odcinka drogi;rok budowy lub
 * ostatniego remontu;nazwa miasta;długość odcinka drogi;rok budowy lub
 * ostatniego remontu;nazwa miasta;…;nazwa miasta.
 * Kolejność miast na liście jest taka, aby miasta @p city1 i @p city2, podane
 * w wywołaniu funkcji @ref newRoute, które utworzyło tę drogę krajową, zostały
 * wypisane w tej kolejności.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej.
 * @return Wskaźnik na napis lub NULL, gdy nie udało się zaalokować pamięci.
 */
char const* getRouteDescription(Map *map, unsigned routeId){
    return "elo";
}

