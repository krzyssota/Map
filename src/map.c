#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

// lista miast, kazde ma liste dróg, miasto ma wskaznik na poczatek (i koniec ? ) drogi krajowej na ktorej sie znajduje

typedef struct City City; // kazde miasto ma liste drog
typedef struct Road Road; // miastA miastoB
typedef struct CityList CityList; // lista miast
typedef struct RouteList RouteList; // kazda route ma liste miast
typedef struct RoadList RoadList;

typedef struct Route{

    CityList* cities;
    unsigned routeId;


} Route;

typedef struct RouteList{

    RouteList* prevRoute;
    RouteList* nextRoute;
    Route* route;

} RouteList;


typedef struct RoadList{

    RoadList* prev;
    RoadList* next;
    Road* road;

} RoadList;

struct Road{

    City* cityA;
    City* cityB;

    unsigned length;
    int year;

    Route** routes; // TODO maybe delete
};

typedef struct CityList{

    struct CityList* next;
    struct CityList* prev;
    City* city;

} CityList;

struct City{

    char* name;
    RoadList* roadList;
    int proximity;

};




typedef struct Map{

    CityList* cityList;
    RouteList* routeList;

} Map;


/** @brief Tworzy nową strukturę.
 * Tworzy nową, pustą strukturę niezawierającą żadnych miast, odcinków dróg ani
 * dróg krajowych.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Map* newMap(void){
    Map* newMap = malloc(sizeof(Map));
    newMap->cityList = NULL;
    newMap->routeList = NULL;
    return newMap;
}


/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p map.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] map        – wskaźnik na usuwaną strukturę.
 */
void deleteMap(Map *map){
    deleteCityList(map->cityList);
    deleteRouteList(map->routeList);
    free(map);
}

// ------------------------------------------------------------------------------
bool findCity(CityList* clist, const char* cityString, City* result) {

    while(clist != NULL && strcmp(clist->city->name, cityString) != 0){
        clist = clist->next;
    }
    result = clist->city;

    if(result == NULL) return false;
    return true;
}
// ------------------------------------------------------------------------------
bool roadToList(RoadList* rlist, Road* newRoad){

    while(rlist->next != NULL && rlist->road != newRoad) rlist = rlist->next;

    if(rlist->road == newRoad) return false;

    rlist->next = malloc(sizeof(RoadList));
    (rlist->next)->next = NULL;
    (rlist->next)->prev = rlist;
    (rlist->next)->road = newRoad;

    return true;
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

    if(strcmp(city1, city2) == 0){
        return false;
    }

    City* cityA;
    City* cityB;
    if(!findCity(map->cityList, city1, cityA) && !findCity(map->cityList, city2, cityB)) return false;

    Road* newRoad = malloc(sizeof(Road));
    if(!newRoad) return false;

    newRoad->cityA = cityA;
    newRoad->cityB = cityB;

    newRoad->length = length;
    newRoad->year = builtYear;

    if(!roadToList(cityA->roadList, newRoad)){
        free(newRoad);
        return false;
    }
    roadToList(cityB->roadList, newRoad);

    return true;
}
// ----------------------------------------------------------------------------------
bool findRoad(RoadList* rlist, City* city, Road* rfound){

    while(rlist->next != NULL && rlist->road->cityB != city) rlist = rlist->next;

    rfound = rlist->road;

    if(rlist->road->cityB == city){
        return true;
    }
    return false;

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

    City *cityA;
    if(!findCity(map->cityList, city1, cityA)){
        return false;
    }
    City *cityB;
    if(!findCity(map->cityList, city2, cityB)){
        return false;
    }
    Road* road;
    if(!findRoad(cityA->roadList, cityB, road)){
        return false;
    }
    if(repairYear >= road->year){
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

    if(strcmp(city1, city2) == 0) return false;


    return false;
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
    return false;
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

bool correctName(char* cityName){

    uint32_t counter = 0;
    char* tmp = cityName;

    while((int)(*tmp) > 31 && (int)(*tmp) != 59){
        tmp++;
        counter++;
    }
    return (counter == strlen(cityName) && (int)(*tmp) == 0);

}
