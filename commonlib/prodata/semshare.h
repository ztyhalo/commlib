/****************************************************
 *************进程间通讯数据库**********************
 *Version: 1.1
 *History: 2017.7.3
 *         2017.7.6添加回调类
 *         2017.7.11 将1个文件拆分为多个文件
****************************************************/

#ifndef __SEMSHARE_H__
#define __SEMSHARE_H__
#include "sharemem.h"
//linux共享内存加信号 不继承线程类
template <class T>
class Sem_Share_Data:public ShareDataT<T>
{
public:
    int   semid;
    uint  bufsize;
    uint * rd_buf_p;
    uint * wr_buf_p;
    key_t  sem_key;
public:
    Sem_Share_Data()
    {
        semid = 0;
        sem_key = 19860610;
        bufsize = 0;
        rd_buf_p = NULL;
        wr_buf_p = NULL;
    }
    Sem_Share_Data(uint sz, key_t semkey = 19860610, key_t sharekey=20130410)
    {
        creat_sem_data(sz,semkey, sharekey);
    }

    int creat_sem_data(uint sz, key_t semkey = 19860610, key_t sharekey=20130410);
    int write_send_data(T val);
    int read_send_data(T & val);
    int wait_thread_sem(void);
};

template <class T>
int Sem_Share_Data<T>::creat_sem_data(uint sz, key_t semkey, key_t sharekey)
{
    int err = 0;
    uint * midp = NULL;
    printf("share key is %d\n", sharekey);
    err = this->creat_data(sz+sizeof(uint)*2, sharekey);
    bufsize = sz/sizeof(T);
    if(err == 0)
    {
        midp = (uint *)(this->data+bufsize);
        rd_buf_p = midp;
        wr_buf_p = midp+1;
        *rd_buf_p = 0;
        *wr_buf_p = 0;
        semid = create_sem(semkey, 0);
        err = semid >0 ? 0: -2;
    }
    return err;
}

template <class T>
int Sem_Share_Data<T>::write_send_data(T val)
{
    uint mid = *wr_buf_p;
    if(bufsize == 1 && mid == 0)
        this->set_data(mid, val);
    else if(bufsize == 1)
        printf("write off \n");
    else
        this->set_data(mid, val);
    mid++;
    if(bufsize > 1)
    {
        mid %= bufsize;
        if(mid == *rd_buf_p)
        {
            printf("write off %d %d\n", mid, *rd_buf_p);
            return -1;
        }
    }

    *wr_buf_p = mid;
    sem_v(semid);
    return 0;
}

template <class T>
int Sem_Share_Data<T>::read_send_data(T & val)
{
    uint mid = *rd_buf_p;

    if(bufsize == 1)
        mid = 0;
    this->get_data(mid, val);

    if(bufsize == 1)
    {
        *wr_buf_p = 0;
    }
    else
    {
        mid++;
        mid %= bufsize;

        *rd_buf_p = mid;
    }
    return 0;
}
template <class T>
int Sem_Share_Data<T>::wait_thread_sem(void)
{
    if(sem_p(semid) == 0)
        return 0;
    else
        return -1;
}

//linux共享内存加信号 继承线程类
template <class T, class FAT>
class Sem_Pth_Data:public Sem_Share_Data<T>, public Call_B_T<T, FAT>
{
public:
        void run();

};

template <class T,class FAT>
void  Sem_Pth_Data<T,FAT>::run(void)
{
    while(1)
    {
        if(sem_p(this->semid) == 0)
        {
            T val;
            if(read_send_data(val) == 0)
            {
                 if(this->z_callbak != NULL)           //执行操作
                 {
                     this->z_callbak(this->father , val);
                 }
            }
        }
    }
}
//qt共享内存加linux信号 不继承线程类
template <class T>
class Sem_Qt_Data:public QTShareDataT<T>
{
public:
    int   semid;
    uint  bufsize;
    uint16_t * rd_buf_p;
    uint16_t * wr_buf_p;
    key_t  sem_key;
public:
    Sem_Qt_Data()
    {
        semid = 0;
        sem_key = 19860610;
        bufsize = 0;
        rd_buf_p = NULL;
        wr_buf_p = NULL;
    }
    Sem_Qt_Data(uint sz, key_t semkey = 19860610, QString sharekey="lhshare")
    {
        creat_sem_data(sz,semkey, sharekey);
    }
    ~Sem_Qt_Data(){
        zprintf3("destory Sem_Qt_Data!\n");
    }

    int creat_sem_data(uint sz, key_t semkey = 19860610, QString sharekey="lhshare");
    int write_send_data(T val);
    int read_send_data(T & val);
    int wait_thread_sem(void);
};

template <class T>
int Sem_Qt_Data<T>::creat_sem_data(uint sz, key_t semkey, QString sharekey)
{
    int err = 0;
    uint16_t * midp = NULL;
    T * midaddr= NULL;

    qDebug()<< sharekey;
    printf("semkey is %d\n\n\n", semkey);
    midaddr = this->creat_data(sz+sizeof(uint16_t)*2, sharekey);
    bufsize = sz/sizeof(T);
    if(midaddr != NULL)
    {
        midp = (uint16_t *)(this->data+bufsize);
        wr_buf_p = midp;
        rd_buf_p = midp+1;
        *rd_buf_p = 0;
        *wr_buf_p = 0;
        semid = create_sem(semkey, 0);
        err = semid >0 ? 0: -2;
    }
    else
        err = -1;
    return err;
}

template <class T>
int Sem_Qt_Data<T>::write_send_data(T val)
{
    this->lock_qtshare();
    uint16_t mid = *wr_buf_p;
    if(bufsize == 1 && mid == 0)
        this->noblock_set_data(mid, val);
    else if(bufsize == 1)
        printf("write off \n");
    else
        this->noblock_set_data(mid, val);
    mid++;
    if(bufsize > 1)
    {
        mid %= bufsize;
        if(mid == *rd_buf_p)
        {
            printf("write off %d %d\n", mid, *rd_buf_p);
            this->unlock_qtshare();
            return -1;
        }
    }

    *wr_buf_p = mid;
    this->unlock_qtshare();
    sem_v(semid);
    return 0;
}

template <class T>
int Sem_Qt_Data<T>::read_send_data(T & val)
{
    this->lock_qtshare();
    uint16_t mid = *rd_buf_p;


    if(bufsize == 1)
        mid = 0;
    this->noblock_get_data(mid, val);

    if(bufsize == 1)
    {
        *wr_buf_p = 0;
    }
    else
    {
        mid++;
        mid %= bufsize;

        *rd_buf_p = mid;
//        qDebug("bufsize %d sem qt read %d write %d\n", bufsize ,mid, *wr_buf_p);
    }
    this->unlock_qtshare();
    return 0;
}
template <class T>
int Sem_Qt_Data<T>::wait_thread_sem(void)
{
    if(sem_p(semid) == 0)
        return 0;
    else
        return -1;
}

//qt共享内存加linux信号 继承线程类
template <class T, class FAT>
class Sem_QtPth_Data:public Sem_Qt_Data<T>, public Call_B_T<T, FAT>
{
public:
        ~Sem_QtPth_Data(){
        zprintf3("destory Sem_QtPth_Data!\n");
    }

        void run();

};

template <class T,class FAT>
void  Sem_QtPth_Data<T,FAT>::run(void)
{
    printf("qt sem run!\n\n");
//    int res;
//   res = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,   NULL);   //设置立即取消
//   if (res != 0)
//   {
//       perror("Thread pthread_setcancelstate failed");
//       exit(EXIT_FAILURE);
//   }
    while(1)
    {
       if(sem_p(this->semid, 10) == 0)
        {
            printf("qt sem receive!\n\n");
            T val;
            if(this->read_send_data(val) == 0)
            {
                 if(this->z_callbak != NULL)           //执行操作
                 {
                     this->z_callbak(this->father , val);
                 }
            }
        }
        else{
          pthread_testcancel();
       }
    }
    pthread_exit(NULL);
}

#endif /*__SEMSHARE_H__*/
