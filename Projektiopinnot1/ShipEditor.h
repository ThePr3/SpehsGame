#ifndef SHIPEDITOR_H
#define SHIPEDITOR_H
//Forward declared dependencies
class PlayerData;
class Button;
enum ButtonId;
//Included dependencies
#include <fstream>
#include <stdio.h>//?
#include <vector>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "UpgradeButton.h"




namespace editor
{
	enum Focus
	{
		base,
		component,
		actions,
		rotate,
		configuration,
		esc,
		upgrade,
	};

	enum Action
	{
		accelerate,
		reverse,
		turnRight,
		turnLeft,
		in,
		out,
		turretRight,
		turretLeft,
		fire,
		thrust
	};
}



class ShipEditor
{
public:
	~ShipEditor();
	ShipEditor(sf::RenderWindow&, PlayerData&);

	int run();
	void savePlayerData();
	void runSettings();

	void updateLines();
	void updateGridSpriteTextures();
	void updateGridSpriteLocations();
	void updateMouseGrab();
	void scrapComponent(int, int);
	void rotateGlowAngle();
	void applyRotation();
	void updateCoreConfigurationButtonVisibility();
	void updateTurretConfigurationButtonVisibility();
	void updateTurretConfigurationButtonStrings();
	void updateCoreConfigurationButtonStrings();
	void updateEngineConfigurationButtonStrings();
	void drawWindow();
	void drawSelectedRect();
	void drawSelectionShadeHighlight();
	void drawInheritanceSprites();
	void drawActions();
	void drawEscButtons();
	void drawUpgrades();
	void drawCost();
	void reloadUpgradeButtons();
	void reloadActions();
	void closeActions(editor::Focus);
	void drawCircleSlider();

	void drawConfigurations();

	void adjustColor();
	void updateTurretColorPreview();

	void closeCoreConfigurations();
	void closeTurretConfigurations();
	void closeEngineConfigurations();

	//Control schemes
	void setSelectionJoystickIndex(int);
	//void extractNamesFromString(std::string&, std::vector<std::string>&);
	void deleteControlScheme(std::string, std::string);
	//Turret
	void saveTurretControlScheme();
	void loadTurretControlScheme();
	void reloadTurretControlSchemeList();
	void updateTurretControlSchemeList();
	//Core
	void saveCoreControlScheme();
	void loadCoreControlScheme();
	void reloadCoreControlSchemeList();
	void updateCoreControlSchemeList();


	//Input
	MyKeys detectKey(ButtonId);
	std::pair<int, sf::Joystick::Axis> detectAxis();
	std::string getInputAsString(MyKeys);
	std::string getAxisAsString(sf::Joystick::Axis);
	std::string getPolarityAsString(float);
	void mouseLeftPressed();
	void mouseRightPressed();
	void zoom(int);

private:
	//References outside class
	PlayerData& playerData;
	sf::RenderWindow& mWindow;

	//Misc
	int keepRunning = 1;
	float scaleFactor;
	float glowAngle = 0; //What it does is... spin around in the game loop. Can be used to apply glowing effects
	float glowAmount = 0; // 0.0 - 1.0
	sf::Event mEvent;
	sf::Font font1;
	std::pair<int, sf::Joystick::Axis> temp_idAxisPair;
	float cameraX = 0; //(EDITOR_WIDTH / 2) * 100;
	float cameraY = 0; //(EDITOR_HEIGHT / 2) * 100;
	std::string saveControlSchemeInput;
	std::string workingFileName;
	std::fstream mFileStream;
	std::vector<std::string> turretControlSchemeNameList;
	std::vector<std::string> coreControlSchemeNameList;


	//Button lengths
	//Common
	int actionButtonWidth;
	int actionButtonHeight;
	int buttonHeight;
	int button1Width;
	int button2Width;
	int buttonBorder;
	//Turret conf
	int turretConfX1;
	int turretConfY1;
	int button1X1;
	int button2X1;
	int button3X1;
	int turretConfScrollBarY1;
	//Core conf
	int coreConfX1;
	int coreConfY1;
	int coreConfSchemeY;
	int coreConfScrollBarY1;
	//Engine conf
	int engineConfX1;
	int engineConfY1;
	//Upgrades
	int turretUpgradesY1;
	int engineUpgradesY1;

	//Misc states
	bool gettingUserInput = false;
	int clickTimer = 0;
	int adjustingColor = 0;
	bool mouseGrab = false;
	float mouseGrabX = 0;
	float mouseGrabY = 0;
	float grabCameraOriginX = 0;
	float grabCameraOriginY = 0;
	editor::Focus focus = editor::base;
	int selectedX = -1;
	int selectedY = -1;
	int checkX;
	int checkY;
	bool usingScrollBar = false;
	int scrollDelta = 0;
	int scrollState = 0;
	int cost = -1;
	sf::Vector2i mousePos;

	//Editor sprites/shapes/misc elements
	sf::RectangleShape selectedRect;
	sf::RectangleShape shadeRect;
	sf::RectangleShape coreConfigurationRect1;
	sf::RectangleShape coreConfigurationRect2;
	sf::RectangleShape coreConfigurationRect3;
	sf::RectangleShape turretConfigurationRect1;
	sf::RectangleShape turretConfigurationRect2;
	sf::RectangleShape turretConfigurationRect3;
	sf::RectangleShape engineConfigurationRect1;
	sf::RectangleShape engineConfigurationRect2;
	sf::RectangleShape engineConfigurationRect3;
	sf::RectangleShape upgradesBackground1;
	sf::RectangleShape upgradesBackground2;
	sf::Sprite circleSliderSpr;
	sf::Sprite colorPreviewTurret;
	std::vector<sf::VertexArray> horizontalLines;
	std::vector<sf::VertexArray> verticalLines;
	std::vector<sf::Sprite> gridSprites[EDITOR_WIDTH][EDITOR_HEIGHT];
	std::vector<sf::Sprite> inheritanceSprites[EDITOR_WIDTH][EDITOR_HEIGHT];
	sf::RectangleShape costBackground;
	sf::Text costText;
	sf::Text balanceText;


	//Buttons
	std::vector<Button> escButtons;
	std::vector<Button> actionButtons;
	std::vector<Button> actionTurretSchemeButtons;
	std::vector<Button> actionCoreSchemeButtons;
	std::vector<Button> actionJoystickIndexButtons;
	std::vector<Button> configurationButtons;
	std::vector<Button> coreConfigurationButtons;
	std::vector<Button> turretConfigurationButtons;
	std::vector<Button> turretControlSchemeList;
	std::vector<Button> coreControlSchemeList;
	std::vector<Button> engineConfigurationButtons;
	std::vector<UpgradeButton> turretUpgradeButtons;
	std::vector<UpgradeButton> engineUpgradeButtons;
	Button exitUpgrades;
	UpgradeButton upgradeArmor;
	bool actionTurretSchemeSelectionOpen = false;
	bool actionCoreSchemeSelectionOpen = false;
	bool actionJoystickIndexSelectionOpen = false;



};

#endif