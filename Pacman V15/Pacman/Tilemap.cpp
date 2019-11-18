#include "Pacman.h"
#include "Ghost.h"
#include "General.h"
#include "FileManagement.h"

#include <iostream>
#include <sstream>
#include <array>
#include <string>

bool Pacman::TilesetLoadContent()
{
	//Setup Tilemap tiles:
	m_tTileTexture = new Texture2D();
	m_tTileTexture->Load("Textures/p_tileset24.png", true);
	m_rTileRect = new Rect(7 * m_fSpriteWidthHeight, 0.0f, m_fTilemapWidthHeight, m_fTilemapWidthHeight);
	m_vTilePosition = new Vector2(0, 3 * m_fSpriteWidthHeight);

	//Load a level (even if we switch later on)
	m_aatTilemap = Tileset_DrawTileset(1);
	array <float, 3> afPacPos = Tileset_StartPositionPacman(1);
	m_vPacmanPosition = new Vector2(afPacPos[0] - (m_fSpriteWidthHeight / 2), afPacPos[1] - (m_fSpriteWidthHeight / 2));
	m_iMovementKeyPress = (int)afPacPos[2];

	return true;
}

array <array <tile, 31>, 28> Pacman::Tileset_DrawTileset(int level)
{
	if (level < 1 || level > 10)
		level = 1;

	array <array <tile, 31>, 28> returnA;//What we'll return
	//cout << "SET: ";
	for (int x = 0; x < 28; x++)
		for (int y = 0; y < 31; y++)
		{
			//cout << "(" << x << ", " << y << "), ";
			returnA[x][y].tpLocation.setPosition(x, y);//Set the default locations. Used for neighbours
		}
	//cout << endl;
			

	tile passable; passable.bPassable = false;//Used to set the tiles we're attempting to update
	tile prison; prison.bPassable = false; prison.bPrison = true;
	tile tCorner;

	tile passable2; passable2.bPassable = true;

	if (level > 2)
		Tileset_DrawTilesetFromFile(to_string(level), &returnA, &m_atcCollectables);
	else
	{
		switch (level)
		{
		case 1:
			//Outlines:
			Tileset_DrawBox(&returnA, 0, 0, 28, 1, passable);//Top Line
			Tileset_DrawBox(&returnA, 0, 0, 1, 10, passable);//Left Top Line
			Tileset_DrawBox(&returnA, 27, 0, 1, 10, passable);//Right Top Line

			Tileset_DrawBox(&returnA, 0, 9, 6, 5, passable);//Left Top
			Tileset_DrawBox(&returnA, 22, 9, 6, 5, passable);//Right Top

			Tileset_DrawBox(&returnA, 0, 15, 6, 5, passable);//Left Bottom
			Tileset_DrawBox(&returnA, 22, 15, 6, 5, passable);//Right Bottom

			Tileset_DrawBox(&returnA, 0, 20, 1, 11, passable);//Left Bottom Line
			Tileset_DrawBox(&returnA, 27, 20, 1, 11, passable);//Right Bottom Line
			Tileset_DrawBox(&returnA, 0, 30, 28, 1, passable);//Bottom Line

			//Box Row 1:
			Tileset_DrawBox(&returnA, 2, 2, 4, 3, passable);
			Tileset_DrawBox(&returnA, 7, 2, 5, 3, passable);
			Tileset_DrawBox(&returnA, 13, 1, 2, 4, passable);
			Tileset_DrawBox(&returnA, 16, 2, 5, 3, passable);
			Tileset_DrawBox(&returnA, 22, 2, 4, 3, passable);

			//Box Row 2:
			Tileset_DrawBox(&returnA, 2, 6, 4, 2, passable);
			Tileset_DrawBox(&returnA, 7, 6, 2, 8, passable); Tileset_DrawBox(&returnA, 9, 9, 3, 2, passable);
			Tileset_DrawBox(&returnA, 10, 6, 8, 2, passable); Tileset_DrawBox(&returnA, 13, 8, 2, 3, passable);
			Tileset_DrawBox(&returnA, 19, 6, 2, 8, passable); Tileset_DrawBox(&returnA, 16, 9, 3, 2, passable);
			Tileset_DrawBox(&returnA, 22, 6, 4, 2, passable);

			//Box Row 3:
			Tileset_DrawBox(&returnA, 7, 15, 2, 5, passable);
			Tileset_DrawBox(&returnA, 10, 18, 8, 2, passable); Tileset_DrawBox(&returnA, 13, 20, 2, 3, passable);
			Tileset_DrawBox(&returnA, 19, 15, 2, 5, passable);

			//Box Row 4:
			Tileset_DrawBox(&returnA, 2, 21, 4, 2, passable); Tileset_DrawBox(&returnA, 4, 23, 2, 3, passable);
			Tileset_DrawBox(&returnA, 7, 21, 5, 2, passable);
			Tileset_DrawBox(&returnA, 16, 21, 5, 2, passable);
			Tileset_DrawBox(&returnA, 22, 21, 4, 2, passable); Tileset_DrawBox(&returnA, 22, 23, 2, 3, passable);

			//Box Row 5:
			Tileset_DrawBox(&returnA, 1, 24, 2, 2, passable);
			Tileset_DrawBox(&returnA, 7, 24, 2, 3, passable); Tileset_DrawBox(&returnA, 2, 27, 10, 2, passable);
			Tileset_DrawBox(&returnA, 10, 24, 8, 2, passable); Tileset_DrawBox(&returnA, 13, 26, 2, 3, passable);
			Tileset_DrawBox(&returnA, 19, 24, 2, 3, passable); Tileset_DrawBox(&returnA, 16, 27, 10, 2, passable);
			Tileset_DrawBox(&returnA, 25, 24, 2, 2, passable);

			//Open a tunnel above
			/*Tileset_DrawBox(&returnA, 10, 1, 2, 1, passable);
			Tileset_DrawBox(&returnA, 10, 29, 2, 1, passable);
			Tileset_DrawBox(&returnA, 13, 29, 2, 1, passable);
			passable.bPassable = true;
			Tileset_DrawBox(&returnA, 12, 0, 1, 1, passable);
			Tileset_DrawBox(&returnA, 12, 30, 1, 1, passable);*/

			//Prison
			Tileset_DrawBox(&returnA, 10, 12, 8, 5, prison);
			prison.bPrisonEnterance = true;
			Tileset_DrawBox(&returnA, 13, 12, 2, 1, prison);

			//Ghost Corners
			tCorner = returnA[26][1]; tCorner.m_bIsCorner = true;
			//Red:
			tCorner.m_bIsCornerMain = true; tCorner.m_gtCornerType = gtRed;
			Tileset_DrawBox(&returnA, 26, 1, 1, 1, tCorner);
			tCorner.m_bIsCornerMain = false;
			Tileset_DrawBox(&returnA, 21, 5, 1, 1, tCorner);
			//Pink:
			tCorner.m_bIsCornerMain = true; tCorner.m_gtCornerType = gtPink;
			Tileset_DrawBox(&returnA, 1, 1, 1, 1, tCorner);
			tCorner.m_bIsCornerMain = false;
			Tileset_DrawBox(&returnA, 6, 5, 1, 1, tCorner);
			//Blue:
			tCorner.m_bIsCornerMain = true; tCorner.m_gtCornerType = gtBlue;
			Tileset_DrawBox(&returnA, 1, 29, 1, 1, tCorner);
			tCorner.m_bIsCornerMain = false;
			Tileset_DrawBox(&returnA, 9, 23, 1, 1, tCorner);
			//Orange:
			tCorner.m_bIsCornerMain = true; tCorner.m_gtCornerType = gtOrange;
			Tileset_DrawBox(&returnA, 26, 29, 1, 1, tCorner);
			tCorner.m_bIsCornerMain = false;
			Tileset_DrawBox(&returnA, 18, 23, 1, 1, tCorner);

			m_tpFruitSpawnLocation.setPosition(13, 17);//Set Fruit Spawn
			m_tpPacmanSpawn.setPosition(13, 23);//Set Pacman Spawn
			break;
		case 2:
			Tileset_DrawBox(&returnA, 0, 0, 28, 1, passable);//Top Line
			Tileset_DrawBox(&returnA, 0, 0, 1, 10, passable);//Left Top Line
			Tileset_DrawBox(&returnA, 0, 9, 3, 4, passable);//Left LineBox
			Tileset_DrawBox(&returnA, 27, 0, 1, 10, passable);//Right Top Line
			Tileset_DrawBox(&returnA, 25, 9, 3, 4, passable);//Left LineBox

			Tileset_DrawBox(&returnA, 0, 14, 4, 2, passable); Tileset_DrawBox(&returnA, 4, 12, 2, 6, passable);// Left Outcropping
			Tileset_DrawBox(&returnA, 24, 14, 4, 2, passable); Tileset_DrawBox(&returnA, 22, 12, 2, 6, passable);// Right Outcropping

			Tileset_DrawBox(&returnA, 0, 17, 3, 6, passable);//Left Bottom LineBox
			Tileset_DrawBox(&returnA, 0, 23, 1, 8, passable);//Left Bottom Line
			Tileset_DrawBox(&returnA, 25, 17, 3, 6, passable);//Right Bottom LineBox
			Tileset_DrawBox(&returnA, 27, 23, 1, 8, passable);//Right Bottom Line
			Tileset_DrawBox(&returnA, 0, 30, 28, 1, passable);//Bottom Line
			//Bottom Outcroping here

			//Box Row 1:
			Tileset_DrawBox(&returnA, 2, 2, 2, 6, passable); Tileset_DrawBox(&returnA, 4, 6, 2, 2, passable);
			Tileset_DrawBox(&returnA, 5, 2, 4, 3, passable);
			Tileset_DrawBox(&returnA, 10, 2, 8, 2, passable); Tileset_DrawBox(&returnA, 10, 4, 2, 4, passable); Tileset_DrawBox(&returnA, 16, 4, 2, 4, passable);
			Tileset_DrawBox(&returnA, 19, 2, 4, 3, passable);
			Tileset_DrawBox(&returnA, 24, 2, 2, 6, passable); Tileset_DrawBox(&returnA, 22, 6, 2, 2, passable);

			//Box Row 2:
			Tileset_DrawBox(&returnA, 7, 6, 2, 8, passable); Tileset_DrawBox(&returnA, 4, 9, 8, 2, passable);
			Tileset_DrawBox(&returnA, 13, 5, 2, 6, passable);
			Tileset_DrawBox(&returnA, 19, 6, 2, 8, passable); Tileset_DrawBox(&returnA, 16, 9, 8, 2, passable);

			//Box Row 3:
			Tileset_DrawBox(&returnA, 7, 15, 2, 5, passable); Tileset_DrawBox(&returnA, 9, 18, 3, 2, passable);
			Tileset_DrawBox(&returnA, 13, 18, 2, 5, passable);
			Tileset_DrawBox(&returnA, 19, 15, 2, 5, passable); Tileset_DrawBox(&returnA, 16, 18, 3, 2, passable);

			//Box Row 4:
			Tileset_DrawBox(&returnA, 4, 19, 2, 4, passable); Tileset_DrawBox(&returnA, 6, 21, 3, 2, passable);
			Tileset_DrawBox(&returnA, 10, 21, 2, 5, passable); Tileset_DrawBox(&returnA, 12, 24, 4, 2, passable); Tileset_DrawBox(&returnA, 16, 21, 2, 5, passable);
			Tileset_DrawBox(&returnA, 22, 19, 2, 4, passable); Tileset_DrawBox(&returnA, 19, 21, 3, 2, passable);

			//Box Row 5:
			Tileset_DrawBox(&returnA, 2, 24, 4, 2, passable); Tileset_DrawBox(&returnA, 2, 26, 2, 3, passable);
			Tileset_DrawBox(&returnA, 7, 24, 2, 3, passable); Tileset_DrawBox(&returnA, 5, 27, 7, 2, passable);
			Tileset_DrawBox(&returnA, 13, 27, 2, 3, passable);
			Tileset_DrawBox(&returnA, 19, 24, 2, 3, passable); Tileset_DrawBox(&returnA, 16, 27, 7, 2, passable);
			Tileset_DrawBox(&returnA, 22, 24, 4, 2, passable); Tileset_DrawBox(&returnA, 24, 26, 2, 3, passable);

			//Pen
			Tileset_DrawBox(&returnA, 10, 12, 8, 5, prison);
			prison.bPrisonEnterance = true;
			Tileset_DrawBox(&returnA, 13, 12, 2, 1, prison);

			//Ghost Corners
			//Red:
			tCorner = returnA[26][1]; tCorner.m_bIsCorner = true; tCorner.m_bIsCornerMain = true; tCorner.m_gtCornerType = gtRed;
			Tileset_DrawBox(&returnA, 26, 1, 1, 1, tCorner);
			tCorner.m_bIsCornerMain = false;
			Tileset_DrawBox(&returnA, 21, 8, 1, 1, tCorner);
			//Pink:
			tCorner.m_bIsCornerMain = true; tCorner.m_gtCornerType = gtPink;
			Tileset_DrawBox(&returnA, 1, 1, 1, 1, tCorner);
			tCorner.m_bIsCornerMain = false;
			Tileset_DrawBox(&returnA, 6, 8, 1, 1, tCorner);
			//Blue:
			tCorner.m_bIsCornerMain = true; tCorner.m_gtCornerType = gtBlue;
			Tileset_DrawBox(&returnA, 1, 29, 1, 1, tCorner);
			tCorner.m_bIsCornerMain = false;
			Tileset_DrawBox(&returnA, 6, 23, 1, 1, tCorner);
			//Orange:
			tCorner.m_bIsCornerMain = true; tCorner.m_gtCornerType = gtOrange;
			Tileset_DrawBox(&returnA, 26, 29, 1, 1, tCorner);
			tCorner.m_bIsCornerMain = false;
			Tileset_DrawBox(&returnA, 21, 23, 1, 1, tCorner);

			m_tpFruitSpawnLocation.setPosition(13, 17);//Set Fruit Spawn
			m_tpPacmanSpawn.setPosition(13, 23);//Set Pacman Spawn
			break;
		}
	}
	returnA = Tileset_SetImageForTiles(returnA);
	Tileset_SetExits(&returnA);
	Tileset_SetTunnels(&returnA);
	return returnA;
}

array <float, 3> Pacman::Tileset_StartPositionPacman(int level)
{
	switch (level)
	{
	case 1:
	default:
		//Orignally 17 on the Y but that's incorrect
		return { (m_tpPacmanSpawn.getCord(false) * m_fTilemapWidthHeight) + (m_fTilemapWidthHeight / 2) + (m_fSpriteWidthHeight / 2), (m_tpPacmanSpawn.getCord(true) * m_fTilemapWidthHeight) + (m_fTilemapWidthHeight / 2) + m_fTilemapMarginTop, 4 };
		//return { (m_fTilemapWidthHeight * 13) + 8, (m_fTilemapWidthHeight * 18) + 16, 4 };//+ 20
	}
}

void Pacman::Tileset_DrawBox(array <array <tile, 31>, 28>* at, int x, int y, int width, int height, tile t)
{
	if (x > 27 || x < 0 || y > 30 || y < 0 || x + width > 28 || y + height > 31)
	{
		cout << "Tileset_DrawBox: Could not draw box at: x: " << x << " y: " << y << endl;
	}
	else
	{
		array <array <tile, 31>, 28> returnArray = *at;

		for (int i = x; i < x + width; i++)
			for (int j = y; j < y + height; j++)
				returnArray[i][j].UpdateTileInformation(t);
		*at = returnArray;
	}
}

void Pacman::Tileset_SetExits(array <array <tile, 31>, 28>* at)
{
	array <tile, 9> atTileNeighbours;
	array <array <tile, 31>, 28> atReturn = *at;
	int iNumberOfNeightbours = 0;

	for (int x = 0; x < 28; x++)
		for (int y = 0; y < 31; y++)
		{
			iNumberOfNeightbours = 0;
			atTileNeighbours = Tileset_GetInformationAroundTile(*at, { x,y });
			if (atTileNeighbours[2].bPassable) iNumberOfNeightbours++;
			if (atTileNeighbours[4].bPassable) iNumberOfNeightbours++;
			if (atTileNeighbours[6].bPassable) iNumberOfNeightbours++;
			if (atTileNeighbours[8].bPassable) iNumberOfNeightbours++;

			if (iNumberOfNeightbours > 0)
				atReturn[x][y].bIsExit = true;
		}
	*at = atReturn;
}

void Pacman::Tileset_SetTunnels(array <array <tile, 31>, 28>* aat)
{
	array <array <tile, 31>, 28> aatLocal = *aat;
	array <array <bool, 31>, 28> aabClosedList;
	for (int x = 0; x < 28; x++)
		for (int y = 0; y < 31; y++)
			aabClosedList[x][y] = false;

	array <tile, 9> atTileNeighbours;
	int iNumberOfNeightbours = 0;
	array <int, 2> aiLocalParent = { -1,-1 }, aiForeignParent = { -1, -1 }, aiCurrentTile = { -1,-1 };
	/*
		FIND A TUNNEL TILE:
		Might seem counter intuitive to look in all tiles
		however I want one peice of code for vertical and horizontal tiles.

		Additionally some of the if statments are broken up into stages just to make
		it more readable.
	*/
	for (int x = 0; x < 28; x ++)
	{
		for (int y = 0; y < 31; y++)
		{
			if (x != 0 && x != 27 && y != 0 && y != 30)//Not an edge tile
				continue;
			else if (!aatLocal[x][y].bPassable)//Isn't passable
				continue;

			//The following will exist for all tunnel tiles:
			if (x == 0)
				aiLocalParent = { 27,y };//Parent on the other side
			else if (x == 27)
				aiLocalParent = { 0,y };//Parent on the other side
			else if (y == 0)
				aiLocalParent = { x,30 };//Parent on the other side
			else if (y == 30)
				aiLocalParent = { x,0 };//Parent on the other side
			else
				aiLocalParent = { -1,-1 };//Used for error checking

			if (x == 0)
				aiForeignParent = { 27,y };//Parent on the other side
			else if (x == 27)
				aiForeignParent = { 0,y };//Parent on the other side
			else if (y == 0)
				aiForeignParent = { x,30 };//Parent on the other side
			else if (y == 30)
				aiForeignParent = { x,0 };//Parent on the other side
			else
				aiForeignParent = { -1,-1 };//Used for error checking

			aiCurrentTile = { x, y };
			while (true)
			{
				iNumberOfNeightbours = 0;
				atTileNeighbours = Tileset_GetInformationAroundTile(*aat, aiCurrentTile);
				if (atTileNeighbours[2].bPassable) iNumberOfNeightbours++;
				if (atTileNeighbours[4].bPassable) iNumberOfNeightbours++;
				if (atTileNeighbours[6].bPassable) iNumberOfNeightbours++;
				if (atTileNeighbours[8].bPassable) iNumberOfNeightbours++;

				if (iNumberOfNeightbours > 2)//There is more than 2 directions. In otherwords a choice.
					break;

				aabClosedList[aiCurrentTile[0]][aiCurrentTile[1]] = true;//Don't look at the tile again

				//Stuff with current tile:
				aatLocal[aiCurrentTile[0]][aiCurrentTile[1]].m_bIsTunnel = true;//This is a tunnel tile
				aatLocal[aiCurrentTile[0]][aiCurrentTile[1]].m_tpLocalParent.setPosition(aiLocalParent[0], aiLocalParent[1]);//Set the local parent
				aatLocal[aiCurrentTile[0]][aiCurrentTile[1]].m_tpForeignParent.setPosition(aiForeignParent[0], aiForeignParent[1]);//Set the foreign parent

				//If the tile is passable and we've not already set a parent/tunnel for this: (cycles 2,4,6,8)
				for(int i = 2; i <= 8; i += 2)
					if (atTileNeighbours[i].bPassable && !aabClosedList[atTileNeighbours[i].tpLocation.getCord(false)][atTileNeighbours[i].tpLocation.getCord(true)])
						aiCurrentTile = { atTileNeighbours[i].tpLocation.getCord(false), atTileNeighbours[i].tpLocation.getCord(true) };//Get the new potential tunnel tile
			}

		}
	}
	*aat = aatLocal;//Update
}

array <array <tile, 31>, 28> Pacman::Tileset_SetImageForTiles(array <array <tile, 31>, 28> at)
{
	tile tNorth, tEast, tWest, tSouth, tAccess, tNorthEast, tNorthWest, tSouthEast, tSouthWest;
	for (int i = 0; i < 2; i++)
	{
		for (int x = 0; x < 28; x++)
		{
			for (int y = 0; y < 31; y++)
			{
				tAccess = at[x][y];
				if (tAccess.bPassable)
					at[x][y].iTileshown = 33;
				if (tAccess.bPassable || at[x][y].iTileshown != 0)
					continue;

				if (x <= 0)
					tWest = tAccess;
				else
					tWest = at[x - 1][y];

				if (x >= 27)
					tEast = tAccess;
				else
					tEast = at[x + 1][y];

				if (y <= 0)
					tNorth = tAccess;
				else
					tNorth = at[x][y - 1];

				if (y >= 30)
					tSouth = tAccess;
				else
					tSouth = at[x][y + 1];

				//Diagonal
				if (!(y <= 0 || x <= 0))
					tNorthWest = at[x - 1][y - 1];
				else
					tNorthWest = tAccess;

				if (!(y <= 0 || x >= 27))
					tNorthEast = at[x + 1][y - 1];
				else
					tNorthEast = tAccess;

				if (!(y >= 30 || x >= 27))
					tSouthEast = at[x + 1][y + 1];
				else
					tSouthEast = tAccess;

				if (!(y >= 30 || x <= 0))
					tSouthWest = at[x - 1][y + 1];
				else
					tSouthWest = tAccess;


				//NONE PRISON

				if (tNorth.bPassable && tWest.bPassable && !tSouth.bPassable && !tEast.bPassable && !tAccess.bPrison)
					at[x][y].iTileshown = 2;//Top Left
				else if (tNorth.bPassable && tEast.bPassable && !tSouth.bPassable && !tWest.bPassable && !tAccess.bPrison)
					at[x][y].iTileshown = 4;//Top Right
				else if (!tNorth.bPassable && tSouth.bPassable && tWest.bPassable && !tEast.bPassable && !tAccess.bPrison)
					at[x][y].iTileshown = 62;//Bottom Left
				else if (!tNorth.bPassable && tSouth.bPassable && !tWest.bPassable && tEast.bPassable && !tAccess.bPrison)
					at[x][y].iTileshown = 64;//Bottom Right
				else if (!tNorth.bPassable && !tSouth.bPassable && tWest.bPassable && !tEast.bPassable && !tAccess.bPrison)
					at[x][y].iTileshown = 12;//Left
				else if (!tNorth.bPassable && !tSouth.bPassable && !tWest.bPassable && tEast.bPassable && !tAccess.bPrison)
					at[x][y].iTileshown = 14;//Right
				else if (tNorth.bPassable && !tSouth.bPassable && !tWest.bPassable && !tEast.bPassable && !tAccess.bPrison)
					at[x][y].iTileshown = 3;//Top
				else if (!tNorth.bPassable && tSouth.bPassable && !tWest.bPassable && !tEast.bPassable && !tAccess.bPrison)
					at[x][y].iTileshown = 63;//Bottom

				//PRISON:

				else if (tNorth.bPassable && tWest.bPassable && !tSouth.bPassable && !tEast.bPassable && tAccess.bPrison)
					at[x][y].iTileshown = 56;//Top Left
				else if (tNorth.bPassable && tEast.bPassable && !tSouth.bPassable && !tWest.bPassable && tAccess.bPrison)
					at[x][y].iTileshown = 59;//Top Right
				else if (!tNorth.bPassable && tSouth.bPassable && tWest.bPassable && !tEast.bPassable && tAccess.bPrison)
					at[x][y].iTileshown = 86;//Bottom Left
				else if (!tNorth.bPassable && tSouth.bPassable && !tWest.bPassable && tEast.bPassable && tAccess.bPrison)
					at[x][y].iTileshown = 89;//Bottom Right
				else if (!tNorth.bPassable && !tSouth.bPassable && tWest.bPassable && !tEast.bPassable && tAccess.bPrison)
					if(tAccess.bPrisonEnterance)
						at[x][y].iTileshown = 47;//Left
					else
						at[x][y].iTileshown = 66;//Left
				else if (!tNorth.bPassable && !tSouth.bPassable && !tWest.bPassable && tEast.bPassable && tAccess.bPrison)
					if (tAccess.bPrisonEnterance)
						at[x][y].iTileshown = 49;//Right
					else
						at[x][y].iTileshown = 69;//Right
				else if (tNorth.bPassable && !tSouth.bPassable && !tWest.bPassable && !tEast.bPassable && tAccess.bPrison)
					if (tAccess.bPrisonEnterance)
						at[x][y].iTileshown = 38;//Top
					else
						at[x][y].iTileshown = 57;//Top
				else if (!tNorth.bPassable && tSouth.bPassable && !tWest.bPassable && !tEast.bPassable && tAccess.bPrison)
					if (tAccess.bPrisonEnterance)
						at[x][y].iTileshown = 48;//Bottom
					else
						at[x][y].iTileshown = 87;//Bottom


				//WEIRDER CORNERS:

				else if (tEast.iTileshown == 63 && tSouth.iTileshown == 14)
					at[x][y].iTileshown = 44;//InnerLineTopLeft
				else if (tEast.iTileshown == 3 && tNorth.iTileshown == 14)
					at[x][y].iTileshown = 24;//InnerLineBottomLeft
				else if ((tWest.iTileshown == 63 || tWest.iTileshown == 44) && tSouth.iTileshown == 12)
					at[x][y].iTileshown = 42;//Inner Corner Bottom Left
				else if ((tWest.iTileshown == 63 || tWest.iTileshown == 42) && tSouth.iTileshown == 14)
					at[x][y].iTileshown = 44;//Inner Corner Bottom Right
				else if ((tNorth.iTileshown == 2 || tNorth.iTileshown == 12) && tWest.iTileshown == 3)
					at[x][y].iTileshown = 22;//Inner Corner Top Left

				//DOUBLE INNER CORNERS (OverLapping)
				else if (tNorthWest.bPassable && tSouthEast.bPassable && !tEast.bPassable && !tWest.bPassable && !tNorth.bPassable && !tSouth.bPassable)
					at[x][y].iTileshown = 82;//NW/SE
				else if (tNorthEast.bPassable && tSouthWest.bPassable && !tEast.bPassable && !tWest.bPassable && !tNorth.bPassable && !tSouth.bPassable)
					at[x][y].iTileshown = 84;//NE/SW

				//INNER CORNERS
				else if ((tWest.iTileshown == 63 || tWest.iTileshown == 62) && (tSouth.iTileshown == 12 || tSouth.iTileshown == 62))
					at[x][y].iTileshown = 42;//Inner Bottom Left 
				else if ((tWest.iTileshown == 2 || tWest.iTileshown == 3) && (tNorth.iTileshown == 12 || tNorth.iTileshown == 2))
					at[x][y].iTileshown = 22;//Inner Top Left 
				else if ((tEast.iTileshown == 64 || tEast.iTileshown == 63) && (tSouth.iTileshown == 14 || tSouth.iTileshown == 64))
					at[x][y].iTileshown = 44;//Inner Bottom Right 
				else if ((tEast.iTileshown == 4 || tEast.iTileshown == 3) && (tNorth.iTileshown == 4 || tNorth.iTileshown == 14))
					at[x][y].iTileshown = 24;//Inner Top Right

				if (i > 0 && at[x][y].iTileshown == 0)
					at[x][y].iTileshown = 33;
				
			}
		}
	}
	return at;
}

array <tile, 9> Pacman::Tileset_GetInformationAroundTile(array <array <tile, 31>, 28> at, array <int, 2> aiCurentTile)
{
	tile tAccess = at[aiCurentTile[0]][aiCurentTile[1]];
	array <tile, 9> retrunAt;//Whereever I missspell return it's so C++ never thinks I mean return. Just FYI.
	array <int, 2> aiParent = { -1,-1 };
	
	for (int i = 0; i < 9; i++)
		retrunAt[i] = tAccess;

	if (aiCurentTile[0] < 0)
		retrunAt[4] = tAccess;
	else if (aiCurentTile[0] == 0)
	{
		if (at[aiCurentTile[0]][aiCurentTile[1]].m_bIsTunnel)//We're dealing with a tunnel
		{
			aiParent = at[aiCurentTile[0]][aiCurentTile[1]].m_tpForeignParent.ConvertToAI();
			retrunAt[4] = at[aiParent[0]][aiParent[1]];
		}
		else
		{
			retrunAt[4] = tAccess;
		}
	}
	else
		retrunAt[4] = at[aiCurentTile[0] - 1][aiCurentTile[1]];

	if (aiCurentTile[0] > 27)
		retrunAt[6] = tAccess;
	else if (aiCurentTile[0] == 27)
	{
		if (at[aiCurentTile[0]][aiCurentTile[1]].m_bIsTunnel)//We're dealing with a tunnel
		{
			aiParent = at[aiCurentTile[0]][aiCurentTile[1]].m_tpForeignParent.ConvertToAI();
			retrunAt[6] = at[aiParent[0]][aiParent[1]];
		}
		else
		{
			retrunAt[6] = tAccess;
		}
	}
	else
		retrunAt[6] = at[aiCurentTile[0] + 1][aiCurentTile[1]];

	if (aiCurentTile[1] < 0)
		retrunAt[8] = tAccess;
	else if (aiCurentTile[1] == 0)
	{
		if (at[aiCurentTile[0]][aiCurentTile[1]].m_bIsTunnel)//We're dealing with a tunnel
		{
			aiParent = at[aiCurentTile[0]][aiCurentTile[1]].m_tpForeignParent.ConvertToAI();
			retrunAt[8] = at[aiParent[0]][aiParent[1]];
		}
		else
		{
			retrunAt[8] = tAccess;
		}
	}
	else
		retrunAt[8] = at[aiCurentTile[0]][aiCurentTile[1] - 1];

	if (aiCurentTile[1] > 30)
		retrunAt[2] = tAccess;
	else if (aiCurentTile[1] == 30)
	{
		if (at[aiCurentTile[0]][aiCurentTile[1]].m_bIsTunnel)//We're dealing with a tunnel
		{
			aiParent = at[aiCurentTile[0]][aiCurentTile[1]].m_tpForeignParent.ConvertToAI();
			retrunAt[2] = at[aiParent[0]][aiParent[1]];
		}
		else
		{
			retrunAt[2] = tAccess;
		}
	}
	else
		retrunAt[2] = at[aiCurentTile[0]][aiCurentTile[1] + 1];

	return retrunAt;
}

array <float, 2> Pacman::Tileset_ReturnCordOfTile(int x, int y)
{
	float returnX = (x * m_fTilemapWidthHeight) + (m_fTilemapWidthHeight / 2);
	float returnY = (y * m_fTilemapWidthHeight) + (m_fTilemapWidthHeight / 2) + m_fTilemapMarginTop;
	return { returnX, returnY };
}

array <int, 2> Pacman::Tileset_FindCloestPassableTile(array <array <tile, 31>, 28> aatLocal, array <int, 2> aiTestTile)
{
	int iNewDirection = 1;
	int iAttempts = 0;

	//Just ensure the tile is on the screen at the start:
	//Without this the while loop will check beyond the array
	auto adjustTileCheck = [](int* x, int* y)
	{
		if (*x < 0)		*x = 0;		else if (*x > 27)	*x = 27;
		if (*y < 0)		*y = 0;		else if (*y > 30)	*y = 30;
	};
	adjustTileCheck(&aiTestTile[0],&aiTestTile[1]);

	while (!aatLocal[aiTestTile[0]][aiTestTile[1]].bPassable)
	{
		if (aiTestTile[0] == 0)
		{
			aiTestTile[0] = 1;
			continue;
		}
		else if (aiTestTile[0] == 27)
		{
			aiTestTile[0] = 26;
			continue;
		}
		else if (aiTestTile[1] == 0)
		{
			aiTestTile[1] = 1;
			continue;
		}
		else if (aiTestTile[1] == 30)
		{
			aiTestTile[1] = 29;
			continue;
		}

		switch (iNewDirection)
		{
		case 1:
			aiTestTile[0] -= 1;
			aiTestTile[1] += 1;
			break;
		case 2:
			aiTestTile[1] += 1;
			break;
		case 3:
			aiTestTile[0] += 1;
			aiTestTile[1] += 1;
			break;
		case 4:
			aiTestTile[0] -= 1;
			iNewDirection++;
			break;
		case 6:
			aiTestTile[0] += 1;
			break;
		case 7:
			aiTestTile[0] -= 1;
			aiTestTile[1] -= 1;
			break;
		case 8:
			aiTestTile[1] -= 1;
			break;
		case 9:
			aiTestTile[0] += 1;
			aiTestTile[1] -= 1;
			break;
		case 10:
		default:
			iNewDirection = 0;
			break;
		}
		iNewDirection++;
		iAttempts++;

		//Just ensure the tile is on the screen at the start:
		adjustTileCheck(&aiTestTile[0], &aiTestTile[1]);


		if (iAttempts > 31 * 28)
			return { -1,-1 };
	}
	return aiTestTile;
}

array <array <int, 2>, 400> Pacman::BasicMovement_AStarCalc(classPacketPacman pacman, array <int, 2> aiOrigin, array <int, 2> aiTarget, bool bExportRoute, bool bOmmitOrigin, array <array <int, 2>, 10> aaiExpensiveTiles)
{
	//Used in loop
	array <int, 2> aiCurrentLowest = { -1, -1 };
	bool bMoreThanOneLowestF = false;
	tilePosition tpCurrent(-1, -1);
	int iMaxTimesForSearch = 28 * 31;//Max number of times Loop can run
	int iTimesRun = 0;

	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();//For Input

	if (keyboardState->IsKeyDown(Input::Keys::SPACE))
	{
		if (aaiExpensiveTiles[0][0] != -1) {
			for (int i = 0; i < 10; i++)
				cout << "aaiExpensiveTiles[" << i << "] = (" << aaiExpensiveTiles[i][0] << ", " << aaiExpensiveTiles[i][1] << ")" << endl;
		}
	}

	//Setup
	array <int, 2> aiCurrentTile = aiOrigin;
	array <array <tile, 31>, 28> aatLocal = *pacman.ptrAatTileMap;//Multidimentional arrays either need awquard passes or just a lcl copy by reference/pointer

	//Vaildation
	if (aiOrigin[0] == -1 || aiOrigin[1] == -1)
	{
		cout << "Pacman::BasicMovement_AStarCalc: Origin not passed: " << aiOrigin[0] << ", " << aiOrigin[1] << endl;
		return { {-1,-1} };
	}
	if (!aatLocal[aiOrigin[0]][aiOrigin[1]].bPassable)//Don't try and find a new origin
	{
		cout << "Pacman::BasicMovement_AStarCalc: Origin not Passable: " << aiOrigin[0] << ", " << aiOrigin[1] << endl;
		return { {-1,-1} };
	}

	aiTarget = Pacman::Tileset_FindCloestPassableTile(aatLocal, aiTarget);//But we can find a new target
	if (aiTarget[0] == -1 || aiTarget[1] == -1)
	{
		aiTarget = Pacman::Tileset_FindCloestPassableTile(aatLocal, { 14, 15 });//But we can find a new target
	}
	if (aiTarget[0] == -1 || aiTarget[1] == -1)
	{
		cout << "Pacman::BasicMovement_AStarCalc: Target not Passable and couldn't find a passable tile" << endl;
		return { {-1,-1} };
	}

	array <array <AStarScore, 31>, 28> aabScoreList;
	AStarScore assPassover, assTunnel, assFParent;//Used to shorten text later one

	array <array <bool, 31>, 28> aabOpenList;
	array <array <bool, 31>, 28> aabClosedList;
	for (int x = 0; x < 28; x++)//Defaults:
		for (int y = 0; y < 31; y++)
		{
			aabOpenList[x][y] = false;
			aabClosedList[x][y] = false;
		}

	//Calulate the origin Costs:
	aabScoreList[aiOrigin[0]][aiOrigin[1]] = BasicMovement_CalcFGH(aabScoreList, aiTarget, aiOrigin, aiOrigin);

	aabOpenList[aiOrigin[0]][aiOrigin[1]] = true; //Add the origin to the open list

	/*
		START OF SEARCH:
	*/

	bool bFoundTarget = false;//Could use true and break but there are loops within loops
	while (!bFoundTarget)
	{
		iTimesRun++;
		if (iTimesRun > iMaxTimesForSearch)
		{
			cout << "Pacman::BasicMovement_AStarCalc: No Route found between: " << aiOrigin[0] << ", " << aiOrigin[1] << " and " << aiTarget[0] << ", " << aiTarget[1] << endl;
			return { {-1,-1} };
		}

		aiCurrentLowest = { -1, -1 };//Find the node in the open list with the lowest Cost
		bMoreThanOneLowestF = false;//Used if two F cost are the same
		for (int x = 0; x < 28; x++)
			for (int y = 0; y < 31; y++)
			{
				if (aabOpenList[x][y] && !aabClosedList[x][y])//We can look at this tile
				{
					if (aiCurrentLowest[0] == -1)//This is the first open list tile
					{
						aiCurrentLowest = { x,y };//Set this as lowest
					}
					else if (aabScoreList[x][y].fFCost < aabScoreList[aiCurrentLowest[0]][aiCurrentLowest[1]].fFCost)
					{
						aiCurrentLowest = { x,y };//Set this as lowest
						bMoreThanOneLowestF = false;//There is currently a lowest F Cost
					}
					else if (aabScoreList[x][y].fFCost == aabScoreList[aiCurrentLowest[0]][aiCurrentLowest[1]].fFCost)
					{
						bMoreThanOneLowestF = true;//There is more than one lowest F Cost
					}
				}
			}

		if (bMoreThanOneLowestF)//If there are two F costs the same we instead look for the lowest H cost
		{
			for (int x = 0; x < 28; x++)
				for (int y = 0; y < 31; y++)
				{
					if (!aabOpenList[x][y] || aabClosedList[x][y])//We don't need to look here
						continue;

					if (aabScoreList[x][y].fFCost != aabScoreList[aiCurrentLowest[0]][aiCurrentLowest[1]].fFCost) //If the F cost differs I don't need to look here
						continue;

					if (aabScoreList[x][y].fHCost < aabScoreList[aiCurrentLowest[0]][aiCurrentLowest[1]].fHCost)//Instead look for H cost
						aiCurrentLowest = { x,y };//Set this as lowest
				}
		}


		aiCurrentTile = aiCurrentLowest;//Lowest cost is current
		if (aiCurrentTile[0] >= 0 && aiCurrentTile[0] < 28 && aiCurrentTile[1] >= 0 && aiCurrentTile[1] < 31)
		{
			aabOpenList[aiCurrentTile[0]][aiCurrentTile[1]] = false;//Remove it from the open list
			aabClosedList[aiCurrentTile[0]][aiCurrentTile[1]] = true;//Add to closed list
		}

		if (aiCurrentTile == aiTarget)
			bFoundTarget = true;

		array <tile, 9> atNeighbours = Pacman::Tileset_GetInformationAroundTile(*pacman.ptrAatTileMap, aiCurrentTile);
		for (int i = 2; i <= 8; i += 2)//For each neighbour
		{

			tpCurrent = atNeighbours[i].tpLocation;//Gets a bit lengthy so lets do this

			if (aabClosedList[tpCurrent.getCord(false)][tpCurrent.getCord(true)])
			{
				continue;//If it's in the closed list we do not need to look at it
			}
			else if (!aatLocal[tpCurrent.getCord(false)][tpCurrent.getCord(true)].bPassable)
			{
				aabClosedList[tpCurrent.getCord(false)][tpCurrent.getCord(true)] = true;//Pop it on the closed list to be sure
				continue; //We can't go there
			}

			//Calculate new score
			assPassover = BasicMovement_CalcFGH(aabScoreList, aiTarget, { tpCurrent.getCord(false), tpCurrent.getCord(true) }, aiCurrentTile);

			if (aaiExpensiveTiles[0][0] != -1)//Something was passed
				for (int i = 0; i < 10; i++)
					if (aaiExpensiveTiles[i][0] == tpCurrent.getCord() && aaiExpensiveTiles[i][1] == tpCurrent.getCord(true))
						assPassover.fFCost *= 1000;//Make the tile on the list expensive		

			if (aatLocal[tpCurrent.getCord(false)][tpCurrent.getCord(true)].m_bIsTunnel)//We're dealing with a tunnel
			{
				assFParent = BasicMovement_CalcFGH(aabScoreList, aiTarget, aatLocal[tpCurrent.getCord(false)][tpCurrent.getCord(true)].m_tpForeignParent.ConvertToAI(), aiCurrentTile);
				assTunnel = assPassover;
				assTunnel.fGCost = assFParent.fGCost;
				assTunnel.fFCost = assFParent.fGCost + assFParent.fHCost;

				if (aaiExpensiveTiles[0][0] != -1)//Something was passed
					for (int i = 0; i < 10; i++)
						if (aaiExpensiveTiles[i][0] == aiTarget[0] && aaiExpensiveTiles[i][1] == aiTarget[1])
							assTunnel.fFCost *= 1000;

				if (assTunnel.fFCost < assPassover.fFCost)
					assPassover = assTunnel;
			}

			aabScoreList[tpCurrent.getCord(false)][tpCurrent.getCord(true)] = assPassover;

			if (!aabOpenList[tpCurrent.getCord(false)][tpCurrent.getCord(true)])//If the current node is not on the open list
				aabOpenList[tpCurrent.getCord(false)][tpCurrent.getCord(true)] = true;//Put it on the open list
		}
	}
	if (bFoundTarget)//We found the target
	{
		tpCurrent.setPosition(aiTarget[0], aiTarget[1]);//Start from the target

		array <array <int, 2>, 400> aaiTargetToOrigin;//This is how the parent path is found
		array <array <int, 2>, 400> aaiOriginToTarget;//This is how I'd like to have the path list
		for (int r = 0; r < 400; r++)
			for (int c = 0; c < 2; c++)
			{
				aaiTargetToOrigin[r][c] = -1;
				aaiOriginToTarget[r][c] = -1;
			}


		int iRow = 0;

		while (tpCurrent.getCord(false) != aiOrigin[0] || tpCurrent.getCord(true) != aiOrigin[1])
		{
			if (iRow >= 400)//If it's more than 400 steps then it's not worth using (867 is the number of tiles)
			{
				cout << "Pacman::BasicMovement_AStarCalc: Too many tiles to move between tiles" << endl;
				return { {-1,-1} };
			}
			aaiTargetToOrigin[iRow] = { tpCurrent.getCord(false) , tpCurrent.getCord(true) };
			iRow++;

			assPassover = aabScoreList[tpCurrent.getCord(false)][tpCurrent.getCord(true)];//Current score which contains parent
			tpCurrent.setPosition(assPassover.aiParent[0], assPassover.aiParent[1]);//Move to parent
		}
		if (iRow >= 400)//If it's more than 400 steps then it's not worth using (867 is the number of tiles)
		{
			cout << "Pacman::BasicMovement_AStarCalc: Too many tiles to move between tiles" << endl;
			return { {-1,-1} };
		}
		if (!bOmmitOrigin)//Include Origin
			aaiTargetToOrigin[iRow] = { tpCurrent.getCord(false) , tpCurrent.getCord(true) };//Add Origin to the end
		else
			iRow--;

		for (int r = 0; r <= iRow; r++)
			aaiOriginToTarget[r] = aaiTargetToOrigin[iRow - r];//So I have to flip the path around


		if (bExportRoute)
			FileManagement::AStarExport(aaiOriginToTarget);

		return aaiOriginToTarget;
	}
	return { {-1,-1} };
}

Pacman::AStarScore Pacman::BasicMovement_CalcFGH(array <array <AStarScore, 31>, 28> aabScoreList, array <int, 2> aiTarget, array <int, 2> aiCurrent, array <int, 2> aiParent)
{
	/*
		Work out F, G, H
		G = DISTANCE FROM THE START TO THIS NEIGHBOUR
		H = DISTANCE FROM THE TARGET TO THE NEIGHBOUR ASSUMING NO WALLS
		F = G + H

		assPassover is the new neighbour score element used simply to shorten the lines and make it more readable
	*/
	AStarScore assPassover;

	assPassover.fGCost = aabScoreList[aiParent[0]][aiParent[1]].fGCost;//Distance from the start to the current tile
	assPassover.fGCost += 1;//Distance to the neighbour is therefore one step away from the current

	assPassover.fHCost = aiTarget[0] + aiTarget[1];//Target X + Y
	assPassover.fHCost -= aiCurrent[0] + aiCurrent[1];//Take away the neighbour location x + y
	assPassover.fHCost = abs(assPassover.fHCost);//Ensure this is a positive number

	assPassover.fFCost = assPassover.fGCost + assPassover.fHCost;//F = G + H

	assPassover.aiParent = aiParent;//Set the parent to current tile

	return assPassover;
}

void Pacman::Tileset_DrawTilesetFromFile(string sLevel, array <array <tile, 31>, 28>* aat, array <array <tileCollectables, 31>, 28>* m_atcCollectables)
{
	array <array <char, 31>, 28> aac = FileManagement::LoadLevelFromFile(sLevel);//Loads the level from file

	tile floor; floor.bPassable = true;//Used to set the tiles we're attempting to update
	tile prison; prison.bPassable = false; prison.bPrison = true;
	tile prisonEnt; prisonEnt.bPassable = false; prisonEnt.bPrison = true; prisonEnt.bPrisonEnterance = true;
	tile wall; wall.bPassable = false;

	tile redCorner; redCorner.bPassable = true; redCorner.m_bIsCorner = true; redCorner.m_gtCornerType = gtRed;
	bool bRedCorner = false;

	tile blueCorner; blueCorner.bPassable = true; blueCorner.m_bIsCorner = true; blueCorner.m_gtCornerType = gtBlue;
	bool bBlueCorner = false;

	tile orangeCorner; orangeCorner.bPassable = true; orangeCorner.m_bIsCorner = true; orangeCorner.m_gtCornerType = gtOrange;
	bool bOrangeCorner = false;

	tile pinkCorner; pinkCorner.bPassable = true; pinkCorner.m_bIsCorner = true; pinkCorner.m_gtCornerType = gtPink;
	bool bPinkCorner = false;

	/*
			//Ghost Corners
			tCorner = returnA[26][1]; tCorner.m_bIsCorner = true;
			//Red:
			tCorner.m_bIsCornerMain = true; tCorner.m_gtCornerType = gtRed;
			Tileset_DrawBox(&returnA, 26, 1, 1, 1, tCorner);
			tCorner.m_bIsCornerMain = false;
			Tileset_DrawBox(&returnA, 21, 5, 1, 1, tCorner);
			//Pink:
			tCorner.m_bIsCornerMain = true; tCorner.m_gtCornerType = gtPink;
			Tileset_DrawBox(&returnA, 1, 1, 1, 1, tCorner);
			tCorner.m_bIsCornerMain = false;
			Tileset_DrawBox(&returnA, 6, 5, 1, 1, tCorner);
			//Blue:
			tCorner.m_bIsCornerMain = true; tCorner.m_gtCornerType = gtBlue;
			Tileset_DrawBox(&returnA, 1, 29, 1, 1, tCorner);
			tCorner.m_bIsCornerMain = false;
			Tileset_DrawBox(&returnA, 9, 23, 1, 1, tCorner);
			//Orange:
			tCorner.m_bIsCornerMain = true; tCorner.m_gtCornerType = gtOrange;
			Tileset_DrawBox(&returnA, 26, 29, 1, 1, tCorner);
			tCorner.m_bIsCornerMain = false;
			Tileset_DrawBox(&returnA, 18, 23, 1, 1, tCorner);
	*/

	tileCollectables blankBit; blankBit.iCollectable = -1;
	tileCollectables standardBit; standardBit.iCollectable = 0; standardBit.iScoreIncrease = 10; standardBit.iAbility = -1; standardBit.bContributesToCount = true;
	tileCollectables specialBit; specialBit.iCollectable = 1; specialBit.iScoreIncrease = 50; specialBit.iAbility = 0; specialBit.bContributesToCount = true;
	/*
		
	*/
	for(int x = 0; x < 28; x++)
		for (int y = 0; y < 31; y++)
		{
			//Movement, Prison and Wall:
			switch (aac[x][y])
			{
			case '0'://Numbers are bits
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'a'://Fruit Spawn
			case 'z'://Random Drop spawn
			case 'm'://Pacman Spawn
			case 'n'://Fruit Spawn
			case 'f':
				Tileset_DrawBox(aat, x, y, 1, 1, floor);
				break;
			case 'p':
				Tileset_DrawBox(aat, x, y, 1, 1, prison);
				break;
			case 'e':
				Tileset_DrawBox(aat, x, y, 1, 1, prisonEnt);
				break;
			default:
			case 'w':
				Tileset_DrawBox(aat, x, y, 1, 1, wall);
				break;
			}
			
			//Special Gameplay (spawn)
			switch (aac[x][y])
			{
			case 'n'://Fruit
				m_tpFruitSpawnLocation.setPosition(x, y);
				break;
			case 'm'://Pacman
				m_tpPacmanSpawn.setPosition(x, y);//Set Pacman Spawn
				break;
			case 't':
				if (bPinkCorner)
					pinkCorner.m_bIsCornerMain = false;
				Tileset_DrawBox(aat, x - 1, y - 1, 1, 1, pinkCorner);
				bPinkCorner = true;
				break;
			case 'o':
				//cout << "O: X:" << x << " Y:" << y << endl;
				if (bOrangeCorner)
					orangeCorner.m_bIsCornerMain = false;
				Tileset_DrawBox(aat, x - 1, y - 1, 1, 1, orangeCorner);
				bOrangeCorner = true;
				break;
			case 'r':
				if (bRedCorner)
					redCorner.m_bIsCornerMain = false;
				Tileset_DrawBox(aat, x - 1, y - 1, 1, 1, redCorner);
				bRedCorner = true;
				break;
			case 'b':
				if (bBlueCorner)
					blueCorner.m_bIsCornerMain = false;
				Tileset_DrawBox(aat, x - 1, y - 1, 1, 1, blueCorner);
				bBlueCorner = true;
				break;
			}
			/*
			//Default corners
			if (!bRedCorner)
			{
				redCorner.m_bIsCornerMain = true; Tileset_DrawBox(aat, 26, 1, 1, 1, redCorner);
				redCorner.m_bIsCornerMain = false; Tileset_DrawBox(aat, 21, 5, 1, 1, redCorner);
			}
			if (!bPinkCorner)
			{
				pinkCorner.m_bIsCornerMain = true; Tileset_DrawBox(aat, 1, 1, 1, 1, pinkCorner);
				pinkCorner.m_bIsCornerMain = false; Tileset_DrawBox(aat, 6, 5, 1, 1, pinkCorner);
			}
			if (!bOrangeCorner)
			{
				orangeCorner.m_bIsCornerMain = true; Tileset_DrawBox(aat, 26, 29, 1, 1, orangeCorner);
				orangeCorner.m_bIsCornerMain = false; Tileset_DrawBox(aat, 18, 23, 1, 1, orangeCorner);
			}
			if (!bBlueCorner)
			{
				blueCorner.m_bIsCornerMain = true; Tileset_DrawBox(aat, 1, 29, 1, 1, blueCorner);
				blueCorner.m_bIsCornerMain = false; Tileset_DrawBox(aat, 9, 23, 1, 1, blueCorner);
			}
			*/

			//Collectables:
			if (m_atcCollectables != nullptr)
			{
				switch (aac[x][y])
				{
				case '0'://Numbers are bits
					LevelManagement_DrawBits(m_atcCollectables, x, y, 1, 1, standardBit);
					break;
				case '1'://Numbers are bits
					LevelManagement_DrawBits(m_atcCollectables, x, y, 1, 1, specialBit);
					break;
				}
				m_iNumberOfBits = LevelManagement_CountBits(m_atcCollectables); //Number of bits
			}
		}
}