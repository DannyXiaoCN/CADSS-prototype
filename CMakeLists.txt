set(CMAKE_MACOSX_RPATH 1)
cmake_minimum_required(VERSION 3.10)
project(CADSS_prototype)

SET(CMAKE_CXX_FLAGS -pthread)
SET(CMAKE_EXE_LINKER_FLAGS " -no-pie")
set(CMAKE_CXX_STANDARD 14)

find_package(Threads REQUIRED)


include_directories(Behaviours)
include_directories(Calculation_core)
include_directories(Log)
include_directories(Maps)
include_directories(Simulator)
include_directories(ThreadPool)
include_directories(../../simulator2021/Lanelet2Lib)
include_directories(PlannerLib)
include_directories(
        ${LANELET2_INCLS_DIRS}
)
file(GLOB_RECURSE ALGLIB_FILES "alglib/*")


add_executable(
        CADSS_prototype
        main.cpp

        ThreadPool/ThreadController.cpp
        ThreadPool/ThreadController.hpp
        ThreadPool/MyThreadPool.cpp
        ThreadPool/MyThreadPool.hpp
        ThreadPool/ThreadTask.cpp
        ThreadPool/ThreadTask.hpp

        Simulator/SimuClock.cpp
        Simulator/SimuClock.hpp
        Simulator/Simulator.cpp
        Simulator/Simulator.hpp

        Predictors/NoPredictor.cpp
        Predictors/NoPredictor.hpp
        Predictors/Predictor.cpp
        Predictors/Predictor.hpp

        Log/AgentLog.cpp
        Log/AgentLog.hpp
        Log/LogContent.hpp
        Log/LogManager.cpp
        Log/LogManager.hpp
        Log/LogRequest.cpp
        Log/LogRequest.hpp
        Log/LogResponse.cpp
        Log/LogResponse.hpp

        Calculation_core/CalTask.cpp
        Calculation_core/CalTask.hpp
        Calculation_core/TaskManager.cpp
        Calculation_core/TaskManager.hpp
        Calculation_core/CalPlanner.cpp
        Calculation_core/CalPlanner.hpp
        Calculation_core/CalController.cpp
        Calculation_core/CalController.hpp

        Controllers/PerfectController.cpp
        Controllers/PerfectController.hpp

        Model/PerfectModel.cpp
        Model/PerfectModel.hpp

        Planners/AstarPlanner.hpp
        Planners/AstarPlanner.cpp
        Planners/Planner.hpp
        Planners/Planner.cpp

        Behaviours/ReplayGenerator.cpp
        Behaviours/ReplayGenerator.hpp

        Maps/MapInfo.cpp
        Maps/MapInfo.hpp
        Maps/LaneletMapReader.cpp
        Maps/LaneletMapReader.hpp
        Maps/TrafficInfo.cpp
        Maps/TrafficInfo.hpp

        Calculation_core/CalPredictor.cpp
        Calculation_core/CalPredictor.hpp

        Calculation_core/CalPredictor.cpp
        Calculation_core/CalPredictor.hpp
        Calculation_core/CalPredictor.cpp
        Calculation_core/CalPredictor.hpp
)
