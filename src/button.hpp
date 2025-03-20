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
    sf::Text text;
    int fontSize;
    sf::Vector2f position;
    sf::Vector2f size;
};

class StartButton : public Button {
public:
    StartButton(int fontSize, sf::Vector2f position, sf::Vector2f size, sf::Font &font) {
        fontSize = fontSize;
        position = position;
        size = size;

        rect.setFillColor({64, 64, 64, 200});
        rect.setPosition(position);
        rect.setSize(size);

        text.setString("Start");
        text.setFont(font);
        text.setCharacterSize(fontSize);
        text.setPosition({position.x + ((rect.getSize().x / 2) - (text.getLocalBounds().width / 2)),
                position.y + ((rect.getSize().y / 2) - (text.getLocalBounds().height - 4))});
    }

    std::string update(sf::RenderWindow &window, float dt) override {
        return state(window);
    }
    void render(sf::RenderWindow &window) override {
        if (state(window) == "hover") {
            rect.setFillColor({94, 94, 94, 200});
        } else if (state(window) == "click") {
            rect.setFillColor({128, 128, 128, 200});
        } else {
            rect.setFillColor({64, 64, 64, 200});
        }

        window.draw(rect);
        window.draw(text);
    }
};

class ExitButton : public Button {
public:
    ExitButton(int fontSize, sf::Vector2f position, sf::Vector2f size, sf::Font &font) {
        fontSize = fontSize;
        position = position;
        size = size;

        rect.setFillColor({64, 64, 64, 200});
        rect.setPosition(position);
        rect.setSize(size);

        text.setString("Exit");
        text.setFont(font);
        text.setCharacterSize(fontSize);
        text.setPosition({position.x + ((rect.getSize().x / 2) - (text.getLocalBounds().width / 2)),
                position.y + ((rect.getSize().y / 2) - (text.getLocalBounds().height - 4))});
    }

    std::string update(sf::RenderWindow &window, float dt) override {
        return state(window);
    }
    void render(sf::RenderWindow &window) override {
        if (state(window) == "hover") {
            rect.setFillColor({94, 94, 94, 200});
        } else if (state(window) == "click") {
            rect.setFillColor({128, 128, 128, 200});
        } else {
            rect.setFillColor({64, 64, 64, 200});
        }

        window.draw(rect);
        window.draw(text);
    }
};

#endif /* BUTTON_HPP */
