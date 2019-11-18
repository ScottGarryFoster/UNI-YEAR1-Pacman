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
#include "Pacman.h"
#include "General.h"

#include <array>
#include <string>

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

class prisonDetails
{
private:
	tilePosition m_aiTopLeftTile;
	tilePosition m_aiTopRightPrison;
	tilePosition m_aiBottomLeftPrison;
	tilePosition m_aiBottomRightPrison;
	tilePosition m_tpPrisonEnterance;
public:
	bool m_bHaveDefined = false;//Because this is used to avoid running get, I don't mind this being public

	enum detail { topLeft = 0, topRight, bottomLeft, bottomRight, enterance};

	void getPrisonInfo(array <array <tile, 31>, 28> *aat);

	tilePosition getPrisonDetail(detail dReturn);
};