//
// Graeme Zinck, 2020
//
// ViconAdapter.cpp for processing and retransmitting Vicon data for
// use in another framework, if desired.
//

#include "DataStreamClient.h"
#include "Server.h"
#include <string.h>
#include <iostream>
#include <chrono>
#include <set>
#include <json/writer.h>
#include <websocketpp/common/thread.hpp>

// For sleep() or Sleep()
#ifdef WIN32
	#include <windows.h> // For Sleep()
#else
	#include <unistd.h> // For nanosleep()
#endif // WIN32

#ifndef MICRO
#define MICRO 1000000 // Number of nanoseconds per second
#endif

#ifndef SERVE_PORT
#define SERVE_PORT 3456
#endif

/** The subjects to retreive from the Vicon datastream */
std::set<std::string> subjects = {"Active Wand (Origin Tracking)"};

/**
 * Checks if the subject is one we want to track.
 *
 * @param value A potential subject's name.
 */
bool isSubject(std::string &value) {
	return subjects.count(value) > 0;
}

using namespace ViconDataStreamSDK::CPP;

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

void getViconStream(std::string host, server::Server* server) {
//	while (true) {
//		*server << "We're just sending lots of messages.";
//	}

	// Connect to Vicon datastream
	std::cout << "Connecting to Vicon at " << host << std::endl;

	ViconDataStreamSDK::CPP::Client client;
	Output_Connect cxResult = client.Connect(host);
	while (cxResult.Result != Result::Success) {
		std::cout << "Vicon connection failed...";
		
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
	
	std::cout << "Vicon connection successful!" << std::endl;

	client.SetStreamMode(StreamMode::ServerPush);
    client.EnableMarkerData();

	while (true) {
		while (client.GetFrame().Result != Result::Success) {
			std::cout << "Waiting for a new frame" << std::endl;
			// Sleep so we don't overload the CPU.
#ifdef WIN32
			Sleep(200);
#else
			usleep(200000);
#endif
		}

		// Hang on to the frame's information
		Json::Value frameInfo;

		// Stop waiting, record the time
		long long frameTime = getFrameTime(client.GetLatencyTotal().Total);
		frameInfo["time"] = frameTime;
		frameInfo["frameno"] = client.GetFrameNumber().FrameNumber;

		// The following is no longer needed because the frameTime is the
		// current time minus the latency.
		// frameInfo["latency"] = client.GetLatencyTotal().Total;

		// Get the subjects we want
		int numSubjects = client.GetSubjectCount().SubjectCount;
		for (int subjectIndex = 0; subjectIndex < numSubjects; subjectIndex++) {
			std::string subjectName = client.GetSubjectName(subjectIndex).SubjectName;
			if (isSubject(subjectName)) {
				// Collect the marker information
				int numMarkers = client.GetMarkerCount(subjectName).MarkerCount;
				for (int markerIndex = 0; markerIndex < numMarkers; markerIndex++) {
					std::string markerName = client.GetMarkerName(subjectName, markerIndex).MarkerName;
					Output_GetMarkerGlobalTranslation translation = client.GetMarkerGlobalTranslation(subjectName, markerName);

					Json::Value coordinates;
					for (unsigned int dim = 0; dim < 3; dim++) {
						coordinates.append(translation.Translation[dim]);
					}
					frameInfo[subjectName][markerName] = coordinates;
				}
			}
		}
		*server << frameInfo;
	}
}

int main(int argc, char* argv[]) {
	// Get the vicon host
	std::string host;
	if (argc > 1) host = argv[1];
	//else host = "localhost:801";
	else host = "192.168.1.105:801";

	// Set up the server to share our data
	int servePort;
	if (argc > 2) servePort = atoi(argv[2]);
	else servePort = SERVE_PORT;

	server::Server serv;

	websocketpp::lib::thread t(websocketpp::lib::bind(&getViconStream, host, &serv));

	serv.run(SERVE_PORT);

	t.join();
	return 0;
}
