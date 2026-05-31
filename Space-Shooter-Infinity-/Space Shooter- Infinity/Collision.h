#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Collision {

public:
    Sprite sprite;
    float duration = 0.1f;
    float timer = 0.f;
    bool active = true;

    Collision(Texture& texture,Vector2f position) {
        sprite.setTexture(texture);
        sprite.setScale(0.15f, 0.15f);
        sprite.setPosition(position);
        sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    }

    //  logic for a timer counting UP

    void update(float dt) {
        timer += dt;
        if (timer >= duration)
            active = false;
    }

    void draw(RenderWindow& window) {
        if (active)
            window.draw(sprite);
    }

};