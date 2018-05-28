#ifndef ZEVENT_H
#define ZEVENT_H

#include "e_poll.h"

#include <sys/eventfd.h>
//template <class DATA_T>
class EVENT_BUF:public MUTEX_CLASS
{
public:
    int evt_fd;
public:
    EVENT_BUF()
    {
        ;
    }
};

class Z_EVENT:public EVENT_BUF, public z_poll
{
public:
    int val;
public:
    Z_EVENT()
    {
        lock();
        val = 5;
        unlock();
        zprintf1("%d\n", val);
    }
};

//template <class PARA_T>
//class Z_EVENT:public FILE_POLL<PARA_T>
//{
//public:
//    int evt_fd;
//public:
//    Z_EVENT()
//    {
//        ;
//    }
//    ~Z_EVENT()
//    {
//        zprintf3("destory Z_EVENT!\n");
//    }

//    void pth_exe(void)
//    {
//        if(get_epoll_size() > 1)
//        {
//            zprintf1("FILE_POLL add file too much!\n");
//        }
//        struct epoll_event events[1];
//        PARA_T buf;
//        int num;
//        int nfds;
//        for (;  ; )
//        {
//            memset(&events, 0, sizeof(events));
//            nfds = epoll_wait(epfd, events, 1, -1);

//            if(nfds > 0)
//            {
//                zprintf4("fd is %d\n", get_fd(0));
//                if(eventfd_read(get_fd(0), &buf) != 0)
//                {
//                    fileval(buf);
//                }
//                else
//                {
//                    zprintf1("file read num %d is error!\n", num);
//                }
//                zprintf4("num is %d para is %d\n", num, sizeof(PARA_T));
//            }


//        }
//    }
//};

#endif // ZEVENT_H

