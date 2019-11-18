#include "Pacman.h"
#include "General.h"
#include "PrisonDetails.h"
#include "Ghost.h"

#include <sstream>
#include <array>
#include <string>
#include <iostream>

using namespace S2D;

void prisonDetails::getPrisonInfo(array <array <tile, 31>, 28> *aat)
{
	array <array <tile, 31>, 28> lcl_Aat = *aat;

	bool bSetTopLeftPrison = false;
	int iFirstY = -1;

	for (int x = 0; x < 28; x++)
		for (int y = 0; y < 31; y++)
			if (lcl_Aat[x][y].bPrison)
			{
				if (!bSetTopLeftPrison)
				{
					m_aiTopLeftTile.setPosition(x, y);
					bSetTopLeftPrison = true;
					iFirstY = y;
				}
				else if (y == iFirstY)
				{
					m_aiTopRightPrison.setPosition(x, y);
				}
				m_aiBottomRightPrison.setPosition(x, y);

				if (lcl_Aat[x][y].bPrisonEnterance)
					m_tpPrisonEnterance.setPosition(x, y);
			}

	m_aiBottomLeftPrison.setPosition(
		m_aiTopLeftTile.getCord(),
		m_aiBottomRightPrison.getCord(true)
	);


	if (m_aiTopLeftTile.getCord() == -1)
		cout << "Ghost::No Prison" << endl;

	m_bHaveDefined = true;//Just used so we don't run this more than we need to
}

tilePosition prisonDetails::getPrisonDetail(detail dReturn)
{
	switch (dReturn)
	{
	case topLeft:
		return m_aiTopLeftTile;
		break;
	case topRight:
		return m_aiTopRightPrison;
		break;
	case bottomLeft:
		return m_aiBottomLeftPrison;
	case bottomRight:
		return m_aiBottomRightPrison;
	case enterance:
	default:
		return m_tpPrisonEnterance;
	}
}