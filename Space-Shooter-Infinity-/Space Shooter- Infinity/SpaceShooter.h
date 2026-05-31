#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include"Bullet.h"
#include<iostream>
#include<vector>

using namespace sf;
using namespace std;

/* Class for Spaceshooter as player */

class SpaceShooter
{

private:

// player variables 

	Sprite sprite;  
	Texture texture; 
    float movementSpeed; 


	//bullets and shooting 

	vector<Bullet> bullets;
	Texture bulletTexture;
	float shootCoolDown;
	float shootTimer;


// private functions
	void inittexture();
	void initSprite();

   public:

// constructors and destructors 

	SpaceShooter();
	virtual ~SpaceShooter();
		
	// functions

	void update(float deltatime, const RenderWindow& window);
	void render(RenderTarget& target);
	void movement(float dirX, float dirY,float deltaTime);

	// Accessor

vector<Bullet>& getBullets()
{ 
	return bullets; 
}

void setPosition(const sf::Vector2f& pos);
const Vector2f getpos() const;
const Sprite& getSprite() const { return this->sprite;}


};

