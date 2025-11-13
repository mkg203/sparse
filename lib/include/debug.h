#define print_debug(x) \
    fprintf(stderr, "%s:%d: " _Generic((x),                    \
        int: "%d\n",                                           \
        long: "%ld\n",                                         \
        float: "%f\n",                                         \
        double: "%f\n",                                        \
        char*: "%s\n",                                         \
        const char*: "%s\n",                                   \
        default: "%p\n"                                        \
    ), __FILE__, __LINE__, (x))
