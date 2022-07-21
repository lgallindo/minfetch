#ifndef COMMON__HEADER
#define COMMON__HEADER

#define ERR_CRSH(A) do {\
    perror(A); exit(EXIT_FAILURE);\
    } while(0);
#define ERR_NTCE(A) do {\
    perror(A); } while(0);
#define MAX(A, B) (A > B ? A : B)

#endif
