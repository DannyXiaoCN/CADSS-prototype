/**
 * @author Bocheng Xiao
 * @date 2021-04-25
 * @version 1.0
 */

#include "MyThreadPool.hpp"
#include <thread>
#include <iostream>
#include "ThreadController.hpp"
#include "TaskManager.hpp"


/**
 * @brief Initialization and start running of a thread pool
 *
 * First, assign the is_survive flag with true, number_of_thread with the input thread number
 * mtpId with input id. Then, use mutex lock and push the corresponding number of threads
 * (in the form of ThreadController class) into the idle_thread_container. Finally, use another
 * thread to run the Start(), which start the system.
 *
 * @param mtpId Input ID number of the created thread pool
 * @param thread_number Initial thread number of the created thread pool
 * @return created thread pool
 */
void MyThreadPool::Initialize(int mtpId, int thread_number) {

    is_survive = true;
    number_of_thread = thread_number;
    // Assign ID
    mtp_id = mtpId;

    // Initialize idle_thread_container by input thread_number
    idle_mtx.lock();
    for (int current_thread_number = 0; current_thread_number < thread_number; current_thread_number++) {
        ThreadController tc = ThreadController(current_thread_number, mtpId, this);
        IdleThreadContainer.push_back(tc);
    }
    idle_mtx.unlock();
    start_thread = std::thread(&MyThreadPool::Start, this);
    start_thread.detach();
}

/**
 * @brief System MAIN function including check if survived, assign thread to task and transform threads
 *
 * System will trigger a while-true loop to stay running. First check if system survive by is_survive flag.
 * Then, check if there any tasks remain to run in the TaskBuffer. If so[1], check any idle threads in
 * IdleThreadContainer can be assigned. If so[2], take one out, let it run the task and then put it into the
 * BusyThreadContainer, and then use ThreadRun function inside ThreadController to run the task.
 * If not[2], check idle thread circularly until one comes out and enter into processing.
 * If not[1], check task circularly until one comes out and enter into processing.
 */
void MyThreadPool::Start() {

    std::cout<< "SUCCESS" << std::endl;

    while(true) {
        if (!is_survive) {
            std::cout<< "EXIT()" << std::endl;
            break;
        }
        while(!TaskBuffer.empty()) {
            task_mtx.lock();
            ThreadTask* cur_task = TaskBuffer.back();
            TaskBuffer.pop_back();
            task_mtx.unlock();
            if (!IdleThreadContainer.empty()) {
                // mutex here
                idle_mtx.lock();
                ThreadController curr_tc = IdleThreadContainer.back();
                IdleThreadContainer.pop_back();
                idle_mtx.unlock();
                // concurrently running
                busy_mtx.lock();
                BusyThreadContainer.insert(std::map<int, ThreadController>::value_type(curr_tc.get_tc_id(), curr_tc));
                busy_mtx.unlock();
                curr_tc.ThreadRun(cur_task);
            } else {
                continue;
            }
        }
        // Able to set wait time here
    }
}

/**
 * @brief Add new task to the thread pool by pushing it into the TaskBuffer
 * @param input_function the input function in the format of Task class
 */
void MyThreadPool::AddTask(ThreadTask* input_function) {

    task_mtx.lock();
    TaskBuffer.push_back(input_function);
    task_mtx.unlock();
    std::cout <<"AddTask____________________________________________"<<std::endl;
}

/**
 * @brief Add thread to the thread pool by pushing new idle threads(in format of ThreadController) into IdleThreadContainer
 *        Also modify the number_of_thread attribute of the thread pool
 * @param thread_number the number of adding threads
 */
void MyThreadPool::AddThread(int thread_number) {

    // mutex
    idle_mtx.lock();
    int past_number_of_thread = this->number_of_thread;
    this->number_of_thread += thread_number;
    for (int current_thread_number = past_number_of_thread; current_thread_number < this->number_of_thread; current_thread_number++) {
        ThreadController tc = ThreadController(current_thread_number, mtp_id, this);
        IdleThreadContainer.push_back(tc);
    }
    idle_mtx.unlock();
}

/**
 * @brief Ending function for the thread pool by clearing 3 vessels and mark is_survive flag as false
 */
void MyThreadPool::End() {

    // mutex
    is_survive = false;
    IdleThreadContainer.clear();
    BusyThreadContainer.clear();
    TaskBuffer.clear();
}


///**
// * @brief Derived function of AddTask() aims at specialization for Prediction Task
// * [TESTING...]
// * @param input_function
// */
//void MyThreadPool::AddPredTask(PredictionTask input_function) {
//
//    std::cout<< input_function.getId() << std::endl;
//    task_mtx.lock();
//    TaskBuffer.push_back(input_function);
//    task_mtx.unlock();
//
//}
//
//
