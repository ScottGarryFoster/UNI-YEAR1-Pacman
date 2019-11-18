#include "Pacman.h"
#include "Sprites.h"
#include "FileManagement.h"

#include <sstream>
#include <array>
#include <string>
#include <iostream>

bool Pacman::LevelManagementLoadContent()
{
	m_vTileCollectPosition = new Vector2(-9001, -9001);
	m_rTileCollectRect = new Rect(0.0f, 0.0f, m_fTilemapWidthHeight, m_fTilemapWidthHeight);
	m_tTileCollectTexture = new Texture2D();
	m_tTileCollectTexture->Load("Textures/tiles_collectables.png", false);

	LevelManagement_ReturnBlankBits(&m_atcCollectables);

	//This sets up the scattermode times.
	//All you need to know is one dimention is chase (even)
	//The other is scatter.
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 7; j++)
			if (j % 2 == 0)
				m_aaiListOfScatterChaseModes[i][j] = 20;
			else if(i == 0)
				if(j <= 3)//1 and 3
					m_aaiListOfScatterChaseModes[i][j] = 7;
				else
					m_aaiListOfScatterChaseModes[i][j] = 5;
			else if (i <= 3)//1-3 Inclusive
				if (j <= 3)
					m_aaiListOfScatterChaseModes[i][j] = 7;
				else
					m_aaiListOfScatterChaseModes[i][j] = 0;//There is no chase after the second time
			else //Round 5 and onwards
				if (j <= 3)
					m_aaiListOfScatterChaseModes[i][j] = 5;
				else
					m_aaiListOfScatterChaseModes[i][j] = 0;//There is no chase after the second time


	m_tpFruitSpawnLocation.setPosition(13,17);

	return true;
}

bool Pacman::LevelManagement_Update(int elapsedTime, Sprites* sprMain)
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();//For Input

	/*if (keyboardState->IsKeyDown(Input::Keys::P))
	{
		cout << endl << "m_bLevelHasStartted: "; if (m_bLevelHasStartted) cout << "TRUE"; else cout << "FALSE";
		cout << endl << "m_bCanStartLevel: "; if (m_bLevelHasStartted) cout << "TRUE"; else cout << "FALSE";

	}*/
	if (!m_bGameover) //Not in a gameover state
	{
		if (!m_bLevelHasStartted && m_bCanStartLevel)//Stage one setup the level a new
		{
			//This must appear before you add collectables
			Collectables_UpdateFruitScore(true);//Ensures the collectable scores don't remain on the tileset
			

			if (m_bIsNewGame)//Literally from new
			{
				if (m_iLevelNumber <= 2)
					m_iMapNumber = m_iLevelNumber;
				else
				{
					LevelManagement_MixUpLevelsForArcade();
					m_iArcadeCurrentMap = 0;
					m_iMapNumber = m_iMapsForArcade[m_iArcadeCurrentMap];
				}
				
				//cout << "a" << endl;
				LevelManagement_CreateLevelTiles(m_iMapNumber, true);
				LevelManagement_ResetLives(3);
				m_iTargetLives = m_iNumberOfLives - 1;//Set the target number of lives before the next round
				m_bLivesNeedAnimating = true;
				m_bIsNewGame = false;
				Audio_SetSoundEffect(0, 1);
				m_iRoundNumber = 0;//Is first level
				m_bPlayerOneNeedAnimating = true;
				m_iScoreForNewLives = 0;//Number of lives awarded
			}
			
			m_iRoundNumber++;//Increase level number

			if (m_iLevelNumber > 2 && m_iMapNumber == -1 && m_iArcadeCurrentMap < 5)
			{
				m_iArcadeCurrentMap++;
				if (m_iArcadeCurrentMap == 5)
				{
					m_bGameover = true; m_iMapNumber = 1;
				}
				else
				{
					m_iMapNumber = m_iMapsForArcade[m_iArcadeCurrentMap];
					LevelManagement_CreateLevelTiles(m_iMapNumber, true);
				}
			}
				
				
				//m_iMapNumber = rand() % 10 + 1;

			//Just for the round new
			m_iCollablesCollectedThisRound = 0; //Used for escaping ghosts
			sprMain->SpawnInPen(elapsedTime, cpp);//Spawns the ghosts
			LevelManagement_CreateLevelTiles(m_iMapNumber, false);//Move Pacman
			LevelManagement_SendPlayerLocation(sprMain);//Sends new Pacman location

			//For Pacman Sprite
			if (m_iLevelNumber <= 2)
				m_iCurrentPacmanSprite = m_iLevelNumber;
			else
				m_iCurrentPacmanSprite = rand() % 10 + 1; //Random for Arcade Mode

			Collectables_ResetActiveCollectable(sprMain);
			LevelManagement_PlaceBits(&m_atcCollectables, m_iMapNumber);//Places bits on the tilemap

			m_bLevelHasStartted = true;
			m_bRoundStarted = false;
			m_bGhostsCanMove = false;
			m_bPlayerCanMove = false;
			m_iCollectableAffectSeconds = -1;
			m_iScoreEntryStep = 0;

			if (m_bPlayerOneNeedAnimating)
				m_bHideGhosts = true;
			else
				m_bHideGhosts = false;

			Audio_SetMusic(m_iMapNumber, true); //The music for the level
			Audio_SetSoundEffect(2, 2);//Ghost death music(2) is sound effect (2)
			m_bHaveStartedMusic = true;//Ensures we only start playing once
		}
		else if (m_bLevelHasStartted && m_bCanStartLevel)//We can setup the round
		{
			if (m_iTargetLives == -1)//Player has no more lives
			{
				m_bGameover = true;
				return false;
			}

			m_bGhostAI_ScatterPhase = false;//Used for the scatter phase of ghosts
			m_bGhostAI_SpecialBitActive = false;//Used for when the special bit is picked up
			m_bGhostAI_SpecialBitClose = false;//For when the Special phase is almost over
			m_iNextScatterTime = -1;
			m_iTimesScattered = -1;
			Audio_StopMusic();

			//cout << "2" << endl;
			m_bCanStartLevel = false;//We've setup the level, onto the round below

			m_iAnimationCount = 1;//Just resets the animation frame so pacman doesn't look like a circle

			LevelManagement_CreateLevelTiles(m_iMapNumber, false);
			//Check for no lives left
			m_bPlayerCanMove = false;
			m_iLivesFrameTick = 0;
			if (m_iNumberOfLives != m_iTargetLives)
				m_bLivesNeedAnimating = true;

			//The follow is a repeat because you can come into from here.
			sprMain->SpawnInPen(elapsedTime, cpp);//Spawns the ghosts
			Collectables_ResetActiveCollectable(sprMain);
			m_bGhostsCanMove = false;
			m_bPlayerCanMove = false;
			m_iCollectableAffectSeconds = -1;

			//Occasionally when in vunrable mode and finishing a level it doens't reset.
			//This is to solve this:
			m_bResetSpecialBit = false;
			m_iCollectableAffectSeconds = -1;
			m_iCurrentActiveBit = -1;

			//Reset Fruit Spawning
			m_bFruitHasSpawnedThisRound = false;
			m_iFruitSecondsPassedThisRound = 0;
			m_iFruitDespawnSecond = -1;
			LevelManagement_RemoveFruit(elapsedTime, &m_atcCollectables);//Gets rid of the fruit

			if (m_bPlayerOneNeedAnimating)
				m_bHideGhosts = true;

			m_bRoundStarted = false;
			m_sReadyStatus = 1;//Show the words ready
		}
		else if (!m_bRoundStarted && m_bLevelHasStartted && !m_bCanStartLevel)
		{
			//cout << "3" << endl;
			if (m_bLivesNeedAnimating)
				m_bLivesNeedAnimating = LevelManagement_StartNewRound();//Start the round

			if (m_bHaveStartedMusic)
				m_bHaveStartedMusic = Audio_PlayMusic();

			if (m_bPlayerOneNeedAnimating)
				m_bPlayerOneNeedAnimating = LevelManagement_PlayerOneAnimating();
			else
				m_bHideGhosts = false;

			if (!m_bLivesNeedAnimating && !m_bHaveStartedMusic && !m_bPlayerOneNeedAnimating)
			{
				m_bRoundStarted = true;
				m_sReadyStatus = 0;//Hide the words ready
			}
			LevelManagement_SetPacmanSprite();//Reload Pacman sprite
			//In the above is the sound stuff also.
		}
		else if (m_bRoundStarted && m_bLevelHasStartted && !m_bCanStartLevel)//Level and Round is startted and in action
		{
			if (m_iNumberOfBits > 0) //Round is running
			{
				m_bCanCollect = true;
				m_bHideGhosts = false;
				m_bGhostsCanMove = true;
				m_bPlayerCanMove = true;
				LevelManagement_UpdateScatter();
				LevelManagement_AwardedLivesUpdate();
				LevelManagement_DoubleCheckBits();
				LevelManagement_KonamiCode();
				if(!m_bFruitHasSpawnedThisRound)
					LevelManagement_RandomiseFruit(elapsedTime, &m_atcCollectables);
			}
			else
			{
				m_bCanCollect = false;
				m_bGhostsCanMove = false;
				m_bPlayerCanMove = false;
				m_bLevelHasStartted = false;
				m_bCanStartLevel = true;
				m_bRoundStarted = false;

				m_bGhostAI_ScatterPhase = false;//Used for the scatter phase of ghosts
				m_bGhostAI_SpecialBitActive = false;//Used for when the special bit is picked up
				m_bGhostAI_SpecialBitClose = false;//For when the Special phase is almost over
				m_iNextScatterTime = -1;
				m_iTimesScattered = -1;
				m_iCollectableAffectSeconds = -1;
				Audio_StopMusic();

				if (m_iLevelNumber > 2) m_iMapNumber = -1;//Makes it so the map is random
			}
		}
	}
	else //Game Over
	{
		if (m_iScoreEntryStep == 0)
		{
			if (m_bGameOverFrames < 25)
			{
				m_bGhostsCanMove = false;
				m_bPlayerCanMove = false;
				m_bLevelHasStartted = false;
				m_bCanStartLevel = true;
				m_bRoundStarted = false;

				m_bGameOverFrames++;
				m_sReadyStatus = 2;

				Audio_StopMusic();
			}
			else
			{
				m_sReadyStatus = -1;
			}

			if (m_sReadyStatus == -1)
			{
				Audio_UpdateHasSoundPlaying(2);
				if (FileManagement::CanAddScore(m_iLevelNumber, m_iCurrentGameScore))
				{
					m_iScoreEntryStep = 1;
					m_iWhereToAddScore = FileManagement::WhereScore(m_iLevelNumber, m_iCurrentGameScore);

					Menu_PacmanHighScore(&m_auiMenuTiles, &m_auiMenuBackgroundTiles, true, m_iWhereToAddScore);
					Menu_WriteLetters(&m_auiMenuTiles, 7, 12 + (m_iWhereToAddScore * 2), to_string(m_iCurrentGameScore), red);
					m_bHasGottenToHighScoreBoard = true;//Used for sound effect
				}
				else
				{
					LevelManagement_ResetAllVars(); //Resets Menu and Level Management vars (soft game reset)
				}
			}
		}
		else if(m_iScoreEntryStep == 1)
		{
			LevelManagement_EnterNewScore();
		}
		
	}
	return true;
}

void Pacman::LevelManagement_DoubleCheckBits()
{
	/*
		So I bug tested one of the arcade levels and when the level respawned it was imposible because
		the bit count would never be zero.

		I've not been able to recreate it however this check is a catch for this.
	*/
	m_frameDoubleCheckBits++;
	if (m_frameDoubleCheckBits > 60)
	{
		m_iNumberOfBits = 0;
		m_iNumberOfBits = LevelManagement_CountBits(&m_atcCollectables); //Number of bits
		m_frameDoubleCheckBits = 0;
	}
}

bool Pacman::LevelManagement_EnterNewScore()
{
	if(m_bHasGottenToHighScoreBoard)
		Audio_PlaySoundEffect(5, true);//LevelUpSound Effect
	m_bHasGottenToHighScoreBoard = false;

	if (UtilityEnterName(m_sScoreNameStorage, m_auiMenuTiles, 12 + (m_iWhereToAddScore * 2), 18, 7, m_bScoreReadyForInput))//True means a name is entered
	{
		Audio_PlaySoundEffect(5, true);//LevelUpSound Effect
		FileManagement::AddScore(m_iLevelNumber, m_iCurrentGameScore, m_sScoreNameStorage);
		m_bSwitchingUI = true;
		LevelManagement_ResetAllVars();//Resets Menu and Level Management vars (soft game reset)
	}
	else if(m_sScoreNameStorageOld != m_sScoreNameStorage)
	{
		if(m_sScoreNameStorageOld.length() > m_sScoreNameStorage.length())
			Audio_PlaySoundEffect(4, true);//Bad
		else
			Audio_PlaySoundEffect(3, true);//Good

		m_sScoreNameStorageOld = m_sScoreNameStorage;
	}
	return true;
}

void Pacman::LevelManagement_MixUpLevelsForArcade()
{
	m_iMapsForArcade = { 1,2,3,4,5 };//Reset

	random_shuffle(std::begin(m_iMapsForArcade), std::end(m_iMapsForArcade)); //Randomise the order
}

void Pacman::LevelManagement_KonamiCode()
{
	char cBtn = '@';
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();//For Input
	if (keyboardState->IsKeyDown(Input::Keys::UP))
	{
		if (m_bKonamiReadyForInput)
		{
			cBtn = '8';
			m_bKonamiReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::DOWN))
	{
		if (m_bKonamiReadyForInput)
		{
			cBtn = '2';
			m_bKonamiReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::RIGHT))
	{
		if (m_bKonamiReadyForInput)
		{
			cBtn = '6';
			m_bKonamiReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::LEFT))
	{
		if (m_bKonamiReadyForInput)
		{
			cBtn = '4';
			m_bKonamiReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::B))
	{
		if (m_bKonamiReadyForInput)
		{
			cBtn = 'B';
			m_bKonamiReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::A))
	{
		if (m_bKonamiReadyForInput)
		{
			cBtn = 'A';
			m_bKonamiReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::RETURN))
	{
		if (m_bKonamiReadyForInput)
		{
			cBtn = 'R';
			m_bKonamiReadyForInput = false;
		}
	}
	else
		m_bKonamiReadyForInput = true;

	if (cBtn != '@')
	{
		if (m_cKonamiCode[m_iKonamiCodePlace] == cBtn)
			m_iKonamiCodePlace++;
		else
			m_iKonamiCodePlace = 0;

		if (m_iKonamiCodePlace == 11)
		{
			m_iKonamiCodePlace = 0;
			m_iNumberOfLives = 10;
			Audio_PlaySoundEffect(5, true);//LevelUpSound Effect
		}
	}
}

bool Pacman::LevelManagement_ResetAllVars()
{
	//Game Vars (Level Management mainly)
	m_bPlayerCanMove = false;
	m_bGhostsCanMove = false;
	m_bIsNewGame = true;//Is the current game a new game
	m_bLevelHasStartted = false;//Is the level loaded
	m_bCanStartLevel = false;//Can we load the level
	m_bRoundStarted = false;//Has the round startted?
	m_iLivesAnimationFrame = 1;//The frame in Animation
	m_iLivesFrameTick = 0;//Literal Frame
	m_cLivesCharacterAnimation = ' ';//The literal Menu Frame
	m_iLevelNumber = 1;//What level are we currently running
	m_iRoundNumber = 1;//The round used for Ghost AI mainly but also fruit spawns
	m_sReadyStatus = 0;//0- Nothing, 1- Ready, 2 - Game Over
	m_bLivesNeedAnimating = false;
	m_bPlayerOneNeedAnimating = false;//Shows Player one at start of game
	m_iPlayerOneFrame = -1;//Used for timing

	m_bHaveStartedMusic = false;//Used to not continually play music

	m_bGhostAI_ScatterPhase = false;//Used for the scatter phase of ghosts
	m_bGhostAI_SpecialBitActive = false;//Used for when the special bit is picked up
	m_bGhostAI_SpecialBitClose = false;//For when the Special phase is almost over
	m_iNextScatterTime = -1;
	m_iTimesScattered = -1;
	//array <array <int, 7>, 5> m_aaiListOfScatterChaseModes;
	m_bPacmanDeath = false;//Used to soft Reset
	m_bGameover = false;
	m_bGameOverFrames = -1;
	m_iWhereToAddScore = -1;
	m_bHideGhosts = true;
	m_bScoreReadyForInput = false;
	m_iScoreEntryStep = 0;


	//MENUS:
	m_iMenuLocation = 0;
	m_iSubMenuLocation = 0;
	m_bReadyForNewSelection = false;
	m_bReadyForNewSelectionSub = false;
	m_iUITileSize = 24;
	m_iCurrentGameScore = 0;
	m_bSwitchingUI = true;

	//Collectables:
	array <array <tileCollectables, 31>, 28> atcBlank;
	m_atcCollectables = atcBlank;

	//Tilemap:
	array <array <tile, 31>, 28> aatBlank;
	m_aatTilemap = aatBlank;
	return true;
}

bool Pacman::LevelManagement_RandomiseFruit(int elapsedTime, array <array <tileCollectables, 31>, 28>* atc)
{
	/*
		In my system, fruit can't spawn earlier than the 10 second mark
		If fruit hasn't spawned then every 5 seconds from the first 15 seconds into the level,
		it'll flip a coin. Heads (1) means it'll spawn fruit.
	*/
	int iIntialMSeconds = 10000, iGapBetweenTries = 5000;
	if (m_iFruitSecondsPassedThisRound < iIntialMSeconds + iGapBetweenTries)//15 seconds passed
		m_iFruitSecondsPassedThisRound += elapsedTime;//Count towards 10 seconds

	if (m_iFruitSecondsPassedThisRound >= iIntialMSeconds + iGapBetweenTries)//15 seconds passed
	{
		int iSpawnFruit = rand() % 2 + 1;
		if (iSpawnFruit == 1)
		{
			LevelManagement_SpawnFruit(atc);
			m_bFruitHasSpawnedThisRound = true;
			m_iFruitDespawnSecond = UtilityTimeInTheFuture(10);
		}
		m_iFruitSecondsPassedThisRound = iIntialMSeconds;//Set this back down to 10 seconds
	}
	return true;
}

bool Pacman::LevelManagement_RemoveFruit(int elapsedTime, array <array <tileCollectables, 31>, 28>* atc)
{
	/*
		Get's rid of fruit. Used for round reset.
	*/
	tileCollectables blankBit;
	blankBit.iCollectable = 0;
	blankBit.bCollected = true;

	LevelManagement_DrawBits(atc, m_tpFruitSpawnLocation.getCord(false), m_tpFruitSpawnLocation.getCord(true), 1, 1, blankBit);
	return true;
}

bool Pacman::LevelManagement_SpawnFruit(array <array <tileCollectables, 31>, 28>* atc)
{
	int iCollectable = 0;
	int iScoreIncrease = 0;
	switch (m_iRoundNumber)
	{
		case 2://Strawberry
			iCollectable = 6;
			iScoreIncrease = 300;
			break;
		case 3://Orange
			iCollectable = 7;
			iScoreIncrease = 500;
			break;
		case 4://Apple
			iCollectable = 8;
			iScoreIncrease = 700;
			break;
		case 5://Melon
			iCollectable = 9;
			iScoreIncrease = 1000;
			break;
		case 6://Galaxian 
			iCollectable = 15;
			iScoreIncrease = 2000;
			break;
		case 7://Bell 
			iCollectable = 16;
			iScoreIncrease = 3000;
			break;
		case 8://Key
			iCollectable = 17;
			iScoreIncrease = 5000;
			break;
		default:
		case 1://Cherry
			iCollectable = 5;
			iScoreIncrease = 100;
			break;
	}

	tileCollectables fruitBit; 
	fruitBit.iCollectable = iCollectable;
	fruitBit.iScoreIncrease = iScoreIncrease;
	fruitBit.iAbility = 2;
	fruitBit.bContributesToCount = false;
	fruitBit.bNudgeRight = true;
	fruitBit.bAnimateWithTheTileBelow = true;
	fruitBit.iAnimationOrigin = iCollectable;
	fruitBit.bShowScoreOnTile = true;

	LevelManagement_DrawBits(atc, m_tpFruitSpawnLocation.getCord(false), m_tpFruitSpawnLocation.getCord(true), 1, 1, fruitBit);

	return true;
}

bool Pacman::LevelManagement_PlayerOneAnimating()
{
	m_bHideGhosts = true;
	m_iPlayerOneFrame++;
	if (m_iPlayerOneFrame == 100)
		return false;
	return true;
}

void Pacman::LevelManagement_UpdateScatter()
{
	if (m_iLevelNumber <= 2)//In the first two levels
	{
		if (m_iRoundNumber > 0)
		{
			if (m_iTimesScattered < 0)
				m_iTimesScattered = 0;//Always positive

			if (m_iTimesScattered < 7)//After 7 times there is only chase
			{
				//The following is short hand for the round number array:
				int iListRoundNumber = m_iRoundNumber - 1;
				if (iListRoundNumber > 4)
					iListRoundNumber = 4;

				if (m_iNextScatterTime == -1)
					m_iNextScatterTime = UtilityTimeInTheFuture(m_aaiListOfScatterChaseModes[iListRoundNumber][m_iTimesScattered]);

				if (m_iNextScatterTime <= UtilityReturnSeconds())//If switch in mode
				{
					if (m_iTimesScattered % 2 == 0)//If it's even
						m_bGhostAI_ScatterPhase = true;//It's scatter time
					else
						m_bGhostAI_ScatterPhase = false;//If it's odd it's chase

					m_iTimesScattered += 1;//Add one to the timees
					m_iNextScatterTime = -1;//Reset this so a new time is generated
				}
			}
			else
				m_bGhostAI_ScatterPhase = false;//Perm chase
		}
	}
	if (m_bLastSent != m_bGhostAI_ScatterPhase)
	{
		/*if (m_bGhostAI_ScatterPhase)
			cout << "SCATTER" << endl;
		else
			cout << "CHASE" << endl;*/

		m_bLastSent = m_bGhostAI_ScatterPhase;
	}
}

bool Pacman::LevelManagement_StartNewLevel()
{
	if (m_bIsNewGame)
	{
		m_iLevelNumber = 1; //This is the first level
		m_iNumberOfLives = 3; //Reset lives
	}
	return true;
}

bool Pacman::LevelManagement_StartNewRound()
{
	if (m_iTargetLives != m_iNumberOfLives)
	{
		m_iLivesAnimationFrame = 1;
		m_cLivesCharacterAnimation = 'A';
		m_iNumberOfLives = m_iTargetLives;
	}

	if (m_iLivesAnimationFrame < 10)
	{
		m_iLivesFrameTick += 1;
		if (m_iLivesFrameTick == 3)
		{
			m_cLivesCharacterAnimation += 1;
			m_iLivesAnimationFrame += 1;
			m_iLivesFrameTick = 0;
		}
		return true;
	}
	m_cLivesCharacterAnimation = ' ';
	return false;
}

bool Pacman::LevelManagement_ResetLives(int iNumberOfLives)
{
	m_iNumberOfLives = 3;
	m_iAnimateLives = false;
	return true;
}

void Pacman::LevelManagement_ReturnBlankBits(array <array <tileCollectables, 31>, 28>* atc)
{
	array <array <tileCollectables, 31>, 28> returnATC;
	for (int x = 0; x < 28; x++)
	{
		for (int y = 0; y < 31; y++)
		{
			returnATC[x][y].bCollected = false;
			returnATC[x][y].iCollectable = -1;
		}
	}
	*atc = returnATC;
}

void Pacman::LevelManagement_PlaceBits(array <array <tileCollectables, 31>, 28>* atc, int iLevel)
{
	m_bCanCollect = false;

	tileCollectables blankBit; blankBit.iCollectable = -1;
	tileCollectables standardBit; standardBit.iCollectable = 0; standardBit.iScoreIncrease = 10; standardBit.iAbility = -1; standardBit.bContributesToCount = true; standardBit.bCollected = false;
	tileCollectables specialBit; specialBit.iCollectable = 1; specialBit.iScoreIncrease = 50; specialBit.iAbility = 0; specialBit.bContributesToCount = true; specialBit.bCollected = false;

	array <array <tileCollectables, 31>, 28> lclatc; *atc = lclatc; //Wipe Collectables

	m_iNumberOfBits = 0;//Reset the number of bits because none have been drawn.
	//Set all the bits for the level
	switch (iLevel)
	{
	case 1:
	//	LevelManagement_DrawBits(atc, 15, 20, 12, 1, standardBit);
	//	break;
	//case 3:
		
		//Horizontal
		LevelManagement_DrawBits(atc, 1, 1, 12, 1, standardBit);
		LevelManagement_DrawBits(atc, 15, 1, 12, 1, standardBit);

		LevelManagement_DrawBits(atc, 1, 5, 26, 1, standardBit);

		LevelManagement_DrawBits(atc, 1, 8, 6, 1, standardBit);
		LevelManagement_DrawBits(atc, 9, 8, 4, 1, standardBit);
		LevelManagement_DrawBits(atc, 15, 8, 4, 1, standardBit);
		LevelManagement_DrawBits(atc, 21, 8, 6, 1, standardBit);

		LevelManagement_DrawBits(atc, 1, 20, 12, 1, standardBit);
		LevelManagement_DrawBits(atc, 15, 20, 12, 1, standardBit);

		LevelManagement_DrawBits(atc, 1, 23, 3, 1, standardBit);
		LevelManagement_DrawBits(atc, 6, 23, 16, 1, standardBit);
		LevelManagement_DrawBits(atc, 24, 23, 3, 1, standardBit);

		LevelManagement_DrawBits(atc, 1, 26, 6, 1, standardBit);
		LevelManagement_DrawBits(atc, 9, 26, 4, 1, standardBit);
		LevelManagement_DrawBits(atc, 15, 26, 4, 1, standardBit);
		LevelManagement_DrawBits(atc, 21, 26, 6, 1, standardBit);

		LevelManagement_DrawBits(atc, 1, 29, 26, 1, standardBit);

		LevelManagement_DrawBits(atc, 13, 23, 2, 1, blankBit);//Position Pacman spawns has no bits

		//Vert
		LevelManagement_DrawBits(atc, 1, 1, 1, 8, standardBit);
		LevelManagement_DrawBits(atc, 1, 20, 1, 4, standardBit);
		LevelManagement_DrawBits(atc, 1, 26, 1, 4, standardBit);

		LevelManagement_DrawBits(atc, 3, 23, 1, 4, standardBit);

		LevelManagement_DrawBits(atc, 6, 1, 1, 26, standardBit);

		LevelManagement_DrawBits(atc, 9, 5, 1, 4, standardBit);
		LevelManagement_DrawBits(atc, 9, 23, 1, 4, standardBit);

		LevelManagement_DrawBits(atc, 12, 1, 1, 5, standardBit);
		LevelManagement_DrawBits(atc, 12, 20, 1, 4, standardBit);
		LevelManagement_DrawBits(atc, 12, 26, 1, 4, standardBit);

		LevelManagement_DrawBits(atc, 15, 1, 1, 5, standardBit);
		LevelManagement_DrawBits(atc, 15, 20, 1, 4, standardBit);
		LevelManagement_DrawBits(atc, 15, 26, 1, 4, standardBit);

		LevelManagement_DrawBits(atc, 18, 5, 1, 4, standardBit);
		LevelManagement_DrawBits(atc, 18, 23, 1, 4, standardBit);

		LevelManagement_DrawBits(atc, 21, 1, 1, 26, standardBit);

		LevelManagement_DrawBits(atc, 24, 23, 1, 4, standardBit);

		LevelManagement_DrawBits(atc, 26, 1, 1, 8, standardBit);
		LevelManagement_DrawBits(atc, 26, 20, 1, 4, standardBit);
		LevelManagement_DrawBits(atc, 26, 26, 1, 4, standardBit);

		//Special Bits:
		LevelManagement_DrawBits(atc, 1, 3, 1, 1, specialBit);//Top Left
		LevelManagement_DrawBits(atc, 26, 3, 1, 1, specialBit);//Top Right
		LevelManagement_DrawBits(atc, 1, 23, 1, 1, specialBit);//Bottom Left
		LevelManagement_DrawBits(atc, 26, 23, 1, 1, specialBit);//Bottom Left
		break;
	case 2:
		//Horizontal
		LevelManagement_DrawBits(atc, 1, 1, 26, 1, standardBit);

		LevelManagement_DrawBits(atc, 4, 5, 6, 1, standardBit);
		LevelManagement_DrawBits(atc, 18, 5, 6, 1, standardBit);

		LevelManagement_DrawBits(atc, 12, 4, 4, 1, standardBit);

		LevelManagement_DrawBits(atc, 1, 8, 6, 1, standardBit);
		LevelManagement_DrawBits(atc, 9, 8, 4, 1, standardBit);
		LevelManagement_DrawBits(atc, 15, 8, 4, 1, standardBit);
		LevelManagement_DrawBits(atc, 21, 8, 6, 1, standardBit);

		LevelManagement_DrawBits(atc, 3, 11, 4, 1, standardBit);
		LevelManagement_DrawBits(atc, 21, 11, 4, 1, standardBit);

		LevelManagement_DrawBits(atc, 3, 18, 4, 1, standardBit);
		LevelManagement_DrawBits(atc, 21, 18, 4, 1, standardBit);

		LevelManagement_DrawBits(atc, 6, 20, 4, 1, standardBit);
		LevelManagement_DrawBits(atc, 18, 20, 4, 1, standardBit);

		LevelManagement_DrawBits(atc, 1, 23, 9, 1, standardBit);
		LevelManagement_DrawBits(atc, 18, 23, 9, 1, standardBit);

		LevelManagement_DrawBits(atc, 4, 26, 3, 1, standardBit);
		LevelManagement_DrawBits(atc, 9, 26, 10, 1, standardBit);
		LevelManagement_DrawBits(atc, 21, 26, 3, 1, standardBit);

		LevelManagement_DrawBits(atc, 1, 29, 12, 1, standardBit);
		LevelManagement_DrawBits(atc, 15, 29, 12, 1, standardBit);

		//Vert
		LevelManagement_DrawBits(atc, 1, 1, 1, 8, standardBit);
		LevelManagement_DrawBits(atc, 4, 1, 1, 5, standardBit);
		LevelManagement_DrawBits(atc, 9, 1, 1, 8, standardBit);
		LevelManagement_DrawBits(atc, 12, 4, 1, 5, standardBit);
		LevelManagement_DrawBits(atc, 15, 4, 1, 5, standardBit);
		LevelManagement_DrawBits(atc, 18, 1, 1, 8, standardBit);
		LevelManagement_DrawBits(atc, 23, 1, 1, 5, standardBit);
		LevelManagement_DrawBits(atc, 26, 1, 1, 8, standardBit);

		LevelManagement_DrawBits(atc, 6, 6, 1, 2, standardBit);
		LevelManagement_DrawBits(atc, 21, 6, 1, 2, standardBit);

		LevelManagement_DrawBits(atc, 3, 9, 1, 2, standardBit);
		LevelManagement_DrawBits(atc, 24, 9, 1, 2, standardBit);

		LevelManagement_DrawBits(atc, 6, 11, 1, 10, standardBit);
		LevelManagement_DrawBits(atc, 21, 11, 1, 10, standardBit);

		LevelManagement_DrawBits(atc, 3, 18, 1, 6, standardBit);
		LevelManagement_DrawBits(atc, 24, 18, 1, 6, standardBit);

		LevelManagement_DrawBits(atc, 9, 20, 1, 7, standardBit);
		LevelManagement_DrawBits(atc, 18, 20, 1, 7, standardBit);

		LevelManagement_DrawBits(atc, 1, 23, 1, 7, standardBit);
		LevelManagement_DrawBits(atc, 26, 23, 1, 7, standardBit);

		LevelManagement_DrawBits(atc, 6, 24, 1, 2, standardBit);
		LevelManagement_DrawBits(atc, 21, 24, 1, 2, standardBit);

		LevelManagement_DrawBits(atc, 4, 27, 1, 2, standardBit);
		LevelManagement_DrawBits(atc, 23, 27, 1, 2, standardBit);

		LevelManagement_DrawBits(atc, 12, 27, 1, 2, standardBit);
		LevelManagement_DrawBits(atc, 15, 27, 1, 2, standardBit);

		//Special Bits:
		LevelManagement_DrawBits(atc, 1, 3, 1, 1, specialBit);//Top Left
		LevelManagement_DrawBits(atc, 26, 3, 1, 1, specialBit);//Top Right
		LevelManagement_DrawBits(atc, 1, 27, 1, 1, specialBit);//Bottom Left
		LevelManagement_DrawBits(atc, 26, 27, 1, 1, specialBit);//Bottom Left
		break;
	default:
		Tileset_DrawTileset(iLevel);
		break;
	}
	m_iNumberOfBits = LevelManagement_CountBits(atc); //Number of bits
}

int Pacman::LevelManagement_CountBits(array <array <tileCollectables, 31>, 28>* atc)
{
	array <array <tileCollectables, 31>, 28> atcWork = *atc;
	int iNumberOfBits = 0;
	for (int x = 0; x < 28; x++)
		for (int y = 0; y < 31; y++)
			if (atcWork[x][y].iCollectable > -1 && atcWork[x][y].bContributesToCount && !atcWork[x][y].bCollected)
					iNumberOfBits++;
	return iNumberOfBits;
}

void Pacman::LevelManagement_DrawBits(array <array <tileCollectables, 31>, 28>* atc, int x, int y, int width, int height, tileCollectables t)
{
	if (x > 27 || x < 0 || y > 30 || y < 0 || x + width > 28 || y + height > 31)
	{
		cout << "Tileset_DrawBox: Could not draw box at: x: " << x << " y: " << y << endl;
	}
	else
	{
		array <array <tileCollectables, 31>, 28> returnArray = *atc;

		for (int i = x; i < x + width; i++)
			for (int j = y; j < y + height; j++)
				returnArray[i][j] = t;
		*atc = returnArray;
	}
}

bool Pacman::LevelManagement_CreateLevelTiles(int level, bool bDrawTileset)
{
	//Load a level
	if(bDrawTileset)
		m_aatTilemap = Tileset_DrawTileset(level);
	array <float, 3> afPacPos = Tileset_StartPositionPacman(level);
	m_vPacmanPosition->X = afPacPos[0] - (m_fSpriteWidthHeight / 2);
	m_vPacmanPosition->Y = afPacPos[1] - (m_fSpriteWidthHeight / 2);
	m_iMovementKeyPress = (int)afPacPos[2];
	*m_rPacmanSourceRect = PlayerMovement_UpdateDirection(m_iMovementKeyPress);//Turn Pacman in image. This generally wouldn't happen as movement is stopped on round/level load

	return true;
}

bool Pacman::LevelManagement_SendPlayerLocation(Sprites* sprMain)
{
	return sprMain->UpdatePacmanLocation(m_vPacmanPosition, cpp);
}

bool Pacman::LevelManagement_SetPacmanSprite(int iSprite)
{
	if (iSprite == -1)
		iSprite = m_iLevelNumber;

	switch (iSprite)
	{
	case 2:
		m_t2dPacmanTexture->Load("Textures/MsPacman_Sprite_Bigger.png", false);
		m_t2dPacman2Texture->Load("Textures/MsPacman_Sprite_Bigger.png", false);
		break;
	default:
	case 1:
		m_t2dPacmanTexture->Load("Textures/Pacman_Sprite_Bigger.png", false);
		m_t2dPacman2Texture->Load("Textures/Pacman_Sprite_Bigger.png", false);
		break;
	}

	return true;
}

bool Pacman::LevelManagement_AwardedLivesUpdate()
{
	int iScoreInLives = m_iCurrentGameScore / 10000;
	if (iScoreInLives > m_iScoreForNewLives)
	{
		if (m_iNumberOfLives == 10)
		{
			Audio_PlaySoundEffect(4, true);//Bad Sound
			m_iScoreForNewLives++;
		}
		else
		{
			Audio_PlaySoundEffect(5, true);//LevelUpSound Effect
			m_iNumberOfLives++;
			m_iTargetLives++;
			m_iScoreForNewLives++;
		}
	}
	return true;
}