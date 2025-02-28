#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

#include "level_manager.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "level_objects.hpp"
#include "goal.hpp"
#include "sound_manager.hpp"

using namespace sf;
using namespace std;

Texture backgroundTexture;
Sprite background;

SoundBuffer buffer;
Sound jumpsound;


LevelManager lm;

Font font;

int gameWidth = 1200;
int gameHeight = 900;

float dt;

Text fpstext;
Text gameOverText;
Text deathsText;
Time elapsedTime;

int deaths = 0;
int loops = 0;
int fps = 0.0f;

bool freeze = false;
bool complete = false;

void Load() {
    // Load font-face from res dir
    font.loadFromFile("C:/Users/angus/SET09121-Coursework/res/font/RobotoMono-Regular.ttf");

    //TEXT DISPLAYS
    gameOverText.setFillColor(Color::Black);
    gameOverText.setPosition(0, 1200);

    deathsText.setCharacterSize(20);
    deathsText.setFont(font);
    String strdeaths = to_string(deaths);
    deathsText.setString("Deaths: " + strdeaths);
    deathsText.setPosition(0, 0);

    // Entities
    Player* p = new Player(50.f, 825.f);
    Enemy* e1 = new Enemy(450.f, 660.f, 50, -1);
    Enemy* e2 = new Enemy(400.f, 450.f, 80, -1);

    EnemyMoveRight* emr1 = new EnemyMoveRight(310.f, 680.f, 20, 20);
    EnemyMoveRight* emr2 = new EnemyMoveRight(270.f, 470.f, 20, 20);
    EnemyMoveLeft* eml1 = new EnemyMoveLeft(570.f, 680.f, 20, 20);
    EnemyMoveLeft* eml2 = new EnemyMoveLeft(620.f, 470.f, 20, 20);

    Goal* g = new Goal(690.f, 198.f);

    lm.addEntity(p);
    lm.addEntity(e1);
    lm.addEntity(e2);
    lm.addEntity(emr1);
    lm.addEntity(emr2);
    lm.addEntity(eml1);
    lm.addEntity(eml2);
    lm.addEntity(g);

    // Platforms
    sf::RectangleShape* p0 = new sf::RectangleShape({1200, 20});
    sf::RectangleShape* p1 = new sf::RectangleShape({200, 20});
    sf::RectangleShape* p2 = new sf::RectangleShape({200, 20});
    sf::RectangleShape* p3 = new sf::RectangleShape({200, 20});
    sf::RectangleShape* p4 = new sf::RectangleShape({200, 20});
    sf::RectangleShape* p5 = new sf::RectangleShape({200, 20});
    sf::RectangleShape* p6 = new sf::RectangleShape({300, 20});
    sf::RectangleShape* p7 = new sf::RectangleShape({200, 20});
    sf::RectangleShape* p8 = new sf::RectangleShape({200, 20});
    sf::RectangleShape* p9 = new sf::RectangleShape({200, 20});

    p0->setPosition(0, 880);
    p1->setPosition(100, 770);
    p2->setPosition(350, 720);
    p3->setPosition(675, 720);
    p4->setPosition(960, 640);
    p5->setPosition(680, 520);
    p6->setPosition(300, 510);
    p7->setPosition(70, 440);
    p8->setPosition(350, 320);
    p9->setPosition(630, 240);

    lm.addPlatform(p0);
    lm.addPlatform(p1);
    lm.addPlatform(p2);
    lm.addPlatform(p3);
    lm.addPlatform(p4);
    lm.addPlatform(p5);
    lm.addPlatform(p6);
    lm.addPlatform(p7);
    lm.addPlatform(p8);
    lm.addPlatform(p9);

    p->addPlatforms(lm.getPlatforms());
}

void Reset(RenderWindow& window) {
    window.clear();
    lm.clearEntities();
    lm.clearPlatforms();
    Load();
}

void Update(RenderWindow& window) {
    // Reset clock, recalculate deltatime
    static Clock clock;
    dt = clock.restart().asSeconds();

    // check and consume events
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
            return;
        }
    }

    // Quit Via ESC Key
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
    }
}


void Render(RenderWindow& window) {
    window.draw(background);
    window.draw(fpstext);
    window.draw(gameOverText);
    window.draw(deathsText);
}

int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "EGG WITH LEGG");
    Load();

    window.setFramerateLimit(60);
    Clock clock;

    if (!backgroundTexture.loadFromFile("C:/Users/angus/SET09121-Coursework/res/background/kitchen.png")) {
        cerr << __FILE__ << ":" << __LINE__ << ": ERROR: loading background " << endl;
    } else {
        background.setTexture(backgroundTexture);
        background.setScale(1.2f, 1.2f);
    }

    while (window.isOpen()) {
        elapsedTime = clock.restart();
        if (freeze != true && complete != true) {
            /*      FPS DISPLAY     */

            fps = 0.f;
            loops = 0;
            // set the character size to 24 pixels
            fpstext.setCharacterSize(20);

            if (elapsedTime.asSeconds() < 1) {
                loops++;
            }
            if (elapsedTime.asSeconds() > 0.0f) {
                fps = loops / elapsedTime.asSeconds();
            }
            String str_fps = to_string(fps);
            String str_Header = "FPS: " + str_fps;
            fpstext.setFont(font);
            fpstext.setString(str_Header);
            fpstext.setPosition(0, 25);

            Update(window);
            Render(window);

            std::string s = lm.update(window, dt);

            if (s == "PlayerDeath") {
                freeze = true;
                complete = false;
                gameOverText.setCharacterSize(40);
                gameOverText.setFont(font);
                gameOverText.setFillColor(Color::Red);
                gameOverText.setString("GAME OVER");
                deaths++;
                gameOverText.setPosition((gameWidth * .5f) - (gameOverText.getLocalBounds().width * .5f),
                                         static_cast<float>(gameHeight) / 2);
                window.draw(gameOverText);
            } else if (s == "Goal") {
                freeze = true;
                complete = true;
                gameOverText.setCharacterSize(40);
                gameOverText.setFont(font);
                gameOverText.setFillColor(Color::Green);
                gameOverText.setString("LEVEL COMPLETE");
                gameOverText.setPosition((gameWidth * .5f) - (gameOverText.getLocalBounds().width * .5f),
                                         static_cast<float>(gameHeight) / 2);
                window.draw(gameOverText);
            }

            window.display();

            loops++;
        } else if (freeze == true && complete != true) {
            sleep(seconds(2));
            gameOverText.setString("");
            freeze = false;
            Reset(window);
        } else {
            sleep(seconds(2));
            window.close();
        }

    }
    return 0;
}
