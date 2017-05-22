#ifndef CAPITALIZE_CAPITALIZE_H
#define CAPITALIZE_CAPITALIZE_H

static void capitalize(char *str) {
    for (unsigned int p = 0; str[p] != '\0'; p++) {
        if (str[p] >= 'a' && str[p] <= 'z') {
            str[p] -= 'a' - 'A';
        }
    }
}

#endif //CAPITALIZE_CAPITALIZE_H
