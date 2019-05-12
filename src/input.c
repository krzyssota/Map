#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "map.h"
#include <string.h>


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

void putZeroes(char *line){

    int i = 0;
    int ch = getchar();

    while (ch != '\n' && ch != EOF){

        if(ch == ';'){
            line[i] = 0;
        }

        i++;
        ch = getchar();
    }
}
enum Command getCommand(char* line){

    char command[20];
    strcpy(command, line);

    if(strcmp(command, "addRoute") == 0) return ADD_ROUTE;
    if(strcmp(command, "addRoad") == 0) return ADD_ROAD;
    if(strcmp(command, "repairRoad") == 0) return REPAIR_ROAD;
    if(strcmp(command, "getRouteDescription") == 0) return DESCRIBE_ROUTE;
    return ERROR;

}
void getParametersAndAddRoute(char* line){
    //TODO
}
void getParametersAndAddRoad(char* line, Map* map){

    size_t cityNameLength = 0;
    char* tmp = line;

    while(tmp != 0 && tmp != EOF){
        cityNameLength++;
        tmp++;
    }
    char cityA[cityNameLength];
    strcpy(cityA, line);

    line = &line[strlen(line) + 1];

    cityNameLength = 0;
    tmp = line;

    while(tmp != 0 && tmp != EOF){
        cityNameLength++;
        tmp++;
    }
    char cityB[cityNameLength];
    strcpy(cityB, line);

    line = &line[strlen(line) + 1];





    if (!addRoad(map, (const char*) cityA, (const char*) cityB, unsigned length, int builtYear)){
        handleError();
    }
}
void getParametersAndRepairRoad(char* line){
    //TODO
}
void getParametersAndGetRouteDescription(char* line){
    //TODO
}

void executeCommand(enum Command command, char* line, Map* map){
    switch(command){
        case ADD_ROUTE:
            getParametersAndAddRoute(&line[strlen(line) + 1]);
            break;
        case ADD_ROAD:
            getParametersAndAddRoad(&line[strlen(line) + 1], map);
            break;
        case REPAIR_ROAD:
            getParametersAndRepairRoad(&line[strlen(line) + 1]);
            break;
        case DESCRIBE_ROUTE:
            getParametersAndGetRouteDescription(&line[strlen(line) + 1]);
            break;
        default:
            exit(1);
    }
}

void readInput(Map* map) {

    int lineNo = 0;
    char *line = NULL;
    size_t len = 0;
   /* ssize_t read;*/

    while ((/*read = */getline(&line, &len, stdin)) != -1) {
        /*printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);*/
        if(line[0] == '#' || line[0] == 0){
            continue;
        }
        putZeroes(line);
        enum Command command = getCommand(line);
        if(command == ERROR){
            handleError();
        }
        executeCommand(command, line, map);


        lineNo++;
    }
    
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}



