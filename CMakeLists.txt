cmake_minimum_required(VERSION 3.22)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_DISABLE_PRECOMPILE_HEADERS ON)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

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
	worker_thread.cpp
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
	worker_thread.h
)

add_library(epolllib STATIC ${SOURCES_LIB} ${HEADERS})

add_executable(web_server ${SOURCES_MAIN})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT web_server)

file(COPY run.sh DESTINATION ./)

target_link_libraries(web_server epolllib pthread)
