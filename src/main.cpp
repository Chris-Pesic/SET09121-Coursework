#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

void Reset();

const Keyboard::Key controls[3] = {
    Keyboard::W,   // Player1 Up
    Keyboard::A,   // Player1 Left
    Keyboard::D,  // Player1 Right
};

const Vector2f platformSize(200.f, 20.f);
const Vector2f groundSize(800.f, 20.f);
const int gameWidth = 800;
const int gameHeight = 600;
const float ballRadius = 10.f;
Vector2f ballVelocity;
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
int loops = 0;
int fps = 0.0f;
Time elapsedTime;
bool freeze = false;


CircleShape ball;
RectangleShape platform[5];
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
    ball.setOrigin(ballRadius, ballRadius); //Should be half the ball width and height
    ball.setPosition(100, gameHeight/2);

    // Reset positions

    //GROUND PLATFORM
    platform[0].setSize(groundSize);
    platform[0].setPosition(Vector2f(400.f, 590.f));
    platform[0].setOrigin(groundSize.x / 2, groundSize.y / 2);
    
    //FLOATING PLATFORMS
    platform[1].setPosition(Vector2f(200.f, 480.f));
    platform[2].setPosition(Vector2f(300.f, 380.f));
    platform[3].setPosition(Vector2f(500.f,500.f));

    //LAVA PLATFORM
    platform[4].setPosition(Vector2f(600.f, 590.f));


    ball.setPosition(Vector2f(gameWidth / 2.f, gameHeight / 2.f));

    // Set Ball falling speed
    if (canJump == false) {
        ballVelocity = { 0, initialVelocityY };
    }



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

    // Reset Ball falling speed
    //ballVelocity = { 0, initialVelocityY };


        //if (ballVelocity.y < 1000.f) {
            //ballVelocity = { 0.f, ballVelocity.y + 1 };
        //}
    if (canJump == false){
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


  //ball.move(ballVelocity * dt);

  // Quit Via ESC Key
  if (Keyboard::isKeyPressed(Keyboard::Escape)) {
      window.close();
  }

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

  if (by > gameHeight - ballRadius) { //bottom wall
      // bottom wall
      // ballVelocity.x *= velocityMultiplier;
      ballVelocity.y = 0.f;
      //ball.move(Vector2f(0.f, -10.f));
      canJump = true;
  }
  else if (by < 0) { //top wall
      // top wall
      // ballVelocity.x *= velocityMultiplier;
      //ballVelocity.y *= 0;
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

      if (i == 0) {
          pL = platform[i].getPosition().x - 400;
          pR = platform[i].getPosition().x + 400;
          pT = platform[i].getPosition().y - 10;
          pB = platform[i].getPosition().y + 10;
      }

      if (bx > pL && bx < pR && by >= pT - ballRadius && by < pB - ballRadius) {
          //if (by > pT - ballRadius && by < pB - ballRadius) {
              ballVelocity.y = 0.f;
              canJump = true;
              ball.setPosition(Vector2f(bx, pT - ballRadius));
          //}
          /*else*/ if (by < pB + ballRadius && by > pB) {
              ballVelocity.y = 0.f;
              ball.move(Vector2f(0.f, 10.f));
          }
          if (i == 4) {
              window.clear(Color::Black);
              gameOverText.setCharacterSize(40);
              gameOverText.setFont(font);
              gameOverText.setColor(Color::Red);
              gameOverText.setString("GAME OVER");

              gameOverText.setPosition((gameWidth * .5f) - (gameOverText.getLocalBounds().width * .5f), gameHeight/2);
              freeze = true;
          }
      }
  }
  

  // handle ball jump
  if (canJump == true) {
      if (Keyboard::isKeyPressed(controls[0])) {
          //ball.move(Vector2f(0.f, ballJumpSpeed * dt));
          ballVelocity = { 0.f, ballJumpSpeed };
          jumpTime = 10;
      }
  }

  ball.move(ballVelocity * dt);

  if (by > gameHeight) {
      //ball.move(Vector2f(0.f, -10.f));
      ball.setPosition(Vector2f(bx, gameHeight - ballRadius));
  }


  
}

void Render(RenderWindow& window) {
    // Draw Everything
    for (int i = 0; i < platformArray; i++){
        window.draw(platform[i]);
    }
    window.draw(ball);
    window.draw(fpstext);
    window.draw(gameOverText);
}

int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "EGG WITH LEGG");
    Load();

    window.setFramerateLimit(60);
    Clock clock;

    while (window.isOpen()) {

        if (freeze != true) {
            fps = 0.f;
            loops = 0;
            // set the character size to 24 pixels
            fpstext.setCharacterSize(24);
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
            fpstext.setPosition((gameWidth * .5f) - (fpstext.getLocalBounds().width * .5f), 0);


            window.clear();

            Update(window);

            Render(window);

            window.display();

            loops++;
        }
    }
    return 0;
}