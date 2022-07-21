#ifndef USERNAME__HEADER
#define USERNAME__HEADER

void* makeUsername(void* input);

struct username_input {
    struct passwd* ptr;
    uid_t uid;
};

#endif
