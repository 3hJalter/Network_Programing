cmake_minimum_required(VERSION 3.25)
project(NetworkProgram C)

set(CMAKE_C_STANDARD 11)

add_executable(NetworkProgram Lab_1/main.c lib/DayOfWeek.h lib/StudentScheduleManagement.h lib/DynamicList.h
        lib/Path.h
        Testing/testSocket.c
)

add_executable(EchoClient Lab_2/Ex1/EchoClient.c
        Lab_2/Ex3/Response.h)
add_executable(EchoServer Lab_2/Ex1/EchoServer.c)
add_executable(main Lab_1/main.c)
add_executable(SendClient Lab_2/Ex2/SendClient.c)
add_executable(ReceiveServer Lab_2/Ex2/ReceiveServer.c)
add_executable(UserClient Lab_2/Ex3/UserClient.c
        Lab_2/Ex3/Request.h
        Lab_2/Ex3/Response.h)
add_executable(StudentManagementServer Lab_2/Ex3/StudentManagementServer.c
        Lab_2/Ex3/Request.h
        Lab_2/Ex3/Response.h)