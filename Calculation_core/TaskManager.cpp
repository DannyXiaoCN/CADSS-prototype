//
// Created by 萧博成 on 2021/10/17.
//
#include <thread>
#include <LogResponse.hpp>
#include <utility>
#include "TaskManager.hpp"

TaskManager::TaskManager(int Agentid, SimuClock* simuClock, CalTask* TaskInput, SimuClock::SimulationType simulationType) {

    _simuClock = simuClock;
    AgentID = Agentid;
    Calulation_ = TaskInput;
    simuClock->updateTaskNum();
    _simulationType = simulationType;
    std::cout <<"TaskManager____________________________________________"<<std::endl;
}


void TaskManager::TaskRun() {

    int ETS = _simuClock->getEndTimeStamp();
    int currentClock = _simuClock->getClock();
    while(currentClock <= ETS) {
        currentClock = _simuClock->getClock();
        if (currentClock >= _simuClock->getStartTimeStamp()) {
            TaskClock = _simuClock->getClock();
            TaskActualRun();
        }
        if (currentClock == ETS) {
            break;
        }
    }
}

void TaskManager::TaskActualRun() {
    Calulation_->Run(_simuClock, TaskClock, _simulationType);
}
