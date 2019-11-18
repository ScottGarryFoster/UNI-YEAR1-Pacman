#include "FileManagement.h"

#include <sstream>
#include <array>
#include <string>
#include <iostream>
#include <cstdlib>
#include <iostream>
#include <fstream>


bool FileManagement::AStarExport(array <array <int, 2>, 400> aai) //This function is just for the testing of AStar
{
	ofstream osFile("astar.csv");

	// Check if the file was opened.
	if (!osFile.is_open())
	{
		return false;
	}

	osFile << "X,Y\n";
	for (int r = 0; r < 150; r++)
	{
		for (int c = 0; c < 2; c++)
			if(aai[r][0] != -1)
				osFile << aai[r][c] << ",";

		osFile << "\n";
	}
		

	// Close the file.
	osFile.close();
	return true;
}

bool FileManagement::CanAddScore(int iGame, int iScore)
{
	Scores scores = LoadScores(iGame);
	for (int i = 0; i < 10; i++)
	{
		if (!scores.m_ascoList[i].m_bLoaded)
			return true;
		if (scores.m_ascoList[i].m_iScore < iScore)
			return true;
	}
	return false;
}

int FileManagement::WhereScore(int iGame, int iScore)
{
	Scores scores = LoadScores(iGame);
	for (int i = 0; i < 10; i++)
	{
		if (!scores.m_ascoList[i].m_bLoaded)
			return i;
		if (scores.m_ascoList[i].m_iScore < iScore)
			return i;
	}
	return -1;
}
bool FileManagement::AddScore(int iGame, int iScore, string sName)
{
	Scores scores = LoadScores(iGame);
	if (!scores.m_bLoaded)
		CreateScoreFile(iGame);
	bool bForceEntry = CanAddScore(iGame, iScore);
	scores.InsertNewScore(iScore, sName, bForceEntry);
	scores.Reorder();
	scores.WriteScoresToFile(iGame);
	return true;
}

bool FileManagement::CreateScoreFile(int iGame)
{
	ofstream scoreFile("Scores_" + to_string(iGame) + ".txt");
	scoreFile.close();
	return true;
}

FileManagement::Scores FileManagement::LoadScores(int iGame)
{
	ifstream ifsScoreFile("Scores_" + to_string(iGame) + ".txt");
	FileManagement::Scores fmsReturnScores;
	if (!ifsScoreFile.is_open())
		return fmsReturnScores;

	fmsReturnScores.m_bLoaded = true;
	string sLine;
	Score scoreCurrent;
	while (getline(ifsScoreFile, sLine))
	{
		scoreCurrent.ExtractFromLine(sLine);
		if(scoreCurrent.m_bLoaded)
			fmsReturnScores.InsertNewScore(scoreCurrent.m_iScore, scoreCurrent.m_sName);
		scoreCurrent.Reset();
	}
	ifsScoreFile.close();
	fmsReturnScores.m_iGame = iGame;
	return fmsReturnScores;
}

int FileManagement::ReturnHighScore(int iGame)
{
	FileManagement::Scores fms = LoadScores(iGame);
	return fms.m_ascoList[0].m_iScore;
}

int FileManagement::ConvertStringToInteger(string sNumber)
{
	if (sNumber.length() > 501)
		return -9001;
	int iNumber;

	try
	{
		iNumber = stoi(sNumber, nullptr, 0);
	}
	catch (const std::invalid_argument& ia) //If the above fails (you've not entered a number)
	{
		return -9001;
	}
	return iNumber;
}

array <array <char, 31>, 28> FileManagement::LoadLevelFromFile(string sLevelLocation)
{
	array <array <char, 31>, 28> aac;
	ifstream ifsScoreFile("Levels/" + sLevelLocation + ".txt");
	string sLine;
	int y = 0;
	while (getline(ifsScoreFile, sLine))
	{
		for (int x = 0; x < 28; x++)
			aac[x][y] = tolower(sLine[x]);
		y++;
		if (y > 31) break;
	}
	return aac;
}

/*
	STRUCTS:

					SCORE
*/

bool FileManagement::Score::ExtractFromLine(string sLine)
{
	int iScoreCheck = 0;
	size_t found = sLine.find("[NEXT]");
	if (found == string::npos)
	{
		m_bLoaded = false;
		return false;
	}

	iScoreCheck = FileManagement::ConvertStringToInteger(sLine.substr(0, found));
	if (iScoreCheck <= 0)
	{
		m_bLoaded = false;
		return false;
	}

	m_iScore = iScoreCheck;
	m_sName = sLine.substr(found + 6, sLine.length());
	m_bLoaded = true;
	return true;
}

bool FileManagement::Score::Reset()
{
	m_iScore = 0;
	m_sName = "";
	m_bLoaded = false;
	return true;
}


bool FileManagement::Score::SetScore(int iScore, string sName)
{
	m_iScore = iScore;
	m_sName = sName;
	return true;
}
/*
	STRUCTS:
				SCORES
*/
bool FileManagement::Scores::InsertNewScore(int iScore, string sName, bool bForceInsert)
{
	for (int i = 0; i < 10; i++)
	{
		if (m_ascoList[i].m_bLoaded)
			continue;

		m_ascoList[i].m_iScore = iScore;
		m_ascoList[i].m_sName = sName;
		m_ascoList[i].m_bLoaded = true;
		return true;
	}
	if (bForceInsert)
	{
		m_ascoList[9].m_iScore = iScore;
		m_ascoList[9].m_sName = sName;
		m_ascoList[9].m_bLoaded = true;
		return true;
	}
	return false;
}

bool FileManagement::Scores::WriteScoresToFile(int iGame)
{
	ofstream scoreFile("Scores_" + to_string(iGame) + ".txt");
	for (int i = 0; i < 10; i++)
	{
		scoreFile << m_ascoList[i].m_iScore << "[NEXT]" << m_ascoList[i].m_sName << endl;
	}
	scoreFile.close();
	return true;
}

bool FileManagement::Scores::Reorder()
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 9; j++)
		{
			if (m_ascoList[j].m_iScore > m_ascoList[j + 1].m_iScore)
				SwapScore(j, j + 1);
		}
	array <Score, 10> ascoList = m_ascoList;
	for (int i = 0; i < 10; i++)
		m_ascoList[9 - i] = ascoList[i];
	return true;
}

bool FileManagement::Scores::SwapScore(int iFirst, int iSecond)
{
	Score scoreFirst = m_ascoList[iFirst];
	m_ascoList[iFirst] = m_ascoList[iSecond];
	m_ascoList[iSecond] = scoreFirst;
	return true;

}

void FileManagement::Scores::OutputToConsole()
{
	for (int i = 0; i < 10; i++)
		cout << "INDEX: " << i << " NAME: " << m_ascoList[i].m_sName << " SCORE: " << m_ascoList[i].m_iScore << " LOADED: " << m_ascoList[i].m_bLoaded << endl;
	cout << "GAME: " << m_iGame << " LOADED: " << m_bLoaded << endl;
}