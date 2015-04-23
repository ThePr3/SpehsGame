#include "Enemy.h"
#include "Sentry.h"


Sentry::Sentry(sf::RenderWindow&, Game*, std::vector<Object*>&, int behaviourLevel) : refVector(rVector), Object(windowref, game)
{
	enemyBehaviourLevel = behaviourLevel;

	dodging = false;
	repositioning = false;
	fleeing = false;
	aggroRange = SPAWN_RANGE;
	maxActionRange = 1000;
	closeRange = 500;
	maxTurnSpeedLimit = 0.014;
	maxSpeedLimit = 3;
	accelerationConstant;//?
	accelerationConstant;//?
	closeAngle;//set

	components.push_back(new Component(this, mGame->playerObj, 0, 0));
	components[components.size() - 1]->sprites.push_back(sf::Sprite());
	components[components.size() - 1]->sprites[components[components.size() - 1]->sprites.size() - 1].setTexture(RM.getTexture("enemy_base_green.png"));
	components[components.size() - 1]->sprites[components[components.size() - 1]->sprites.size() - 1].setOrigin(50, 50);
	components.push_back(new Turret(this, centerObj, 0, 0));
	components[components.size() - 2]->childComponents.push_back(components[components.size() - 1]->id);
}


Sentry::~Sentry()
{
}


bool Sentry::update()
{
	//Counters
	dodgeCounter++;
	shootingCounter++;
	repositionCounter++;
		//Behaviour counter!!
		//RotationDirection changes...


	AIupdate();
	HPMemory = components[0]->hp;

	return Enemy::update();
}


void Sentry::AIupdate()
{
	if (dodging)
	{
		dodgeMove();
	}
	else if (repositioning)
	{
		//
	}
	else if (fleeing)
	{
		//
	}
	else if (distance < closeRange) //Close state
	{
		follow = true;
		xSpeed = -(cos(2 * PI - angle))*accelerationConstant;
		ySpeed = -(sin(2 * PI - angle))*accelerationConstant;
	}
	else if (distance > closeRange && distance < maxActionRange) //Active state
	{
		if (tempHPMemory > this->components[0]->hp)
		{
			follow = false;
			dodging = true;
		}
		else
		{
			follow = true;
			xSpeed += (cos(2 * PI - angle))*accelerationConstant;
			ySpeed += (sin(2 * PI - angle))*accelerationConstant;
		}
		if (shootingCounter >= 10)
		{
			if (angle < playerDirection + closeAngle || angle > -playerDirection - closeRange)
			{
				for (unsigned int i = 0; i < components.size(); i++)
					for (unsigned int k = 0; k < components[i]->types.size(); k++)
						if (components[i]->types[k] == component::turret)
						{
							components[i]->fire();
						}
				shootingCounter = irandom(-25, -15);
			}
		}
	}
	else if (distance > maxActionRange && distance < aggroRange) //Detection state
	{
		follow = true;
		xSpeed = cos(2 * PI - angle)*accelerationConstant;
		ySpeed = sin(2 * PI - angle)*accelerationConstant;
	}
	else //Passive state
	{
		follow = false;
		xSpeed = xSpeed*0.96;
		ySpeed = ySpeed*0.96;
		if (xSpeed > -0.01 && xSpeed < 0.01)
		{
			xSpeed = 0;
		}
		if (ySpeed > -0.01 && ySpeed < 0.01)
		{
			ySpeed = 0;
		}
	}
}


void Sentry::dodgeMove()
{//work very much in progress over here
	if (angle >= 0 && angle < PI / 2) //1st quarter
	{
		if (rotationDirection)
		{
			turnSpeed += turnAccelerationConstant;
			xSpeed = (sin(angle))*accelerationConstant;
			ySpeed = (cos(angle))*accelerationConstant;
		}
		else if (!rotationDirection)
		{
			turnSpeed -= turnAccelerationConstant;
			xSpeed = (sin(angle))*accelerationConstant;
			ySpeed = (cos(angle))*accelerationConstant;
		}
	}
	else if (angle >= PI / 2 && angle < PI) //2nd quarter
	{
		if (rotationDirection)
		{
			turnSpeed += turnAccelerationConstant;
			xSpeed = (sin(angle))*accelerationConstant;
			ySpeed = (-cos(angle))*accelerationConstant;
		}
		else if (!rotationDirection)
		{
			turnSpeed -= turnAccelerationConstant;
			xSpeed = (sin(angle))*accelerationConstant;
			ySpeed = (-cos(angle))*accelerationConstant;
		}
	}
	else if (angle >= PI && angle < PI*1.5)//3rd quarter
	{
		if (rotationDirection)
		{
			turnSpeed += turnAccelerationConstant;
			xSpeed = (-sin(angle))*accelerationConstant;
			ySpeed = (-cos(angle))*accelerationConstant;
		}
		else if (!rotationDirection)
		{
			turnSpeed -= turnAccelerationConstant;
			xSpeed = (-sin(angle))*accelerationConstant;
			ySpeed = (-cos(angle))*accelerationConstant;
		}
	}
	else //4th quarter
	{
		if (rotationDirection)
		{
			turnSpeed += turnAccelerationConstant;
			xSpeed = (-sin(angle))*accelerationConstant;
			ySpeed = (cos(angle))*accelerationConstant;
		}
		else if (!rotationDirection)
		{
			turnSpeed -= turnAccelerationConstant;
			xSpeed = (-sin(angle))*accelerationConstant;
			ySpeed = (cos(angle))*accelerationConstant;
		}
	}
}


void Sentry::reposition()
{
	//TBD
}


void Sentry::flee()
{
	//TBD
}