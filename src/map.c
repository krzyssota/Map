#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "structures.h"
#include "stringsHandling.h"
#include "roadsRelated.h"
#include "deleteStructure.h"
#include "routeRelated.h"
#include <limits.h>



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



void deleteMap(Map *map){

    if(map != NULL) {

        deleteCitiesRoads(map, map->cityList);
        deleteRoutes(map->routes);

        deleteLine(map->inputLine);

        free(map);
    }
}


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
    CityList* shortestPathAToB = findShortestPath(map, newRoute, dummyRoute, cityA, cityB, &dummyYear, dummyRoad);
    CityList* shortestPathBToA = findShortestPath(map, newRoute, dummyRoute, cityB, cityA, &dummyYear, dummyRoad);
    if(shortestPathAToB == NULL || shortestPathBToA == NULL){ ///< Nie udalo sie znalezc najkrotszego polaczenia.

        deleteCityList(shortestPathAToB);
        deleteCityList(shortestPathBToA);

        deleteRoute(newRoute);
        return false;
    }

    deleteCityList(shortestPathBToA);

    newRoute->cityList = shortestPathAToB;

    addRouteInfoToRoads(newRoute); ///< Drogi maja informacje do jakich drog krajowych naleza.

    map->routes[newRoute->routeId] = newRoute;

    return true;
}


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

    CityList* borderCityList = map->routes[routeId]->cityList;

    /** Rozwaza dwie mozliwosci przedluzenia drogi krajowej.
     */
    int firstOldestRoadYear = INT_MIN;
    Route* dummyRoute = NULL;
    Road* dummyRoad = NULL;

    CityList* firstPathAToB = findShortestPath(map, map->routes[routeId], dummyRoute,
                                              additionalCity, borderCityList->city, &firstOldestRoadYear, dummyRoad);
    CityList* firstPathBToA = findShortestPath(map, map->routes[routeId], dummyRoute,
                                              borderCityList->city, additionalCity, &firstOldestRoadYear, dummyRoad);
    if(firstPathAToB == NULL || firstPathBToA == NULL){
        deleteCityList(firstPathAToB);
        deleteCityList(firstPathBToA);
        return false;
    }

    unsigned firstLength = calculateLength(firstPathAToB);

    while(borderCityList->next != NULL){
        borderCityList = borderCityList->next;
    }
    int secondOldestRoadYear = INT_MIN;

    CityList* secondPathAToB = findShortestPath(map, map->routes[routeId], dummyRoute,
                                                  borderCityList->city, additionalCity, &secondOldestRoadYear, dummyRoad);
    CityList* secondPathBToA = findShortestPath(map, map->routes[routeId], dummyRoute,
                                                  additionalCity, borderCityList->city, &secondOldestRoadYear, dummyRoad);
    if(secondPathAToB == NULL || secondPathBToA == NULL){
        deleteCityList(secondPathAToB);
        deleteCityList(secondPathBToA);
        return false;
    }

    unsigned secondLength = calculateLength(secondPathAToB);


    switch(betterPath(firstOldestRoadYear, firstLength, secondOldestRoadYear, secondLength)){
        case 1: {
            CityList *endOfPath = firstPathAToB;
            while (endOfPath->next != NULL) {
                endOfPath = endOfPath->next;
            }

            endOfPath->next = map->routes[routeId]->cityList->next;
            (map->routes[routeId]->cityList->next)->prev = endOfPath;

            map->routes[routeId]->cityList = firstPathAToB;

            deleteCityList(firstPathBToA);

            deleteCityList(secondPathAToB);
            deleteCityList(secondPathBToA);
            break;
        }
        case 2: {

            CityList *endOfRoute = map->routes[routeId]->cityList;
            while (endOfRoute->next != NULL) {
                endOfRoute = endOfRoute->next;
            }

            endOfRoute->next = secondPathAToB->next;
            (secondPathAToB->next)->prev = endOfRoute;

            deleteCityList(firstPathAToB);
            deleteCityList(firstPathBToA);

            deleteCityList(secondPathBToA);
            break;
        }
        case 0: {

            deleteCityList(firstPathAToB);
            deleteCityList(firstPathBToA);

            deleteCityList(secondPathAToB);
            deleteCityList(secondPathBToA);
            return false;
        }
        default: {
            exit(1);
        }
    }
    addRouteInfoToRoads(map->routes[routeId]);

    return true;
}


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
            if(routeA == NULL){
                return false;
            }

            CityList* cityListA = NULL;
            CityList* previous = NULL;

            CityList* startA = NULL;


            CityList* cityListToCopy = road->routesBelonging[i]->cityList;

            bool last = false;
            while(!last){ ///< Skopiuj miasta z drogi krajowej az do poczatku usunietej drogi.

                cityListA = newCityList();
                if(cityListA == NULL){
                    deleteRoute(routeA);
                    return false;
                }

                cityListA->city = cityListToCopy->city;
                cityListA->prev = previous;
                if(previous != NULL){
                    previous->next = cityListA;
                } else {
                    startA = cityListA;
                }

                previous = cityListA;

                if(cityListToCopy->city == road->cityA || cityListToCopy->city == road->cityB){
                    last = true;
                }

                cityListToCopy = cityListToCopy->next;
            }
            routeA->cityList = startA;

            cityListToCopy = cityListToCopy->next;

            Route* routeB = createNewRoute(1000);

            CityList* cityListB = NULL;
            CityList* previousB = NULL;
            CityList* startB = NULL;



            while(cityListToCopy != NULL){ ///< Skopiuj miasta z drogi krajowej od konca usunietej drogi.

                cityListB = newCityList();
                if(cityListB == NULL){
                    deleteRoute(routeA);
                    deleteRoute(routeB);
                    return false;
                }

                cityListB->city = cityListToCopy->city;

                cityListB->prev= previousB;
                if(previousB != NULL) {
                    previousB->next = cityListB;
                } else {
                    startB = cityListB;
                }

                previousB = cityListB;

                cityListToCopy = cityListToCopy->next;
            }
            routeB->cityList = startB;


            int dummyY;
            CityList* shortestPathAToB = findShortestPath(map, routeA, routeB, cityA, cityB, &dummyY, road);
            CityList* shortestPathBToA = findShortestPath(map, routeA, routeB, cityB, cityA, &dummyY, road);
            if(shortestPathAToB == NULL || shortestPathBToA == NULL){

                deleteCityList(shortestPathAToB);
                deleteCityList(shortestPathBToA);

                deleteRoute(routeA);
                deleteRoute(routeB);

                return false;
            }

            deleteCityList(shortestPathBToA);
            deleteRoute(routeA);
            deleteRoute(routeB);

            insertPathIntoRoute(shortestPathAToB, road->routesBelonging[i], cityA, cityB); ///< Includes found path in rotue.
        }
    }
    deleteRoadAndTwoRoadLists(road);

    return true;
}

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

bool removeRoute(Map *map, unsigned routeId){

    if(!correctId(routeId) || map->routes[routeId] == NULL){
        return false;
    }
    deleteRouteInfoFromRoad(map->routes[routeId]);
    Route* tmp =  map->routes[routeId];
    map->routes[routeId] = NULL;
    deleteRoute(tmp);

    return true;
}

/*Usuwa z mapy dróg drogę krajową o podanym numerze,
jeśli taka istnieje, dając wynik true,
a w przeciwnym przypadku, tzn. gdy podana droga krajowa nie istnieje lub podany numer jest niepoprawny,
niczego nie zmienia w mapie dróg, dając wynik false.
Nie usuwa odcinków dróg ani miast.*/

