#include "Pacman.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>   // std::default_random_engine
#include <ctime>
#include <cstdlib>
#include <array>

bool Pacman::UtilityLoadContent()
{
	m_iTimeSecondsTotal = 0;
	m_bOnASecondFrame = false;
	m_iElapsedSeconds = 0;
	m_iElapsedMinutes = 0;
	m_iElapsedHours = 0;
	m_iFpsUpdateSecond = 0;
	m_iCurrentFPS = 0;
	return true;
}

void Pacman::UtilityUpdateTime(int elapsedTime)
{
	m_iTimeSecondsTotal += elapsedTime;
	if (m_iTimeSecondsTotal >= 1000)
	{
		m_iTimeSecondsTotal -= 1000;
		m_bOnASecondFrame = true;
		m_iElapsedSeconds += 1;

		if (m_iElapsedSeconds > 59)
		{
			m_iElapsedSeconds -= 60;
			m_iElapsedMinutes += 1;
		}

		if (m_iElapsedMinutes > 59)
		{
			m_iElapsedMinutes -= 60;
			m_iElapsedHours += 1;
		}

		if (m_iElapsedHours > 999999999)
		{
			//If we don't do this and have the prediction method there would be a memory leak eventually...
			m_iElapsedSeconds = 0;
			m_iElapsedMinutes = 0;
			m_iElapsedHours = 0;
		}
	}
}

int Pacman::UtilityReturnSeconds(int seconds, int minutes, int hours, bool bMicro)
{
	if(bMicro)
		return ((seconds + (minutes * 60) + (hours * 60 * 60))*1000) + m_iTimeSecondsTotal;
	else
		return seconds + (minutes * 60) + (hours * 60 * 60);
}

int Pacman::UtilityReturnSeconds(bool bMicro)
{
	return UtilityReturnSeconds(m_iElapsedSeconds, m_iElapsedMinutes, m_iElapsedHours, bMicro);
}

int Pacman::UtilityTimeInTheFuture(int secondToLook, bool bMicro)
{
	int currentSeconds = UtilityReturnSeconds(m_iElapsedSeconds, m_iElapsedMinutes, m_iElapsedHours, bMicro);
	int newSeconds = currentSeconds + secondToLook;
	if (newSeconds > UtilityReturnSeconds(0, 0, 999999999, bMicro))
	{
		newSeconds -= UtilityReturnSeconds(0, 0, 999999999, bMicro);
	}
	return newSeconds;
}

int Pacman::UtilitySearchArraysInt(array <int, 100> mainArr, int searchItem)
{
	for (int i = 0; i < 100; i++)
	{
		if (mainArr[i] == NULL)
			continue;

		if (mainArr[i] == searchItem)
			return i;
	}
	return -1;
}

int Pacman::UtilitySearchArraysChar(array <char, 100> mainArr, char searchItem)
{
	for (int i = 0; i < 100; i++)
	{
		if (mainArr[i] == NULL)
			continue;

		if (mainArr[i] == searchItem)
			return i;
	}
	return -1;
}

int Pacman::UtilityRandomNumberBetween(int min, int max)
{
	return rand() % ((max - min) + 1) + min;
}

int Pacman::UtilityDirectionConvert(int convert, bool toNumPad)
{
	if (toNumPad)
	{
		switch (convert)
		{
		case 0:
			return 8;
		case 1:
			return 4;
		case 2:
			return 2;
		case 3:
			return 6;
		default:
			return -1;
		}
	}
	else
	{
		switch (convert)
		{
		case 2:
			return 2;
		case 4:
			return 1;
		case 6:
			return 3;
		case 8:
			return 0;
		default:
			return -1;
		}
	}
}

bool Pacman::UtilityCollisionCheck(Rect* rFirst, Rect* rSecond)
{
	if (rFirst->Y + rFirst->Height < rSecond->Y)
		return false;
	if (rFirst->Y > rSecond->Y + rSecond->Height)
		return false;
	if (rFirst->X + rFirst->Width < rSecond->X)
		return false;
	if (rFirst->X > rSecond->X + rSecond->Width)
		return false;

	return true;
}

bool Pacman::UtilityCollisionPosCheck(Vector2* v2FirstPosition, Vector2* v2SecondPosition, float fMarginOnFirst)
{
	if (v2FirstPosition->X - fMarginOnFirst <= v2SecondPosition->X && v2FirstPosition->X + fMarginOnFirst >= v2SecondPosition->X && v2FirstPosition->Y - fMarginOnFirst <= v2SecondPosition->Y && v2FirstPosition->Y + fMarginOnFirst >= v2SecondPosition->Y)
		return true;

	return false;
}

string Pacman::UtilityStringToUpper(string sContent)
{
	for (int i = 0; i < sContent.length(); i++)
		sContent[i] = toupper(sContent[i]);
	return sContent;
}

bool Pacman::UtilityEnterName(string &sOutput, array <array <uiTile, 35>, 28> &auiMenu, int iLine, int iStartChar, int iMaxLength, bool &bReadyForInput)
{
	char cKeypress = UtilityButtonPress(bReadyForInput);
	if (cKeypress == '¬' && sOutput.length() > 0)
		return true;

	if (cKeypress != '`' && cKeypress != '@' && sOutput.length() < iMaxLength)//Not blank and no backspace pressed
		sOutput += cKeypress;
	else if (cKeypress == '`' && sOutput.length() > 0)//Backspace pressed
		sOutput = sOutput.substr(0, sOutput.size() - 1);
		

	Menu_WriteLetters(&auiMenu, iStartChar, iLine, sOutput, red, 0, true, iMaxLength);
	return false;
}

char Pacman::UtilityButtonPress(bool &bReadyForInput)
{
	/*
		RETURNS:
		A-Z = The Letter to add to a string
		@ = No Character pressed successfully
		¬ = Return key was pressed
		` = Backspace Pressed
	
	*/
	char cReturn = '@';
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();//For Input

	/*
		NOTE TO SELF AND MARKER:
		It seems like ready for input bool can be put on the same line as the key check.
		However the key checks are doing two jobs.
		One - They check to see if a key is pressed
			SUB JOB - If there was no active key pressed before then lock in this key
		Two - Allow the else to set the "no active key pressed"

		For this reason you can't merge this line without a seporate function for checking no relevant key is pressed.
	*/

	if (keyboardState->IsKeyDown(Input::Keys::Q))
	{	
		if (bReadyForInput)
		{
			cReturn = 'Q';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::W))
	{
		if (bReadyForInput)
		{
			cReturn = 'W';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::E))
	{
		if (bReadyForInput)
		{
			cReturn = 'E';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::R))
	{
		if (bReadyForInput)
		{
			cReturn = 'R';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::T))
	{
		if (bReadyForInput)
		{
			cReturn = 'T';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::Y))
	{
		if (bReadyForInput)
		{
			cReturn = 'Y';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::U))
	{
		if (bReadyForInput)
		{
			cReturn = 'U';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::I))
	{
		if (bReadyForInput)
		{
			cReturn = 'I';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::O))
	{
		if (bReadyForInput)
		{
			cReturn = 'O';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::P))
	{
		if (bReadyForInput)
		{
			cReturn = 'P';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::A))
	{
		if (bReadyForInput)
		{
			cReturn = 'A';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::S))
	{
		if (bReadyForInput)
		{
			cReturn = 'S';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::D))
	{
		if (bReadyForInput)
		{
			cReturn = 'D';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::F))
	{
		if (bReadyForInput)
		{
			cReturn = 'F';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::G))
	{
		if (bReadyForInput)
		{
			cReturn = 'G';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::H))
	{
		if (bReadyForInput)
		{
			cReturn = 'H';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::J))
	{
		if (bReadyForInput)
		{
			cReturn = 'J';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::K))
	{
		if (bReadyForInput)
		{
			cReturn = 'K';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::L))
	{
		if (bReadyForInput)
		{
			cReturn = 'L';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::Z))
	{
		if (bReadyForInput)
		{
			cReturn = 'Z';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::X))
	{
		if (bReadyForInput)
		{
			cReturn = 'X';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::C))
	{
		if (bReadyForInput)
		{
			cReturn = 'C';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::V))
	{
		if (bReadyForInput)
		{
			cReturn = 'V';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::B))
	{
		if (bReadyForInput)
		{
			cReturn = 'B';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::N))
	{
		if (bReadyForInput)
		{
			cReturn = 'N';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::M))
	{
		if (bReadyForInput)
		{
			cReturn = 'M';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::RETURN))
	{
		if (bReadyForInput)
		{
			cReturn = '¬';
			bReadyForInput = false;
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::BACKSPACE))
	{
	if (bReadyForInput)
	{
		cReturn = '`';
		bReadyForInput = false;
	}
	}
	else
		bReadyForInput = true;

	return cReturn;
}

int Pacman::UtilityScoreToTileLocation(int iScore)
{
		switch (iScore)
		{
		case 200:
			return 1;
		case 300:
			return 2;
		case 400:
			return 3;
		case 500:
			return 4;
		case 600:
			return 5;
		case 700:
			return 6;
		case 800:
			return 7;
		case 900:
			return 8;
		case 1000:
			return 9;
		case 1600:
			return 10;
		case 2000:
			return 11;
		case 3000:
			return 12;
		case 4000:
			return 13;
		case 5000:
			return 15;
		default:
		case 100:
			return 0;
		}
		return 0;
}