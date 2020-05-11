cmake_minimum_required(VERSION 3.5.1)
project(cppio)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS	"${CMAKE_CXX_FLAGS} -Wall -O3 -mtune=native -march=native")

option(WITH_BLUETOOTH "build with bluetooth support" OFF)

set(SOURCES             lib/ServerSocket.hpp
                        lib/ServerSocket.cpp
                        lib/Socket.hpp
                        lib/Socket.cpp
                        lib/IODevice.hpp
                        lib/IODevice.cpp
                        lib/File.hpp
                        lib/File.cpp
                        lib/I2CDev.hpp
                        lib/I2CDev.cpp 
			lib/Serial.hpp 
			lib/Serial.cpp)

if (WITH_BLUETOOTH)
	set(SOURCES	${SOURCES}
			lib/BluetoothSocket.hpp 
			lib/BluetoothSocket.cpp 
			lib/BluetoothServerSocket.hpp 
			lib/BluetoothServerSocket.cpp)
endif()

add_library(cppio SHARED ${SOURCES})
target_include_directories(cppio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/)
set_target_properties(cppio PROPERTIES PUBLIC_HEADER "lib/ServerSocket.hpp;lib/Socket.hpp;lib/IODevice.hpp;lib/File.hpp;lib/I2CDev.hpp;lib/Serial.hpp;lib/IODevice.hpp")

if (WITH_BLUETOOTH)
	target_link_libraries(cppio PUBLIC bluetooth)
endif()

install(TARGETS cppio DESTINATION /usr/lib PUBLIC_HEADER DESTINATION /usr/include)
