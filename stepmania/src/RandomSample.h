/*
-----------------------------------------------------------------------------
 File: RandomSample.h

 Desc: Holds multiple sounds samples and can play a random sound easily.

 Copyright (c) 2001-2002 by the persons listed below.  All rights reserved.
-----------------------------------------------------------------------------
*/

#ifndef _RandomSample_H_
#define _RandomSample_H_


#include "RageSound.h"
#include "RageSoundSample.h"
#include "RageUtil.h"

class RandomSample
{
public:
	RandomSample();
	~RandomSample();

	virtual bool Load( CString sFilePath )
	{
		CString sDir, sFName, sExt;
		splitrelpath( sFilePath, sDir, sFName, sExt );

		sExt.MakeLower();

		if( sExt == "" )	return LoadSoundDir( sFilePath );
		else				return LoadSound( sFilePath );
	};

	void PlayRandom();
	void Pause();
	void Stop();

private:
	bool LoadSoundDir( CString sDir );
	bool LoadSound( CString sSoundFilePath );


	CArray<RageSoundSample*, RageSoundSample*> m_pSamples;
	int m_iIndexLastPlayed;
};




#endif