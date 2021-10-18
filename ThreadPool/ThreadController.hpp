/**
 * @author Bocheng Xiao
 * @date 2021-04-25
 * @version 1.0
 */
#ifndef CADSS_PROTOTYPR_THREADCONTROLLER_HPP
#define CADSS_PROTOTYPR_THREADCONTROLLER_HPP

class MyThreadPool;
class ThreadController {

public:

    // Override equal and not_equal symbol
    friend bool operator == (ThreadController tc1, ThreadController tc2);
    friend bool operator != (ThreadController tc1, ThreadController tc2);

    // Basic functions
    ThreadController(int tc_number, int mtp_number, MyThreadPool *my_thread_pool);
    void ThreadRun(ThreadTask* task_t);
    void ActualRun(ThreadTask* task_t);
    int get_tc_id() const;

private:
    // Basic attributes
    MyThreadPool *my_thread_pool;
    int tc_id;
    int mtp_belong_to;
    bool is_busy = false;
};

#endif //CADSS_PROTOTYPR_THREADCONTROLLER_HPP
