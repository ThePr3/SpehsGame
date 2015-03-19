#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Bullet.h"

#include <math.h>

Object::~Object()
{
}
Object::Object(sf::RenderWindow& windowref, Game* game) : mWindow(windowref)
{
	mGame = game;
	centerObj = game->playerObj;
	scale = 1;

	//Randomize x/y
	if (flipCoin())
		x = centerObj->x + irandom(-SPAWN_RANGE, SPAWN_RANGE);
	else
		x = centerObj->x + irandom(-SPAWN_RANGE, SPAWN_RANGE);
	if (flipCoin())
		y = centerObj->y + irandom(-SPAWN_RANGE, SPAWN_RANGE);
	else
		y = centerObj->y + irandom(-SPAWN_RANGE, SPAWN_RANGE);

	spr.setPosition(x, y);

}
Object::Object(sf::RenderWindow& windowref, Game* game, int cx, int cy) : mWindow(windowref)
{
	mGame = game;
	centerObj = mGame->playerObj;
	x = cx;
	y = cy;
}

bool Object::update()
{
	if (getDistance(x, y, centerObj->x, centerObj->y) > DESPAWN_RANGE || hp <= 0)
		return false;

	//update opacity
	if (opacity < 255)
	{
		opacity += 17;
		spr.setColor(sf::Color(255, 255, 255, opacity));
	}

	//Update variable values
	angle += turnSpeed;
	x += xSpeed;
	y += ySpeed;

	//Keep angle positi
	if (angle > 2*PI)
		angle -= 2*PI;
	else if (angle < 0)
		angle += 2*PI;

	//Relative coordinates for screen
	//+ left x right -
	//+ up y down -
	relativeXCoordinate = -(mGame->playerObj->x - this->x) + (WINDOW_WIDTH/2);
	relativeYCoordinate = -(mGame->playerObj->y - this->y) + (WINDOW_HEIGHT/2);

	//acceleration timer
	if (accTimer <= 1)
	{
		accTimer++;
	}
	else
		accTimer = 0;
	
	//acceleration
	if (accTimer == 1)
	{
		xSpeed0 = xSpeed;
		ySpeed0 = ySpeed;
	}
	xAcc = (xSpeed / xSpeed0);
	yAcc = (ySpeed / ySpeed0);
	

	xScreenDistance = (WINDOW_WIDTH / 2.5 - abs(scrSpeedX)) / (WINDOW_WIDTH / 2.5);
	yScreenDistance = (WINDOW_HEIGHT / 2.5 - abs(scrSpeedY)) / (WINDOW_WIDTH / 2.5);
	//scrSpeeds
	//Check if ship is accelerating, limit and set scrSpeed
	//X
		//NOT ACCELERATING
	if (centerObj->xAcc == 1 || centerObj->xAcc == -1)
	{
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
		{
			scrSpeedX = 0.99 * scrSpeedX;
			relativeSpeedX = 0.95 * relativeSpeedX;
		}
	}
		//ACCELERATING
	else
	{
		scrSpeedX += ((relativeSpeedX * abs(relativeSpeedX)) * (xScreenDistance*0.2))*zoomFactor;
	}


	//Y
		//NOT ACCELERATING
	if (centerObj->yAcc == 1 || centerObj->yAcc == -1)
	{
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
		{
			scrSpeedY = 0.99 * scrSpeedY;
			relativeSpeedY = 0.95 * relativeSpeedY;
		}
	}
		//ACCELERATING
	else
	{
		scrSpeedY += ((relativeSpeedY * abs(relativeSpeedY)) * (yScreenDistance*0.2))*zoomFactor;
	}
	

	//Update screen positions
	if (centerObj != this) //If the object is not the player
	{
		screenX = centerObj->screenX + resFactor*zoomFactor*(x - centerObj->x);
		screenY = centerObj->screenY + resFactor*zoomFactor*(y - centerObj->y);
	}
	else
	{//Player update, make the player object appear in the center of the screen
		screenX = WINDOW_WIDTH / 2 - (scrSpeedX);
		screenY = WINDOW_HEIGHT / 2 - (scrSpeedY);
	}

	//Apply variables
	spr.setPosition(screenX, screenY);
	spr.setScale(scale*resFactor*zoomFactor, scale*resFactor*zoomFactor);
	spr.setRotation(360 - (angle/PI)*180);
	return true;
}

void Object::draw()
{
	mWindow.draw(spr);
}

void Object::checkCollisions(unsigned int selfIndex)
{
	for (unsigned int i = 0; i < mGame->objects.size(); i++)
		if (i != selfIndex)
		{
			collisionCheckAngle = -1 * atan2(mGame->objects[i]->y - y, mGame->objects[i]->x - x);
			if (collisionCheckAngle < 0)
				collisionCheckAngle = ((2 * PI) + collisionCheckAngle);


			checkCollisionDistance = getDistance(x, y, mGame->objects[i]->x, mGame->objects[i]->y);
			checkCollisionRange = textureRadius + mGame->objects[i]->textureRadius;

			if (checkCollisionDistance < checkCollisionRange)
			{
				double ys;
				if (ySpeed > 0)
					ys = 1 + ySpeed;
				else
					ys = 1 - ySpeed;
				ySpeed += 0.001*sin(collisionCheckAngle)*ys;
				y += 3 * sin(collisionCheckAngle);

				double xs;
				if (xSpeed > 0)
					xs = 1 - xSpeed;
				else
					xs = 1 + xSpeed;
				xSpeed += 0.001*cos(collisionCheckAngle)*xs;
				x += -3 * cos(collisionCheckAngle);
			}
		}
}


void Object::checkBulletCollision(Bullet* b)
{
	b->collisionCheckAngle = -1 * atan2(y - b->y, x - b->x);
	if (b->collisionCheckAngle < 0)
		b->collisionCheckAngle = ((2 * PI) + b->collisionCheckAngle);


	b->checkCollisionDistance = getDistance(b->x, b->y, x, y);
	b->checkCollisionRange = b->textureRadius + textureRadius;

	if (b->checkCollisionDistance < b->checkCollisionRange)
	{
		if (b->canDamage == true)
		{
			hp -= b->damage;
			b->canDamage = false;
			x += 6 * cos(angle);
			y += -6 * sin(angle);
		}

		b->speed = b->speed*0.75;

		b->angle = PI / 2 + (irandom(0, 180) / double(180))*PI;
		b->xSpeed = cos(2 * PI - b->angle) * b->speed;
		b->ySpeed = sin(2 * PI - b->angle) * b->speed;
	}
}