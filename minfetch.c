#if !(defined(linux) || defined(__linux__))
#   error Not available outside of Linux
#endif

#define COLORS "\x1b[1;31m#####\x1b[1;32m#####\x1b[1;33m#####"\
               "\x1b[1;34m#####\x1b[1;35m#####\x1b[1;36m#####\x1b[0m"
#define DEFAULT_STRING_SIZE 128
#define ABORT(...) do {                             \
                   fprintf(stderr, __VA_ARGS__);    \
                   pointersFree();                  \
                   exit(EXIT_FAILURE); } while(1);
#define NOCOLOR "\x1b[1;0m"
#define HEADER(A) pointers->distro->logo[A], pointers->distro->color, NOCOLOR

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#include <pwd.h>
#include <fcntl.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/utsname.h>

#include "logos.h"

struct uptime {
    long seconds;
    long minutes;
    long hours;
    int days;
};

struct distro {
    char* logo[DEFAULT_STRING_SIZE];
    char color[DEFAULT_STRING_SIZE];
    char name[DEFAULT_STRING_SIZE];
    enum {
        GENERIC, ARCH_LINUX, GENTOO_LINUX, DEBIAN_LINUX,
    } type;
};

struct system {
    struct sysinfo *sysinfo;
    struct passwd  *passwd;
    struct utsname *utsname;
    struct distro  *distro;
    char hostname[DEFAULT_STRING_SIZE];
    char uptime[DEFAULT_STRING_SIZE];
    char terminal[DEFAULT_STRING_SIZE];
    char editor[DEFAULT_STRING_SIZE];
    long availableram;
    uid_t uid;
};

static struct system *pointers = NULL;
static void *passwd_tmp_pointer = NULL;

static void *xcalloc(size_t size, size_t len)
{
    void *ret = calloc(size, len);
    if(ret == NULL) {
        fprintf(stderr, "minfetch: couldn't allocate memory: %s\n",
                strerror(errno));
        exit(EXIT_FAILURE);
    }
    return ret;
}

static inline void pointersAlloc(void)
{
    pointers = xcalloc(1, sizeof(struct system));
    pointers->sysinfo = xcalloc(1, sizeof(struct sysinfo));
    pointers->utsname = xcalloc(1, sizeof(struct utsname));
    pointers->distro = xcalloc(1, sizeof(struct distro));
    pointers->passwd = xcalloc(1, sizeof(struct passwd));
    passwd_tmp_pointer = pointers->passwd;
}

static inline void pointersFree(void)
{
    free(pointers->sysinfo);
    free(pointers->utsname);
    free(pointers->distro);
    free(passwd_tmp_pointer);
    free(pointers);
}

static inline void makeThread(pthread_t *thread, void *(*function)(void*), void *input)
{
    if(pthread_create(thread, NULL, function, input))
        ABORT("minfetch: failed creating thread: %s\n", strerror(errno));
}

static void *getKernel(void *arg)
{
    uname((struct utsname*)arg);
    return arg;
}

static void *getOs(void *arg)
{
    FILE *fp;
    char *buf = xcalloc(64, sizeof(char));

    if((fp = fopen("/etc/os-release", "r")) == NULL)
        ABORT("minfetch: failed opening /etc/os-release: %s\n", strerror(errno));

    while(buf[0] != 'P')
        buf = fgets(buf, 64, fp);
    sscanf(buf, "PRETTY_NAME=\"%[^\"]", ((struct distro*)arg)->name);
    free(buf); fclose(fp);

    return arg;
}

static void *getUsername(void *arg)
{
    pointers->passwd = getpwuid(pointers->uid);
    return arg;
}

static void *getAvailableRam(void *arg)
{
    FILE *fp;
    char *buf = xcalloc(BUFSIZ, sizeof(char));
    char *buffer = xcalloc(BUFSIZ, sizeof(char));

    if((fp = fopen("/proc/meminfo", "r")) == NULL)
        ABORT("minfetch: failed opening /proc/meminfo: %s\n", strerror(errno));

    while(buf[3] != 'A')
        buf = fgets(buf, BUFSIZ, fp);
    sscanf(buf, "MemAvailable: %[^k]", buffer);
    ((struct system*)arg)->availableram = atol(buffer);
    free(buf); free(buffer); fclose(fp);

    return arg;
}

static void *getSysInfo(void *arg)
{
    sysinfo(arg);
    return arg;
}

static void *getTerminal(void *arg)
{
    char *buf = xcalloc(128, sizeof(char));
    void *tmp = buf;

    if((buf = getenv("TERMINAL")) == NULL)
        buf = getenv("TERM");
    arg = strcpy(arg, buf);
    free(tmp);

    return arg;
}

static void *getEditor(void *arg)
{
    char *buf = xcalloc(128, sizeof(char));
    void *tmp = buf;

    if((buf = getenv("VISUAL")) == NULL)
        buf = getenv("EDITOR");
    arg = strcpy(arg, buf);
    free(tmp);

    return arg;
}

struct uptime makeUptime(long uptime)
{
    struct uptime ret;
    ret.seconds = uptime;
    ret.minutes = (uptime / 60) % 60;
    ret.hours = uptime / 3600;
    ret.days = ret.hours / 24;
    return ret;
}

static void getUptime(char *string, struct sysinfo *arg)
{
    struct uptime uptime = makeUptime(arg->uptime);
    if (arg->uptime < 60)
        snprintf(string, DEFAULT_STRING_SIZE, "%lds", arg->uptime);
    else if (arg->uptime < 3600)
        snprintf(string, DEFAULT_STRING_SIZE, "%ldm", uptime.minutes);
    else if (uptime.days == 0)
        snprintf(string, DEFAULT_STRING_SIZE, "%ldh %ldm",
                 uptime.hours, uptime.minutes);
    else
        snprintf(string, DEFAULT_STRING_SIZE, "%dd %ldh %ldm",
                 uptime.days, uptime.hours, uptime.minutes);
}

static void getDistro(struct distro *input, char *arg)
{
    if(!strcmp(arg, "Arch Linux"))
        input->type = ARCH_LINUX;
    else if(!strcmp(arg, "Gentoo Linux"))
        input->type = GENTOO_LINUX;
    else if(!strcmp(arg, "Debian Linux"))
        input->type = DEBIAN_LINUX;
    else
        input->type = GENERIC;
}

static void allocateLogo(struct distro *distro)
{
    switch(distro->type) {
    case ARCH_LINUX: {
        char *LOGO[DEFAULT_STRING_SIZE] = ARCH_LOGO;
        memcpy(distro->logo, LOGO, sizeof(LOGO));
        strcpy(distro->color, "\x1b[1;96m");
    } break;

    case GENTOO_LINUX: {
        char *LOGO[DEFAULT_STRING_SIZE] = GENTOO_LOGO;
        memcpy(distro->logo, LOGO, sizeof(LOGO));
        strcpy(distro->color, "\x1b[1;35m");
    } break;

    case DEBIAN_LINUX: {
        char *LOGO[DEFAULT_STRING_SIZE] = DEBIAN_LOGO;
        memcpy(distro->logo, LOGO, sizeof(LOGO));
        strcpy(distro->color, "\x1b[1;31m");
    } break;

    default:
        char *LOGO[DEFAULT_STRING_SIZE] = GENERIC_LOGO;
        memcpy(distro->logo, LOGO, sizeof(LOGO));
        strcpy(distro->color, "\x1b[1;32m");
    }
}

static inline void pointersAssign(void)
{
    pointers->uid = getuid();
    pthread_t threads[5] = {0};
    makeThread(&threads[0], &getKernel, pointers->utsname);
    makeThread(&threads[1], &getOs, pointers->distro);
    makeThread(&threads[2], &getUsername, pointers->passwd);
    makeThread(&threads[3], &getAvailableRam, pointers);
    makeThread(&threads[4], &getTerminal, pointers->terminal);
    pthread_join(threads[2], NULL);
    makeThread(&threads[2], &getSysInfo, pointers->sysinfo);
    pthread_join(threads[3], NULL);
    makeThread(&threads[3], &getEditor, pointers->editor);
    pthread_join(threads[4], NULL);
    getUptime(pointers->uptime, pointers->sysinfo);
    pthread_join(threads[1], NULL);
    getDistro(pointers->distro, pointers->distro->name);
    allocateLogo(pointers->distro);
    pthread_join(threads[0], NULL);
    pthread_join(threads[2], NULL);
    pthread_join(threads[3], NULL);
}

int main(void)
{
    pointersAlloc();
    pointersAssign();

    printf("%s%s        %s%s@%s%s\n", pointers->distro->logo[0], pointers->distro->color,
            pointers->passwd->pw_name, NOCOLOR, pointers->distro->color, pointers->utsname->nodename);
    printf("%s\n", pointers->distro->logo[1]);
    printf("%s%sDistribution%s:   %s\n", HEADER(2), pointers->distro->name);
    printf("%s%sOperat. System%s: %s %s\n", HEADER(3), pointers->utsname->sysname, pointers->utsname->release);
    printf("%s%sSystem Memory%s:  %ldMiB / %luMiB\n", HEADER(4), ((pointers->sysinfo->totalram / 1024) -
           (pointers->availableram + (pointers->sysinfo->bufferram / 1024))) / 1024, pointers->sysinfo->
           totalram / 1024 / 1024);
    printf("%s%sCurrent Uptime%s: %s\n", HEADER(5), pointers->uptime);
    printf("%s%sShell%s:          %s\n", HEADER(6), pointers->passwd->pw_shell);
    printf("%s%sTerminal%s:       %s\n", HEADER(7), pointers->terminal);
    printf("%s%sEditor%s:         %s\n", HEADER(8), pointers->editor);
    printf("%s%s\n", pointers->distro->logo[9], COLORS);

    pointersFree();
    return EXIT_SUCCESS;
}
