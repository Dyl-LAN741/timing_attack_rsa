#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "logs.h"

FILE *logs = NULL;

void init_logs(void)
{
    if(mkdir("./output", 0777) == -1 && errno != EEXIST) {
        perror("mkdir failed : ");
        exit(20);
    }

    logs = fopen("./output/logs.txt", "w");
    if(!logs) {
        perror("fopen logs failed : ");
        exit(21);
    }
}
