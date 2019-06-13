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
    NEW_ROUTE,
    EXTEND_ROUTE,
    REMOVE_ROAD,
    REMOVE_ROUTE,
    IGNORE, ///< komentarz, lub pusta linijka
    ERROR ///< nierozpoznana komenda
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
bool readUnsigned(char **line, ssize_t *lineLength, unsigned *number){ ///< id 1 do 999, dlugosc 1 do unsigned max

    while (('0' <= **line && **line <= '9') && *lineLength > 0) {
        if (*number <= (UINT_MAX - (**line - '0')) / 10) { ///< If argument does not exceed limit value of unsigned
            (*number) = (*number) * 10 + (**line - '0');
            (*line)++;
            (*lineLength)--;
        } else {
            return false;
        }
    }
    if(**line == 0) {
        (*line)++;
        if(*lineLength != 0)  *lineLength -= 1;
        return true;
    } else {
        return false;
    }


}
/**@brief Zczytuje liczbę całkowitą z podanego napisu.
 * @param[in, out] line - wskaźnik na wskaźnik na przetwarzany napis
 * @param[in, out] lineLength - wskaźnik na pozostała do przetworzenia długośc wiersza
 * @param[in, out] number - wskaźnik na liczbę całkowitą
 * @return Wartość @p true jeśli operacja powiodła się i wartość pod adresem wskazywanym przez year ma dopuszczalną wartość,
 *          @p false w przeciwnym przypadku.
 */
bool readInt(char **line, ssize_t *lineLength, int *number){

    if(**line == '-') {
        (*line)++;
        (*lineLength)--;
        while (('0' <= **line && **line <= '9') && *lineLength > 0) {
            if (*number >= (INT_MIN + (**line - '0'))/10) { ///< If argument is greater than minimal integer.
                (*number) = (*number) * 10 - (**line - '0');
                (*line)++;
                (*lineLength)--;
            } else {
                return false; ///< Exceeds limit or character other than digit
            }
        }
    } else {
        while (('0' <= **line && **line <= '9') && *lineLength > 0) {
            if (*number <= (INT_MAX - (**line - '0')) / 10) { ///< If argument is lesser thank maximal integer.
                (*number) = (*number) * 10 + (**line - '0');
                (*line)++;
                (*lineLength)--;
            } else {
                return false; ///< Exceeds limit or character other than digit
            }
        }
    }

    if(**line == 0) {
        (*line)++;
        if(*lineLength != 0)  *lineLength -= 1;
        return true;
    } else {
        return false;
    }
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


    unsigned id = 0;
    if(!readUnsigned(&line, &lineLength, &id) || !correctId(id) || (lineLength == 0)){
        handleError(lineNo);
        return;
    }

    char* firstCity = NULL;
    readCity(&line, &lineLength, &firstCity);
    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    RouteParam* routeParam = newRouteParam(id);
    if(routeParam == NULL){
        return;
    }
    if(!addCityToRouteParam(routeParam, firstCity)){ ///< memory error
        deleteRouteParam(routeParam);
        exit(0);
    }
    while(lineLength > 0){

        unsigned roadLength = 0;
        if(!readUnsigned(&line, &lineLength, &roadLength) || roadLength <= 0 || lineLength == 0){

            handleError(lineNo);
            deleteRouteParam(routeParam);
            return;
        }

        int year = 0;
        if(!readInt(&line, &lineLength, &year) || year == 0 || lineLength == 0){
            handleError(lineNo);
            deleteRouteParam(routeParam);
            return;
        }

        if (!addRoadToRouteParam(routeParam, roadLength, year)){
            deleteRouteParam(routeParam);
            exit(0);
        }

        char* city = NULL;
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

    if(!newRouteFromRouteParam(map, routeParam)|| lineLength > 0){
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
        return;
    }

    char* cityA = NULL;
    readCity(&line, &lineLength, &cityA);
    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    char* cityB = NULL;
    readCity(&line, &lineLength, &cityB);
    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    unsigned roadLength = 0;
    if(!readUnsigned(&line, &lineLength, &roadLength) || roadLength <= 0 ||  lineLength == 0){
        handleError(lineNo);
        return;
    }

    int year = 0;
    if(!readInt(&line,&lineLength,&year) || year == 0 || lineLength > 0){
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

    char* cityA = NULL;
    readCity(&line, &lineLength, &cityA);
    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    char* cityB = NULL;
    readCity(&line, &lineLength, &cityB);
    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    int repairYear = 0;
    if(!readInt(&line, &lineLength, &repairYear) || repairYear == 0 || lineLength > 0){
        handleError(lineNo);
        return;
    }

    if(!repairRoad(map, (const char*) cityA, (const char*) cityB, repairYear)){
        handleError(lineNo);
        return;
    }
}
/**@brief Z napisu zczytuje parametry opisujące nową drogę krajową i wywołuję funkcję newRoute.
 * @param[in, out] line - wskaźnik na przetwarzany napis
 * @param[in, out] lineLength - pozostała do przetworzenia długośc wiersza
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] lineNo - numer przetwarzanego wiersza
 */
void getParametersAndNewRoute(char* line, ssize_t lineLength, Map* map, int lineNo) {

    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    unsigned id = 0;
    if(!readUnsigned(&line, &lineLength, &id) || !correctId(id) || lineLength == 0){
        handleError(lineNo);
        return;
    }

    char* cityA = NULL;
    readCity(&line, &lineLength, &cityA);
    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    char* cityB = NULL;
    readCity(&line, &lineLength, &cityB);
    if(lineLength > 0){
        handleError(lineNo);
        return;
    }
    if(!newRoute(map, id, cityA, cityB)){
        handleError(lineNo);
        return;
    }
}
/**@brief Z napisu zczytuje id drogi krajowej i miasto o które ma ona zostać przedłużona. Wywołuje funkcję extendRoute.
 * @param[in, out] line - wskaźnik na przetwarzany napis
 * @param[in, out] lineLength - pozostała do przetworzenia długośc wiersza
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] lineNo - numer przetwarzanego wiersza
 */
void getParametersAndExtendRoute(char* line, ssize_t lineLength, Map* map, int lineNo) {

    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    unsigned id = 0;
    if(!readUnsigned(&line, &lineLength, &id) || !correctId(id) || lineLength == 0){
        handleError(lineNo);
        return;
    }

    char* city = NULL;
    readCity(&line, &lineLength, &city);
    if(lineLength > 0){
        handleError(lineNo);
        return;
    }

    if(!extendRoute(map, id, city)){
        handleError(lineNo);
        return;
    }
}
/**@brief Z napisu zczytuje parametry opisujące drogę i wywołuje funkcję removeRoad.
 * @param[in, out] line - wskaźnik na przetwarzany napis
 * @param[in, out] lineLength - pozostała do przetworzenia długośc wiersza
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] lineNo - numer przetwarzanego wiersza
 */
void getParametersAndRemoveRoad(char* line, ssize_t lineLength, Map* map, int lineNo) {

    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    char* cityA = NULL;
    readCity(&line, &lineLength, &cityA);
    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    char* cityB = NULL;
    readCity(&line, &lineLength, &cityB);
    if(lineLength > 0){
        handleError(lineNo);
        return;
    }
    if(!removeRoad(map, cityA, cityB)){
        handleError(lineNo);
        return;
    }
}
/**@brief Z napisu zczytuje id drogi krajowej i wywołuje funkcję removeRoute.
 * @param[in, out] line - wskaźnik na przetwarzany napis
 * @param[in, out] lineLength - pozostała do przetworzenia długośc wiersza
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] lineNo - numer przetwarzanego wiersza
 */
void getParametersAndRemoveRoute(char* line, ssize_t lineLength, Map* map, int lineNo) {

    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    unsigned id = 0;
    if(!readUnsigned(&line, &lineLength, &id) || !correctId(id) || lineLength > 0){
        handleError(lineNo);
        return;
    }
    if(!removeRoute(map, id)){
        handleError(lineNo);
        return;
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

    unsigned id = 0;
    if(!readUnsigned(&line, &lineLength, &id) || lineLength > 0){
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

        case NEW_ROUTE:
            getParametersAndNewRoute(&line[strlen(line) + 1], lineLength-(strlen(line) + 1), map, lineNo);
            break;

        case EXTEND_ROUTE:
            getParametersAndExtendRoute(&line[strlen(line) + 1], lineLength-(strlen(line) + 1), map, lineNo);
            break;

        case REMOVE_ROAD:
            getParametersAndRemoveRoad(&line[strlen(line) + 1], lineLength-(strlen(line) + 1), map, lineNo);
            break;

        case REMOVE_ROUTE:
            getParametersAndRemoveRoute(&line[strlen(line) + 1], lineLength-(strlen(line) + 1), map, lineNo);
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
    if(strcmp(line, "newRoute") == 0) return NEW_ROUTE;
    if(strcmp(line, "extendRoute") == 0) return EXTEND_ROUTE;
    if(strcmp(line, "removeRoad") == 0) return REMOVE_ROAD;
    if(strcmp(line, "removeRoute") == 0) return REMOVE_ROUTE;

    if('0' <= *line && *line <= '9'){
        return ADD_ROUTE;
    }

    return ERROR;

}
/**@brief W napisie podmienia ';' i '\n' na '\0'.
* @param[in] line - wskaźnik na przetwarzany napis
 * @param[in] lineLength - wskaźnik na pozostała do przetworzenia długośc wiersza
 * */
bool segmentLine(char *line, ssize_t lineLength){

    if(line[lineLength - 2] == ';'){
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

        lineLength = getline(&line, &dummy, stdin);
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



