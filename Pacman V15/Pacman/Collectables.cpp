#include "Pacman.h"

#include <sstream>
#include <array>
#include <string>
#include <iostream>

bool Pacman::CollectablesLoadContent()
{
	CollectablesLoadAbilities();

	m_vInventory = new Vector2(0, 0);
	m_rInventory = new Rect(0, 0, 72.0f, 72.0f);
	m_tInventory = new Texture2D();
	m_tInventory->Load("Textures/Menu/ui_inventory.png", false);
	return true;
}

bool Pacman::CollectablesLoadAbilities()
{
	int iCount = 0;
	//Main Special bit:
	m_taArray[iCount].bPerformImmediately = true;
	m_taArray[iCount].bTimed = true;
	m_taArray[iCount].iTimedSeconds = 10;
	m_taArray[iCount].bVunrabilityState = true;
	//Blast Bit:
	m_taArray[++iCount].bPerformImmediately = false;
	m_taArray[iCount].bTimed = false;
	m_taArray[iCount].iTimedSeconds = -1;
	m_taArray[iCount].bVunrabilityState = false;
	m_taArray[iCount].bDestroyGhostsInArea = true;

	return true;
}

//bool Pacman::Collectables_Update(int elapsedTime)
//{
//	array <int, 2> aiCurrentTile = PlayerMovement_UpdateCurrentPosition(m_vPacmanCenter);
//	if (m_atcCollectables[aiCurrentTile[0]][aiCurrentTile[1]].iCollectable != -1 && !m_atcCollectables[aiCurrentTile[0]][aiCurrentTile[1]].bCollected)
//	{
//		array <float, 2> currentTilepos = Tileset_ReturnCordOfTile(aiCurrentTile[0], aiCurrentTile[1]);
//		float fCurrentTileX = currentTilepos[0], fCurrentTileY = currentTilepos[1];
//		Vector2 vCurrentTile; vCurrentTile.X = currentTilepos[0]; vCurrentTile.Y = currentTilepos[1];
//
//		float fMarginForTurning = 5.0f;
//
//		//if(UtilityCollisionPosCheck(&vCurrentTile, m_vPacmanCenter, fMarginForTurning))
//		if (m_vPacmanCenter->X - fMarginForTurning <= fCurrentTileX && m_vPacmanCenter->X + fMarginForTurning >= fCurrentTileX && m_vPacmanCenter->Y - fMarginForTurning <= fCurrentTileY && m_vPacmanCenter->Y + fMarginForTurning >= fCurrentTileY)
//		{
//			//Basically if they're within a small region around the bit collect it:
//			m_atcCollectables[aiCurrentTile[0]][aiCurrentTile[1]].bCollected = true;//This happens for all bits
//			m_iCurrentGameScore += m_atcCollectables[aiCurrentTile[0]][aiCurrentTile[1]].iScoreIncrease;//Add the score
//			if (m_atcCollectables[aiCurrentTile[0]][aiCurrentTile[1]].iAbility > 0)
//			{
//				Collectables_Abilities(elapsedTime, m_atcCollectables[aiCurrentTile[0]][aiCurrentTile[1]].iAbility);
//			}
//			m_iNumberOfBits -= 1;//One bit has been collected
//			m_fPacmanSpeedModify = m_fPacmanModifyForBits;//Pacman moves slower when eating bits
//			m_aiLastTileOnCollection = aiCurrentTile;
//			m_iCollablesCollectedThisRound++; //Used for Escaping ghosts
//			
//			Audio_PlaySoundEffect(0, false);
//		}
//	}
//	else if(m_atcCollectables[aiCurrentTile[0]][aiCurrentTile[1]].iCollectable == -1)
//	{
//		m_aiLastTileOnCollection = { -1,-1 };
//	}
//
//	if (m_aiLastTileOnCollection != aiCurrentTile)
//	{
//		m_fPacmanSpeedModify = 0;
//		m_aiLastTileOnCollection = { -1,-1 };
//	}
//	
//	if (m_iCollectableAffectSeconds != -1)
//	{
//		//Special bit time
//		if (m_iCollectableAffectSeconds >= UtilityReturnSeconds() && !m_bGhostAI_SpecialBitActive)
//		{
//			m_bGhostAI_SpecialBitActive = true;
//		}
//		else if (m_iCollectableAffectSeconds - 3 < UtilityReturnSeconds() && !m_bGhostAI_SpecialBitClose && m_bGhostAI_SpecialBitActive)
//		{
//			m_bGhostAI_SpecialBitClose = true;
//		}
//		else if (m_iCollectableAffectSeconds < UtilityReturnSeconds() && m_bGhostAI_SpecialBitActive)
//		{
//			m_bGhostAI_SpecialBitClose = false;
//			m_bGhostAI_SpecialBitActive = false;
//			m_iCollectableAffectSeconds = -1;
//		}
//	}
//
//
//	return true;
//}

bool Pacman::Collectables_Update(int elapsedTime, Sprites* spr)
{
	if (!m_bCanCollect)
		return false;

	Collectables_UpdateAnimations();//Animates fruit

	array <int, 2> aiCurrentTile = PlayerMovement_UpdateCurrentPosition(m_vPacmanCenter);
	if (m_atcCollectables[aiCurrentTile[0]][aiCurrentTile[1]].iCollectable != -1 && !m_atcCollectables[aiCurrentTile[0]][aiCurrentTile[1]].bCollected)
	{
		array <float, 2> currentTilepos = Tileset_ReturnCordOfTile(aiCurrentTile[0], aiCurrentTile[1]);
		float fCurrentTileX = currentTilepos[0], fCurrentTileY = currentTilepos[1];

		Vector2 vCurrentTile; vCurrentTile.X = currentTilepos[0]; vCurrentTile.Y = currentTilepos[1];

		if (m_atcCollectables[aiCurrentTile[0]][aiCurrentTile[1]].bNudgeRight)
			vCurrentTile.X += m_fTilemapWidthHeight / 2;//Just makes the hit box perfectly on the collectable;

		float fMarginForTurning = 10.0f;

		if (UtilityCollisionPosCheck(&vCurrentTile, m_vPacmanCenter, fMarginForTurning))//Basically if they're within a small region around the bit collect it:
		{
			m_atcCollectables[aiCurrentTile[0]][aiCurrentTile[1]].bCollected = true;//This happens for all bits
			m_iCurrentGameScore += m_atcCollectables[aiCurrentTile[0]][aiCurrentTile[1]].iScoreIncrease;//Add the score

			if (m_atcCollectables[aiCurrentTile[0]][aiCurrentTile[1]].iAbility >= 0)
				Collectables_Abilities(elapsedTime, m_atcCollectables[aiCurrentTile[0]][aiCurrentTile[1]].iAbility, spr);

			m_fPacmanSpeedModify = m_fPacmanModifyForBits;//Pacman moves slower when eating bits
			m_aiLastTileOnCollection = aiCurrentTile;//Used for Speed Modification

			if (m_atcCollectables[aiCurrentTile[0]][aiCurrentTile[1]].bContributesToCount)
			{
				m_iNumberOfBits -= 1;//One bit has been collected
				m_iCollablesCollectedThisRound++; //Used for Escaping ghosts
			}

			//This handles the score appearing on the screen.
			if (m_atcCollectables[aiCurrentTile[0]][aiCurrentTile[1]].bShowScoreOnTile)
			{
				uiTile ut;
				ut.iTileshown = (26 * 15) + Pacman::UtilityScoreToTileLocation(m_atcCollectables[aiCurrentTile[0]][aiCurrentTile[1]].iScoreIncrease);
				ut.iSheet = 0;
				ut.bShiftTileLocationRight = true;
				m_auiMenuTiles[aiCurrentTile[0]][aiCurrentTile[1] + 2] = ut;

				m_atcCollectables[aiCurrentTile[0]][aiCurrentTile[1]].iScoreOnTileTime = UtilityTimeInTheFuture(3);
				m_atcCollectables[aiCurrentTile[0]][aiCurrentTile[1]].ut = &m_auiMenuTiles[aiCurrentTile[0]][aiCurrentTile[1] + 2];
			}

			Audio_PlaySoundEffect(0, false);
		}
	}
	else if (m_atcCollectables[aiCurrentTile[0]][aiCurrentTile[1]].iCollectable == -1 || m_atcCollectables[aiCurrentTile[0]][aiCurrentTile[1]].bCollected)
	{
		m_aiLastTileOnCollection = { -1,-1 };
	}

	if (m_aiLastTileOnCollection != aiCurrentTile)
	{
		m_fPacmanSpeedModify = 0;
		m_aiLastTileOnCollection = { -1,-1 };
	}

	if (m_iCollectableAffectSeconds != -1)
	{
		if (m_taArray[m_iCurrentActiveBit].bVunrabilityState)
		{
			if (!Audio_UpdateHasSoundPlaying(2) && !m_bPlayGhostDeath)
			{
				if (m_iMusicTimeLeftOver != -1)
					Audio_ResumeMusic();
				else
					Audio_PlayMusic();
			}
			else if (m_bPlayGhostDeath)
			{
				Audio_PauseMusic();
				Audio_PlaySoundEffect(2, false);
			}

			m_bPlayGhostDeath = false;
				

			if (m_iCollectableAffectSeconds >= UtilityReturnSeconds() && !m_bGhostAI_SpecialBitActive)
			{
				m_bGhostAI_SpecialBitActive = true;
			}
			else if (m_iCollectableAffectSeconds - 3 < UtilityReturnSeconds() && !m_bGhostAI_SpecialBitClose && m_bGhostAI_SpecialBitActive)
			{
				m_bGhostAI_SpecialBitClose = true;
			}
			else if (m_iCollectableAffectSeconds < UtilityReturnSeconds() && m_bGhostAI_SpecialBitActive)
			{
				m_bGhostAI_SpecialBitClose = false;
				m_bGhostAI_SpecialBitActive = false;
				spr->RunOncePerSpecialBit();
			}
		}

		if (m_iCollectableAffectSeconds < UtilityReturnSeconds())//If the duration end is before
		{
			m_iCollectableAffectSeconds = -1;
			m_bGhostAI_SpecialBitClose = false;
			m_bGhostAI_SpecialBitActive = false;
			spr->RunOncePerSpecialBit();
			Audio_StopMusic();
		}
			
	}

	Collectables_UpdateFruitScore();//Used to remove the tile score shown (fruit in default game)

	return true;
}

void Pacman::Collectables_UpdateFruitScore(bool bEraseScore)
{
	//Removes score if on tile
	for (int x = 0; x < 28; x++)
		for (int y = 0; y < 31; y++)
		{
			if (m_atcCollectables[x][y].iScoreOnTileTime != -1 && m_atcCollectables[x][y].iScoreOnTileTime <= UtilityReturnSeconds())
			{
				m_atcCollectables[x][y].ut->bShiftTileLocationRight = false;
				m_atcCollectables[x][y].ut->iTileshown = -1;
				m_atcCollectables[x][y].ut->iSheet = 0;
				m_atcCollectables[x][y].iScoreOnTileTime = -1;
			}
			else if (bEraseScore && m_atcCollectables[x][y].iScoreOnTileTime != -1 && m_atcCollectables[x][y].ut->iTileshown != -1)
			{
				m_atcCollectables[x][y].ut->bShiftTileLocationRight = false;
				m_atcCollectables[x][y].ut->iTileshown = -1;
				m_atcCollectables[x][y].ut->iSheet = 0;
				m_atcCollectables[x][y].iScoreOnTileTime = -1;
			}
		}

}

bool Pacman::Collectables_UpdateAnimations()
{
	m_iAnimatedBitFrameCount++;
	if (m_iAnimatedBitFrameCount >= 10)
	{
		for (int x = 0; x < 27; x++)
			for (int y = 0; y < 31; y++)
			{
				if (m_atcCollectables[x][y].bAnimateWithTheTileBelow)
				{
					if (m_atcCollectables[x][y].iCollectable != m_atcCollectables[x][y].iAnimationOrigin)
						m_atcCollectables[x][y].iCollectable = m_atcCollectables[x][y].iAnimationOrigin;
					else
						m_atcCollectables[x][y].iCollectable += m_atcCollectables[x][y].iCollectableTilesheetWidth;
				}
			}
	}
	if (m_iAnimatedBitFrameCount >= 10)
		m_iAnimatedBitFrameCount = 0;
	return true;
}

bool Pacman::Collectables_ResetActiveCollectable(Sprites* sprMain)
{
	m_iCollectableAffectSeconds = -1;
	m_bGhostAI_SpecialBitClose = false;
	m_bGhostAI_SpecialBitActive = false;
	sprMain->RunOncePerSpecialBit();
	return true;
}

bool Pacman::Collectables_Abilities(int elapsedTime, int iAbility, Sprites* spr, bool bCollection)
{
	tileAbilities ta = m_taArray[iAbility];
	if (ta.bPerformImmediately || !bCollection)
	{
		m_iCurrentActiveBit = iAbility;

		if (ta.bTimed)
			m_iCollectableAffectSeconds = UtilityTimeInTheFuture(ta.iTimedSeconds);

		if (ta.bVunrabilityState)
		{

			m_bGhostAI_SpecialBitActive = false;
			m_bGhostAI_SpecialBitClose = false;
			spr->RunOncePerSpecialBit();
			Audio_StopMusic();
			Audio_SetMusic(10, true);

			//AIRunOncePerSpecialBit()
		}
	}
	else
	{
		m_iInventoryItem = iAbility;//Inventory Store

	}
	return true;
}

void Pacman::Collectables_InvUpdate()
{
	if(m_iInventoryItem > -1)
		Collectables_InvUseItem();
}

void Pacman::Collectables_InvUseItem()
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();//For Input

	if (keyboardState->IsKeyDown(Input::Keys::SPACE))
	{
		if (m_bInvReady)
		{
			
			m_bInvReady = false;
		}
	}
	else
		m_bInvReady = true;
}