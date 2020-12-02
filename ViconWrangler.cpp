//
// Graeme Zinck, 2020
//
// ViconAdapter.cpp for processing and retransmitting Vicon data for
// use in another framework, if desired.
//

#include "DataStreamClient.h"
#include <string.h>
#include <iostream>
#include <chrono>
#include <ctime>

#ifndef MICRO
#define MICRO 1000000 // Number of nanoseconds per second
#endif

// For sleep() or Sleep()
#ifdef WIN32
	#include <windows.h> // For Sleep()
#else
	#include <unistd.h> // For nanosleep()
#endif // WIN32

using namespace ViconDataStreamSDK::CPP;

// /**
//  * Tranform a string representing some fraction of seconds into
//  * a long representing an integer number of microseconds.
//  *
//  * @param str The fraction number of seconds to convert, formatted
//  * similar to "0.00704355s".
//  * @return The integer number of microseconds represented (like
//  * 7043550).
//  */
// long long strToMicrosecs(std::string str) {
// 	str = str.substr(0, str.length() - 1); // remove the "s" at the end
// 	int decIndex = str.find('.');
// 	str.erase(decIndex, 1);
// 	while (str.length() - decIndex < MICROSECS) {
// 		str.push_back('0');
// 	}
// 	if (str.length() - decIndex > MICROSECS) {
// 		str.erase(decIndex + MICROSECS);
// 	}
// 	return std::stol(str);
// }

/**
 * Gets the time in time_since_epoch (in microseconds), minus
 * the latency provided in seconds.
 *
 * @param str The latency in seconds.
 * @return The integer number of microseconds since epoch.
 */
long long getFrameTime(double latencySecs) {
	std::chrono::time_point<std::chrono::system_clock> currTime = std::chrono::system_clock::now();
	return currTime.time_since_epoch().count() - (long long) (latencySecs * MICRO);
}

int main(int argc, char* argv[]) {
	std::string host;
	if (argc > 1) host = argv[1];
	else host = "localhost:801";
	std::cout << "Connecting to " << host << std::endl;

	ViconDataStreamSDK::CPP::Client client;
	Output_Connect cxResult = client.Connect(host);
	while (cxResult.Result != Result::Success) {
		std::cout << "Connection failed...";
		
		switch (cxResult.Result) {
			case Result::InvalidHostName:
				std::cout << "Invalid host name" << std::endl;
				break;
			case Result::ClientAlreadyConnected:
				std::cout << "Client already connected" << std::endl;
				break;
			case Result::ClientConnectionFailed:
				std::cout << "Client connection failed" << std::endl;
				break;
			default:
				std::cout << "Unknown error" << std::endl;
		}

		// Sleep before continuing
#ifdef WIN32
		Sleep(1000);
#else
		usleep(1000000);
#endif
		cxResult = client.Connect(host);
	}
	
	std::cout << "Connection successful!" << std::endl;

	client.SetStreamMode(StreamMode::ServerPush);

	while (true) {
		std::cout << "Waiting for a new frame" << std::endl;
		while (client.GetFrame().Result != Result::Success) {
			// Sleep so we don't overload the CPU.
#ifdef WIN32
			Sleep(200);
#else
			usleep(200000);
#endif
			std::cout << ".";
		}

		// Stop waiting, record the time
		long long frameTime = getFrameTime(client.GetLatencyTotal().Total);
		std::cout << std::endl;

		std::cout << "Frame number: " << client.GetFrameNumber().FrameNumber << std::endl;
		std::cout << "Latency: " << client.GetLatencyTotal().Total << "s" << std::endl;
		std::cout << "Frame rate: " << client.GetFrameRate().FrameRateHz << std::endl;
		std::cout << "Frame time: " << frameTime << std::endl;
	}
}
