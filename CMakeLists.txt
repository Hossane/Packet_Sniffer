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

# 2. Define your executable target
add_executable(packet_sniffer main.cpp)

# 3. Link PcapPlusPlus targets to your executable
# This automatically handles include paths, library files, and transitive links (like libpcap)
target_link_libraries(packet_sniffer PRIVATE 
    PcapPlusPlus::Pcap++ 
    PcapPlusPlus::Packet++ 
    PcapPlusPlus::Common++
)
