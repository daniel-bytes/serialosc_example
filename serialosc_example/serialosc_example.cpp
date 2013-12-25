#include <iostream>
#include "SerialOsc.h"

#ifdef _WIN32
	// additional libs needed for oscpack
	#pragma comment(lib, "ws2_32.lib") 
	#pragma comment(lib, "winmm.lib") 
#endif

//
// Example serial osc data handler.
// 
class MonomeDemo
	: public SerialOsc::Listener
{
public:
	MonomeDemo(SerialOsc *osc)
		: osc(osc)
	{
		osc->start(this);
	}

public:
	virtual void deviceFound(const MonomeDevice * const device)
	{
		std::cout << "Found device " << device->id << " (type " << device->type << ")." << std::endl;
		osc->sendDeviceLedAllCommand(device, false);
	}

	virtual void deviceRemoved(const std::string &id)
	{
		std::cout << "Device " << id << " removed." << std::endl;
	}
	
	virtual void buttonPressMessageReceived(MonomeDevice *device, int x, int y, bool state)
	{
		std::cout << "Button press from " << device->id << " received. Prefix = " << device->prefix << ",  x = " << x << ", y = " << y << ", state = " << state << std::endl;
		osc->sendDeviceLedCommand(device, x, y, state);
	}

private:
	SerialOsc *osc;
};


int main(int argc, const char* argv[])
{
	std::string input;
	SerialOsc osc("test", 13000);
	MonomeDemo device(&osc);

	while (input != "q") {
		std::cout << "type 'q' to quit." << std::endl;
		std::getline(std::cin, input);
	}

	osc.stop();

	return 0;
}

