#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
#include <iostream>


/*

        KNOWN ISSUES:

        if you jump while touching the edge of a lava platform, the player will respawn in the floor
        you can still jump out so its no big deal. Or the player will fall from the ceiling, idk how any of it happens.
        STATUS: FIXED - 26/11/2024

*/

using namespace sf;
using namespace std;

//eggy textures
Texture spritesheet;
Sprite eggsprite;
//deviled egg textures
Texture spritesheetdevil;
Sprite devilsprite;
//background textures
Texture backgroundTexture;
Sprite background;
//Goal texture
Texture goalTexture;
Sprite goalsprite;
//Hammer textures
Texture hammerTexture;
Sprite hammersprite;

IntRect eggSourceSprite(1, 1, 110, 125);
IntRect devilSourceSprite(223, 1, 110, 125); //grid 3
IntRect hammerSourceSprite(1, 1, 203, 129);

//SoundBuffer buffer;
//Sound jumpsound;

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
const Vector2f groundSize(1200.f, 20.f);
const Vector2f goalHitbox(112.f, 43.f);
int gameWidth = 1200;
int gameHeight = 900;
const float ballRadius = 60.f;
const float deggRadius = 60.f;
const Vector2f hammerHitbox(20.f, 20.f);
Vector2f ballVelocity;
Vector2f deggVelocity;
const float deggHorizontalspeed = 50.f;
const float degg2Horizontalspeed = 150.f;
const float ballHorizontalSpeed = 400.f;
const float ballJumpSpeed = -1200.f;
const float initialVelocityY = 80.f;
bool canJump = false;
float dt;
int jumpTime = 0;
int hangTime = 0;
int currentFrame = 0;
Font font;
Text fpstext;
Text gameOverText;

Clock animateClock;
Clock animateClockDevil;
//Clock animateClockHammer;


int deaths = 0;
Text deathsText;
int loops = 0;
int fps = 0.0f;
Time elapsedTime;
bool freeze = false;
bool complete = false;

// These are used in the state machine for the deviled egg, the flags are used to change the state of the entity
bool degg_IS_SEEKING = false;
bool degg_IS_WANDERING = true;

bool hasReached_Wall_left = false;
bool hasReached_Wall_right = true;


//CircleShape ball;
CircleShape degg;
CircleShape degg2;
//RectangleShape hammer;

RectangleShape platform[12];

int platformArray = sizeof(platform) / sizeof(platform[0]);


void Load() {
    // Load font-face from res dir
    font.loadFromFile("C:/Users/chris/ENU OneDrive/OneDrive - Edinburgh Napier University/Year 3/Modules/TR1/Games Engineering/Coursework/SET09121-Coursework/res/fonts/RobotoMono-Regular.ttf");

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
    //ball.setOrigin(ballRadius, ballRadius);
    //ball.setPosition(100, gameHeight / 2);
    eggsprite.setPosition(100, gameHeight / 2);

    degg.setRadius(deggRadius);
    degg.setOrigin(deggRadius, deggRadius);

    degg2.setRadius(deggRadius);
    degg2.setOrigin(deggRadius, deggRadius);


    // Reset positions

    //TEXT DISPLAYS
    gameOverText.setColor(Color::Black);
    gameOverText.setPosition(0, 1200);

    deathsText.setCharacterSize(20);
    deathsText.setFont(font);
    String strdeaths = to_string(deaths);
    deathsText.setString("Deaths: " + strdeaths);
    deathsText.setPosition(0, 0);

    //GROUND PLATFORM
    platform[0].setSize(groundSize);
    platform[0].setPosition(Vector2f(600.f, 890.f));
    platform[0].setOrigin(groundSize.x / 2, groundSize.y / 2);

    //FLOATING PLATFORMS
    platform[1].setPosition(Vector2f(200.f, 780.f));
    platform[2].setPosition(Vector2f(450.f, 730.f)); //degg
    platform[3].setPosition(Vector2f(775.f, 730.f));
    platform[5].setPosition(Vector2f(1060.f, 650.f));
    platform[7].setPosition(Vector2f(780.f, 530.f)); 
    platform[8].setPosition(Vector2f(450.f, 520.f)); //degg
    platform[8].setSize(Vector2f(300, 20));
    platform[9].setPosition(Vector2f(170.f, 450.f));
    platform[10].setPosition(Vector2f(450.f, 330.f));
    platform[11].setPosition(Vector2f(730.f, 250.f)); //goal

    //LAVA PLATFORM
    platform[4].setPosition(Vector2f(600.f, 890.f));

    //GOAL HITBOX 112x43
    goalsprite.setPosition(Vector2f(950.f, 550.f));
    platform[6].setPosition(Vector2f(950.f, 560.f));
    platform[6].setSize(Vector2f(20, 20));
    //platform[6].setFillColor(Color::Color(255,255,255,255));

    platform[6].setOrigin(goalHitbox.x / 2, goalHitbox.y / 2);
    goalsprite.setOrigin(goalHitbox.x / 2, goalHitbox.y / 2);

    //ENTITIES
    //ball.setPosition(Vector2f(50, 825));
    eggsprite.setPosition(Vector2f(50.f, 825.f));
    //eggsprite.setOrigin(Vector2f(55, 72.5));

    degg.setPosition(Vector2f(500.f, 710.f));
    degg.setFillColor(Color::Magenta);
/*
    //205x130, 391 total
    hammer.setSize(Vector2f(hammerHitbox));
    hammer.setOrigin(hammerHitbox.x / 2, hammerHitbox.y / 2);
    hammersprite.setOrigin(hammerHitbox.x / 2, hammerHitbox.y / 2);
    hammer.setPosition(Vector2f(650.f, 560.f));
    hammersprite.setPosition(Vector2f(650.f, 560.f));
    hammer.setFillColor(Color::Yellow);*/
  
    degg2.setPosition(Vector2f(450.f, 500.f));
    degg2.setFillColor(Color::Magenta);


    // Set Ball falling speed
    if (canJump == false) {
        ballVelocity = { 0, initialVelocityY };
    }

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
    eggsprite.setPosition(Vector2f(50, 825));
    devilsprite.setPosition(Vector2f(500, 710));
    //degg.setPosition(Vector2f(350, 660));
    //Render(window);
}

void GameOver(/*&Renderwindow window*/) {
    if (complete == true) {
        gameOverText.setCharacterSize(40);
        gameOverText.setFont(font);
        gameOverText.setColor(Color::Green);
        gameOverText.setString("LEVEL COMPLETE");
        gameOverText.setPosition((gameWidth * .5f) - (gameOverText.getLocalBounds().width * .5f), gameHeight / 2);
        freeze = true;
    }
    else {
        gameOverText.setCharacterSize(40);
        gameOverText.setFont(font);
        gameOverText.setColor(Color::Red);
        gameOverText.setString("GAME OVER");
        deaths++;
        gameOverText.setPosition((gameWidth * .5f) - (gameOverText.getLocalBounds().width * .5f), gameHeight / 2);
        freeze = true;
        player.setState(DYING);
        //Update(window);
    }
}

/*
void deviledEgg_StateMachine() {

    if (degg_IS_SEEKING == true) {
        if (degg.getPosition().x > ball.getPosition().x && degg.getPosition().x > 260 && ball.getPosition().y >= 620 && ball.getPosition().y <= degg.getPosition().y) {
            degg.move(Vector2f(-1 * deggHorizontalspeed * dt, 0.f));
        }
        else if (degg.getPosition().x < ball.getPosition().x && degg.getPosition().x < 440 && ball.getPosition().y >= 620 && ball.getPosition().y <= degg.getPosition().y) {
            degg.move(Vector2f(1 * deggHorizontalspeed * dt, 0.f));
        }
        else {
            degg_IS_SEEKING = false;
            degg_IS_WANDERING = true;
        }
    }

    //If entity deviled egg has not detected the player, initially move to the left wall
    //Once that wall has been reached, move to opposite wall, repeat until player detected
    else if (degg_IS_WANDERING == true) {
        if (hasReached_Wall_left == false) {
            while (degg.getPosition().x > 270) {
                degg.move(Vector2f(-1 * deggHorizontalspeed * dt, 0.f));
                if (degg.getPosition().x <= 270) {
                    hasReached_Wall_left = true;
                    hasReached_Wall_right = false;
                    break;
                }
            }
        }
        else if (hasReached_Wall_right == false) {
            while (degg.getPosition().x < 430) {
                degg.move(Vector2f(1 * deggHorizontalspeed * dt, 0.f));
                if (degg.getPosition().x >= 430) {
                    hasReached_Wall_left = false;
                    hasReached_Wall_right = true;
                    break;
                }
            }
        }
        else if (ball.getPosition().y <= degg.getPosition().y && ball.getPosition().x >= 260 || ball.getPosition().x <= 440 && ball.getPosition().y >= 620) {
            degg_IS_SEEKING = true;
            degg_IS_WANDERING = false;
        }
    }
}
*/

void Update(RenderWindow& window) {
    //deviledEgg_StateMachine();

    // Reset Jump validity
    canJump = false;

    switch (player.getState()) {
    case EggState::STANDING:
        eggSourceSprite.top = 1;
        // Animate grids 1-3
        if (animateClock.getElapsedTime().asSeconds() > 0.2f) {
            if (eggSourceSprite.left == 223)
                eggSourceSprite.left = 1;
            else
            {
                eggSourceSprite.left += 111;
            }

            eggsprite.setTextureRect(eggSourceSprite);
            animateClock.restart();
        }
        eggsprite.setScale(0.5f, 0.5f); // Ensure normal scale
        eggsprite.setOrigin(0, 0);      // Reset origin
        break;

    case EggState::WALKING_LEFT:
        // Animate grids 4-6 (left-facing)
        eggSourceSprite.top = 126;
        if (animateClock.getElapsedTime().asSeconds() > 0.2f) {
            if (eggSourceSprite.left == 223)
                eggSourceSprite.left = 1;
            else
            {
                eggSourceSprite.left += 111;
            }

            eggsprite.setTextureRect(eggSourceSprite);
            animateClock.restart();
        }
        eggsprite.setScale(-0.5f, 0.5f); // Flipped horizontally
        eggsprite.setOrigin(110, 0);     // Adjust origin for flipping
        break;

    case EggState::WALKING_RIGHT:
        eggSourceSprite.top = 126;
        // Animate grids 4-6 (right-facing)
        if (animateClock.getElapsedTime().asSeconds() > 0.2f) {
            if (eggSourceSprite.left == 223)
                eggSourceSprite.left = 1;
            else
                eggSourceSprite.left += 111;

            eggsprite.setTextureRect(eggSourceSprite);
            animateClock.restart();
        }
        eggsprite.setScale(0.5f, 0.5f); // Normal orientation
        eggsprite.setOrigin(0, 0);      // Reset origin
        break;

    case EggState::RISING:
        // Static frame while jumping up
        eggSourceSprite.top = 223; // Grid 3
        eggSourceSprite.left = 1;
        //eggsprite.setTextureRect(IntRect(223, 2, 110, 125)); 
        eggsprite.setScale(0.5f, 0.5f); // Ensure normal scale
        eggsprite.setOrigin(0, 0);      // Reset origin
        break;

    case EggState::FALLING:
        // Static frame for FALLING
        eggsprite.setTextureRect(IntRect(111, 253, 110, 125)); // Grid 8
        eggsprite.setScale(0.5f, 0.5f);
        eggsprite.setOrigin(0, 0);
        break;

    case EggState::DYING:
        if (freeze == true) {
            eggSourceSprite.top = 379;
            // Animate grids 4-6 (right-facing)
            if (animateClock.getElapsedTime().asSeconds() > 0.4f) {
                if (eggSourceSprite.left == 223)
                    break;
                else
                    eggSourceSprite.left += 111;

                eggsprite.setTextureRect(eggSourceSprite);
                animateClock.restart();
            }
            eggsprite.setScale(0.5f, 0.5f); // Normal orientation
            eggsprite.setOrigin(0, 0);      // Reset origin
            break;
            eggsprite.setTextureRect(IntRect(1, 379, 110, 125)); // Grid 10  
        }

    default:
        break;
    }

    //animateClockDevil;
    // grids 3,-9,7,10,8,-10,-7,9, back to 3 (I understand this is overcomplicated but I don't want to edit the png file)

    struct Frame {
        int top;
        int left;
        bool flipped; // Indicates whether the sprite should be flipped horizontally
    };

    vector<Frame> devilFrames = {
        {1, 223, false},  // Grid 3 (regular)
        {252, 223, true}, // Grid 9 (flipped)
        {252, 1, false},  // Grid 7 (regular)
        {378, 1, false},  // Grid 10 (regular)
        {252, 111, false}, // Grid 8 (regular)
        {378, 1, true},   // Grid 10 (flipped)
        {252, 1, true},   // Grid 7 (flipped)
        {252, 223, false} // Grid 9 (regular)
    };

    //devilSourceSprite.top = 1;
    if (animateClockDevil.getElapsedTime().asSeconds() >= 0.1) {
        Frame frame = devilFrames[currentFrame];
        devilsprite.setTextureRect(IntRect(frame.left, frame.top, 110, 125));
        //cout << "top = " << devilSourceSprite.top << ", left = " << devilSourceSprite.left << endl;

        // Handle flipping
        if (frame.flipped) {
            devilsprite.setScale(-0.5f, 0.5f);  // Flipped
            devilsprite.setOrigin(110, 0);   // Adjust origin to flip properly
        }
        else {
            devilsprite.setScale(0.5f, 0.5f); // Normal
            devilsprite.setOrigin(0, 0);
        }

        // Move to the next frame
        currentFrame = (currentFrame + 1) % devilFrames.size();
        animateClockDevil.restart();
    }

    //Hammer animation and movement
    /*
    if (animateClockHammer.getElapsedTime().asSeconds() >= 0.3)
    {
        hammerSourceSprite.top += 130;
        if (animateClockHammer.getElapsedTime().asSeconds() >= 0.6)
        {
            hammerSourceSprite.top += 130;
            //fall
            hammersprite.move(Vector2f(0.f, -1 * ballHorizontalSpeed * dt));

            if (animateClockHammer.getElapsedTime().asSeconds() >= 2)
            {
                //rise

            }
        }
    */

    //position failsafe
    if (eggsprite.getPosition().y < 0 || eggsprite.getPosition().y > 880) {
        ballVelocity.y = 0;
        ballVelocity.x = 0;
        eggsprite.setPosition(Vector2f(eggsprite.getPosition().x, 880));
    }
    /*
    if (degg.getPosition().x < 250 || degg.getPosition().x > 450) {
        degg.setPosition(Vector2f(350, 660));
    */
    if (devilsprite.getPosition().x < 350 || devilsprite.getPosition().x > 550) {
        devilsprite.setPosition(Vector2f(500, 710));

        if (degg2.getPosition().x < 350 || degg2.getPosition().x > 650) {
            degg2.setPosition(Vector2f(450, 500));
        }

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

        // Quit Via ESC Key
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }


        /*
        const float dx = degg.getPosition().x;
        const float dy = degg.getPosition().y;
        */

        const float dx = devilsprite.getPosition().x;
        const float dy = devilsprite.getPosition().y;

        const float d2x = degg2.getPosition().x;
        const float d2y = degg2.getPosition().y;

        //const float hx = hammer.getPosition().x;
        //const float hy = hammer.getPosition().y;

        // handle ball movement (horizontal)
        float direction = 0.0f;
        if (Keyboard::isKeyPressed(controls[1])) {
            direction--;
            player.setState(WALKING_LEFT);
        }
        else if (Keyboard::isKeyPressed(controls[2])) {
            direction++;
            player.setState(WALKING_RIGHT);
        }
        else
            player.setState(STANDING);

        eggsprite.move(Vector2f(direction * ballHorizontalSpeed * dt, 0.f));



        // Check Collision with platform

        const float bx = eggsprite.getPosition().x;
        //cout << bx << endl;
        const float by = eggsprite.getPosition().y;
        /*
        if (bx > gameWidth - ballRadius) {
            ballVelocity.x = -ballVelocity.x;
        }
        */
        if (by > gameHeight - ballRadius) { //bottom wall
            ballVelocity.y = 0.f;
            canJump = true;
        }
        else if (by < 0) { //top wall
            eggsprite.move(Vector2f(0.f, 10.f));
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
                pL = platform[i].getPosition().x - 600;
                pR = platform[i].getPosition().x + 600;
                pT = platform[i].getPosition().y - 10;
                pB = platform[i].getPosition().y + 10;
            }

            //GOAL HITBOX
            if (i == 6) {
                pL = platform[i].getPosition().x - 40;
                pR = platform[i].getPosition().x + 40;
                pT = platform[i].getPosition().y - 40;
                pB = platform[i].getPosition().y + 40;
            }

            if (i == 8) {
                pL = platform[i].getPosition().x - 150;
                pR = platform[i].getPosition().x + 150;
                pT = platform[i].getPosition().y - 10;
                pB = platform[i].getPosition().y + 10;
            }

            if (bx > pL && bx < pR && by >= pT - ballRadius && by < pB - ballRadius) {

                ballVelocity.y = 0.f;
                canJump = true;

                /*if (by <= pB + ballRadius && by > pT + ballRadius) {
                    ballVelocity.y = ballVelocity.y * -1;
                    eggsprite.setPosition(Vector2f(bx, by - 10.f));
                    cout << "hit bottom" << endl;
                }*/
                if (i == 4) {
                    cout << "hit lava" << endl;
                    GameOver();
                }

                if (i == 6) {
                    complete = true;
                    GameOver();
                }
                eggsprite.setPosition(Vector2f(bx, pT - ballRadius));
            }

            if ((bx + 20) > pL && (bx + 20) < pR && by <= pB + 1 && by > pT + 1) {
                ballVelocity.y = 0.f;
                eggsprite.setPosition(Vector2f(bx, by + 10.f));
                cout << "hit bottom" << endl;
            }

        }


        // handle ball jump
        if (canJump == true) {
            if (Keyboard::isKeyPressed(controls[0])) {
                //ball.move(Vector2f(0.f, ballJumpSpeed * dt));
                ballVelocity = { 0.f, ballJumpSpeed };
                jumpTime = 10;

                //jumpsound.play();
                player.setState(RISING);
            }
        }

        eggsprite.move(ballVelocity * dt);

        if (by > gameHeight) {
            //ball.move(Vector2f(0.f, -10.f));
            eggsprite.setPosition(Vector2f(bx, gameHeight - ballRadius));
        }

        if (dx > (27.5 + bx) && dx > 360) {
            //degg.move(Vector2f(-1 * deggHorizontalspeed * dt, 0.f));
            devilsprite.move(Vector2f(-1 * deggHorizontalspeed * dt, 0.f));
        }
        else if (dx < (27.5 + bx) && dx < 540) {
            //degg.move(Vector2f(1 * deggHorizontalspeed * dt, 0.f));
            devilsprite.move(Vector2f(1 * deggHorizontalspeed * dt, 0.f));
        }

        if (d2x > (27.5 + bx) && d2x > 360) {
            degg2.move(Vector2f(-1 * deggHorizontalspeed * dt, 0.f));
        }
        else if (d2x < (27.5 + bx) && d2x < 640) {
            degg2.move(Vector2f(1 * deggHorizontalspeed * dt, 0.f));
        }

        float eggCollideX1 = dx - (27.5 + bx);
        if (eggCollideX1 < 0) {
            eggCollideX1 = eggCollideX1 * -1;
        }

        float eggCollideY1 = dy - (31.25 + by);
        if (eggCollideY1 < 0) {
            eggCollideY1 = eggCollideY1 * -1;
        }

        float eggCollideX2 = d2x - (27.5 + bx);
        if (eggCollideX2 < 0) {
            eggCollideX2 = eggCollideX2 * -1;
        }

        float eggCollideY2 = d2y - (31.25 + by);
        if (eggCollideY2 < 0) {
            eggCollideY2 = eggCollideY2 * -1;
        }

        // Check for collision with either entity
        if ((eggCollideX1 <= (13.75 + 9) && eggCollideY1 <= (15.625 + 9)) ||
            (eggCollideX2 <= (13.75 + 9) && eggCollideY2 <= (15.625 + 9))) {
            GameOver();
        }
    }
}


void Render(RenderWindow& window) {
    // Draw Everything

    window.draw(background);
    for (int i = 0; i < platformArray -1; i++) {
        window.draw(platform[i]);
    }
    window.draw(goalsprite);
    window.draw(eggsprite);
    //window.draw(degg);
    window.draw(devilsprite);
    //window.draw(hammer);
    //window.draw(hammersprite);
    //window.draw(degg2);
    window.draw(fpstext);
    window.draw(gameOverText);
    window.draw(deathsText);

}

int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "EGG WITH LEGG");
    Load();

    window.setFramerateLimit(60);
    Clock clock;


    if (!backgroundTexture.loadFromFile("C:/Users/chris/ENU OneDrive/OneDrive - Edinburgh Napier University/Year 3/Modules/TR1/Games Engineering/Coursework/SET09121-Coursework/res/KitchenBackground.png"))
    {
        cout << "ERROR loading background" << endl;
    }
    else
    {
        background.setTexture(backgroundTexture);
        background.setScale(1.2f, 1.2f);
    }


    if (!spritesheet.loadFromFile("C:/Users/chris/ENU OneDrive/OneDrive - Edinburgh Napier University/Year 3/Modules/TR1/Games Engineering/Coursework/SET09121-Coursework/res/EggSpritesheet.png"))
    {
        cout << "ERROR loading spritesheet" << endl;
    }
    else
    {
        eggsprite.setTexture(spritesheet);
        //IntRect(1, 1, 110, 125)
        eggsprite.setTextureRect(eggSourceSprite);
        eggsprite.setScale(0.5f, 0.5f);
    }

    // Load in spritesheet for Deviled Egg
    if (!spritesheetdevil.loadFromFile("C:/Users/chris/ENU OneDrive/OneDrive - Edinburgh Napier University/Year 3/Modules/TR1/Games Engineering/Coursework/SET09121-Coursework/res/DevilSpritesheet.png"))
    {
        cout << "ERROR loading devil spritesheet" << endl;
    }
    else
    {
        devilsprite.setTexture(spritesheetdevil);
        //IntRect(1, 1, 110, 125)
        devilsprite.setTextureRect(devilSourceSprite);
        devilsprite.setScale(0.5f, 0.5f);
    }

    // Load in spritesheet for Goal
    if (!goalTexture.loadFromFile("C:/Users/chris/ENU OneDrive/OneDrive - Edinburgh Napier University/Year 3/Modules/TR1/Games Engineering/Coursework/SET09121-Coursework/res/GoalSprite.png"))
    {
        cout << "ERROR loading goal sprite" << endl;
    }
    else
    {
        goalsprite.setTexture(goalTexture);
    }

    // Load in spritesheet for Hammer
    if (!hammerTexture.loadFromFile("C:/Users/chris/ENU OneDrive/OneDrive - Edinburgh Napier University/Year 3/Modules/TR1/Games Engineering/Coursework/SET09121-Coursework/res/HammerSpritesheet.png"))
    {
        cout << "ERROR loading hammer sprite" << endl;
    }
    else
    {
        hammersprite.setTexture(hammerTexture);
        hammersprite.setTextureRect(hammerSourceSprite);
        hammersprite.setOrigin(1, 129); //203x129
        //hammersprite.setScale(0.8f, 0.8f);
    }

    /*
    if (!buffer.loadFromFile("C:/Users/angus/SET09121-Coursework/res/sound/jump.wav"))
    {
        cout << "ERROR loading jump sound!" << endl;
    }

    jumpsound.setBuffer(buffer);
    */
    

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

            //112 223

            window.clear();
            window.draw(eggsprite);
            window.draw(devilsprite);

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