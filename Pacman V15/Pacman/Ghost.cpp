#include "General.h"
#include "Pacman.h"
#include "PrisonDetails.h"
#include "Ghost.h"
#include "Player.h"
#include "FileManagement.h"

#include <stdlib.h>//I use ABS from this
#include <sstream>
#include <array>
#include <string>
#include <iostream>

using namespace S2D;

Ghost::Ghost(int iGhostType)
{
	m_gt = Ghosts_intToGhostType(iGhostType);//Set the ghost type
}

Ghost::~Ghost()
{
	delete m_vGhostPosition;
	m_vGhostPosition = nullptr;
}
void Ghost::LoadContent(array <array <tile, 31>, 28>* aat)
{
	m_vGhostPosition = new Vector2(0, 0);//Add the vector to the stack
	m_vGhostCenterPosition = new Vector2(0, 0);//Add the vector to the stack
	m_t2dGhostTexture = new Texture2D();//Texture
	m_t2dGhostTexture->Load("Textures/Ghosts_Bigger.png", false);//Default Ghost Texture
	m_rGhostRect = new Rect(0.0f, 0.0f, m_fSpriteSize, m_fSpriteSize);//Rect
	m_rGhostRect->X = Ghosts_ghostTypeToInt(m_gt) * (42 * 2);//Know this can go above but easier to read here

	m_vShadowPosition = new Vector2(0, 0);//Add the vector to the stack
	m_vShadowCenterPosition = new Vector2(0, 0);//Add the vector to the stack
	m_t2dShadowTexture = new Texture2D();//Texture
	m_t2dShadowTexture->Load("Textures/Ghosts_Bigger.png", false);//Default Ghost Texture
	m_rShadowRect = new Rect(0.0f, 0.0f, m_fSpriteSize, m_fSpriteSize);//Rect
	m_rShadowRect->X = Ghosts_ghostTypeToInt(m_gt) * (42 * 2);//Know this can go above but easier to read here
	//m_rShadowRect->X = 42;//Know this can go above but easier to read here

	SetCorner(aat);//Sets corner vars
}

bool Ghost::Ghosts_Update(classPacketPacman pacman)
{
	UpdateSpriteRect(pacman);
	return true;
}
bool Ghost::AIRunOncePerSpecialBit()
{
	m_bHaveBeenCapturedThisSpecial = false;
	m_rGhostRect->X = Ghosts_ghostTypeToInt(m_gt) * (42 * 2);//Know this can go above but easier to read here
	m_rShadowRect->X = Ghosts_ghostTypeToInt(m_gt) * (42 * 2);//Know this can go above but easier to read here
	m_iAnimationFrameCount = 0;
	m_fGhostModifiedSpeed = 0;
	return true;
}

void Ghost::UpdateSpriteRect(classPacketPacman pacman)
{
	m_iAnimationFrameCount++;

	if (!pacman.bGhostAI_SpecialBitActive || m_bHaveBeenCapturedThisSpecial)
	{
		//Now start to calulate the modifer
		int iYRectModifer = 0;

		switch (m_iDirection)
		{
		case 2:
			iYRectModifer = m_fSpriteSize * 2;
			break;
		case 4:
			iYRectModifer = m_fSpriteSize * 1;
			break;
		case 6:
			iYRectModifer = m_fSpriteSize * 3;
			break;
		}
		m_rGhostRect->Y = iYRectModifer;
		m_rShadowRect->Y = iYRectModifer;

		//if (m_gt == ghostTypeEnum::gtRed)
		//	cout << "NOT" << endl;

		if (m_bHasSpecialBit)
		{
			m_rGhostRect->X = Ghosts_ghostTypeToInt(m_gt) * (m_fSpriteSize * 2);//Know this can go above but easier to read here
			m_rShadowRect->X = Ghosts_ghostTypeToInt(m_gt) * (m_fSpriteSize * 2);//Know this can go above but easier to read here
			m_iAnimationFrameCount = m_iAnimationFrameChange;//Do the add first. Without this the position set by below would ruin the animation
			m_bHasSpecialBit = false; //Only do this once
		}

		if (m_iAnimationFrameCount == m_iAnimationFrameChange)
		{
			m_rGhostRect->X += m_fSpriteSize;
			m_rShadowRect->X += m_fSpriteSize;
		}
		else if (m_iAnimationFrameCount == (m_iAnimationFrameChange * 2))
		{
			m_rGhostRect->X -= m_fSpriteSize;
			m_rShadowRect->X -= m_fSpriteSize;
			m_iAnimationFrameCount = 0;
		}

		m_iAnimationFrameCountB = 0;//Reset this for the flashing
	}
	else if(pacman.bGhostAI_SpecialBitActive && !m_bHaveBeenCapturedThisSpecial && m_gbGhostMode != gbCaught)
	{
		m_bHasSpecialBit = true;
		//if (m_gt == ghostTypeEnum::gtRed)
		//	cout << "SPECIAL" << endl;

		if (m_iAnimationFrameCount == m_iAnimationFrameChange)
		{
			m_rGhostRect->X = m_fSpriteSize * 8;
			m_rShadowRect->X = m_fSpriteSize * 8;
		}
		else if (m_iAnimationFrameCount == (m_iAnimationFrameChange * 2))
		{
			m_rGhostRect->X = (m_fSpriteSize * 8) + m_fSpriteSize;
			m_rShadowRect->X = (m_fSpriteSize * 8) + m_fSpriteSize;
			m_iAnimationFrameCount = 0;
		}

		//Change this to be slower maybe another animaiton frame catch
		if (pacman.bGhostAI_SpecialBitClose)
		{
			m_iAnimationFrameCountB++;

			if (m_iAnimationFrameCountB == m_iAnimationFrameChangeB)
			{
				m_rGhostRect->Y = m_fSpriteSize;
				m_rShadowRect->Y = m_fSpriteSize;
			}
			else if (m_iAnimationFrameCountB == (m_iAnimationFrameChangeB * 2))
			{
				m_rGhostRect->Y = 0;
				m_rShadowRect->Y = 0;
				m_iAnimationFrameCountB = 0;
			}
		}
		else
		{
			m_rGhostRect->Y = 0;
			m_rShadowRect->Y = 0;
		}
	}
	else if (m_gbGhostMode == gbCaught)
	{
		float fSpriteX = 0, fSpriteY = 0;
		switch (m_iDirection)
		{
		case 2:
			fSpriteX = m_fSpriteSize * 8;
			fSpriteY = m_fSpriteSize * 3;
			break;
		case 4:
			fSpriteX = m_fSpriteSize * 9;
			fSpriteY = m_fSpriteSize * 2;
			break;
		case 6:
			fSpriteX = m_fSpriteSize * 9;
			fSpriteY = m_fSpriteSize * 3;
		case 8:
			fSpriteX = m_fSpriteSize * 8;
			fSpriteY = m_fSpriteSize * 2;
			break;
		}
		m_rGhostRect->X = fSpriteX;
		m_rShadowRect->X = fSpriteX;

		m_rGhostRect->Y = fSpriteY;
		m_rShadowRect->Y = fSpriteY;
	}
	else
	{
		cout << "GHOST::UpdateSpriteRect: Couldn't resolve an animation state" << endl;
	}
	//if (m_gt == ghostTypeEnum::gtRed)
	//	cout << "END" << endl;
}

ghostTypeEnum Ghost::Ghosts_intToGhostType(int iGhostType)
{
	switch (iGhostType)
	{
	case 1:
		return gtPink;
	case 2:
		return gtBlue;
	case 3:
		return gtOrange;
	default:
	case 0:
		return gtRed;
	}
}

int Ghost::Ghosts_ghostTypeToInt(ghostTypeEnum gtType)
{
	switch (gtType)
	{
	case gtPink:
		return 1;
	case gtBlue:
		return 2;
	case gtOrange:
		return 3;
	default:
	case gtRed:
		return 0;
	}
}


bool Ghost::Spawn(classPacketPacman pacman, bool bTesting)
{
	array <array <tile, 31>, 28>* aat = pacman.ptrAatTileMap;//Copy the pointer
	SetCorner(aat);//Sets corner vars
	//if(!m_pd.m_bHaveDefined)//Not gotten the prison Details
		m_pd.getPrisonInfo(aat);//Get them
	
	tilePosition tpSpawnTile;
	Vector2 vSpawnPosition;
	bool bTakeHalf = true;
	//Defaults:
	m_bHaveEscapedOnce = false;
	m_bInPenHeadUp = false;//Head down
	m_bAreInCorner = false;
	m_bHaveBeenCapturedThisSpecial = false;
	m_bHasSpecialBit = false;
	EmptyPath();//Empty Path
	m_bIsHeadingAlongPath = false;

	switch (m_gt)
	{
	case gtPink:
		tpSpawnTile.setPosition(m_pd.getPrisonDetail(prisonDetails::enterance).getCord(), m_pd.getPrisonDetail(prisonDetails::enterance).getCord(true) + 2);
		//if(bTesting)
		//	cout << "gtPink: " << tpSpawnTile.getCord() << ", " << tpSpawnTile.getCord(true) << "MAIN CORNER" << m_tCornerMain.getCord() << ", " << m_tCornerMain.getCord(true) << endl;

		m_gbGhostMode = gbEscaping; //Pink Escapes straight away.

		m_tpGhostTile.setPosition(m_pd.getPrisonDetail(prisonDetails::enterance).getCord(), m_pd.getPrisonDetail(prisonDetails::enterance).getCord(true) - 1);

		bTakeHalf = true;
		break;
	case gtBlue:
		tpSpawnTile.setPosition(m_pd.getPrisonDetail(prisonDetails::enterance).getCord() - 2, m_pd.getPrisonDetail(prisonDetails::enterance).getCord(true) + 2);
		m_gbGhostMode = gbCaptured;
		//if(bTesting)
		//	cout << "gtBlue: " << tpSpawnTile.getCord() << ", " << tpSpawnTile.getCord(true) << "MAIN CORNER" << m_tCornerMain.getCord() << ", " << m_tCornerMain.getCord(true) << endl;
		break;
	case gtOrange:
		tpSpawnTile.setPosition(m_pd.getPrisonDetail(prisonDetails::enterance).getCord() + 2, m_pd.getPrisonDetail(prisonDetails::enterance).getCord(true) + 2);
		m_gbGhostMode = gbCaptured;
		//if(bTesting)
		//	cout << "gtOrange: " << tpSpawnTile.getCord() << ", " << tpSpawnTile.getCord(true) << "MAIN CORNER" << m_tCornerMain.getCord() << ", " << m_tCornerMain.getCord(true) << endl;
		break;
	case gtRed:
	default:
		tpSpawnTile.setPosition(m_pd.getPrisonDetail(prisonDetails::enterance).getCord(), m_pd.getPrisonDetail(prisonDetails::enterance).getCord(true) - 1);
		//if(bTesting)
		//	cout << "gtRed: " << tpSpawnTile.getCord() << ", " << tpSpawnTile.getCord(true) << "MAIN CORNER" << m_tCornerMain.getCord() << ", " << m_tCornerMain.getCord(true) << endl;

		m_gbGhostMode = gbCornerOnce; //Red spawns outside so moves to corner once

		m_tpGhostTile.setPosition(m_pd.getPrisonDetail(prisonDetails::enterance).getCord(), m_pd.getPrisonDetail(prisonDetails::enterance).getCord(true) - 1);

		bTakeHalf = true;
		break;
	}
	//Go grab the literal cords:
	vSpawnPosition = GetSpawnPosition(tpSpawnTile, m_fSpriteSize, bTakeHalf);

	//Lock in the position:
	m_vGhostPosition->X = vSpawnPosition.X;
	m_vGhostPosition->Y = vSpawnPosition.Y;

	m_vGhostPosition->Y += 48;

	//Some once a game sets:
	m_bHaveEscapedOnce = false;//There is difforent escaping times if this is true

	//if(bTesting)
	//	cout << "ENTERANCE: " << m_pd.getPrisonDetail(prisonDetails::enterance).getCord() << ", " << m_pd.getPrisonDetail(prisonDetails::enterance).getCord(true) << endl;

	return true;
}

Vector2 Ghost::GetSpawnPosition(tilePosition tp, float fSpriteSize, bool bTakeHalf)
{
	Vector2 vReturn;

	//Top Left of Square
	vReturn.X = tp.getCord() * 24;//Would prefer to grab this from tilemap
	vReturn.Y = tp.getCord(true) * 24;//Would prefer to grab this from tilemap

	//Center of Square
	vReturn.X += 24 / 2;//Would prefer to grab this from tilemap
	vReturn.Y += 24 / 2;//Would prefer to grab this from tilemap

	//Sprite Cord:
	vReturn.X -= fSpriteSize / 2;
	vReturn.Y -= fSpriteSize / 2;

	//Odd Positions
	if(bTakeHalf)
		vReturn.X -= 24 / 2;

	return vReturn;
}

bool Ghost::Ghosts_Move(classPacketPacman pacman, Player* player, Ghost* ghosts[4])
{
	SetCenterPosition();

	if (m_gbGhostMode != gbCaught || m_bHaveBeenCapturedThisSpecial)
	{
		if (pacman.bGhostAI_ScatterPhase && m_gbGhostMode == gbMain)
			m_gbGhostMode = gbScatter;
		else if (pacman.bGhostAI_SpecialBitActive && (m_gbGhostMode == gbMain || m_gbGhostMode == gbScatter) && !m_bHaveBeenCapturedThisSpecial)
		{
			m_gbGhostMode = gbSpecial;//Put them into Special mode
			m_bIsHeadingAlongPath = false;//Path should be empty now
			EmptyPath();//Empty it
		}
		else if (!pacman.bGhostAI_SpecialBitActive && m_gbGhostMode == gbSpecial)
			m_gbGhostMode = gbMain;
		else if (!pacman.bGhostAI_ScatterPhase && m_gbGhostMode == gbMain)
			m_gbGhostMode = gbMain;

		if (!pacman.bGhostAI_SpecialBitActive)
			m_bHaveBeenCapturedThisSpecial = false;//Reset this because the specialbit phase is over

		if (m_gt != gbScatter)
			m_bAreInCorner = false;//Reset this
	}

	switch (m_gbGhostMode)
	{
	case gbCaptured:
		AIgbCaptured(pacman, player, ghosts);
		break;
	case gbEscaping:
		AIgbEscaping(pacman, player, ghosts);
		break;
	case gbCornerOnce:

		//The first moving to the corner AI
		if (!m_bIsHeadingAlongPath)//No path calulated
			m_aaiPath = Pacman::BasicMovement_AStarCalc(pacman, m_tpGhostTile.ConvertToAI(), m_tCornerMain.ConvertToAI(), false, true);//Calulate one

		m_bIsHeadingAlongPath = true;//You are now heading along the path

		if (BasicMovingPath(pacman))//Move to the end and if you reach it:
		{
			m_gbGhostMode = gbMain;//Switch to the next mode
			m_bIsHeadingAlongPath = false;//Path should be empty now
		}
		break;
	case gbMain:
		AIgbMain(pacman, player, ghosts);
		break;
	case gbSpecial:
	case gbScatter:
		AIgbScatter(pacman, player, ghosts);
		break;
	case gbCaught:
		AIgbCaught(&pacman, player, ghosts);
		break;
	}
	

	/*
	//Debugging AI, uncomment this:
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();//For Input

	if (keyboardState->IsKeyDown(Input::Keys::A) && m_gt == gtPink)
	{
		cout << "GHOST MODE: ";
		switch (m_gbGhostMode)
		{
		case gbCaptured:
			cout << "gbCaptured";
			break;
		case gbEscaping:
			cout << "gbEscaping";
			break;
		case gbCornerOnce:
			cout << "gbCornerOnce";
			break;
		case gbMain:
			cout << "gbMain";
			break;
		case gbSpecial:
			cout << "gbSpecial";
			break;
		case gbScatter:
			cout << "gbScatter";
			break;
		case gbCaught:
			cout << "gbCaught";
			break;
		}

		cout << " TYPE: ";
		switch (m_gt)
		{
		case ghostTypeEnum::gtBlue:
			cout << "gtBlue";
			break;
		case ghostTypeEnum::gtOrange:
			cout << "gtOrange";
			break;
		case ghostTypeEnum::gtPink:
			cout << "gtPink";
			break;
		case ghostTypeEnum::gtRed:
			cout << "gtRed";
			break;
		}
		cout << endl;
	}*/

	return true;
}

bool Ghost::SetCorner(array <array <tile, 31>, 28> *aat)
{
	array <array <tile, 31>, 28> aatUseArray = *aat;
	for(int x = 0; x < 28; x++)
		for(int y = 0; y < 31; y++)
			if (aatUseArray[x][y].m_bIsCorner && aatUseArray[x][y].m_gtCornerType == m_gt)
			{
				if (aatUseArray[x][y].m_bIsCornerMain)
					m_tCornerMain.setPosition(x, y);
				else
					m_tCornerSub.setPosition(x, y);
			}
	*aat = aatUseArray;
	return true;
}

void Ghost::SetCenterPosition()
{
	m_vGhostCenterPosition->X = m_vGhostPosition->X + (m_fSpriteSize / 2);
	m_vGhostCenterPosition->Y = m_vGhostPosition->Y + (m_fSpriteSize / 2);

	m_vShadowCenterPosition->X = m_vShadowPosition->X + (m_fSpriteSize / 2);
	m_vShadowCenterPosition->Y = m_vShadowPosition->Y + (m_fSpriteSize / 2);
}

/*
	AI
*/
bool Ghost::AIgbCaptured(classPacketPacman pacman, Player* player, Ghost* ghosts[4])
{
	int iElapsedTime = pacman.iElapsedTime;
	int iRoundNumber = pacman.iRoundNumber;

	bool bGhostIsEscaping = false;
	for (int i = 0; i < 4; i++)
		if (ghosts[i]->GetBehaviour() == gbEscaping)
			bGhostIsEscaping = true;

	if ((iRoundNumber > 2 && !bGhostIsEscaping) || (m_bHaveEscapedOnce && !bGhostIsEscaping))//If the level is above 2 and there is room to escape or we've already escaped once
		m_gbGhostMode = gbEscaping;//Switch to Escape mode
	else if (iRoundNumber == 1 && m_gt == ghostTypeEnum::gtBlue && pacman.iCollablesCollectedThisRound >= 30 && !bGhostIsEscaping)
		m_gbGhostMode = gbEscaping;//Inky First Round escaping
	else if (iRoundNumber == 1 && m_gt == ghostTypeEnum::gtOrange && pacman.iCollablesCollectedThisRound >= 90 && !bGhostIsEscaping)
		m_gbGhostMode = gbEscaping;//Inky First Round escaping
	else if (iRoundNumber == 2 && m_gt == ghostTypeEnum::gtBlue && !bGhostIsEscaping)
		m_gbGhostMode = gbEscaping;//Inky First Round escaping
	else if (iRoundNumber == 2 && m_gt == ghostTypeEnum::gtOrange && pacman.iCollablesCollectedThisRound >= 50)
		m_gbGhostMode = gbEscaping;//Inky First Round escaping

	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();//For Input

	if (keyboardState->IsKeyDown(Input::Keys::RETURN))
	{
		cout << pacman.iCollablesCollectedThisRound << endl;
	}


	if(m_gbGhostMode == gbEscaping)
		return false;

	//We use RelativeOffset here because top and bottom are the walls but I want the three tile in between the walls
	float fTopTileY = m_pd.getPrisonDetail(prisonDetails::topLeft).RelativeOffset(0,1).getCenterCord(pacman.iTileMapTileSize, pacman.iTileMapOffset, true);
	float fBottomTileY = m_pd.getPrisonDetail(prisonDetails::bottomLeft).RelativeOffset(0, -1).getCenterCord(pacman.iTileMapTileSize, pacman.iTileMapOffset, true);

	SetCenterPosition();

	if (m_bInPenHeadUp)
	{
		m_iDirection = 8;
		m_vGhostPosition->Y -= m_fGhostInPenSpeed * iElapsedTime;
		SetCenterPosition();
		if (m_vGhostCenterPosition->Y <= fTopTileY)
		{
			m_vGhostPosition->Y += m_fGhostInPenSpeed * iElapsedTime;
			m_bInPenHeadUp = false;
		}	
	}
	else
	{
		m_iDirection = 2;
		m_vGhostPosition->Y += m_fGhostInPenSpeed * iElapsedTime;
		SetCenterPosition();
		if (m_vGhostCenterPosition->Y >= fBottomTileY)
		{
			m_vGhostPosition->Y -= m_fGhostInPenSpeed * iElapsedTime;
			m_bInPenHeadUp = true;
		}
	}
	//if(m_gt == gtBlue)
	//	cout << "m_vGhostPosition->Y: " << m_vGhostPosition->Y << "iElapsedTime: " << iElapsedTime << " iBottomTileY: " << iBottomTileY << endl;

	return true;
}

bool Ghost::AIgbEscaping(classPacketPacman pacman, Player* player, Ghost* ghosts[4])
{
	int iRoundNumber = pacman.iRoundNumber;

	//float fTopTileY = m_pd.getPrisonDetail(prisonDetails::topLeft).RelativeOffset(0,1).getCenterCord(pacman.iTileMapTileSize, pacman.iTileMapOffset, true);
	float fEnteranceX = m_pd.getPrisonDetail(prisonDetails::enterance).getCenterCord(pacman.iTileMapTileSize, pacman.iTileMapOffset, false);
	fEnteranceX -= pacman.iTileMapTileSize / 2;//The enterance is two wide, this is then the center of the two
	float fEnteranceY = m_pd.getPrisonDetail(prisonDetails::enterance).RelativeOffset(0,-1).getCenterCord(pacman.iTileMapTileSize, pacman.iTileMapOffset, true);
	//Relative above because I want the tile just above the enterance

	if (AIMoveDirectToPos(&pacman,player,ghosts, fEnteranceX, fEnteranceY))//This moves to an exact X and Y and doesn't check Passable. Used for the pen.
	{
		if (!m_bHaveBeenCapturedThisSpecial)//Spawned in Pen
		{
			m_gbGhostMode = gbCornerOnce;//Start moving to your corner
		}
		else
		{
			m_gbGhostMode = gbMain;//Start moving to your corner
		}
		
		m_tpGhostTile.setPosition(m_pd.getPrisonDetail(prisonDetails::enterance).getCord(), m_pd.getPrisonDetail(prisonDetails::enterance).RelativeOffset(0, -1).getCord(true));
	}
	

	return true;
}

bool Ghost::AIMoveDirectToPos(classPacketPacman* pacman, Player* player, Ghost* ghosts[4], float fDestinationX, float fDestinationY)
{
	int iElapsedTime = pacman->iElapsedTime;
	int iMargin = 2;

	//cout << "fDestinationY: " << fDestinationY << endl;

	SetCenterPosition();
	if (m_vGhostCenterPosition->X != fDestinationX)//Ghost is not on the same X
	{
		if (m_vGhostCenterPosition->X - iMargin <= fDestinationX && m_vGhostCenterPosition->X + iMargin >= fDestinationX)
		{
			m_vGhostCenterPosition->X = fDestinationX;//If within the margin, just snap the Ghost in place
			SetPositionFromCenter();//Sets Position based on the center we set above
			return false;//Stop this running for this loop
		}

		if (m_vGhostCenterPosition->X > fDestinationX) //If Ghost is to the right
		{
			m_vGhostPosition->X -= m_fGhostBaseSpeed * iElapsedTime;//Move it left
			m_iDirection = 4;//Change sprite to left
			SetCenterPosition();//Reset the center
			if (m_vGhostCenterPosition->X < fDestinationX)//If we've gone over snap
			{
				m_vGhostCenterPosition->X = fDestinationX;//If within the margin, just snap the Ghost in place
				SetPositionFromCenter();//Sets Position based on the center we set above
				return false;//Stop this running for this loop
			}
		}
		else//Ghost is to the left
		{
			m_vGhostPosition->X += m_fGhostBaseSpeed * iElapsedTime;//Move it right
			m_iDirection = 6;//Change sprite to right
			SetCenterPosition();//Reset the center
			if (m_vGhostCenterPosition->X > fDestinationX)//If we've gone over snap
			{
				m_vGhostCenterPosition->X = fDestinationX;//If within the margin, just snap the Ghost in place
				SetPositionFromCenter();//Sets Position based on the center we set above
				return false;//Stop this running for this loop
			}
		}
	}
	else if (m_vGhostCenterPosition->Y != fDestinationY)//Ghost is not on the same X
	{
		if (m_vGhostCenterPosition->Y - iMargin <= fDestinationY && m_vGhostCenterPosition->Y + iMargin >= fDestinationY)
		{
			m_vGhostCenterPosition->Y = fDestinationY;//If within the margin, just snap the Ghost in place
			SetPositionFromCenter();//Sets Position based on the center we set above
			return false;//Stop this running for this loop
		}
		if (m_vGhostCenterPosition->Y > fDestinationY) //If Ghost is below
		{
			//In theory this should be imposible but just in case:
			m_vGhostPosition->Y -= m_fGhostBaseSpeed * iElapsedTime;//Move it up
			m_iDirection = 8;//Change sprite to down
			SetCenterPosition();//Reset the center
			if (m_vGhostCenterPosition->Y < fDestinationY)//If we've gone over snap
			{
				m_vGhostCenterPosition->Y = fDestinationY;//If within the margin, just snap the Ghost in place
				SetPositionFromCenter();//Sets Position based on the center we set above
				return false;//Stop this running for this loop
			}
		}
		else//Ghost is above
		{
			m_vGhostPosition->Y += m_fGhostBaseSpeed * iElapsedTime;//Move it down
			m_iDirection = 2;//Change sprite to up
			SetCenterPosition();//Reset the center
			if (m_vGhostCenterPosition->Y > fDestinationY)//If we've gone over snap
			{
				m_vGhostCenterPosition->Y = fDestinationY;//If within the margin, just snap the Ghost in place
				SetPositionFromCenter();//Sets Position based on the center we set above
				return false;//Stop this running for this loop
			}
		}
	}
	else //Reached the target
	{
		return true;
	}

	return false;
}


bool Ghost::AIgbMain(classPacketPacman pacman, Player* player, Ghost* ghosts[4])
{
	array <array <tile, 31>, 28> aatLocal = *pacman.ptrAatTileMap;
	array <array <int, 2>, 10> aaiExpensive;
	for (int i = 0; i < 10; i++)
		aaiExpensive[i] = { -1,-1 };
	aaiExpensive[0] = m_tpGhostPreviousTile.ConvertToAI();

	tilePosition tpReuse; //Just a tp we can use to shorten text

	/*
	//The below means the ghost will recalulate a new path on every exit
	//This ensures their pathfinding is acurate and not based on a previous poistion of sprites
	*/
	if (aatLocal[m_tpGhostTile.getCord()][m_tpGhostTile.getCord(true)].bIsExit && m_tpLastExitCalulation.getCord() != m_tpGhostTile.getCord() && m_tpLastExitCalulation.getCord(true) != m_tpGhostTile.getCord(true))
	{
		m_tpLastExitCalulation.setPosition(m_tpGhostTile.getCord(), m_tpGhostTile.getCord(true));
		EmptyPath();
		m_bIsHeadingAlongPath = false;//Path should be empty now
	}

	//Path calulations:
	if (!m_bIsHeadingAlongPath)//No path calulated
	{
		switch (m_gt)
		{
		case ghostTypeEnum::gtRed:
			if (m_bMaybeHeadAhead)
			{
				tpReuse = getFourTilesInFrontOfPacman(pacman, player);
				m_aaiPath = Pacman::BasicMovement_AStarCalc(pacman, m_tpGhostTile.ConvertToAI(), tpReuse.ConvertToAI(), false, true, aaiExpensive);//Pink's Path
			}
			else
				m_aaiPath = Pacman::BasicMovement_AStarCalc(pacman, m_tpGhostTile.ConvertToAI(), player->getTile().ConvertToAI(), false, true, aaiExpensive);//Calulate one
			break;
		case ghostTypeEnum::gtPink:
			if (!m_bStoppedStill)
			{
				tpReuse = getFourTilesInFrontOfPacman(pacman, player);
				m_aaiPath = Pacman::BasicMovement_AStarCalc(pacman, m_tpGhostTile.ConvertToAI(), tpReuse.ConvertToAI(), false, true, aaiExpensive);//Calulate one
			}
			else
			{
				m_aaiPath = Pacman::BasicMovement_AStarCalc(pacman, m_tpGhostTile.ConvertToAI(), player->getTile().ConvertToAI(), false, true, aaiExpensive);//Red's Path
				m_bStoppedStill = false;
			}
			break;
		case ghostTypeEnum::gtBlue:
			tpReuse = getDoubleDistanceOfPacman(pacman, player, ghosts);
			m_aaiPath = Pacman::BasicMovement_AStarCalc(pacman, m_tpGhostTile.ConvertToAI(), tpReuse.ConvertToAI(), false, true, aaiExpensive);//Calulate one
			break;
		case ghostTypeEnum::gtOrange:
			if(player->getTile().ReturnDistanceBetweenTiles(m_tpGhostTile) > 10)//Pacman is Ten Tiles away
				m_aaiPath = Pacman::BasicMovement_AStarCalc(pacman, m_tpGhostTile.ConvertToAI(), player->getTile().ConvertToAI(), false, true, aaiExpensive);//Red's Path
			else
			{
				tpReuse = ReturnOppositeCorner(player->getTile());
				m_aaiPath = Pacman::BasicMovement_AStarCalc(pacman, m_tpGhostTile.ConvertToAI(), tpReuse.ConvertToAI(), false, true, aaiExpensive);//Move away
			}
			break;
		}
		m_bIsHeadingAlongPath = true;//You are now heading along the path
		m_bMaybeHeadAhead = false;//Used to stop jittering
	}

	Vector2 v2PreviousPosition;
	v2PreviousPosition.X = m_vGhostPosition->X;
	v2PreviousPosition.Y = m_vGhostPosition->Y;

	if (BasicMovingPath(pacman))//Move to the end and if you reach it:
	{
		m_bIsHeadingAlongPath = false;//Path should be empty now
		EmptyPath();

		if (player->getTile().getCord() == m_tpGhostTile.getCord() || player->getTile().getCord(true) == m_tpGhostTile.getCord(true))
			m_bMaybeHeadAhead = true;

		BasicMovingPath(pacman);//Make sure you do a movement this gameloop

		if (v2PreviousPosition.X == m_vGhostPosition->X && v2PreviousPosition.Y == m_vGhostPosition->Y)
			m_bStoppedStill = true;

		return true;
	}
		
	return false;
}

bool Ghost::AIgbScatter(classPacketPacman pacman, Player* player, Ghost* ghosts[4])
{
	array <array <tile, 31>, 28> aatLocal = *pacman.ptrAatTileMap;
	array <array <int, 2>, 10> aaiExpensive;
	for (int i = 0; i < 10; i++)
		aaiExpensive[i] = { -1,-1 };

	tilePosition tpReuse; //Just a tp we can use to shorten text
	tilePosition tpAvoid; //Just a tp we can use to shorten text

	if (!m_bInCorner)
		tpReuse = m_tCornerMain;
	else
	{
		tpReuse = m_tCornerSub;
		m_bAreInCorner = true;//Considering this only calls if we're in the corner
	}
	
	if (m_bAreInCorner)
		aaiExpensive[0] = m_tpGhostPreviousTile.ConvertToAI();
		

	if (!m_bIsHeadingAlongPath)//No path calulated
		m_aaiPath = Pacman::BasicMovement_AStarCalc(pacman, m_tpGhostTile.ConvertToAI(), tpReuse.ConvertToAI(), false, true, aaiExpensive);//Calulate one

	m_bIsHeadingAlongPath = true;//You are now heading along the path

	if (BasicMovingPath(pacman))//Move to the end and if you reach it:
	{
		m_gbGhostMode = gbMain;//Switch to the next mode
		m_bIsHeadingAlongPath = false;//Path should be empty now

		if (m_bInCorner)
			m_bInCorner = false;
		else
			m_bInCorner = true;
	}

	return true;
}


bool Ghost::AIgbCaught(classPacketPacman* pacman, Player* player, Ghost* ghosts[4])
{
	if (!m_bCaughtMovingInPen) //First phase, move near pen
	{
		//The first moving to the corner AI
		if (!m_bIsHeadingAlongPath)//No path calulated
		{
			EmptyPath();//Empty it (should be done before but this is just in case)
						//We use RelativeOffset here because top and bottom are the walls but I want the three tile in between the walls
			int iEnterancesOffset = -1;
			if (m_tpGhostTile.getCord(false) > 13)
				iEnterancesOffset = 0;

			tilePosition enteranceTile = m_pd.getPrisonDetail(prisonDetails::enterance).RelativeOffset(iEnterancesOffset, -1);
			m_aaiPath = Pacman::BasicMovement_AStarCalc(*pacman, m_tpGhostTile.ConvertToAI(), enteranceTile.ConvertToAI(), false, true);//Calulate one
		}

		m_bIsHeadingAlongPath = true;//You are now heading along the path
		m_fGhostModifiedSpeed = 0.3f;

		if (BasicMovingPath(*pacman))//Move to the end and if you reach it:
		{
			m_bCaughtMovingInPen = true;
			m_fGhostModifiedSpeed = 0;
			return true;
		}
	}
	else
	{

		float fEnteranceX = m_pd.getPrisonDetail(prisonDetails::enterance).getCenterCord(pacman->iTileMapTileSize, pacman->iTileMapOffset, false);
		fEnteranceX -= pacman->iTileMapTileSize / 2;//The enterance is two wide, this is then the center of the two
		float fEnteranceY = m_pd.getPrisonDetail(prisonDetails::enterance).RelativeOffset(0, 2).getCenterCord(pacman->iTileMapTileSize, pacman->iTileMapOffset, true);

		if (AIMoveDirectToPos(pacman, player, ghosts, fEnteranceX, fEnteranceY))//This moves to an exact X and Y and doesn't check Passable. Used for the pen.
		{
			m_gbGhostMode = gbEscaping;//Start moving to your corner
			m_bHaveBeenCapturedThisSpecial = true;
			m_bIsHeadingAlongPath = false;
			m_bCaughtMovingInPen = false;

			EmptyPath();//Empty it (should be done before but this is just in case)
			return true;
		}
	}
	

	return false;
}


void Ghost::EmptyPath()
{
	for (int i = 0; i < 400; i++)
		for (int j = 0; j < 2; j++)
			m_aaiPath[i][j] = -1;
}

tilePosition Ghost::ReturnOppositeCorner(tilePosition tp)
{
	bool bAreOnTheLeft = false;
	if (tp.getCord() <= 13)
		bAreOnTheLeft = true;

	bool bAreAtTheTop = false;
	if (tp.getCord(true) <= 14)
		bAreAtTheTop = true;

	tilePosition tpReturn;
	
	if (bAreOnTheLeft && bAreAtTheTop)
		tpReturn.setPosition(26, 29);
	else if (!bAreOnTheLeft && bAreAtTheTop)
		tpReturn.setPosition(1, 29);
	else if (bAreOnTheLeft && !bAreAtTheTop)
		tpReturn.setPosition(26, 1);
	else
		tpReturn.setPosition(1, 1);

	return tpReturn;
}

/*
	BASIC MOVEMENT METHODS
	These are split over three steps:
	1. Move towards a target tile which is adjacent
		Method in Ghost:
		bool Ghost::BasicMovingAdjacent(classPacketPacman pacman, tilePosition tpMoveTo);
	2. Move down a path to a target tile which uses the adjacent
		
	3. Calulate a path to a target tile which feeds into the above
		Method in Pacman:
		static array <array <int, 2>, 400> BasicMovement_AStarCalc(classPacketPacman pacman, array <int, 2> aiOrigin, array <int, 2> aiTarget, bool bExportRoute = false);
*/

bool Ghost::BasicMovingAdjacent(classPacketPacman pacman, tilePosition tpMoveTo, bool bSnapToTile)
{
	SetCenterPosition();

	array <array <tile, 31>, 28> aatLocal = *pacman.ptrAatTileMap;
	int iMargin = 2, iElapsedTime = pacman.iElapsedTime;

	bool bSwitchMovementDirection = false;
	tilePosition tpCurrentTile = m_tpGhostTile;

	float fGhostTrueSpeed = m_fGhostBaseSpeed + m_fGhostModifiedSpeed;
	
	if (aatLocal[m_tpGhostTile.getCord()][m_tpGhostTile.getCord(true)].m_bIsTunnel)
		fGhostTrueSpeed += m_fGhostModifyForTunnel;
	if (m_bHaveBeenCapturedThisSpecial == false && pacman.bGhostAI_SpecialBitActive)
		fGhostTrueSpeed += m_fGhostModifyForSpecialBit;

	if (aatLocal[tpCurrentTile.getCord()][tpCurrentTile.getCord(true)].m_bIsTunnel)//We are on a tunnel tile
	{
		if (aatLocal[tpCurrentTile.getCord()][tpCurrentTile.getCord(true)].m_tpForeignParent.getCord() == tpMoveTo.getCord() && aatLocal[tpCurrentTile.getCord()][tpCurrentTile.getCord(true)].m_tpForeignParent.getCord(true) == tpMoveTo.getCord(true))
		{
			//We are moving from local to parent
			bSwitchMovementDirection = true;//Go up instead of down, left instead of right.
			iMargin = 24; //If you're within a tile I'm certain you've crossed sides
			bSnapToTile = false;//But let's not snap you because I know it's the same direction.
								//We also need to set the above because the dection will always make this true because the tile is 'technically' in the other direction
								//However it's not so we need this special case.
		}
	}


	float fMoveToX = tpMoveTo.getCenterCord(pacman.iTileMapTileSize, pacman.iTileMapOffset, false);
	float fMoveToY = tpMoveTo.getCenterCord(pacman.iTileMapTileSize, pacman.iTileMapOffset, true);

	if (m_vGhostCenterPosition->X - iMargin <= fMoveToX && m_vGhostCenterPosition->X + iMargin >= fMoveToX && m_vGhostCenterPosition->Y - iMargin <= fMoveToY && m_vGhostCenterPosition->Y + iMargin >= fMoveToY)
	{
		if (bSnapToTile)
		{
			m_vGhostCenterPosition->X = fMoveToX;
			m_vGhostCenterPosition->Y = fMoveToY;
			SetPositionFromCenter();
		}
		ShadowGhost(pacman);//Updates shadow
		m_tpGhostTile = tpMoveTo;//This is the new location
		return true;
	}

	if (m_vGhostCenterPosition->X != fMoveToX)//Ghost is not on the same X
	{
		if ((!bSwitchMovementDirection && m_vGhostCenterPosition->X > fMoveToX) || (bSwitchMovementDirection && m_vGhostCenterPosition->X < fMoveToX)) //If Ghost is to the right
		{
			m_vGhostPosition->X -= (fGhostTrueSpeed)* iElapsedTime;//Move it left

			m_iDirection = 4;//Change sprite to left
			SetCenterPosition();//Reset the center
			ShadowGhost(pacman);//Updates shadow

			if (m_vGhostCenterPosition->X < fMoveToX && !bSwitchMovementDirection)//If we've gone over snap
			{
				if (bSnapToTile)
				{
					m_vGhostCenterPosition->X = fMoveToX;//If within the margin, just snap the Ghost in place
					SetPositionFromCenter();//Sets Position based on the center we set above
				}
				m_tpGhostTile = tpMoveTo;//This is the new location
				ShadowGhost(pacman);//Updates shadow
				return true;//false //Stop this running for this loop
			}
		}
		else if (!bSwitchMovementDirection || (bSwitchMovementDirection && m_vGhostCenterPosition->X > fMoveToX))//Ghost is to the left
		{
			m_vGhostPosition->X += (fGhostTrueSpeed)* iElapsedTime;//Move it right

			m_iDirection = 6;//Change sprite to right
			SetCenterPosition();//Reset the center
			ShadowGhost(pacman);//Updates shadow

			if (m_vGhostCenterPosition->X > fMoveToX && !bSwitchMovementDirection)//If we've gone over snap
			{
				if (bSnapToTile)
				{
					m_vGhostCenterPosition->X = fMoveToX;//If within the margin, just snap the Ghost in place
					SetPositionFromCenter();//Sets Position based on the center we set above
				}
				m_tpGhostTile = tpMoveTo;//This is the new location
				ShadowGhost(pacman);//Updates shadow
				return true;//false //Stop this running for this loop
			}
		}
	}
	else if (m_vGhostCenterPosition->Y != fMoveToY)//Ghost is not on the same X
	{
		/*if (m_vGhostCenterPosition->Y - iMargin <= fMoveToY && m_vGhostCenterPosition->Y + iMargin >= fMoveToY)
		{
		if (bSnapToTile)
		{
		m_vGhostCenterPosition->Y = fMoveToY;//If within the margin, just snap the Ghost in place
		SetPositionFromCenter();//Sets Position based on the center we set above
		}
		ShadowGhost(pacman);//Updates shadow
		return true;//Stop this running for this loop
		}*/

		if ((m_vGhostCenterPosition->Y > fMoveToY && !bSwitchMovementDirection) || (m_vGhostCenterPosition->Y < fMoveToY && bSwitchMovementDirection)) //If Ghost is below
		{
			m_vGhostPosition->Y -= (fGhostTrueSpeed)* iElapsedTime;//Move it up

			m_iDirection = 8;//Change sprite to down
			SetCenterPosition();//Reset the center
			ShadowGhost(pacman);//Updates shadow
			if (m_vGhostCenterPosition->Y < fMoveToY && !bSwitchMovementDirection)//If we've gone over snap
			{
				if (bSnapToTile)
				{
					m_vGhostCenterPosition->Y = fMoveToY;//If within the margin, just snap the Ghost in place
					SetPositionFromCenter();//Sets Position based on the center we set above
				}
				m_tpGhostTile = tpMoveTo;//This is the new location
				ShadowGhost(pacman);//Updates shadow
				return true;//Stop this running for this loop
			}
		}
		else if (!bSwitchMovementDirection || (m_vGhostCenterPosition->Y > fMoveToY && bSwitchMovementDirection))//Ghost is above
		{
			m_vGhostPosition->Y += (fGhostTrueSpeed)* iElapsedTime;//Move it down

			m_iDirection = 2;//Change sprite to up
			SetCenterPosition();//Reset the center
			ShadowGhost(pacman);//Updates shadow
			if (m_vGhostCenterPosition->Y > fMoveToY && !bSwitchMovementDirection)//If we've gone over snap
			{
				if (bSnapToTile)
				{
					m_vGhostCenterPosition->Y = fMoveToY;//If within the margin, just snap the Ghost in place
					SetPositionFromCenter();//Sets Position based on the center we set above
				}
				m_tpGhostTile = tpMoveTo;//This is the new location
				ShadowGhost(pacman);//Updates shadow
				return true;//Stop this running for this loop
			}
		}
	}
	else
	{
		m_tpGhostTile = tpMoveTo;//This is the new location
		ShadowGhost(pacman);//Updates shadow
		return true;
	}

	return false;
}

bool Ghost::BasicMovingPath(classPacketPacman pacman)
{
	if (m_aaiPath[0][0] == -1)
		return true;

	tilePosition tpTarget;
	tpTarget.setPosition(m_aaiPath[0][0], m_aaiPath[0][1]);
	tilePosition tpStepAfter;
	tpStepAfter.setPosition(m_aaiPath[1][0], m_aaiPath[1][1]);

	bool bSnapToTile = false; //Used for turning
	if (tpTarget.ReturnDirectionFromTile(m_tpGhostTile) != m_iDirection)
		bSnapToTile = true; //We are turning

	if (BasicMovingAdjacent(pacman, tpTarget,bSnapToTile))
	{
		m_tpGhostTile.setPosition(m_aaiPath[0][0], m_aaiPath[0][1]);
		//cout << m_tpGhostTile.getCord() << "," << m_tpGhostTile.getCord(true) << endl;

		int i = 0;
		for (i = 0; i < 399; i++)//Find the first -1. 399 because we don't need to worry about 400 as it is set just after the loop.
		{
			m_aaiPath[i] = m_aaiPath[i + 1];//Copy the element in front over the current

			if (m_aaiPath[i + 1][0] == -1)//If you're at the end no need to do the full loop
				break;
		}
		m_aaiPath[i + 1] = { -1,-1 };//Just ensure the last element is definately -1.

		if (m_gt == ghostTypeEnum::gtRed)
		{
			Input::KeyboardState* keyboardState = Input::Keyboard::GetState();//For Input

			if (keyboardState->IsKeyDown(Input::Keys::SPACE))
			{
				//cout << "m_tpGhostPreviousTile: " << m_tpGhostPreviousTile.getCord() << ", " << m_tpGhostPreviousTile.getCord(true) << endl;
				//cout << "m_tpGhostTile: " << m_tpGhostTile.getCord() << ", " << m_tpGhostTile.getCord(true) << endl;
				//FileManagement::AStarExport(m_aaiPath);
				//cout << "Created" << endl;
			}
		}
	}
	else
		m_tpGhostPreviousTile = m_tpGhostTile;//This is the tile behind
	return false;
}

/*
	MOVING THROUGH TUNNEL:
*/
bool Ghost::ShadowGhost(classPacketPacman pacman)
{
	/*
		This deals with the tunnels.
		Don't need the Y for the main levels but I'm really happy I got it working and
		how smooth it is.
	*/
	Vector2* vReturnPosition = new Vector2(-9001, -9001);

	if (m_vGhostPosition->X < 0 || (m_vGhostPosition->X + m_fSpriteSize) >(pacman.iTileMapTileSize * 28))
	{
		if (m_vGhostPosition->X < 0)
		{
			vReturnPosition->Y = m_vGhostPosition->Y;
			vReturnPosition->X = ((pacman.iTileMapTileSize * 28)) - (0 - m_vGhostPosition->X);
			if (vReturnPosition->X <= (pacman.iTileMapTileSize * 28) - m_fSpriteSize)
			{
				m_vGhostPosition->X = vReturnPosition->X;
				m_vShadowPosition->X = 9001;
				m_vShadowPosition->Y = 9001;
				return true;
			}
		}
		else
		{
			vReturnPosition->Y = m_vGhostPosition->Y;
			vReturnPosition->X = ((m_vGhostPosition->X + m_fSpriteSize) - (pacman.iTileMapTileSize * 28)) - m_fSpriteSize;
			if (vReturnPosition->X >= 0)
			{
				m_vGhostPosition->X = vReturnPosition->X;
				m_vShadowPosition->X = 9001;
				m_vShadowPosition->Y = 9001;
				return true;
			}
		}
	}
	else if (m_vGhostPosition->Y < pacman.iTileMapOffset || (m_vGhostPosition->Y + m_fSpriteSize) >(31 * pacman.iTileMapTileSize) + pacman.iTileMapOffset)
	{
		vReturnPosition->X = m_vGhostPosition->X;
		if (m_vGhostPosition->Y < pacman.iTileMapOffset)
		{
			vReturnPosition->Y = ((31 * pacman.iTileMapTileSize) + pacman.iTileMapOffset) - (pacman.iTileMapOffset - m_vGhostPosition->Y);
			if (vReturnPosition->Y <= (31 * pacman.iTileMapTileSize) + pacman.iTileMapOffset - m_fSpriteSize)
			{
				m_vGhostPosition->Y = vReturnPosition->Y;
				m_vShadowPosition->X = 9001;
				m_vShadowPosition->Y = 9001;
				return true;
			}
		}
		else
		{
			vReturnPosition->Y = ((m_vGhostPosition->Y + m_fSpriteSize) - ((31 * pacman.iTileMapTileSize))) - m_fSpriteSize;
			if (vReturnPosition->Y >= (pacman.iTileMapOffset))
			{
				m_vGhostPosition->Y = vReturnPosition->Y;
				m_vShadowPosition->X = 9001;
				m_vShadowPosition->Y = 9001;
				return true;
			}
		}
	}
	m_vShadowPosition->X = vReturnPosition->X;
	m_vShadowPosition->Y = vReturnPosition->Y;
	return true;
}

/*
	INFORMATION FOR AI INPUT:
*/
//PINK LOCATION
tilePosition Ghost::getFourTilesInFrontOfPacman(classPacketPacman pacman, Player* player)
{
	tilePosition tpPacmanPlusFour = player->getTile();//Start with the player tile

	switch (pacman.iPacmanDirection)//Use the Player Direction
	{
	case 2://Down
		tpPacmanPlusFour.setPosition(tpPacmanPlusFour.getCord(), tpPacmanPlusFour.RelativeOffset(0, 4).getCord(true)); //Go four tiles in front
		break;
	case 4://Left
		tpPacmanPlusFour.setPosition(tpPacmanPlusFour.RelativeOffset(-4,0).getCord(), tpPacmanPlusFour.getCord(true)); //Go four tiles to the left
		break;
	case 6://Right
		tpPacmanPlusFour.setPosition(tpPacmanPlusFour.RelativeOffset(4, 0).getCord(), tpPacmanPlusFour.getCord(true)); //Go four tiles to the right
		break;
	case 8://Up
	default://Just in case
		tpPacmanPlusFour.setPosition(tpPacmanPlusFour.getCord(), tpPacmanPlusFour.RelativeOffset(-4, -4).getCord(true));//Go four tiles up (pacman original glitch version)
		//tpPacmanPlusFour.setPosition(tpPacmanPlusFour.getCord(), tpPacmanPlusFour.RelativeOffset(0, -4).getCord(true));//Go four tiles up (none original pacman glitch version)
		break;
	}

	array <int, 2> aiClosestPassable = Pacman::Tileset_FindCloestPassableTile(*pacman.ptrAatTileMap, tpPacmanPlusFour.ConvertToAI());//Find the closest tile which you can go down

	tpPacmanPlusFour.setPosition(aiClosestPassable[0], aiClosestPassable[1]);//Reuse this local tilePosition

	return tpPacmanPlusFour;//Return it
}
//BLUE LOCATION
tilePosition Ghost::getDoubleDistanceOfPacman(classPacketPacman pacman, Player* player, Ghost* ghosts[4])
{
	tilePosition tpPlayerLocation = player->getTile();//Start with the player tile
	tilePosition tpRedLocation; //Store red ghost position

	for (int i = 0; i < 4; i++)
		if (ghosts[i]->getType() == ghostTypeEnum::gtRed)
		{
			tpRedLocation.setPosition(ghosts[i]->getTile().getCord(), ghosts[i]->getTile().getCord(true)); //Get redghost position
			break;//Stops this running more times than needed
		}
	
	int iX = tpPlayerLocation.getCord() - tpRedLocation.getCord();
	iX *= 2;
	iX += tpPlayerLocation.getCord();
	int iY = tpPlayerLocation.getCord(true) - tpRedLocation.getCord(true);
	iY *= 2;
	iY += tpPlayerLocation.getCord(true);
	tilePosition tpTarget;
	tpTarget.setPosition(iX, iY);

	array <int, 2> aiClosestPassable = Pacman::Tileset_FindCloestPassableTile(*pacman.ptrAatTileMap, tpTarget.ConvertToAI());//Find the closest tile which you can go down

	tpTarget.setPosition(aiClosestPassable[0], aiClosestPassable[1]);//Reuse this local tilePosition

	return tpTarget;//Return it
}