/******************************************************************
 * File:   timer_poll.h
 * Author: Administrator
 *
 * linux定时器库V1.1
 * history:
 *          2017.7.6 修改timerevent中的成员，去除timer_internal和repeat
 ********************************************************************/

#ifndef __Z_TIMERS_H__
#define __Z_TIMERS_H__

#include "e_poll.h"

typedef struct
{
    char buf[8];
}T_L_Timer_Val;

class Z_TIMER:public F_VOID_POLL<T_L_Timer_Val>
{
public:
    int     timer_fd;
public:
    Z_TIMER()
    {
          timer_fd = timerfd_create(CLOCK_REALTIME, 0);
          if(timer_fd > 0)
          {
              e_poll_add(timer_fd);
          }
    }
    ~Z_TIMER()
    {
        if(timer_fd >0)
        {
            e_poll_del(timer_fd);
            close(timer_fd);
        }
        zprintf3("destory Z_TIMER!\n");
    }
    int timer_start(double timer_internal, bool repeat = true)
    {
        struct itimerspec ptime_internal;
        memset(&ptime_internal, 0x00, sizeof(ptime_internal));
        ptime_internal.it_value.tv_sec = (int) timer_internal;
        ptime_internal.it_value.tv_nsec = (timer_internal - (int) timer_internal)*1000000000;
        if(repeat)
        {
            ptime_internal.it_interval.tv_sec = ptime_internal.it_value.tv_sec;
            ptime_internal.it_interval.tv_nsec = ptime_internal.it_value.tv_nsec;
        }

        timerfd_settime(timer_fd, 0, &ptime_internal, NULL);
        return 0;
    }
};


void linuxDly(int s, int ms);
void linuxDly(int ms);
#endif // __Z_TIMERS_H__

