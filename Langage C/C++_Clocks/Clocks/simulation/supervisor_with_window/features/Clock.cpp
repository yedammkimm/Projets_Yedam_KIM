#include "Clock.h"
#include <math.h>

Clock::Clock(float x, float y) : hand_1(45, sf::Color::Black), hand_2(45, sf::Color::Black) {
    dial.setRadius(58.8); 
    dial.setFillColor(sf::Color::White);  
    dial.setOutlineThickness(2);  
    dial.setOutlineColor(sf::Color::Black); 
    dial.setOrigin(60, 60);                // Center origin for proper rotation
    dial.setPosition(x, y);                // Position on screen
}

// Calculates smooth interpolation between two angles, using shortest arc
float interpolateAngle(float start, float end, float t) {
    float delta = fmod((end - start + 540.0f), 360.0f) - 180.0f;
    return start + delta * t;
}

void Clock::update(float targetAngle1, float targetAngle2, sf::RenderWindow& window) {
    float startAngle1 = hand_1.getAngle();
    float startAngle2 = hand_2.getAngle();


    // Same smart angle swap as in update()
    float originalCost = std::abs(targetAngle1 - startAngle1) + std::abs(targetAngle2 - startAngle2);
    float swappedCost = std::abs(targetAngle2 - startAngle1) + std::abs(targetAngle1 - startAngle2);

    if (swappedCost < originalCost) {
        std::swap(targetAngle1, targetAngle2);
    }

    float diff1 = angularDistance(targetAngle1, startAngle1);
    float diff2 = angularDistance(targetAngle2, startAngle2);
    float maxDiff = std::max(diff1, diff2);
    int steps = std::max(1, static_cast<int>(maxDiff * (MAXSTEP-1.5)));

    for (int i = 0; i <= steps; ++i) {
        float t = static_cast<float>(i) / steps;

        float interpolated1 = interpolateAngle(startAngle1, targetAngle1, t);
        float interpolated2 = interpolateAngle(startAngle2, targetAngle2, t);

        this->eraseHands(window, sf::Color::White);
        hand_1.setAngle(interpolated1);
        hand_2.setAngle(interpolated2);
        this->drawHands(window);
        window.display();

        // Prevent window freezing during animation
        sf::Clock delayTimer;
        while (delayTimer.getElapsedTime().asMilliseconds() < 8) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
        }
    }
}

// Lambda to wrap angles between 0 and 359
float normalize (float angle) {
    while (angle < 0) angle += 360;
    while (angle >= 360) angle -= 360;
    return angle;
}

float angularDistance(float a, float b) {
    float diff = std::fmod(std::abs(a - b), 360.f);
    return (diff > 180.f) ? 360.f - diff : diff;
}

void Clock::update_with_send(float targetAngle1, float targetAngle2, sf::RenderWindow& window) {
    float startAngle1 = hand_1.getAngle();
    float startAngle2 = hand_2.getAngle();

    // Same smart angle swap as in update()
    float originalCost = std::abs(targetAngle1 - startAngle1) + std::abs(targetAngle2 - startAngle2);
    float swappedCost = std::abs(targetAngle2 - startAngle1) + std::abs(targetAngle1 - startAngle2);

    if (swappedCost < originalCost) {
        std::swap(targetAngle1, targetAngle2);
    }

    float diff1 = angularDistance(targetAngle1, startAngle1);
    float diff2 = angularDistance(targetAngle2, startAngle2);
    float maxDiff = std::max(diff1, diff2);
    int steps = std::max(1, static_cast<int>(maxDiff * (MAXSTEP-1.5)));
    
    for (int i = 0; i <= steps; ++i) {
        float t = static_cast<float>(i) / steps;

        float interpolated1 = interpolateAngle(startAngle1, targetAngle1, t);
        float interpolated2 = interpolateAngle(startAngle2, targetAngle2, t);

        // Send current interpolated angles to receptor
        ClockMotion motion;
        motion.hourAngle = normalize(interpolated1);
        motion.minuteAngle = normalize(interpolated2);
        sendClockMotionToReceptor(motion);

        // Animate hand movement
        this->eraseHands(window, sf::Color::White);
        hand_1.setAngle(interpolated1);
        hand_2.setAngle(interpolated2);
        this->drawHands(window);
        window.display();

        // Keep UI responsive during animation
        sf::Clock delayTimer;
        while (delayTimer.getElapsedTime().asMilliseconds() < 8) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
        }
    }
}

void Clock::eraseHands(sf::RenderWindow& window, sf::Color backgroundColor) const {
    sf::Vector2f pos = dial.getPosition();
    hand_1.erase(window, pos, backgroundColor);
    hand_2.erase(window, pos, backgroundColor);
}

void Clock::draw(sf::RenderWindow& window) const {
    window.draw(dial);
    hand_1.draw(window, dial.getPosition());
    hand_2.draw(window, dial.getPosition());
}

void Clock::drawHands(sf::RenderWindow& window) const {
    sf::Vector2f pos = dial.getPosition();
    hand_1.draw(window, pos);
    hand_2.draw(window, pos);
}

void Clock::setInstant(float angle1, float angle2) {
    hand_1.setAngle(angle1);
    hand_2.setAngle(angle2);
}
