#include "Hand.h"

Hand::Hand(float length, sf::Color color) 
    : angle(0), color(color), is_visible(true) { 
    shape.setSize(sf::Vector2f(length, 3));        // Set the length and thickness of the hand
    shape.setOrigin(0, 1.5);                       // Pivot point at the base (center height-wise)
    shape.setFillColor(this->color);
}

float Hand::getAngle() const {
    return angle;
}

void Hand::setAngle(float newAngle) {
    angle = newAngle;
    shape.setRotation(angle);                     // Rotate the hand to the new angle
}

void Hand::draw(sf::RenderWindow& window, sf::Vector2f position) const {
    if (is_visible) {
        shape.setPosition(position);
        window.draw(shape);
    }
}

void Hand::erase(sf::RenderWindow& window, sf::Vector2f position, sf::Color backgroundColor) const {
    if (is_visible) {
        sf::RectangleShape temp = shape;          // Create a copy to avoid modifying original shape
        temp.setPosition(position);
        temp.setFillColor(backgroundColor);       // Set to background color to simulate erasing
        window.draw(temp);
    }
}
