//
// Created by 萧博成 on 2021/10/17.
//

#ifndef CADSS_PROTOTYPR_TASKMANAGER_HPP
#define CADSS_PROTOTYPR_TASKMANAGER_HPP

#include "SimuClock.hpp"
#include "AgentLog.hpp"
#include "ThreadTask.hpp"
#include "CalTask.hpp"

class TaskManager : public ThreadTask {

public:
    ~TaskManager(){
        std::cout<<"task ~   "<<std::endl;
    };
    TaskManager(int Agentid, SimuClock* simuClock, CalTask* TaskInput, SimuClock::SimulationType simulationType);
    void TaskRun() override;

private:

    SimuClock::SimulationType _simulationType;
    void TaskActualRun();
    SimuClock* _simuClock;
    CalTask* Calulation_;
    int AgentID;
    int TaskClock = -1;

};

#endif //CADSS_PROTOTYPR_TASKMANAGER_HPP
