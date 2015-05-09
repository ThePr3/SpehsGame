#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
//Included dependencies
#include <string>
#include <cassert>
#include <map>
#include <functional>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>



class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	//int hashPath(std::string);
	//std::string getFileName(std::string);
	//void loadTexture(std::string);
	//sf::Texture& getTexture(std::string);
	//void removeTexture(std::string);
	//void loadSound(std::string);
	//sf::SoundBuffer& getSound(std::string);
	//void removeSound(std::string);

//RESOURCES
	//Textures
	sf::Texture skeletonTex;
	sf::Texture coreTex;
	sf::Texture turretTex;
	sf::Texture engineTex;
	sf::Texture engineFireTex;
	sf::Texture engineFireAnimation;
	sf::Texture commanderTex;
	sf::Texture flierTex;
	sf::Texture enemyBasePurpleTex;
	sf::Texture enemyBaseGreenTex;
	sf::Texture enemyBaseTex;
	sf::Texture seekerTex;
	sf::Texture seekerActiveTex;
	sf::Texture seekerPassiveTex;
	sf::Texture seekerDodgeTex;
	sf::Texture sentinelTex;
	sf::Texture inheritanceArrowTex;
	sf::Texture circleSliderTex;
	sf::Texture slider1Tex;
	sf::Texture editorTurretTex;
	sf::Texture editorSkeletonTex;
	sf::Texture editorEngineTex;
	sf::Texture menuPlanetTex;
	sf::Texture menuSpace1Tex;
	sf::Texture menuLogoTex;
	sf::Texture sky1Tex;
	sf::Texture bullet1Tex;
	sf::Texture debris1Tex;
	sf::Texture ball1Tex;
	sf::Texture pointerArrowTex;
	//Sounds

	//Music

	//Font
	sf::Font menuFont;

private:
	//std::hash<std::string> hasher;
	//std::map<int, sf::Texture> textureMap;
	//std::map<int, sf::Texture>::iterator textureMapIt;
	//std::map<int, sf::SoundBuffer> soundMap;
	//std::map<int, sf::SoundBuffer>::iterator soundMapIt;


};

#endif