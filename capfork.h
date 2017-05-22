#ifndef CAPITALIZE_CAPFORK_H
#define CAPITALIZE_CAPFORK_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>

void capfork(char *args, size_t size);
std::vector<char> capfork2(char *args, size_t size);

#endif //CAPITALIZE_CAPFORK_H
