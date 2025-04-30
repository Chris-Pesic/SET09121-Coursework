#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

#include "level_manager.hpp"
#include "button.hpp"
#include "checkbox.hpp"
#include "sound_manager.hpp"

using namespace sf;
using namespace std;

Texture gameBackgroundTexture;
Texture menuBackgroundTexture;
Sprite background;


SoundBuffer buffer;
Sound jumpsound;

enum ScreenState {
    MainMenuState = 0,
    GameState,
    SettingsState
};

ScreenState state;

bool fullscreen;

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
    if (state == GameState) {
        window.draw(fpstext);
        window.draw(gameOverText);
        window.draw(deathsText);
    }
}

/**
 * @brief Runs first when program is ran
 * @return Returns error code
 */
int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "EGG WITH LEGG");
    Load();

    SoundManager::getInstance().playBGM1();
    fullscreen = false;

    window.setFramerateLimit(60);
    Clock clock;
    Clock time;
    time.restart();

    state = MainMenuState;

    if (!gameBackgroundTexture.loadFromFile("./res/background/kitchen.png")) {
        cerr << __FILE__ << ":" << __LINE__ << ": ERROR: loading background " << endl;
    }
    if (!menuBackgroundTexture.loadFromFile("./res/background/mainmenu.png")) {
        cerr << __FILE__ << ":" << __LINE__ << ": ERROR: loading background " << endl;
    } else {
        background.setTexture(menuBackgroundTexture);
        background.setScale(1.5f, 1.5f);
    }

    // Main menu buttons
    std::unique_ptr<Button> start_button(new Button("Start", 20, {413, 500}, {374, 73}, font));
    std::unique_ptr<Button> settings_button(new Button("Settings", 20, {413, 600}, {374, 73}, font));
    std::unique_ptr<Button> exit_button(new Button("Exit", 20, {413, 700}, {374, 73}, font));

    // Settings buttons
    std::unique_ptr<Button> back_button(new Button("Back", 20, {10, 817}, {374, 73}, font));
    std::unique_ptr<Checkbox> mute_checkbox(new Checkbox("Mute SFX", 20, {500, 430}, {25, 25}, font));
    std::unique_ptr<Checkbox> fullscreen_checkbox(new Checkbox("Fullscreen", 20, {500, 500}, {25, 25}, font));

    while (window.isOpen()) {
        elapsedTime = clock.restart();

        if (state == MainMenuState) {
            Update(window);
            Render(window);

            start_button->render(window);
            settings_button->render(window);
            exit_button->render(window);

            if (start_button->update(window, dt) == "pressed") {
                background.setTexture(gameBackgroundTexture);
                background.setScale(1.5f, 1.5f);
                state = GameState;
            }
            if (settings_button->update(window, dt) == "pressed") {
                state = SettingsState;
            }
            if (exit_button->update(window, dt) == "pressed") {
                exit(0);
            }

            window.display();
        } else if (state == SettingsState) {
            Update(window);
            Render(window);

            back_button->render(window);

            mute_checkbox->render(window);
            fullscreen_checkbox->render(window);

            if (mute_checkbox->update(window, dt) == "true") {
                SoundManager::getInstance().setVolume(0.0f);
            } else {
                SoundManager::getInstance().setVolume(0.8f);
            }

            if (fullscreen_checkbox->update(window, dt) == "true" && !fullscreen) {
                window.create(sf::VideoMode::getFullscreenModes()[0], "EGG WITH LEGG", sf::Style::Fullscreen);
                fullscreen = true;
            } else if (fullscreen_checkbox->update(window, dt) == "false" && fullscreen) {
                window.create(VideoMode(gameWidth, gameHeight), "EGG WITH LEGG");
                fullscreen = false;
            }

            if (back_button->update(window, dt) == "pressed") {
                state = MainMenuState;
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
