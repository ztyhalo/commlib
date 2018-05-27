#include "zprint.h"
#include <assert.h>
#include <stdarg.h>
#include <time.h>


void PRINTF_CLASS::zprintf(const char * format, ...)
{
    va_list args;

    if(pfd != NULL)
    {
        lock();
        va_start(args, format);
        vfprintf(pfd, format, args);
        va_end(args);
        fflush(pfd);
        unlock();
    }
}

void PRINTF_CLASS::timeprintf(const char * format, ...)
{
    va_list args;

    if(pfd != NULL)
    {
        struct tm *p;
        struct timeval tv;
        va_start(args, format);

        gettimeofday(&tv, NULL);
        p = localtime(&tv.tv_sec);
        lock();
        fprintf(pfd,"%d-%02d-%02d %02d:%02d:%02d ",
               1900+p->tm_year, 1+p->tm_mon, p->tm_mday,
               p->tm_hour, p->tm_min, p->tm_sec);
        vfprintf(pfd, format, args);
        va_end(args);
        fflush(pfd);
        unlock();
    }
}

void PRINTF_CLASS::timemsprintf(const char * format, ...)
{
    va_list args;

    if(pfd != NULL)
    {
        struct tm *p;
        struct timeval tv;
        va_start(args, format);

        gettimeofday(&tv, NULL);
        p = localtime(&tv.tv_sec);
        lock();
        fprintf(pfd,"%d-%02d-%02d %02d:%02d:%02d.%6ld ",
               1900+p->tm_year, 1+p->tm_mon, p->tm_mday,
               p->tm_hour, p->tm_min, p->tm_sec, tv.tv_usec);
        vfprintf(pfd, format, args);
        va_end(args);
        fflush(pfd);
        unlock();
    }
}

