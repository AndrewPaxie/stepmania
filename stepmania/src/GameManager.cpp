#include "stdafx.h"
/*
-----------------------------------------------------------------------------
 Class: GameManager

 Desc: See Header.

 Copyright (c) 2001-2002 by the persons listed below.  All rights reserved.
	Chris Danford
-----------------------------------------------------------------------------
*/

#include "GameManager.h"
#include "ErrorCatcher/ErrorCatcher.h"


GameManager*	GAME = NULL;	// global and accessable from anywhere in our program

#define DANCE_PAD_BUTTON_LEFT		INSTRUMENT_BUTTON_1
#define DANCE_PAD_BUTTON_RIGHT		INSTRUMENT_BUTTON_2
#define DANCE_PAD_BUTTON_UP			INSTRUMENT_BUTTON_3
#define DANCE_PAD_BUTTON_DOWN		INSTRUMENT_BUTTON_4
#define DANCE_PAD_BUTTON_UPLEFT		INSTRUMENT_BUTTON_5
#define DANCE_PAD_BUTTON_UPRIGHT	INSTRUMENT_BUTTON_6
#define DANCE_PAD_BUTTON_NEXT		INSTRUMENT_BUTTON_7
#define DANCE_PAD_BUTTON_BACK		INSTRUMENT_BUTTON_8
#define NUM_DANCE_PAD_BUTTONS		8



GameManager::GameManager()
{
	m_iNumGameDefs = 0;
	ReadGamesAndStylesFromDir( "Games" );
	SwitchGame( "dance" );
	SwitchStyle( "single" );
}

GameManager::~GameManager()
{
	for( int i=0; i<m_iNumGameDefs; i++ )
		delete m_pGameDefs[i];
}

void GameManager::ReadGamesAndStylesFromDir( CString sDir )
{
	// trim off the trailing slash if any
	sDir.TrimRight( "/\\" );

	// Find all group directories in "Games" folder
	CStringArray arrayGameNames;
	GetDirListing( sDir+"\\*.*", arrayGameNames, true );
	SortCStringArray( arrayGameNames );
	
	for( int i=0; i< arrayGameNames.GetSize(); i++ )	// for each dir in /Songs/
	{
		CString sGameName = arrayGameNames[i];

		if( 0 == stricmp( sGameName, "cvs" ) )	// the directory called "CVS"
			continue;		// ignore it

		CString sGameDir = ssprintf( "%s\\%s", sDir, sGameName );  // game file must have same name as dir
		m_pGameDefs[m_iNumGameDefs++] = new GameDef(sGameDir);
	}
}

GameDef* GameManager::GetGameDef( CString sGame )
{
	for( int i=0; i<m_iNumGameDefs; i++ )
	{
		if( m_pGameDefs[i]->m_sName == sGame )
			return m_pGameDefs[i];
	}

	return NULL;
}

StyleDef* GameManager::GetStyleDef( CString sGame, CString sStyle )
{
	GameDef* pGameDef = GetGameDef( sGame );

	return pGameDef->GetStyleDef( sStyle );

	return NULL;
}


void GameManager::GetGameNames( CStringArray &arrayGameNames )
{
	for( int i=0; i<MAX_GAME_DEFS; i++ )
		arrayGameNames.Add( m_pGameDefs[i]->m_sName );
}


void GameManager::GetStyleNames( CString sGameName, CStringArray &arrayStyleNames )
{
	GameDef* pGameDef = GetGameDef( sGameName );

	for( int i=0; i<pGameDef->m_iNumStyleDefs; i++ )
	{
		arrayStyleNames.Add( pGameDef->m_pStyleDefs[i]->m_sName );
	}
}


void GameManager::GetSkinNames( CString sGameName, CStringArray &arraySkinNames )
{
	GameDef* pGameDef = GetGameDef( sGameName );

	for( int i=0; i<pGameDef->m_iNumSkinFolders; i++ )
		arraySkinNames.Add( pGameDef->m_sSkinFolders[i] );
}

void GameManager::SwitchSkin( PlayerNumber p, CString sSkin )
{
	if( !m_pCurrentGameDef->HasASkinNamed( sSkin ) )
		FatalError( "The current game doesn't have a skin named '%s'.", sSkin );
		
	m_sCurrentSkin[p] = sSkin;
}


bool GameManager::IsPlayerEnabled( PlayerNumber PlayerNo )
{
	StyleDef* pStyleDef = GetCurrentStyleDef();
	ASSERT( pStyleDef != NULL );

	switch( pStyleDef->m_iNumPlayers )
	{
	case 1:
		switch( PlayerNo )
		{
		case PLAYER_1:	return true;
		case PLAYER_2:	return false;
		default:	ASSERT( false );
		}
		break;
	case 2:
		switch( PlayerNo )
		{
		case PLAYER_1:	return true;
		case PLAYER_2:	return true;
		default:	ASSERT( false );
		}
		break;
	default:
		ASSERT( false );	// invalid m_iNumPlayers
	}

	return false;
}

