#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class HUD {

public:
    int highScore;
    int score = 0;
    float healthPercent = 1.0f;
    sf::Font font;
    sf::Text scoreText;
    sf::RectangleShape healthBarBg, healthBar;
    sf::Text highScoreText;

    HUD();

    void updateTexts();
    void update(float dt);
    void draw(sf::RenderWindow& window);

    void reset() {
        score = 0;
        healthPercent = 1.0f;
        updateTexts();
        healthBar.setSize(sf::Vector2f(200.f * healthPercent, 20.f));
        healthBar.setFillColor(sf::Color::Green);
    }
};