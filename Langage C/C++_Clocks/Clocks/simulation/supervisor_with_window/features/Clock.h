#ifndef CLOCK_H
#define CLOCK_H

#include "Hand.h"
#include "../../receptor/communication.h"

#define MAXSTEP 2 // Max number of steps for smooth motion interpolation for motors **Change the steps which depends on your motors**

// Represents a graphical clock with two moving hands
class Clock {
private:
    sf::CircleShape dial;      // Clock face
    Hand hand_1, hand_2;       // Clock hands (e.g., hour and minute)

public:
    // Constructor: set clock at position (x, y)
    Clock(float x, float y);

    // Animate clock hands toward target angles
    void update(float targetAngle1, float targetAngle2, sf::RenderWindow& window);

    // Same as update(), but also sends motion info (e.g., to motor or log)
    void update_with_send(float targetAngle1, float targetAngle2, sf::RenderWindow& window);

    // Erase hands by redrawing background color (used for transitions)
    void eraseHands(sf::RenderWindow& window, sf::Color backgroundColor) const;

    // Draw the full clock (face and hands)
    void draw(sf::RenderWindow& window) const;

    // Draw only the hands (no dial)
    void drawHands(sf::RenderWindow& window) const;

    // Instantly set hand positions without animation
    void setInstant(float angle1, float angle2);
};

float interpolateAngle(float start, float end, float t);

float angularDistance(float a, float b);

float normalize (float angle);

#endif // CLOCK_H
