#include "Pacman.h"
#include "FileManagement.h"

#include <sstream>
#include <array>
#include <string>
#include <iostream>

bool Pacman::MenuLoadContent()
{
	//Setup of the grid:
	m_vUITilePosition = new Vector2(-9001, -9001);
	m_rUITleRect = new Rect(0.0f, 0.0f, m_iUITileSize, m_iUITileSize);
	m_tUITileTexture = new Texture2D();
	m_tUITileTexture->Load("Textures/Menu/Menu_Elements_Characters.png", false);
	m_sLastUITileTexture = "Textures/Menu/Menu_Elements_Characters.png";

	m_vGameLogoPos = new Vector2(-9001, -9001);
	m_rGameLogoRec = new Rect(0.0f, 0.0f, 616, 277);
	m_t2dGameLogoTex = new Texture2D();
	m_t2dGameLogoTex->Load("Textures/Menu/Menu_GameLogos_Pacman.png", false);
	return true;
}
void Pacman::Menu_Update(int elapsedTime)
{
	if (m_iMenuLocation == 0) //Fake Loading Screen
	{
		Audio_SetSoundEffect(3, 15);//Menu Confirm Sound
		Audio_SetSoundEffect(4, 18);//Menu Delete Sound
		Audio_SetSoundEffect(5, 3);//Levelup Sound
		m_iMenuLocation = 2;
		m_bSwitchingUI = true;
	}
	else if(m_iMenuLocation == 1) //No Button Press Screen
	{

	}
	else if (m_iMenuLocation == 2) //Game Selection Screen
	{
		if (m_iSubMenuLocation == 0)
			Menu_PacmanMainMenu(&m_auiMenuTiles, &m_auiMenuBackgroundTiles, m_bSwitchingUI);
		else if (m_iSubMenuLocation == 1)
			Menu_PacmanHighScore(&m_auiMenuTiles, &m_auiMenuBackgroundTiles, m_bSwitchingUI);
	}
	else if (m_iMenuLocation == 3) //In a Pacman Level
	{
		if(m_iScoreEntryStep <= 0)
			Menu_InGameMenus();//Used for Pause and unpause

		if (m_iSubMenuLocation == 0)
			Menu_PacmanMainUI(&m_auiMenuTiles, &m_auiMenuBackgroundTiles, m_bSwitchingUI);
		else if (m_iSubMenuLocation == 1)
			Menu_PacmanHighScore(&m_auiMenuTiles, &m_auiMenuBackgroundTiles, m_bSwitchingUI);
	}
	else if (m_iMenuLocation == 4) //In a Ms. Pacman Level
	{
		if (m_iScoreEntryStep <= 0)
			Menu_InGameMenus();//Used for Pause and unpause

		if (m_iSubMenuLocation == 0)
			Menu_PacmanMainUI(&m_auiMenuTiles, &m_auiMenuBackgroundTiles, m_bSwitchingUI);
		else if (m_iSubMenuLocation == 1)
			Menu_PacmanHighScore(&m_auiMenuTiles, &m_auiMenuBackgroundTiles, m_bSwitchingUI);
	}
	else if (m_iMenuLocation == 5) //In a Pacman Arcade Level
	{
		if (m_iScoreEntryStep <= 0)
			Menu_InGameMenus();//Used for Pause and unpause

		if (m_iSubMenuLocation == 0)
			Menu_PacmanMainUI(&m_auiMenuTiles, &m_auiMenuBackgroundTiles, m_bSwitchingUI);
		else if (m_iSubMenuLocation == 1)
			Menu_PacmanHighScore(&m_auiMenuTiles, &m_auiMenuBackgroundTiles, m_bSwitchingUI);
	}
}

void Pacman::Menu_PacmanMainMenu(array <array <uiTile, 35>, 28>* auiMenu, array <array <uiTile, 35>, 28>* auiBackground, bool bSwitchingUI)
{
	if (bSwitchingUI)//Only true if we've moved from one state to another. This is so we're not continually mvoing things which don't change on a single scene.
	{
		//Empty Menu Screens:
		array <array <uiTile, 35>, 28> aui;
		*auiMenu = aui;
		*auiBackground = aui;

		string sColorCharacter = "";
		for (int x = 0; x < 28; x++)
		{
			sColorCharacter += '@';
		}
		//Add Black Background to the top and bottom:
		for (int y = 0; y < 35; y++)
		{
			Menu_WriteLetters(auiBackground, 0, y, sColorCharacter, black);
		}
		//Writing at the top of the screen:
		Menu_WriteLetters(auiMenu, 3, 0, "1UP", white);
		Menu_WriteLetters(auiMenu, 9, 0, "HIGH SCORE", white);

		Menu_WriteLetters(auiMenu, 5, 17, "PUSH ENTER BUTTON", orange);
		Menu_WriteLetters(auiMenu, 7, 20, "1 PLAYER ONLY", skyblue);
		Menu_WriteLetters(auiMenu, 1, 23, "BONUS PACMAN FOR 10000 PTS", orange);
		Menu_WriteLetters(auiMenu, 7, 26, "~ FOSTER 1980", pink);

		Menu_WriteLetters(auiMenu, 1, 29, "PUSH LEFT", darkblue);

		Menu_WriteLetters(auiMenu, 27, 29, "PUSH RIGHT", darkblue, 2);

		Menu_WriteLetters(auiMenu, 2, 33, "CREDIT 1", white);

		//GameLogo Pos:
		m_vGameLogoPos->X = 25;
		m_vGameLogoPos->Y = 75;

		m_bSwitchingUI = false;//Ensure we do not build core elements again
	}

	Menu_UpdateGameSelection();//Uses Keyboard to edit m_iGameSelection and m_iMenuLocation for starting a game

	if (m_iGameSelection == 0) //Pacman. Left: Arcade Mode, Right Ms. Pac-man
	{
		if(m_sLastGameLogoTexture != "Textures/Menu/Menu_GameLogos_Pacman.png")
			m_t2dGameLogoTex->Load("Textures/Menu/Menu_GameLogos_Pacman.png", false);
		m_sLastGameLogoTexture = "Textures/Menu/Menu_GameLogos_Pacman.png";

		Menu_WriteLetters(auiMenu, 1, 30, "ARCADE MODE", white, 0, true, 11);
		Menu_WriteLetters(auiMenu, 27, 30, "MS. PAC-MAN", white, 1, true, 11);
	}
	else if (m_iGameSelection == 1)//Ms. Pacman, Left: Pacman, Right: Arcade Mode
	{
		if (m_sLastGameLogoTexture != "Textures/Menu/Menu_GameLogos_MsPacman.png")
			m_t2dGameLogoTex->Load("Textures/Menu/Menu_GameLogos_MsPacman.png", false);
		m_sLastGameLogoTexture = "Textures/Menu/Menu_GameLogos_MsPacman.png";

		Menu_WriteLetters(auiMenu, 1, 30, "PAC-MAN", white, 0, true, 11);
		Menu_WriteLetters(auiMenu, 27, 30, "ARCADE MODE", white, 1, true, 11);
	}
	else if (m_iGameSelection == 2)//Arcade Mode, Left: Ms. Pacman, Right: Pac-man
	{
		if (m_sLastGameLogoTexture != "Textures/Menu/Menu_GameLogos_PacmanArcade.png")
			m_t2dGameLogoTex->Load("Textures/Menu/Menu_GameLogos_PacmanArcade.png", false);
		m_sLastGameLogoTexture = "Textures/Menu/Menu_GameLogos_PacmanArcade.png";

		Menu_WriteLetters(auiMenu, 1, 30, "MS. PAC-MAN", white, 0, true, 11);
		Menu_WriteLetters(auiMenu, 27, 30, "PAC-MAN", white, 1, true, 11);
	}
}

void Pacman::Menu_PacmanHighScore(array <array <uiTile, 35>, 28>* auiMenu, array <array <uiTile, 35>, 28>* auiBackground, bool bSwitchingUI, int iScorePlace)
{
	if (bSwitchingUI)//Only true if we've moved from one state to another. This is so we're not continually mvoing things which don't change on a single scene.
	{
		//Empty Menu Screens:
		array <array <uiTile, 35>, 28> aui, auiValue;
		string sColorCharacter = "";
		float fYmodifer = black * (26 * 2);
		sColorCharacter += '@';
		//Add Black Background to the top and bottom:
		for (int x = 0; x < 28; x++)
			for (int y = 0; y < 35; y++)
				aui[x][y].iTileshown = Menu_LettersToNumbers('@') + fYmodifer;

		*auiMenu = aui;
		*auiBackground = aui;

		Menu_WriteLetters(auiMenu, 9, 10, "HIGH SCORES", white);

		FileManagement::Scores scores = FileManagement::LoadScores(m_iGameSelection + 1);

		int iCurrentLine = 12, iScoreModifier = 0, iScoreEditLine = -1;
		colour colour = orange;
		for (int i = 0; i < 10; i++)
		{
			if (i == 1)
				colour = skyblue;
			else if (i == 2)
				colour = darkblue;
			else if(i == 3)
				colour = white;

			Menu_WriteLetters(auiMenu, 3, iCurrentLine, to_string(i + 1), white);

			if (scores.m_ascoList[i].m_iScore == 0 && iScorePlace == -1)
				continue;

			if (iScorePlace == i)//We're writting a highscore
			{
				iCurrentLine += 2;
				iScoreModifier -= 1;
				iScoreEditLine = iCurrentLine;
				continue;
			}

			Menu_WriteLetters(auiMenu, 7, iCurrentLine, to_string(scores.m_ascoList[i + iScoreModifier].m_iScore), colour);

			if (to_string(scores.m_ascoList[i + iScoreModifier].m_iScore).length() > 7)
				cout << "SCORE TOO MANY NUMBERS: " << i << " Score: " << to_string(scores.m_ascoList[i + iScoreModifier].m_iScore) << " Name: " << UtilityStringToUpper(scores.m_ascoList[i + iScoreModifier].m_sName) << endl;

			Menu_WriteLetters(auiMenu, 18, iCurrentLine, UtilityStringToUpper(scores.m_ascoList[i + iScoreModifier].m_sName), colour);

			if (to_string(scores.m_ascoList[i + iScoreModifier].m_iScore).length() > 7)
				cout << "SCORE NAME TOO LONG: " << i << " Score: " << to_string(scores.m_ascoList[i + iScoreModifier].m_iScore) << " Name: " << UtilityStringToUpper(scores.m_ascoList[i + iScoreModifier].m_sName) << endl;

			iCurrentLine += 2;
		}

		if (iScorePlace > -1)
			Menu_WriteLetters(auiMenu, 3, iCurrentLine, " PLEASE ENTER A NAME", white);
		else
			Menu_WriteLetters(auiMenu, 3, iCurrentLine, "PRESS ESCAPE TO RETURN", white);

		if (iScorePlace > -1)
			iScorePlace = iScoreEditLine;//This is a way to return the edit line

		//GameLogo Pos:
		m_vGameLogoPos->X = 25;
		m_vGameLogoPos->Y = 5;
		
		m_bSwitchingUI = false;//Ensure we do not build core elements again
	}

	Menu_UpdateGameSelection();//Uses Keyboard to edit m_iGameSelection and m_iMenuLocation for starting a game

	if (m_iGameSelection == 0) //Pacman. Left: Arcade Mode, Right Ms. Pac-man
	{
		if (m_sLastGameLogoTexture != "Textures/Menu/Menu_GameLogos_Pacman.png")
			m_t2dGameLogoTex->Load("Textures/Menu/Menu_GameLogos_Pacman.png", false);
		m_sLastGameLogoTexture = "Textures/Menu/Menu_GameLogos_Pacman.png";
	}
	else if (m_iGameSelection == 1)//Ms. Pacman, Left: Pacman, Right: Arcade Mode
	{
		if (m_sLastGameLogoTexture != "Textures/Menu/Menu_GameLogos_MsPacman.png")
			m_t2dGameLogoTex->Load("Textures/Menu/Menu_GameLogos_MsPacman.png", false);
		m_sLastGameLogoTexture = "Textures/Menu/Menu_GameLogos_MsPacman.png";

	}
	else if (m_iGameSelection == 2)//Arcade Mode, Left: Ms. Pacman, Right: Pac-man
	{
		if (m_sLastGameLogoTexture != "Textures/Menu/Menu_GameLogos_PacmanArcade.png")
			m_t2dGameLogoTex->Load("Textures/Menu/Menu_GameLogos_PacmanArcade.png", false);
		m_sLastGameLogoTexture = "Textures/Menu/Menu_GameLogos_PacmanArcade.png";
	}
}

void Pacman::Menu_PacmanMainUI(array <array <uiTile, 35>, 28>* auiMenu, array <array <uiTile, 35>, 28>* auiBackground, bool bSwitchingUI)//35,28
{
	if (bSwitchingUI)//Only true if we've moved from one state to another. This is so we're not continually mvoing things which don't change on a single scene.
	{
		//Empty Menu Screens:
		array <array <uiTile, 35>, 28> aui;
		for (int x = 0; x < 28; x++)
			for (int y = 0; y < 35; y++)
				aui[x][y].iTileshown = -1;
		
		*auiMenu = aui;
		*auiBackground = aui;

		//GameLogo Pos:
		m_vGameLogoPos->X = -9001;
		m_vGameLogoPos->Y = -9001;

		//Add Black Background to the top and bottom:
		for (int i = 0; i < 28; i++)
		{
			Menu_WriteLetters(auiBackground, i, 0, "@", black);
			Menu_WriteLetters(auiBackground, i, 1, "@", black);
			Menu_WriteLetters(auiBackground, i, 33, "@", black);
			Menu_WriteLetters(auiBackground, i, 34, "@", black);
		}
		//Writing at the top of the screen:
		Menu_WriteLetters(auiMenu, 3, 0, "1UP", white);
		Menu_WriteLetters(auiMenu, 9, 0, "HIGH SCORE", white);
		m_iCurrentHighScore = FileManagement::ReturnHighScore(m_iLevelNumber);

		m_bSwitchingUI = false;//Ensure we do not build core elements again
	}
	//Everything here therefore is subject to change within the scene:
	if (to_string(m_iCurrentGameScore).length() <= 7) //High score isn't too big
		Menu_WriteLetters(auiMenu, 7, 1, to_string(m_iCurrentGameScore), white, 1, true, 7);//Draw the highscore

	if (m_iNumberOfLives >= 0)
	{
		if (m_iNumberOfLives > 10)
			m_iNumberOfLives = 10;

		string sPacmanLives = "";
		for (int i = 0; i < m_iNumberOfLives; i++)
			sPacmanLives += 'A';

		sPacmanLives += m_cLivesCharacterAnimation;

		Menu_WriteLetters(auiMenu, 2, 33, sPacmanLives, icon, 0, true, 10);//Draw the lives
	}
	if(m_bPlayerOneNeedAnimating)
		Menu_WriteLetters(auiMenu, 9, 13, "PLAYER ONE!", skyblue, 0, true, 11);
	else
		Menu_WriteLetters(auiMenu, 9, 13, "", white, 0, true, 11);

	if (m_sReadyStatus == 0 && m_sReadyStatus != m_iPreviousReadyState)
		Menu_WriteLetters(auiMenu, 9, 19, "", white, 0, true, 11);
	else if (m_sReadyStatus == 1)
		Menu_WriteLetters(auiMenu, 9, 19, "  READY!", orange, 0, true, 11);
	else if (m_sReadyStatus == 2)
		Menu_WriteLetters(auiMenu, 9, 19, "GAME OVER!", red, 0, true, 11);

	m_iPreviousReadyState = m_sReadyStatus;//This is so we can change the menu beyond this method

	if (m_iCurrentHighScore < m_iCurrentGameScore)
		Menu_WriteLetters(auiMenu, 17, 1, to_string(m_iCurrentGameScore), skyblue, 1, true, 7);//Draw the highscore
	else
		Menu_WriteLetters(auiMenu, 17, 1, to_string(m_iCurrentHighScore), orange, 1, true, 7);//Draw the highscore

	//Menu_WriteLetters(auiMenu, 7, 1, "X" + to_string((int)m_vPacmanPosition->X) + "Y" + to_string((int)m_vPacmanPosition->Y), white,0,true,18);
}

void Pacman::Menu_WriteLetters(array <array <uiTile, 35>, 28>* aui, int iX, int iY, string words, colour colour, int iAlignment, bool bOverrideBlanks, int iSetLength)
{
	float fYmodifer = colour * (26 * 2);
	int iCurrentTile = 0, iCurrentShown = 0;
	array <array <uiTile, 35>, 28> auiReturn = *aui;
	if (iSetLength == 0)
		iSetLength = words.length();

	if (iY < 35)//I don't handle two lines here so this check is outside of everything
	{
		if (iAlignment == 0)//Left Aligned
		{
			if (bOverrideBlanks)//Cleanup
			{
				for (int i = 0; i < iSetLength; i++)//Clean all tiles in the length (meaning if you want 5 characters, they'll be reset)
				{
					iCurrentTile = i + iX;
					auiReturn[iCurrentTile][iY].iSheet = iCurrentShown;
					auiReturn[iCurrentTile][iY].iTileshown = -1;
				}
			}

			for (int i = 0; i < words.length(); i++)//Write from the left
			{
				iCurrentTile = i + iX;
				if (iCurrentTile >= 28)//X is checked here jic
					continue;

				iCurrentShown = Menu_LettersToNumbers(words[i]);//Grabs the tile X

				if (iCurrentShown == -1 && !bOverrideBlanks)//If it's -1 and we're not erasing blanks then don't reset it
					continue;

				auiReturn[iCurrentTile][iY].iSheet = iCurrentShown;//The sheet which is always 0 for the letters
				auiReturn[iCurrentTile][iY].iTileshown = iCurrentShown + fYmodifer;//The number of the tile. I'll split it to X and Y in the draw
			}
		}
		else //Right Align
		{
			if (bOverrideBlanks)//Cleanup
			{
				for (int i = (iSetLength - 1); i >= 0; i--)//Clean all tiles in the length (meaning if you want 5 characters, they'll be reset)
				{
					iCurrentTile = i + iX - iSetLength;
					if (iCurrentTile < 0)
						continue;
					auiReturn[iCurrentTile][iY].iSheet = iCurrentShown;
					auiReturn[iCurrentTile][iY].iTileshown = -1;
				}
			}

			for (int i = (words.length() - 1); i >= 0; i--)//Write from the left
			{
				iCurrentTile = i + iX - words.length();
				if (iCurrentTile < 0)//X is checked here jic
					continue;

				iCurrentShown = Menu_LettersToNumbers(words[i]);//Grabs the tile X

				if (iCurrentShown == -1 && !bOverrideBlanks)//If it's -1 and we're not erasing blanks then don't reset it
					continue;

				auiReturn[iCurrentTile][iY].iSheet = iCurrentShown;//The sheet which is always 0 for the letters
				auiReturn[iCurrentTile][iY].iTileshown = iCurrentShown + fYmodifer;//The number of the tile. I'll split it to X and Y in the draw
			}
		}
		*aui = auiReturn;//Set the pointer to the new version (This is because it's a multidimentional array and it's a little easier to add to the stack and do this)
	}
}

int Pacman::Menu_LettersToNumbers(char letter)
{
	array <char, 100> characters = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','0','1','2','3','4','5','6','7','8','9','-','~','.','/','"','!' };
	characters[(26*2)-2] = ' ';
	characters[(26*2)-1] = '@';
	return UtilitySearchArraysChar(characters, letter);
}

void Pacman::Menu_UpdateGameSelection()
{
	//cout << "SEL" << m_iNumber++ << endl;
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();//For Input

	if (keyboardState->IsKeyDown(Input::Keys::LEFT) && m_iSubMenuLocation == 0)
	{
		if (m_bReadyForNewSelection)
		{
			Audio_PlaySoundEffect(3, true);
			m_bReadyForNewSelection = false;
			m_iGameSelection -= 1;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::RIGHT) && m_iSubMenuLocation == 0)
	{
		if (m_bReadyForNewSelection)
		{
			Audio_PlaySoundEffect(3, true);
			m_bReadyForNewSelection = false;
			m_iGameSelection += 1;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::RETURN) && m_iSubMenuLocation == 0)
	{
		if (m_bReadyForNewSelection)
		{
			m_bReadyForNewSelection = false;
			m_iMenuLocation = 3 + m_iGameSelection;
			m_iLevelNumber = m_iMenuLocation - 2;//This is the level number
			m_bSwitchingUI = true; //We're moving to a new part of the program
			m_bLevelHasStartted = false;//Load the level from scratch
			m_bCanStartLevel = true;//Can start when ready
			m_bIsNewGame = true;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::ESCAPE))
	{
		if (m_bReadyForNewSelection)
		{
			m_bReadyForNewSelection = false;
			if (m_iSubMenuLocation == 0)
			{
				m_iSubMenuLocation = 1;
				Audio_PlaySoundEffect(4, true);//Deny Sound
			}
			else
			{
				m_iSubMenuLocation = 0;
				Audio_PlaySoundEffect(3, true);//Confirm
			}
				

			m_bSwitchingUI = true;
		}
	}
	else
		m_bReadyForNewSelection = true;

	if (m_iGameSelection < 0)
		m_iGameSelection = 2;
	else if (m_iGameSelection > 2)
	{
		m_iGameSelection = 0;
	}
}

void Pacman::Menu_InGameMenus()//Only runs when the game is in action
{
	//cout << "INGAME:" << m_iNumber++ << endl;
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();//For Input

	if (keyboardState->IsKeyDown(Input::Keys::ESCAPE))
	{
		if (m_bReadyForNewSelectionSub)
		{
			m_bReadyForNewSelectionSub = false;
			m_bReadyForNewSelection = false; //Stop Game Switching UI from Activating on the same frame
			if (m_iSubMenuLocation == 0)
			{
				m_iSubMenuLocation = 1;
				Audio_PlaySoundEffect(4, true);//Deny Sound
				if(m_bMusicIsPlaying)//If music is playing I need to pause it. If I didn't have this statment music in the buffer might pause/resume
					Audio::Pause(m_seMusicSound);
			}
			else
			{
				m_iSubMenuLocation = 0;
				Audio_PlaySoundEffect(3, true);//Confirm
				if(m_bMusicIsPlaying)//If music is playing I need to pause it. If I didn't have this statment music in the buffer might pause/resume
					Audio::Resume(m_seMusicSound);
			}
			m_bSwitchingUI = true;
		}
	}
	else
		m_bReadyForNewSelectionSub = true;

}