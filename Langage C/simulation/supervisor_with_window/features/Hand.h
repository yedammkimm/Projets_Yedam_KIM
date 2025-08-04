#ifndef HAND_H
#define HAND_H

#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>

// Represents a single hand (hour or minute) of a clock
class Hand {
private:
    mutable sf::RectangleShape shape;  // Graphical shape of the hand (mutable for const draw/erase)
    float angle;                       // Current angle (in degrees)
    sf::Color color;                   // Hand color
    bool is_visible;                   // Flag to control visibility

public:
    // Constructor: initializes hand with given length and color
    explicit Hand(float length, sf::Color color);

    // Sets the hand's angle (rotation)
    void setAngle(float newAngle);

    // Draws the hand at a specified clock center position
    void draw(sf::RenderWindow& window, sf::Vector2f position) const;

    // Returns the current angle of the hand
    float getAngle() const;

    // Erases the hand by overdrawing it in the background color
    void erase(sf::RenderWindow& window, sf::Vector2f position, sf::Color backgroundColor) const;
};

#endif // HAND_H
