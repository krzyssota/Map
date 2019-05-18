#include "map.h"

#include <stdlib.h>
#include <string.h>

#undef NDEBUG

#include <assert.h>
#include <stdio.h>
#include "input.h"

Map* map; ///< Zmienna globalna używana do zwalniania pamięci przy użyciu funkcji atexit()

void freeMemory(void){
    deleteMap(map);
}

int main() {

    map = newMap();
    assert(map);

    readExecuteInput(map);

    atexit(freeMemory);

    return 0;
}
