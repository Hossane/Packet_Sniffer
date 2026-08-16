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

static void onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie) {
	int* packetCount = (int*)cookie;

	(*packetCount)++;
	int packetLength = packet->getRawDataLen();
	
	std::cout << packet->getLinkLayerType() << std::endl;
		
	std::stringstream ss;
 	ss << std::hex << std::setfill('0');
    
    	for (size_t i = 0; i < packetLength; ++i) {
                ss << std::setw(2) << static_cast<int>((packet->getRawData())[i]);
    	}


	std::cout << ss.str() << "\n\n";
	
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
