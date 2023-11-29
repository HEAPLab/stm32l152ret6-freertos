#include "main.h"

static int COUNT=0;

void Incorrect_Result() {
    Error_Handler();
}

int done() {
    COUNT++;
    return COUNT;
}