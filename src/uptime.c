#include <stdio.h>
#include <stdlib.h>

#include "uptime.h"

struct uptime
transformUptime(long uptime)
{
    struct uptime ret;
    ret.seconds = uptime;
    ret.minutes = (uptime / 60) % 60;
    ret.hours = uptime / 3600;
    ret.days = ret.hours / 24;
    return ret;
}

char*
makeUptime(const long uptime)
{
    char* ret = malloc(sizeof(char*));
    struct uptime info = transformUptime(uptime);
    if (uptime < 60)
        snprintf(ret, BUFSIZ, "%lds", uptime);
    else if (uptime < 3600)
        snprintf(ret, BUFSIZ, "%ldm", info.minutes);
    else if (info.days == 0)
        snprintf(ret, BUFSIZ, "%ldh %ldm",
                 info.hours, info.minutes);
    else
        snprintf(ret, BUFSIZ, "%dd %ldh %ldm",
                 info.days, info.hours, info.minutes);
    return ret;
}

