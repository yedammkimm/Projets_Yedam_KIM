#include "Transition.h"


Transition::Transition()
    : currentHourAngle(0), currentMinuteAngle(0),
      targetHourAngle(0), targetMinuteAngle(0),
      duration(5.0f) {}

void Transition::startTransition(float currentH, float currentM, float targetH, float targetM, float transitionDuration) {
    startHourAngle = currentH;     // Save start angles for interpolation
    startMinuteAngle = currentM;
    currentHourAngle = currentH;
    currentMinuteAngle = currentM;
    targetHourAngle = targetH;
    targetMinuteAngle = targetM;
    duration = transitionDuration;
}

// elapsedTime: how much time has passed since the transition started
void Transition::update(float elapsedTime) {
    float t = elapsedTime / duration;     // Normalize time (0.0 to 1.0)
    if (t > 1.0f) t = 1.0f;

    // Linear interpolation toward target angles
    currentHourAngle = (1 - t) * startHourAngle + t * targetHourAngle;
    currentMinuteAngle = (1 - t) * startMinuteAngle + t * targetMinuteAngle;
}

bool Transition::isComplete() {
    // Check if both hands are close enough to their targets
    return std::abs(currentHourAngle - targetHourAngle) < 0.1f &&
           std::abs(currentMinuteAngle - targetMinuteAngle) < 0.1f;
}

float Transition::getHourAngle() { return currentHourAngle; }
float Transition::getMinuteAngle() { return currentMinuteAngle; }
