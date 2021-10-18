//
// Created by 萧博成 on 2021/10/17.
//

#include "Predictor.hpp"

Predictor::Predictor(std::shared_ptr<AgentLog> agent_ibt, LogManager* logManager, double time_step, double horizon){
    agent_ibt_ = agent_ibt;
    time_step_ = time_step;
    horizon_ = horizon;
    state = PredictorState::fine;
}

PredictorState Predictor::get_state(){
    return state;
}

void Predictor::set_state(PredictorState s){
    state = s;
}
