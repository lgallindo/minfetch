#ifndef LOGO__HEADER
#define LOGO__HEADER

struct logo {
    char* l0;
    char* l1;
    char* l2;
    char* l3;
    char* l4;
    char* l5;
    char* l6;
    char* l7;
    char* l8;
    char* l9;
    char* color;
};

#define NOCOLOR "\x1b[0m"

#define ARCH_L0 "          #           "
#define ARCH_L1 "         /#\\         "
#define ARCH_L2 "        .###\\         "
#define ARCH_L3 "       ;#####.        "
#define ARCH_L4 "      /#######;       "
#define ARCH_L5 "     .##/   \\##\\      "
#define ARCH_L6 "    /###     ###;     "
#define ARCH_L7 "   .####\\   /####.    "
#define ARCH_L8 "  /##.         .##\\   "
#define ARCH_L9 " .#/             \\#.  "

#define GNTO_L0 "   ###########         "
#define GNTO_L1 "  #@@@@@@@@@@@@#       "
#define GNTO_L2 " #@@@@@@@@@###@@@#     "
#define GNTO_L3 "  #@@@@@@@#   #@@@#    "
#define GNTO_L4 "   ####@@@@###@@@@@@#  "
#define GNTO_L5 "       #@@@@@@@@@@@@@# "
#define GNTO_L6 "      #@@@@@@@@@@@@@@#  "
#define GNTO_L7 "    ##@@@@@@@@@@@@@#   "
#define GNTO_L8 "   #@@@@@@@@@@@@@#     "
#define GNTO_L9 "    ############       "

#define GNRC_L0 "       /#######\\      "
#define GNRC_L1 "      |#o#####o#|     "
#define GNRC_L2 "       \\#PPPPP#/      "
#define GNRC_L3 "      /##88P88##\\     "
#define GNRC_L4 "     /##8888888##\\    "
#define GNRC_L5 "    /##888888888##\\   "
#define GNRC_L6 "   |__|888888888__|   "
#define GNRC_L7 "   /  \\88888888/  /   "
#define GNRC_L8 "   \\   \\888888/  /    "
#define GNRC_L9 "    \\___######__/     "

#endif
