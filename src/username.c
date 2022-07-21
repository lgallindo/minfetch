#include <sys/types.h>
#include <pwd.h>

#include "username.h"

void*
makeUsername(void* input)
{
    struct username_input* buf = input;
    buf->ptr = (struct passwd*)getpwuid(buf->uid);
    return input;
}
