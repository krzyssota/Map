#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "map.h"
#include <string.h>
#include "additionalFunctions.h"


enum Command {ADD_ROUTE, ADD_ROAD, REPAIR_ROAD, DESCRIBE_ROUTE, ERROR};
/*
 numer drogi krajowej;nazwa miasta;długość odcinka drogi;rok budowy lub ostatniego remontu;nazwa miasta

 Format polecenia jest taki sam, jak w wyniku funkcji getRouteDescription.
 To polecenie tworzy drogę krajową o podanym numerze i przebiegu.
 Jeśli jakieś miasto lub odcinek drogi nie istnieje, to go tworzy.
 Jeśli odcinek drogi już istnieje, ale ma wcześniejszy rok budowy lub ostatniego remontu, to modyfikuje ten atrybut odcinka drogi.
 Za błąd uznajemy, jeśli odcinek drogi już istnieje, ale ma inną długość albo późniejszy rok budowy lub ostatniego remontu.
 To polecenie niczego nie wypisuje na standardowe wyjście.


addRoad;city1;city2;length;builtYear
        Wywołuje na mapie dróg funkcję addRoad z podanymi parametrami. Niczego nie wypisuje na standardowe wyjście.


repairRoad;city1;city2;repairYear
        Wywołuje na mapie dróg funkcję repairRoad z podanymi parametrami. Niczego nie wypisuje na standardowe wyjście.


getRouteDescription;routeId
        Wywołuje na mapie dróg funkcję getRouteDescription z podanym parametrem.
        Jeśli wynik działania tej funkcji jest inny niż NULL, to wypisuje na standardowe wyjście jedną linię z wynikiem działania tej funkcji.


 Każde polecenie znajduje się w osobnej linii.
 Puste linie i linie zaczynające się znakiem # należy ignorować.
 W poleceniach nazwa miasta jest niepustym napisem niezawierającym kodów od 0 do 31 ani średnika,
 liczby są zapisywane przy podstawie 10. Spacje w nazwie miasta są istotne.
*/

bool putZeroes(char *line, size_t lineLength){

    if(line[lineLength - 1] == ';'){
        return false;
    }
    int i = 0;
    int ch = line[i];

    while (ch != '\n' && ch != EOF){

        if(ch == ';'){
            line[i] = 0;
        }

        i++;
        ch = line[i];
    }
    if(ch == '\n'){
        line[i] = 0;
        return true;
    }
    return false;
}
enum Command getCommand(char* line){

    char* command = line;

    if(strcmp(command, "addRoute") == 0) return ADD_ROUTE;
    if(strcmp(command, "addRoad") == 0) return ADD_ROAD;
    if(strcmp(command, "repairRoad") == 0) return REPAIR_ROAD;
    if(strcmp(command, "getRouteDescription") == 0) return DESCRIBE_ROUTE;
    return ERROR;

}
void getParametersAndAddRoute(char* line, size_t lineLength, Map* map){
    // numer drogi krajowej;nazwa miasta;długość odcinka drogi;rok budowy lub ostatniego remontu;nazwa miasta
    char* end = NULL;
    unsigned id = (int)strtol(line, &end, 10);
    if(*end != 0){
        handleError();
        return;
    }

    lineLength -= (digitsInNumber(id)+1);
    if(lineLength != 0){
        handleError();
        return;
    }

    char* firstCity = line;

    lineLength -= (strlen(firstCity)+1);
    if(lineLength == 0){
        handleError();
        return;
    }
    line = &line[strlen(firstCity) + 1];

    RouteParam* routeParam = newRouteParam();
    addCityToRouteParam(routeParam, firstCity);

    bool moreArg = true;
    while(moreArg){

    }

}
void getParametersAndAddRoad(char* line, size_t lineLength, Map* map){

    if(lineLength == 0){
        handleError();
    }

    char* cityA = line;

    lineLength -= (strlen(cityA)+1);
    if(lineLength == 0){
        handleError();
        return;
    }
    line = &line[strlen(cityA) + 1];

    char* cityB = line;

    lineLength -= (strlen(cityB)+1);
    if(lineLength == 0){
        handleError();
        return;
    }
    line = &line[strlen(cityB) + 1];

    char* end = NULL;
    int roadLength = (int)strtol(line, &end, 10);
    if(*end != 0){
        handleError();
        return;
    }

    lineLength -= (digitsInNumber(roadLength)+1);
    if(lineLength == 0){
        handleError();
        return;
    }

    line = &line[strlen(line) + 1];
    end = NULL;
    int year = (unsigned) strtol(line, &end, 10);
    if(*end != 0){
        handleError();
        return;
    }

    lineLength -= (digitsInNumber(year)+1);
    if(lineLength != 0){
        handleError();
        return;
    }

    if(!addRoad(map, (const char*) cityA, (const char*) cityB, roadLength, year)){
        handleError();
    } else {
        printf("road added");
    }
}
void getParametersAndRepairRoad(char* line, size_t lineLength, Map* map){
    if(lineLength == 0){
        handleError();
    }

    char* cityA = line;

    lineLength -= (strlen(cityA)+1);
    if(lineLength == 0){
        handleError();
        return;
    }
    line = &line[strlen(cityA) + 1];

    char* cityB = line;

    lineLength -= (strlen(cityB)+1);
    if(lineLength == 0){
        handleError();
        return;
    }
    line = &line[strlen(cityB) + 1];

    char* end = NULL;
    int repairYear = (int)strtol(line, &end, 10);
    if(*end != 0){
        handleError();
        return;
    }

    lineLength -= (digitsInNumber(repairYear)+1);
    if(lineLength != 0){
        handleError();
        return;
    }

    if(!repairRoad(map, (const char*) cityA, (const char*) cityB, repairYear)){
        handleError();
    } else {
        printf("road repaired");
    }
}
void getParametersAndGetRouteDescription(char* line, size_t lineLength, Map* map){
    // getRouteDescription;routeId
    char* end = NULL;
    unsigned id = (int)strtol(line, &end, 10);
    if(*end != 0){
        handleError();
        return;
    }

    lineLength -= (digitsInNumber(id)+1);
    if(lineLength != 0){
        handleError();
        return;
    }

    if(printf("%s", getRouteDescription(map, id))){
    } else {
        handleError();
    }
}

void executeCommand(enum Command command, char* line, size_t lineLength, Map* map){
    switch(command){
        case ADD_ROUTE:
            getParametersAndAddRoute(&line[strlen(line) + 1]);
            break;
        case ADD_ROAD:
            getParametersAndAddRoad(&line[strlen(line) + 1], lineLength-(strlen(line) + 1), map);
            break;
        case REPAIR_ROAD:
            getParametersAndRepairRoad(&line[strlen(line) + 1], lineLength-(strlen(line) + 1), map);
            break;
        case DESCRIBE_ROUTE:
            getParametersAndGetRouteDescription(&line[strlen(line) + 1], lineLength-(strlen(line) + 1), map);
            break;
        default:
            exit(1);
    }
}

void readInput(Map* map) {

    int lineNo = 0;
    char *line = NULL;
    ssize_t lineLength = 0;
    size_t dummy;

    do {
        lineLength = getline(&line, &dummy, stdin);
        lineNo++;

        if(line[0] == '#' || line[0] == '\n'){
            continue;
        }

        if(!putZeroes(line, lineLength)){
            handleError(lineLength);
            continue;
        }

        enum Command command = getCommand(line);

        if(command == ERROR){
            handleError(lineLength);
            continue;
        }

        executeCommand(command, line, lineLength--, map);

        lineNo++;

    } while (lineLength  != -1);
    
    if (line){
        free(line);
    }
}



