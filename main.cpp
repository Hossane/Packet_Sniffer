#include <iostream>
#include "PcapLiveDevice.h"
#include "PcapLiveDeviceList.h"
#include <thread>
#include <chrono>

static void onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie) {
	int* packetCount = (int*)cookie;

	(*packetCount)++;
    
       	//pcpp::Packet parsedPacket(packet);
	//std::cout << "Packet received: " << parsedPacket.toString() << std::endl;
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




  bool o = dev->open();


  if (o) {
	  std::cout << "worked\n";
  }else{
	  std::cout << "not work\n";
  }

    std::cout << "Using interface: " << dev->getName() << std::endl;

    int packetCount = 0;
    std::cout << "Starting async capture for 10 seconds..." << std::endl;

    if (!dev->startCapture(onPacketArrives, &packetCount)) {
        std::cerr << "Could not start capture. Exiting." << std::endl;
        dev->close();
        return 1;
    }

    // 
    std::this_thread::sleep_for(std::chrono::seconds(10)); // Sleep for 10 seconds [citation:9]

    dev->stopCapture();
    std::cout << "Capture stopped." << std::endl;


    std::cout << "Captured " << packetCount << " packets." << std::endl;


    dev->close();

    return 0;
}
