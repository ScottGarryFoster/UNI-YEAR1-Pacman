#include "Pacman.h"

#include <sstream>
#include <array>
#include <string>
#include <iostream>

bool Pacman::LevelManagementLoadContent()
{

}

bool Pacman::LevelManagement_Update()
{
	if (!m_bLevelHasStartted && m_bCanStartLevel)
	{

	}
}

bool Pacman::LevelManagement_StartNewLevel()
{
	if (m_bIsNewGame)
	{
		m_iLevelNumber = 1; //This is the first level
		m_iLmLives = 3; //Reset lives
	}
	
}

void Pacman::LevelManagement_ReturnBlankBits(array <array <tileCollectables, 31>, 28>* atc)
{
	array <array <tileCollectables, 31>, 28> returnATC;
	for (int x = 0; x < 28; x++)
	{
		for (int y = 0; y < 31; y++)
		{
			returnATC[x][y].bCollected = false;
			returnATC[x][y].iCollectable = 0;
		}
	}
	*atc = returnATC;
}

void Pacman::LevelManagement_PlaceBits(array <array <tileCollectables, 31>, 28>* atc, int iLevel)
{
	array <array <tileCollectables, 31>, 28> returnATC = *atc;
	//Set all the bits for the level
}