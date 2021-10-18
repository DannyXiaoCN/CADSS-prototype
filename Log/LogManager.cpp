//
// Created by 萧博成 on 2021/5/6.
//

#include "LogManager.hpp"


LogResponse LogManager::Responser(LogRequest request) {

    int object_agentid = request.getAgent_id();
    auto iter = AcutalManager.find(object_agentid);
    if (iter != AcutalManager.end()) {
        LogResponse logResponse(iter->second);
        return logResponse;
    } else {
        // return ERROR pointer
        // Showed in AgentLog 7777
        return LogResponse(AcutalManager.find(7777)->second);
    }
}

//std::map<int, std::shared_ptr<AgentLog>> AcutalManager;
void LogManager::AddAgentLog(int Agentid, std::shared_ptr<AgentLog> add_agentLog) {
    AcutalManager.insert(std::map<int, std::shared_ptr<AgentLog>>::value_type(Agentid, add_agentLog));
}

bool LogManager::Ready2Update(int CurrentClock) {

    std::cout << "debug: CurrentClock: " << CurrentClock << std::endl;
    auto iter = AcutalManager.begin();
    bool flag = true;
    while (iter!=AcutalManager.end())
    {
        std::shared_ptr<LogContent> lognow = iter->second->GetLog(CurrentClock);
        std::cout << "debug: AgentID: " << iter->first << std::endl;
        if (lognow->next_state_.empty()) {
            flag=false;
            return flag;
        }
        iter++;
    }
    iter = AcutalManager.begin();

    while (iter!= AcutalManager.end())
    {
        std::cout << "debug: Log check 1" << std::endl;
        std::shared_ptr<LogContent> lognow = iter->second->GetLog(CurrentClock);
        iter->second->setPreState(lognow->current_state_);
        std::vector<double> state(lognow->next_state_);

        LogContent LCTemp;
        LCTemp.current_state_ = state;
        iter->second->PushLog(std::make_shared<LogContent>(LCTemp), CurrentClock + 1);
        iter->second->GetLog(CurrentClock + 1)->PlannerResult.pts.empty();
        std::cout << "debug: Log push CurrentClock: " << CurrentClock << std::endl;
        iter++;
    }
    return flag;
}

void LogManager::setEA_STS(int StartTS) {
    EA_StartTimeStamp = StartTS;
}
