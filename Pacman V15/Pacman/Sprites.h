#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"
#include "Ghost.h"
#include "Pacman.h"
#include "Player.h"
#include "PrisonDetails.h"

#include <array>
#include <string>

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Sprites
{
protected:
	Ghost* m_ghostEnemies[4];

	Player* m_playerSpr;

private:
	int m_iLevelNumber = 0;
	int m_iRoundNumber = 0;
	bool m_bStopGhosts = false;//Stops all ghost except ghost heading to prison. Will also stop Pacman.

	//Vars for what EatenGhost does, showing score stopping sprites etc
	bool m_bEatenGhost = false;
	int m_iTimeToDelay = 0;
	int m_iTimeCount = 0;
	bool m_bPacmanDeath = false;
	int m_iDeathFrame = 0;
	int m_iDeathNextFrame = 4;

	int m_iGhostsEatenThisVunState = 0;//The number of ghosts eaten (more score for more ghosts)
public:

	/// <summary> Constructs the Pacman class. </summary>
	Sprites();

	/// <summary> Destroys any data associated with Pacman class. </summary>
	//virtual ~Sprites();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent(array <array <tile, 31>, 28>* aat);

	void Draw(classPacketPacman* pacman);

	bool Update(classPacketPacman* pacman, Vector2* vPlayerPos);

	/*
		GET
	*/

	Vector2* GetPlayerPosition(Player* player) { return player->getPosition(); }

	bool GetGhostEaten() { return m_bEatenGhost; }

	bool GetCanGhostsMove() { return m_bStopGhosts; }

	static int ReturnTileNumberFrom(int iScore);

	int GetEatenGhostScore();

	/*
		SET
	*/

	bool SetGhostEaten(bool bNew) { m_bEatenGhost = bNew; return m_bEatenGhost; }

	bool UpdatePacmanLocation(Vector2* v2, classPacketPacman pacman) { m_playerSpr->setPosition(v2, pacman); return true; }

	/*
		GHOST AI
	*/

	bool RunOncePerSpecialBit();

	/*
		SPRITE UPDATES
	*/

	bool MoveGhosts(classPacketPacman pacman);

	bool UpdateGhost(Ghost* ghost);

	bool SpawnInPen(int elapsedTime, classPacketPacman pacman);

	bool GhostsGetEaten(classPacketPacman* pacman);

	bool PacmanGetEaten(classPacketPacman* pacman);

	bool ShowScoreAndUpdateEatenGhost(classPacketPacman* pacman);

	bool KillPacman(classPacketPacman* pacman, Vector2* vPlayerPos);

	bool PacmanDeath(classPacketPacman* pacman);
};