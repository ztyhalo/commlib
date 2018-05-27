#include <QCoreApplication>
#include "timers.h"
#include <stdio.h>
#include "zprint.h"
#include "e_poll.h"

PRINTF_CLASS gpr(NULL, 1);
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
        gpr.timemsprintf("test \n");
    }
};

F_Timer<void,void> ti;
TEvent  evt;
PRINTF_CLASS test_p(NULL, 0);

FILE_OUT ot;

VAL_CLASS ptest;

FILE_POLL<T_TimeVal> timefile;
Z_TIMER ntimer;

int out_printf(TEvent * val)
{
    test_p.timemsprintf("time ok\n");
}
int out_outprintf(TEvent * val)
{
    test_p.timeprintf("time2 ok\n");
}


int main(int argc, char *argv[])
{
//    timefile.pth.start();
//    timefile.e_poll_add(evt.filed);

    zprintf4("%d file\n", evt.filed);
//    P_Connect(&timefile,fileval, &ot, &FILE_OUT::print_time_context);
//    timefile.fileval.Bind(&ot, &FILE_OUT::print_time_context);
//    evt.timer_start(2);
//    ti.start();
//    ti.add_event(1,out_printf);
//    ti.add_event(1,out_outprintf);
//    ptest.pth.pthread_init(&ptest, &VAL_CLASS::vvfunc);
//    ptest.pth.start();
//    ti.start();

    ntimer.timer_start(2, 1);
    ntimer.f_bind(&ot, &FILE_OUT::print_time_context);

    while(1)
    {
        sleep(1);
    }
}
