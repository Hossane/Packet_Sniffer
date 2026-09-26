#include "packFunc.h"

#include <fstream>
#include <string>
#include <stdexcept>

const std::vector<pcpp::PcapLiveDevice *> &devList = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();

void printUsage(const char *prog)
{
  std::cerr << "Usage: " << prog << " [options]\n"
            << "  -d          Display all devices\n"
            << "  -c          Choose a device to listen on\n"
            << "  -t          Time in seconds to listen\n"
            << "  -h          Display instruction\n";
}

int main(int argc, char *argv[])
{
  int deviceNum = -1;
  int time = 0;

  if (argc < 2)
  {
    printUsage(argv[0]);
    return 1;
  }

  for (int i = 1; i < argc; ++i)
  {
    std::string arg = argv[i];

    if (arg == "-d")
    {
      if (i + 1 < argc)
      {
        std::cerr << "Error: Invalid Syntax\n";
        return 1;
      }
      listDisc(devList);
      return 0;
    }
    else if (arg == "-c")
    {
      if (i + 1 >= argc)
      {
        std::cerr << "Error: Invalid Syntax\n";
        std::cerr << "Require number after -c\n";
        return 1;
      }

      try
      {
        deviceNum = std::stoi(argv[++i]);
        if (deviceNum < 0 || deviceNum > devList.size())
        {
          throw std::runtime_error("Value not in range");
        }
      }
      catch (const std::exception &e)
      {
        std::cerr << e.what() << "\n";
        std::cerr << "Invalid number for -c\n";
        return 1;
      }
    }
    else if (arg == "-t")
    {
      if (i + 1 >= argc)
      {
        std::cerr << "Error: Invalid Syntax\n";
        std::cerr << "Require number after -t\n";
        return 1;
      }

      try
      {
        time = std::stoi(argv[++i]);
      }
      catch (const std::exception &e)
      {
        std::cerr << e.what() << "\n";
        return 1;
      }
    }
    else if (arg == "-h" || arg == "--help")
    {
      if (i + 1 < argc)
      {
        std::cerr << "Error: Invalid Syntax\n";
        return 1;
      }
      printUsage(argv[0]);
      return 0;
    }
    else
    {
      std::cerr << "Unknown option: " << arg << "\n";
      printUsage(argv[0]);
      return 1;
    }
  }

  if (deviceNum != -1)
  {
    if (time != 0)
    {
      sniffPacket(deviceNum, time);
    }
    else
    {
      sniffPacket(deviceNum);
    }
  }

  return 0;
}

/*std::string filename = argv[++i]; // consume next arg

      std::ofstream file(filename);
      if (!file)
      {
        std::cerr << "Error: cannot create '" << filename << "'\n";
        return 1;
      }
      file << "Created by " << argv[0] << "\n";
      std::cout << "Created: " << filename << "\n";*/