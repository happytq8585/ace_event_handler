#include <ace/Log_Msg.h>
#include <ace/Event_Handler.h>
#include <ace/Reactor.h>

class LoopStopper : public ACE_Event_Handler
{
public:
    LoopStopper(int signum=SIGINT)
    {
        this->sig_ = signum;
        ACE_Reactor::instance()->register_handler(signum, this);
    }
    virtual int handle_signal(int i, siginfo_t* ps=NULL, ucontext_t* uc=NULL)
    {
        ACE_DEBUG((LM_DEBUG, "SIGINT\n"));
        return -1;
    }
    virtual int handle_close(ACE_HANDLE fd, ACE_Reactor_Mask mask)
    {
        ACE_Reactor::instance()->remove_handler(this->sig_, ~0);
        ACE_Reactor::instance()->end_reactor_event_loop();
        return 0;
    }
private:
    int sig_;
};
class Timeout : public ACE_Event_Handler
{
public:
    Timeout():time_(0), delay_(1), inter_(2), id_(0), n_(1)
    {
        id_ = ACE_Reactor::instance()->schedule_timer(this, this, delay_, inter_);
    }
    virtual int handle_timeout(const ACE_Time_Value& t, const void* p)
    {
        ACE_DEBUG((LM_DEBUG, "%d %d\n", ((timespec_t)t).tv_sec, time_++));
        Timeout *q = (Timeout*)p;
        q->display();
        if (id_ && n_ == 0) {
            ACE_Reactor::instance()->cancel_timer(id_);
        }
        if (n_ == 0)
        {
            n_ = time_%6+1;
            delay_.set(1, 0);
            inter_.set(n_, 0);
            id_ = ACE_Reactor::instance()->schedule_timer(this, this, delay_, inter_);
        }
        n_--;
        return 0;
    }
    void display()
    {
        ACE_DEBUG((LM_DEBUG, "time_=%d\n", time_));
    }
private:
    int time_;
    int id_;
    int n_;
    ACE_Time_Value delay_;
    ACE_Time_Value inter_;
};
int main()
{
    LoopStopper ls;
    Timeout t;
    ACE_Reactor::instance()->run_reactor_event_loop();
    return 0;
}
