#include "communication.h"


std::string serializeClockMotion(const ClockMotion& motion) {
    std::ostringstream oss;
    oss << "{"
        << "\"hourAngle\":" << motion.hourAngle << ","
        << "\"minuteAngle\":" << motion.minuteAngle
        << "}";
    return oss.str();
}

void sendClockMotionToReceptor(const ClockMotion& motion) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "[COMM] Failed to create socket\n";
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5000);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "[COMM] Failed to connect to receptor\n";
        close(sock);
        return;
    }

    std::string payload = serializeClockMotion(motion);
    send(sock, payload.c_str(), payload.size(), 0);
    close(sock);

    std::cout << "[COMM] Sent to receptor: " << payload << std::endl;
}

ClockMotion parseClockMotion(const std::string& json) {
    ClockMotion motion{};

    size_t hPos = json.find("\"hourAngle\":");
    size_t mPos = json.find("\"minuteAngle\":");

    if (hPos != std::string::npos && mPos != std::string::npos) {
        motion.hourAngle = std::stof(json.substr(hPos + 12, json.find(",", hPos) - (hPos + 12)));
        motion.minuteAngle = std::stof(json.substr(mPos + 14, json.find("}", mPos) - (mPos + 14)));
    }

    return motion;
}
