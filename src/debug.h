#ifndef DEBUG_H
#define DEBUG_H

#define ENABLE_DEBUG 1 // 1 to enable, 0 to disable

#if ENABLE_DEBUG
    #define DEBUG(args) debug->println(args)
#else
    #define DEBUG(...)
#endif

#endif
