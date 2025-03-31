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
#include "button.hpp"

using namespace sf;
using namespace std;

Texture backgroundTexture;
Sprite background;

SoundBuffer buffer;
Sound jumpsound;

enum ScreenState {
    MainMenuState = 0,
    GameState
};

ScreenState state;

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
    font.loadFromFile("./res/font/RobotoMono-Regular.ttf");

    //TEXT DISPLAYS
    gameOverText.setFillColor(Color::Black);
    gameOverText.setPosition(0, 1200);

    deathsText.setCharacterSize(20);
    deathsText.setFont(font);
    String strdeaths = to_string(deaths);
    deathsText.setString("Deaths: " + strdeaths);
    deathsText.setPosition(0, 0);

    lm.loadLevel("./res/levels/level1.xml");
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

/**
 * @brief Runs first when program is ran
 * @return Returns error code
 */
int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "EGG WITH LEGG");
    Load();

    window.setFramerateLimit(60);
    Clock clock;
    Clock time;
    time.restart();

    state = MainMenuState;

    if (!backgroundTexture.loadFromFile("./res/background/kitchen.png")) {
        cerr << __FILE__ << ":" << __LINE__ << ": ERROR: loading background " << endl;
    } else {
        background.setTexture(backgroundTexture);
        background.setScale(1.2f, 1.2f);
    }

    StartButton* sb = new StartButton(20, {100, 100}, {230, 80}, font);
    ExitButton* eb = new ExitButton(20, {100, 200}, {200, 80}, font);

    while (window.isOpen()) {
        elapsedTime = clock.restart();

        if (state == MainMenuState) {
            Update(window);
            Render(window);

            // if (time.getElapsedTime().asSeconds() >= 3.f) {
            //     state = GameState;
            // }

            sb->render(window);
            eb->render(window);

            if (sb->update(window, dt) == "pressed") {
                state = GameState;
            }
            if (eb->update(window, dt) == "pressed") {
                exit(0);
            }

            window.display();
        } else if (state == GameState) {
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
    }
    return 0;
}
