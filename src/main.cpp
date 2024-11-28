#include <SFML/Graphics.hpp>


/*

        KNOWN ISSUES:

        if you jump while touching the edge of a lava platform, the player will respawn in the floor
        you can still jump out so its no big deal. Or the player will fall from the ceiling, idk how any of it happens.
        STATUS: FIXED - 26/11/2024

*/

using namespace sf;
using namespace std;

const Keyboard::Key controls[3] = {
    Keyboard::W,   // Player1 Up
    Keyboard::A,   // Player1 Left
    Keyboard::D,  // Player1 Right
};

const Vector2f platformSize(200.f, 20.f);
const Vector2f groundSize(1200.f, 20.f);
const Vector2f goalHitbox(20.f, 20.f);
int gameWidth = 1200;
int gameHeight = 900;
const float ballRadius = 10.f;
const float deggRadius = 10.f;
const Vector2f hammerHitbox(20.f, 20.f);
Vector2f ballVelocity;
Vector2f deggVelocity;
const float deggHorizontalspeed = 100.f;
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

// These are used in the state machine for the deviled egg, the flags are used to change the state of the entity
bool degg_IS_SEEKING = false;
bool degg_IS_WANDERING = true;

bool hasReached_Wall_left = false;
bool hasReached_Wall_right = true;


CircleShape ball;
CircleShape degg;
RectangleShape hammer;

RectangleShape platform[7];

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
    ball.setRadius(ballRadius);
    ball.setOrigin(ballRadius, ballRadius);
    ball.setPosition(100, gameHeight / 2);

    degg.setRadius(deggRadius);
    degg.setOrigin(deggRadius, deggRadius);


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
    platform[2].setPosition(Vector2f(350.f, 680.f));
    platform[3].setPosition(Vector2f(650.f, 680.f));
    platform[5].setPosition(Vector2f(950.f, 580.f));

    //LAVA PLATFORM
    platform[4].setPosition(Vector2f(600.f, 890.f));

    //GOAL HITBOX
    platform[6].setPosition(Vector2f(950.f, 560.f));
    platform[6].setSize(Vector2f(20, 20));
    platform[6].setFillColor(Color::Cyan);
    platform[6].setOrigin(goalHitbox.x / 2, goalHitbox.y / 2);

    //ENTITIES
    ball.setPosition(Vector2f(50, 825));

    degg.setPosition(Vector2f(350, 660));
    degg.setFillColor(Color::Magenta);

    hammer.setSize(Vector2f(hammerHitbox));
    hammer.setOrigin(hammerHitbox.x / 2, hammerHitbox.y / 2);
    hammer.setPosition(Vector2f(650.f, 560.f));
    hammer.setFillColor(Color::Yellow);

    // Set Ball falling speed
    if (canJump == false) {
        ballVelocity = { 0, initialVelocityY };
    }
}

void Reset(RenderWindow& window) {
    window.clear();
    Load();
    ball.setPosition(Vector2f(50, 825));
    degg.setPosition(Vector2f(350, 660));
    //Render(window);
}

void GameOver() {
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

    //position failsafe
    if (ball.getPosition().y < 0 || ball.getPosition().y > 880) {
        ballVelocity.y = 0;
        ball.setPosition(Vector2f(ball.getPosition().x, 880));
    }

    if (degg.getPosition().x < 250 || degg.getPosition().x > 450) {
        degg.setPosition(Vector2f(350, 660));
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



    const float dx = degg.getPosition().x;
    const float dy = degg.getPosition().y;

    const float hx = hammer.getPosition().x;
    const float hy = hammer.getPosition().y;

    // handle ball movement (horizontal)
    float direction = 0.0f;
    if (Keyboard::isKeyPressed(controls[1])) {
        direction--;
    }
    if (Keyboard::isKeyPressed(controls[2])) {
        direction++;
    }
    ball.move(Vector2f(direction * ballHorizontalSpeed * dt, 0.f));



    // Check Collision with platform

    const float bx = ball.getPosition().x;
    const float by = ball.getPosition().y;

    if (bx > gameWidth - ballRadius) {
        ballVelocity.x = -ballVelocity.x;
    }

    if (by > gameHeight - ballRadius) { //bottom wall
        ballVelocity.y = 0.f;
        canJump = true;
    }
    else if (by < 0) { //top wall
        ball.move(Vector2f(0.f, 10.f));
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
            pL = platform[i].getPosition().x - 10;
            pR = platform[i].getPosition().x + 10;
            pT = platform[i].getPosition().y - 10;
            pB = platform[i].getPosition().y + 10;
        }

        if (bx > pL && bx < pR && by >= pT - ballRadius && by < pB - ballRadius) {

            ballVelocity.y = 0.f;
            canJump = true;
            ball.setPosition(Vector2f(bx, pT - ballRadius));

            if (/*by < pB + ballRadius  &&*/ by > pB) {
                ballVelocity.y = 0.f;
                ball.move(Vector2f(0.f, 10.f));
            }
            if (i == 4) {
                GameOver();
            }

            if (i == 6) {
                complete = true;
                GameOver();
            }
        }
    }


    // handle ball jump
    if (canJump == true) {
        if (Keyboard::isKeyPressed(controls[0])) {
            ball.move(Vector2f(0.f, ballJumpSpeed * dt));
            ballVelocity = { 0.f, ballJumpSpeed };
            jumpTime = 10;
        }
    }

    ball.move(ballVelocity * dt);

    if (by > gameHeight) {
        //ball.move(Vector2f(0.f, -10.f));
        ball.setPosition(Vector2f(bx, gameHeight - ballRadius));
    }

    if (dx > bx && dx > 260) {
        degg.move(Vector2f(-1 * deggHorizontalspeed * dt, 0.f));
    }
    else if (dx < bx && dx < 440) {
        degg.move(Vector2f(1 * deggHorizontalspeed * dt, 0.f));
    }

    float eggCollideX = dx - bx;
    if (eggCollideX < 0) {
        eggCollideX = eggCollideX * -1;
    }

    float eggCollideY = dy - by;
    if (eggCollideY < 0) {
        eggCollideY = eggCollideY * -1;
    }


    if (eggCollideY <= 19) {
        if (eggCollideX <= 19) {
            GameOver();
        }
    }
}


void Render(RenderWindow& window) {
    // Draw Everything

    for (int i = 0; i < platformArray; i++) {
        window.draw(platform[i]);
    }
    window.draw(ball);
    window.draw(degg);
    window.draw(hammer);
    window.draw(fpstext);
    window.draw(gameOverText);
    window.draw(deathsText);

}

int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "EGG WITH LEGG");
    Load();

    window.setFramerateLimit(60);
    Clock clock;

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