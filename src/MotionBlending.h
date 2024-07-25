// MotionBlending.h: interface for the MotionBlending class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOTIONBLENDING_H__BE87A3A4_4167_4345_A670_3EEA459E117D__INCLUDED_)
#define AFX_MOTIONBLENDING_H__BE87A3A4_4167_4345_A670_3EEA459E117D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Curve Fitting\MotionData.h"
#include "Curve Fitting\MotionCurves_UCBS.h"

class MotionBlending  
{
public:
	unsigned int GetMarkerIndex(MotionData *pMD, const char *MarkerName);
	bool IsSketelonIdentical(MotionData *pMD1, MotionData *pMD2);
	bool HasIdenticalName(MotionData *md);
	bool IsIdenticalJoint(MotionMarker *mk1, MotionMarker *mk2);
	bool LinearBlendFrames(MotionData *md1, MotionData *md2,  unsigned int StartFrame, unsigned int EndFrame, float t, MotionData *pMotionData);
	MotionBlending();
	virtual ~MotionBlending();

};

#endif // !defined(AFX_MOTIONBLENDING_H__BE87A3A4_4167_4345_A670_3EEA459E117D__INCLUDED_)
