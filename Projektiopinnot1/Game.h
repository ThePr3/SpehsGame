#ifndef GAME_H
#define GAME_H
//Forward declared dependencies
class Object;
class Background;
//Included dependencies
#include <vector>
#include <sstream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/Event.hpp>
#include "SliderButton.h"



enum GameFocus
{
	gf_game,
	gf_escMenu,
};


class Game
{
public:
	Game(sf::RenderWindow&);
	~Game();

	void run();
	void pollEvents();
	void mouseLeftPressed();
	void drawEscMenu();
	void reloadEscMenuButtonStrings();
	void updateBullets();

	Object* playerObj;
	std::vector<Object*> objects;
	std::vector<Object*> bullets;
	std::vector<Object*>::iterator bulletIt;
	void updateBackgrounds();
	void updateObjects();
	void demo();
	std::string intToString(int);
	sf::Event mEvent;

	sf::RenderWindow& mWindow;
	bool keepRunning = true;
protected:
	GameFocus focus = gf_game;
	sf::Vector2i mousePos;
	std::vector<Background*> backgrounds;
	sf::Text balanceText;


	//GUI elements
	std::vector<sf::Sprite> elements;
	//Esc menu
	sf::RectangleShape escMenuShade;
	std::vector<Button> escMenuButtons;
	std::vector<SliderButton> escMenuSliders;

	//Iterators
	std::vector<Object*>::iterator obIt;
	std::vector<Background*>::iterator bgIt;

	//FOR TEST GAME
	int distanceFromStart;
	int spawnRandomization;
	sf::Text distanceText;

};

#endif