#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class Button {
public:
    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f position);
    sf::Vector2f getSize();
    void setSize(sf::Vector2f size);
    int getFontSize();
    void setFontSIze(int fontSize);

    Button(std::string string, int fontSize, sf::Vector2f position, sf::Vector2f size, sf::Font &font) {
        fontSize = fontSize;
        position = position;
        size = size;

        pressed = false;

        rect.setFillColor({64, 64, 64, 200});
        rect.setPosition(position);
        rect.setSize(size);

        text.setString(string);
        text.setFont(font);
        text.setCharacterSize(fontSize);
        text.setPosition({position.x + ((rect.getSize().x / 2) - (text.getLocalBounds().width / 2)),
                position.y + ((rect.getSize().y / 2) - (text.getLocalBounds().height - 4))});
    }

    virtual std::string update(sf::RenderWindow &window, float dt) {
        return state(window);
    }
    virtual void render(sf::RenderWindow &window) {
        if (state(window) == "hover") {
            rect.setFillColor({94, 94, 94, 200});
        } else if (state(window) == "pressed") {
            rect.setFillColor({128, 128, 128, 200});
        } else {
            rect.setFillColor({64, 64, 64, 200});
        }

        window.draw(rect);
        window.draw(text);
    }

    std::string state(sf::RenderWindow &window) {
        if (rect.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) &&
            sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            return "pressed";
        } else if (rect.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
            return "hover";
        }

        bool mouseInRect = rect.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
        if (mouseInRect && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !pressed) {
            pressed = true;
        } else if (mouseInRect && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && pressed) {
            pressed = false;
            return "pressed";
        } else if (mouseInRect) {
            return "hover";
        }

        return "";
    }
protected:
    sf::RectangleShape rect;
    sf::Text text;
    int fontSize;
    sf::Vector2f position;
    sf::Vector2f size;
    bool pressed;
};

#endif /* BUTTON_HPP */
