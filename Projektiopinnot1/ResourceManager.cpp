#include "Main.h"
#include "ResourceManager.h"

ResourceManager::~ResourceManager() {}
ResourceManager::ResourceManager()
{//LOAD ALL RESOURCES INTO MEMORY
//TEXTURES
	//PLAYER
	skeletonTex.loadFromFile("Texture/Player/skeleton.png");
	coreTex.loadFromFile("Texture/Player/core.png");
	turretTex.loadFromFile("Texture/Player/turret.png");
	engineTex.loadFromFile("Texture/Player/engine.png");
	engineFireTex.loadFromFile("Texture/Player/engine_fire.png");
	engineFireAnimation.loadFromFile("Texture/Player/engine_fire_animation_testversion.png");
	engineFireTier1Animation.loadFromFile("Texture/Player/engine_fire_animation_tier1.png");
	engineFireTier4Animation.loadFromFile("Texture/Player/engine_fire_animation_tier4.png");
	shipArmor1Tex.loadFromFile("Texture/Player/armor1.png");
	//Turret
	turretPlatformTex.loadFromFile("Texture/Player/Turret/platform.png");
	turretRotationEngineTex.loadFromFile("Texture/Player/Turret/rotatingEngine.png");
	turretFrameTex.loadFromFile("Texture/Player/Turret/frame.png");
	turretBarrelTex.loadFromFile("Texture/Player/Turret/barrel.png");
	turretMagazineTex.loadFromFile("Texture/Player/Turret/magazine.png");
	turretRecoilAbsorberTex.loadFromFile("Texture/Player/Turret/recoilAbsorber.png");
	turretReloaderTex.loadFromFile("Texture/Player/Turret/reloader.png");
	//Engine
	engineFuelTankTex.loadFromFile("Texture/Player/Engine/fuelTank.png");
	engineThrusterTex.loadFromFile("Texture/Player/Engine/thruster.png");
	engineRechargerTex.loadFromFile("Texture/Player/Engine/recharger.png");
	//ENEMY
	flierTex.loadFromFile("Texture/Enemy/Flier.png");
	enemyBasePurpleTex.loadFromFile("Texture/Enemy/enemy_base_purple.png");
	enemyBaseGreenTex.loadFromFile("Texture/Enemy/enemy_base_green.png");
	enemyBaseTex.loadFromFile("Texture/Enemy/enemy_base.png");
	//Seeker
	seekerTex.loadFromFile("Texture/Enemy/Seeker.png");
	seekerActiveTex.loadFromFile("Texture/Enemy/seeker_active_animation.png");
	seekerPassiveTex.loadFromFile("Texture/Enemy/seeker_passive_animation.png");
	seekerDodgeTex.loadFromFile("Texture/Enemy/seeker_dodge_animation.png");
	//Sentinel
	sentinelTex.loadFromFile("Texture/Enemy/Sentinel.png");
	//Flier
	flierTex.loadFromFile("Texture/Enemy/flier_active_animation.png");
	//Commander
	commanderTex.loadFromFile("Texture/Enemy/commander_standard_texture.png");
	//MENU
	inheritanceArrowTex.loadFromFile("Texture/Menu/inheritanceArrow.png");
	circleSliderTex.loadFromFile("Texture/Menu/circleSlider.png");
	slider1Tex.loadFromFile("Texture/Menu/slider1.png");
	editorTurretTex.loadFromFile("Texture/Menu/editorTurret.png");
	editorSkeletonTex.loadFromFile("Texture/Menu/editorSkeleton.png");
	editorEngineTex.loadFromFile("Texture/Menu/editorEngine.png");
	menuSpace1Tex.loadFromFile("Texture/Menu/menuSpace1_update.png");
	menuLogoTex.loadFromFile("Texture/Menu/menuLogo_update.png");
	//EFFECTS
	explosion1Tex.loadFromFile("Texture/Effects/explosion1.png");
	//BACKGROUNDS
	sky1Tex.loadFromFile("Texture/Background/sky.png");
	sky2Tex.loadFromFile("Texture/Background/sky2.png");
	sky3Tex.loadFromFile("Texture/Background/sky3.png");
	//Bullets
	bullet1Tex.loadFromFile("Texture/Misc/bullet1.png");
	bullet2Tex.loadFromFile("Texture/Misc/bullet2.png");
	bullet3Tex.loadFromFile("Texture/Misc/bullet3.png");
	bullet4Tex.loadFromFile("Texture/Misc/bullet4.png");
	bullet5Tex.loadFromFile("Texture/Misc/bullet5.png");
	//MISC
	debrisTex.loadFromFile("Texture/Misc/debris2.png");
	stationMainTex.loadFromFile("Texture/Misc/DockingStation_Main.png");
	stationDocksTex.loadFromFile("Texture/Misc/DockingStation_Docks.png");
	//In-game GUI
	compassTex.loadFromFile("Texture/Misc/compass.png");
	ball1Tex.loadFromFile("Texture/Misc/ball.png");
	pointerArrowTex.loadFromFile("Texture/Misc/pointer_arrow.png");
	pressEnterToDockTex.loadFromFile("Texture/Misc/pressEnterToDock.png");
	displayMeterTex.loadFromFile("Texture/Misc/spehsGuiMeter.png");

	//Sounds
	menuButtonHoverBuffer.loadFromFile("Audio/Sound Effects/editor_menu_hover_sound_ver03.wav");//Menu hover
	menuButtonHoverSound.setBuffer(menuButtonHoverBuffer);
	menuUpgradeBuffer.loadFromFile("Audio/Sound Effects/upgrade_purchase_sound_ver00.wav");//Upgrade
	menuUpgradeSound.setBuffer(menuUpgradeBuffer);
	menuClickBuffer.loadFromFile("Audio/Sound Effects/menu_select_sound_ver02.wav");//Menu Select
	menuClickSound.setBuffer(menuClickBuffer);
	enterMenuBuffer.loadFromFile("Audio/Sound Effects/enter_menu_sound_ver00.wav");//Enter Menu
	enterMenuSound.setBuffer(enterMenuBuffer);
	turretFireBuffer.loadFromFile("Audio/Sound Effects/turret_fire_sound_ver01.wav");//Turret
	turretFireSound.setBuffer(turretFireBuffer);
	explosionBuffer.loadFromFile("Audio/Sound Effects/explosion_sound_ver04.wav");//Explosion
	explosionSound.setBuffer(explosionBuffer);
	thrustingBuffer.loadFromFile("Audio/Sound Effects/engine_sound_ver03.wav");//Engine
	thrustingSound.setBuffer(thrustingBuffer);

	//Music

	//Fonts
	menuFont.loadFromFile("Font/ORANGEKI.ttf");
}

void ResourceManager::updateVolumeLevels()
{//This function is called everytime the user saves/loads settings
	//Sound effects
	menuButtonHoverSound.setVolume(SFX_VOLUME);
	menuUpgradeSound.setVolume(SFX_VOLUME);
	menuClickSound.setVolume(SFX_VOLUME);
	enterMenuSound.setVolume(SFX_VOLUME);
	turretFireSound.setVolume(SFX_VOLUME);
	explosionSound.setVolume(SFX_VOLUME);
	thrustingSound.setVolume(SFX_VOLUME);

	//Music
	
}






//int ResourceManager::hashPath(std::string fname)
//{
//	size_t hashedPath = hasher(fname);
//	//std::cout << "\nHashed name for " << fname << ": " << hashedPath;
//	return hashedPath;
//}
///*
//Removes file path and returns file name
//for example: "Texture/Player/turret.png" would return "turret.png"
//*/
//std::string ResourceManager::getFileName(std::string path)
//{
//	std::string fileName;
//	int fileNameLength = 0;
//	for (int i = path.size() - 1; i >= 0; i--)
//		if (path[i] != '/')
//			fileNameLength++;
//		else
//			i = -1;//End loop
//
//	for (int i = 0; i < fileNameLength; i++)
//	{
//		fileName += path[path.size() - fileNameLength + i];
//	}
//	//std::cout << "\n\t\t " << fileNameLength << ":" << fileName;
//
//	return fileName;
//}
////Texture
//void ResourceManager::loadTexture(std::string path)
//{
//	//std::string fileName = getFileName(path);
//	//std::cout << "\nLoading " << fileName;
//
//	size_t hashedPath = hashPath(getFileName(path));
//
//	textureMap.insert(std::make_pair(hashedPath, sf::Texture()));
//	textureMap[hashedPath].loadFromFile(path);
//}
//sf::Texture& ResourceManager::getTexture(std::string path)
//{
//	std::string fileName = getFileName(path);
//	if (textureMap.find(hashPath(fileName)) != textureMap.end())
//		return textureMap[hashPath(fileName)];
//	else
//	{
//		std::cout << "\nResourceManager: No texture found [" << fileName << "] ";
//		return textureMap[hashPath(fileName)];
//	}
//}
//void ResourceManager::removeTexture(std::string path)
//{
//	std::string fileName = getFileName(path);
//	textureMap.erase(hashPath(fileName));
//}
////Sound
//void ResourceManager::loadSound(std::string path)
//{
//	std::string fileName = getFileName(path);
//
//	size_t hashedPath = hashPath(fileName);
//
//	soundMap.insert(std::make_pair(hashedPath, sf::SoundBuffer()));
//	soundMap[hashedPath].loadFromFile(path);
//}
//sf::SoundBuffer& ResourceManager::getSound(std::string path)
//{
//	std::string fileName = getFileName(path);
//	return soundMap[hashPath(fileName)];
//}
//void ResourceManager::removeSound(std::string path)
//{
//	std::string fileName = getFileName(path);
//	soundMap.erase(hashPath(fileName));
//}