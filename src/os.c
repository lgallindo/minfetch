#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logos.h"
#include "common.h"

_Bool
isEqual(const char* string1, const char* string2)
{
    size_t len = MAX((strlen(string1)), strlen(string2));
    for(size_t n = 0; n < len; n++) {
        if(string1[n] != string2[n])
            return 1;
    }
    return 0;
}

void*
makeOs(void* input)
{
    FILE* fp;
    if((fp = fopen("/etc/os-release", "r")) == NULL)
        ERR_NTCE("Failed opening /etc/os-release");
    fscanf(fp, "NAME=\"%[^\"]", (char*)input);
    fclose(fp);
    return input;
}

struct logo
compareOs(const char* os)
{
    struct logo ret = {0};
    if(!strcmp(os, "Arch Linux")) {
        ret.l0 = ARCH_L0;
        ret.l1 = ARCH_L1;
        ret.l2 = ARCH_L2;
        ret.l3 = ARCH_L3;
        ret.l4 = ARCH_L4;
        ret.l5 = ARCH_L5;
        ret.l6 = ARCH_L6;
        ret.l7 = ARCH_L7;
        ret.l8 = ARCH_L8;
        ret.l9 = ARCH_L9;
        ret.color = "\x1b[1;34m";
        return ret;
    }
    else {
        ret.l0 = GNRC_L0;
        ret.l1 = GNRC_L1;
        ret.l2 = GNRC_L2;
        ret.l3 = GNRC_L3;
        ret.l4 = GNRC_L4;
        ret.l5 = GNRC_L5;
        ret.l6 = GNRC_L6;
        ret.l7 = GNRC_L7;
        ret.l8 = GNRC_L8;
        ret.l9 = GNRC_L9;
        ret.color = "\x1b[1;32m";
        return ret;
    }
}
