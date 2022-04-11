#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <Windows.h>

struct BallOject {
    sf::CircleShape SfBall;
    float angle = 3.14f;
    float speed = 2.0f;
    float radius = 36.0f;
    int ID = 0;
};

sf::Clock GameClock;
std::vector<BallOject> BALLS;
int ID_counter = 1;
float radius = 36.0f;

void ComputePhysics() {
    while (true) {
        if (GameClock.getElapsedTime().asSeconds() >= 0.016f) {
            for (BallOject& Ball : BALLS) {
                for (BallOject& OtherBall : BALLS) {
                    if (OtherBall.ID != Ball.ID) {
                        sf::Vector2f Ball1Position = Ball.SfBall.getPosition();
                        sf::Vector2f Ball2Position = OtherBall.SfBall.getPosition();

                        float Distance = std::sqrtf(std::powf(Ball2Position.x - Ball1Position.x, 2) + std::powf(Ball2Position.y - Ball1Position.y, 2));
                        if (Distance <= Ball.radius + OtherBall.radius) {
                            float Angle1 = std::atan2f(-(Ball2Position.y - Ball1Position.y), -(Ball2Position.x - Ball1Position.x));
                            float Angle2 = std::atan2f(-(Ball1Position.y - Ball2Position.y), -(Ball1Position.x - Ball2Position.x));

                            Ball.angle = Angle1;
                            OtherBall.angle = Angle2;
                        }
                    }
                }

                Ball.SfBall.setPosition(Ball.SfBall.getPosition() + sf::Vector2f(std::cosf(Ball.angle) * Ball.speed, std::sinf(Ball.angle) * Ball.speed));

                if (Ball.SfBall.getPosition().x + Ball.radius >= 1280.0f || Ball.SfBall.getPosition().x - Ball.radius <= 0.0f) {
                    float y = std::sinf(Ball.angle);
                    float x = std::cosf(Ball.angle);
                    Ball.angle = std::atan2f(y, -x);
                }
                if (Ball.SfBall.getPosition().y + Ball.radius >= 720.0f || Ball.SfBall.getPosition().y - Ball.radius <= 0.0f) {
                    float y = std::sinf(Ball.angle);
                    float x = std::cosf(Ball.angle);
                    Ball.angle = std::atan2f(-y, x);
                }
            }

            GameClock.restart();
        }
    }
}

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    sf::ContextSettings RendererSettings;
    RendererSettings.antialiasingLevel = 8;

    sf::RenderWindow RendererWindow;
    RendererWindow.create(sf::VideoMode(1280, 720), "Balls", sf::Style::Titlebar | sf::Style::Close, RendererSettings);
    RendererWindow.setVerticalSyncEnabled(true);

    sf::CircleShape OutlineBall(radius);
    OutlineBall.setOrigin(radius, radius);
    OutlineBall.setOutlineThickness(1.0f);
    OutlineBall.setOutlineColor(sf::Color::Green);
    OutlineBall.setFillColor(sf::Color::Transparent);

    sf::Thread PhysicsThread(&ComputePhysics);
    PhysicsThread.launch();

    bool MousePressed = false;
    sf::Vector2f StartPosition;
    sf::Vector2f EndPosition;

    while (RendererWindow.isOpen()) {
        sf::Event GameEvent;

        if (MousePressed) {
            EndPosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(RendererWindow));
        }

        while (RendererWindow.pollEvent(GameEvent)) {
            if (GameEvent.type == sf::Event::MouseWheelScrolled) {
                radius = radius + GameEvent.mouseWheelScroll.delta * 2.0f;
                OutlineBall.setRadius(radius);
                OutlineBall.setOrigin(radius, radius);
            }

            if (GameEvent.type == sf::Event::MouseButtonPressed) {
                MousePressed = true;
                StartPosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(RendererWindow));
                EndPosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(RendererWindow));
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                BALLS.clear();
                ID_counter = 1;
            }

            if (GameEvent.type == sf::Event::MouseButtonReleased) {
                MousePressed = false;

                BallOject Newball;
                Newball.SfBall = sf::CircleShape(radius);
                Newball.SfBall.setPosition(StartPosition);
                Newball.SfBall.setOrigin(radius, radius);
                Newball.radius = radius;
                Newball.ID = ID_counter;
                Newball.angle = std::atan2(EndPosition.y - StartPosition.y, EndPosition.x - StartPosition.x);
                Newball.speed = (std::sqrtf(std::powf(EndPosition.x - StartPosition.x, 2) + std::powf(EndPosition.y - StartPosition.y, 2)) / 367.0f) * 10.0f;
                BALLS.push_back(Newball);
                ID_counter++;
            }

            if (GameEvent.type == sf::Event::Closed) {
                PhysicsThread.terminate();
                RendererWindow.close();

                return EXIT_SUCCESS;
            }
        }

        sf::Vector2f MousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(RendererWindow));
        OutlineBall.setPosition(MousePosition);

        RendererWindow.clear();

        for (BallOject& BALL : BALLS)
            RendererWindow.draw(BALL.SfBall);

        if (MousePressed) {
            sf::Vertex Line[2] = {
                sf::Vertex(StartPosition, sf::Color::Cyan),
                sf::Vertex(EndPosition, sf::Color::Cyan),
            };

            RendererWindow.draw(Line, 2, sf::Lines);
        }

        RendererWindow.draw(OutlineBall);
        RendererWindow.display();
    }

    return EXIT_SUCCESS;
}