#include "packFunc.h"

void getPacketEndpoints(pcpp::Packet &parsedPacket)
{
    std::string srcIP, dstIP;
    uint16_t srcPort = 0, dstPort = 0;
    bool hasIP = false;
    bool hasPort = false;

    pcpp::IPv4Layer *ipv4 = parsedPacket.getLayerOfType<pcpp::IPv4Layer>();
    if (ipv4)
    {
        srcIP = ipv4->getSrcIPAddress().toString();
        dstIP = ipv4->getDstIPAddress().toString();
        hasIP = true;
    }
    else
    {
        pcpp::IPv6Layer *ipv6 = parsedPacket.getLayerOfType<pcpp::IPv6Layer>();
        if (ipv6)
        {
            srcIP = ipv6->getSrcIPAddress().toString();
            dstIP = ipv6->getDstIPAddress().toString();
            hasIP = true;
        }
    }

    pcpp::TcpLayer *tcp = parsedPacket.getLayerOfType<pcpp::TcpLayer>();
    if (tcp)
    {
        srcPort = tcp->getSrcPort();
        dstPort = tcp->getDstPort();
        hasPort = true;
    }
    else
    {
        pcpp::UdpLayer *udp = parsedPacket.getLayerOfType<pcpp::UdpLayer>();
        if (udp)
        {
            srcPort = udp->getSrcPort();
            dstPort = udp->getDstPort();
            hasPort = true;
        }
    }

    if (hasIP && hasPort)
    {
        std::cout << "Source: " << srcIP << "." << srcPort << "|" << "Dest: " << dstIP << "." << dstPort << std::endl;
    }
    else if (hasIP)
    {
        std::cout << "Source IP: " << srcIP << std::endl;
        std::cout << "Dest IP:   " << dstIP << std::endl;
        std::cout << "(No transport layer - ICMP or ARP)" << std::endl;
    }
    else
    {
        std::cout << "Not an IP packet" << std::endl;
    }
}

std::string getProtocolTypeAsString(pcpp::ProtocolType protocolType)
{
    switch (protocolType)
    {
    case pcpp::Ethernet:
        return "Ethernet";
    case pcpp::IPv4:
        return "IPv4";
    case pcpp::TCP:
        return "TCP";
    case pcpp::UDP:
        return "UDP";
    case pcpp::HTTPRequest:
        return "HTTP Request";
    default:
        return "Unknown";
    }
}

void listDisc(const std::vector<pcpp::PcapLiveDevice *> &devList)
{
    for (int i = 0; i < devList.size(); i++)
    {
        std::cout << "[" << i << "] " << devList[i]->getName()
                  << " (" << devList[i]->getDesc() << ")" << std::endl;
    }
}

static void onPacketArrives(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *dev, void *cookie)
{
    int *packetCount = (int *)cookie;

    (*packetCount)++;

    pcpp::Packet parsedPacket(packet);

    getPacketEndpoints(parsedPacket);
}

int sniffPacket(int device, int sec)
{
    if (devList.empty())
    {
        std::cerr << "No network interfaces found!" << std::endl;
        return 1;
    }

    pcpp::PcapLiveDevice *dev = devList[device];

    if (dev->open())
    {
        std::cout << "device" << dev->getName() << "opened successfuly\n";
    }
    else
    {
        std::cout << "\n";
    }

    std::cout << "Using interface: " << dev->getName() << std::endl;

    int packetCount = 0;
    std::cout << "Starting async capture for " << sec << " seconds..." << std::endl;

    if (!dev->startCapture(onPacketArrives, &packetCount))
    {
        std::cerr << "Could not start capture. Exiting." << std::endl;
        dev->close();
        return 1;
    }

    //
    std::this_thread::sleep_for(std::chrono::seconds(sec));

    dev->stopCapture();
    std::cout << "Capture stopped." << std::endl;

    std::cout << "Captured " << packetCount << " packets." << std::endl;

    dev->close();

    return 0;
}

// int main()
// {

//     if (devList.empty())
//     {
//         std::cerr << "No network interfaces found!" << std::endl;
//         return 1;
//     }

//     pcpp::PcapLiveDevice *dev = devList[0];

//     if (dev->open())
//     {
//         std::cout << "device" << dev->getName() << "opened successfuly\n";
//     }
//     else
//     {
//         std::cout << "\n";
//     }

//     std::cout << "Using interface: " << dev->getName() << std::endl;

//     int packetCount = 0;
//     std::cout << "Starting async capture for 20 seconds..." << std::endl;

//     if (!dev->startCapture(onPacketArrives, &packetCount))
//     {
//         std::cerr << "Could not start capture. Exiting." << std::endl;
//         dev->close();
//         return 1;
//     }

//     //
//     std::this_thread::sleep_for(std::chrono::seconds(20)); // Sleep for 10 seconds [citation:9]

//     dev->stopCapture();
//     std::cout << "Capture stopped." << std::endl;

//     std::cout << "Captured " << packetCount << " packets." << std::endl;

//     dev->close();

//     return 0;
// }
