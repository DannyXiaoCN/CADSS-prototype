/**
 * @author Bocheng Xiao
 * @date 2021-04-25
 * @version 1.0
 */
#ifndef CADSS_PROTOTYPR_MYTHREADPOOL_HPP
#define CADSS_PROTOTYPR_MYTHREADPOOL_HPP

#include <vector>
#include <mutex>
#include <thread>
#include <map>
#include "ThreadController.hpp"

class ThreadController;
class ThreadTask;
class MyThreadPool {

public:
    // Initialization
    void Initialize(int mtpId, int thread_number);
    // Basic generate function for thread pool
    void Start();
    void AddTask(ThreadTask* input_function);
    void AddThread(int thread_number);
    void End();
    /**
     * 3 vessels for thread and task
     * IdleThreadContainer: Container of idle threads, threads inside can be used for running task immediately
     *                      Will be initialized in the beginning as a specific number.
     * BusyThreadContainer: Container of busy threads, threads inside indicate unavailable for current task running
     *                      Start as 0 threads inside
     * TaskBuffer: Buffer container of Tasks, tasks inside need to be processed ASAP.
     *
     * Idle thread used for running task will switch into Busy thread, and the Task will be removed from TaskBuffer
     */
    std::vector<ThreadController> IdleThreadContainer;
    std::map<int, ThreadController> BusyThreadContainer;
    std::vector<ThreadTask*> TaskBuffer;
    // 3 mutexes for concurrency
    std::mutex idle_mtx;
    std::mutex busy_mtx;
    std::mutex task_mtx;

    // Attributes for thread pool
    int number_of_thread;
    int number_of_tasks = 0;
    bool is_survive;
    int mtp_id;
    // thread pool start helper
    std::thread start_thread;
};

#endif //CADSS_PROTOTYPR_MYTHREADPOOL_HPP
