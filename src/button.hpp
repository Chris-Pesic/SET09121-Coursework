#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

class Button {
public:
    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f position);
    sf::Vector2f getSize();
    void setSize(sf::Vector2f size);
    int getFontSize();
    void setFontSIze(int fontSize);

    virtual std::string update(sf::RenderWindow &window, float dt) {return "";}
    virtual void render(sf::RenderWindow &window) {}

    std::string state(sf::RenderWindow &window) {
        if (rect.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) &&
            sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            return "pressed";
        } else if (rect.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
            return "hover";
        }

        return "";
    }
protected:
    sf::RectangleShape rect;
    sf::Text buttonText;
    int fontSize;
    sf::Vector2f position;
    sf::Vector2f size;
};

class StartButton : public Button {
public:
    StartButton(std::string text, int fontSize, sf::Vector2f position, sf::Vector2f size, sf::Font font) {
        buttonText.setString(text);
        fontSize = fontSize;
        position = position;
        size = size;

        buttonText.setCharacterSize(fontSize);
        buttonText.setPosition(position);
        buttonText.setFont(font);
        rect.setFillColor({64, 64, 64, 200});
        rect.setPosition(position);
        rect.setSize(size);
    }

    std::string update(sf::RenderWindow &window, float dt) override {
        return state(window);
    }
    void render(sf::RenderWindow &window) override {
        window.draw(rect);
        window.draw(buttonText);
    }
};

#endif /* BUTTON_HPP */
