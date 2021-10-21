cmake_minimum_required(VERSION 3.0.0)
project(Anomaly_Detector VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(Anomaly_Detector MainTrain.cpp anomaly_detection_util.cpp anomaly_detection_util.h SimpleAnomalyDetector.cpp
        SimpleAnomalyDetector.h timeseries.cpp timeseries.h AnomalyDetector.h)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
