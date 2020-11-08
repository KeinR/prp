#ifndef PLATFORM_H_INCLUDED
#define PLATFORM_H_INCLUDED

#if (defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__)
#   ifdef IS_OS_WINDOWS
#       error "IS_OS_WINDOWS is defined somewhere else, should fix"
#   endif
#   define IS_OS_WINDOWS
#endif

#endif
