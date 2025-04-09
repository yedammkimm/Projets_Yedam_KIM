#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <ctime>
#include <cstring>
#include <arpa/inet.h>
#include <netinet/in.h>

// Default target clock position
const int targetRow = 2;
const int targetCol = 7;

using namespace std;

// Structure representing the rotation angles of a clock
struct ClockMotion {
    float hourAngle;     // Angle for the hour hand
    float minuteAngle;   // Angle for the minute hand
};

// Sends the given ClockMotion data to the receptor over TCP
void sendClockMotionToReceptor(const ClockMotion& motion);

// Serializes a ClockMotion object into a JSON-like string
std::string serializeClockMotion(const ClockMotion& motion);

// Parses a JSON-like string to extract hour and minute angles
ClockMotion parseClockMotion(const std::string& json);

#endif // COMMUNICATION_H
