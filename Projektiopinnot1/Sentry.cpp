#include "Main.h"
#include "Sentry.h"
#include "Game.h"
#include "Turret.h"



Sentry::Sentry(sf::RenderWindow& windowref, Game* game, int behaviourLevel) : Enemy(windowref, game)
{
	enemyBehaviourLevel = behaviourLevel;
	state = state_spawned;
	metal = irandom(10, 15) * ((enemyBehaviourLevel * 5 + 4) / 3);

	angle = playerDirection;
	dodging = false;
	repositioning = false;
	fleeing = false;
	aggroRange = 1900;
	maxActionRange = 1000;
	closeRange = 550;
	maxTurnSpeedLimit = 0.01;
	maxSpeedLimit = 5;
	accelerationConstant = 0.5;
	turnAccelerationConstant = 0.009;
	closeAngle = 0.003;

	//Reserve memory for all of the components
	components.reserve(2);
	components.push_back(new Component(this, mGame->playerObj, 0, 0));
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.enemyBaseGreenTex);
	components.back()->sprites.back().setOrigin(50, 50);
	components.push_back(new Turret(this, centerObj, 0, 0));
	components[components.size() - 2]->childComponents.push_back(components.back()->id);

	components.back()->hp = 70 + (enemyBehaviourLevel * 10);
	components.back()->maxHp = components.back()->hp;
}


Sentry::~Sentry()
{
}


bool Sentry::update()
{
	//When player is kill
	if (mGame->playerObj->components.size() == 0)
	{
		state = state_victory;
	}

	//Counters
	dodgeCounter--;
	if (dodgeCounter < 0)
	{
		dodging = false;
	}

	shootingCounter++;

	rotationCounter += irandom(0, 2);
	if (rotationCounter <= 40)
	{
		rotationDirection = true;
	}
	else if (rotationCounter > 40 && rotationCounter < 80)
	{
		rotationDirection = false;
	}
	else if (rotationCounter > 80 && rotationCounter < 140)
	{
		rotationDirection = irandom(0, 1);
	}
	else if (rotationCounter > 140)
	{
		rotationCounter = 0;
	}

	repositionCounter--;
	if (repositionCounter < 0)
	{
		repositioning = false;
	}


	AIupdate();
	if (components.size() > 0)
	{
		HPMemory = components[0]->hp;
	}
	memoryState = state;

	return Enemy::update();
}


void Sentry::AIupdate()
{
	//Special cases
	if (state == state_spawned)
	{
		//Enemy::update();
		//angle = (180 / PI)*playerDirection;

		xSpeed = (cos(2 * PI - angle))*(maxSpeedLimit / 3);
		ySpeed = (sin(2 * PI - angle))*(maxSpeedLimit / 3);
		state = state_passive;
		return;
	}
	if (state == state_victory)
	{
		follow = false;
		xSpeed += cos(angle)*accelerationConstant;
		ySpeed += sin(angle)*accelerationConstant;
		return;
	}


	if (dodging)
	{
		state = state_dodging;

		dodgeMove();
	}
	else if (repositioning)
	{
		state = state_repositioning;

		//
	}
	else if (fleeing)
	{
		state = state_fleeing;

		//
	}
	else if (distance < closeRange) //Close state
	{
		state = state_closeRange;

		follow = true;
		xSpeed += -(cos(2 * PI - angle))*accelerationConstant;
		ySpeed += -(sin(2 * PI - angle))*accelerationConstant;
	}
	else if (distance > closeRange && distance < maxActionRange) //Active state
	{
		if (stationDistance < 3000)
		{
			fleeing = true;
			return;
		}
		state = state_active;

		if (rotationDirection)
		{
			if (components.size() > 0)
			{
				if (HPMemory > this->components[0]->hp && follow == true)
				{
					follow = false;
					dodging = true;
					dodgeCounter = 25;
				}
			}
			else
			{
				follow = true;
				xSpeed += (-sin(angle))*accelerationConstant;
				ySpeed += (-cos(angle))*accelerationConstant;
			}
		}
		else if (!rotationDirection)
		{
			if (components.size() > 0)
			{
				if (HPMemory > this->components[0]->hp && follow == true)
				{
					follow = false;
					dodging = true;
					dodgeCounter = 25;
				}
			}
			else
			{
				follow = true;
				xSpeed += (sin(angle))*accelerationConstant;
				ySpeed += (cos(angle))*accelerationConstant;
			}
		}
		
		if (shootingCounter >= 20)
		{
			if (angle < playerDirection + closeAngle || angle > -playerDirection - closeRange)
			{
				for (unsigned int i = 0; i < components.size(); i++)
					if (components[i]->type == component::turret)
					{
						components[i]->fire();
					}
				shootingCounter = irandom(-25, -15);
			}
		}
	}
	else if (distance > maxActionRange && distance < aggroRange) //Detection state
	{
		if (stationDistance < 3000)
		{
			fleeing = true;
			return;
		}
		state = state_detected;

		follow = true;
		xSpeed += cos(2 * PI - angle)*accelerationConstant;
		ySpeed += sin(2 * PI - angle)*accelerationConstant;
	}
	else //Passive state
	{
		state = state_passive;

		follow = false;
		//Wandering around
		xSpeed += cos(2 * PI - angle)*accelerationConstant*irandom(0.9, 1.1);
		ySpeed += sin(2 * PI - angle)*accelerationConstant*irandom(0.9, 1.1);
		turnSpeed += irandom(-1, 1)*turnAccelerationConstant;
	}
}


void Sentry::dodgeMove()
{//work very much in progress over here

	follow = false;
	negFollow = true;
	xSpeed += (cos(2 * PI - angle))*accelerationConstant;
	ySpeed += (sin(2 * PI - angle))*accelerationConstant;

	//if (angle >= 0 && angle < PI / 2) //1st quarter
	//{
	//	if (rotationDirection)
	//	{
	//		turnSpeed += turnAccelerationConstant;
	//		xSpeed += (sin(angle))*accelerationConstant;
	//		ySpeed += (cos(angle))*accelerationConstant;
	//	}
	//	else if (!rotationDirection)
	//	{
	//		turnSpeed -= turnAccelerationConstant;
	//		xSpeed += (sin(angle))*accelerationConstant;
	//		ySpeed += (cos(angle))*accelerationConstant;
	//	}
	//}
	//else if (angle >= PI / 2 && angle < PI) //2nd quarter
	//{
	//	if (rotationDirection)
	//	{
	//		turnSpeed += turnAccelerationConstant;
	//		xSpeed += (sin(angle))*accelerationConstant;
	//		ySpeed += (-cos(angle))*accelerationConstant;
	//	}
	//	else if (!rotationDirection)
	//	{
	//		turnSpeed -= turnAccelerationConstant;
	//		xSpeed += (sin(angle))*accelerationConstant;
	//		ySpeed += (-cos(angle))*accelerationConstant;
	//	}
	//}
	//else if (angle >= PI && angle < PI*1.5)//3rd quarter
	//{
	//	if (rotationDirection)
	//	{
	//		turnSpeed += turnAccelerationConstant;
	//		xSpeed += (-sin(angle))*accelerationConstant;
	//		ySpeed += (-cos(angle))*accelerationConstant;
	//	}
	//	else if (!rotationDirection)
	//	{
	//		turnSpeed -= turnAccelerationConstant;
	//		xSpeed += (-sin(angle))*accelerationConstant;
	//		ySpeed += (-cos(angle))*accelerationConstant;
	//	}
	//}
	//else //4th quarter
	//{
	//	if (rotationDirection)
	//	{
	//		turnSpeed += turnAccelerationConstant;
	//		xSpeed += (-sin(angle))*accelerationConstant;
	//		ySpeed += (cos(angle))*accelerationConstant;
	//	}
	//	else if (!rotationDirection)
	//	{
	//		turnSpeed -= turnAccelerationConstant;
	//		xSpeed += (-sin(angle))*accelerationConstant;
	//		ySpeed += (cos(angle))*accelerationConstant;
	//	}
	//}
}


void Sentry::reposition()
{
	//TBD
}


void Sentry::flee()
{
	follow = false;
	negFollow = true;
	xSpeed += (cos(2 * PI - angle))*accelerationConstant;
	ySpeed += (sin(2 * PI - angle))*accelerationConstant;
}