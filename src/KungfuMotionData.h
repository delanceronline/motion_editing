// KungfuMotionData.h: interface for the KungfuMotionData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KUNGFUMOTIONDATA_H__FE453E0E_7D1D_4AEF_B841_BB863E82D1C0__INCLUDED_)
#define AFX_KUNGFUMOTIONDATA_H__FE453E0E_7D1D_4AEF_B841_BB863E82D1C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Curve Fitting\MotionData.h"
#include "Curve Fitting\MotionCurves_UCBS.h"

class KungfuMotionData  
{
public:
	unsigned int GetNumMarkers(void);
	MotionMarker * GetMarker(unsigned int nMarker);
	bool SetMotionName(const char *_name);
	bool Initialize(unsigned int MotionType);
	bool Initialize(const char *Filename, unsigned int MotionType);
	KungfuMotionData();
	virtual ~KungfuMotionData();

	HTREEITEM tItem;
	MotionData MotionBuffer;
	MotionCurves_UCBS *MC_UCBS;
	KungfuMotionData *pNext;
	unsigned int nKungfuMotionData;
	unsigned int MotionDataType;
	char name[256];
	bool m_IsShowCP, m_IsShowIP, m_IsShowPP;

	MarkerToItem *mti;
};

#endif // !defined(AFX_KUNGFUMOTIONDATA_H__FE453E0E_7D1D_4AEF_B841_BB863E82D1C0__INCLUDED_)
