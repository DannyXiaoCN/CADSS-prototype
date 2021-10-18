//
// Created by 萧博成 on 2021/5/6.
//

#include <string>
#include <thread>
#include <memory>
#include <map>
#include <utility>
#include <iostream>
#include "Simulator.hpp"

std::vector<std::string> split(const std::string& str, const std::string& pattern) {
    // const char* convert to char*
    char* strc = new char[strlen(str.c_str()) + 1];
    strcpy(strc, str.c_str());
    std::vector<std::string> resultVec;
    char* tmpStr = strtok(strc, pattern.c_str());
    while (tmpStr != nullptr)
    {
        resultVec.emplace_back(tmpStr);
        tmpStr = strtok(nullptr, pattern.c_str());
    }
    delete[] strc;
    return resultVec;
}


Simulator::Simulator() {
    // Initialize ThreadPool
    TaskPool.Initialize(0, 100);

}

void Simulator::SimulatorStart() {

    // TODO: start server and client
    std::cout << "Now system start" << std::endl;
    std::thread run_thread;
    run_thread = std::thread(&SimuClock::RunningClock, &SimulatorClock);
    run_thread.detach();
}

int Simulator::AddAgentHelper() {
    return Agent_num++;
}
// TODO: read CSV and fill in the Log
// return AgentID
int Simulator::GenerateReplayCar(ReplayCarInfo replayCarInfo) {

    std::cout << "debug: now generating ReplayCar" << std::endl;

    //ReplayCarInfo: StartTimeStamp, EndTimeStamp, track_id, in_Predictor,
    // in_Predictor.dt,in_Predictor.horizon,ex_Predictor,ex_Predictor.dt,ex_Predictor.horizon
    auto it1 = ReplayCarBase.find(std::get<2>(replayCarInfo));
    auto targetReplayCar = it1->second;
    // push to the Log
    std::shared_ptr<AgentLog> ReplayCarTemp = std::make_shared<AgentLog>();
    auto it2 = targetReplayCar.find(std::get<0>(replayCarInfo));
    auto it2_next = it2;
    it2_next++;
    //    if(verbose_){
    //        std::cout <<"track_id:"<<it1->first<<std::endl;
    //        std::cout <<"frame_id:"<<it2->first<<std::endl;}
    while (it2 != targetReplayCar.end() &&it2->first <= std::get<1>(replayCarInfo)) {
        std::shared_ptr<LogContent> LCTemp = std::make_shared<LogContent>();
        for (int i = 0; i <= 4; i++) {
            LCTemp->current_state_[i] = it2->second.state[i];
        }
        if (it2_next != targetReplayCar.end()) {
            for (int i = 0; i <= 4; i++) {
                LCTemp->next_state_[i] = it2_next->second.state[i];
            }
        }
        LCTemp->agentInfo = it2->second.params;
        ReplayCarTemp->UpdateLog(LCTemp, it2->first);
        it2_next++;
        it2++;
    }
    // if(verbose_){
    //     for (int i = 0; i <= 4; i++)
    //         std::cout <<ReplayCarTemp.GetLog().get()->current_state_[i]<<std::endl;}
    int AgentID = AddAgentHelper();
    ReplayCarTemp->Initialize(AgentID);

    logManager.AddAgentLog(AgentID, ReplayCarTemp);

    auto traj= ReplayCarBase.find(std::get<2>(replayCarInfo));


    Vector initstate(6, 0.0);
    for (int i = 0; i < 6; i ++) initstate[i] = ReplayCarTemp->GetLog(StartTimeStamp)->current_state_[i];


    return AgentID;
}

int Simulator::GenerateRobotCar(RobotCarInfo robotCarInfo) {

    int AgentID = AddAgentHelper();
    if (std::get<9>(robotCarInfo) == "yes") {
        EgoCarID = AgentID;
    }
    std::shared_ptr<LogContent> LCTemp = std::make_shared<LogContent>();

    auto itf = ReplayCarBase.find(std::get<0>(robotCarInfo))->second.begin();
    if(itf->first<StartTimeStamp)
        itf = ReplayCarBase.find(std::get<0>(robotCarInfo))->second.find(StartTimeStamp);
    if(itf==ReplayCarBase.find(std::get<0>(robotCarInfo))->second.end())
        std::cout << "can not find this car in the StartTimeStamp__________________"<< std::endl;
    else{
        for (int i = 0; i <= 4; i++) {
            LCTemp->current_state_[i] = itf->second.state[i];
        }
        if(verbose_){
            std::cout << "Length_:"<<itf->second.params.Length_<< std::endl;
            std::cout << "width_:"<<itf->second.params.width_<< std::endl;
            std::cout << "mass_:"<<itf->second.params.mass_<< std::endl;
        }
        LCTemp->agentInfo.agentType = itf->second.params.agentType;
        LCTemp->agentInfo.Length_ = itf->second.params.Length_;
        LCTemp->agentInfo.width_ = itf->second.params.width_;
        LCTemp->agentInfo.mass_ = itf->second.params.mass_;
    }
    std::shared_ptr<AgentLog> RobotCarTemp = std::make_shared<AgentLog>();
    RobotCarTemp->PushLog(LCTemp, StartTimeStamp);
    RobotCarTemp->Initialize(AgentID);
    logManager.AddAgentLog(AgentID, RobotCarTemp);

    auto RCB_it1 = ReplayCarBase.find(std::get<0>(robotCarInfo));
    auto RCB_it2_prior = ++RCB_it1->second.begin();

    for (auto RCB_it2 = RCB_it1->second.begin(); RCB_it2->first < StartTimeStamp; RCB_it2++, RCB_it2_prior++) {

        Vector preFrame = RCB_it2->second.state;
        Vector priorFrame = RCB_it2_prior->second.state;

        for (int j = 0; j < REPLAY_INTERVAL; j ++){ // push several same states
            double interpolateValue = 1.0 - (1.0 * j / REPLAY_INTERVAL);
            double x = preFrame[0]; //interpolateValue*preFrame[0] + (1 - interpolateValue)*priorFrame[0];
            double y = preFrame[1]; //interpolateValue*preFrame[1] + (1 - interpolateValue)*priorFrame[1];
            double yaw = preFrame[2];   //interpolateValue*preFrame[2] + (1 - interpolateValue)*priorFrame[2];
            double v_x = interpolateValue*preFrame[3] + (1 - interpolateValue)*priorFrame[3];
            double v_y = interpolateValue*preFrame[4] + (1 - interpolateValue)*priorFrame[4];

            Vector result{x, y, yaw, v_x, v_y, 0};
            // TODO: FIGURE OUT WHY!!
            if (x == 0 && y == 0 && yaw == 0 && v_x == 0 && v_y == 0) {
                continue;
            }
            std::cout << "debug: pre states index: " <<  RCB_it2->first << std::endl;
            std::cout << "Pre state result: " << x << "," << y << "," << yaw << "," << v_x << "," << v_y << std::endl;

            RobotCarTemp->setPreState(result);
        }
    }
    AstarPlanner* astarPlanner = new AstarPlanner(RobotCarTemp, mapinfo, &logManager);
    CalPlanner* calPlanner = new CalPlanner(astarPlanner, RobotCarTemp, StartTimeStamp);
    TaskManager* plannerTask= new TaskManager(AgentID, &SimulatorClock, calPlanner, _simuType);
    TaskPool.AddTask(plannerTask);

    //    class NoPredictor* in_noPredictor = new class NoPredictor(cur_agent, &logManager, std::get<4>(robotCarInfo), std::get<5>(robotCarInfo));
    //    CalPredictor* calPredictor = new CalPredictor(in_noPredictor, cur_agent);
    //    TaskManager* predictorTask= new TaskManager(AgentID, &SimulatorClock, calPredictor);
    //    TaskPool.AddTask(predictorTask);

    PerfectController* perfectController = new PerfectController(RobotCarTemp);
    CalController* calController = new CalController(perfectController, RobotCarTemp, StartTimeStamp);
    TaskManager* controllerTask= new TaskManager(AgentID, &SimulatorClock, calController, _simuType);
    TaskPool.AddTask(controllerTask);

    return AgentID;
}

// std::map<int, LogContent> Simulator::LogByTimeStampGrpc(int agentID, int startTS, int endTS) {
//     LogRequest curr_request = LogRequest(agentID);
//     LogResponse curr_response = logManager.Responser(curr_request);
//     std::shared_ptr<AgentLog> current_log = curr_response.getResponse();
//     return current_log->CheckLog(startTS, endTS);
// }

void Simulator::AllTrackInfoLoadIn(const std::string& Track_Path) {
    std::map<int, Car_info>  carByframe;
    Car_info car;

    printf("Load CSV From %s\n", Track_Path.c_str());
    int ft_flag = 0;
    std::ifstream infile;
    infile.open(Track_Path);
    std::string x, y, line;
    int track_id, last_track_Id, frame_id, next_frame_id = 0;
    double xd, yd, yaw, vx, vy, yaw_rate, length, width;
    //TrajectoryPoints trajectory_points;
    getline(infile, line); // ignore the first line
    while (getline(infile, line))
    {
        std::vector<std::string> values = split(line, ",");
        track_id = stringToNum<double>(values[0]);
        frame_id = stringToNum<double>(values[1]);
        xd = stringToNum<double>(values[4]) - 1000.0;
        yd = stringToNum<double>(values[5]) - 1000.0;
        vx = stringToNum<double>(values[6]);
        vy = stringToNum<double>(values[7]);
        yaw = stringToNum<double>(values[8]);
        length = stringToNum<double>(values[9]);
        width = stringToNum<double>(values[10]);

        if (ft_flag == 0) {
            last_track_Id = track_id;
            ft_flag = 1;
        }

        car.state[0] = xd;
        car.state[1] = yd;
        car.state[2] = yaw;
        car.state[3] = sqrt(vx * vx + vy * vy);
        car.state[4] = 0;
        car.state[5] = 0;
        car.params.Length_ = length;
        car.params.width_ = width;
        car.params.agentType = values[3];

        if(last_track_Id!=track_id){
            ReplayCarBase.insert(std::pair<int, std::map<int, Car_info>>(last_track_Id, carByframe));
            carByframe.erase(carByframe.begin(), carByframe.end());
            carByframe.insert(std::pair<int, Car_info>(frame_id, car));
            last_track_Id=track_id;
        }
        else{
            carByframe.insert(std::pair<int, Car_info>(frame_id, car));
        }
    }
    ReplayCarBase.insert(std::pair<int, std::map<int, Car_info>>(last_track_Id, carByframe));
}

void Simulator::InitGrpc(std::string Map_Path, const std::string& Track_Path, std::string MapName, double egoEndPositionX,
                         double egoEndPositionY, std::list<int> TargetRightofWay, int startTimestamp, int endTimestamp,
                         const std::list<ReplayCarInfo>& ReplayCarList, const std::list<RobotCarInfo>& RobotCarList,
                         SimuClock::SimulationType simulationType) {
    mapName = std::move(MapName);
    EgoEndPositionX = egoEndPositionX;
    EgoEndPositionY = egoEndPositionY;
    targetRightofWay = std::move(TargetRightofWay);
    StartTimeStamp = startTimestamp;
    EndTimeStamp = endTimestamp;
    _simuType = simulationType;
    printf("# mapName: %s\n",mapName.c_str());
    printf("# EgoEndPositionX: %f,EgoEndPositionY: %f\n",EgoEndPositionX,EgoEndPositionY);
    printf("# StartTimeStamp: %d,EndTimeStamp: %d\n",StartTimeStamp,EndTimeStamp);

    SimulatorClock.ClockInit(StartTimeStamp, EndTimeStamp, simulationType, &logManager);

    printf("# MapPath_: %s, StartLaneletIds: %d, EndLaneletIds: %d\n", Map_Path.c_str(), int(mapReader->StartLaneletIds.size()), int(mapReader->EndLaneletIds.size()));
    AllTrackInfoLoadIn(Track_Path);
    for (auto & ReplayIt : ReplayCarList) {
        GenerateReplayCar(ReplayIt);
    }
    for (auto & RobotIt : RobotCarList) {
        std::cout << "Now generating: " << GenerateRobotCar(RobotIt) << std::endl;
    }
    logManager.setEA_STS(StartTimeStamp);
}
