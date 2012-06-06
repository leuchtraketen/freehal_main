#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <string.h>

int main(int argc, char** argv) {
    char filename[300];
    sprintf(filename, "/srv/www/vhosts/boinc.freehal.org/httpdocs/projects/freehal_at_home/limits/%s", argv[1]);
    
    FILE* f = fopen(filename, "r");
    int i;
        if (f) fread(&i, sizeof(int), 1, f);
//    if (i  && i > 25) i = 25;
    if (!i || i < 1 ) i = 1;
    printf("%d", i);
}
