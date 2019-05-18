/**
 * @file
 * Moduł zawiera funkcję potrzebne do przetwarzania standardowego wejścia.
 * @author Krzysztof Sota
 * @date 18.05.2019
 * */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "input.h"
#include "map.h"
#include <string.h>
#include <values.h>
#include <errno.h>
#include "stringsHandling.h"
#include "routeRelated.h"
#include "deleteStructure.h"

/**@brief typ wyliczeniowy możliwych komend z wejścia.
 */
typedef enum Command {
    ADD_ROUTE,
    ADD_ROAD,
    REPAIR_ROAD,
    DESCRIBE_ROUTE,
    IGNORE, ///< komentarz, lub pusta linijki
    ERROR
} Command;

/**@brief Zczytuje nazwę miasta z podanego napisu.
 * @param[in, out] line - wskaźnik na wskaźnik na przetwarzany napis
 * @param[in, out] lineLength - wskaźnik na pozostała do przetworzenia długośc wiersza
 * @param[in, out] city - wskaźnik na wskaźnik na nazwę miasta
 */
void readCity(char** line, ssize_t* lineLength, char** city){

    *city = *line;

    *lineLength -= strlen(*city);
    if(*lineLength != 0)  *lineLength -= 1;

    *line = &(*line)[strlen(*city) + 1];
}
/**@brief Zczytuje dodatnią liczbę z podanego napisu.
 * @param[in, out] line - wskaźnik na wskaźnik na przetwarzany napis
 * @param[in, out] lineLength - wskaźnik na pozostała do przetworzenia długośc wiersza
 * @param[in, out] number - wskaźnik na liczbę całkowitą
 * @return Wartość @p true jeśli operacja powiodła się, @p false w przeciwnym przypadku.
 */
bool readPositiveNumber(char **line, ssize_t *lineLength, unsigned *number){

    char* end = NULL;

    errno = 0;
    *number = (int)strtol(*line, &end, 10);
    if(*end != 0 || errno != 0){
        return false;
    }

    size_t digits = digitsInNumber(*number);

    *lineLength -= digits;
    if(*lineLength != 0)  *lineLength -= 1;
    *line = &(*line)[digits + 1];
    return true;
}
/**@brief Zczytuje liczbę z podanego napisu.
 * @param[in, out] line - wskaźnik na wskaźnik na przetwarzany napis
 * @param[in, out] lineLength - wskaźnik na pozostała do przetworzenia długośc wiersza
 * @param[in, out] year - wskaźnik na liczbę całkowitą
 * @return Wartość @p true jeśli operacja powiodła się i wartość pod adresem wskazywanym przez year ma dopuszczalną wartość,
 *          @p false w przeciwnym przypadku.
 */
bool readYear(char **line, ssize_t *lineLength, int *year){

    char* end = NULL;
    
    bool negative = false;
    if(**line == '-'){
        negative = true;
    }

    errno = 0;
    int64_t number = 0;
    number =
            (int)strtol(
            *line,
            &end,
            10);

    if(*end != 0) {
        return false;
    }
    if(errno == ERANGE && (number == LONG_MIN || number == LONG_MAX)){
        return false;
    }
    if(number == 0 || number <  INT_MIN || number > INT_MAX || (negative && number > 0) || (!negative && number < 0)){
        return false;
    }

    *year = number;

    size_t digitsInYear;
    if(negative) {
        digitsInYear = digitsInNumber(*year * (-1));
    } else {
        digitsInYear= digitsInNumber(*year);
    }

    if(negative){
        digitsInYear++;
    }

    *lineLength -= digitsInYear;
    if(*lineLength != 0)  *lineLength -= 1;


    *line = &(*line)[digitsInYear + 1];

    return true;
}


/**@brief Z napisu zczytuje parametry potrzebne do stworzenia drogi krajowej i, jeśli to możliwe, robi to.
 * @param[in, out] line - wskaźnik na przetwarzany napis
 * @param[in, out] lineLength - pozostała do przetworzenia długośc wiersza
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] lineNo - numer przetwarzanego wiersza
 */
void getParametersAndAddRoute(char* line, ssize_t lineLength, Map* map, int lineNo){

    if(lineLength == 0){
        handleError(lineNo);
        return;
    }


    unsigned id;
    if(!readPositiveNumber(&line, &lineLength, &id) || !correctId(id) || (lineLength == 0)){
        handleError(lineNo);
        return;
    }

    char* firstCity;
    readCity(&line, &lineLength, &firstCity);
    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    RouteParam* routeParam = newRouteParam(id);
    if(!addCityToRouteParam(routeParam, firstCity)){
        deleteRouteParam(routeParam);
        exit(0);
    }

    while(lineLength > 0){

        unsigned roadLength = 0;
        if(!readPositiveNumber(&line, &lineLength, &roadLength) || roadLength <= 0 || lineLength == 0){
            handleError(lineNo);
            deleteRouteParam(routeParam);
            return;
        }

        int year = 0;
        if(!readYear(&line, &lineLength, &year) || year == 0 || lineLength == 0){
            handleError(lineNo);
            deleteRouteParam(routeParam);
            return;
        }

        if (!addRoadToRouteParam(routeParam, roadLength, year)){
            deleteRouteParam(routeParam);
            exit(0);
        }

        char* city;
        readCity(&line, &lineLength, &city);

        if(cityAlreadyInRouteParam(routeParam, city)){
            handleError(lineNo);
            deleteRouteParam(routeParam);
            return;
        }


        if( !addCityToRouteParam(routeParam, city)){
            deleteRouteParam(routeParam);
            exit(0);
        }
    }

    if(!newRouteFromRouteParam(map, routeParam) || lineLength > 0){
        handleError(lineNo);
        deleteRouteParam(routeParam);
        return;
    }

    deleteRouteParam(routeParam);

}
/**@brief Z napisu zczytuje parametry potrzebne do stworzenia drogi i, jeśli to możliwe, robi to.
 * @param[in, out] line - wskaźnik na przetwarzany napis
 * @param[in, out] lineLength - pozostała do przetworzenia długośc wiersza
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] lineNo - numer przetwarzanego wiersza
 */
void getParametersAndAddRoad(char* line, ssize_t lineLength, Map* map, int lineNo){

    if(lineLength == 0){
        handleError(lineNo);
    }

    char* cityA;
    readCity(&line, &lineLength, &cityA);
    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    char* cityB;
    readCity(&line, &lineLength, &cityB);
    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    unsigned roadLength = 0;
    if(!readPositiveNumber(&line, &lineLength, &roadLength) || roadLength <= 0 ||  lineLength == 0){
        handleError(lineNo);
        return;
    }

    int year = 0;
    if(!readYear(
            &line,
            &lineLength,
            &year)
            || year == 0
            || lineLength > 0){
        handleError(lineNo);
        return;
    }

    if(!addRoad(map, (const char*) cityA, (const char*) cityB, roadLength, year)){
        handleError(lineNo);
    }
}
/**@brief Z napisu zczytuje parametry potrzebne do remontu drogi i, jeśli to możliwe, robi to.
 * @param[in, out] line - wskaźnik na przetwarzany napis
 * @param[in, out] lineLength - pozostała do przetworzenia długośc wiersza
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] lineNo - numer przetwarzanego wiersza
 */
void getParametersAndRepairRoad(char* line, ssize_t lineLength, Map* map, int lineNo){

    if(lineLength == 0){
        handleError(lineNo);
    }

    char* cityA;
    readCity(&line, &lineLength, &cityA);
    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    char* cityB;
    readCity(&line, &lineLength, &cityB);
    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    int repairYear;
    if(!readYear(&line, &lineLength, &repairYear) || repairYear == 0 || lineLength > 0){
        handleError(lineNo);
        return;
    }

    if(!repairRoad(map, (const char*) cityA, (const char*) cityB, repairYear)){
        handleError(lineNo);
    }
}
/**@brief Wyświetla opis drogi krajowej o podanym id.
 * @param[in, out] line - wskaźnik na przetwarzany napis
 * @param[in, out] lineLength - pozostała do przetworzenia długośc wiersza
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] lineNo - numer przetwarzanego wiersza
 */
void getParametersAndGetRouteDescription(char* line, ssize_t lineLength, Map* map, int lineNo){

    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    unsigned id;
    if(!readPositiveNumber(&line, &lineLength, &id) || !correctId(id) || lineLength > 0){
        handleError(lineNo);
        return;
    }

    char* description = (char*)getRouteDescription(map, id);

    if(description != NULL){
        printf("%s\n", description);
        free(description);
    } else {
        handleError(lineNo);
    }
}
/**@brief Wykonuje zadaną komendę.
 * @param[in] command - typ wyliczeniowy komendy
* @param[in, out] line - wskaźnik na przetwarzany napis
 * @param[in, out] lineLength - wskaźnik na pozostała do przetworzenia długośc wiersza
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] lineNo - numer przetwarzanego wiersza
 */
 void executeCommand(Command command, char* line, ssize_t lineLength, Map* map, int lineNo){
    switch(command){

        case ADD_ROUTE:
            getParametersAndAddRoute(line, lineLength, map, lineNo);
            break;

        case ADD_ROAD:
            getParametersAndAddRoad(&line[strlen(line) + 1], lineLength-(strlen(line) + 1), map, lineNo);
            break;

        case REPAIR_ROAD:
            getParametersAndRepairRoad(&line[strlen(line) + 1], lineLength-(strlen(line) + 1), map, lineNo);
            break;

        case DESCRIBE_ROUTE:
            getParametersAndGetRouteDescription(&line[strlen(line) + 1], lineLength-(strlen(line) + 1), map, lineNo);
            break;

        case IGNORE:
            break;

        case ERROR:
            handleError(lineNo);
            break;

        default:
            exit(1);
    }
}
/**@brief Rozpoznaje komendę z napisu.
* @param[in] line - wskaźnik na przetwarzany napis
 * @return odpowiedni typ wyliczeniowy.
*/
Command getCommand(char* line){

    if(line[0] == '#' || line[0] == '\n'){
        return IGNORE;
    }
    
    if(strcmp(line, "addRoad") == 0) return ADD_ROAD;
    if(strcmp(line, "repairRoad") == 0) return REPAIR_ROAD;
    if(strcmp(line, "getRouteDescription") == 0) return DESCRIBE_ROUTE;

    char* end;
    strtol(line, &end, 10);
    if(*end == 0){
        return ADD_ROUTE;
    }

    return ERROR;

}
/**@brief W napisie podmienia ';' i '\n' na '\0'.
* @param[in] line - wskaźnik na przetwarzany napis
 * @param[in] lineLength - wskaźnik na pozostała do przetworzenia długośc wiersza
 * */
bool segmentLine(char *line, ssize_t lineLength){

    if(line[lineLength - 1] == ';'){
        return false;
    }

    int i = 0;
    int ch = line[i];

    while (ch != '\n' && lineLength > 0){

        if(ch == ';'){
            line[i] = 0;
        }

        i++;
        lineLength--;
        ch = line[i];
    }
    if(ch == '\n'){
        line[i] = 0;
        return true;
    }
    return false;
}

void readExecuteInput(Map *map) {

    int lineNo = 0;

    size_t dummy = 0;

    do {

        char *line = NULL;
        ssize_t lineLength = 0;

        lineLength =
                getline(
                        &line,
                        &dummy,
                        stdin);
        lineNo++;
        
        map->inputLine = line;

        if(lineLength == -1){
            map->inputLine = NULL;
            free(line);
            break;
        }

        if(line[0] != '\n' && !segmentLine(line, lineLength)){
            handleError(lineNo);
            continue;
        }

        Command command = getCommand(line);

        executeCommand(command, line, lineLength-1, map, lineNo);

        char* tmp = line;
        map->inputLine = line = NULL;
        free(tmp);

    } while (true);
}



