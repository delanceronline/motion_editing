// MotionMarker.cpp: implementation of the MotionMarker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MotionMarker.h"
#include "string.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MotionMarker::MotionMarker()
{
	pps = NULL;
	cps = NULL;
	CurveName = NULL;
	prev = NULL;
	next = NULL;
	parent = NULL;
	m_cbsc = NULL;

	numCPs = 0;
	numDOFs = 0;

	MarkerType = UNKNOWN_MARKER;
	IsEndSite = false;

	TranMat.ResetDimension(4, 4);
}

MotionMarker::~MotionMarker()
{
	if(pps != NULL)
	{
		delete[] pps;
		pps = NULL;
	}

	if(cps != NULL)
	{
		delete[] cps;
		cps = NULL;
	}

	if(CurveName != NULL)
	{
		delete[] CurveName;
		CurveName = NULL;
	}

	if(m_cbsc != NULL)
	{
		delete[] m_cbsc;
		m_cbsc = NULL;
	}
}

bool MotionMarker::Initialize(unsigned int numDOFs_in, unsigned int numFrames_in, char *name_in, unsigned int type)
{
	numDOFs = numDOFs_in;
	numFrames = numFrames_in;
	MarkerType = type;
	pps = new PathPointSet[numDOFs];
	CurveName = new DOFName[numDOFs];
	m_cbsc = new CubicBSplineCurve[numDOFs];

	for(unsigned int i = 0; i < numDOFs; i++)
		pps[i].Reset(numFrames);

	strcpy(name, name_in);

	return true;
}

bool MotionMarker::SetDOFName(unsigned int nDOF, char *name_in)
{
	strcpy(CurveName[nDOF].name, name_in);

	return true;
}

MotionMarker& MotionMarker::operator =(const MotionMarker &MK_in)
{
	if(this == &MK_in)
		return *this;
	
	if(pps != NULL)
		delete[] pps;
	if(cps != NULL)
		delete[] cps;
	if(CurveName != NULL)
		delete[] CurveName;
	if(m_cbsc != NULL)
		delete[] m_cbsc;

	unsigned int i;

	
	numDOFs = MK_in.numDOFs;
	numFrames = MK_in.numFrames;
	MarkerType = MK_in.MarkerType;
	
	pps = new PathPointSet[numDOFs];
	CurveName = new DOFName[numDOFs];
	m_cbsc = new CubicBSplineCurve[numDOFs];
	strcpy(name, MK_in.name);


	for(i = 0; i < numDOFs; i++)
	{
		pps[i].Reset(numFrames);
		pps[i] = MK_in.pps[i];
	}

	if(MK_in.numCPs != 0)
	{
		cps = new ControlPointSet[MK_in.numCPs];
		for(i = 0; i < numDOFs; i++)
		{
			cps[i].Reset(MK_in.numCPs);
			cps[i] = MK_in.cps[i];
		}
	}


	offset = MK_in.offset;
	nPosition = MK_in.nPosition;
	parent = MK_in.parent;
	prev = MK_in.prev;
	next = MK_in.next;

	TranMat = MK_in.TranMat;

	return *this;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      