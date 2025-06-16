#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

class Cat {
public:  
    sf::Sprite sprite;
    sf::Clock timer;
    bool isVisible;
//Constructor 
    Cat(sf::Texture& texture, sf::Vector2f position) {
        sprite.setTexture(texture);
        sprite.setPosition(position);
        sprite.setScale(0.2f, 0.2f);
        isVisible = true;
        timer.restart();
    }
//erase cat
    void update() {
        if (timer.getElapsedTime().asSeconds() > 1.0f) {
            isVisible = false;
        }
    }
//new cat
    void reset(sf::Texture& texture, sf::Vector2f position) {
        sprite.setTexture(texture);
        sprite.setPosition(position);
        sprite.setScale(0.2f, 0.2f);
        isVisible = true;
        timer.restart();
    }
};

class Game {
private:
    sf::RenderWindow window;
    sf::Clock clock;
    sf::Time elapsed;
    sf::Font font;
    sf::Text timeText;
    sf::Text scoreText;
    sf::Color bgColor;
    Cat* currentCat;
    sf::Texture catTexture[3];
    int score;
    float timeLeft;
    bool isGameRunning;

public:
//Constructor
    Game() : window(sf::VideoMode(800, 600), "Kotek Game"), score(0), timeLeft(30.0f), isGameRunning(true), currentCat(nullptr) {
        window.setFramerateLimit(60);

        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Font loading failed!" << std::endl;
        }
//set 3 cats textures
        for (int i = 0; i < 3; i++) {
            if (!catTexture[i].loadFromFile("kot" + std::to_string(i + 1) + ".png")) {
                std::cerr << "Error loading texture!" << std::endl;
            }
        }
// time and score text
        timeText.setFont(font);
        timeText.setCharacterSize(24);
        timeText.setFillColor(sf::Color::White);
        timeText.setPosition(10, 10);

        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10, 40);
//background color
        bgColor = sf::Color(255, 105, 180);
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            if (isGameRunning) {
                update();
                render();
            }
        }
    }
//mouse clicks
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
//mouse clik at cat
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (currentCat != nullptr && currentCat->isVisible && currentCat->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    score++;
                    currentCat->isVisible = false;
                }
            }
        }
    }

    void update() {
        elapsed = clock.restart();

        timeLeft -= elapsed.asSeconds();
        if (timeLeft <= 0) {
            timeLeft = 0;
            isGameRunning = false;
        }
//cat erase
        if (currentCat != nullptr) {
            currentCat->update();
        }
//new cat
        if (currentCat == nullptr || !currentCat->isVisible) {
            spawnCat();
        }
//color change
        int r = (bgColor.r + 1) % 256;
        int g = (bgColor.g + 1) % 256;
        int b = (bgColor.b + 1) % 256;
        bgColor = sf::Color(r, g, b);
//time and score change
        timeText.setString("Time: " + std::to_string(static_cast<int>(timeLeft)));
        scoreText.setString("Score: " + std::to_string(score));
    }

    void render() {
        window.clear(bgColor);

        if (currentCat != nullptr && currentCat->isVisible) {
            window.draw(currentCat->sprite);
        }

        window.draw(timeText);
        window.draw(scoreText);

        window.display();
    }

    void spawnCat() {
        //random position without leaving frame
        sf::Vector2f position(rand() % (window.getSize().x - 100), rand() % (window.getSize().y - 100));
        //delete cat
        if (currentCat != nullptr) {
            delete currentCat;
        }
        currentCat = new Cat(catTexture[rand() % 3], position);
    }
};

int main() {
    std::srand(static_cast<unsigned int>(std::time(0)));
    Game game;
    game.run();
    return 0;
}
