#include "Pacman.h"

#include <sstream>
#include <array>
#include <string>
#include <iostream>

bool Pacman::PlayerLoadContent()
{
	// Load Pacman
	m_t2dPacmanTexture = new Texture2D();
	m_t2dPacmanTexture->Load("Textures/Pacman_Sprite_Bigger.png", false);
	m_vPacmanPosition = new Vector2(-9001, -9001);
	m_rPacmanSourceRect = new Rect(0.0f, 0.0f, m_fSpriteWidthHeight, m_fSpriteWidthHeight);
	//Pacman2
	m_t2dPacman2Texture = new Texture2D();
	m_t2dPacman2Texture->Load("Textures/Pacman_Sprite_Bigger.png", false);
	m_vPacman2Position = new Vector2(-9001, -9001);
	m_rPacman2SourceRect = new Rect(0.0f, 0.0f, m_fSpriteWidthHeight, m_fSpriteWidthHeight);

	m_vPacmanCenter = new Vector2(0, 0);

	return true;
}

void Pacman::PlayerMovement_Update(int elapsedTime)
{
	/*
		PlayerMovement is 1% moving pacman, 0.1% turning pacman and the rest are checks and balances to ensure 100%
		that Pacman is drawn exactly on the screen in an acceptable position.
		It may seem I'm going a little too far with these but trust me, occasionally some of these checks push Pacman
		back even if most never do.

		If I didn't want the movement to act like Pacman this would be much smaller.
	*/

	PlayerMovement_UpdateCenterPosition(m_vPacmanPosition, m_vPacmanCenter);
	array <int,2> aiCurrentTile = PlayerMovement_UpdateCurrentPosition(m_vPacmanCenter);
	PlayerMovement_UpdateBufferKeypress();

	if(m_iBufferKeyPress > 0)
		m_bChangingDirection = PlayerMovement_UpdateTargetTile(aiCurrentTile, m_vPacmanCenter);//Will change m_vPacmanPosition

	if (m_bChangingDirection)
		m_bChangingDirection = PlayerMovement_MovePlayerToSwitchingTile(elapsedTime, aiCurrentTile, m_vPacmanCenter);//Will change m_vPacmanPosition
	else
	{
		m_vPacmanPosition = PlayerMovement_UpdateMove(elapsedTime, m_iMovementKeyPress, m_vPacmanCenter);
		m_vPacman2Position = PlayerMovement_UpdateMovePacman2(m_vPacmanPosition);
	}
	
	*m_rPacmanSourceRect = PlayerMovement_UpdateDirection(m_iMovementKeyPress);
	*m_rPacman2SourceRect = *m_rPacmanSourceRect;//PlayerMovement_UpdateDirection(m_iMovementKeyPress, false);//False so we don't progress the frame
}

bool Pacman::PlayerMovement_UpdateBufferKeypress()
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();//For Input

	if (keyboardState->IsKeyDown(Input::Keys::W) || keyboardState->IsKeyDown(Input::Keys::UP))
		m_iBufferKeyPress = 8;
	else if (keyboardState->IsKeyDown(Input::Keys::A) || keyboardState->IsKeyDown(Input::Keys::LEFT))
		m_iBufferKeyPress = 4;
	else if (keyboardState->IsKeyDown(Input::Keys::S) || keyboardState->IsKeyDown(Input::Keys::DOWN))
		m_iBufferKeyPress = 2;
	else if (keyboardState->IsKeyDown(Input::Keys::D) || keyboardState->IsKeyDown(Input::Keys::RIGHT))
		m_iBufferKeyPress = 6;

	if (m_iBufferKeyPress == m_iMovementKeyPress)
		m_iBufferKeyPress = 0;
	return true;
}

void Pacman::PlayerMovement_UpdateCenterPosition(Vector2* topLeftPos, Vector2* vReturn)
{
	vReturn->X = topLeftPos->X + (m_fSpriteWidthHeight / 2);
	vReturn->Y = topLeftPos->Y + (m_fSpriteWidthHeight / 2);
	//return new Vector2(topLeftPos->X + (m_fSpriteWidthHeight / 2), topLeftPos->Y + (m_fSpriteWidthHeight / 2));
}

array <int, 2> Pacman::PlayerMovement_UpdateCurrentPosition(Vector2* centerPos)
{
	int tileX = (centerPos->X / m_fTilemapWidthHeight);

	if (tileX < 0)
		tileX = 0;
	else if (tileX > 27)
		tileX = 27;

	int tileY = ((centerPos->Y - m_fTilemapMarginTop) / m_fTilemapWidthHeight);

	if (tileY < 0)
		tileY = 0;
	else if (tileY > 30)
		tileY = 30;

	return { tileX, tileY };
}

bool Pacman::PlayerMovement_UpdateTargetTile(array <int, 2> aiCurrentTile, Vector2* vCenterPos)
{
	if (((m_iMovementKeyPress == 4 || m_iMovementKeyPress == 6) && (m_iBufferKeyPress == 4 || m_iBufferKeyPress == 6)) || ((m_iMovementKeyPress == 2 || m_iMovementKeyPress == 8) && (m_iBufferKeyPress == 2 || m_iBufferKeyPress == 8)))
	{
		m_iMovementKeyPress = m_iBufferKeyPress;
		m_iBufferKeyPress = 0;
		return false;
	}
	if (m_iBufferKeyPress == 0)
		return false;

	array <float, 2> currentTilepos = Tileset_ReturnCordOfTile(aiCurrentTile[0], aiCurrentTile[1]);
	float fCurrentTileX = currentTilepos[0], fCurrentTileY = currentTilepos[1];

	array <tile, 9> atTilesAround;
	if (vCenterPos->X - 5 <= fCurrentTileX && vCenterPos->X + 5 >= fCurrentTileX && vCenterPos->Y - 5 <= fCurrentTileY && vCenterPos->Y + 5 >= fCurrentTileY)
	{
		atTilesAround = Tileset_GetInformationAroundTile(m_aatTilemap, aiCurrentTile);

		if (atTilesAround[m_iBufferKeyPress].bPassable)
		{
			m_iMovementKeyPress = m_iBufferKeyPress;
			m_iBufferKeyPress = 0;
			return true;
		}
	}
	return false;
}

bool Pacman::PlayerMovement_MovePlayerToSwitchingTile(int elapsedTime, array <int, 2> aiCurrentTile, Vector2* vCenterPos)
{
	array <float, 2> currentTilepos = Tileset_ReturnCordOfTile(aiCurrentTile[0], aiCurrentTile[1]);
	float fCurrentTileX = currentTilepos[0], fCurrentTileY = currentTilepos[1];

	if (vCenterPos->X == fCurrentTileX && vCenterPos->Y == fCurrentTileY)
	{
		return false;
	}

	float fMarginForTurning = 0.1f;
	if (vCenterPos->X - fMarginForTurning <= fCurrentTileX && vCenterPos->X + fMarginForTurning >= fCurrentTileX && vCenterPos->Y - fMarginForTurning <= fCurrentTileY && vCenterPos->Y + fMarginForTurning >= fCurrentTileY)
	{
		m_vPacmanPosition = new Vector2(fCurrentTileX - (m_fSpriteWidthHeight / 2), fCurrentTileY - (m_fSpriteWidthHeight / 2));
		return false;
	}
	else
	{
		//Input::KeyboardState* keyboardState = Input::Keyboard::GetState();//For Input

		//if (keyboardState->IsKeyDown(Input::Keys::SPACE))
		//	cout << aiCurrentTile[0] << ", " << aiCurrentTile[1] << endl;

		if (vCenterPos->X > fCurrentTileX)
		{
			m_vPacmanPosition->X -= (m_fPacmanBaseSpeed + m_fPacmanSpeedModify) * elapsedTime;
			vCenterPos->X -= (m_fPacmanBaseSpeed + m_fPacmanSpeedModify) * elapsedTime;

			if (vCenterPos->X < fCurrentTileX)
				m_vPacmanPosition->X = fCurrentTileX - (m_fSpriteWidthHeight / 2);
		}
		else
		{
			m_vPacmanPosition->X += (m_fPacmanBaseSpeed + m_fPacmanSpeedModify) * elapsedTime;
			vCenterPos->X += (m_fPacmanBaseSpeed + m_fPacmanSpeedModify) * elapsedTime;

			if (vCenterPos->X > fCurrentTileX)
				m_vPacmanPosition->X = fCurrentTileX - (m_fSpriteWidthHeight / 2);
		}

		if (vCenterPos->Y > fCurrentTileY)
		{
			m_vPacmanPosition->Y -= (m_fPacmanBaseSpeed + m_fPacmanSpeedModify) * elapsedTime;
			vCenterPos->Y -= (m_fPacmanBaseSpeed + m_fPacmanSpeedModify) * elapsedTime;

			if (vCenterPos->Y < fCurrentTileY)
				m_vPacmanPosition->Y = fCurrentTileY - (m_fSpriteWidthHeight / 2);
		}
		else
		{
			m_vPacmanPosition->Y += (m_fPacmanBaseSpeed + m_fPacmanSpeedModify) * elapsedTime;
			vCenterPos->Y += (m_fPacmanBaseSpeed + m_fPacmanSpeedModify) * elapsedTime;

			if (vCenterPos->Y > fCurrentTileY)
				m_vPacmanPosition->Y = fCurrentTileY - (m_fSpriteWidthHeight / 2);
		}

		return true;
	}
}

Vector2* Pacman::PlayerMovement_UpdateMove(int elapsedTime, int direction, Vector2* centerPos)
{
	Vector2* newCenterPos = new Vector2(centerPos->X, centerPos->Y);
	array <int, 2> oldPosition = PlayerMovement_UpdateCurrentPosition(centerPos);
	array <tile, 9> atTilesAround = Tileset_GetInformationAroundTile(m_aatTilemap, oldPosition);

	if (!PlayerMovement_MovementAreOnCorrectLine(direction, centerPos, oldPosition))
	{
		newCenterPos->X -= m_fSpriteWidthHeight / 2;
		newCenterPos->Y -= m_fSpriteWidthHeight / 2;
		return newCenterPos;
	}

	switch (direction)
	{
	case 2:
		newCenterPos->Y += (m_fPacmanBaseSpeed + m_fPacmanSpeedModify) * elapsedTime;
		break;
	case 4:
		newCenterPos->X -= (m_fPacmanBaseSpeed + m_fPacmanSpeedModify) * elapsedTime;
		break;
	case 6:
		newCenterPos->X += (m_fPacmanBaseSpeed + m_fPacmanSpeedModify) * elapsedTime;
		break;
	case 8:
		newCenterPos->Y -= (m_fPacmanBaseSpeed + m_fPacmanSpeedModify) * elapsedTime;
		break;
	}

	array <float, 2> targetPosition = Tileset_ReturnCordOfTile(oldPosition[0], oldPosition[1]);
		
	if (!atTilesAround[4].bPassable)
	{
		if (newCenterPos->X < targetPosition[0])
			newCenterPos->X = targetPosition[0];
	}
	else if (!atTilesAround[6].bPassable)
	{
		if (newCenterPos->X > targetPosition[0])
			newCenterPos->X = targetPosition[0];
	}

	if (!atTilesAround[8].bPassable)
	{
		if (newCenterPos->Y < targetPosition[1])
			newCenterPos->Y = targetPosition[1];//centerPos->Y;
	}
	else if (!atTilesAround[2].bPassable)
	{
		if (newCenterPos->Y > targetPosition[1])
			newCenterPos->Y = targetPosition[1];
	}
	
	if (newCenterPos->X == centerPos->X && newCenterPos->Y == centerPos->Y)
		m_bPlayerStill = true;
	else
		m_bPlayerStill = false;

	newCenterPos->X -= m_fSpriteWidthHeight / 2;
	newCenterPos->Y -= m_fSpriteWidthHeight / 2;
	return newCenterPos;
}

Vector2* Pacman::PlayerMovement_UpdateMovePacman2(Vector2* pacmanPos)
{
	/*
		This deals with the tunnels.
		Don't need the Y for the main levels but I'm really happy I got it working and 
		how smooth it is.
	*/
	Vector2* vReturnPosition = new Vector2(-9001, -9001);

	//array <int, 2> aiCurrentTile;
	//array <tile, 9> atTilesAround;

	if (pacmanPos->X < 0 || (pacmanPos->X + m_fSpriteWidthHeight) > (m_fTilemapWidthHeight * 28))
	{
		//aiCurrentTile = PlayerMovement_UpdateCurrentPosition(pacmanPos);
		//atTilesAround = Tileset_GetInformationAroundTile(m_aatTilemap, aiCurrentTile);

		
		if (pacmanPos->X < 0)
		{
			vReturnPosition->Y = pacmanPos->Y;
			vReturnPosition->X = ((m_fTilemapWidthHeight * 28)) - (0 - pacmanPos->X);
			if (vReturnPosition->X <= (m_fTilemapWidthHeight * 28) - m_fSpriteWidthHeight)
			{
				pacmanPos->X = vReturnPosition->X;
				return new Vector2(-9001, -9001);
			}
		}
		else 
		{
			vReturnPosition->Y = pacmanPos->Y;
			vReturnPosition->X = ((pacmanPos->X + m_fSpriteWidthHeight) - (m_fTilemapWidthHeight * 28)) - m_fSpriteWidthHeight;
			if (vReturnPosition->X >= 0)
			{
				pacmanPos->X = vReturnPosition->X;
				return new Vector2(-9001, -9001);
			}
		}
	}
	else if (pacmanPos->Y < m_fTilemapMarginTop || (pacmanPos->Y + m_fSpriteWidthHeight) > (31 * m_fTilemapWidthHeight) + m_fTilemapMarginTop)
	{
		vReturnPosition->X = pacmanPos->X;
		if (pacmanPos->Y < m_fTilemapMarginTop)
		{
			vReturnPosition->Y = ((31 * m_fTilemapWidthHeight) + m_fTilemapMarginTop) - (m_fTilemapMarginTop - pacmanPos->Y);
			if (vReturnPosition->Y <= (31 * m_fTilemapWidthHeight) + m_fTilemapMarginTop - m_fSpriteWidthHeight)
			{
				pacmanPos->Y = vReturnPosition->Y;
				return new Vector2(-9001, -9001);
			}
		}
		else
		{
			vReturnPosition->Y = ((pacmanPos->Y + m_fSpriteWidthHeight) - ((31 * m_fTilemapWidthHeight))) - m_fSpriteWidthHeight;
			if (vReturnPosition->Y >= (m_fTilemapMarginTop))
			{
				pacmanPos->Y = vReturnPosition->Y;
				return new Vector2(-9001, -9001);
			}
		}
	}
	return vReturnPosition;
}

Rect Pacman::PlayerMovement_UpdateDirection(int direction, bool bDealingWithPlayer)
{
	int iRectX = 0, iRectY = 0;

	switch (direction)
	{
	case 2:
		iRectY = m_fSpriteWidthHeight * 3;
		break;
	case 4:
		iRectY = m_fSpriteWidthHeight;
		break;
	case 8:
		iRectY = m_fSpriteWidthHeight * 2;
		break;
	default:
		iRectY = 0.0f;
		break;
	}
	iRectX = PlayerMovement_UpdateAnimation(bDealingWithPlayer);

	Rect r;
	r.X = iRectX;
	r.Y = iRectY;
	r.Width = m_fSpriteWidthHeight;
	r.Height = m_fSpriteWidthHeight;

	return r;
}

float Pacman::PlayerMovement_UpdateAnimation(bool bDealingWithPlayer)
{
	if((!m_bPlayerStill || (m_bPlayerStill && m_iAnimationCount != 1)) && bDealingWithPlayer)
		m_iFrameCount += 1;

	if (m_iFrameCount >= m_iFrameToAct)
	{
		m_iFrameCount = 0;
		if(m_bAnimationDirection)
			m_iAnimationCount += 1;
		else
			m_iAnimationCount -= 1;
	}

	if (m_iAnimationCount > 4)
	{
		m_iAnimationCount = 3;
		m_bAnimationDirection = false;
	}
	else if (m_iAnimationCount < 0)
	{
		m_iAnimationCount = 1;
		m_bAnimationDirection = true;
	}

	return m_iAnimationCount * m_fSpriteWidthHeight;
}

bool Pacman::PlayerMovement_MovementAreOnCorrectLine(int direction, Vector2* centerPos, array <int, 2> currentTile)
{
	float targetX = ((currentTile[0]) * m_fTilemapWidthHeight) + (m_fTilemapWidthHeight / 2);
	float targetY = ((currentTile[1]) * m_fTilemapWidthHeight) + (m_fTilemapWidthHeight / 2) + m_fTilemapMarginTop;

	switch (direction)
	{
	case 4:
	case 6:
		if (centerPos->Y == targetY)
			return true;
		break;
	case 2:
	case 8:
		if (centerPos->X == targetX)
			return true;
		break;
	default:
		return false;
	}
	return false;
}