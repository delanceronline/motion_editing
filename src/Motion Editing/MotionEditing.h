// MotionEditing.h: interface for the MotionEditing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOTIONEDITING_H__24F6F7FD_7E6F_4656_93EF_6708FA299811__INCLUDED_)
#define AFX_MOTIONEDITING_H__24F6F7FD_7E6F_4656_93EF_6708FA299811__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Curve Fitting\MotionData.h"
#include "..\Curve Fitting\MotionCurves_UCBS.h"

struct MotionEditingInfo
{
	float StartTime_M1, StartTime_M2, EndTime_M1, EndTime_M2;
	unsigned int StartFrame_M1, StartFrame_M2, EndFrame_M1, EndFrame_M2;
	unsigned int numFrames, numMarkers, MotionDataType;
	float FrameTime;
	float *pSimVal;
	float TransitingTime;
	float Duration;
	MotionData *pMotionData1, *pMotionData2;
	char MotionName[256];
};

class MotionEditing  
{
public:
	bool FlipFilter(PathPointSet *pps, float tol, bool IsIgnoreSign);
	bool WarpMotionByCurve(MotionData *pMB, MotionEditingInfo *pMEInfo, MotionData *pMotionData);
	bool ExtractMotionByCurve(MotionData *pMB, MotionEditingInfo *pMEInfo, MotionData *pFinalMotionData);
	bool LinkToParent(MotionData *pMD1, MotionData *pMD2);
	bool TransiteMotionsByCurve(MotionData *pMB1, MotionData *pMB2, MotionEditingInfo *pMEInfo, MotionData *pFinalMotionData);
	bool LinearExaggeratingFramesByCurve(MotionData *pMB1, MotionData *pMB2, MotionEditingInfo *pMBInfo, MotionData *pMotionData);
	bool LinearBlendFramesByCurve(MotionData *pMB1, MotionData *pMB2, MotionEditingInfo *pMBInfo, MotionData *pMotionData);
	bool LinearBlendFrames(MotionData *pMB1, MotionData *pMB2, MotionEditingInfo *pMBInfo, MotionData *pMotionData);
	MotionEditing();
	unsigned int GetMarkerIndex(MotionData *pMD, const char *MarkerName);
	bool IsSketelonIdentical(MotionData *pMD1, MotionData *pMD2);
	bool HasIdenticalName(MotionData *md);
	bool IsIdenticalJoint(MotionMarker *mk1, MotionMarker *mk2);
	bool LinearBlendFrames(MotionData *md1, MotionData *md2,  unsigned int StartFrame, unsigned int EndFrame, float t, MotionData *pMotionData);
	virtual ~MotionEditing();

};

#endif // !defined(AFX_MOTIONEDITING_H__24F6F7FD_7E6F_4656_93EF_6708FA299811__INCLUDED_)
