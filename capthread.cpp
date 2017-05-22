#include <iostream>
#include "capitalize.h"

void *capthread(void *args) {
    capitalize((char *) args);
    pthread_exit(NULL);
}