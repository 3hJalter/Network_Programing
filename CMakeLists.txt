cmake_minimum_required(VERSION 3.25)
project(NetworkProgram C)

set(CMAKE_C_STANDARD 11)

add_executable(NetworkProgram Lab_1/main.c Lab_1/lib/DayOfWeek.h Lab_1/lib/StudentScheduleManagement.h Lab_1/lib/DynamicList.h
        Lab_1/lib/Path.h
)
