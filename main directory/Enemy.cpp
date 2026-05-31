#include "Enemy.h"

Enemy::Enemy(sf::Texture& texture, sf::Vector2f position, sf::Vector2f vel)
    : velocity(vel)
{
    sprite.setTexture(texture);
    sprite.setPosition(position);
    sprite.setScale(0.09f, 0.09f);
    
}

void Enemy::update(float dt)
{
    sprite.move(velocity * dt);
    if (sprite.getPosition().y > 1080.0f)
        alive = false;
}

void Enemy::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

sf::Vector2f Enemy::getMuzzlePosition() const
{
    sf::FloatRect bounds = sprite.getGlobalBounds();
    return sf::Vector2f(bounds.left + bounds.width / 2.f, bounds.top + bounds.height);
}
