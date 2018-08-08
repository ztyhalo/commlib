/*
 * File:   timer_poll.h
 * Author: Administrator
 *
 * 文件监管库V1.1
 */
 
#ifndef __E_POLL_H__
#define __E_POLL_H__

#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <pthread.h>
#include <map>
#include <errno.h>
#include <iostream>
#include <QDebug>
#include "zprint.h"
#include "mutex.h"
#include "sigslot.h"

#define MAXFDS 4
#define EVENTS 100


class z_poll:virtual public MUTEX_CLASS
{
public:
      int epfd;        //文件指针
private:

    int delmark;     //是否在析构时关闭文件
    int eposize;     //监控文件大小
    int epomax;
    map<int, int> fdid;

public:
    z_poll(int max_num= MAXFDS)
    {

        epfd = epoll_create(max_num);
        if(epfd == -1)
        {
            zprintf1("epoll creat failed!\n");
            eposize = 0;
            delmark = 0;
            eposize = 0;
            return;
        }
        delmark = 1;
        epomax = max_num;
        eposize = 0;

    }
    void set_del_mark(int mark)
    {
        lock();
        delmark = mark;
        unlock();
    }

    int e_poll_add(int fd)
    {
        int err = 0;
        lock();
        if(eposize >= epomax)
        {
           zprintf1("add fd over!\n");
           err = -1;
           goto err_pro;
        }
        if(setNonBlock(fd) == false)
        {
            err =  -2;
            goto err_pro;
        }

        struct epoll_event ev;
        ev.data.fd = fd;
        ev.events = EPOLLIN | EPOLLET;
        err = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
        if(err == -1)
        {
            zprintf1("%s\n",strerror(errno));
            goto err_pro;
        }
        fdid.insert(pair<int,int>(eposize, fd));
        eposize++;
err_pro:
        unlock();
        return err;
    }

    int e_poll_del(int fd)
    {
        int err = 0;
        lock();
        struct epoll_event ev;
        ev.data.fd = fd;
        ev.events = EPOLLIN | EPOLLET;
        err = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);
        if(err == -1)
        {
            zprintf1("%s\n",strerror(errno));
            goto del_err;
        }
        eposize--;
del_err:
        unlock();
        return err;
    }

     int get_epoll_size(){
        return eposize;
    }
    bool setNonBlock (int fd)
    {
         int flags = fcntl (fd, F_GETFL, 0);
         flags |= O_NONBLOCK;
         if (-1 == fcntl (fd, F_SETFL, flags))
         {
             zprintf1("fd%d set non block failed!\n", fd);
             return false;
         }

         return true;
    }

    int wait_fd_change(int time)
    {
         struct epoll_event events[get_epoll_size()];
         memset(&events, 0, sizeof(events));
         int nfds = epoll_wait(epfd, events, get_epoll_size(), time);
         if(nfds > 0)
         {
             return nfds;
         }
         else
             return -1;
    }
    int get_fd(int id)
    {
        return fdid[id];
    }

    ~ z_poll()
    {
        zprintf3("delete destory!\n");
        if(delmark && epfd > 0)
            close(epfd);
    }

};

class Pth_Class
{
private:
    pthread_t pid;
private:
     static void * start_thread(void * arg){
          ((Pth_Class *)arg)->run();
         return NULL;
     }
public:
     Pth_Class(){
         pid = 0;
     }
     ~Pth_Class(){
         zprintf1("destory Pth_Class!\n");
         if(pid > 0){
            pthread_cancel(pid);
            pthread_join(pid, NULL);
            pid = 0;
         }
         zprintf1("destory Pth_Class over!\n");
     }
     void pth_class_exit(void){
          if(pid > 0){
             pthread_cancel(pid);
             pthread_join(pid, NULL);
             pid = 0;
          }
          zprintf3("destory Pth_Class over!\n");
     }

     int start(){
         if(pid == 0)
         {
             if(pthread_create(&pid, NULL, start_thread,this) != 0)
             {
                 zprintf1("creat pthread failed!\n");
                 return -1;
             }
             else
             {
                 return 0;
             }
         }
         zprintf2("pid %d have creat\n",(int)pid);
         return -1;
     }

     virtual void run() = 0;


};

class NCbk_Poll:public z_poll,public Pth_Class
{
public:
     NCbk_Poll(int max):z_poll(max){
     }
     ~NCbk_Poll()
     {
         zprintf3("destory NCbk_Poll!\n");
     }
};



template<class PTH_OWN>
class Z_PTH : public Pth_Class
{
public:
    Z_PTH()
    {
        m_pObj = NULL;
        m_Func = NULL;
    }

    Z_PTH(PTH_OWN* pObj, void (PTH_OWN::*func)(void) )
    {
        m_pObj = pObj;
        m_Func = func;
    }
    ~Z_PTH()
    {
        zprintf3("destory Z_PTH!\n");
    }

    void run()
    {
        if(m_pObj != NULL && m_Func != NULL){

            (m_pObj->*m_Func)();

        }
    }
    void pthread_init(PTH_OWN* pObj, void (PTH_OWN::*func)(void))
    {
        m_pObj = pObj;
        m_Func = func;
    }

private:
    PTH_OWN * m_pObj;
    void (PTH_OWN::*m_Func)(void);
};


template<class PARA_T>
class V_FILE_POLL:public z_poll
{
public:
Z_PTH<V_FILE_POLL> pth;
ZSignal<PARA_T>  fileval;
public:
    V_FILE_POLL()
    {
        pth.pthread_init(this, &V_FILE_POLL::pth_exe);
        pth.start();
    }
    ~V_FILE_POLL()
    {
        zprintf3("destory FILE_POLL!\n");
    }
    virtual void pth_exe(void) =0;

    template<class SLOT_OWN>
    void f_bind(SLOT_OWN* pObj, void (SLOT_OWN::*func)(PARA_T))
    {
//        m_pSlotSet.push_back( new ZSlot<PARA_T>(pObj,func) );
        fileval.Bind(pObj, func);
    }

};


template<class PARA_T>
class FILE_POLL:public V_FILE_POLL<PARA_T>
{
public:
        void pth_exe(void)
        {
            if(this->get_epoll_size() > 1)
            {
                zprintf1("FILE_POLL add file too much!\n");
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
                    zprintf4("FILE_POLL fd is %d\n", this->get_fd(0));
                    if((num = read(this->get_fd(0), &buf, sizeof(PARA_T))) == sizeof(PARA_T))
                    {
                        this->fileval(buf);
                    }
                    else
                    {
                        zprintf1("FILE_POLL file read num %d is error!\n", num);
                    }
                    zprintf4("FILE_POLL num is %d para is %d\n", num, sizeof(PARA_T));
                }


            }
        }
};

//template<class PARA_T>
//class FILE_POLL:public z_poll
//{
//public:
//Z_PTH<FILE_POLL> pth;
//ZSignal<PARA_T>  fileval;
//public:
//    FILE_POLL()
//    {
//        pth.pthread_init(this, &FILE_POLL::pth_exe);
//        pth.start();
//    }
//    ~FILE_POLL()
//    {
//        zprintf3("destory FILE_POLL!\n");
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
//                if((num = read(get_fd(0), &buf, sizeof(PARA_T))) == sizeof(PARA_T))
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
//    template<class SLOT_OWN>
//    void f_bind(SLOT_OWN* pObj, void (SLOT_OWN::*func)(PARA_T))
//    {
////        m_pSlotSet.push_back( new ZSlot<PARA_T>(pObj,func) );
//        fileval.Bind(pObj, func);
//    }

//};


template<class PARA_T>
class F_VOID_POLL:public z_poll
{
public:
Z_PTH<F_VOID_POLL> pth;
No_Signal          fileval;
public:
    F_VOID_POLL()
    {
        pth.pthread_init(this, &F_VOID_POLL::pth_exe);
        pth.start();
    }


    ~F_VOID_POLL()
    {
        zprintf3("destory FILE_POLL!\n");
    }
    void pth_exe(void)
    {
        if(get_epoll_size() > 1)
        {
            zprintf1("FILE_POLL add file too much!\n");
        }
        struct epoll_event events[1];
        PARA_T buf;
        int num;
        int nfds;
        for (;  ; )
        {
            memset(&events, 0, sizeof(events));
            nfds = epoll_wait(epfd, events, 1, -1);

            if(nfds > 0)
            {
                zprintf4("fd is %d\n", get_fd(0));
                if((num = read(get_fd(0), &buf, sizeof(PARA_T))) == sizeof(PARA_T))
                {
                    fileval();
                }
                else
                {
                    zprintf1("file read num %d is error!\n", num);
                }
                zprintf4("num is %d para is %d\n", num, sizeof(PARA_T));
            }


        }
    }
    template<class SLOT_OWN>
    void f_bind(SLOT_OWN* pObj, void (SLOT_OWN::*func)(void))
    {
//        m_pSlotSet.push_back( new ZSlot<PARA_T>(pObj,func) );
        fileval.Bind(pObj, func);
    }
};
 
#endif  /* __E_POLL_H__ */
