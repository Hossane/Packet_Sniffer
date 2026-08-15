cmake_minimum_required(VERSION 3.16)
project(PacketSnifferProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PcapPlusPlus REQUIRED)

# Target name is "packet_sniffer", source file is "main.cpp"
add_executable(packet_sniffer main.cpp)
target_link_libraries(packet_sniffer PRIVATE PcapPlusPlus::Pcap++)

