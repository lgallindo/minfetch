#ifndef UPTIME__HEADER
#define UPTIME__HEADER

struct uptime transformUptime(long uptime);
char* makeUptime(const long uptime);

struct uptime {
    long seconds;
    long minutes;
    long hours;
    int days;
};

#endif
