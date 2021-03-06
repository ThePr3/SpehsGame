#include "Main.h"
#include "Enemy.h"
#include "Game.h"
#include "Player.h"



Enemy::Enemy(sf::RenderWindow& windowref, Game* game) : Object(windowref, game)
{
}


Enemy::~Enemy()
{
}


bool Enemy::update()
{
	if (location.distanceFrom(centerObj->location) > DESPAWN_RANGE)
		return false;
	if (components.size() <= 0)
	{//All components destroyed, drop metal
		centerObj->dataPtr->money += metal;
		return false;
	}
	

	complexUpdate();
	updateComponents();


	if (nearestComponent != nullptr)
		distance = location.distanceFrom(nearestComponent->location);

	stationDistance = location.distanceFrom(mGame->nearestStationLocation);

	//if (nearestComponent != nullptr)
	//	playerDirection = location.angleTowardsRad(nearestComponent->location);
	if (nearestComponent != nullptr)
		playerDirection = -1 * atan2(nearestComponent->location.y - location.y, nearestComponent->location.x - location.x);
	if (playerDirection < 0)
		playerDirection = ((2 * PI) + playerDirection);

	
	//follow
	if (follow == true) //set angle to match playerdirection
	{
		if (angle >= 0 && angle < PI / 2) //1st quarter
		{
			if (playerDirection < PI && playerDirection > angle)
			{
				turnSpeed += turnAccelerationConstant;
			}
			else if (playerDirection < angle || playerDirection > PI*1.5)
			{
				turnSpeed -= turnAccelerationConstant;
			}
			else if (playerDirection < angle + PI)
			{
				turnSpeed += turnAccelerationConstant;
			}
			else
			{
				turnSpeed -= turnAccelerationConstant;
			}
		}
		else if (angle >= PI / 2 && angle < PI) //2nd quarter
		{
			if (playerDirection < PI*1.5 && playerDirection > angle)
			{
				turnSpeed += turnAccelerationConstant;
			}
			else if (playerDirection < angle)
			{
				turnSpeed -= turnAccelerationConstant;
			}
			else if (playerDirection > angle + PI)
			{
				turnSpeed -= turnAccelerationConstant;
			}
			else
			{
				turnSpeed += turnAccelerationConstant;
			}
		}
		else if (angle >= PI && angle < PI*1.5)//3rd quarter
		{
			if (playerDirection > angle)
			{
				turnSpeed += turnAccelerationConstant;
			}
			else if (playerDirection < angle && playerDirection > PI / 2)
			{
				turnSpeed -= turnAccelerationConstant;
			}
			else if (playerDirection < angle - PI)
			{
				turnSpeed += turnAccelerationConstant;
			}
			else
			{
				turnSpeed -= turnAccelerationConstant;
			}
		}
		else //4th quarter
		{
			if (playerDirection > angle || playerDirection < PI / 2)
			{
				turnSpeed += turnAccelerationConstant;
			}
			else if (playerDirection > PI && playerDirection < angle)
			{
				turnSpeed -= turnAccelerationConstant;
			}
			else if (playerDirection < angle - PI)
			{
				turnSpeed += turnAccelerationConstant;
			}
			else
			{
				turnSpeed -= turnAccelerationConstant;
			}
		}
	}
	else
	{
		if (turnSpeed > 0.001)
		{
			turnSpeed = turnSpeed*0.9;
		}
	}

	//negFollow
	if (negFollow == true) //flee
	{
		if (angle >= 0 && angle < PI / 2) //1st quarter
		{
			if (playerDirection < PI && playerDirection > angle)
			{
				turnSpeed -= turnAccelerationConstant * 4;
			}
			else if (playerDirection < angle || playerDirection > PI*1.5)
			{
				turnSpeed += turnAccelerationConstant * 4;
			}
			else if (playerDirection < angle + PI)
			{
				turnSpeed -= turnAccelerationConstant * 4;
			}
			else
			{
				turnSpeed += turnAccelerationConstant * 4;
			}
		}
		else if (angle >= PI / 2 && angle < PI) //2nd quarter
		{
			if (playerDirection < PI*1.5 && playerDirection > angle)
			{
				turnSpeed -= turnAccelerationConstant * 4;
			}
			else if (playerDirection < angle)
			{
				turnSpeed += turnAccelerationConstant * 4;
			}
			else if (playerDirection > angle + PI)
			{
				turnSpeed += turnAccelerationConstant * 4;
			}
			else
			{
				turnSpeed -= turnAccelerationConstant * 4;
			}
		}
		else if (angle >= PI && angle < PI*1.5)//3rd quarter
		{
			if (playerDirection > angle)
			{
				turnSpeed -= turnAccelerationConstant * 4;
			}
			else if (playerDirection < angle && playerDirection > PI / 2)
			{
				turnSpeed += turnAccelerationConstant * 4;
			}
			else if (playerDirection < angle - PI)
			{
				turnSpeed -= turnAccelerationConstant * 4;
			}
			else
			{
				turnSpeed += turnAccelerationConstant * 4;
			}
		}
		else //4th quarter
		{
			if (playerDirection > angle || playerDirection < PI / 2)
			{
				turnSpeed -= turnAccelerationConstant * 4;
			}
			else if (playerDirection > PI && playerDirection < angle)
			{
				turnSpeed += turnAccelerationConstant * 4;
			}
			else if (playerDirection < angle - PI)
			{
				turnSpeed -= turnAccelerationConstant * 4;
			}
			else
			{
				turnSpeed += turnAccelerationConstant * 4;
			}
		}
	}


	//limit turnSpeed
	if (turnSpeed > maxTurnSpeedLimit)
		turnSpeed = maxTurnSpeedLimit;
	else if (turnSpeed < -maxTurnSpeedLimit)
		turnSpeed = -maxTurnSpeedLimit;

	//limit speed
	if (this->xSpeed > maxSpeedLimit)
		this->xSpeed = maxSpeedLimit;
	else if (this->ySpeed > maxSpeedLimit)
		this->ySpeed = maxSpeedLimit;
	if (this->xSpeed < -maxSpeedLimit)
		this->xSpeed = -maxSpeedLimit;
	else if (this->ySpeed < -maxSpeedLimit)
		this->ySpeed = -maxSpeedLimit;

	return Object::update();
}


void Enemy::notifyComponentDestruction(Component* component)
{
	//Erase child id from it's parents' memory
	for (unsigned int i = 0; i < components.size(); i++)
		for (unsigned int c = 0; c < components[i]->childComponents.size(); c++)
			if (components[i]->childComponents[c] == component->id)
			{
				components[i]->childComponents.erase(components[i]->childComponents.begin() + c);
			}
}





void Enemy::complexUpdate()
{
	double tempDistance = 0;
	int tempIndex = -1;

	for (unsigned int i = 0; i < mGame->playerObj->components.size(); i++)
	{
		if (i == 0)
		{
			tempDistance = location.distanceFrom(centerObj->components[i]->location);
			tempIndex = i;
		}
		else if (tempDistance > location.distanceFrom(centerObj->components[i]->location))
		{
			tempDistance = location.distanceFrom(centerObj->components[i]->location);
			tempIndex = i;
		}
	}
	if (tempIndex >= 0)
	{
		nearestComponent = mGame->playerObj->components[tempIndex];
	}
	else
	{
		if (components.size() > 0)
			nearestComponent = components[0];
		else
			nearestComponent = nullptr;
	}
}


void Enemy::explosion(int dmg, float explosionRadius)
{
	if (components.size() < 1)
		return;//Prevents fatal vector out of range error happening (components[0])
	for (unsigned int i = 0; i < mGame->playerObj->components.size(); i++)
	{
		if ( location.distanceFrom(mGame->playerObj->components[i]->location) < this->components[0]->textureRadius * explosionRadius)
		{
			mGame->playerObj->components[i]->hp -= dmg;
		}
	}
}


void Enemy::shootLaser(int dmg)
{
	if (components.size() < 1)
		return;
	sf::VertexArray line(sf::Lines, 2);
	line[0].position = sf::Vector2f(this->screenX + resFactor*zoomFactor, this->screenY + resFactor*zoomFactor);
	line[1].position = sf::Vector2f(nearestComponent->screenX + (irandom(-15, 15))*resFactor*zoomFactor, nearestComponent->screenY + (irandom(-15, 15))*resFactor*zoomFactor);
	line[0].color = sf::Color::Red;
	line[1].color = sf::Color::Red;
	nearestComponent->hp -= dmg;

	mWindow.draw(line);
}