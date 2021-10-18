//
// Created by 萧博成 on 2021/5/6.
//

#include "LogRequest.hpp"

int LogRequest::getAgent_id() const {
    return Agent_id;
}

LogRequest::LogRequest(int agentId) {

    Agent_id = agentId;

}
