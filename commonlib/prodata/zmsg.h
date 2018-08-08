/****************************************************
 *************zmap库**********************
 *Version: 1.0
 *History: 2017.7.7
 *
****************************************************/


#ifndef __ZMSG_H__
#define __ZMSG_H__
#include <sys/types.h>
#include <map>
#include <string.h>
#include <sys/msg.h>
#include <zprint.h>

using namespace std;

template<class MSGDATA>
class Z_Msg
{
private:
    int msg_key;
    int msg_id;
    int msg_type;
public:
typedef    struct {
        long  int type;
        MSGDATA   val;
    }ZMSG;

public:
    Z_Msg(int key = 0, int type = 1):msg_key(key), msg_type(type)
    {
       if(key != 0)
       {
           create_object();
       }
    }

    ~Z_Msg()
    {
        if(msg_key != 0)
        {
            delete_object();
        }

    }

    void msg_init(int key = 0, int type = 1)
    {
        msg_key = key;
        msg_type = type;
    }

    bool get_msg(void)
    {
        msg_id = msgget(msg_key,  0666);

        if (msg_id == -1)
        {
            zprintf3("get failed!\n");
            return false;
        }
        zprintf3("get success!\n");
        return true;
    }

    bool create_object(void);
    bool delete_object(void);
    bool send_object(MSGDATA data);
    bool receive_object(MSGDATA & val, int mode);
    int GetMsgKey(void);
    bool send_object(MSGDATA data, int type);
};


template<class MSGDATA>
bool Z_Msg<MSGDATA>::create_object(void)
{
    msg_id = msgget(msg_key,  0666|IPC_CREAT);

    if (msg_id == -1)
    {
        zprintf3("creat failed!\n");
        return false;
    }
    zprintf3("creat success!\n");
    return true;
}
template<class MSGDATA>
bool Z_Msg<MSGDATA>::delete_object(void)
{
    if(msgctl(msg_id,IPC_RMID,0)== -1)
        return false;
    return true;
}

template<class MSGDATA>
bool Z_Msg<MSGDATA>::send_object(MSGDATA data)
{
    ZMSG v;
    v.type = msg_type;
    v.val = data;

    if( msgsnd(msg_id, &v, sizeof(MSGDATA), IPC_NOWAIT) ==-1)
    {
        zprintf3("send failed!\n");
        fprintf(stderr, "msgsnd failed\n");
        return false;
    }
    printf("send success!\n");
    return true;
}
template<class MSGDATA>
bool Z_Msg<MSGDATA>::send_object(MSGDATA data, int type)
{
    ZMSG v;
    v.type = type;
    v.val = data;

    if( msgsnd(msg_id, &v, sizeof(MSGDATA), IPC_NOWAIT) ==-1)
    {
        zprintf3("send failed!\n");
        fprintf(stderr, "msgsnd failed\n");
        return false;
    }
    printf("send success!\n");
    return true;
}


template<class MSGDATA>
bool Z_Msg<MSGDATA>::receive_object(MSGDATA & val, int mode)
{
    int len;

    ZMSG v;


    len = msgrcv(msg_id, &v, sizeof(MSGDATA), msg_type, mode);
    if( len ==-1)
    {
        zprintf3("receive failed!\n");
        return false;
    }
    val = v.val;
    return true;
}

template<class MSGDATA>
int Z_Msg<MSGDATA>::GetMsgKey(void)
{
    return msg_key;
}

#endif /*__ZMSG_H__*/
