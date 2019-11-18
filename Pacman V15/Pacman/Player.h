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
#include "General.h"

#include <array>
#include <string>

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Player
{
private:
	//Main Pacman
	Vector2* m_vPacmanPosition;
	Rect* m_rPacmanSourceRect;
	Texture2D* m_t2dPacmanTexture;
	//AcrossTheScreenPacman Pacman
	Vector2* m_vPacman2Position;
	Rect* m_rPacman2SourceRect;
	Texture2D* m_t2dPacman2Texture;

	float m_fSpriteWidthHeight = 42;
	Vector2* m_vPacmanCenter;
	tilePosition m_tpPlayerTile;

	classPacketPacman m_pacman;
public:

	/// <summary> Constructs the Pacman class. </summary>
	Player();

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Player();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	bool Update(int elapsedTime);

	/*
		SETS
	*/

	bool setPosition(Vector2* vPos, classPacketPacman pacman);

	/*
		GETS
	*/

	Vector2* getPosition() { return m_vPacmanPosition;  }

	Vector2* getCenterPosition() { PlayerMovement_UpdateCenterPosition(); return m_vPacmanCenter;  }

	float getSpriteWidthHeight() { return m_fSpriteWidthHeight;}

	void PlayerMovement_UpdateCenterPosition();

	tilePosition getTile();
};