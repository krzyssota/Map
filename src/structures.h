#ifndef DROGI_STRUCTURES_H
#define DROGI_STRUCTURES_H

#include <stdint.h>
#include "map.h"
#include <stdint.h>

#define INF UINT32_MAX

typedef struct City City;
typedef struct Road Road;
typedef struct CityList CityList;
typedef struct RouteList RouteList;
typedef struct RoadList RoadList;
typedef struct Route Route;


typedef struct Route{

    CityList* cityList; ///< wskaźnik na listę miast.
    unsigned routeId; ///< id drogi krajowej.

} Route;

typedef struct RoadList{

    RoadList* prev; ///< wskaźnik na poprzedni element list dróg.
    RoadList* next; ///< wskaźnik na następny element listy dróg.
    Road* road; ///< wskaźnik na drogę.

} RoadList;

struct Road{

    City* cityA; ///< wskaźnik na miasto do którego prowadzi.
    City* cityB; ///< wskaźnik na drugie miasto do którego prowadzi.

    unsigned length; ///< długość drogi.
    int year; ///< rok budowy/remontu drogi.

    Route* routesBelonging[1000]; ///< tablica wskaźników na drogi krajowe, w których zawarta jest droga.
};

typedef struct CityList{

    struct CityList* next;  ///< wskaźnik na następny element listy miast.
    struct CityList* prev;  ///< wskaźnik na poprzedni element listy miast.
    City* city; ///< wskaźnik na miasto

} CityList;

struct City{

    char* name;  ///< wskaźnik na nazwę miasta.
    RoadList* roadList;  ///< wskaźnik na listę dróg.

};

typedef struct QueueElement{

    City* city;  ///< wskaźnik na miasto.
    long int distance;  ///< obecna odległość od miasta źródłowego.
    struct QueueElement* predecessor;  ///< poprzednik na ścieżce z miasta źródłowego na miasta wskazywane przez element.
    Road* oldestRoad;  ///< wskaźnik na najstarsza drogę na tej ścieżce.
    struct QueueElement* next;  ///< wskaźnik na następny element kolejki priorytetowej.
    struct QueueElement* prev; ///< wskaźnik na poprzedni element kolejki priorytetowej.
    bool ambiguous; ///< flaga czy element jest został jednoznacznie wyznaczony - na dwóch różnych ścieżkach uzyskał taki sam priorytet.

} QueueElement;

/** Kolejka priorytetowa.
 */
typedef struct Queue{

    QueueElement* head; ///< wskaźnik na element z najwyższym priorytetem.
    City* destination; ///< wskaźnik na miasto źródłowe.

} Queue;

typedef struct RouteParam{

    int id;

    char** cities;
    int cFilled;
    int cSize;

    unsigned* lengths;
    int lFilled;
    int lSize;

    int* years;
    int yFilled;
    int ySize;

} RouteParam;

RouteParam* newRouteParam(int id);
bool addCityToRouteParam(RouteParam* routeParam, char* cityName);
bool addRoadToRouteParam(RouteParam* routeParam, unsigned roadLength, int year);

/** @brief Tworzy nowy element list dróg.
    @return Zwraca wskaźnik na nowy element. NULL jeśli alokacją nie udała się.
 */
RoadList* newRoadList();

/** @brief Tworzy nowe miasto o podanej nazwie.
 * @param[in] name - wskaźnik na nazwę.
    @return Zwraca wskaźnik na nowe miasto. NULL jeśli alokacją nie udała się.
 */
City* newCity(const char *name);

/** @brief Tworzy nowy element list miast.
   @return Zwraca wskaźnik na nowy element. NULL jeśli alokacją nie udała się.
*/
CityList* newCityList();

/** @brief Tworzy nową drogę krajową o podanym id..
 * @param[in] id - id drogi krajowej.
    @return Zwraca wskaźnik na nową drogę krajową. NULL jeśli alokacją nie udała się.
 */
Route* createNewRoute(unsigned id);
/** @brief Tworzy kolejkę priorytetową o podanym mieście źródłowym.
 * @param[in] destination - wskaźnik na miasto.
    @return Zwraca wskaźnik na nową kolejkę. NULL jeśli alokacją nie udała się.
 */
Queue* newQueue(City* destination);

/** @brief Tworzy nowy element kolejki priorytetowej
 * @param[in] city - wskaźnik na miasto.
 * @param[in] distance - odległość od miasta źródłowego.
 * @param[in] predecessor - poprzednik na ścieżce z miasta źródłowego do miasta.
 * @param[in] road - wskaźnik na najstarszą drogę na powyższej ścieżce.
    @return Zwraca wskaźnik na nowy element kolejki. NULL jeśli alokacją nie udała się.
 */
QueueElement* newQueueElement(City* city, long int distance, QueueElement* predecessor, Road* oldestRoad);
/** @brief Tworzy nową drogę.
   @return Zwraca wskaźnik na nową drogę. NULL jeśli alokacją nie udała się.
*/
Road* createNewRoad();




#endif //DROGI_STRUCTURES_H
