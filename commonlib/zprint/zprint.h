#ifndef __ZPRINT_H__
#define __ZPRINT_H__
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <string>
#include <iostream>
#include <sys/time.h>
#include "mutex.h"
using namespace std;

#define timeprf(...) do { time_t now;\
                        struct tm timenow;\
                        char stbuf[32];\
    time(&now);\
    localtime_r(&now, &timenow);\
    asctime_r(&timenow,stbuf);\
    stbuf[strlen(stbuf)-1] = '\0';\
    printf("%s ", stbuf);\
    printf( __VA_ARGS__);}while(0)

#define timemsprf(...) do {\
    struct tm *p;\
    struct timeval tv;\
    gettimeofday(&tv, NULL);\
    p = localtime(&tv.tv_sec);\
    printf("%d-%02d-%02d %d:%d:%d.%3ld\n",\
           1900+p->tm_year, 1+p->tm_mon, p->tm_mday,\
           p->tm_hour, p->tm_min, p->tm_sec, tv.tv_usec);\
    printf( __VA_ARGS__);}while(0)

class PRINTF_CLASS:public MUTEX_CLASS
{
private:
    FILE * pfd;
    int    std_fd;

public:
    PRINTF_CLASS(const char * name = NULL, int fd = 1)
    {
        if(fd == 1)          //标准输出
        {
            std_fd = fd;
            pfd = stdout;
        }
        else
        {
            pfd = NULL;
            if(name != NULL)
            {
                pfd = fopen(name, "a+");
                if(pfd == NULL)
                {
                    printf("file %s open fail\n", name);
                }
            }
        }
    }
    ~PRINTF_CLASS()
    {
        if(std_fd != 1 && pfd != NULL)
        {
            fclose(pfd);
        }
    }
    void zprintf(const char * format, ...);
    void timeprintf(const char * format, ...);
    void timemsprintf(const char * format, ...);


};

class PRINTF_INSTANCE:public PRINTF_CLASS
{
private:

     PRINTF_INSTANCE(char * name = NULL, int fd = 1):PRINTF_CLASS(name,fd)
     {
         ;
     }

public:
    static PRINTF_INSTANCE * get_printf_instance(void)
    {
        static PRINTF_INSTANCE gPrintf;
        return &gPrintf;
    }
    ~PRINTF_INSTANCE()
    {
        ;
    }

};

#define PRINT_PRO      1

#if PRINT_PRO >=1
#define zprintf1 printf
#else
#define zprintf1(...)
#endif

#if PRINT_PRO >=2
#define zprintf2 printf
#else
#define zprintf2(...)
#endif

#if PRINT_PRO >=3
#define zprintf3 printf
#define prop_printf     printf
#else
#define zprintf3(...)
#endif

#if PRINT_PRO >=4
#define zprintf4 printf
#else
#define zprintf4(...)
#endif

#ifndef prop_printf
#define prop_printf(...)
#endif



#endif //__ZPRINT_H__
