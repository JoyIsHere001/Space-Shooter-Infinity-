#include "HUD.h"
#include <iostream>

using namespace std;
using namespace sf;

HUD::HUD() 
{
  
    if (!font.loadFromFile("assets/Fonts/Orbitron-Regular.ttf")) {
        cout << "Failed to load font\n"<<endl;
    }

    highScore = 0;
    highScoreText.setFont(font);
    highScoreText.setCharacterSize(28);
    highScoreText.setFillColor(sf::Color::Yellow);
    highScoreText.setPosition(15.f, 130.f);

    scoreText.setFont(font);
    scoreText.setCharacterSize(40);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(15.f, 10.f);

    healthBarBg.setSize(sf::Vector2f(200.f, 20.f));
    healthBarBg.setFillColor(Color(50, 50, 50));
    healthBarBg.setPosition(15.f, 80.f);

    healthBar.setSize(Vector2f(200.f, 20.f));
    healthBar.setFillColor(Color::Green);
    healthBar.setPosition(15.f, 80.f);

    updateTexts();
}

void HUD::updateTexts() 
{
    scoreText.setString("Score:" +  to_string(score));
    highScoreText.setString("HIGHEST SCORE : " + std::to_string(highScore));

}

void HUD::update(float dt)
{
  
    healthPercent = std::max(0.f, std::min(1.f, healthPercent));

    healthBar.setSize(sf::Vector2f(200.f *healthPercent, 20.f));

    if (healthPercent > 0.5f)
        healthBar.setFillColor(sf::Color::Green);
    else if (healthPercent > 0.25f)
        healthBar.setFillColor(sf::Color::Yellow);
    else
        healthBar.setFillColor(sf::Color::Red);
}

void HUD::draw(sf::RenderWindow& window)
{
    window.draw(scoreText);
    window.draw(healthBarBg);
    window.draw(healthBar);
    window.draw(highScoreText);

}
