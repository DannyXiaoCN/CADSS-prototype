//
// Created by 萧博成 on 2021/4/22.
//

#include <cstdlib>
#include <fstream>
#include <string>
#include <string.h>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <utility>
#include "AgentLog.hpp"

PlanTra PlanTraReading(const std::string& Log_path);
Obstacle ObstacleReading(const std::string& Log_path);
PredictTra PredictTraReading(const std::string& Log_path);
Mode ModeReading(const std::string& Log_path);
std::string TraPt2CSVHelper(TraPt traPt);
/**
 * @brief Create an AgentLog for LogUser
 * @param LogUser the Agent
 */

AgentLog::AgentLog(){
};

void AgentLog::Initialize(int LogUser) {
    AgentID = LogUser;
}

void AgentLog::SetPath(std::string Current_path) {
    system_path = std::move(Current_path);
}

/**
 * @brief Print function for outputing the Log into a .csv file
 */
int AgentLog::Log2CSV() {

    std::map<int, std::shared_ptr<LogContent>>::iterator iter;
    for(iter = agentLog.begin(); iter != agentLog.end(); iter++) {
        std::cout << "enter printing" << std::endl;
        int current_ts = iter->first;
        LogContent lc_t = *iter->second;

        auto tempPR = lc_t.PlannerResult.pts; // PlannerResult
        std::string dir = system_path + "LogFile";
        mkdir(dir.c_str(), S_IRWXG | S_IRWXU);
        dir = system_path + "LogFile/Agent" + std::to_string(AgentID) + "_" + std::to_string(current_ts);
        mkdir(dir.c_str(), S_IRWXG | S_IRWXU);
        std::ofstream out_p(system_path + "LogFile/Agent" + std::to_string(AgentID) + "_" + std::to_string(current_ts) + "/PlannerResult" + ".txt");
        //         std::cout << system_path + "LogFile/Agent" + std::to_string(AgentID) + "_" + std::to_string(current_ts) + "/PlannerResult" + ".txt" << std::endl;
        //
        //         if (out_p.is_open()) {
        //             std::cout << "OPEN PASS" << std::endl;
        //         } else {
        //             std::cout << "OPEN FAIL" << std::endl;
        //         }

        for (auto & iterPR : tempPR) {
            out_p << TraPt2CSVHelper(iterPR.second) << std::endl;
        }
        out_p.close();

        auto tempOI = lc_t.obstacle_info_.obstacle; // ObstacleInfo
        std::ofstream out_o(system_path + "LogFile/Agent" + std::to_string(AgentID) + "_" + std::to_string(current_ts) + "/ObstacleInfo" + ".txt");
        out_o << "TEST" << std::endl;
        for (auto & iterOI : tempOI) {
            out_o << iterOI.first << std::endl;
            out_o << iterOI.second.distence << std::endl;
            out_o << iterOI.second.yielding << std::endl;
            out_o << TraPt2CSVHelper(iterOI.second.point_in) << std::endl;
        }
        out_o.close();

        std::ofstream out_m(system_path + "LogFile/Agent" + std::to_string(AgentID) + "_" + std::to_string(current_ts) + "/Mode" + ".txt");
        out_m << lc_t.current_mode_ << std::endl; // Mode
        out_m.close();

        std::ofstream out_e(system_path + "LogFile/Agent" + std::to_string(AgentID) + "_" + std::to_string(current_ts) + "/ExPrediction" + ".txt");
        auto tempEP = lc_t.ex_prediction.Trajs; // ExPrediction
        for (auto & iterEP : tempEP) {
            out_e << iterEP->Probability << std::endl;
            std::string conflict_temp;
            for (auto & iterEPC : iterEP->confilictlanesID) {
                conflict_temp.append(std::to_string(iterEPC.second));
            }
            out_e << conflict_temp << std::endl;
            for (auto & iterEPS : iterEP->pts) {
                out_e << TraPt2CSVHelper(iterEPS.second) << std::endl;
            }
            out_e << "END" << std::endl;
        }
        out_e.close();

        std::ofstream out_i(system_path + "LogFile/Agent" + std::to_string(AgentID) + "_" + std::to_string(current_ts) + "/InPrediction" + ".txt");
        auto tempIP = lc_t.in_prediction.Trajs; // ExPrediction
        for (auto & iterIP : tempIP) {
            out_i << iterIP->Probability << std::endl;
            std::string conflict_temp;
            for (auto & iterIPC : iterIP->confilictlanesID) {
                conflict_temp.append(std::to_string(iterIPC.second));
            }
            out_i << conflict_temp << std::endl;
            int counter_id = 0;
            for (auto & iterIPS : iterIP->pts) {
                out_i << counter_id << std::endl;
                out_i << TraPt2CSVHelper(iterIPS.second) << std::endl;
                counter_id++;
            }
            out_i << "END" << std::endl;
        }
        out_i.close();

        std::ofstream out_c(system_path + "LogFile/Agent" + std::to_string(AgentID) + "_" + std::to_string(current_ts) + "/CurrentState" + ".txt");
        std::string state_temp; // Current State
        auto tempS = lc_t.current_state_;
        for (int i = 0; i < 6; i++) {
            state_temp.append(std::to_string(tempS[i]));
            state_temp.append(",");
        }
        out_c << state_temp << std::endl;
        out_c.close();

        std::ofstream out_cc(system_path + "LogFile/Agent" + std::to_string(AgentID) + "_" + std::to_string(current_ts) + "/CurrentControl" + ".txt");
        std::string control_temp; // Current Control
        auto tempC = lc_t.current_control;
        for (int i = 0; i < 3; i++) {
            control_temp.append(std::to_string(tempC[i]));
            control_temp.append(",");
        }
        out_cc << control_temp << std::endl;
        out_cc.close();
    }
    return 1;
}


//// ReplayCar: Allin / Selective: Timestamp, certain area other is empty
// void AgentLog::CSV2Log(const std::string& Log_path, ImportMode mode, int StartTimeStamp, int EndTimeStamp) {
//
//     if (mode == ReplyCarGenerate) {
//         int agentLogCounter = 0;
//         std::ifstream in;
//         LogContent result;
//         for (int i = StartTimeStamp; i <= EndTimeStamp; i++) {
//             result.PlannerResult = PlanTraReading(Log_path + "/" + std::to_string(i));
//             agentLog[agentLogCounter] =std::make_shared<LogContent>(result) ;
//         }
//     } else if (mode == Others) {
//
//     }
//
// }


LogContent AgentLog::Log2Update() {
    return AgentUpdate;
}


void AgentLog::PushLog(std::shared_ptr<LogContent> lc, int cur_timpstamp) {
    agentLog.insert(std::map<int, std::shared_ptr<LogContent>>::value_type(cur_timpstamp, lc));
}

void AgentLog::UpdateLog(std::shared_ptr<LogContent> lc, int cur_timpstamp) {
    agentLog[cur_timpstamp] = lc;
}


std::map<int, std::shared_ptr<LogContent>> AgentLog::CheckLog(int start_timestamp, int end_timestamp) {

    std::map<int, std::shared_ptr<LogContent>> temp_v;

    auto iter = agentLog.find(start_timestamp);
    if (start_timestamp == end_timestamp) {
        temp_v.insert(std::map<int, std::shared_ptr<LogContent>>::value_type(iter->first, iter->second));
    } else {
        while (iter->first != end_timestamp) {
            temp_v.insert(std::map<int, std::shared_ptr<LogContent>>::value_type(iter->first, iter->second));
            iter++;
        }
    }
    return temp_v;
}
void AgentLog::setPreState(Vector state) {
    if (this->dimState != state.size()) {
        throw std::runtime_error("bad dimension");
    }
    this->preState.push_back(state);
}

std::vector<Vector> AgentLog::get_preState(){
    return preState;
}

std::shared_ptr<LogContent> AgentLog::GetLog(int timestamp) {

    auto iter = agentLog.find(timestamp);
    return iter->second;
}

int AgentLog::isEgoCar() {
    return EgoCar;
}

void AgentLog::setEgoCar() {
    EgoCar = 1;
}

int AgentLog::isExPredictor() {
    return ExPredictor;
}

void AgentLog::setExPredictor() {
    ExPredictor = 1;
}

int AgentLog::getAgentID() {
    return AgentID;
}

void AgentLog::SetMapinfo(MapInfo* m)
{
    this->mapinfo = m;
}

std::string TraPt2CSVHelper(TraPt traPt) {
    std::string temp = std::to_string(traPt.t);
    temp.append(","+std::to_string(traPt.x));
    temp.append(","+std::to_string(traPt.y));
    temp.append(","+std::to_string(traPt.yaw));
    temp.append(","+std::to_string(traPt.yaw_rate));
    temp.append(","+std::to_string(traPt.vx));
    temp.append(","+std::to_string(traPt.vy));
    temp.append(","+std::to_string(traPt.a));
    temp.append(","+std::to_string(traPt.jerk));
    temp.append(","+std::to_string(traPt.current_lanelet_ID));
    temp.append(","+std::to_string(traPt.s_of_current_lanelet));
    temp.append(","+std::to_string(traPt.d_of_current_lanelet));
    std::cout << temp << std::endl;
    return temp;
}

/// Split a string
std::list<std::string> splitl(const std::string &str,const std::string &pattern) {
    // const char* convert to char*
    char * strc = new char[strlen(str.c_str())+1];
    strcpy(strc, str.c_str());
    std::list<std::string> resultVec;
    char* tmpStr = strtok(strc, pattern.c_str());
    while (tmpStr != nullptr)
    {
        resultVec.emplace_back(tmpStr);
        tmpStr = strtok(nullptr, pattern.c_str());
    }
    delete[] strc;
    return resultVec;
}

std::list<int> ConflictSplit(const std::string &str,const std::string &pattern) {
    // const char* convert to char*
    char * strc = new char[strlen(str.c_str())+1];
    strcpy(strc, str.c_str());
    std::list<int> resultVec;
    char* tmpStr = strtok(strc, pattern.c_str());
    while (tmpStr != nullptr)
    {
        resultVec.emplace_back(atoi(tmpStr));
        tmpStr = strtok(nullptr, pattern.c_str());
    }
    delete[] strc;
    return resultVec;
}

// TraPt TraPtReading(std::list<std::string> input) {
//     TraPt result;
//     result.t = atof(input.front().c_str());
//     input.pop_front();
//     result.x = atof(input.front().c_str());
//     input.pop_front();
//     result.y = atof(input.front().c_str());
//     input.pop_front();
//     result.yaw = atof(input.front().c_str());
//     input.pop_front();
//     result.yaw_rate = atof(input.front().c_str());
//     input.pop_front();
//     result.vx = atof(input.front().c_str());
//     input.pop_front();
//     result.vy = atof(input.front().c_str());
//     input.pop_front();
//     result.a = atof(input.front().c_str());
//     input.pop_front();
//     result.jerk = atof(input.front().c_str());
//     input.pop_front();
//     result.current_lanelet_ID = atoi(input.front().c_str());
//     input.pop_front();
//     result.s_of_current_lanelet = atof(input.front().c_str());
//     input.pop_front();
//     result.d_of_current_lanelet = atof(input.front().c_str());
//     input.pop_front();
//     return result;
// }

// Obstacle_info ObstacleInfoReading(std::list<std::string> input) {
//
//     Obstacle_info result;
//     result.Agent_ID = atoi(input.front().c_str());
//     input.pop_front();
//     result.distence = atof(input.front().c_str());
//     input.pop_front();
//     // TODO: str -> bool ?
//     result.yielding = input.front().c_str();
//     input.pop_front();
//     result.point_in = TraPtReading(input);
//     return result;
// }
//
// PlanTra PlanTraReading(const std::string& Log_path) {
//     PlanTra result;
//     std::string str_temp;
//     std::ifstream in;
//     in.open(Log_path + "/PlannerResult.csv", std::ios::in);
//     while (getline(in, str_temp)) {
//         std::list<std::string> Read_TraPt = splitl(str_temp, ",");
//         result.pts.push_back(TraPtReading(Read_TraPt));
//     }
//     return result;
// }

// Obstacle ObstacleReading(const std::string& Log_path) {
//     Obstacle result;
//     std::string str_temp;
//     std::string temp_distance;
//     std::string temp_yielding;
//     std::string temp_tra;
//     std::ifstream in;
//     in.open(Log_path + "/ObstacleInfo.csv", std::ios::in);
//     getline(in, str_temp);
//     while (getline(in, str_temp)) {
//         getline(in, temp_distance);
//         getline(in, temp_yielding);
//         getline(in, temp_tra);
//         std::list<std::string> ReadIn = splitl(temp_tra, ",");
//         ReadIn.push_front(temp_yielding);
//         ReadIn.push_front(temp_distance);
//         ReadIn.push_front(str_temp);
//         result.obstacle.push_back(ObstacleInfoReading(ReadIn));
//     }
//     return result;
// }


// have to specify all including In/ExPrediction
//PredictTra PredictTraReading(const std::string& Log_path) {
//    PredictTra result;
//    OnePreTra pre_result;
//    std::string temp_pts;
//    std::string temp_conflict;
//    std::string temp_probability;
//    std::ifstream in;
//    in.open(Log_path, std::ios::in);
//    while (getline(in, temp_probability)) {
//        getline(in, temp_conflict);
//        getline(in, temp_pts); // get Tra or END
//        while (temp_pts != "END") {
//            //pre_result.pts.push_back(TraPtReading(splitl(temp_pts, ",")));
//            int a =0;
//            pre_result.pts.insert({a,TraPtReading(splitl(temp_pts, ","))});
//            getline(in, temp_pts);
//            a++;
//        }
//        pre_result.Probability = atof(temp_probability.c_str());
//        pre_result.confilictlanesID = ConflictSplit(temp_conflict, ",");
//        result.Trajs.push_back(pre_result);
//    }
//    return result;
//}


//Mode ModeReading(const std::string& Log_path) {
//    Mode result = Mode::init;
//    std::ifstream in;
//    in.open(Log_path + "/Mode" + ".csv", std::ios::in);
//    std::string temp;
//    getline(in, temp);
//    if (temp == "following") {
//        result = Mode::following;
//    } else if (temp == "merging") {
//        result = Mode::merging;
//    } else if (temp == "yielding") {
//        result = Mode::yielding;
//    } else if (temp == "stopping") {
//        result = Mode::stopping;
//    } else if (temp == "vanishing") {
//        result = Mode::vanishing;
//    } else if (temp == "synchronizing") {
//        result = Mode::synchronizing;
//    } else if (temp == "allWayStopping") {
//        result = Mode::allWayStopping;
//    } else if (temp == "linechange") {
//        result = Mode::linechange;
//    } else if (temp == "end") {
//        result = Mode::end;
//    } else if (temp == "init") {
//        result = Mode::init;
//    } else if (temp == "stop2follow") {
//        result = Mode::stop2follow;
//    }
//    return result;
//}
