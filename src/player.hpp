#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "misc.hpp"
#include "entity.hpp"

/**
 * @brief The player class.
 */
class Player : public Entity {
public:
    Player(float sx, float sy);
    ~Player();

    std::string collide(std::string object) override;

    void addPlatforms(std::vector<sf::RectangleShape*> platforms);

    void update(sf::RenderWindow &window, float dt) override;
    void render(sf::RenderWindow &window) override;
private:
    int skinNum = 0;
    std::string spritePath;
    bool isGrounded;
    EggState spriteState;
    std::vector<sf::RectangleShape*> platforms;
};

#endif /* PLAYER_HPP */
