cmake_minimum_required(VERSION 2.8)
project(epoll_project)

SET(CMAKE_CXX_FLAGS "-O3 -std=c++0x -Wall -Wextra -pedantic")

set(SOURCES_LIB client_item.cpp
	command_line.cpp
	epoll_selector.cpp
	file_holder.cpp
	http_file_sender.cpp
	http_request_header.cpp
	http_request_reader.cpp
	http_response.cpp
	http_src_file_holder.cpp
	http_user_session.cpp
	inet_address.cpp
	inet_address_v4.cpp
	io_helper.cpp
	listen_thread.cpp
	logger.cpp
	selector_thread.cpp
	socket.cpp
	socket_holder.cpp
	socket_tuner.cpp
	tcp_server.cpp
	tcp_server_socket.cpp
	thread_loop.cpp
)
set(SOURCES_MAIN main.cpp)

SET( HEADERS client_item.h
	command_line.h
	common_utils.h
	epoll_selector.h
	exceptions.h
	file_holder.h
	http_file_sender.h
	http_request_header.h
	http_request_reader.h
	http_response.h
	http_src_file_holder.h
	http_status_code.h
	http_user_session.h
	iconnection_ctrl.h
	idisposable.h
	inet_address.h
	inet_address_v4.h
	io_helper.h
	iselector.h
	iuser_session.h
	listen_thread.h
	logger.h
	non_copyable.h
	queue.h
	queueBlock.h
	selector_thread.h
	socket.h
	socket_holder.h
	socket_tuner.h
	tcp_server.h
	tcp_server_socket.h
	thread_loop.h
	types.h
)
add_library(epolllib STATIC ${SOURCES_LIB} ${HEADERS})
add_executable(web_server ${SOURCES_MAIN})
file(COPY run.sh DESTINATION ./)

target_link_libraries(web_server epolllib pthread)
