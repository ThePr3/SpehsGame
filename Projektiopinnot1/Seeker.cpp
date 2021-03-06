#include "Main.h"
#include "Seeker.h"
#include "Game.h"
#include "VisualEffect.h"


Seeker::Seeker(sf::RenderWindow& windowref, Game* game, int behaviourLevel) : Enemy(windowref, game)
{
	enemyBehaviourLevel = behaviourLevel;
	state = state_spawned;
	metal = irandom(5, 10) * ((enemyBehaviourLevel * 5 + 4) / 4);

	angle = playerDirection;
	explosionLimiter = false;
	dodging = false;
	fleeing = false;
	aggroRange = 1500 + enemyBehaviourLevel*2;
	maxActionRange = 700;
	closeRange = 150;
	maxTurnSpeedLimit = 0.03;
	maxSpeedLimit = 5;
	accelerationConstant = 0.25;
	turnAccelerationConstant = 0.003;
	closeAngle = 0.01;

	//Reserve memory for all of the components
	components.reserve(1);

	components.push_back(new Component(this, mGame->playerObj, 0, 0));
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.seekerTex);
	components.back()->sprites.back().setOrigin(50, 50);

	components.back()->hp = 80 + (enemyBehaviourLevel * 15);
	components.back()->maxHp = components.back()->hp;

	//Initialize Animations
	components[0]->animatedSprites.push_back(RM.seekerActiveTex);
	components[0]->animatedSprites.back().setVisibility(false);
	components[0]->animatedSprites.back().setOrigin(50, 50);
	components[0]->animatedSprites.back().setFrameSize(100, 100);
	components[0]->animatedSprites.back().setTilesetSize(4, 2);
	components[0]->animatedSprites.back().setFrameDuration(1);

	components[0]->animatedSprites.push_back(RM.seekerPassiveTex);
	components[0]->animatedSprites.back().setVisibility(false);
	components[0]->animatedSprites.back().setOrigin(50, 50);
	components[0]->animatedSprites.back().setFrameSize(100, 100);
	components[0]->animatedSprites.back().setTilesetSize(4, 2);
	components[0]->animatedSprites.back().setFrameDuration(1);

	components[0]->animatedSprites.push_back(RM.seekerDodgeTex);
	components[0]->animatedSprites.back().setVisibility(false);
	components[0]->animatedSprites.back().setOrigin(50, 50);
	components[0]->animatedSprites.back().setFrameSize(100, 100);
	components[0]->animatedSprites.back().setTilesetSize(5, 3);
	components[0]->animatedSprites.back().setFrameDuration(2);
}


Seeker::~Seeker()
{
}


bool Seeker::update()
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
	activationCounter--;
	
	AIupdate();
	if (components.size() > 0)
	{
		HPMemory = components[0]->hp;
	}
	memoryState = state;

	return Enemy::update();
}


void Seeker::AIupdate()//maybe not follow true all the time
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
		if (state != memoryState)
		{
			animationHandler(anim_dodge);
		}

		dodgeMove(xSpeed, ySpeed);
	}
	else if (fleeing)
	{
		state = state_fleeing;

		flee();
	}
	else if (distance < closeRange) //Close state
	{
		state = state_closeRange;
		if (state != memoryState)
		{
			animationHandler(anim_active);
		}

		follow = true;
		xSpeed += (cos(2 * PI - angle))*accelerationConstant;
		ySpeed += (sin(2 * PI - angle))*accelerationConstant;

		if (nearestComponent != nullptr)
		{
			if (distance < this->textureRadius + nearestComponent->textureRadius && explosionLimiter == false) //Contact
				{
					state = state_contact;
					if (state != memoryState)
					{
						//explosion animation
						mGame->frontVisualEffects.push_back(VisualEffect(RM.explosion1Tex, location.x, location.y, 1, 9));
						mGame->frontVisualEffects.back().setFrameSize(200, 200);
						mGame->frontVisualEffects.back().setTilesetSize(3, 3);
						mGame->frontVisualEffects.back().setOrigin(100, 100);
						mGame->frontVisualEffects.back().setColor(sf::Color(255, 255, 255, 180));
					}

					RM.explosionSound.play();
					explosion(50 * ((enemyBehaviourLevel + 6) / 6), 1.5);
					this->hp = 0;
					explosionLimiter = true;
				}
		}		
	}
	else if (distance > closeRange && distance < maxActionRange) //Active state
	{
		if (stationDistance < 3000)
		{
			fleeing = true;
			return;
		}
		state = state_active;
		if (state != memoryState)
		{
			animationHandler(anim_active);
		}

		follow = true;
		xSpeed += (cos(2 * PI - angle))*accelerationConstant;
		ySpeed += (sin(2 * PI - angle))*accelerationConstant;

		if (components.size() > 0)
		{
			if (HPMemory > this->components[0]->hp)
			{
				dodging = true;
				dodgeCounter = 40;
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
		if (activationCounter < 0)
			state = state_detected;
		if (state != memoryState)
		{	
			animationHandler(anim_active);	
			if (memoryState == state_passive)
			{
				state = state_activated;
				activationCounter = 40;//change accordingly!
				//activation animation
			}		
		}

		follow = true;
		xSpeed += cos(2 * PI - angle)*accelerationConstant;
		ySpeed += sin(2 * PI - angle)*accelerationConstant;
		if (components.size() > 0)
		{
			if (HPMemory > this->components[0]->hp)
			{
				dodging = true;
				dodgeCounter = 35;
			}
		}
	}
	else //Passive state
	{
		state = state_passive;
		if (state != memoryState)
		{
			animationHandler(anim_passive);
			//deactivation(dodge) animation?
		}

		follow = false;

		xSpeed += cos(2 * PI - angle)*accelerationConstant*irandom(0.9, 1.1);
		ySpeed += sin(2 * PI - angle)*accelerationConstant*irandom(0.9, 1.1);
		turnSpeed += irandom(-1, 1)*turnAccelerationConstant;
	}
}


void Seeker::dodgeMove(const double tempXSpeed, const double tempYSpeed)
{
	follow = false;

	if (xSpeed != -tempXSpeed)
	{
		if (tempXSpeed > 0)
			xSpeed -= accelerationConstant * 6*enemyBehaviourLevel;
		else if (tempXSpeed < 0)
			xSpeed += accelerationConstant * 4*enemyBehaviourLevel;
	}
	if (ySpeed != -tempYSpeed)
	{
		if (tempYSpeed > 0)
			ySpeed -= accelerationConstant * 4*enemyBehaviourLevel;
		else if (tempYSpeed < 0)
			ySpeed += accelerationConstant * 6*enemyBehaviourLevel;
	}
	turnSpeed += turnAccelerationConstant * enemyBehaviourLevel;
}


void Seeker::flee()
{
	follow = false;
	negFollow = true;
	xSpeed += (cos(2 * PI - angle))*accelerationConstant;
	ySpeed += (sin(2 * PI - angle))*accelerationConstant;
}


void Seeker::animationHandler(AnimationID ID)
{
	if (!(components.size() > 0))
		return;

	for (unsigned int i = 0; i < components[0]->animatedSprites.size(); i++)
	{
		components[0]->animatedSprites[i].setVisibility(false);
	}

	switch (ID)
	{
	case anim_active:
	{
		components[0]->animatedSprites[0].setVisibility(true);
		break;
	}
	case anim_passive:
	{
		components[0]->animatedSprites[1].setVisibility(true);
		break;
	}
	case anim_dodge:
	{
		components[0]->animatedSprites[2].setVisibility(true);
		break;
	}
	default:
		break;
	}
}