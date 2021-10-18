//
// Created by 萧博成 on 2021/10/17.
//

#ifndef CADSS_PROTOTYPR_SIMULATOR_HPP
#define CADSS_PROTOTYPR_SIMULATOR_HPP


#include "SimuClock.hpp"
#include "../ThreadPool/MyThreadPool.hpp"
#include "../Log/LogManager.hpp"
#include "CalPredictor.hpp"
#include "CalPlanner.hpp"
#include "CalController.hpp"
#include <mutex>
#include <sstream>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <cstring>
#include <tuple>
#include <map>
#include <utility>
#include <../Maps/TrafficInfo.hpp>
#include <LaneletMapReader.hpp>
#include <../Behaviours/ReplayGenerator.hpp>
#include <TaskManager.hpp>


// track_id,Planner,Planner.Para,in_Predictor,in_Predictor.dt,in_Predictor.horizon,ex_Predictor,ex_Predictor.dt,ex_Predictor.horizon,ego_car
typedef std::tuple<int, std::string, std::string, std::string, double, double, std::string, double, double, std::string> RobotCarInfo;
// StartTimeStamp, EndTimeStamp, track_id, in_Predictor,in_Predictor.dt,in_Predictor.horizon,ex_Predictor,ex_Predictor.dt,ex_Predictor.horizon
typedef std::tuple<int, int, int, std::string, double, double, std::string, double, double> ReplayCarInfo;

class Car_info {
public:
    AgentInfo params;
    std::vector<double> state = std::vector<double>(6);
};

template <class Type>
        Type stringToNum(const std::string& str)
        {
            std::istringstream iss(str);
            Type num;
            iss >> num;
            return num;
        }

        class Simulator {

            public:
                void SimulatorStart();
                Simulator();

                int GenerateReplayCar(ReplayCarInfo replayCarInfo);
                int GenerateRobotCar(RobotCarInfo robotCarInfo);
                int AddAgentHelper();

                void InitGrpc(std::string Map_Path, const std::string& Track_Path, std::string MapName,
                              double egoEndPositionX, double egoEndPositionY, std::list<int> TargetRightofWay,
                              int StartTimestamp, int EndTimestamp,const std::list<ReplayCarInfo>& ReplayCarList,
                              const std::list<RobotCarInfo>& RobotCarList, SimuClock::SimulationType simulationType);
                std::map<int, LogContent> LogByTimeStampGrpc(int agentID, int startTS, int endTS);
                void AllTrackInfoLoadIn(const std::string& Track_Path);

                int MaxUpdateTimes_=3600000;



                LogManager logManager;

                static bool verbose_;
                static int updateTimes;

                //void  Simulator::setmap4Robotcar(MapInfo*mapinfo,RobotCarInfo* robotCarInfo, AgentLog* LogTemp);

            private:

                std::map<int, std::map<int, Car_info>> ReplayCarBase;

                MyThreadPool TaskPool;

                std::mutex simulatorMutex;

                std::string mapName;
                std::list<int> targetRightofWay;
                SimuClock SimulatorClock;
                int Agent_num = 0;
                int Task_num = 0;

                double EgoEndPositionX, EgoEndPositionY;
                int EgoCarID;
                int StartTimeStamp = 0;
                int EndTimeStamp;

                SimuClock::SimulationType _simuType;
            };


#endif //CADSS_PROTOTYPR_SIMULATOR_HPP
