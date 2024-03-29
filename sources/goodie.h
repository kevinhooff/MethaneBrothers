/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 * Program WebSite: http://methane.sourceforge.net/index.html              *
 *                                                                         *
 ***************************************************************************/

//------------------------------------------------------------------------------
// Methane Brothers Goodie Object (Header File)
//------------------------------------------------------------------------------

#ifndef _GoodieObj_h
#define _GoodieObj_h

#include "objlist.h"
#include "suck.h"
#include "maps.h"

//------------------------------------------------------------------------------
// Goodie group types
//------------------------------------------------------------------------------
#define GOODIE_TOYS	0
#define GOODIE_FOOD	1
#define GOODIE_FRUIT	2
#define GOODIE_COMP	3
#define GOODIE_FLOWER	4
#define GOODIE_SAUSAGE	5
#define GOODIE_NOTES	6
#define GOODIE_CHEESE	7
#define GOODIE_JEWEL	8
#define GOODIE_COIN	9
#define GOODIE_TRES	10
#define GOODIE_MAX	10	// Equal to the highest goodie type

#define MAX_TOYS	16
#define MAX_FOOD	13
#define MAX_FRUIT	10
#define MAX_COMP	20
#define MAX_FLOWERS	4
#define MAX_SAUSAGE	1
#define MAX_NOTES	10
#define MAX_CHEESE	2
#define MAX_COIN	1
#define MAX_JEWEL	5
#define MAX_TRES	5

//------------------------------------------------------------------------------
// goodie list used by goodie.cpp
//------------------------------------------------------------------------------
typedef struct _GOODIE_TYPE {
	int startframe;
	int score;
	int risescore;
	int flags;
	int	**animseq;
	int	sound;
	OBJSIZE *size;
	OBJMOVE *movement;
	int goodlength;
	} GOODIE_TYPE;
class CGoodieObj : public CLinkObject
{
public:
	CGoodieObj();
	virtual void Do();
	virtual void Draw();
	void Reset();
	void SetStruct( GOODIE_TYPE *group );
	void CheckStuckInWall();
	void MoveGoodie();
	void Explode();
	void DoNormal();
	void DoExplode();
	void DriveGoodie();
	void FlowerGoodie();
	void SausageGoodie();
	int CheckPickedUp();
	void ChkSpecial( int *cntptr, int toynumber, int max, const char *text );
public:
	int m_GFlags;
	int m_GScore;
	int m_RiseScore;
	int m_PickUpDelay;
	int m_GID;
	int m_GoodLength;
	int m_Command;
	int **m_ppAnimSeq;
	int m_Sound;
};

class CHotSpotObj : public CLinkObject
{
public:
	CHotSpotObj();
	virtual void Do();
	virtual void Draw();
};

class CTomatoObj : public CSuckable
{
public:
	CTomatoObj();
	virtual void Do();
	virtual void Draw();
	void LoadGfx();
	void Reset();
	virtual void SetExplode();
public:
	int	m_ExplodeFlag;
	int	m_OrigXPos;
	int	m_OrigYPos;
};


class CBowlingObj : public CSuckable
{
public:
	CBowlingObj();
	virtual void Do();
	virtual void Draw();
	void LoadGfx();
	void Reset();
	virtual void SetExplode();
public:
};


class CBlockObj : public CSuckable
{
public:
	CBlockObj();
	virtual void Do();
	virtual void Draw();
	void LoadGfx();
	void Reset();
	virtual void SetExplode();
	void DoTheAnim();
	void SetBoxPos();
	void ClearBoxPos();
	virtual void SetupCaught();
public:
	int m_WinkInterval;
	int m_WinkOffset;
	BOXPOS *m_pBox;
};

class CSpringObj : public CSuckable
{
public:
	CSpringObj();
	virtual void Do();
	virtual void Draw();
	void LoadGfx();
	void Reset();
	virtual void SetExplode();
	void SetBoxPos();
	void ClearBoxPos();
	void CheckBouncePlayer();
	virtual void SetupCaught();
public:
	BOXPOS *m_pBox;
};

class CSkittleObj : public CSuckable
{
public:
	CSkittleObj();
	virtual void Do();
public:
	int m_BounceCnt;
};

class CChickenEggObj : public CSuckable
{
public:
	CChickenEggObj();
	virtual void Do();
public:
	int m_BounceCnt;
};


class CCarryDoorObj : public CSuckable
{
public:
	CCarryDoorObj();
	virtual void Do();
	void LoadGfx();
public:
	int m_Pause;
	int m_LifeTime;
};

class CBigCheeseObj : public CSuckable
{
public:
	CBigCheeseObj();
	virtual void Do();
	virtual void Draw();
	void LoadGfx();
	void ExplodeCheese();
public:
	int m_MainCheeseFlag;
	int m_CheeseCounter;
};

class CChestObj : public CSuckable
{
public:
	CChestObj();
	virtual void Do();
	virtual void Draw();
	void LoadGfx();
	void ReleaseToys();
	void CheckToOpen();
	void CheckEndBonusLevel();

public:
	int m_BonusRoomFlag;
	int m_ReleaseType;	// GOODIE_xxx types
	int m_LiftPos;
	int m_ToysCnt;
	int m_LevelLength;
};



//------------------------------------------------------------------------------
// Global Data
//------------------------------------------------------------------------------
extern GOODIE_TYPE toy_list[MAX_TOYS] ;
extern GOODIE_TYPE food_list[MAX_FOOD] ;
extern GOODIE_TYPE fruit_list[MAX_FRUIT] ; 
extern GOODIE_TYPE comp_list[MAX_COMP] ; 
extern GOODIE_TYPE *goodie_group[GOODIE_MAX+1] ;
extern GOODIE_TYPE flowers_list[MAX_FLOWERS];
extern GOODIE_TYPE sausage_list[MAX_SAUSAGE];
extern int maxvals[GOODIE_MAX+1] ;


#endif // _GoodieObj_h

