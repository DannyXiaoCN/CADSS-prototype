//
// Created by 萧博成 on 2021/6/4.
//

#include <ctime>
#include <iostream>
#include "SimuClock.hpp"

void SimuClock::ClockInit(int StartTimeStamp, int EndTimeStamp, SimulationType simulationType, LogManager *logManager) {
    startTimeStamp = StartTimeStamp;
    endTimeStamp = EndTimeStamp;
    _simulationType = simulationType;
    _logManager = logManager;
}

// TODO: Modify if perform bad
void ClockDelayHelper(int time) {
    clock_t now = clock();
    while (clock() - now < time * 1000);
}

[[noreturn]] void SimuClock::RunningClock( ) {
    int STEP_lastTS;
    simuClock = startTimeStamp - 1;
    while(simuClock <= endTimeStamp) {
        if (_simulationType == SimulationType::RealTime) {
            ClockDelayHelper(SIM_TICK_MS);
            clockMutex.lock();
            if (simuClock == startTimeStamp - 1) {

            } else {
                if (!_logManager->Ready2Update(simuClock)) {
                    throw std::runtime_error("BUG: States are not ready");
                }
            }
            simuClock += 1;
            clockMutex.unlock();
            std::cout << "debug: Current Clock: " << simuClock << std::endl;
        } else if (_simulationType == SimulationType::STEP) {
            if (simuClock == startTimeStamp - 1 || STEP_CLOCK_INDICATOR == TaskNum) {
                std::cout << "debug: Simuclock update " << std::endl;
                if (simuClock != startTimeStamp - 1) {
                    if (!_logManager->Ready2Update(simuClock)) {
                        throw std::runtime_error("BUG: States are not ready");
                    }
                    std::cout << "debug: Simuclock Ready2Update pass " << std::endl;
                }
                clockMutex.lock();
                simuClock += 1;
                clockMutex.unlock();
                STEP_CLOCK_INDICATOR = 0;
                std::cout << "debug: Simuclock + 1 pass " << std::endl;
            }
        }
    }
    std::cout << "Clock Stop" << std::endl;
}

int SimuClock::getClock() {
    return simuClock;
}

int SimuClock::getStartTimeStamp() {
    return startTimeStamp;
}

int SimuClock::getEndTimeStamp() {
    return endTimeStamp;
}

void SimuClock::updateTaskNum() {
    TaskNum += 1;
}

