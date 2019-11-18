#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING

#include "Pacman.h"
#include "Sprites.h"
#include "Player.h"
#include "Ghost.h"
#include "General.h"

#include <sstream>
#include <array>
#include <string>
#include <iostream>

using namespace S2D;

Sprites::Sprites()
{
	for (int i = 0; i < 4; i++)
		m_ghostEnemies[i] = new Ghost(i);
	
	m_playerSpr = new Player();
}

/*Sprites::~Sprites()
{
	
}*/

void Sprites::LoadContent(array <array <tile, 31>, 28>* aat)
{
	for (int i = 0; i < 4; i++)
		m_ghostEnemies[i]->LoadContent(aat);
	//m_playerSpr->LoadContent();
}

bool Sprites::Update(classPacketPacman* pacman, Vector2* vPlayerPos)
{
	m_iRoundNumber = pacman->iRoundNumber;
	m_iLevelNumber = pacman->iLevelNumber;//Set the level number

	m_playerSpr->setPosition(vPlayerPos, *pacman);

	if (!*pacman->ptrBLevelHasStartted) return false;//DO not continue;
	GhostsGetEaten(pacman);
	PacmanGetEaten(pacman);

	if (m_bEatenGhost)
		ShowScoreAndUpdateEatenGhost(pacman);
	else if (m_bPacmanDeath)
		KillPacman(pacman, vPlayerPos);

	if (m_bPacmanDeath) return false;

	for (int i = 0; i < 4; i++)
	{
		m_ghostEnemies[i]->Ghosts_Update(*pacman);

		if(pacman->bResetSpecialBit)
			m_ghostEnemies[i]->AIRunOncePerSpecialBit();
	}


	return true;
}

bool Sprites::ShowScoreAndUpdateEatenGhost(classPacketPacman* pacman)
{
	int iGivenScore = 0;
	if (m_iTimeToDelay == 0)
	{
		m_iTimeToDelay = 500;
		tilePosition tpTilePos = m_playerSpr->getTile();
		int iTileX = tpTilePos.getCord(), iTileY = tpTilePos.getCord(true) + 1;

		m_iGhostsEatenThisVunState++;
		iGivenScore = GetEatenGhostScore();
		*pacman->ptrICurrentGameScore += iGivenScore;

		uiTile ut;
		ut.iTileshown = (26 * 15) + Pacman::UtilityScoreToTileLocation(iGivenScore);
		ut.iSheet = 0;
		pacman->SetMenuTiles(iTileX, iTileY, ut);//Sets the one in the main class using pointers
	}
	m_iTimeCount += pacman->iElapsedTime;

	if (m_iTimeCount >= m_iTimeToDelay)
	{
		tilePosition tpTilePos = m_playerSpr->getTile();
		int iTileX = tpTilePos.getCord(), iTileY = tpTilePos.getCord(true) + 1;

		uiTile ut;
		ut.iTileshown = -1;
		ut.iSheet = -1;
		pacman->SetMenuTiles(iTileX, iTileY, ut);//Sets the one in the main class using pointers

		m_bStopGhosts = false;
		m_bEatenGhost = false;
		m_iTimeCount = 0;
	}

	return true;
}

int Sprites::GetEatenGhostScore()
{
	switch (m_iGhostsEatenThisVunState)
	{
	case 2:
		return 400;
	case 3:
		return 800;
	case 4:
		return 1600;
	case 1:
	default:
		return 200;
	}
}

bool Sprites::KillPacman(classPacketPacman* pacman, Vector2* vPlayerPos)
{
	if (!m_bStopGhosts)
	{
		m_bStopGhosts = true;
		m_bEatenGhost = false;
		m_iDeathFrame = -8;
		*pacman->ptrBPlayPacmanDeath = true;
		
		//Change over the Pacman sprite
		pacman->tex2dPlayerTex->Load("Textures/Pacman_Sprite_Bigger_Death.png", false);

		float fRectY = 0;
		switch (pacman->iPacmanDirection)
		{
		case 8:
			fRectY = m_playerSpr->getSpriteWidthHeight() * 2;
			break;
		case 4:
			fRectY = m_playerSpr->getSpriteWidthHeight() * 1;
			break;
		case 2:
			fRectY = m_playerSpr->getSpriteWidthHeight() * 3;
			break;
		}
		pacman->rPlayerRect->X = 0;
		pacman->rPlayerRect->Y = fRectY;
		//cout << "pacman->rPlayerRect->X:" << pacman->rPlayerRect->X << endl;
	}
	m_iDeathFrame++;

	if (m_iDeathFrame >= m_iDeathNextFrame)
	{
		if (pacman->rPlayerRect->X < m_playerSpr->getSpriteWidthHeight() * 4)
			pacman->rPlayerRect->X += m_playerSpr->getSpriteWidthHeight();
		else if (pacman->rPlayerRect->Y != m_playerSpr->getSpriteWidthHeight() * 4)
		{
			pacman->rPlayerRect->Y = m_playerSpr->getSpriteWidthHeight() * 4;
			pacman->rPlayerRect->X = 0;
		}
		else if(pacman->vPlayerVector->X != -9001)
		{
			pacman->vPlayerVector->X = -9001;
			pacman->vPlayerVector->Y = -9001;
		}
		else
		{
			PacmanDeath(pacman);
			//*pacman->ptrBPacmanDeath = true;
			m_bPacmanDeath = false;
			m_bStopGhosts = false;
			m_bEatenGhost = false;
		}
		m_iDeathFrame -= m_iDeathNextFrame;
	}

	return false;
}

bool Sprites::PacmanDeath(classPacketPacman* pacman)
{
	*pacman->ptrITargetLives = *pacman->ptrINumberOfLives - 1;
	*pacman->ptrBCanStartLevel = true;
	*pacman->ptrBLevelHasStartted = true;
	*pacman->ptrIsNewGame = false;
	
	return true;
}



bool Sprites::GhostsGetEaten(classPacketPacman* pacman)
{
	if (pacman->bGhostAI_SpecialBitActive)
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_ghostEnemies[i]->getHaveBeenCapturedThisSpecial())//If they've already been caught once
				continue;//Don't need to bother checking

			if (m_bEatenGhost)
				continue;

			float fMarginForTurning = 10.0f;

			Vector2* ghostPos = m_ghostEnemies[i]->getCenterPosition();
			Vector2* ghostShadowPos = m_ghostEnemies[i]->getCenterShadowPosition();
			Vector2* vPlayerCenterPos = m_playerSpr->getCenterPosition();

			if (Pacman::UtilityCollisionPosCheck(vPlayerCenterPos, ghostPos, fMarginForTurning) || Pacman::UtilityCollisionPosCheck(vPlayerCenterPos, ghostShadowPos, fMarginForTurning))
			{
				m_bStopGhosts = true;
				m_ghostEnemies[i]->setCaught();
				m_bEatenGhost = true;//Used to show score and make Pacman move again
				m_iTimeToDelay = 0;
				*pacman->ptrBPlayGhostDeath = true;
				break;
			}
		}
	}

	return true;
}

bool Sprites::PacmanGetEaten(classPacketPacman* pacman)
{
	for (int i = 0; i < 4; i++)
	{
		if (m_bEatenGhost)//If we're eating a ghost don't check anything
			continue;

		if (!m_ghostEnemies[i]->getHaveBeenCapturedThisSpecial() && pacman->bGhostAI_SpecialBitActive || m_ghostEnemies[i]->GetBehaviour() == gbCaught)//If they've already been caught once
			continue;//Don't need to bother checking

		float fMarginForTurning = 10.0f;

		Vector2* ghostPos = m_ghostEnemies[i]->getCenterPosition();
		Vector2* ghostShadowPos = m_ghostEnemies[i]->getCenterShadowPosition();
		Vector2* vPlayerCenterPos = m_playerSpr->getCenterPosition();

		if (Pacman::UtilityCollisionPosCheck(vPlayerCenterPos, ghostPos, fMarginForTurning) || Pacman::UtilityCollisionPosCheck(vPlayerCenterPos, ghostShadowPos, fMarginForTurning))
		{
			m_bPacmanDeath = true;
			break;
		}
	}
	return true;
}

bool Sprites::MoveGhosts(classPacketPacman pacman)
{
	int elapsedTime = pacman.iElapsedTime;
	for (int i = 0; i < 4; i++)
	{
		if (!pacman.bGhostsCanMove)
			continue;
		/*
			BIT COMPLICATED THE BIT BELOW SO:
			If Stop Ghosts is True - Set when Pacman eats a special
								AND
			Their Behaviour isn't caught - Meaning they're the one which was caught

			(Modified Value I'm playing with is adding:)
								AND
			They've not been caught this special - Meaning they've been caught and hence aren't subject to stopping

			This creates a special bit movement system that is -close enough- for me to Real Pacman.
		*/
		//if (m_bStopGhosts && m_ghostEnemies[i]->GetBehaviour() != gbCaught && !m_ghostEnemies[i]->getHaveBeenCapturedThisSpecial())
		if (m_bStopGhosts && m_ghostEnemies[i]->GetBehaviour() != gbCaught)
			continue;

		m_ghostEnemies[i]->Ghosts_Move(pacman, m_playerSpr, m_ghostEnemies);
	}
		
	return true;
}

bool Sprites::UpdateGhost(Ghost* ghost)
{
	
	return true;
}

bool Sprites::SpawnInPen(int elapsedTime, classPacketPacman pacman)
{
	for (int i = 0; i < 4; i++)
		m_ghostEnemies[i]->Spawn(pacman, true);
	return true;
}

void Sprites::Draw(classPacketPacman* pacman)
{
	//SpriteBatch::Draw(m_t2dPacman2Texture, m_vPacman2Position, m_rPacman2SourceRect); // Draws Pacman2
	if (!m_bPacmanDeath && !pacman->bHideGhosts)
	{
		for (int i = 0; i < 4; i++)
		{
			SpriteBatch::Draw(m_ghostEnemies[i]->getTexture(), m_ghostEnemies[i]->getPosition(), m_ghostEnemies[i]->getRect()); // Draws Ghost
			SpriteBatch::Draw(m_ghostEnemies[i]->getTexture(true), m_ghostEnemies[i]->getPosition(true), m_ghostEnemies[i]->getRect(true)); // Draws Ghost
		}
	}
}

bool Sprites::RunOncePerSpecialBit()
{
	for (int i = 0; i < 4; i++)
		m_ghostEnemies[i]->AIRunOncePerSpecialBit();

	m_iGhostsEatenThisVunState = 0;
	return true;
}