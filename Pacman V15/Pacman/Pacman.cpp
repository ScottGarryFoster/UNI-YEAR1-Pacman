#include "Pacman.h"
#include "Sprites.h"
#include "FileManagement.h"

#include <sstream>
#include <array>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv)
{
	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, (24*28), (24*35), false, 25, 25, "Pacman", 60);
	Input::Initialise();
	Audio::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	//Main Pacman
	delete m_vPacmanPosition;
	delete m_rPacmanSourceRect;
	delete m_t2dPacmanTexture;
	//AcrossTheScreenPacman Pacman
	delete m_vPacman2Position;
	delete m_rPacman2SourceRect;
	delete m_t2dPacman2Texture;

	//MetaData for the Pacman sprites and Tilemap
	delete &m_fSpriteWidthHeight;
	delete &m_fTilemapWidthHeight;
	delete &m_fTilemapMarginTop;

	//Inventory
	delete m_vInventory;
	delete m_rInventory;
	delete m_tInventory;

	//Tilemap:
	delete m_vTilePosition;
	delete m_rTileRect;
	delete m_tTileTexture;
	delete &m_aatTilemap;

	//PlayerMovement
	delete m_vPacmanCenter;
	delete &m_iBufferKeyPress;
	delete &m_iMovementKeyPress;
	delete &m_bPlayerStill;

	//PlayerVars
	delete &m_iAnimationCount;
	delete &m_bAnimationDirection;
	delete &m_iFrameCount;
	delete &m_iFrameToAct;

	//Utility
	delete &m_iTimeSecondsTotal;
	delete &m_bOnASecondFrame;
	delete &m_iElapsedSeconds;
	delete &m_iElapsedMinutes;
	delete &m_iElapsedHours;
	delete &m_iFpsUpdateSecond;
	delete &m_iCurrentFPS;
}

void Pacman::LoadContent()
{
	srand(time(NULL));//Make random numbers kind of random

	PlayerLoadContent();
	TilesetLoadContent();
	MenuLoadContent();
	LevelManagementLoadContent();
	CollectablesLoadContent();
	AudioLoadContent();
	UtilityLoadContent();

	m_sprMain = new Sprites();
	m_sprMain->LoadContent(&m_aatTilemap);
}

void Pacman::Update(int elapsedTime)
{
	iTimes = 0;
	if (m_iSubMenuLocation == 0)
	{
		UpdateClassPacket(elapsedTime);
		UtilityUpdateTime(elapsedTime); //Just used for literal time

		if (m_bLevelHasStartted)
		{
			if (m_bPlayerCanMove && !m_sprMain->GetCanGhostsMove())
				PlayerMovement_Update(elapsedTime);

			m_sprMain->Update(&cpp, m_vPacmanPosition);

			m_sprMain->MoveGhosts(cpp);

			Collectables_Update(elapsedTime, m_sprMain);
		}
		LevelManagement_Update(elapsedTime, m_sprMain); //Handles start of a new game and stage of a game
		
		Audio_Update();
	}
	
	Menu_Update(elapsedTime);
	m_bResetSpecialBit = false;//Only active once
}

void Pacman::Draw(int elapsedTime)
{

	SpriteBatch::BeginDraw(); // Starts Drawing
	
	//If in a game and not in a pause menu. This just stops the flash at the start and wasting mem
	if (m_iMenuLocation >= 3 && m_iSubMenuLocation == 0)
	{
		//Draw Tiles - Pacman Tiles
		for (int x = 0; x < 28; x++)
		{
			for (int y = 0; y < 31; y++)
			{
				if (m_aatTilemap[x][y].iTileshown == 0)
				{
					if (m_aatTilemap[x][y].bPassable)
						m_rTileRect->X = 6 * m_fTilemapWidthHeight;
					else
						m_rTileRect->X = 7 * m_fTilemapWidthHeight;
				}
				else
				{
					m_rTileRect->X = (m_aatTilemap[x][y].iTileshown % 10) * m_fTilemapWidthHeight;
					m_rTileRect->Y = (int)(m_aatTilemap[x][y].iTileshown / 10) * m_fTilemapWidthHeight;
				}
				
				m_vTilePosition->X = x * m_fTilemapWidthHeight;
				m_vTilePosition->Y = (y * m_fTilemapWidthHeight) + m_fTilemapMarginTop;
				SpriteBatch::Draw(m_tTileTexture, m_vTilePosition, m_rTileRect);
			}
		}

		//Draw Collectables
		for (int x = 0; x < 28; x++)
		{
			for (int y = 0; y < 31; y++)
			{
				if (m_atcCollectables[x][y].iCollectable == -1 || m_atcCollectables[x][y].bCollected)
					continue;

				//cout << "m_atcCollectables[x][y].iCollectable:" << m_atcCollectables[x][y].iCollectable << endl;
				m_rTileCollectRect->X = (m_atcCollectables[x][y].iCollectable % 5) * m_fTilemapWidthHeight;
				m_rTileCollectRect->Y = (int)(m_atcCollectables[x][y].iCollectable / 5) * m_fTilemapWidthHeight;

				m_vTileCollectPosition->X = x * m_fTilemapWidthHeight;
				if (m_atcCollectables[x][y].bNudgeRight)
					m_vTileCollectPosition->X += m_fTilemapWidthHeight / 2;

				m_vTileCollectPosition->Y = (y * m_fTilemapWidthHeight) + m_fTilemapMarginTop;
				SpriteBatch::Draw(m_tTileCollectTexture, m_vTileCollectPosition, m_rTileCollectRect);
			}
		}

		SpriteBatch::Draw(m_t2dPacmanTexture, m_vPacmanPosition, m_rPacmanSourceRect); // Draws Pacman
		SpriteBatch::Draw(m_t2dPacman2Texture, m_vPacman2Position, m_rPacman2SourceRect); // Draws Pacman2

		m_sprMain->Draw(&cpp);
	}
	

	//Draw Tiles - Menu UI BACKGROUND tiles
	for (int x = 0; x < 28; x++)
	{
		for (int y = 0; y < 35; y++)
		{
			if (m_auiMenuBackgroundTiles[x][y].iTileshown == -1)
				continue;

			m_rUITleRect->X = (m_auiMenuBackgroundTiles[x][y].iTileshown % 26) * m_iUITileSize;
			m_rUITleRect->Y = (int)(m_auiMenuBackgroundTiles[x][y].iTileshown / 26) * m_iUITileSize;
			
			m_vUITilePosition->X = x * m_iUITileSize;
			m_vUITilePosition->Y = (y * m_iUITileSize);
			SpriteBatch::Draw(m_tUITileTexture, m_vUITilePosition, m_rUITleRect);
		}
	}
	/*
	Didn't have time
	//Inventory UI:
	if (m_iLevelNumber == 3)
		SpriteBatch::Draw(m_tInventory, m_vInventory, m_rInventory);
		*/
	//Draw Tiles - Menu UI tiles
	for (int x = 0; x < 28; x++)
	{
		for (int y = 0; y < 35; y++)
		{
			if (m_auiMenuTiles[x][y].iTileshown == -1)
				continue;

			m_rUITleRect->X = (m_auiMenuTiles[x][y].iTileshown % 26) * m_iUITileSize;
			m_rUITleRect->Y = (int)(m_auiMenuTiles[x][y].iTileshown / 26) * m_iUITileSize;

			m_vUITilePosition->X = x * m_iUITileSize;
			if (m_auiMenuTiles[x][y].bShiftTileLocationRight)
				m_vUITilePosition->X += m_iUITileSize / 2;

			m_vUITilePosition->Y = (y * m_iUITileSize);
			SpriteBatch::Draw(m_tUITileTexture, m_vUITilePosition, m_rUITleRect);
		}
	}

	SpriteBatch::Draw(m_t2dGameLogoTex, m_vGameLogoPos, m_rGameLogoRec); // Draws Pacman Logos

	SpriteBatch::EndDraw(); // Ends Drawing
}