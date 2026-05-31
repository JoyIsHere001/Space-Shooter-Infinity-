#include "SpaceShooter.h"
#include"Bullet.h"
#include<iostream>

using namespace std;
using namespace sf;

void SpaceShooter::inittexture()
{
	if (!this->texture.loadFromFile("C:/Users/js467/OneDrive/Documents/Programming/C++ programs/SFML projects/2D_Space_Shooter/assets/Art_assets/rocket.png")) 
	{
		cout << "ERROR:Failed to load texture file." <<endl;
	}

}

void SpaceShooter::initSprite() 
{

	this->sprite.setTexture(this->texture);

	this->sprite.scale(0.17f, 0.17f); //adjusting the image

	this->sprite.setPosition(400.0f, 500.0f);

}

// player constructor and destructors 

SpaceShooter::SpaceShooter() 
{

	this->movementSpeed = 300.0f;
	this->inittexture();
	this->initSprite();

    // bullet mechanism 

	this->bulletTexture.loadFromFile("C:/Users/js467/OneDrive/Documents/Programming/C++ programs/SFML projects/2D_Space_Shooter/assets/Art_assets/bullets.png");
	this->shootCoolDown = 0.3f;
	this->shootTimer = 0.0f;

}

 SpaceShooter:: ~SpaceShooter()
 {

	 // deleteing the objects ,shape and  texture 

 }

 void SpaceShooter::movement(float dirX, float dirY,float deltaTime)
 {
	 //player movement 

	 this->sprite.move(this->movementSpeed * dirX*deltaTime, this->movementSpeed * dirY * deltaTime);
 }

 void SpaceShooter::setPosition(const Vector2f& pos)
 {
	 sprite.setPosition(pos);
 }

 void SpaceShooter::update(float deltaTime , const RenderWindow& window) 
 {

	 // anims and boundaries ...

     // --- Movement ---

	 float dirX = 0.f;
	 float dirY = 0.f;


     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        

		 dirX -= 1.f;


     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    
		 dirX += 1.f;


     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
     

		 dirY -= 1.f;

     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
       
		 dirY += 1.f;


	 // move the sprite 

	 this->sprite.move(this->movementSpeed * dirX * deltaTime, this->movementSpeed * dirY * deltaTime);

     // --- Keep Player Inside Window ---

     Vector2u winSize = window.getSize();
     FloatRect bounds = this->sprite.getGlobalBounds();

     // Left boundary
     if (bounds.left < 0.f)
         this->sprite.setPosition(0.f, bounds.top);

	 else if (bounds.left + bounds.width > winSize.x)
		 this->sprite.setPosition(winSize.x - bounds.width, bounds.top);



     // Top boundary
     if (bounds.top < 0.f)
         this->sprite.setPosition(bounds.left, 0.f);

	 else if (bounds.top + bounds.height > winSize.y)
		 this->sprite.setPosition(bounds.left, winSize.y - bounds.height);

 
	 //update shoot timer
	 this->shootTimer += deltaTime;

	 //shooting bullets 

	 if(Keyboard::isKeyPressed(Keyboard::Space) && this->shootTimer >= this->shootCoolDown) 
	 {

		 float playerCenterX = this->sprite.getPosition().x + this->sprite.getGlobalBounds().width / 2.0f;
		 float playerTopY = this->sprite.getPosition().y -10.0f;
		 this->bullets.emplace_back(this->bulletTexture, playerCenterX - (bulletTexture.getSize().x * 0.10f) / 2.0f , playerTopY);
		 this->shootTimer = 0.0f;
	 }
	 //update all bullets 

	 for (size_t i = 0; i < this->bullets.size();) 
	 {

		 this->bullets[i].update(deltaTime);
		 if (this->bullets[i].isOffScreen(window)) {
			 this->bullets.erase(this->bullets.begin() + i);
		 }

		 else {

			 ++i;
		 }

	    }

	   }

 void SpaceShooter::render(RenderTarget& target) 
 {

	 for (auto& bullet : this->bullets)

	 {
		 bullet.render(target);
	 }

	 target.draw(this->sprite);

}

 const Vector2f SpaceShooter::getpos() const
 {
	 return this->sprite.getPosition(); 
	 
 }

