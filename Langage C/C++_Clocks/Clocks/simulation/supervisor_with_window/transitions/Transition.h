#ifndef TRANSITION_H
#define TRANSITION_H

#include <cmath>
#include "../features/Clock.h"  // Needed if angles are used in visual updates

// Handles smooth transition between two sets of angles over time
class Transition {
private:
    float currentHourAngle;
    float currentMinuteAngle;
    float targetHourAngle;
    float targetMinuteAngle;
    float startHourAngle;     // Initial hour angle when transition starts
    float startMinuteAngle;   // Initial minute angle
    float duration;           // Duration of the transition (in some time unit)

public:
    Transition();

    // Initializes the transition with current and target values + total duration
    void startTransition(float currentH, float currentM, float targetH, float targetM, float duration);

    // Updates the internal state based on elapsed time
    void update(float);

    // Returns the interpolated hour angle
    float getHourAngle();

    // Returns the interpolated minute angle
    float getMinuteAngle();

    // Checks if transition has finished
    bool isComplete();
};

#endif // TRANSITION_H
