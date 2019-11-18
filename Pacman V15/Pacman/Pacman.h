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
#include "Player.h"
#include "PrisonDetails.h"
#include "General.h"
#include "Sprites.h"

#include <array>
#include <string>

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
	int m_iElapsedTime = 0;

	//Game Vars (Level Management mainly)
	bool m_bPlayerCanMove = false;
	bool m_bGhostsCanMove = false;
	bool m_bIsNewGame = true;//Is the current game a new game
	bool m_bLevelHasStartted = false;//Is the level loaded
	bool m_bCanStartLevel = false;//Can we load the level
	bool m_bRoundStarted = false;//Has the round startted?
	int m_iLivesAnimationFrame = 1;//The frame in Animation
	int m_iLivesFrameTick = 0;//Literal Frame
	char m_cLivesCharacterAnimation = ' ';//The literal Menu Frame
	int m_iLevelNumber = 1;//What level are we currently running
	int m_iRoundNumber = 1;//The round used for Ghost AI mainly but also fruit spawns
	int m_sReadyStatus = 0;//0- Nothing, 1- Ready, 2 - Game Over
	bool m_bLivesNeedAnimating = false;
	bool m_bPlayerOneNeedAnimating = false;//Shows Player one at start of game
	int m_iPlayerOneFrame = -1;//Used for timing
	int m_iCurrentPacmanSprite = -1;
	int m_iMapNumber = -1;
	array <int,5> m_iMapsForArcade;
	int m_iArcadeCurrentMap = 0;
	int m_iKonamiCodePlace = 0;
	char m_cKonamiCode[11] = {'8','8','2','2','4','6','4','6','B','A','R'};
	bool m_bKonamiReadyForInput = false;

	bool m_bHaveStartedMusic = false;//Used to not continually play music

	bool m_bGhostAI_ScatterPhase = false;//Used for the scatter phase of ghosts
	bool m_bGhostAI_SpecialBitActive = false;//Used for when the special bit is picked up
	bool m_bGhostAI_SpecialBitClose = false;//For when the Special phase is almost over
	int m_iNextScatterTime = -1;
	int m_iTimesScattered = -1;
	array <array <int, 7>, 5> m_aaiListOfScatterChaseModes;
	bool m_bPacmanDeath = false;//Used to soft Reset
	bool m_bGameover = false;
	int m_bGameOverFrames = -1;
	int m_iWhereToAddScore = -1;
	bool m_bHideGhosts = true;
	bool m_bHasGottenToHighScoreBoard = false;//Used for Audio
	string m_sScoreNameStorage = "";
	string m_sScoreNameStorageOld = "";
	bool m_bScoreReadyForInput = false;
	int m_iScoreEntryStep = 0;
	int m_iScoreForNewLives = 0;

	bool m_bFruitHasSpawnedThisRound = false;
	int m_iFruitSecondsPassedThisRound = 0;
	int m_iFruitDespawnSecond = -1;

	//Inventory
	int m_iInventoryItem = -1;
	Vector2* m_vInventory;
	Rect* m_rInventory;
	Texture2D* m_tInventory;
	bool m_bInvReady = false;
	
	//Collectables
	array <array <tileCollectables, 31>, 28> m_atcCollectables;
	Vector2* m_vTileCollectPosition;
	Rect* m_rTileCollectRect;
	Texture2D* m_tTileCollectTexture;
	int m_iCollectableAffectSeconds = -1;
	string m_sPrintToMenu = "";
	int m_iNumberOfBits = 0;
	int m_iNumberOfLives = 0;
	int m_iTargetLives = 0;
	int m_iAnimateLives = false;
	int m_iCollablesCollectedThisRound = 0;
	bool m_bResetSpecialBit = false;
	tileAbilities m_taArray[10];
	int m_iCurrentActiveBit = -1;//Used to update the currently active
	bool m_bCanCollect = false;//Sets if you are able to collect things
	bool m_bPlayGhostDeath = false;//Used to play the ghost death sound
	bool m_bPlayPacmanDeath = false;//Used to play Pacman Death sound
	int m_iAnimatedBitFrameCount = 0;
	tilePosition m_tpFruitSpawnLocation;
	int m_frameDoubleCheckBits = 0;//Used for a double check on number of bits

	//Main Pacman
	Vector2* m_vPacmanPosition;
	Rect* m_rPacmanSourceRect;
	Texture2D* m_t2dPacmanTexture;
	//AcrossTheScreenPacman Pacman
	Vector2* m_vPacman2Position;
	Rect* m_rPacman2SourceRect;
	Texture2D* m_t2dPacman2Texture;

	//MetaData for the Pacman sprites and Tilemap
	float m_fSpriteWidthHeight = 42.0f;
	float m_fTilemapWidthHeight = 24.0f;
	float m_fTilemapMarginTop = 48.0f;

	//Tilemap:
	Vector2* m_vTilePosition;
	Rect* m_rTileRect;
	Texture2D* m_tTileTexture;
	array <array <tile, 31>, 28> m_aatTilemap;
	tilePosition m_tpPacmanSpawn;

	//PlayerMovement
	Vector2* m_vPacmanCenter;
	int m_iBufferKeyPress = 0;
	int m_iMovementKeyPress = 0;
	bool m_bPlayerStill = false;
	bool m_bChangingDirection = false;
	float m_fPacmanBaseSpeed = 0.19f;
	float m_fPacmanModifyForBits = -0.03f;
	float m_fPacmanSpeedModify = 0;
	//With bits: 0.161f		Without bits: 0.19f based on movement on the bottom line being 3.8 with / 3.23 without
	array <int, 2> m_aiLastTileOnCollection = { -1, -1 };

	//PlayerVars
	int m_iAnimationCount = 0;
	bool m_bAnimationDirection = true;
	int m_iFrameCount = 0;
	int m_iFrameToAct = 1;

	int iOld = -1;
	int iTimes = 0;
	int iOld2 = 0;

	//Utility
	int m_iTimeSecondsTotal;
	bool m_bOnASecondFrame;
	int m_iElapsedSeconds;
	int m_iElapsedMinutes;
	int m_iElapsedHours;
	int m_iFpsUpdateSecond;
	int m_iCurrentFPS;

	//Menus
	int m_iMenuLocation = 0;
	int m_iSubMenuLocation = 0;
	int m_iGameSelection = 0;
	Vector2* m_vGameLogoPos;
	Rect* m_rGameLogoRec;
	Texture2D* m_t2dGameLogoTex;
	string m_sLastGameLogoTexture;
	bool m_bReadyForNewSelection = false;
	bool m_bReadyForNewSelectionSub = false;
	array <array <uiTile, 35>, 28> m_auiMenuTiles;
	array <array <uiTile, 35>, 28> m_auiMenuBackgroundTiles;
	float m_iUITileSize = 24;
	Vector2* m_vUITilePosition;
	Rect* m_rUITleRect;
	Texture2D* m_tUITileTexture;
	string m_sLastUITileTexture;
	int m_iCurrentGameScore = 0;
	bool m_bSwitchingUI = true;
	int m_iPreviousReadyState = -1;
	int m_iCurrentHighScore = -1;

	//Audio
	float m_fOptionsVolumeMusic = 1;
	float m_fOptionsVolumeSoundEffects = 0.5f;
		//Music
	SoundEffect* m_seMusicSound;
	int m_iMusicLengthInSeconds = 0;
	int m_iMusicLengthInSecondsElap = 0; //Time passed
	bool m_bMusicIsPlaying = false;
	int m_iMusicTimeLeftOver = -1;
		//Sound Effects
	soundEffect m_seSoundEffects[10];
		//Chop Sound:
	SoundEffect* m_seChopSound;

	//Sprites
	Sprites* m_sprMain;

	//Sent to other classes:
	classPacketPacman cpp;

	//Debug
	bool m_bLastSent = false;

	int m_iNumber = 0;

public:

	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

	/*
		GETS
	*/
	int getElapsedTime() { return m_iElapsedTime; }//Inline

	int getRoundNumber() { return m_iRoundNumber; }//Inline

	int getLevelNumber() { return m_iLevelNumber; }//Inline

	/*
		TILESET
	*/
	bool TilesetLoadContent();

	array <array <tile, 31>, 28> Tileset_DrawTileset(int level = 1);

	array <float, 3> Tileset_StartPositionPacman(int level = 1);

	void Tileset_DrawBox(array <array <tile, 31>, 28>* at, int x, int y, int width, int height, tile t);

	array <array <tile, 31>, 28> Tileset_SetImageForTiles(array <array <tile, 31>, 28> at);

	static array <tile, 9> Tileset_GetInformationAroundTile(array <array <tile, 31>, 28> at, array <int, 2> aiCurentTile);

	array <float, 2> Tileset_ReturnCordOfTile(int x, int y);

	array <array <tile, 31>, 28> Tileset_GetTilemap(){return m_aatTilemap;};

	void Tileset_SetExits(array <array <tile, 31>, 28>* at);

	void Tileset_SetTunnels(array <array <tile, 31>, 28>* aat);

	static array <int, 2> Tileset_FindCloestPassableTile(array <array <tile, 31>, 28> aatLocal, array <int, 2> aiTestTile);

	void Tileset_DrawTilesetFromFile(string sLevel, array <array <tile, 31>, 28>* aat, array <array <tileCollectables, 31>, 28>* m_atcCollectables = nullptr);

	//AStar

	struct AStarScore
	{
		int fGCost = 0;
		int fHCost = 0;
		int fFCost = 0;
		array <int, 2> aiParent = { -1,-1 };

		void dumpToConsole()
		{
			cout << "fFCost: " << fFCost << " = fGCost(" << fGCost << ") + fHCost(" << fHCost << ")";
		}
	};

	static array <array <int, 2>, 400> BasicMovement_AStarCalc(classPacketPacman pacman, array <int, 2> aiOrigin, array <int, 2> aiTarget, bool bExportRoute = false, bool bOmmitOrigin = false, array <array <int, 2>, 10> aaiExpensiveTiles = { {-1,-1} });

	static Pacman::AStarScore BasicMovement_CalcFGH(array <array <AStarScore, 31>, 28> aabScoreList, array <int, 2> aiTarget, array <int, 2> aiCurrent, array <int, 2> aiParent);

	/*
		PLAYER MOVEMENT
	*/
	bool PlayerLoadContent();

	void PlayerMovement_Update(int elapsedTime);

	bool PlayerMovement_UpdateBufferKeypress();

	void PlayerMovement_UpdateCenterPosition(Vector2* topLeftPos, Vector2* vReturn);

	array <int, 2> PlayerMovement_UpdateCurrentPosition(Vector2* centerPos);

	bool PlayerMovement_UpdateTargetTile(array <int, 2> aiCurrentTile, Vector2* vCenterPos);

	bool PlayerMovement_MovePlayerToSwitchingTile(int elapsedTime, array <int, 2> aiCurrentTile, Vector2* vCenterPos);

	Vector2* PlayerMovement_UpdateMove(int elapsedTime, int direction, Vector2* centerPos);

	Vector2* PlayerMovement_UpdateMovePacman2(Vector2* pacmanPos);

	Rect PlayerMovement_UpdateDirection(int direction,bool bDealingWithPlayer = true);

	float PlayerMovement_UpdateAnimation(bool bDealingWithPlayer = true);

	bool PlayerMovement_MovementAreOnCorrectLine(int direction, Vector2* centerPos, array <int, 2> currentTile);

	/*
		LEVEL MANAGEMENT
	*/

	bool LevelManagementLoadContent();

	bool LevelManagement_Update(int elapsedTime, Sprites* sprMain);

	bool LevelManagement_StartNewLevel();

	bool LevelManagement_StartNewRound();

	bool LevelManagement_ResetLives(int iNumberOfLives);

	void LevelManagement_ReturnBlankBits(array <array <tileCollectables, 31>, 28>* atc);

	void LevelManagement_PlaceBits(array <array <tileCollectables, 31>, 28>* atc, int iLevel);

	void LevelManagement_DrawBits(array <array <tileCollectables, 31>, 28>* atc, int x, int y, int width, int height, tileCollectables t);

	bool LevelManagement_CreateLevelTiles(int level, bool bDrawTileset = false);

	void LevelManagement_UpdateScatter();

	int LevelManagement_CountBits(array <array <tileCollectables, 31>, 28>* atc);

	bool LevelManagement_PlayerOneAnimating();

	bool LevelManagement_EnterNewScore();

	bool LevelManagement_ResetAllVars();

	bool LevelManagement_SendPlayerLocation(Sprites* sprMain);

	bool LevelManagement_RandomiseFruit(int elapsedTime, array <array <tileCollectables, 31>, 28>* atc);

	bool LevelManagement_SpawnFruit(array <array <tileCollectables, 31>, 28>* atc);

	bool LevelManagement_RemoveFruit(int elapsedTime, array <array <tileCollectables, 31>, 28>* atc);

	bool LevelManagement_SetPacmanSprite(int iSprite = -1);

	bool LevelManagement_AwardedLivesUpdate();

	void LevelManagement_DoubleCheckBits();

	void LevelManagement_MixUpLevelsForArcade();

	void LevelManagement_KonamiCode();

	/*
		COLLECTABLES
	*/
	bool CollectablesLoadContent();

	bool CollectablesLoadAbilities();

	bool Collectables_Update(int elapsedTime, Sprites* spr);

	bool Collectables_Abilities(int elapsedTime, int iAbility, Sprites* spr, bool bCollection = true);

	bool Collectables_ResetActiveCollectable(Sprites* sprMain);

	bool Collectables_UpdateAnimations();

	void Collectables_UpdateFruitScore(bool bEraseScore = false);

	void Collectables_InvUpdate();

	void Collectables_InvUseItem();

	/*
		MENU FUNCTIONS
	*/
	bool MenuLoadContent();

	void Menu_Update(int elapsedTime);

	void Menu_PacmanMainMenu(array <array <uiTile, 35>, 28>* auiMenu, array <array <uiTile, 35>, 28>* auiBackground, bool bSwitchingUI);

	void Menu_PacmanHighScore(array <array <uiTile, 35>, 28>* auiMenu, array <array <uiTile, 35>, 28>* auiBackground, bool bSwitchingUI, int iScorePlace = -1);

	void Menu_PacmanMainUI(array <array <uiTile, 35>, 28>* auiMenu, array <array <uiTile, 35>, 28>* auiBackground, bool bSwitchingUI = true);

	static void Menu_WriteLetters(array <array <uiTile, 35>, 28>* aui, int iX, int iY, string words, colour colour = white, int iAlignment = 0, bool bOverrideBlanks = false, int iSetLength = 0);

	static int Menu_LettersToNumbers(char letter);

	void Menu_UpdateGameSelection();

	void Menu_InGameMenus();

	/*
		AUDIO FUNCTIONS
	*/
	bool AudioLoadContent();

	bool Audio_Update();

	bool Audio_SetMusic(int iTrack, bool bSetup = false);

	bool Audio_PlayMusic();

	bool Audio_SetSoundEffect(int iSoundEffectID, int iSoundEffectToLoad = 1);

	bool Audio_PlaySoundEffect(int iSoundEffectID, bool bInterupt = false);

	bool Audio_PauseMusic();

	bool Audio_UpdateHasSoundPlaying(int iSoundEffectID);

	bool Audio_ResumeMusic();

	bool Audio_StopMusic();

	/*
	UTILITY FUNCTIONS
	*/
	
	bool UtilityLoadContent();

	void UtilityUpdateTime(int elapsedTime);

	int UtilityReturnSeconds(int seconds, int minutes, int hours, bool bMicro = false);

	int UtilityReturnSeconds(bool bMicro = false);

	int UtilityTimeInTheFuture(int secondToLook, bool bMicro = false);

	int UtilitySearchArraysInt(array <int, 100> mainArr, int searchItem);

	static int UtilityRandomNumberBetween(int min, int max);

	static int UtilityDirectionConvert(int convert, bool toNumPad);

	static int UtilitySearchArraysChar(array <char, 100> mainArr, char searchItem);

	static bool UtilityCollisionCheck(Rect* rFirst, Rect* rSecond);

	static bool UtilityCollisionPosCheck(Vector2* v2FirstPosition, Vector2* v2SecondPosition, float fMarginOnFirst);

	static string UtilityStringToUpper(string sContent);

	static bool UtilityEnterName(string &sOutput, array <array <uiTile, 35>, 28> &auiMenu, int iLine, int iStartChar, int iMaxLength, bool &bReadyForInput);

	static char UtilityButtonPress(bool &bReadyForInput);

	static int UtilityScoreToTileLocation(int iScore);

	/*
		CLASS PACKETS
	*/
	void UpdateClassPacket(int iElapsedTime)
	{
		//Level Vars
		cpp.iElapsedTime = iElapsedTime;
		cpp.iRoundNumber = m_iRoundNumber;
		cpp.iLevelNumber = m_iLevelNumber;
		cpp.sReadyStatus = m_sReadyStatus;
		//Tileset Vars
		cpp.iTileMapTileSize = m_fTilemapWidthHeight;
		cpp.iTileMapOffset = m_fTilemapMarginTop;
		if (cpp.ptrAatTileMap == nullptr)
		{
			cpp.ptrAatTileMap = &m_aatTilemap;
			cpp.ptrAuiMenuTiles = &m_auiMenuTiles;
		}
		//Collectable Vars
		cpp.iCollablesCollectedThisRound = m_iCollablesCollectedThisRound;
		cpp.iPacmanDirection = m_iMovementKeyPress;
		cpp.bResetSpecialBit = m_bResetSpecialBit;
		//Movement Vars
		cpp.bPlayerCanMove = m_bPlayerCanMove;
		cpp.bGhostsCanMove = m_bGhostsCanMove;
		//Ghost AI vars
		cpp.bGhostAI_ScatterPhase = m_bGhostAI_ScatterPhase;
		cpp.bGhostAI_SpecialBitActive = m_bGhostAI_SpecialBitActive;
		cpp.bGhostAI_SpecialBitClose = m_bGhostAI_SpecialBitClose;
		cpp.bHideGhosts = m_bHideGhosts;

		//Player Pointers (only to used as a last resort)
		cpp.vPlayerVector = m_vPacmanPosition;
		cpp.rPlayerRect = m_rPacmanSourceRect;
		cpp.tex2dPlayerTex = m_t2dPacmanTexture;
		cpp.iPacmanDirection = m_iMovementKeyPress;

		if (cpp.ptrITargetLives == nullptr)//Pacman Death
		{
			cout << "YES 100%" << endl;
			cpp.ptrITargetLives = &m_iTargetLives;
			cpp.ptrINumberOfLives = &m_iNumberOfLives;
			cpp.ptrBRoundStarted = &m_bRoundStarted;
			cpp.ptrBLevelHasStartted = &m_bLevelHasStartted;
			cpp.ptrBCanStartLevel = &m_bCanStartLevel;
			cpp.ptrIsNewGame = &m_bIsNewGame;

			cpp.ptrBPacmanDeath = &m_bPacmanDeath;

			cpp.ptrICurrentGameScore = &m_iCurrentGameScore;

			cpp.ptrBPlayGhostDeath = &m_bPlayGhostDeath;
			cpp.ptrBPlayPacmanDeath = &m_bPlayPacmanDeath;
		}

	}
};