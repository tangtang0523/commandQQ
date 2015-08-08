#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "common.h"

void get_cur_time(char *time_str)
{
    time_t timep;
    struct tm *p_curtime = NULL;
    char *time_tmp = NULL;
    time_tmp = (char *)malloc(2);
    memset(time_tmp, 0, 2);

    memset(time_str, 0, 20);
    time(&timep);
    p_curtime = localtime(&timep);
    strcat(time_str, "(");
    itoa(p_curtime->tm_hour,time_tmp);
    strcat(time_str,time_tmp);
    strcat(time_str,":");
    itoa(p_curtime->tm_min,time_tmp);
    strcat(time_str,time_tmp);
    strcat(time_str,":");
    itoa(p_curtime->tm_sec,time_tmp);
    strcat(time_str,time_tmp);
    strcat(time_str,")");
    free(time_tmp);
}

void disp_time(char *time_str)
{
    get_cur_time(time_str);
    printf("%s\n", time_str);
}
