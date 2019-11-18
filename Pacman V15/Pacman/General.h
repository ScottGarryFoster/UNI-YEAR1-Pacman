#pragma once

#include "S2D/S2D.h"
using namespace S2D;

#include <array>
#include <string>
#include <iostream>

enum ghostTypeEnum : int { gtRed = 0, gtPink = 1, gtBlue = 2, gtOrange };

enum ghostBehaviour : int { gbCaptured = 0, gbEscaping = 1, gbCornerOnce = 2, gbMain, gbScatter, gbSpecial, gbCaught };

/*
	PURPOSE
	This header file has very general classes and structs which can be used by the other files.
	I'm avoiding using a CPP for the simple programs ohwever such does exist.

	WHY
	There is a hidden reasoning for this, dependancies. If something is used in almost all the
	classes then you want to avoid a circular dependancies. So although some of these may end up
	moving, it's a place where if something is being used in very difforent classes, both can
	get access to them without using the scope modifiers and the like.
*/

class tilePosition
{
private:
	int m_iX = 0, m_iY = 0;
public:
	///CONSTRUCTOR, TAKES X AND Y DEFAULTED TO -1
	tilePosition::tilePosition(int iX = -1, int iY = -1)
	{
		m_iX = iX;
		m_iY = iY;
	}

	///SETS THE X/Y
	void setPosition(int iX, int iY)
	{
		m_iX = iX;
		m_iY = iY;
	}

	///Returns the cords. FALSE = X, TRUE = Y
	int getCord(bool bReturnX = false)
	{
		if (!bReturnX)
			return m_iX;
		else
			return m_iY;
	}

	///Returns the center cord, requires tile information
	float getCenterCord(float iTileMapTileSize, float iTileMapTileOffset, bool bReturnX = false)
	{
		if (!bReturnX)
			return m_iX * iTileMapTileSize + (iTileMapTileSize / 2);
		else
			return m_iY * iTileMapTileSize + (iTileMapTileSize / 2) + iTileMapTileOffset;
	}

	tilePosition RelativeOffset(int iXOffset, int iYOffset)
	{
		int iNewX = m_iX + iXOffset;
		int iNewY = m_iY + iYOffset;
		tilePosition tpNewReturn(iNewX, iNewY);
		return tpNewReturn;
	}

	int ReturnDirectionFromTile(tilePosition tp)
	{
		int iOriginX = tp.getCord();
		int iOriginY = tp.getCord(true);

		if (iOriginX > m_iX)
			return 4;
		else if (iOriginX < m_iX)
			return 6;
		if (iOriginY > m_iY)
			return 8;
		else if (iOriginY < m_iY)
			return 2;
		return 0;
	}

	int ReturnDistanceBetweenTiles(tilePosition tp)
	{
		int iOriginX = tp.getCord();
		int iOriginY = tp.getCord(true);

		int iDistanceX = 0, iDistanceY = 0;

		if (iOriginX > m_iX)
			iDistanceX = iOriginX - m_iX;
		else if (iOriginX > m_iX)
			iDistanceX = m_iX - iOriginX;

		if (iOriginY > m_iY)
			iDistanceY = iOriginY - m_iY;
		else if (iOriginY > m_iY)
			iDistanceY = m_iY - iOriginY;

		return iDistanceX + iDistanceY;
	}

	array <int, 2> ConvertToAI()
	{
		return { m_iX, m_iY };
	}

	string outputVars()
	{
		string sOutput = "(" + m_iX + ',' + m_iY + ')';
		return sOutput;
	}
};

struct tile
{
public:
	bool bPassable = true;
	int iTileshown = 0;
	bool bPrison = false;
	bool bPrisonEnterance = false;

	bool bIsExit = false;//Used for Decision making in Ghost AI

	//Corner Pars
	bool m_bIsCorner = false;
	ghostTypeEnum m_gtCornerType = gtRed;
	bool m_bIsCornerMain = true; //True for Main, False for sub position

	//Position Paras:
	tilePosition tpLocation;

	//Tunnel Paras
	bool m_bIsTunnel = false;//If you're in a tunnel
	tilePosition m_tpLocalParent;//The tunnel tile on this side of the board
	tilePosition m_tpForeignParent;//The tunnel you'd end up on.

	///Updates but never touches tpLocation as this shouldn't change
	void UpdateTileInformation(tile t)
	{
		bPassable = t.bPassable;
		iTileshown = t.iTileshown;
		bPrison = t.bPrison;
		bPrisonEnterance = t.bPrisonEnterance;
		bIsExit = t.bIsExit;
		m_bIsCorner = t.m_bIsCorner;
		m_gtCornerType = t.m_gtCornerType;
		m_bIsCornerMain = t.m_bIsCornerMain;
	}
};

/*
GLOBAL STRUTS:
*/

struct uiTile
{
	int iSheet = 0;
	int iTileshown = -1;
	bool bShiftTileLocationRight = false;//Shifts the location of the tile to the right by half a tile
};
struct tileCollectables
{
	int iCollectable = -1;
	bool bCollected = false;
	int iScoreIncrease = 0;
	int iAbility = 0;
	bool bContributesToCount = true;
	float fSpeedModification = 0;
	bool bNudgeRight = false;//Nudges the image half a tile right
	bool bAnimateWithTheTileBelow = false;//Used for a simple two image animation.
	bool bShowScoreOnTile = false;//Will replace tile with the score gained for 2 seconds.
	int iAnimationOrigin = -1;
	int iCollectableTilesheetWidth = 5;

	int iScoreOnTileTime = -1;//Used for score on tile. This is used by the code.
	uiTile* ut;
};
struct tileAbilities
{
	bool bPerformImmediately = true;//Perform action on Pickup, False is inventory
	bool bTimed = false;//True is timed, False sets on Pickup
	int iTimedSeconds = 0;//If above then this must be set to the number of seconds the ablilty is taking place
	bool bVunrabilityState = false;//True puts the ghosts into vunrabilty, false does nothing.
	bool bDestroyGhostsInArea = false;//If True all ghosts within a square of 7 tiles will be sent to the pen
};
enum colour { white = 0, red, darkblue, skyblue, pink, orange, black, icon };

struct soundEffect
{
	SoundEffect* seSoundEffect;
	int seSoundEffectsLengthInSeconds = 0;
	int seSoundEffectsSecondsElap = 0;
	bool bSoundEffectIsPlaying = false;
};

struct classPacketPacman
{
	int iElapsedTime = 0;
	int iRoundNumber = 0;
	int iLevelNumber = 0;
	int sReadyStatus = 0;
	int iTileMapTileSize = 24;
	int iTileMapOffset = 48;
	array <array <tile, 31>, 28>* ptrAatTileMap = nullptr;
	array <array <uiTile, 35>, 28>* ptrAuiMenuTiles = nullptr;
	

	int iCollablesCollectedThisRound = 0;
	int iPacmanDirection = 0;
	int iPacmanPos = 0;

	bool bGhostAI_ScatterPhase = false;//Used for the scatter phase of ghosts
	bool bGhostAI_SpecialBitActive = false;//Used for when the special bit is picked up
	bool bGhostAI_SpecialBitClose = false; //Used for when the special bit is close
	bool bResetSpecialBit = false;//Used to reset. This is only true the first time a bit is collected

	bool bPlayerCanMove = false;
	bool bGhostsCanMove = false;

	int *ptrICurrentGameScore = nullptr; //Allows sub classes to alter score

	bool bHideGhosts = false;
	bool *ptrBPlayGhostDeath = nullptr;
	bool *ptrBPlayPacmanDeath = nullptr;

	//Pacman Death:
	int *ptrITargetLives = nullptr, *ptrINumberOfLives = nullptr;
	bool *ptrBRoundStarted = nullptr;
	bool *ptrBLevelHasStartted = nullptr;
	bool *ptrBCanStartLevel = nullptr;
	bool *ptrIsNewGame = nullptr;

	bool *ptrBPacmanDeath = nullptr;

	//Player Stuff (only for the interim when Player is in the main Pacman)
	Vector2* vPlayerVector;
	Rect* rPlayerRect;
	Texture2D* tex2dPlayerTex;
	int iPlayerDirection = 0;

	void outputToConsole()
	{
		cout << "iElapsedTime: " << iElapsedTime << endl;
		cout << "iRoundNumber: " << iRoundNumber << endl;
		cout << "iLevelNumber: " << iLevelNumber << endl;
		cout << "sReadyStatus: " << sReadyStatus << endl;
		cout << "iTileMapTileSize: " << iTileMapTileSize << endl;
		cout << "iTileMapOffset: " << iTileMapOffset << endl;
		cout << "iCollablesCollectedThisRound: " << iCollablesCollectedThisRound << endl;
		cout << "iPacmanDirection: " << iPacmanDirection << endl;
	}

	bool SetMenuTiles(int x, int y, uiTile uit)
	{
		(*ptrAuiMenuTiles)[x][y] = uit;
		return true;
	}

	bool SetTileMap(int x, int y, tile tile)
	{
		(*ptrAatTileMap)[x][y] = tile;
		return true;
	}
};