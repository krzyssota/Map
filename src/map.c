#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "structures.h"
#include "additionalFunctions.h"
#include "roadsRelated.h"
#include "deleteStructure.h"
#include "routeRelated.h"
#include <limits.h>


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

    for(int i = 0; i <= 999; i++) {
        map->routes[i] = NULL;
    }

    map->inputLine = NULL;

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
        deleteRoutes(map->routes);

        deleteLine(map->inputLine);

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

    if(!correctName(city1) || !correctName(city2) || length <= 0 || builtYear == 0){ ///< Któryś z parametrów ma niepoprawną wartość.
        return false;
    }

    if(strcmp(city1, city2) == 0){ ///< Podane nazwy miast są identyczne.
        return false;
    }

    Road* newRoad = createNewRoad();
    if(newRoad == NULL){
        return false;
    }

    City* cityA = findCityByName(map->cityList, city1);
    if(cityA == NULL){ ///< Create city if it is not yet in the structure.
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



    if(!addRoadToCity(cityA, cityA->roadList, newRoad)){ ///< Odcinek drogi między tymi miastami już istnieje.
        free(newRoad);
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

    if (!correctName(city1) || !correctName(city2) || repairYear == 0) { ///< Któryś z parametrów ma niepoprawną wartość.
        return false;
    }

    City *cityA = findCityByName(map->cityList, city1); ///< Któreś z podanych miast nie istnieje,
    if (cityA == NULL) {
        return false;
    }
    City *cityB = findCityByName(map->cityList, city2);
    if (cityB == NULL) {
        return false;
    }

    Road *road = findRoad(cityA, cityB);
    if (road == NULL) {  ///< Nie ma odcinka drogi między podanymi miastami,
        return false;
    }
    if (repairYear >= road->year) { ///< Podany rok jest wcześniejszy niz zapisany dla tej drogi.
        road->year = repairYear;
        return true;
    }
    return false;
}

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

    if(!correctName(city1) || !correctName(city2) || !correctId(routeId)){ ///< Któryś z parametrów ma niepoprawną wartość.
        return false;
    }

    if(strcmp(city1, city2) == 0){ ///< Podane nazwy miast są identyczne.
        return false;
    }

    if(map->routes[routeId] != NULL){ ///< Droga krajowa o podanym Id juz istnieje.
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

    int dummyYear;
    Route* dummyRoute = NULL;
    Road* dummyRoad = NULL;
    CityList* shortestPath = findShortestPath(map, newRoute, dummyRoute, cityA, cityB, &dummyYear, dummyRoad);
    if(shortestPath == NULL){ ///< Nie udalo sie znalezc najkrotszego polaczenia.
        return false;
    }

    newRoute->cityList = shortestPath;

    addRouteInfoToRoads(newRoute); ///< Drogi maja informacje do jakich drog krajowych naleza.

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
bool extendRoute(Map *map, unsigned routeId, const char *city){

    if(!correctName(city) || !correctId(routeId)){ ///< Któryś z parametrów ma niepoprawną wartość.
        return false;
    }

    if(map->routes[routeId] == NULL){ ///< Droga krajowa o podanym Id nie istnieje.
        return false;
    }

    City* additionalCity = findCityByName(map->cityList, city); ///< Nie ma miasta o podanej nazwie.
    if(additionalCity == NULL){
        return false;
    }

    if(routeContainsCity(map->routes[routeId], additionalCity)){ ///< Miasto jest juz czescia drogi krajowej o podanym Id
        return false;
    }

    CityList* tmp = map->routes[routeId]->cityList;

    /** Rozwaza dwie mozliwosci przedluzenia drogi krajowej.
     */
    int firstOldestRoadYear = INT_MIN;
    Route* dummyRoute = NULL;
    Road* dummyRoad = NULL;

    CityList* firstPath = findShortestPath(map, map->routes[routeId], dummyRoute,additionalCity,
                                           tmp->city, &firstOldestRoadYear, dummyRoad);
    if(firstPath == NULL){
        return false;
    }

    unsigned firstLength = calculateLength(firstPath);

    while(tmp->next != NULL){
        tmp = tmp->next;
    }
    int secondOldestRoadYear = INT_MIN;

    CityList* secondPath = findShortestPath(map, map->routes[routeId], dummyRoute,
                                            tmp->city, additionalCity, &secondOldestRoadYear, dummyRoad);
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

            deleteCityList(firstPath);

            break;
        }
        case 0: {

            deleteCityList(firstPath);
            deleteCityList(secondPath);
            return false;
        }
        default: {
            return false;
        }
    }
    addRouteInfoToRoads(map->routes[routeId]);

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

    if(!correctName(city1) || !correctName(city2)){ ///< Któryś z parametrów ma niepoprawną wartość.
        return false;
    }

    City* cityA = findCityByName(map->cityList, city1);
    if(cityA == NULL){  ///< Któreś z podanych miast nie istnieje.
        return false;
    }
    City* cityB = findCityByName(map->cityList, city2);
    if(cityB == NULL){ ///< Któreś z podanych miast nie istnieje.
        return false;
    }

    Road* road = findRoad(cityA, cityB);
    if(road == NULL){ ///< Nie ma odcinka drogi między podanymi miastami.
        return false;
    }
    for(int i = 1; i <= 999; ++i) {
        if(road->routesBelonging[i] != NULL) { ///< Dla istniejacych drog krajowych.

            Route* routeA = createNewRoute(1000);

            CityList* firstCityList = NULL;
            CityList* previous = NULL;

            CityList* start = NULL;


            CityList *cityListToCopy = road->routesBelonging[i]->cityList;

            bool last = false;
            while(!last){ ///< Skopiuj miasta z drogi krajowej az do poczatku usunietej drogi.

                firstCityList = newCityList();
                if(firstCityList == NULL){
                    return false;
                }

                firstCityList->city = cityListToCopy->city;
                firstCityList->prev = previous;
                if(previous != NULL){
                    previous->next = firstCityList;
                }

                if(previous == NULL) {
                    start = firstCityList;
                }

                previous = firstCityList;


                if(cityListToCopy->city == road->cityA || cityListToCopy->city == road->cityB){
                    last = true;
                }

                cityListToCopy = cityListToCopy->next;
            }
            routeA->cityList = start;


            cityListToCopy = cityListToCopy->next;

            Route* routeB = createNewRoute(1000);

            CityList* secondCityList = NULL;
            CityList* previousB = NULL;
            CityList* startB = NULL;



            while(cityListToCopy != NULL){ ///< Skopiuj miasta z drogi krajowej od konca usunietej drogi.

                secondCityList = newCityList();
                if(secondCityList == NULL){
                    return false;
                }

                secondCityList->city = cityListToCopy->city;
                secondCityList->prev= previousB;
                if(previousB != NULL) {
                    previousB->next = secondCityList;
                }

                if(previousB == NULL) {
                    startB = secondCityList;
                }

                previousB = secondCityList;

                cityListToCopy = cityListToCopy->next;
            }
            routeB->cityList = startB;


            int dummyY;
            CityList* shortestPath = findShortestPath(map, routeA, routeB, cityA, cityB, &dummyY, road);
            if(shortestPath == NULL){

                return false;
            }
            deleteRoute(routeA);
            deleteRoute(routeB);

            insertPathIntoRoute(shortestPath, road->routesBelonging[i], cityA, cityB); ///< Includes found path in rotue.
        }
    }
    deleteRoadAndTwoRoadLists(road);

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

    if(map->routes[routeId] == NULL){
        char* str = malloc(sizeof(char));
        if(str == NULL){
            return NULL;
        }
        str[0] = 0;
        return str;
    } else {
        int length = count(map->routes[routeId]); ///< Calculates how many characters will be needed.
        char* str = getDescription(map->routes[routeId], length); ///< Creates description.
        return str;
    }
}

