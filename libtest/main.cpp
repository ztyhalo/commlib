#include <QCoreApplication>
#include "timers.h"
#include <stdio.h>
#include "zprint.h"
#include "e_poll.h"


class VAL_CLASS
{
public:
    int val;

    Z_PTH<VAL_CLASS> pth;
public:
    VAL_CLASS()
    {
        val = 9;
    }
    void vvfunc(void)
    {
        while(1)
        {
            printf("val is %d\n",val);
            val++;
            sleep(1);
        }
    }

};
typedef struct
{
    char buf[8];
}T_TimeVal;


class FILE_OUT
{
public:
    void print_time_context(void)
    {
        zprintf1("test \n");
    }
};


PRINTF_CLASS test_p(NULL, 0);

FILE_OUT ot;

VAL_CLASS ptest;

FILE_POLL<T_TimeVal> timefile;
Z_TIMER ntimer;




int main(int argc, char *argv[])
{

    zprintf4("%d file\n", evt.filed);

    ntimer.timer_start(2, 1);
    ntimer.f_bind(&ot, &FILE_OUT::print_time_context);

    while(1)
    {
        sleep(1);
    }
}
