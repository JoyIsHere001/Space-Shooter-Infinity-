#pragma once
#include <SFML/Graphics.hpp>

class Enemy {

public:

    sf::Sprite sprite;
    sf::Vector2f velocity;
    bool alive = true;

    Enemy(sf::Texture& texture, sf::Vector2f position, sf::Vector2f vel);

    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getMuzzlePosition() const;
};
