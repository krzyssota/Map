#ifndef DROGI_ADDITIONALFUNCTIONS_H
#define DROGI_ADDITIONALFUNCTIONS_H

#include <stdbool.h>
#include "map.h"

char* getName(const char *name);
bool correctId(unsigned id);
bool correctName(const char* cityName);
int count(Route* route);
char* getDescription(Route* route, int length);

#endif //DROGI_ADDITIONALFUNCTIONS_H
