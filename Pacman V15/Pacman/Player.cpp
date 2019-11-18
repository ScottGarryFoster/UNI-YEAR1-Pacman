#include "Pacman.h"
#include "Player.h"
#include "General.h"

#include <sstream>
#include <array>
#include <string>
#include <iostream>

using namespace S2D;

Player::Player()
{
	
}

Player::~Player()
{

}
void Player::LoadContent()
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

	m_pacman.iElapsedTime = -1;
}

bool Player::Update(int elapsedTime)
{

	return true;
}

bool Player::setPosition(Vector2* vPos, classPacketPacman pacman)
{
	if (m_vPacmanPosition == nullptr)
		LoadContent();

	m_vPacmanPosition->X = vPos->X;
	m_vPacmanPosition->Y = vPos->Y;
	PlayerMovement_UpdateCenterPosition();

	if(m_pacman.iElapsedTime == -1)
		m_pacman = pacman;

	return true;
}

tilePosition Player::getTile()
{
	int tileX = (m_vPacmanCenter->X / m_pacman.iTileMapTileSize);

	if (tileX < 0)
		tileX = 0;
	else if (tileX > 27)
		tileX = 27;

	int tileY = ((m_vPacmanCenter->Y - m_pacman.iTileMapOffset) / m_pacman.iTileMapTileSize);

	if (tileY < 0)
		tileY = 0;
	else if (tileY > 30)
		tileY = 30;

	return { tileX, tileY };
}

void Player::PlayerMovement_UpdateCenterPosition()
{
	m_vPacmanCenter->X = m_vPacmanPosition->X + (m_fSpriteWidthHeight / 2);
	m_vPacmanCenter->Y = m_vPacmanPosition->Y + (m_fSpriteWidthHeight / 2);
	//return new Vector2(topLeftPos->X + (m_fSpriteWidthHeight / 2), topLeftPos->Y + (m_fSpriteWidthHeight / 2));
}