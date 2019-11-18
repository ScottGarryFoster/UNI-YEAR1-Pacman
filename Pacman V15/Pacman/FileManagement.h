#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"

#include <sstream>
#include <array>
#include <string>
#include <iostream>
#include <cstdlib>
#include <iostream>
#include <fstream>

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

static class FileManagement
{
private:

public:

	static bool AStarExport(array <array <int, 2>, 400> aai); //Used for Testing
	
	//Below are Files used for Scores:

	static bool CanAddScore(int iGame, int iScore);

	static int WhereScore(int iGame, int iScore);

	static int ConvertStringToInteger(string sNumber);

	static bool AddScore(int iGame, int iScore, string sName);

	static bool CreateScoreFile(int iGame);

	struct Score
	{
		int m_iScore = 0;
		string m_sName = "";
		bool m_bLoaded = false;

		bool SetScore(int iScore, string sName);

		bool ExtractFromLine(string sLine);

		bool Reset();
	};

	struct Scores
	{
		array <Score, 10> m_ascoList;
		int m_iGame = -1;
		bool m_bLoaded = false;

		bool InsertNewScore(int iScore, string sName, bool bForceInsert = false);

		bool WriteScoresToFile(int iGame);

		bool Reorder();

		bool SwapScore(int iFirst, int iSecond);

		void OutputToConsole();
	};

	static Scores LoadScores(int iGame);

	static int ReturnHighScore(int iGame);

	//Below is the Level Loader:

	static array <array <char, 31>, 28> LoadLevelFromFile(string sLevelLocation);
};