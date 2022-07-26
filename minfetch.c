#define _GNU_SOURCE

#ifndef __linux__
#   error "Cannot compile outside of Linux"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#ifdef __linux__
#   include <pwd.h>
#   include <sys/sysinfo.h>
#   include <sys/types.h>
#   include <sys/utsname.h>
#endif

#include "logos.h"

#define ERR_CRASH(A) do {\
    perror(A); exit(EXIT_FAILURE);\
    } while(0);
#define ERR_NOTICE(A) do {\
    perror(A); } while(0);

/* Enum & Struct declarations */

struct thr_usernameInput {
    struct passwd* username;
    uid_t uid;
};

enum distro {
    GENERIC, ARCH_LINUX, GENTOO_LINUX
};

typedef struct {
    char* logo[12];
    char color[32];
    char name[32];
#ifdef __linux__
    enum distro distro;
#endif
} os_t;

typedef struct {
    long seconds;
    long minutes;
    long hours;
    int days;
} uptime_t;

typedef struct {
    struct sysinfo* system;
    struct thr_usernameInput* username;
    struct utsname* kernel;
    char hostname[128];
    char uptime[BUFSIZ];
    char terminal[128];
    os_t* os;
} pointers_t;

/* Function declarations */

pointers_t createPointers(void) {
    pointers_t ret;
    ret.system = malloc(sizeof(struct sysinfo));
    ret.username = malloc(sizeof(struct thr_usernameInput));
    ret.kernel = malloc(sizeof(struct utsname));
    ret.os = malloc(sizeof(os_t));
    return ret;
}

void freePointers(pointers_t ptrs) {
    free(ptrs.system);
    free(ptrs.username);
    free(ptrs.os);
    free(ptrs.kernel);
}

void* getSysInfo(void* arg) {
    sysinfo(arg);
    return arg;
}

void* getOs(void* arg) {
    FILE* fp;
    os_t* buffer = arg;
    if((fp = fopen("/etc/os-release", "r")) == NULL)
        ERR_NOTICE("Failed opening /etc/os-release");
    char* buf = malloc(64 * sizeof(char));
    size_t n = 64;
    while(buf[0] != 'P')
        getline(&buf, &n, fp);
    sscanf(buf, "PRETTY_NAME=\"%[^\"]", buffer->name);
    free(buf); fclose(fp);
    return arg;
}

#ifdef __linux__
void getDistro(os_t* input, char* arg) {
    if(!strcmp(arg, "Arch Linux"))
        input->distro = ARCH_LINUX;
    else if(!strcmp(arg, "Gentoo Linux"))
        input->distro = GENTOO_LINUX;
    else
        input->distro = GENERIC;
}
#endif /* __linux__ */

uptime_t makeUptime(long uptime) {
    uptime_t ret;
    ret.seconds = uptime;
    ret.minutes = (uptime / 60) % 60;
    ret.hours = uptime / 3600;
    ret.days = ret.hours / 24;
    return ret;
}

void getUptime(char* string, struct sysinfo* arg) {
    uptime_t uptime = makeUptime(arg->uptime);
    if (arg->uptime < 60)
        snprintf(string, BUFSIZ, "%lds", arg->uptime);
    else if (arg->uptime < 3600)
        snprintf(string, BUFSIZ, "%ldm", uptime.minutes);
    else if (uptime.days == 0)
        snprintf(string, BUFSIZ, "%ldh %ldm",
                 uptime.hours, uptime.minutes);
    else
        snprintf(string, BUFSIZ, "%dd %ldh %ldm",
                 uptime.days, uptime.hours, uptime.minutes);
}

void* getUsername(void* arg) {
    struct thr_usernameInput* buf = arg;
    buf->username = getpwuid(buf->uid);
    return arg;
}

void* getKernel(void* arg) {
    struct utsname* buf = arg;
    uname(buf);
    return arg;
}

void* getTerminal(void* arg) {
    char* buf = malloc(128 * sizeof(char));
    void* tmp = buf; /* temporary pointer to free later */
    if((buf = getenv("TERMINAL")) == NULL)
        buf = getenv("TERM");
    arg = strcpy(arg, buf);
    free(tmp);
    return arg;
}

void allocateLogo(os_t* os) {
    if(os->distro == ARCH_LINUX) {
        os->logo[0] = ARCH_L0;
        os->logo[1] = ARCH_L1;
        os->logo[2] = ARCH_L2;
        os->logo[3] = ARCH_L3;
        os->logo[4] = ARCH_L4;
        os->logo[5] = ARCH_L5;
        os->logo[6] = ARCH_L6;
        os->logo[7] = ARCH_L7;
        os->logo[8] = ARCH_L8;
        os->logo[9] = ARCH_L9;
        strcpy(os->color, "\x1b[1;96m");
    } else if(os->distro == GENTOO_LINUX) {
        os->logo[0] = GENTOO_L0;
        os->logo[1] = GENTOO_L1;
        os->logo[2] = GENTOO_L2;
        os->logo[3] = GENTOO_L3;
        os->logo[4] = GENTOO_L4;
        os->logo[5] = GENTOO_L5;
        os->logo[6] = GENTOO_L6;
        os->logo[7] = GENTOO_L7;
        os->logo[8] = GENTOO_L8;
        os->logo[9] = GENTOO_L9;
        strcpy(os->color, "\x1b[1;35m");
    } else {
        os->logo[0] = GENERIC_L0;
        os->logo[1] = GENERIC_L1;
        os->logo[2] = GENERIC_L2;
        os->logo[3] = GENERIC_L3;
        os->logo[4] = GENERIC_L4;
        os->logo[5] = GENERIC_L5;
        os->logo[6] = GENERIC_L6;
        os->logo[7] = GENERIC_L7;
        os->logo[8] = GENERIC_L8;
        os->logo[9] = GENERIC_L9;
        strcpy(os->color, "\x1b[1;32m");
    }
}

int main(void) {
    pthread_t threads[5] = {0};
    pointers_t pointers = createPointers();

    pointers.username->uid = getuid();
    if(pthread_create(&threads[0], NULL, &getSysInfo, pointers.system)) {
        ERR_CRASH("Failed creating thread[0]");
        freePointers(pointers);
    } if(pthread_create(&threads[1], NULL, &getOs, pointers.os)) {
        ERR_CRASH("Failed creating thread[1]");
        freePointers(pointers);
    } if(pthread_create(&threads[2], NULL, &getUsername, pointers.username)) {
        ERR_CRASH("Failed creating thread[2]");
        freePointers(pointers);
    } if(pthread_create(&threads[3], NULL, &getKernel, pointers.kernel)) {
        ERR_CRASH("Failed creating thread[3]");
        freePointers(pointers);
    } if(pthread_create(&threads[4], NULL, &getTerminal, pointers.terminal)) {
        ERR_CRASH("Failed creating thread[4]");
        freePointers(pointers);
    }
    pthread_join(threads[0], NULL); getUptime(pointers.uptime, pointers.system);
    pthread_join(threads[1], NULL); getDistro(pointers.os, pointers.os->name);
    pthread_join(threads[2], NULL); allocateLogo(pointers.os);
    pthread_join(threads[3], NULL);
    pthread_join(threads[4], NULL);

    char* colors = "\x1b[1;31m#####\x1b[1;32m#####\x1b[1;33m#####\x1b[1;34m#####\x1b[1;35m#####\
\x1b[1;36m#####\x1b[0m";
    printf("%s%s     %s%s@%s%s\n",
            pointers.os->logo[0], pointers.os->color, pointers.username->username->pw_name,
            NOCOLOR, pointers.os->color, pointers.kernel->nodename);
    printf("%s\n", pointers.os->logo[1]);
    printf("%s%sDistribution%s:   %s\n", pointers.os->logo[2], pointers.os->color, NOCOLOR,
            pointers.os->name);
    printf("%s%sOperat. System%s: %s %s\n", pointers.os->logo[3], pointers.os->color, NOCOLOR,
            pointers.kernel->sysname, pointers.kernel->release);
    printf("%s%sSystem Memory%s:  %luMiB\n", pointers.os->logo[4], pointers.os->color, NOCOLOR,
            pointers.system->totalram / 1024 / 1024);
    printf("%s%sCurrent Uptime%s: %s\n", pointers.os->logo[5], pointers.os->color, NOCOLOR,
            pointers.uptime);
    printf("%s%sShell%s:          %s\n", pointers.os->logo[6], pointers.os->color, NOCOLOR,
            pointers.username->username->pw_shell);
    printf("%s%sTerminal%s:       %s\n", pointers.os->logo[7], pointers.os->color, NOCOLOR,
            pointers.terminal);
    printf("%s%s\n", pointers.os->logo[8], colors);
    printf("%s%s\n", pointers.os->logo[9], colors);

    freePointers(pointers);
    return EXIT_SUCCESS;
}
