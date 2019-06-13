/**
 * @file
 * Moduł zawiera struktury i udostępnia funkcję do ich tworzenia i przetwarzania.
 * @author Krzysztof Sota
 * @date 18.05.2019
 * */
#ifndef DROGI_STRUCTURES_H
#define DROGI_STRUCTURES_H

#include <stdint.h>
#include "map.h"
#include <stdint.h>

/**
 * Infinity in Dijkstra algorithm.
 */
#define INF UINT32_MAX

/**
 * Struktura przechowująca miasta przez, które przechodzi droga krajowa o podanym numerze id.
 */
typedef struct Route{

    struct CityList* cityList; ///< wskaźnik na listę miast.
    unsigned routeId; ///< id drogi krajowej.

} Route;
/**
 * Lista dwukierunkowa przechowująca wskaźniki na drogi.
 */
typedef struct RoadList{

    struct RoadList* prev; ///< wskaźnik na poprzedni element list dróg.
    struct RoadList* next; ///< wskaźnik na następny element listy dróg.
    struct Road* road; ///< wskaźnik na drogę.

} RoadList;
/**
 * Struktura określająca parametry drogi, jakie miasta łączy i jakich dróg krajowych jest częścią.
 */
typedef struct Road{

    struct City* cityA; ///< wskaźnik na miasto do którego prowadzi.
    struct City* cityB; ///< wskaźnik na drugie miasto do którego prowadzi.

    unsigned length; ///< długość drogi.
    int year; ///< rok budowy/remontu drogi.

    struct Route* routesBelonging[1000]; ///< tablica wskaźników na drogi krajowe, w których zawarta jest droga.
} Road;
/**
 * Lista dwukierunkowa przechowująca wskaźniki na miasta.
 */
typedef struct CityList{

    struct CityList* next;  ///< wskaźnik na następny element listy miast.
    struct CityList* prev;  ///< wskaźnik na poprzedni element listy miast.
    struct City* city; ///< wskaźnik na miasto

} CityList;
/**
 * Struktura przetrzymuję nazwę miasta i listę dróg, które z niego wychodzą.
 */
typedef struct City{

    char* name;  ///< wskaźnik na nazwę miasta.
    struct RoadList* roadList;  ///< wskaźnik na listę dróg.

} City;
/**
 * Element kolejki priorytetowej. Zawiera priorytet i informacje potrzebne do odtworzenia drogi po wykorzystaniu algorytmu Dijkstry.
 */
typedef struct QueueElement{

    struct City* city;  ///< wskaźnik na miasto.
    long int distance;  ///< obecna odległość od miasta źródłowego.
    struct QueueElement* predecessor;  ///< poprzednik na ścieżce z miasta źródłowego na miasta wskazywane przez element.
    struct Road* oldestRoad;  ///< wskaźnik na najstarsza drogę na tej ścieżce.
    struct QueueElement* next;  ///< wskaźnik na następny element kolejki priorytetowej.
    struct QueueElement* prev; ///< wskaźnik na poprzedni element kolejki priorytetowej.
    bool ambiguous; ///< flaga czy element jest został jednoznacznie wyznaczony - na dwóch różnych ścieżkach uzyskał taki sam priorytet.

} QueueElement;

/** Kolejka priorytetowa.
 */
typedef struct Queue{

    struct QueueElement* head; ///< wskaźnik na element z najwyższym priorytetem.
    struct City* target; ///< wskaźnik na miasto, do którego szukana będzie najkrótsza droga.

} Queue;
/**
 * Typ wyliczeniowy służący oznaczeniu wyniku szukania optymalnej drogi krajowej.
 */
typedef enum PathResult{
    AMBIGUOUS, ///< droga krajowa wyznaczona niejednoznacznie.
    NOT_FOUND, ///< droga krajowa nieznaleziona.
    FOUND ///< droga krajowa znaleziona i jednoznaczna.
} PathResult;
 /**
  * Struktura opakowująca znaleziona drogę krajową.
  */
typedef struct ShortestPathResult{
    struct CityList* path; ///< lista wskaźników na miasta przez które przebiega droga krajowa.
    unsigned length; ///< długość znalezionej drogi.
    int oldestRoadYear; ///< wiek najstarszej drogi.
    PathResult resultEnum; ///< znalezienie i jednoznaczość drogi krajowej.

} ShortestPathResult;

/**
 * Struktura przechowująca listę nazw miast, które mają zostać zawarte w drodze krajowe,
 * oraz długości i lata budowy/remontu dróg je łączących.
 */
typedef struct RouteParam{

    unsigned id; ///< id potencjalnej drogi krajowej.

    char** cities; ///< Podwójny wskaźnik na nazwy miast.
    int cFilled; ///< Ile nazw znajduje się w strukturze.
    int cSize; ///< Na ile nazw zaalokowano miejsce w pamięci.

    unsigned* lengths; ///< Wskaźnik na długości dróg.
    int lFilled; ///< Ile liczb odpowiadających długości znajduje się w strukturze.
    int lSize; ///< Na ile liczb odpowiadających długości zaalokowano miejsce w pamięci.

    int* years; ///< Wskaźnik na lata budowy/remontów dróg.
    int yFilled; ///< Ile liczb odpowiadających rokom budowy/remontu znajduje się w strukturze.
    int ySize; ///< Na ile odpowiadających rokom budowy/remontu zaalokowano miejsce w pamięci.

} RouteParam;
/** @brief Tworzy nowy wzór drogi krajowej zwarty w routeParam o podanym id.
 * @param[in] id - liczba całkowita od 1 do 999
    @return Zwraca wskaźnik na nowy routeParam. NULL jeśli alokacją nie udała się.
 */
RouteParam* newRouteParam(unsigned id);

/**@brief Dodaje nazwę miasta do routeParam.
 * @param[in, out] routeParam - wskaźnik na wskaźnik strukturę routeParam
 * @param[in] cityName - wskaźnik na nazwę miasta
 * @return Wartość @p true jeśli udało się dodać miasto. @p false jeśli realokacją nie udała się.
 */
bool addCityToRouteParam(RouteParam* routeParam, char* cityName);

/**@brief Dodaje parametry drogi do routeParam.
 * @param[in, out] routeParam - wskaźnik na wskaźnik na strukturę routeParam
 * @param[in] roadLength - długość drogi
 * @param[in] year - rok budowy drogi
 * @return Wartość @p true jeśli udało się dodać parametry drogi. @p false jeśli realokacją nie udała się.
 */
bool addRoadToRouteParam(RouteParam* routeParam, unsigned roadLength, int year);

/**@brief Sprawdza czy nazwa miasta podana w argumencie jest już zapisana w routeParam
 * @param[in, out] routeParam - wskaźnik na strukturę routeParam
 * @param[in] cityName - wskaźnik na nazwę miasta
 * @return Wartość @p true występuje, @p false jeśli nie.
 */
bool cityAlreadyInRouteParam(RouteParam* routeParam, char* cityName);

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

/** @brief Tworzy nową drogę krajową o podanym id.
 * @param[in] id - id drogi krajowej.
    @return Zwraca wskaźnik na nową drogę krajową. NULL jeśli alokacją nie udała się.
 */
Route* createNewRoute(unsigned id);
/** @brief Tworzy kolejkę priorytetową o podanym mieście źródłowym.
 * @param[in] target - wskaźnik na miasto.
    @return Zwraca wskaźnik na nową kolejkę. NULL jeśli alokacją nie udała się.
 */
Queue* newQueue(City* target);

/** @brief Tworzy nowy element kolejki priorytetowej
 * @param[in] city - wskaźnik na miasto.
 * @param[in] distance - odległość od miasta źródłowego.
 * @param[in] predecessor - poprzednik na ścieżce z miasta źródłowego do miasta.
 * @param[in] oldestRoad - wskaźnik na najstarszą drogę na powyższej ścieżce.
    @return Zwraca wskaźnik na nowy element kolejki. NULL jeśli alokacją nie udała się.
 */
QueueElement* newQueueElement(City* city, long int distance, QueueElement* predecessor, Road* oldestRoad);

/** @brief Tworzy nową drogę.
   @return Zwraca wskaźnik na nową drogę. NULL jeśli alokacją nie udała się.
*/
Road* createNewRoad();
/**Tworzy nową instancje struktury ShortestPathResult.
 * @return Wskaźnik na nową strukturę. NULL jeśli alokacja nie udała się.
 */
ShortestPathResult* newShortestPathResult();




#endif //DROGI_STRUCTURES_H
