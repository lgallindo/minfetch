// This code is licensed under the BSD 2 Clause license

#ifndef __unix__
#   error "ERROR: Cannot compile outside of Linux"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include <pwd.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/utsname.h>

#include "username.h"
#include "uptime.h"
#include "logos.h"
#include "common.h"
#include "os.h"

int main(void)
{
    struct sysinfo* system = (struct sysinfo*)malloc(sizeof(struct sysinfo));
    if (sysinfo(system)) {
        free(system);
        ERR_CRSH("Failed grabbing sysinfo");
    }

    char* hostname = malloc(512 * sizeof(char*));
    char* os =  malloc(512 * sizeof(char*));
    struct username_input username;
    username.ptr = malloc(sizeof(struct passwd*));
    void* tmpptr = username.ptr; // temporary buffer ptr to be freed later
    username.uid = getuid();
    pthread_t thread1;
    pthread_t thread2;
    if(pthread_create(&thread1, NULL, &makeOs, os)) {
        free(tmpptr); free(os); free(hostname); free(system);
        ERR_CRSH("Failed creating thread 1");
    }
    if(pthread_create(&thread2, NULL, &makeUsername, &username)) {
        free(tmpptr); free(os); free(hostname); free(system);
        ERR_CRSH("Failed creating thread 2");
    }

    char* uptime = makeUptime(system->uptime);
    struct utsname* kernel = malloc(sizeof(struct utsname));
    if(uname(kernel)) {
        free(tmpptr); free(os); free(hostname); free(system); free(uptime);
        free(kernel); ERR_CRSH("Failed grabbing uname info");
    }
    pthread_join(thread1, NULL);
    struct logo logo = compareOs(os);
    pthread_join(thread2, NULL);
    char* terminal = getenv("TERM");

    char* colors = "\x1b[1;31m#####\x1b[1;32m#####\x1b[1;33m#####\x1b[1;34m#####\
\x1b[1;35m#####\x1b[1;36m#####\x1b[0m\n";
    printf("%s%s      %s%s@%s%s\n", logo.color, logo.l0, username.ptr->pw_name,
            NOCOLOR, logo.color, kernel->nodename);
    printf("%s\n", logo.l1);
    printf("%sDistribution%s:   %s\n%s", logo.l2, NOCOLOR, os, logo.color);
    printf("%sOperat. System%s: %s %s\n%s", logo.l3, NOCOLOR, kernel->sysname, 
            kernel->release, logo.color);
    printf("%sSystem Memory%s:  %luMiB\n%s", logo.l4, NOCOLOR, 
            system->totalram / 1024 / 1024, logo.color);
    printf("%sCurrent Uptime%s: %s\n%s", logo.l5, NOCOLOR, uptime, logo.color);
    printf("%sShell%s:          %s\n%s", logo.l6, NOCOLOR, username.ptr->pw_shell,
            logo.color);
    printf("%sTerminal%s:       %s\n%s", logo.l7, NOCOLOR, terminal, logo.color);
    printf("%s%s%s", logo.l8, colors, logo.color);
    printf("%s%s\n\x1b[0m", logo.l9, colors);
    free(hostname); free(system); free(uptime); free(os); free(tmpptr);
    free(kernel);
    return EXIT_SUCCESS;
}
