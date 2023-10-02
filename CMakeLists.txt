cmake_minimum_required(VERSION 3.25)
project(NetworkProgram C)

set(CMAKE_C_STANDARD 11)

add_executable(NetworkProgram Lab_1/main.c lib/DayOfWeek.h lib/StudentScheduleManagement.h lib/DynamicList.h
        lib/Path.h
        lib/Path.h
        Lab_2/ExampleSource/EchoServer.c
        Lab_2/ExampleSource/EchoClient.c
        Testing/testSocket.c
)

add_executable(EchoClient Lab_2/ExampleSource/EchoClient.c
        Lab_2/Ex2/SendClient.c
        Lab_2/Ex2/SendClient.c
        Lab_2/Ex2/ReceiveServer.c)
add_executable(EchoServer Lab_2/ExampleSource/EchoServer.c)
add_executable(main Lab_1/main.c)
add_executable(SendClient Lab_2/Ex2/SendClient.c
        Lab_2/Ex2/SendClient.c
        Lab_2/Ex2/SendClient.c)
add_executable(ReceiveServer Lab_2/Ex2/ReceiveServer.c
        Lab_2/Ex2/ReceiveServer.c)