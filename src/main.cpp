#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

void Reset();

const Keyboard::Key controls[3] = {
    Keyboard::W,   // Player1 Up
    Keyboard::A,   // Player1 Left
    Keyboard::D,  // Player1 Right
};

const Vector2f platformSize(800.f, 100.f);
const int gameWidth = 800;
const int gameHeight = 600;
const float ballRadius = 10.f;
Vector2f ballVelocity;
const float ballHorizontalSpeed = 400.f;
const float ballJumpSpeed = -3000.f;
const float initialVelocityY = 100.f;
bool canJump = false;
float dt;

CircleShape ball;
RectangleShape platform[1];

void Load() {

    // Set size and origin of platform
    //for (auto &p : platform) {
      //p.setSize(platformSize);
      //p.setOrigin(platformSize / 2.f);


    // Set size and origin of ball
    ball.setRadius(ballRadius);
    ball.setOrigin(ballRadius, ballRadius); //Should be half the ball width and height

    // Reset positions
    //platform[0].setPosition(Vector2f(platformSize.x / 2.f, gameHeight / 12.f));
    ball.setPosition(Vector2f(gameWidth / 2.f, gameHeight / 2.f));

    // Set Ball falling speed
    ballVelocity = { 0, initialVelocityY };

}

void Update(RenderWindow &window) {

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

  if (ballVelocity.y < 100.f) {
      ballVelocity = { 0.f, ballVelocity.y + 10 };
  }

  // Reset Jump validity
  canJump = false;

  
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
  
  if (by > gameHeight - 11) { //bottom wall
    // bottom wall
    // ballVelocity.x *= velocityMultiplier;
     ballVelocity.y *= 0.f;
    // ball.move(Vector2f(0.f, -10.f));
     canJump = true;
  } else if (by < 0) { //top wall
    // top wall
    // ballVelocity.x *= velocityMultiplier;
    //ballVelocity.y *= 0;
    ball.move(Vector2f(0.f, 10.f));
  }

  // handle ball jump
  if (canJump == true) {
      if (Keyboard::isKeyPressed(controls[0])) {
          //ball.move(Vector2f(0.f, ballJumpSpeed * dt));
          ballVelocity = { 0.f, ballJumpSpeed };
      }
  }
  
  ball.move(ballVelocity * dt);
  

  
}

  void Render(RenderWindow &window) {
    // Draw Everything
    window.draw(platform[0]);
    window.draw(ball);
  }

    int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "PONG");
    Load();
	//Reset();
    while (window.isOpen()) {
        while (dt < 1/60){}
        window.clear();
        Update(window);
        Render(window);
        window.display();
    }
    return 0;
}