#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

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

}

void Update(RenderWindow& window) {

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
}

void Render(RenderWindow& window) {
    // Draw Everything
    window.draw(platform[0]);
    window.draw(ball);
}

int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "EGG WITH LEGG");
    Load();

    while (window.isOpen()) {
        while (dt < 1 / 60) {}
        window.clear();
        Update(window);
        Render(window);
        window.display();
    }
    return 0;
}