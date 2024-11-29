#include <SFML/Graphics.hpp>
#include <iostream>

/*

        KNOWN ISSUES:
        
        if you jump while touching the edge of a lava platform, the player will respawn in the floor
        you can still jump out so its no big deal. Or the player will fall from the ceiling, idk how any of it happens.

*/

using namespace sf;
using namespace std;

Texture spritesheet;
Sprite eggsprite;
Texture backgroundTexture;
Sprite background;

const Keyboard::Key controls[3] = {
    Keyboard::W,   // Player1 Up
    Keyboard::A,   // Player1 Left
    Keyboard::D,  // Player1 Right
};

// Define an enum for player states
enum EggState {
    STANDING,
    WALKING_LEFT,
    WALKING_RIGHT,
    RISING,
    FALLING,
    DYING
};

const Vector2f platformSize(200.f, 20.f);
const Vector2f groundSize(800.f, 20.f);
const Vector2f goalHitbox(20, 20);
int gameWidth = 800;
int gameHeight = 600;
const float ballRadius = 60.f;
const float deggRadius = 10.f;
Vector2f ballVelocity;
Vector2f deggVelocity;
const float deggHorizontalspeed = 50.f;
const float ballHorizontalSpeed = 400.f;
const float ballJumpSpeed = -1200.f;
const float initialVelocityY = 80.f;
bool canJump = false;
float dt;
int jumpTime = 0;
int hangTime = 0;
Font font;
Text fpstext;
Text gameOverText;

int deaths = 0;
Text deathsText;
int loops = 0;
int fps = 0.0f;
Time elapsedTime;
bool freeze = false;
bool complete = false;


//CircleShape ball;
CircleShape degg;
RectangleShape platform[7];

int platformArray = sizeof(platform) / sizeof(platform[0]);


void Load() {
    // Load font-face from res dir
    font.loadFromFile("C:/Users/angus/SET09121 - Coursework/res/fonts/RobotoMono - Regular.ttf");

    // Set size and origin of platform
    for (auto& p : platform) {
        p.setSize(platformSize);
        p.setOrigin((platformSize.x / 2.f), (platformSize.y / 2.f));
        p.setFillColor(Color::White);
    }

    platform[4].setFillColor(Color::Red);

    //for (int i = 0; i < 3; i++) {
        //platform[i].setSize(platformSize);
        //platform[i].setOrigin(platformSize / 2.f);
        //platform[i].setFillColor(Color::Cyan);
    //}


    // Set size and origin of ball
    //ball.setRadius(ballRadius);
    //ball.setOrigin(ballRadius, ballRadius); //Should be half the ball width and height
    eggsprite.setPosition(100, gameHeight / 2);

    //degg.setRadius(deggRadius);
    //degg.setOrigin(deggRadius, deggRadius);


    // Reset positions

    //TEXT DISPLAYS

    gameOverText.setColor(Color::Black);
    gameOverText.setPosition(0, 600);

    //GROUND PLATFORM
    platform[0].setSize(groundSize);
    platform[0].setPosition(Vector2f(400.f, 590.f));
    platform[0].setOrigin(groundSize.x / 2, groundSize.y / 2);

    //FLOATING PLATFORMS
    platform[1].setPosition(Vector2f(200.f, 480.f));
    platform[2].setPosition(Vector2f(300.f, 380.f));
    platform[3].setPosition(Vector2f(500.f, 500.f));

    //LAVA PLATFORM
    platform[4].setPosition(Vector2f(600.f, 590.f));


    eggsprite.setPosition(Vector2f(gameWidth / 2.f, gameHeight / 2.f));

    // Set Ball falling speed
    if (canJump == false) {
        ballVelocity = { 0, initialVelocityY };
    }


    deathsText.setCharacterSize(20);
    deathsText.setFont(font);
    String strdeaths = to_string(deaths);
    deathsText.setString("Deaths: " + strdeaths);
    deathsText.setPosition(0, 0);

    //GROUND PLATFORM
    platform[0].setSize(groundSize);
    platform[0].setPosition(Vector2f(400.f, 590.f));
    platform[0].setOrigin(groundSize.x / 2, groundSize.y / 2);

    //GOAL HITBOX
    platform[6].setPosition(Vector2f(550.f, 120.f));
    platform[6].setSize(Vector2f(20, 20));
    platform[6].setFillColor(Color::Cyan);
    platform[6].setOrigin(goalHitbox.x / 2, goalHitbox.y / 2);

    //FLOATING PLATFORMS
    platform[1].setPosition(Vector2f(200.f, 480.f));
    platform[2].setPosition(Vector2f(300.f, 380.f));
    platform[3].setPosition(Vector2f(400.f, 250.f));
    platform[5].setPosition(Vector2f(500.f, 140.f));

    //LAVA PLATFORM
    platform[4].setPosition(Vector2f(600.f, 590.f));


    //ENTITIES
    eggsprite.setPosition(Vector2f(50, 525));
    //degg.setPosition(Vector2f(500, 475));

    // Set Ball falling speed
    if (canJump == false) {
        ballVelocity = { 0, initialVelocityY };
    }

    /*
    if (!spritesheet.loadFromFile("res/Egg_with_Legg_spritesheet3.png")) {
        std::cerr << "Failed to load spritesheet!" << std::endl;
    }
    */

    eggsprite.setTexture(spritesheet);
    // border goes from 0-111,0-126. actual sprite is (x) 1-110, (y) 1-125
    // eggsprite.setTextureRect(IntRect(Vector2i(1, 1), Vector2i(110, 125)));
    eggsprite.setTextureRect(IntRect(1, 1, 110, 125));

    eggsprite.setScale(0.5f, 0.5f);

    //Start player as standing
    //player.setState(STANDING);

}

class Player {
public:
    // Constructor to initialize the player state
    Player() : state(STANDING), frameCounter(0), animationSpeed(10) {}

    // Method to set the player's state
    void setState(EggState newState) {
        state = newState;
    }

    // Method to get the current player's state
    EggState getState() const {
        return state;
    }

    void animateFrames(int startX, int startY, int frameCount) {
        // Calculate which frame to display based on frameCounter
        int currentFrame = (frameCounter / animationSpeed) % frameCount;

        // Set the texture rectangle to the current frame
        eggsprite.setTextureRect(sf::IntRect(startX + currentFrame * 32, startY, 32, 32));

        // Increment frameCounter for next update
        frameCounter++;
    }


private:
    int frameCounter;    // Tracks which frame of the animation to show
    int animationSpeed;  // Controls animation speed (higher = slower animation)
    EggState state;      // Stores the current state of the player


    sf::Texture spritesheet;
    sf::Sprite eggsprite;
};

Player player;

void Reset(RenderWindow& window) {
    window.clear();
    Load();
}

void Update(RenderWindow& window) {

    // Reset Jump validity
    canJump = false;

    // Reset clock, recalculate deltatime
    static Clock clock;
    float dt = clock.restart().asSeconds();
    // check and consume events
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
            return;
        }
    }

    if (canJump == false) {
        if (ballVelocity.y > -100.f && ballVelocity.y < 100.f) {
            //kill time
            hangTime++;
            if (hangTime % 2 == 0) {
                ballVelocity = { 0.f, ballVelocity.y + 100 };
            }
        }
        else if (ballVelocity.y < 1000.f) {
            ballVelocity = { 0.f, ballVelocity.y + 100 };
        }
    }

    switch (player.getState()) {
    case EggState::STANDING:
        player.animateFrames(1, 1, 3); //(animates frames from grids 1-3)
        eggsprite.setScale(0.5f, 0.5f); // ensures scale is normal
        break;

    case EggState::WALKING_LEFT:
        //(animates grids 4-6, same for WALKING_RIGHT)
        player.animateFrames(1, 126, 3); //Same frames as WALKING_RIGHT
        eggsprite.setScale(-0.5f, 0.5f); //Flipped horizontally
        eggsprite.setOrigin(110, 1);
        break;

    case EggState::WALKING_RIGHT:
        player.animateFrames(1, 126, 3);
        eggsprite.setScale(0.5f, 0.5f);
        eggsprite.setOrigin(1, 1);
        break;

    case EggState::RISING:
        // Display the first frame for 0.2 seconds
        if (elapsedTime.asSeconds() < 0.2) {
            // First frame, squat...
            eggsprite.setTextureRect(IntRect(223, 1, 110, 125)); //(coordinates for grid 3)
        }
        else {
            // Second frame, ...jump!
            eggsprite.setTextureRect(IntRect(1, 253, 110, 125)); //(coordinates for grid 7)
        }
        eggsprite.setScale(0.5f, 0.5f);
        break;

    case EggState::FALLING:
        // Display the first frame for 0.2 seconds
        if (elapsedTime.asSeconds() < 0.2) {
            // First frame
            eggsprite.setTextureRect(IntRect(111, 253, 110, 125));  //(coordinates for grid 8)
        }
        else {
            // Second frame
            eggsprite.setTextureRect(IntRect(223, 253, 110, 125)); //(coordinates for grid 9)
        }
        eggsprite.setScale(0.5f, 0.5f);
        break;
    case EggState::DYING:
        //(fixing scale not needed, doesn't matter if flipped horizontally)
        //egg cracks
        eggsprite.setTextureRect(IntRect(1, 379, 110, 125)); //(coordinates for grid 10)

        // TRIGGER GAME FREEZE

        //border goes from 0-111,0-126. actual sprite is (x) 1-110, (y) 1-125
        if (elapsedTime.asSeconds() < 0.2) {
            // First frame
            eggsprite.setTextureRect(IntRect(112, 379, 110, 125)); //(coordinates for grid 11)
        }
        else {
            // Second frame
            eggsprite.setTextureRect(IntRect(223, 379, 110, 125)); //(coordinates for grid 12)
        }
        break;
    }

    //ball.move(ballVelocity * dt);

    // Quit Via ESC Key
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
    }

    // handle ball movement (horizontal)
    float direction = 0.0f;
    if (Keyboard::isKeyPressed(controls[1])) {
        direction--;
        player.setState(WALKING_LEFT);
    }
    if (Keyboard::isKeyPressed(controls[2])) {
        direction++;
        player.setState(WALKING_RIGHT);
    }
    eggsprite.move(Vector2f(direction * ballHorizontalSpeed * dt, 0.f));

    //degg.move(Vector2f(-1 * deggHorizontalspeed * dt, 0.f));



    // Check Collision with platform

    const float bx = eggsprite.getPosition().x;
    const float by = eggsprite.getPosition().y;

    if (bx > gameWidth - ballRadius) {
        ballVelocity.x = -ballVelocity.x;
    }

    if (by > gameHeight - ballRadius) { //bottom wall
        ballVelocity.y = 0.f;
        canJump = true;
        player.setState(STANDING);
    }
    else if (by < 0) { //top wall
        eggsprite.move(Vector2f(0.f, 10.f));
        player.setState(FALLING);
    }


    float pL = 0;
    float pR = 0;
    float pT = 0;
    float pB = 0;

    for (int i = 0; i < platformArray; i++) {
        pL = platform[i].getPosition().x - 100;
        pR = platform[i].getPosition().x + 100;
        pT = platform[i].getPosition().y - 10;
        pB = platform[i].getPosition().y + 10;

        //GROUND HITBOX
        if (i == 0) {
            pL = platform[i].getPosition().x - 400;
            pR = platform[i].getPosition().x + 400;
            pT = platform[i].getPosition().y - 10;
            pB = platform[i].getPosition().y + 10;
        }

        //GOAL HITBOX
        if (i == 6) {
            pL = platform[i].getPosition().x - 10;
            pR = platform[i].getPosition().x + 10;
            pT = platform[i].getPosition().y - 10;
            pB = platform[i].getPosition().y + 10;
        }

        if (bx > pL && bx < pR && by >= pT - ballRadius && by < pB - ballRadius) {

            ballVelocity.y = 0.f;
            canJump = true;
            eggsprite.setPosition(Vector2f(bx, pT - ballRadius));

            if (/*by < pB + ballRadius  &&*/ by > pB) {
                ballVelocity.y = 0.f;
                eggsprite.move(Vector2f(0.f, 10.f));
            }
            if (i == 4) {
                gameOverText.setCharacterSize(40);
                gameOverText.setFont(font);
                gameOverText.setColor(Color::Red);
                gameOverText.setString("GAME OVER");
                deaths++;
                gameOverText.setPosition((gameWidth * .5f) - (gameOverText.getLocalBounds().width * .5f), gameHeight / 2);
                freeze = true;
            }
            if (i == 6) {
                gameOverText.setCharacterSize(40);
                gameOverText.setFont(font);
                gameOverText.setColor(Color::Green);
                gameOverText.setString("LEVEL COMPLETE");
                gameOverText.setPosition((gameWidth * .5f) - (gameOverText.getLocalBounds().width * .5f), gameHeight / 2);
                freeze = true;
                complete = true;
            }
        }
    }


    // handle ball jump
    if (canJump == true) {
        if (Keyboard::isKeyPressed(controls[0])) {
            //ball.move(Vector2f(0.f, ballJumpSpeed * dt));
            ballVelocity = { 0.f, ballJumpSpeed };
            jumpTime = 10;
            player.setState(RISING);
        }
    }

    eggsprite.move(ballVelocity * dt);

    if (by > gameHeight) {
        //ball.move(Vector2f(0.f, -10.f));
        eggsprite.setPosition(Vector2f(bx, gameHeight - ballRadius));
    }
}


void Render(RenderWindow& window) {
    // Draw Everything

    window.draw(background);
    for (int i = 0; i < platformArray; i++){
        window.draw(platform[i]);
    }
    
    window.draw(eggsprite);
    //window.draw(degg);
    window.draw(fpstext);
    window.draw(gameOverText);
    window.draw(deathsText);

}

int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "EGG WITH LEGG");
    Load();

    window.setFramerateLimit(60);
    Clock clock;

    
    if (!backgroundTexture.loadFromFile("C:/Users/angus/SET09121-Coursework/res/KitchenBackground.png")) 
    {
        cout << "ERROR loading background" << endl;
    } else
    {
        background.setTexture(backgroundTexture);
    }

    if (!spritesheet.loadFromFile("C:/Users/angus/SET09121-Coursework/res/EggSpritesheet.png"))
    {
        cout << "ERROR loading spritesheet" << endl;
    }

    while (window.isOpen()) {

        if (freeze != true && complete != true) {

            /*      FPS DISPLAY     */
            
            fps = 0.f;
            loops = 0;
            // set the character size to 24 pixels
            fpstext.setCharacterSize(20);
            elapsedTime = clock.restart();
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


            window.clear();
            window.draw(eggsprite);

            Update(window);

            Render(window);

            window.display();

            loops++;
        }
        else if (freeze == true && complete != true) {
            sleep(seconds(2));
            freeze = false;
            Reset(window);
        }
        else {
            sleep(seconds(2));
            window.close();

        }
    }
    return 0;
}