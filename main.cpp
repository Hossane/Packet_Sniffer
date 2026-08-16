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

void getPacketEndpoints(pcpp::Packet& parsedPacket) {
    std::string srcIP, dstIP;
    uint16_t srcPort = 0, dstPort = 0;
    bool hasIP = false;
    bool hasPort = false;

    
    pcpp::IPv4Layer* ipv4 = parsedPacket.getLayerOfType<pcpp::IPv4Layer>();
    if (ipv4) {
        srcIP = ipv4->getSrcIPAddress().toString();
        dstIP = ipv4->getDstIPAddress().toString();
        hasIP = true;
    } else {
        pcpp::IPv6Layer* ipv6 = parsedPacket.getLayerOfType<pcpp::IPv6Layer>();
        if (ipv6) {
            srcIP = ipv6->getSrcIPAddress().toString();
            dstIP = ipv6->getDstIPAddress().toString();
            hasIP = true;
        }
    }

    pcpp::TcpLayer* tcp = parsedPacket.getLayerOfType<pcpp::TcpLayer>();
    if (tcp) {
        srcPort = tcp->getSrcPort();
        dstPort = tcp->getDstPort();
        hasPort = true;
    } else {
        pcpp::UdpLayer* udp = parsedPacket.getLayerOfType<pcpp::UdpLayer>();
        if (udp) {
            srcPort = udp->getSrcPort();
            dstPort = udp->getDstPort();
            hasPort = true;
        }
    }

    if (hasIP && hasPort) {
        std::cout << "Source: " << srcIP << "." << srcPort << "|" << "Dest: " << dstIP << "." << dstPort << std::endl;
    } else if (hasIP) {
        std::cout << "Source IP: " << srcIP << std::endl;
        std::cout << "Dest IP:   " << dstIP << std::endl;
        std::cout << "(No transport layer - ICMP or ARP)" << std::endl;
    } else {
        std::cout << "Not an IP packet" << std::endl;
    }
}

std::string getProtocolTypeAsString(pcpp::ProtocolType protocolType) {
	switch (protocolType) {
        case pcpp::Ethernet:   return "Ethernet";
        case pcpp::IPv4:       return "IPv4";
        case pcpp::TCP:        return "TCP";
        case pcpp::UDP:        return "UDP";
        case pcpp::HTTPRequest:return "HTTP Request";
        default:               return "Unknown";
    } 
}

static void onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie) {
	int* packetCount = (int*)cookie;

	(*packetCount)++;

	pcpp::Packet parsedPacket(packet);


	getPacketEndpoints(parsedPacket);
	//pcpp::Layer* layer = parsedPacket.getFirstLayer();
	//int layerNum = 1;

	//while (layer != NULL) {
	//	std::cout << "Layer " << layerNum << ": " << getProtocolTypeAsString(layer->getProtocol()) << std::endl;

        //	layer = layer->getNextLayer();
        //	layerNum++;
	//}


//	int packetLength = packet->getRawDataLen();
	
//	std::cout << packet->getLinkLayerType() << std::endl;
		
//	std::stringstream ss;
// 	ss << std::hex << std::setfill('0');
    
//    	for (size_t i = 0; i < packetLength; ++i) {
//                ss << std::setw(2) << static_cast<int>((packet->getRawData())[i]);
//    	}


//	std::cout << ss.str() << "\n\n";
	
}

int main() {
  const std::vector<pcpp::PcapLiveDevice*>& devList = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();
  
  if (devList.empty()) {
	  std::cerr << "No network interfaces found!" << std::endl;
	  return 1;
  }
  
  pcpp::PcapLiveDevice* dev = devList[0];
   
//  std::string interfaceName = dev->getName();  
//  std::cout << "Successfully found network interface: " << interfaceName << std::endl;





  if (dev->open()) {
	  std::cout << "device" << dev->getName() << "opened successfuly\n";
  }else{
	  std::cout << "\n";
  }

  for (int i =0;i< devList.size();i++) {
	  std::cout << devList[i]->getName() << "\n";
  }






    std::cout << "Using interface: " << dev->getName() << std::endl;

    int packetCount = 0;
    std::cout << "Starting async capture for 20 seconds..." << std::endl;

    if (!dev->startCapture(onPacketArrives, &packetCount)) {
        std::cerr << "Could not start capture. Exiting." << std::endl;
        dev->close();
        return 1;
    }

    // 
    std::this_thread::sleep_for(std::chrono::seconds(20)); // Sleep for 10 seconds [citation:9]

    dev->stopCapture();
    std::cout << "Capture stopped." << std::endl;


    std::cout << "Captured " << packetCount << " packets." << std::endl;


    dev->close();

    return 0;
}
