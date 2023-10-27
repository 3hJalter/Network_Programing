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
add_executable(UDPClient Lab_3/UDPClient.c)
add_executable(UDPServer Lab_3/UDPServer.c)
add_executable(testSocket Testing/testSocket.c)
add_executable(UDPChatServer Lab_3/UDPChatServer.c)
add_executable(conEchoServer Lab_4/Example/conEchoServer.c)
add_executable(conEchoClient Lab_4/Example/conEchoClient.c)
add_executable(ssm_client Lab_4/Ex/ssm_client.c)
add_executable(ssm_server Lab_4/Ex/ssm_server.c)
add_executable(server Lab_5/IO_Multiplexing/io_server.c)
add_executable(client Lab_5/IO_Multiplexing/io_client.c)
add_executable(io_server_mul_cli Lab_5/IO_Multiplexing/io_server_mul_cli.c)
add_executable(io_client_signal Lab_5/IO_Multiplexing/io_client_signal.c)