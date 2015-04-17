#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Player.h"
#include "Component.h"
#include "Bullet.h"
#include "Turret.h"
#include "GridData.h"
#include "PlayerData.h"
#include "ShipEditor.h"


Player::~Player()
{//Player constructor CANNOT BE CALLED FROM Game.cpp! Game's objects's vector calls for object destructor instead...
	//std::cout << "\nBegin player DECONSTRUCTOR";
}
Player::Player(sf::RenderWindow& windowref, Game* game, int cx, int cy) : Object(windowref, game, cx, cy)
{
	centerObj = this;

	data = new PlayerData;
	loadPlayerData();
	loadKeybindings();
	massCenterX = 0;
	massCenterY = 0;


	//Initialise core key bindings
	MyKeys key;
	//Keyboard setup

	//Controller setup
	key.inputType = keyboardInput;
	key.keyCode = sf::Keyboard::D;
	coreKeys[key_turnRight] = key;

	key.inputType = keyboardInput;
	key.keyCode = sf::Keyboard::A;
	coreKeys[key_turnLeft] = key;

	key.inputType = keyboardInput;
	key.keyCode = sf::Keyboard::S;
	coreKeys[key_reverse] = key;

	key.inputType = keyboardInput;
	key.keyCode = sf::Keyboard::W;
	coreKeys[key_accelerate] = key;

	key.inputType = keyboardInput;
	key.keyCode = sf::Keyboard::Period;
	coreKeys[key_zoomIn] = key;

	key.inputType = keyboardInput;
	key.keyCode = sf::Keyboard::Comma;
	coreKeys[key_zoomOut] = key;



	//Controller setup
	//key.inputType = joystickInput;
	//key.axisType = positiveAxis;
	//key.joystickIndex = 0;
	//key.joystickAxis = sf::Joystick::X;
	//Keys[key_turnRight] = key;

	//key.inputType = joystickInput;
	//key.axisType = negativeAxis;
	//key.joystickIndex = 0;
	//key.joystickAxis = sf::Joystick::X;
	//Keys[key_turnLeft] = key;

	//key.inputType = joystickInput;
	//key.axisType = positiveAxis;
	//key.joystickIndex = 0;
	//key.joystickAxis = sf::Joystick::Z;
	//Keys[key_reverse] = key;

	//key.inputType = joystickInput;
	//key.axisType = negativeAxis;
	//key.joystickIndex = 0;
	//key.joystickAxis = sf::Joystick::Z;
	//Keys[key_accelerate] = key;

	//key.inputType = joystickInput;
	//key.axisType = noAxis;
	//key.joystickIndex = 0;
	//key.joystickButton = 7;
	//Keys[key_zoomIn] = key;

	//key.inputType = joystickInput;
	//key.axisType = noAxis;
	//key.joystickIndex = 0;
	//key.joystickButton = 6;
	//Keys[key_zoomOut] = key;

	////Handle turrets
	//int turretCount = 0;
	//for (unsigned int i = 0; i < components.size(); i++)
	//	for (unsigned int k = 0; k < components[i]->types.size(); k++)
	//		if (components[i]->types[k] == turret)
	//			turretCount++;	
	//for (int i = 0; i < turretCount; i++)
	//{//Turret rotation
	//	key.inputType = joystickInput;
	//	key.axisType = noAxis;
	//	key.joystickIndex = 0;
	//	key.joystickButton = 4;
	//	Keys[i+1] = key;
	//	key.joystickButton = 5;
	//	Keys[-1 * (i + 1)] = key;
	//	//Turret fire
	//	key.inputType = joystickInput;
	//	key.axisType = noAxis;
	//	key.joystickIndex = 0;
	//	key.joystickButton = 0;
	//	Keys[i + 1 + 0.1] = key;
	//}

}

bool Player::update()
{
	//Update mousePosition
	mousePosition = sf::Mouse::getPosition(mWindow);
	mWindow.pollEvent(mEvent);


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		xSpeed = 0;
		ySpeed = 0;
		turnSpeed = 0;

		relativeSpeedX = 0;
		relativeSpeedY = 0;
	}


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
		editShip();

	if (directionalMovement == false)
	{
		//Accelerate
		if (testInput(coreKeys[key_accelerate], mGame->mEvent))
			if (coreKeys[key_accelerate].axisType != noAxis)
				accelerate(abs(sf::Joystick::getAxisPosition(coreKeys[key_accelerate].joystickIndex, coreKeys[key_accelerate].joystickAxis)));
			else
				accelerate(100);
		//Turn right
		if (testInput(coreKeys[key_turnRight], mGame->mEvent))
			if (coreKeys[key_turnRight].axisType != noAxis)
				turnRight(abs(sf::Joystick::getAxisPosition(coreKeys[key_turnRight].joystickIndex, coreKeys[key_turnRight].joystickAxis)));
			else
				turnRight(100);
		//Turn left
		if (testInput(coreKeys[key_turnLeft], mGame->mEvent))
			if (coreKeys[key_turnLeft].axisType != noAxis)
				turnLeft(abs(sf::Joystick::getAxisPosition(coreKeys[key_turnLeft].joystickIndex, coreKeys[key_turnLeft].joystickAxis)));
			else
				turnLeft(100);
	}
	else if (abs(sf::Joystick::getAxisPosition(moveJoystickId, verticalMoveAxis)) + abs(sf::Joystick::getAxisPosition(moveJoystickId, horizontalMoveAxis)) > 15)
	{//Detect directional movement
		joystickDirection = -1 * atan2(verticalMoveAxisPolarity*sf::Joystick::getAxisPosition(moveJoystickId, verticalMoveAxis), horizontalMoveAxisPolarity*sf::Joystick::getAxisPosition(moveJoystickId, horizontalMoveAxis));
		if (joystickDirection < 0)
			joystickDirection = 2 * PI + joystickDirection;

		//Detect acceleration
		temp_accelerationPower = 100*(pow(sf::Joystick::getAxisPosition(moveJoystickId, verticalMoveAxis)/100, 2) + pow(sf::Joystick::getAxisPosition(moveJoystickId, horizontalMoveAxis)/100, 2));
		if (temp_accelerationPower > 10)
		{
			if (temp_angleVar < PI / 2)
			{
				if (temp_accelerationPower > 100)
					temp_accelerationPower = 100;
				accelerate(temp_accelerationPower);

			}
		}

		//Rotation
		temp_angleVar = angle - joystickDirection;
		if (abs(temp_angleVar) < PI / 18)
		{
			turnSpeed = turnSpeed*0.90;
		}
		else if (temp_angleVar < 0)
		{//-
			if (joystickDirection < angle + PI)
			{//1st L
				turnLeft(100);
			}
			else
			{//4th R
				turnRight(100);
			}
		}//-
		else
		{//+
			if (joystickDirection < angle - PI)
			{//2nd L
				turnLeft(100);
			}
			else
			{//3rd R
				turnRight(100);
			}
		}//+
	}
	else
	{//DirectionalMovement == true && no input -> slow down rotation
		if (turnSpeed > 100 * rotationSpeed*(10.0 / (10 + shipMass)))
			turnRight(100);
		else if (turnSpeed < -100 * rotationSpeed*(10.0 / (10 + shipMass)))
			turnLeft(100);
	}

	//Reverse
	if (testInput(coreKeys[key_reverse], mGame->mEvent))
		if (coreKeys[key_reverse].axisType != noAxis)
			reverse(abs(sf::Joystick::getAxisPosition(coreKeys[key_reverse].joystickIndex, coreKeys[key_reverse].joystickAxis)));
		else
			reverse(100);

		//Zoom in
	if (testInput(coreKeys[key_zoomIn], mGame->mEvent))
		{
		if (coreKeys[key_zoomIn].axisType != noAxis)
				zoomIn(abs(sf::Joystick::getAxisPosition(coreKeys[key_zoomIn].joystickIndex, coreKeys[key_zoomIn].joystickAxis)));
			else
				zoomIn(100);
		}
		//Zoom out
	if (testInput(coreKeys[key_zoomOut], mGame->mEvent))
		{
		if (coreKeys[key_zoomOut].axisType != noAxis)
			zoomOut(abs(sf::Joystick::getAxisPosition(coreKeys[key_zoomOut].joystickIndex, coreKeys[key_zoomOut].joystickAxis)));
			else
				zoomOut(100);
		}

		//Fire/reload turrets
		{// <-need a scope for turretCount...
			int turretCount = 0;
			for (unsigned int i = 0; i < components.size(); i++)
				for (unsigned int k = 0; k < components[i]->types.size(); k++)
				{
				if (components[i]->types[k] == component::turret)
				{
					turretCount++;
					if (testInput(componentKeys[components[i]->gridLocationX + components[i]->gridLocationY*0.001 + KEYB_fire], mGame->mEvent))
					{
						if (data->grid[components[i]->gridLocationX][components[i]->gridLocationY]->holdToFire == true ||
							(data->grid[components[i]->gridLocationX][components[i]->gridLocationY]->holdToFire == false && components[i]->hasFired == false))
							components[i]->fire();
					}
					else
						components[i]->hasFired = false;
				}
				if (testInput(componentKeys[components[i]->gridLocationX + components[i]->gridLocationY*0.001 + KEYB_reload], mGame->mEvent))
					if (components[i]->reloading == false)
						components[i]->reload(); 				
				}
		}//End of fire/reload turrets





		{//Components
			int turretCount = 0;
			for (unsigned int i = 0; i < components.size(); i++)
			{
				//Look through component i's types vector
				for (unsigned int k = 0; k < components[i]->types.size(); k++)
					if (components[i]->types[k] == component::turret)
				{//Turret i
					turretCount++;

					//double turretMinAngle = angle - components[i]->maxAngle;
					//double turretMaxAngle = angle + components[i]->maxAngle;
					//if (turretMinAngle < 0)
					//	turretMinAngle += 2 * PI;
					//if (turretMaxAngle > 2 * PI)
					//	turretMaxAngle -= 2 * PI;

					if (data->grid[components[i]->gridLocationX][components[i]->gridLocationY]->directionalAim == true)
					{//Use directional aiming
						//-------------------
						if (abs(sf::Joystick::getAxisPosition(data->grid[components[i]->gridLocationX][components[i]->gridLocationY]->directionalJoystickId, data->grid[components[i]->gridLocationX][components[i]->gridLocationY]->verticalAxis)) + 
							abs(sf::Joystick::getAxisPosition(data->grid[components[i]->gridLocationX][components[i]->gridLocationY]->directionalJoystickId, data->grid[components[i]->gridLocationX][components[i]->gridLocationY]->horizontalAxis)) > 15)
						{//Detect directional movement
							joystickDirection = -1 * atan2(data->grid[components[i]->gridLocationX][components[i]->gridLocationY]->verticalAxisPolarity * sf::Joystick::getAxisPosition(data->grid[components[i]->gridLocationX][components[i]->gridLocationY]->directionalJoystickId, data->grid[components[i]->gridLocationX][components[i]->gridLocationY]->verticalAxis),
								data->grid[components[i]->gridLocationX][components[i]->gridLocationY]->horizontalAxisPolarity * sf::Joystick::getAxisPosition(data->grid[components[i]->gridLocationX][components[i]->gridLocationY]->directionalJoystickId, data->grid[components[i]->gridLocationX][components[i]->gridLocationY]->horizontalAxis));
							
							if (joystickDirection < 0)
								joystickDirection = 2 * PI + joystickDirection;


							temp_angleVar = components[i]->angle - joystickDirection;
							if (temp_angleVar < 0)
							{//-
								if (joystickDirection < components[i]->angle + PI)
								{//1st L
									components[i]->angle += components[i]->turningSpeed;
								}
								else
								{//4th R
									components[i]->angle -= components[i]->turningSpeed;
								}
							}//-
							else
							{//+
								if (joystickDirection < components[i]->angle - PI)
								{//2nd L
									components[i]->angle += components[i]->turningSpeed;
								}
								else
								{//3rd R
									components[i]->angle -= components[i]->turningSpeed;
								}
							}//+

						}
						//-------------------
					}
					else
					{//Use manual turret rotation (press button)
						if (testInput(componentKeys[components[i]->gridLocationX + components[i]->gridLocationY*0.001 + KEYB_left], mGame->mEvent) && components[i]->mouseAim == false)
						{//Rotate turret i CCW
							//if (turretMaxAngle > turretMinAngle)
							//{
							//	if (components[i]->angle < turretMaxAngle)
									components[i]->angle += components[i]->turningSpeed;
							//}
							//else
							//{
							//	if (components[i]->angle > turretMinAngle - components[i]->turningSpeed)
							//		components[i]->angle += components[i]->turningSpeed;
							//	else if (components[i]->angle < turretMaxAngle)
							//		components[i]->angle += components[i]->turningSpeed;
							//}
						}
						if (testInput(componentKeys[components[i]->gridLocationX + components[i]->gridLocationY*0.001 + KEYB_right], mGame->mEvent) && components[i]->mouseAim == false)
						{//Rotate turret i CW
							//if (turretMaxAngle > turretMinAngle)
							//{
							//	if (components[i]->angle > turretMinAngle)
									components[i]->angle -= components[i]->turningSpeed;
							//}
							//else
							//{
							//	if (components[i]->angle >= 0 && components[i]->angle < turretMaxAngle + components[i]->turningSpeed)
							//		components[i]->angle -= components[i]->turningSpeed;
							//	else if (components[i]->angle > turretMinAngle)
							//		components[i]->angle -= components[i]->turningSpeed;
							//}
						}
					}


					if (components[i]->mouseAim == true)
					{//Mouse input
						//Set positive mouse direction
						if (components[i]->mouseAimRelativeToCenter == true)
							mouseDirection = -1 * atan2(mousePosition.y - screenY, mousePosition.x - screenX);
						else
							mouseDirection = -1 * atan2(mousePosition.y - components[i]->screenY, mousePosition.x - components[i]->screenX);

						if (mouseDirection < 0)
							mouseDirection += 2 * PI;

						temp_angleVar = components[i]->angle - mouseDirection;
						if (temp_angleVar < 0)
						{//-
							if (mouseDirection < components[i]->angle + PI)
							{//1st L
								components[i]->angle += components[i]->turningSpeed;
							}
							else
							{//4th R
								components[i]->angle -= components[i]->turningSpeed;
							}
						}//-
						else
						{//+
							if (mouseDirection < components[i]->angle - PI)
							{//2nd L
								components[i]->angle += components[i]->turningSpeed;
							}
							else
							{//3rd R
								components[i]->angle -= components[i]->turningSpeed;
							}
						}//+
					}//End of mouse aiming
				}
			}
		}
	updateComponents();
	//////////////


	Object::update();
	return true;
}

void Player::turnRight(double factor)
{
	turnSpeed -= factor*rotationSpeed*(10.0 / (10 + shipMass));
}

void Player::turnLeft(double factor)
{
	turnSpeed += factor*rotationSpeed*(10.0 / (10 + shipMass));
}

void Player::accelerate(double factor)
{
	xSpeed += factor*(cos(2*PI - angle)*movementSpeed);
	ySpeed += factor*(sin(2 * PI - angle)*movementSpeed);

	relativeSpeedX += factor*(cos(2 * PI - angle)*movementSpeed);
	relativeSpeedY += factor*(sin(2 * PI - angle)*movementSpeed);
}

void Player::reverse(double factor)
{
	xSpeed -= factor*(cos(2 * PI - angle)*movementSpeed*0.6);
	ySpeed -= factor*(sin(2 * PI - angle)*movementSpeed)*0.6;

	relativeSpeedX -= factor*(cos(2 * PI - angle)*movementSpeed);
	relativeSpeedY -= factor*(sin(2 * PI - angle)*movementSpeed);
}

void Player::zoomIn(double f)
{
	zoomFactor += 0.01;
	limitZoom();
}

void Player::zoomOut(double f)
{
	zoomFactor -= 0.01;
	limitZoom();
}


void Player::updateComponents()
{
	componentIt = components.begin();
	while (componentIt != components.end())
	{
		if ((*componentIt)->alive() == false)
		{
			Component* temp_componentPointer = (*componentIt);
			components.erase(componentIt);
			delete temp_componentPointer;
			componentIt = components.begin();
		}
		else
			componentIt++;
	}


	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->update();
	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->draw();
}

void Player::checkBulletCollision(Bullet* b)
{
	float temp_coordinateModifier = resFactor*zoomFactor*textureRadius;
	for (unsigned int i = 0; i < components.size(); i++)
	{
		b->collisionCheckAngle = -1 * atan2(components[i]->y - b->y - temp_coordinateModifier, components[i]->x - b->x - temp_coordinateModifier);
		if (b->collisionCheckAngle < 0)
			b->collisionCheckAngle = ((2 * PI) + b->collisionCheckAngle);


		b->checkCollisionDistance = getDistance(b->x, b->y, components[i]->x - temp_coordinateModifier, components[i]->y - temp_coordinateModifier);
		b->checkCollisionRange = b->textureRadius + components[i]->textureRadius;

		if (b->checkCollisionDistance < b->checkCollisionRange)
		{
			if (b->canDamage == true)
			{
				components[i]->hp -= b->damage;
				b->canDamage = false;
				x += 6 * cos(angle);
				y += -6 * sin(angle);
			}

			//Bounce
			b->speed = b->speed*0.75;
			b->angle = PI / 2 + (irandom(0, 180) / double(180))*PI;
			b->xSpeed = cos(2 * PI - b->angle) * b->speed;
			b->ySpeed = sin(2 * PI - b->angle) * b->speed;
		}
	}

}


void Player::removeComponent(int cid)
{
	for (unsigned int i = 0; i < components.size(); i++)
		if (components[i]->id == cid)
		{
		components[i]->hp = -999;
		//int tX = components[i]->gridLocationX;
		//int tY = components[i]->gridLocationY;
		////Delete the component save data
		//delete data->grid[tX][tY];
		//data->grid[tX][tY] = new GridData;
		////Delete the actual component
		//delete components[i];
		//components.erase(components.begin() + i);
		}
}

void Player::loadPlayerData()
{
	angle = 0;
	while (!components.empty())
	{
		delete components.back();
		components.pop_back();
	}

	//Search for the core and start adding child components in chain reaction
	for (int ex = 0; ex < EDITOR_WIDTH; ex++)
		for (int ey = 0; ey < EDITOR_HEIGHT; ey++)
		{
		if (data->grid[ex][ey]->core == true)
			{
			coreX = ex;
			coreY = ey;
			addFromGrid(ex, ey);
			}
		}

	calculateCenterOfMass();

}

void Player::calculateCenterOfMass()
{
	//Calculate center of mass
	int temp_componentCount = 0;
	int temp_rowWeight[EDITOR_WIDTH] = {};
	int temp_pillarWeight[EDITOR_HEIGHT] = {};

	for (int ex = 0; ex < EDITOR_WIDTH; ex++)
		for (int ey = 0; ey < EDITOR_HEIGHT; ey++)
		{
		if (data->grid[ex][ey]->armor > 0)
		{
			temp_componentCount++;
			temp_pillarWeight[ex] += 1;
			temp_rowWeight[ey] += 1;
		}
		}
	shipMass = temp_componentCount;
	double centerX = 0;
	double centerY = 0;

	for (int ex = 0; ex < EDITOR_WIDTH; ex++)
	{
		centerX += (temp_pillarWeight[ex] * (1 + ex));
	}
	centerX = centerX / temp_componentCount;
	massCenterX = 100.0*(centerX - double(coreX + 1));

	for (int ey = 0; ey < EDITOR_HEIGHT; ey++)
	{
		centerY += (temp_rowWeight[ey] * (1 + ey));
	}
	centerY = centerY / temp_componentCount;
	massCenterY = 100.0*(centerY - double(coreY + 1));

	//Apply center of mass to all the components
	for (unsigned int i = 0; i < components.size(); i++)
	{
		components[i]->xOffset = components[i]->xOffsetOriginal - massCenterX;
		components[i]->yOffset = components[i]->yOffsetOriginal - massCenterY;
	}

	//Calculate ship width and height
	int temp_firstHorizontal = -1;
	int temp_firstVertical = -1;
	int temp_lastHorizontal = -1;
	int temp_lastVertical = -1;
	for (int ex = 0; ex < EDITOR_WIDTH; ex++)
		for (int ey = 0; ey < EDITOR_HEIGHT; ey++)
		{
		if (data->grid[ex][ey]->armor > 0 && temp_firstHorizontal == -1)
			temp_firstHorizontal = ex;
		if (data->grid[ex][ey]->armor > 0)
			temp_lastHorizontal = ex;
		}
	for (int ey = 0; ey < EDITOR_HEIGHT; ey++)
		for (int ex = 0; ex < EDITOR_WIDTH; ex++)
		{
		if (data->grid[ex][ey]->armor > 0 && temp_firstVertical == -1)
			temp_firstVertical = ey;
		if (data->grid[ex][ey]->armor > 0)
			temp_lastVertical = ey;
		}
	shipWidth = temp_lastHorizontal - temp_firstHorizontal + 1;
	shipHeight = temp_lastVertical - temp_firstVertical + 1;

	std::cout << "\nShip width/height: " << shipWidth << " / " << shipHeight;
	std::cout << "\nShip component mass: " << shipMass;
}

//This is the only way allowed to add components to a player ship
void Player::addFromGrid(int gx, int gy)
{
	components.push_back(new Component(this, this, (gx - coreX) * 100, (gy - coreY) * 100, gx, gy));
	components[components.size() - 1]->spr.setTexture(RM.getTexture("skeleton.png"));
	components[components.size() - 1]->spr.setTextureRect(sf::IntRect(1400, 0, 100, 100));
	components[components.size() - 1]->spr.setOrigin(50,50);
	
	if (data->grid[gx][gy]->turret == 1)
	{
		components[components.size() - 1]->createChild((gx - coreX) * 100, (gy - coreY) * 100, component::turret);
		components[components.size() - 1]->gridLocationX = gx;
		components[components.size() - 1]->gridLocationY = gy;
		components[components.size() - 1]->angleModifier = data->grid[gx][gy]->angleModifier*(PI/180);

	}
	
	//Handle children	
	int selfIndex = components.size() - 1;
	if (data->grid[gx][gy]->childUp == true)
	{
		int tempIndex = components.size();
		addFromGrid(gx, gy - 1);
		components[selfIndex]->childComponents.push_back(components[tempIndex]->id);
	}
	if (data->grid[gx][gy]->childDown == true)
	{
		int tempIndex = components.size();
		addFromGrid(gx, gy + 1);
		components[selfIndex]->childComponents.push_back(components[tempIndex]->id);
	}
	if (data->grid[gx][gy]->childRight == true)
	{
		int tempIndex = components.size();
		addFromGrid(gx + 1, gy);
		components[selfIndex]->childComponents.push_back(components[tempIndex]->id);
	}
	if (data->grid[gx][gy]->childLeft == true)
	{
		int tempIndex = components.size();
		addFromGrid(gx - 1, gy);
		components[selfIndex]->childComponents.push_back(components[tempIndex]->id);
	}
}

void Player::editShip()
{
	ShipEditor editor(mWindow, *data);
	editor.run();
	loadPlayerData();
	loadKeybindings();
}

//This method is called from the component destructor. (For Enemy class it has no use)
void Player::notifyComponentDestruction(int id)
{
	int gx = -1;
	int gy = -1;

	for (unsigned int i = 0; i < components.size(); i++)
		if (components[i]->id == id)
		{
		gx = components[i]->gridLocationX;
		gy = components[i]->gridLocationY;
		}

	
	if (gx < 0 || gy < 0)
	{
		return;
	}
	
	//Notify parent component
	if (gx < EDITOR_WIDTH - 1)
		data->grid[gx + 1][gy]->childLeft = false;
	if (gx > 0)
		data->grid[gx - 1][gy]->childRight = false;
	if (gy < EDITOR_HEIGHT - 1)
		data->grid[gx][gy + 1]->childUp = false;
	if (gy > 0)
		data->grid[gx][gy - 1]->childDown = false;

	//Remove actual data
	delete data->grid[gx][gy];
	data->grid[gx][gy] = new GridData();

	calculateCenterOfMass();
}

void Player::loadKeybindings()
{
	//Erase previous component bindings
	componentKeys.erase(componentKeys.begin(), componentKeys.end());

	//Dynamic key binding per component
	for (unsigned int i = 0; i < components.size(); i++)
		for (unsigned int k = 0; k < components[i]->types.size(); k++)
		{
		if (components[i]->types[k] == component::turret)
		{
			components[i]->mouseAim = data->grid[components[i]->gridLocationX][components[i]->gridLocationY]->mouseAim;
			components[i]->mouseAimRelativeToCenter = data->grid[components[i]->gridLocationX][components[i]->gridLocationY]->mouseAimRelativeToCenter;
			componentKeys[components[i]->gridLocationX + components[i]->gridLocationY*0.001 + KEYB_left] = data->grid[components[i]->gridLocationX][components[i]->gridLocationY]->turretLeft;
			componentKeys[components[i]->gridLocationX + components[i]->gridLocationY*0.001 + KEYB_right] = data->grid[components[i]->gridLocationX][components[i]->gridLocationY]->turretRight;
			componentKeys[components[i]->gridLocationX + components[i]->gridLocationY*0.001 + KEYB_fire] = data->grid[components[i]->gridLocationX][components[i]->gridLocationY]->turretFire;
			componentKeys[components[i]->gridLocationX + components[i]->gridLocationY*0.001 + KEYB_reload] = data->grid[components[i]->gridLocationX][components[i]->gridLocationY]->turretReload;
		}
		if (components[i]->types[k] == component::engine)
		{
			componentKeys[components[i]->gridLocationX + components[i]->gridLocationY*0.001 + KEYB_thrust] = data->grid[components[i]->gridLocationX][components[i]->gridLocationY]->engineThrust;
		}
		}
}