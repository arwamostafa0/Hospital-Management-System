cmake_minimum_required(VERSION 4.0)
project(hospital_version3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(hospital_version3
        Appointment.h
        Doctor.h
        Global.h
        main.cpp
        mainHeader.h
        Patient.h
        ProjectHelp.h)
