#include <iostream>
#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
#include "misc.hpp"
#include "player.hpp"

using namespace sf;
using namespace std;

Texture spritesheet;
Sprite eggsprite;
Texture backgroundTexture;
Sprite background;
Texture spritesheetdevil;
Sprite devilsprite;
Texture spritesheetdevil2;
Sprite devil2sprite;
Texture goalTexture;
Sprite goalsprite;

IntRect eggSourceSprite(1, 1, 110, 125);
IntRect devilSourceSprite(223, 1, 110, 125); //grid 3
IntRect devil2SourceSprite(223, 1, 110, 125);

//SoundBuffer buffer;
//Sound jumpsound;

Font font;

int gameWidth = 1200;
int gameHeight = 900;

const Vector2f platformSize(200.f, 20.f);
const Vector2f groundSize(1200.f, 20.f);
const Vector2f goalHitbox(80.f, 20.f);
const Vector2f hammerHitbox(20.f, 20.f);

const float ballRadius = 60.f;
const float deggRadius = 10.f;

Vector2f ballVelocity;
Vector2f deggVelocity;

Clock animateClockDevil;

const float deggHorizontalspeed = 50.f;
const float degg2Horizontalspeed = 150.f;
const float ballHorizontalSpeed = 400.f;
const float ballJumpSpeed = -1200.f;
const float initialVelocityY = 80.f;

int jumpTime = 0;
int hangTime = 0;
int currentFrame = 0;
float dt;

Text fpstext;
Text gameOverText;
Text deathsText;
Time elapsedTime;

int deaths = 0;
int loops = 0;
int fps = 0.0f;

bool canJump = false;
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
// int platformArray = sizeof(platform) / sizeof(platform[0]); // Is there a reason to do this? We know it's always 12 - Ben

void Load() {
    // Load font-face from res dir
    font.loadFromFile("./res/font/RobotoMono-Regular.ttf");

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
    eggsprite.setPosition(100, static_cast<float>(gameHeight) / 2);

    degg.setRadius(deggRadius);
    degg.setOrigin(deggRadius, deggRadius);

    degg2.setRadius(deggRadius);
    degg2.setOrigin(deggRadius, deggRadius);


    // Reset positions

    //TEXT DISPLAYS
    gameOverText.setFillColor(Color::Black);
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

    //LONG PLATFORM
    platform[8].setSize(Vector2f(300, 20));
    //platform[8].setOrigin((platformSize.x / 2.f), (platformSize.y / 2.f));
    platform[8].setPosition(Vector2f(450.f, 520.f)); //degg
    //platform[8].setSize(Vector2f(300, 20));
    platform[8].setOrigin((300.f / 2.f), ( platformSize.y / 2.f));


    platform[9].setPosition(Vector2f(170.f, 450.f));
    platform[10].setPosition(Vector2f(450.f, 330.f));
    platform[11].setPosition(Vector2f(730.f, 250.f)); //goal

    //LAVA PLATFORM
    platform[4].setPosition(Vector2f(600.f, 890.f));

    //GOAL HITBOX
    goalsprite.setPosition(Vector2f(690.f, 198.f));
    platform[6].setPosition(Vector2f(730.f, 240.f));
    platform[6].setSize(Vector2f(80.f, 20.f));
    platform[6].setFillColor(Color::Cyan);
    platform[6].setOrigin(goalHitbox.x / 2, goalHitbox.y / 2);
    //goalsprite.setOrigin(goalHitbox.x / 2, goalHitbox.y / 2);

    //ENTITIES
    //ball.setPosition(Vector2f(50, 825));
    eggsprite.setPosition(Vector2f(50.f, 825.f));
    //eggsprite.setOrigin(Vector2f(55, 72.5));

    degg.setPosition(Vector2f(500.f, 710.f));
    degg.setFillColor(Color::Magenta);

    degg2.setPosition(Vector2f(450.f, 500.f));
    degg2.setFillColor(Color::Magenta);

    //hammer.setSize(Vector2f(hammerHitbox));
    //hammer.setOrigin(hammerHitbox.x / 2, hammerHitbox.y / 2);
    //hammer.setPosition(Vector2f(650.f, 560.f));
    //hammer.setFillColor(Color::Yellow);

    // Set Ball falling speed
    if (canJump == false) {
        ballVelocity = { 0, initialVelocityY };
    }

}

Player player;

void Reset(RenderWindow& window) {
    window.clear();
    Load();
    eggsprite.setPosition(Vector2f(50, 825));
    devilsprite.setPosition(Vector2f(500, 660));
    devil2sprite.setPosition(Vector2f(450, 450));
    degg.setPosition(Vector2f(500, 710));
    degg2.setPosition(Vector2f(450, 500));
    //Render(window);
}

void GameOver() {
    if (complete == true) {
        gameOverText.setCharacterSize(40);
        gameOverText.setFont(font);
        gameOverText.setFillColor(Color::Green);
        gameOverText.setString("LEVEL COMPLETE");
        gameOverText.setPosition((gameWidth * .5f) - (gameOverText.getLocalBounds().width * .5f), static_cast<float>(gameHeight) / 2);
        freeze = true;
    }
    else {
        gameOverText.setCharacterSize(40);
        gameOverText.setFont(font);
        gameOverText.setFillColor(Color::Red);
        gameOverText.setString("GAME OVER");
        deaths++;
        gameOverText.setPosition((gameWidth * .5f) - (gameOverText.getLocalBounds().width * .5f), static_cast<float>(gameHeight) / 2);
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

    //animateClockDevil;
    // grids 3,-9,7,10,8,-10,-7,9, back to 3 (I understand this is overcomplicated but I don't want to edit the png file)

    struct Frame {
        int top;
        int left;
        bool flipped; // Indicates whether the sprite should be flipped horizontally
    };

    player.updateState(eggsprite, eggSourceSprite);

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
        devil2sprite.setTextureRect(IntRect(frame.left, frame.top, 110, 125));
        //cout << "top = " << devilSourceSprite.top << ", left = " << devilSourceSprite.left << endl;

        // Handle flipping
        if (frame.flipped) {
            devilsprite.setScale(-0.5f, 0.5f);  // Flipped
            devilsprite.setOrigin(110, 0);   // Adjust origin to flip properly
            devil2sprite.setScale(-0.5f, 0.5f);  // Flipped
            devil2sprite.setOrigin(110, 0);   // Adjust origin to flip properly
        }
        else {
            devilsprite.setScale(0.5f, 0.5f); // Normal
            devilsprite.setOrigin(0, 0);
            devil2sprite.setScale(0.5f, 0.5f); // Normal
            devil2sprite.setOrigin(0, 0);
        }

        // Move to the next frame
        currentFrame = (currentFrame + 1) % devilFrames.size();
        animateClockDevil.restart();
    }



    //position failsafe
    if (eggsprite.getPosition().y < 0 || eggsprite.getPosition().y > 880) {
        ballVelocity.y = 0;
        ballVelocity.x = 0;
        eggsprite.setPosition(Vector2f(eggsprite.getPosition().x, 880));
    }

    if (degg.getPosition().x < 350 || degg.getPosition().x > 550) {
        degg.setPosition(Vector2f(500, 710));
    }

    if (devilsprite.getPosition().x < 250 || devilsprite.getPosition().x > 450) {
        devilsprite.setPosition(Vector2f(500, 660));
    }

    if (degg2.getPosition().x < 300 || degg2.getPosition().x > 600) {
        degg2.setPosition(Vector2f(450, 500));
    }

    if (devil2sprite.getPosition().x < 200 || devil2sprite.getPosition().x > 500) {
        devilsprite.setPosition(Vector2f(450, 450));
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
            if (hangTime % 4 == 0) {
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

    const float devilX = devilsprite.getPosition().x;
    //cout << devilX  << endl;
    const float devilY = devilsprite.getPosition().y;

    const float devil2X = devil2sprite.getPosition().x;
    //cout << devilX << endl;
    const float devil2Y = devil2sprite.getPosition().y;

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
    if (by > gameHeight - 62.5) { //bottom wall
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

    for (int i = 0; i < 12; i++) {
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

        if ((bx + 41.25) > pL && (bx + 13.75) < pR && by >= pT - ballRadius && by < pB - ballRadius) {

            ballVelocity.y = 0.f;
            //canJump = true;

            /*if (by <= pB + ballRadius && by > pT + ballRadius) {
                ballVelocity.y = ballVelocity.y * -1;
                eggsprite.setPosition(Vector2f(bx, by - 10.f));
                cout << "hit bottom" << endl;
            }*/
            if (i == 4) {
                GameOver();
            }

            if (i == 6) {
                complete = true;
                GameOver();
            }
            eggsprite.setPosition(Vector2f(bx, pT - ballRadius));

            canJump = true;
        }

        if ((bx + 20) > pL && (bx + 20) < pR && by <= pB + 1 && by > pT + 1) {
            ballVelocity.y = 0.f;
            eggsprite.setPosition(Vector2f(bx, by + 10.f));
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
        degg.move(Vector2f(-1 * degg2Horizontalspeed * dt, 0.f));
    }
    else if (dx < (27.5 + bx) && dx < 540) {
        degg.move(Vector2f(1 * degg2Horizontalspeed * dt, 0.f));
    }

    /*if (devilX >(27.5 + bx) && devilX > 360) {
        devilsprite.move(Vector2f(-1 * degg2Horizontalspeed * dt, 0.f));
    }
    else if (devilX < (27.5 + bx) && devilX < 540) {
        devilsprite.move(Vector2f(1 * degg2Horizontalspeed * dt, 0.f));
    }*/

    devilsprite.setPosition(dx -27.5, dy - 49);

    if (d2x > (27.5 + bx) && d2x > 310) {
        degg2.move(Vector2f(-1 * deggHorizontalspeed * dt, 0.f));
    }
    else if (d2x < (27.5 + bx) && d2x < 590) {
        degg2.move(Vector2f(1 * deggHorizontalspeed * dt, 0.f));
    }

    devil2sprite.setPosition(d2x - 27.5, d2y - 49);


    ///*
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
    //*/
}


void Render(RenderWindow& window) {
    // Draw Everything

    window.draw(background);
    for (int i = 0; i < 12; i++) {
        if (i != 6) {
            window.draw(platform[i]);
        }
    }
    window.draw(goalsprite);
    window.draw(eggsprite);
    //window.draw(degg);
    window.draw(devilsprite);
    //window.draw(degg2);
    window.draw(devil2sprite);
    //window.draw(hammer);
    window.draw(fpstext);
    window.draw(gameOverText);
    window.draw(deathsText);

}

int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "EGG WITH LEGG");
    Load();

    window.setFramerateLimit(60);
    Clock clock;


    if (!backgroundTexture.loadFromFile("./res/background/kitchen.png"))
    {
        cerr << __FILE__ << ":" << __LINE__ << ": ERROR: loading background " << endl;
    }
    else
    {
        background.setTexture(backgroundTexture);
        background.setScale(1.2f, 1.2f);
    }

    if (!spritesheet.loadFromFile("./res/sprite/player.png"))
    {
        cerr << __FILE__ << ":" << __LINE__ << ": ERROR: loading spritesheet" << endl;
    }
    else
    {
        eggsprite.setTexture(spritesheet);
        //IntRect(1, 1, 110, 125)
        eggsprite.setTextureRect(eggSourceSprite);
        eggsprite.setScale(0.5f, 0.5f);
    }

    if (!spritesheetdevil.loadFromFile("./res/sprite/enemy.png"))
    {
        cerr << __FILE__ << ":" << __LINE__ << ": ERROR: loading devil spritesheet" << endl;
    }
    else
    {
        devilsprite.setTexture(spritesheetdevil);
        //IntRect(1, 1, 110, 125)
        devilsprite.setTextureRect(devilSourceSprite);
        devilsprite.setScale(0.5f, 0.5f);
    }

    if (!spritesheetdevil2.loadFromFile("./res/sprite/enemy.png"))
    {
        cerr << __FILE__ << ":" << __LINE__ << ": ERROR loading devil spritesheet" << endl;
    }
    else
    {
        devil2sprite.setTexture(spritesheetdevil);
        //IntRect(1, 1, 110, 125)
        devil2sprite.setTextureRect(devil2SourceSprite);
        devil2sprite.setScale(0.5f, 0.5f);
    }

    if (!goalTexture.loadFromFile("./res/sprite/goal.png"))
    {
        cerr << __FILE__ << ":" << __LINE__ << ": ERROR: loading goal sprite" << endl;
    }
    else
    {
        goalsprite.setTexture(goalTexture);
    }


    /*
    if (!buffer.loadFromFile("C:/Users/angus/SET09121-Coursework/res/sound/jump.wav"))
        return -1;

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
            window.draw(devil2sprite);

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
