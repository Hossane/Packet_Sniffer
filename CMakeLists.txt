cmake_minimum_required(VERSION 3.16)
project(PacketSniffer LANGUAGES CXX)

# Require C++11 or higher (PcapPlusPlus works great with C++11/14/17/20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH 
    "C:/PcapPlusPlus"
    "C:/Npcap-SDK"
)

# Find PcapPlusPlus (it will automatically look for wpcap.lib via its config)
find_package(PcapPlusPlus REQUIRED)

add_library(packfunc_headers INTERFACE)
target_include_directories(packfunc_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(packfunc_headers INTERFACE
    PcapPlusPlus::Pcap++
    PcapPlusPlus::Packet++
    PcapPlusPlus::Common++
)

# add_executable(packet_sniffer packFunc.cpp)
# target_link_libraries(packet_sniffer PRIVATE packfunc_headers)

add_executable(test_flags flags.cpp packFunc.cpp)
target_link_libraries(test_flags PRIVATE packfunc_headers)