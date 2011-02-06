/*
===========================================================================
ARX FATALIS GPL Source Code
Copyright (C) 1999-2010 Arkane Studios SA, a ZeniMax Media company.

This file is part of the Arx Fatalis GPL Source Code ('Arx Fatalis Source Code'). 

Arx Fatalis Source Code is free software: you can redistribute it and/or modify it under the terms of the GNU General Public 
License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Arx Fatalis Source Code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Arx Fatalis Source Code.  If not, see 
<http://www.gnu.org/licenses/>.

In addition, the Arx Fatalis Source Code is also subject to certain additional terms. You should have received a copy of these 
additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Arx 
Fatalis Source Code. If not, please request a copy in writing from Arkane Studios at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing Arkane Studios, c/o 
ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.
===========================================================================
*/
///////////////////////////////////////////////////////////////////////////////
//
// ARX_Interface.h
// ARX Interface Management
//
// Copyright (c) 1999-2000 ARKANE Studios SA. All rights reserved
//
///////////////////////////////////////////////////////////////////////////////

#ifndef ARX_INTERFACE_H
#define ARX_INTERFACE_H

#include <string>
#include <map>

#include "graphics/GraphicsTypes.h"
#include "graphics/data/Mesh.h"
#include "game/Spells.h"

//-----------------------------------------------------------------------------
class INTERFACE_TC
{
public:
	INTERFACE_TC();
	~INTERFACE_TC();

	void Set(const std::string& textureName, TextureContainer* pTexture);
	void Set(const std::string& textureName, const std::string& fileName);
	TextureContainer* Get(const std::string& name);

	void Reset();

public:
	std::string        Level;
	std::string        Xp;

private:
	typedef std::map<std::string, TextureContainer*>  TextureDictionary;

	TextureDictionary  m_Textures;
	
} ;

//-----------------------------------------------------------------------------
typedef struct
{
	TextureContainer*   tc;
	std::string         name;
	std::string         description;
	long                level;
	long                spellid;
	unsigned char       symbols[6];
	bool                bSecret;
	bool                bDuration;
	bool                bAudibleAtStart;
} SPELL_ICON;


const unsigned long MAX_PAGES(256);

enum ARX_INTERFACE_NOTE_TYPE
{
	NOTE_TYPE_UNDEFINED,
	NOTE_TYPE_NOTE,
	NOTE_TYPE_BIGNOTE,
	NOTE_TYPE_NOTICE,
	NOTE_TYPE_BOOK
};

typedef struct
{
	ARX_INTERFACE_NOTE_TYPE type;
	std::string             text;
	long                    textsize;
	long                    pages[MAX_PAGES];
	long                    curpage;
	long                    totpages;
} STRUCT_NOTE;

typedef enum _ARX_STATE_MOUSE
{
	MOUSE_IN_WORLD,
	MOUSE_IN_TORCH_ICON,
	MOUSE_IN_REDIST_ICON,
	MOUSE_IN_GOLD_ICON,
	MOUSE_IN_BOOK_ICON,
	MOUSE_IN_BOOK,
	MOUSE_IN_INVENTORY_ICON,
	MOUSE_IN_INVENTORY_PICKALL_ICON,
	MOUSE_IN_INVENTORY_CLOSE_ICON,
	MOUSE_IN_STEAL_ICON,
	MOUSE_IN_INVENTORY,
	MOUSE_IN_NOTE
} E_ARX_STATE_MOUSE;

//-----------------------------------------------------------------------------

enum ARX_INTERFACE_MOVE_MODE
{
	MOVE_UNDEFINED,
	MOVE_WAIT,
	MOVE_WALK,
	MOVE_RUN
};

enum ARX_INTERFACE_BOOK_ITEM
{
	BOOK_NOTHING,
	BOOK_STRENGTH,
	BOOK_MIND,
	BOOK_DEXTERITY,
	BOOK_CONSTITUTION,
	BOOK_STEALTH,
	BOOK_MECANISM,
	BOOK_INTUITION,
	BOOK_ETHERAL_LINK,
	BOOK_OBJECT_KNOWLEDGE,
	BOOK_CASTING,
	BOOK_CLOSE_COMBAT,
	BOOK_PROJECTILE,
	BOOK_DEFENSE,
	BUTTON_QUICK_GENERATION,
	BUTTON_SKIN,
	BUTTON_DONE,
	WND_ATTRIBUTES,
	WND_SKILLS,
	WND_STATUS,
	WND_LEVEL,
	WND_XP,
	WND_HP,
	WND_MANA,
	WND_AC,
	WND_RESIST_MAGIC,
	WND_RESIST_POISON,
	WND_DAMAGE,
	WND_NEXT_LEVEL

};



// long "EDITION" values (Danae.cpp)
enum ARX_INTERFACE_EDITION_MODE
{
	EDITION_IO,
	EDITION_LIGHTS,
	EDITION_FOGS,
	EDITION_NODES,
	EDITION_ZONES,
	EDITION_PATHWAYS,
	EDITION_PARTICLES
};

enum ARX_INTERFACE_FLAG
{
	INTER_MAP        = 0x00000001,
	INTER_INVENTORY  = 0x00000002,
	INTER_INVENTORYALL = 0x00000004,
	INTER_MINIBOOK   = 0x00000008,
	INTER_MINIBACK   = 0x00000010,
	INTER_LIFE_MANA  = 0x00000020,
	INTER_COMBATMODE = 0x00000040,
	INTER_NOTE       = 0x00000080,
	INTER_STEAL       = 0x00000100,
	INTER_NO_STRIKE   = 0x00000200
};

enum ARX_INTERFACE_CURSOR_MODE
{
	CURSOR_UNDEFINED,
	CURSOR_FIREBALLAIM,
	CURSOR_INTERACTION_ON,
	CURSOR_REDIST,
	CURSOR_COMBINEON,
	CURSOR_COMBINEOFF
};

//-----------------------------------------------------------------------------
extern INTERFACE_TC ITC;
extern EERIE_S2D MemoMouse;
extern EERIE_S2D bookclick;


extern SPELL_ICON spellicons[SPELL_COUNT];
extern char LOADFROM[256];
extern float CINEMA_DECAL;
extern long CURRINVCOUNT;
extern long Book_Mode;
extern long SpecialCursor;

extern long LastMouseClick;
extern long INVERTMOUSE;
extern long CINEMASCOPE;
extern long CINEMA_INC;
extern long CurrFightPos;
extern long FORBID_SAVE;
extern long CONSTANTUPDATELIGHT;

//-----------------------------------------------------------------------------
float INTERFACE_RATIO(const float);
float INTERFACE_RATIO_LONG(const long);	
float INTERFACE_RATIO_DWORD(const DWORD);
short SHORT_INTERFACE_RATIO(const float);

bool MouseInCam(EERIE_CAMERA * cam);
bool MouseInRect(float x0, float y0, float x1, float y1);
long GetMainSpeakingIO();
void DrawBookInterfaceItem(LPDIRECT3DDEVICE7 m_pd3dDevice, TextureContainer * tc, float x, float y, float z = 0.000001f);
bool ARX_INTERFACE_MouseInBook();
void ARX_INTERFACE_PlayerInterfaceModify(long showhide, long smooth);
void ARX_INTERFACE_Reset();
void ARX_INTERFACE_SetCinemascope(long v, long vv);
void ARX_INTERFACE_RenderCursor(long flag = 0);
void ARX_INTERFACE_ManageOpenedBook();
void ARX_INTERFACE_ManageOpenedBook_Finish();
void ARX_INTERFACE_NoteManage();
void ARX_INTERFACE_BookOpenClose(unsigned long t);
void ARX_INTERFACE_NoteOpen(ARX_INTERFACE_NOTE_TYPE type, const std::string& tex);
void ARX_INTERFACE_NoteClose();
void ARX_INTERFACE_NoteClear();
void ARX_INTERFACE_NoteInit();
void ARX_INTERFACE_ShowFISHTANK(LPDIRECT3DDEVICE7 pd3dDevice);
void ARX_INTERFACE_ShowARKANE(LPDIRECT3DDEVICE7 pd3dDevice);
void ARX_INTERFACE_KillFISHTANK();
void ARX_INTERFACE_KillARKANE();
void ARX_INTERFACE_EndIntro();
void ARX_INTERFACE_HALO_Flush(LPDIRECT3DDEVICE7 m_pd3dDevice);
void ARX_INTERFACE_DrawItem(TextureContainer * tc, const float x, const float y, const float z = 0.001f, const D3DCOLOR col = D3DRGB(1, 1, 1));
void LoadScreen(LPDIRECT3DDEVICE7 pd3dDevice);
void LoadLevelScreen(LPDIRECT3DDEVICE7 pd3dDevice = NULL, long lev = -1, float v = 0.f);
void ReleaseHalo();
void ResetPlayerInterface();
void Set_DragInter(INTERACTIVE_OBJ * io);
void CreateInterfaceTextureContainers();
void KillInterfaceTextureContainers();

#define ARX_MOUSE_OVER_BOOK			1
#define ARX_MOUSE_OVER_INVENTORY	2
#define ARX_MOUSE_OVER_INVENTORY_2	4
#define ARX_MOUSE_OVER_IO			8

extern long ARX_MOUSE_OVER;
#endif