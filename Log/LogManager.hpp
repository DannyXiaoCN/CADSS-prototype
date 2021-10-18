//
// Created by 萧博成 on 2021/10/17.
//
#include <map>
#include <vector>
#include "AgentLog.hpp"
#include "LogRequest.hpp"
#include "LogResponse.hpp"
#ifndef CADSS_PROTOTYPR_LOGMANAGER_HPP
#define CADSS_PROTOTYPR_LOGMANAGER_HPP


class LogManager {

public:

    LogResponse Responser(LogRequest request);
    void AddAgentLog(int Agentid, std::shared_ptr<AgentLog> add_agentLog);
    bool Ready2Update(int CurrentClock);
    void setEA_STS(int StartTS);
    int getEA_STS();


private:

    // 指针
    std::map<int, std::shared_ptr<AgentLog>> AcutalManager;
    int EA_StartTimeStamp;

};


#endif //CADSS_PROTOTYPR_LOGMANAGER_HPP
