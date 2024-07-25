// KungfuMotionData.cpp: implementation of the KungfuMotionData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KungfuMotion.h"
#include "KungfuMotionData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KungfuMotionData::KungfuMotionData()
{
	pNext = NULL;
	mti = NULL;
	MC_UCBS = NULL;
	nKungfuMotionData = 0;
	MotionDataType = UNKNOWN_MOTION_TYPE;
	tItem = NULL;

	m_IsShowCP = false;
	m_IsShowIP = false;
	m_IsShowPP = true;
}

KungfuMotionData::~KungfuMotionData()
{
	if(mti != NULL)
	{
		for(unsigned int i = 0; i < MotionBuffer.numMarkers; i++)
		{
			if(mti[i].cti != NULL)
				delete[] mti[i].cti;
		}

		delete[] mti;
		mti = NULL;
	}

	if(MC_UCBS != NULL)
		delete MC_UCBS;
}

bool KungfuMotionData::Initialize(const char *Filename, unsigned int MotionType)
{
	if(MotionType == TRC_MOTION_TYPE)
	{
		if(MotionBuffer.LoadTRCFile(Filename))
		{
			if(MC_UCBS != NULL)
				delete MC_UCBS;

			MC_UCBS = new MotionCurves_UCBS;
			MC_UCBS->InitializeMotionCurve(&MotionBuffer);

			// Structures to hold curve indexs and markers relative to tree items.
			mti = new MarkerToItem[MotionBuffer.numMarkers];
			for(unsigned int i = 0; i < MotionBuffer.numMarkers; i++)
				mti[i].cti = new CurveToItem[MotionBuffer.GetMarker(i)->numDOFs];

			MotionDataType = TRC_MOTION_TYPE;
		}
		else
			return false;
	}

	if(MotionType == BVH_MOTION_TYPE)
	{
		if(MotionBuffer.LoadBVHFile(Filename))
		{
			if(MC_UCBS != NULL)
				delete MC_UCBS;

			MC_UCBS = new MotionCurves_UCBS;
			MC_UCBS->InitializeMotionCurve(&MotionBuffer);

			// Structures to hold curve indexs and markers relative to tree items.
			mti = new MarkerToItem[MotionBuffer.numMarkers];
			for(unsigned int i = 0; i < MotionBuffer.numMarkers; i++)
				mti[i].cti = new CurveToItem[MotionBuffer.GetMarker(i)->numDOFs];

			MotionDataType = BVH_MOTION_TYPE;
		}
		else
			return false;
	}

	MotionDataType = MotionType;

	return true;
}

bool KungfuMotionData::Initialize(unsigned int MotionType)
{
	if(MotionBuffer.m_MotionDataType == UNKNOWN_MOTION_TYPE)
		return false;

	if(MC_UCBS != NULL)
		delete MC_UCBS;

	MC_UCBS = new MotionCurves_UCBS;
	MC_UCBS->InitializeMotionCurve(&MotionBuffer);
	
	// Structures to hold curve indexs and markers relative to tree items.
	mti = new MarkerToItem[MotionBuffer.numMarkers];
	for(unsigned int i = 0; i < MotionBuffer.numMarkers; i++)
		mti[i].cti = new CurveToItem[MotionBuffer.GetMarker(i)->numDOFs];

	MotionDataType = MotionType;

	return true;
}

bool KungfuMotionData::SetMotionName(const char *_name)
{
	strcpy(name, _name);

	return true;
}

MotionMarker * KungfuMotionData::GetMarker(unsigned int nMarker)
{
	return MotionBuffer.GetMarker(nMarker);
}

unsigned int KungfuMotionData::GetNumMarkers()
{
	return MotionBuffer.numMarkers;
}
