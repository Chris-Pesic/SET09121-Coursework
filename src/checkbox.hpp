#ifndef CHECKBOX_HPP
#define CHECKBOX_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "button.hpp"

class Checkbox : protected Button {
public:
    Checkbox(std::string string, int fontSize, sf::Vector2f position, sf::Vector2f size, sf::Font &font) : Button(string, fontSize, position, size, font) {
        fontSize = fontSize;
        position = position;
        size = size;

        ticked = false;
        pressed = false;

        rect.setFillColor({64, 64, 64, 200});
        rect.setOutlineColor({64, 64, 64, 200});
        rect.setOutlineThickness(10.f);
        rect.setPosition(position);
        rect.setSize(size);

        text.setString(string);
        text.setFont(font);
        text.setCharacterSize(fontSize);
        text.setPosition({position.x - (text.getLocalBounds().width + 20),
                position.y + ((rect.getSize().y / 2) - (text.getLocalBounds().height - 4))});
    }

    std::string update(sf::RenderWindow &window, float dt) override {
        bool mouseInRect = rect.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
        if (mouseInRect && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            pressed = true;
        } else if (mouseInRect && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && pressed) {
            ticked = !ticked;
            pressed = false;
        }

        return (ticked) ? "true" : "false";
    }

    void render(sf::RenderWindow &window) override {
        if (ticked) {
            rect.setFillColor({255, 255, 255, 200});
        } else {
            rect.setFillColor({64, 64, 64, 200});
        }

        window.draw(rect);
        window.draw(text);
    }
protected:
    bool ticked;
};

#endif /* CHECKBOX_HPP */
