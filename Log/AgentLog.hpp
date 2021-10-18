//
// Created by 萧博成 on 2021/10/17.
//

#ifndef CADSS_PROTOTYPR_AGENTLOG_HPP
#define CADSS_PROTOTYPR_AGENTLOG_HPP


typedef std::vector<double> Vector;
class MapInfo;


// ///
/// This enumerator stands for the type of an agent.
enum class AgentType {
    HumanCar = 0,
    VirtualCar = 1,
    RealCar = 2,
    ReplayCar = 3,
    NewVirtualCar,
    BehaveCar
};

class AgentLog {

    enum ImportMode {
        // TODO: Need to be extended
        ReplyCarGenerate,
        Others
    };

public:
    // will be changed into Agent Class afterwards
    // Basic functions
    AgentLog();
    void Initialize(int AgentID);
    void SetPath(std::string Current_path);
    std::map<int, std::shared_ptr<LogContent>> CheckLog(int start_timestamp, int end_timestamp);
    std::shared_ptr<LogContent> GetLog(int timestamp);
    void PushLog(std::shared_ptr<LogContent> lc, int cur_timpstamp);
    void UpdateLog(std::shared_ptr<LogContent> lc, int cur_timpstamp);
    void CSV2Log(const std::string& CSV_path, ImportMode mode, int StartTimeStamp, int EndTimeStamp);
    LogContent Log2Update();
    void setPreState(Vector state);
    std::vector<Vector>  get_preState();
    int Log2CSV();
    int isEgoCar();
    void setEgoCar();
    int isExPredictor();
    void setExPredictor();
    int getAgentID();
    void SetMapinfo(MapInfo* m);

private:

    // Basic attributes
    //int AgentID;
    MapInfo* mapinfo;
    int AgentID;
    LogContent AgentUpdate;
    std::map<int, std::shared_ptr<LogContent>> agentLog;
    int EgoCar = 0;
    int ExPredictor = 0;
    std::string system_path = "../";
    int dimState = 6; /*!< the dimension of the agent state */
    std::vector<Vector> preState;

};


#endif //CADSS_PROTOTYPR_AGENTLOG_HPP
