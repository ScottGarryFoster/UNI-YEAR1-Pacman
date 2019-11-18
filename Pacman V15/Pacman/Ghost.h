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
#include "Pacman.h"
#include "General.h"
#include "PrisonDetails.h"
#include "Player.h"

#include <array>
#include <string>
#include <iostream>

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Ghost
{
private:
	ghostTypeEnum m_gt = ghostTypeEnum::gtRed;
	
	//Main Vars
	Vector2* m_vGhostPosition;
	Vector2* m_vGhostCenterPosition;
	Rect* m_rGhostRect;
	Texture2D* m_t2dGhostTexture;
	int m_iDirection = 0;
	tilePosition m_tpGhostTile;
	tilePosition m_tpGhostPreviousTile;
	float m_fGhostBaseSpeed = 0.18f;//0.18f is a tad faster than Pacman when eating munchies
	float m_fGhostModifiedSpeed = 0;//Used for caught states (only to be used temperarly as moving ghosts)
	float m_fGhostModifyForTunnel = -0.05f;//Added for Tunnel Tiles
	float m_fGhostModifyForSpecialBit = -0.07f;//Added when Vunrable State (Called Specail State in here)
	float m_fGhostInPenSpeed = 0.1f;//Speed when in pen (Set)

	//Shadow
	Vector2* m_vShadowPosition;
	Vector2* m_vShadowCenterPosition;
	Rect* m_rShadowRect;
	Texture2D* m_t2dShadowTexture;

	//Meta
	float m_fSpriteSize = 42;

	//Corners
	tilePosition m_tCornerMain;
	tilePosition m_tCornerSub;
	bool m_bInCorner = false;

	//Prison Details
	prisonDetails m_pd;

	//AI
	ghostBehaviour m_gbGhostMode = ghostBehaviour::gbCaptured;//Means captured
	bool m_bHaveEscapedOnce = false;
	bool m_bInPenHeadUp = false;//Head down
	array <array <int, 2>, 400> m_aaiPath;
	bool m_bIsHeadingAlongPath = false;
	tilePosition m_tpLastExitCalulation;
	bool m_bMaybeHeadAhead = false;
	bool m_bStoppedStill = false;
	bool m_bHaveBeenCapturedThisSpecial = false;//True if have been captured and sent back to the prison
	bool m_bAreInCorner = false;//True if we've touched a corner tile.
	bool m_bCaughtMovingInPen = false;//Used to move ghost into pen

	//Sprite/Animation
	bool m_bHasSpecialBit = false;//Used to reset X and Y if special bit called
	int m_iAnimationFrameCount = 0;
	int m_iAnimationFrameChange = 7;
	int m_iAnimationFrameCountB = 0;
	int m_iAnimationFrameChangeB = 14;

public:
	/*
		SETUP
	*/

	/// <summary> Constructs the Pacman class. </summary>
	Ghost(int iGhostType = 0);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Ghost();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent(array <array <tile, 31>, 28>* aat);

	/*
		GET METHODS
	*/

	Vector2* getPosition(bool bShadow = false) { if(!bShadow) return m_vGhostPosition; else return m_vShadowPosition; };//inline

	Rect* getRect(bool bShadow = false) { if (!bShadow) return m_rGhostRect; else return m_rShadowRect;};//inline

	Texture2D* getTexture(bool bShadow = false) { if (!bShadow) return m_t2dGhostTexture; else return m_t2dShadowTexture;};//inline

	Vector2 GetSpawnPosition(tilePosition tp, float fSpriteSize, bool bTakeHalf = false);

	ghostBehaviour GetBehaviour() { return m_gbGhostMode; }//Inline

	ghostTypeEnum getType() { return m_gt; }//Inline

	tilePosition getTile() { return m_tpGhostTile; }//Inline

	Vector2* getCenterPosition() { return m_vGhostCenterPosition; };//Inline

	Vector2* getCenterShadowPosition() { return m_vShadowCenterPosition; };//Inline

	tilePosition getFourTilesInFrontOfPacman(classPacketPacman pacman, Player* player);

	tilePosition getDoubleDistanceOfPacman(classPacketPacman pacman, Player* player, Ghost* ghosts[4]);

	bool getHaveBeenCapturedThisSpecial(){return m_bHaveBeenCapturedThisSpecial;};

	/*
		SET METHODS
	*/

	bool SetCorner(array <array <tile, 31>, 28> *aat);

	void SetCenterPosition();

	void SetPositionFromCenter() { m_vGhostPosition->X = m_vGhostCenterPosition->X - (m_fSpriteSize / 2); m_vGhostPosition->Y = m_vGhostCenterPosition->Y - (m_fSpriteSize / 2);}

	bool setBehaviour(ghostBehaviour gb) { m_gbGhostMode = gb; return true; }

	bool setCaught() { m_gbGhostMode = gbCaught; m_bIsHeadingAlongPath = false; EmptyPath(); return true; }

	/*
		SPRITE AND ANIMATION
	*/
	void UpdateSpriteRect(classPacketPacman pacman);

	/*
		CONVERSION
	*/

	ghostTypeEnum Ghosts_intToGhostType(int iGhostType);

	int Ghosts_ghostTypeToInt(ghostTypeEnum gtType);

	/*
		DOING METHODS
	*/

	bool Ghosts_Update(classPacketPacman pacman);

	bool Ghosts_Move(classPacketPacman pacman, Player* player, Ghost* ghosts[4]);

	bool Spawn(classPacketPacman pacman, bool bTesting = false);

	/*
		---AI
	*/
	bool AIgbCaptured(classPacketPacman pacman, Player* player, Ghost* ghosts[4]);

	bool AIgbEscaping(classPacketPacman pacman, Player* player, Ghost* ghosts[4]);

	bool AIMoveDirectToPos(classPacketPacman* pacman, Player* player, Ghost* ghosts[4], float fDestinationX, float fDestinationY);

	bool AIgbMain(classPacketPacman pacman, Player* player, Ghost* ghosts[4]);

	bool AIgbScatter(classPacketPacman pacman, Player* player, Ghost* ghosts[4]);

	bool AIgbCaught(classPacketPacman* pacman, Player* player, Ghost* ghosts[4]);

	void EmptyPath();

	tilePosition ReturnOppositeCorner(tilePosition tp);

	bool AIRunOncePerSpecialBit();

	/*
		---Basic Moving
	*/
	bool BasicMovingAdjacent(classPacketPacman pacman, tilePosition tpMoveTo, bool bSnapToTile = false);

	bool BasicMovingPath(classPacketPacman pacman);

	bool ShadowGhost(classPacketPacman pacman);

};