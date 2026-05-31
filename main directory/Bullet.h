#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Bullet
{

    private:

    // variables
    Sprite shape;
    float speed;

    public:

    Bullet(const Texture& tex, float startX, float startY)
    {
        this->shape.setTexture(tex);
        this->shape.setScale(0.10f, 0.10f);
        this->shape.setPosition(startX, startY);
        this->speed = -500.0f;
    }

    void update(float deltaTime)
    {
        this->shape.move(0.0f, this->speed * deltaTime);
    }

  
    void render(RenderTarget& target) 
    {
        target.draw(this->shape);
    }

    Vector2f getPosition() const
    {
        return shape.getPosition();
    }

    bool isOffScreen(const RenderWindow& window) const
    {
        return (this->shape.getPosition().y + this->shape.getGlobalBounds().height < 0);
    }

    const FloatRect getBounds() const 
    {
        return this->shape.getGlobalBounds();
    }
};

