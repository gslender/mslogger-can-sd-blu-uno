#ifndef DEBUG_H
#define DEBUG_H

#define ENABLE_DEBUG 0 // 1 to enable, 0 to disable

#if ENABLE_DEBUG
    #define LOG(args...) APP_LOG(APP_LOG_LEVEL_DEBUG, args)
#else
    #define LOG(...)
#endif

#endif
