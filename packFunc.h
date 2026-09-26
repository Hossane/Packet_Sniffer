#ifndef PACKFUNC_H
#define PACKFUNC_H

#include <iostream>
#include "PcapLiveDevice.h"
#include "PcapLiveDeviceList.h"
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <Packet.h>
#include <IPv4Layer.h>
#include <IPv6Layer.h>
#include <TcpLayer.h>
#include <UdpLayer.h>

extern const std::vector<pcpp::PcapLiveDevice *> &devList;

void getPacketEndpoints(pcpp::Packet &parsedPacket);
std::string getProtocolTypeAsString(pcpp::ProtocolType protocolType);
void listDisc(const std::vector<pcpp::PcapLiveDevice *> &devList);
int sniffPacket(int device, int sec = 10);
static void onPacketArrives(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *dev, void *cookie);

#endif