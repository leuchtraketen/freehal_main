#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <string.h>

int main(int argc, char** argv) {
    char filename[300];
    sprintf(filename, "/home/boincadm/www/projects/freehal_at_home/limits/%s", argv[1]);
    
    FILE* f = fopen(filename, "w");
    int i = atoi(argv[2]);
    if (i > 25) i = 25;
        if (f) fwrite(&i, sizeof(int), 1, f);
}
