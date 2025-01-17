// MotionCurves_UCBS.cpp: implementation of the MotionCurves_UCBS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MotionCurves_UCBS.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MotionCurves_UCBS::MotionCurves_UCBS()
{
	m_pMotionBuffer = NULL;
}

MotionCurves_UCBS::MotionCurves_UCBS(MotionData *pMotionBuffer_in)
{
	m_pMotionBuffer = NULL;

	InitializeMotionCurve(pMotionBuffer_in);
}

MotionCurves_UCBS::~MotionCurves_UCBS()
{
}

bool MotionCurves_UCBS::FitAllMarkersToCurves()
{
	// Fit all markers' curves to cubic B-Spline curves.
	if(m_pMotionBuffer->m_MotionDataType != UNKNOWN_MOTION_TYPE)
	{

		if(!m_pMotionBuffer->m_IsMatricesReady)
		{
			// Build Coefficient Matrix.
			m_pMotionBuffer->BuildCoefMatrix(m_pMotionBuffer->numFrames);
			// Build inverse matrix.
			m_pMotionBuffer->m_pInvMat = m_pMotionBuffer->m_pCoefMat->Inverse();

			m_pMotionBuffer->SetMatricesPointersToCurves();
			m_pMotionBuffer->m_IsMatricesReady = true;
		}

		unsigned int j = 0;
		
		for(j = 0; j < m_pMotionBuffer->numMarkers; j++)
			m_pMotionBuffer->FitMarkerToUCBS(j);
	}
	else
		return false;

	return true;
}

float MotionCurves_UCBS::GetCurveVal(unsigned int markerIndex, unsigned int dof, float t)
{
	return m_pMotionBuffer->GetCurveVal(markerIndex, dof, t);
}

bool MotionCurves_UCBS::FitMarkerToUCBS(unsigned int markerIndex)
{
	// Fit nDOF curves of marker to cubic B-Spline curves.

	if(m_pMotionBuffer->m_MotionDataType != UNKNOWN_MOTION_TYPE)
	{

		if(!m_pMotionBuffer->m_IsMatricesReady)
		{
			// Build Coefficient Matrix.
			m_pMotionBuffer->BuildCoefMatrix(m_pMotionBuffer->numFrames);
			// Build inverse matrix.
			m_pMotionBuffer->m_pInvMat = m_pMotionBuffer->m_pCoefMat->Inverse();

			m_pMotionBuffer->SetMatricesPointersToCurves();
			m_pMotionBuffer->m_IsMatricesReady = true;
		}

		m_pMotionBuffer->FitMarkerToUCBS(markerIndex);
	}
	else
		return false;
	
	return true;
}

bool MotionCurves_UCBS::InitializeMotionCurve(MotionData *pMotionBuffer_in)
{
	if(pMotionBuffer_in->m_MotionDataType != UNKNOWN_MOTION_TYPE)
	{
		m_pMotionBuffer = pMotionBuffer_in;
		m_pMotionBuffer->InitializeAllMotionCurves();
	}

	return true;
}

PathPoint MotionCurves_UCBS::GetPathPoint(unsigned int markerIndex, unsigned int Frame, unsigned int dof)
{
	return m_pMotionBuffer->GetPathPoint(markerIndex, Frame, dof);
}

float MotionCurves_UCBS::GetMaxAmp(unsigned int markerIndex, unsigned int dof)
{
	return m_pMotionBuffer->GetMaxAmp(markerIndex, dof);
}

float MotionCurves_UCBS::GetMinAmp(unsigned int markerIndex, unsigned int dof)
{
	return m_pMotionBuffer->GetMinAmp(markerIndex, dof);
}

bool MotionCurves_UCBS::GetParentMarker(unsigned int nMarker, unsigned int *nParent)
{
	MotionMarker *pMK = m_pMotionBuffer->pMarkerHead;
	MotionMarker *pChildMK = m_pMotionBuffer->GetMarker(nMarker);

	if(pMK != NULL && pChildMK != NULL)
	{
		while(pMK != NULL)
		{
			if(pChildMK->parent == pMK)
			{
				*nParent = pMK->nPosition;
				return true;
			}

			pMK = pMK->next;
		}
	}

	return false;
}

MotionMarker * MotionCurves_UCBS::GetMarker(unsigned int nMarker)
{
	return m_pMotionBuffer->GetMarker(nMarker);
}

ControlPoint MotionCurves_UCBS::GetControlPoint(unsigned int markerIndex, unsigned int Frame, unsigned int dof)
{
	return m_pMotionBuffer->GetControlPoint(markerIndex, Frame, dof);
}

bool MotionCurves_UCBS::ReleaseMatrices()
{
	if(m_pMotionBuffer->m_MotionDataType != UNKNOWN_MOTION_TYPE)
	{

		if(m_pMotionBuffer->m_IsMatricesReady)
		{
			// Delete Coefficient Matrix.
			if(m_pMotionBuffer->m_pCoefMat != NULL)
			{
				delete m_pMotionBuffer->m_pCoefMat;
				m_pMotionBuffer->m_pCoefMat = NULL;
			}
			else
				return false;

			// Delete inverse matrix.
			if(m_pMotionBuffer->m_pInvMat != NULL)
			{
				delete m_pMotionBuffer->m_pInvMat;
				m_pMotionBuffer->m_pInvMat = NULL;
			}
			else
				return false;

			m_pMotionBuffer->m_IsMatricesReady = false;
		}
	}
	else
		return false;

	return true;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      