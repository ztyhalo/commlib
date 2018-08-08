#ifndef ZEVENT_H
#define ZEVENT_H

#include "e_poll.h"

#include <sys/eventfd.h>
#include "pro_data.h"
//template <class DATA_T>
class EVENT_L
{
public:
    int     evt_fd;
    eventfd_t val;
public:
    EVENT_L()
    {
       evt_fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
       if (evt_fd < 0)
       {
          zprintf1("event creat fail!\n");
       }
    }
    ~EVENT_L()
    {
        if (evt_fd > 0)
        {
            close(evt_fd);
        }
        zprintf3("destory EVENT_L!\n");
    }

    void add_event(void)
    {
        eventfd_write(evt_fd, 1);
    }
    int sub_event(void)
    {
        return eventfd_read(evt_fd, &val);
    }
};

template <class PARA_T, int N>
class Z_EVENT:public EVENT_L, public V_FILE_POLL<PARA_T>, public Z_Buf<PARA_T,N>
{
public:
    Z_EVENT()
    {
        if(evt_fd > 0)
        {
            this->e_poll_add(evt_fd);
        }
    }
    ~Z_EVENT()
    {
        if(evt_fd > 0)
        {
            this->e_poll_del(evt_fd);
        }
        zprintf3("destory Z_EVENT!\n");
    }

    void send_msg(PARA_T msg)
    {
        if(this->buf_write_data(msg) == 0)
        {
            this->add_event();
        }
    }

    void pth_exe(void)
    {
        if(this->get_epoll_size() > 1)
        {
            zprintf1("Z_EVENT add file too much!\n");
        }
        struct epoll_event events[1];
        PARA_T buf;
        int num;
        int nfds;
        for (;  ; )
        {
            memset(&events, 0, sizeof(events));
            nfds = epoll_wait(this->epfd, events, 1, -1);

            if(nfds > 0)
            {
                zprintf4("fd is %d\n", this->get_fd(0));
                if(this->sub_event() == 0)
                {
                    this->buf_read_data(buf);
                    this->fileval(buf);
                }
                else
                {
                    zprintf1("file read num %d is error!\n", num);
                }
                zprintf4("num is %d para is %d\n", num, sizeof(PARA_T));
            }

        }
    }
};

#endif // ZEVENT_H

