//
// Created by 萧博成 on 2021/10/17.
//

#ifndef CADSS_PROTOTYPR_SIMUCLOCK_HPP
#define CADSS_PROTOTYPR_SIMUCLOCK_HPP

#include <mutex>
#include <LogManager.hpp>

class SimuClock {

public:

    enum SimulationType {
        RealTime = 0,
        STEP = 1,
        Human = 2
    };
    enum SimulatorState {
        Running = 0,
        Paused = 1,
        Reset = 2
    };

    void ClockInit(int StartTimeStamp, int EndTimeStamp, SimulationType simulationType, LogManager *logManager);
    int getClock();
    int getEndTimeStamp();
    int getStartTimeStamp();
    int STEP_CLOCK_INDICATOR = 0;
    void updateTaskNum();
    [[noreturn]] void RunningClock();

private:

    LogManager* _logManager;
    int simuClock = -1;
    std::mutex clockMutex;
    int startTimeStamp;
    int endTimeStamp;
    int TaskNum = 0;
    SimulationType _simulationType;
};

const int SIM_TICK_MS = 100;                        // simulation step (in ms).
const double SIM_TICK_SECOND = 0.001 * SIM_TICK_MS;      // simulation step (in seconds).
const int REPLAY_INTERVAL = 100 / SIM_TICK_MS;      // NOTE: SIM_TICK_MS must be a factor of 100


#endif //CADSS_PROTOTYPR_SIMUCLOCK_HPP
