/**
 * @author Bocheng Xiao
 * @date 2021-04-25
 * @version 1.0
 */
#include "ThreadController.hpp"

/**
 * @brief Create a ThreadController, assign its attributes' values with input
 * @param tc_number ThreadController ID
 * @param mtp_number ID of Thread Pool it belongs to
 * @param my_thread_pool Pointer of Thread Pool it belongs to
 */
ThreadController::ThreadController(int tc_number, int mtp_number, MyThreadPool* my_thread_pool) {
    this->my_thread_pool = my_thread_pool;
    tc_id = tc_number;
    mtp_belong_to = mtp_number;
}

/**
 * @brief Using thread to run the assigned Task
 *
 * First, delete the assigned Task from TaskBuffer, indicated it has been assigned to run.
 * Then, create an ACTUAL thread to run the assigned task.
 * After the task finished, BusyThreadContainer erase this thread and IdleThreadContainer add this thread.
 *
 * @param task_t assigned task needs to be run
 */
void ThreadController::ThreadRun(ThreadTask* task_t) {

    std::thread running_thread(&ThreadController::ActualRun, this, task_t);
    running_thread.detach();
}

/**
 * @brief Actual running function for task_t
 * @param task_t input task
 */
void ThreadController::ActualRun(ThreadTask* Threadtask) {

    Threadtask->TaskRun();

    my_thread_pool->busy_mtx.lock();
    my_thread_pool->BusyThreadContainer.erase(this->tc_id);
    my_thread_pool->busy_mtx.unlock();

    my_thread_pool->idle_mtx.lock();
    my_thread_pool->IdleThreadContainer.push_back(*this);
    my_thread_pool->idle_mtx.unlock();
}

/**
 * @brief Override the equal symbol into comparing Objects' Id instead of Objects
 * @param tc1
 * @param tc2
 * @return If equal between IDs
 */
bool operator==(ThreadController tc1, ThreadController tc2) {
    return tc1.tc_id == tc2.tc_id;
}

/**
 * @brief Override the not_equal symbol into comparing Objects' Id instead of Objects
 * @param t1
 * @param t2
 * @return If not_equal between IDs
 */
bool operator!=(ThreadController tc1, ThreadController tc2) {
    return tc1.tc_id != tc2.tc_id;
}

int ThreadController::get_tc_id() const {
    return this->tc_id;
}
