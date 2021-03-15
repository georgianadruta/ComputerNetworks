#include <stdio.h>
#include "list.h"

int main() {
    char** fileList;
    fileList = list();
    for(int i = 0; i < 5; i++) {
        printf("%s\n", fileList[i]);
    }
}