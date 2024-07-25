// ubicBSplines.cpp: implementation of the CubicBSplines class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CubicBSplineCurve.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CubicBSplineCurve::CubicBSplineCurve()
{
	InitConstantMatrix();

	coef_mat = NULL;
	Inverse_mat = NULL;
	pps = NULL;
	cps = NULL;

	IsExternalMatrix = false;
}

CubicBSplineCurve::CubicBSplineCurve(PathPointSet *pps_in)
{
	InitConstantMatrix();

	cps = NULL;
	coef_mat = NULL;
	Inverse_mat = NULL;

	pps = new PathPointSet();
	*pps = *pps_in;

	BuildCoefMatrix(pps_in->numPoints);

	IsExternalMatrix = false;
}

CubicBSplineCurve::CubicBSplineCurve(ControlPointSet *cps_in)
{
	InitConstantMatrix();

	pps = NULL;
	coef_mat = NULL;
	Inverse_mat = NULL;

	cps = new ControlPointSet();
	*cps = *cps_in;

	BuildCoefMatrix(cps_in->numPoints);

	IsExternalMatrix = false;
}

CubicBSplineCurve::~CubicBSplineCurve()
{
	//Clean out all resources allocated.

	if(!IsExternalMatrix && Inverse_mat != NULL)
	{
		delete Inverse_mat;
		Inverse_mat = NULL;
	}

	if(!IsExternalMatrix && coef_mat != NULL)
	{
		delete coef_mat;
		coef_mat = NULL;
	}
}

bool CubicBSplineCurve::BuildCoefMatrix(unsigned int N)
{
	//Setup coefficient matrix with N x N dimension.
	if(N >= 4)
	{
		(*coef_mat).ResetDimension(N, N);

		(*coef_mat).SetVal(0, 0, 9.0f); (*coef_mat).SetVal(0, 1, -3.0f);
		(*coef_mat).SetVal(1, 0, 0.25f); (*coef_mat).SetVal(1, 1, 7.0f / 12.0f); (*coef_mat).SetVal(1, 2, 1.0f / 6.0f);

		for(unsigned int i = 2; i < N - 2; i++)
		{
			(*coef_mat).SetVal(i, i - 1, 1.0f / 6.0f);
			(*coef_mat).SetVal(i, i, 2.0f / 3.0f);
			(*coef_mat).SetVal(i, i + 1, 1.0f / 6.0f);
		}

		(*coef_mat).SetVal(N - 2, N - 3, 1.0f / 6.0f); (*coef_mat).SetVal(N - 2, N - 2, 7.0f / 12.0f); (*coef_mat).SetVal(N - 2, N - 1, 0.25f);
		(*coef_mat).SetVal(N - 1, N - 2, -3.0f); (*coef_mat).SetVal(N - 1, N - 1, 9.0f); 

		return true;
	}
	else
		return false;
}

void CubicBSplineCurve::GetControlPoint(unsigned int cp_index, ControlPoint *cp)
{
	if(cps != NULL)
		*cp = cps->cp[cp_index];
	else
		cp = NULL;
}

void CubicBSplineCurve::InitConstantMatrix()
{
	m1.ResetDimension(4, 4);
	m2.ResetDimension(4, 4);
	m3.ResetDimension(4, 4);
	m4.ResetDimension(4, 4);
	m5.ResetDimension(4, 4);

	//Set values for these five constant 4x4 matirces.
	m1.SetVal(0, 0, -1.0f); m1.SetVal(0, 1, 7.0f / 4.0f); m1.SetVal(0, 2, -11.0f / 12.0f); m1.SetVal(0, 3, 1 / 6.0f);
	m1.SetVal(1, 0, 3.0f); m1.SetVal(1, 1, -4.5f); m1.SetVal(1, 2, 1.5f); m1.SetVal(1, 3, 0.0f);
	m1.SetVal(2, 0, -3.0f); m1.SetVal(2, 1, 3.0f); m1.SetVal(2, 2, 0.0f); m1.SetVal(2, 3, 0.0f);
	m1.SetVal(3, 0, 1.0f); m1.SetVal(3, 1, 0.0f); m1.SetVal(3, 2, 0.0f); m1.SetVal(3, 3, 0.0f);

	m2.SetVal(0, 0, -0.25f); m2.SetVal(0, 1, 7.0f / 12.0f); m2.SetVal(0, 2, -0.5f); m2.SetVal(0, 3, 1 / 6.0f);
	m2.SetVal(1, 0, 3.0f / 4.0f); m2.SetVal(1, 1, -5.0f / 4.0f); m2.SetVal(1, 2, 0.5f); m2.SetVal(1, 3, 0.0f);
	m2.SetVal(2, 0, -3.0f / 4.0f); m2.SetVal(2, 1, 0.25f); m2.SetVal(2, 2, 0.5f); m2.SetVal(2, 3, 0.0f);
	m2.SetVal(3, 0, 0.25f); m2.SetVal(3, 1, 7.0f / 12.0f); m2.SetVal(3, 2, 1.0f / 6.0f); m2.SetVal(3, 3, 0.0f);

	m3.SetVal(0, 0, -1.0f); m3.SetVal(0, 1, 3.0f); m3.SetVal(0, 2, -3.0f); m3.SetVal(0, 3, 1.0f);
	m3.SetVal(1, 0, 3.0f); m3.SetVal(1, 1, -6.0f); m3.SetVal(1, 2, 3.0f); m3.SetVal(1, 3, 0.0f);
	m3.SetVal(2, 0, -3.0f); m3.SetVal(2, 1, 0.0f); m3.SetVal(2, 2, 3.0f); m3.SetVal(2, 3, 0.0f);
	m3.SetVal(3, 0, 1.0f); m3.SetVal(3, 1, 4.0f); m3.SetVal(3, 2, 1.0f); m3.SetVal(3, 3, 0.0f);

	m4.SetVal(0, 0, -1.0f / 6.0f); m4.SetVal(0, 1, 0.5f); m4.SetVal(0, 2, -7.0f / 12.0f); m4.SetVal(0, 3, 0.25f);
	m4.SetVal(1, 0, 0.5f); m4.SetVal(1, 1, -1.0f); m4.SetVal(1, 2, 0.5f); m4.SetVal(1, 3, 0.0f);
	m4.SetVal(2, 0, -0.5f); m4.SetVal(2, 1, 0.0f); m4.SetVal(2, 2, 0.5f); m4.SetVal(2, 3, 0.0f);
	m4.SetVal(3, 0, 1.0f / 6.0f); m4.SetVal(3, 1, 2.0f / 3.0f); m4.SetVal(3, 2, 1.0f / 6.0f); m4.SetVal(3, 3, 0.0f);

	m5.SetVal(0, 0, -1.0f / 6.0f); m5.SetVal(0, 1, 11.0f / 12.0f); m5.SetVal(0, 2, -7.0f / 4.0f); m5.SetVal(0, 3, 1.0f);
	m5.SetVal(1, 0, 0.5f); m5.SetVal(1, 1, -5.0f / 4.0f); m5.SetVal(1, 2, 3.0f / 4.0f); m5.SetVal(1, 3, 0.0f);
	m5.SetVal(2, 0, -0.5f); m5.SetVal(2, 1, -0.25f); m5.SetVal(2, 2, 3.0f / 4.0f); m5.SetVal(2, 3, 0.0f);
	m5.SetVal(3, 0, 1.0f / 6.0f); m5.SetVal(3, 1, 7.0f / 12.0f); m5.SetVal(3, 2, 0.25f); m5.SetVal(3, 3, 0.0f);
}

void CubicBSplineCurve::GetPathPoint(unsigned int pp_index, PathPoint *pp)
{
	if(pps != NULL)
		*pp = pps->pp[pp_index];
	else
		pp = NULL;
}

void CubicBSplineCurve::CalculateControlPointsFromPathPoints(ControlPointSet *pCPS)
{
	unsigned int i;

	//Two column matrices for amplitude and time of path point set.
	Matrix ppVal_mat(pps->numPoints, 1), ppTime_mat(pps->numPoints, 1);
	//Two column matrices for amplitude and time of control point set.
	Matrix cpVal_mat(pps->numPoints, 1), cpTime_mat(pps->numPoints, 1);

	//Set column matrices of path point set.
	ppVal_mat.SetVal(0, 0, 6.0f * pps->pp[0].GetAmpVal());
	ppTime_mat.SetVal(0, 0, 6.0f * pps->pp[0].GetTimeVal());
	for(i = 1; i < pps->numPoints - 1; i++)
	{
		ppVal_mat.SetVal(i, 0, pps->pp[i].GetAmpVal());
		ppTime_mat.SetVal(i, 0, pps->pp[i].GetTimeVal());
	}
	ppVal_mat.SetVal(pps->numPoints - 1, 0, 6.0f * pps->pp[pps->numPoints - 1].GetAmpVal());
	ppTime_mat.SetVal(pps->numPoints - 1, 0, 6.0f * pps->pp[pps->numPoints - 1].GetTimeVal());

	//Find inverse of coefficient matrix.
	//Matrix *Inverse_mat;
	if(Inverse_mat == NULL)
		Inverse_mat = (*coef_mat).Inverse();

	//Calculate control points using inverse.
	(*Inverse_mat).Product(&ppVal_mat, &cpVal_mat);
	(*Inverse_mat).Product(&ppTime_mat, &cpTime_mat);

	//Put amplitude and time to control point set from two column matrices of control point set.
	//pMotionBuffer->marker->cps = new ControlPointSet(pps->numPoints + 2);
	cps = pCPS;
	//cps = new ControlPointSet(pps->numPoints + 2);
	cps->cp[0].SetAmpVal(pps->pp[0].GetAmpVal());
	cps->cp[0].SetTimeVal(pps->pp[0].GetTimeVal());
	for(i = 1; i < cps->numPoints - 1; i++)
	{
		cps->cp[i].SetAmpVal(cpVal_mat.GetVal(i - 1, 0));
		cps->cp[i].SetTimeVal(cpTime_mat.GetVal(i - 1, 0));
	}
	cps->cp[cps->numPoints - 1].SetAmpVal(pps->pp[pps->numPoints - 1].GetAmpVal());
	cps->cp[cps->numPoints - 1].SetTimeVal(pps->pp[pps->numPoints - 1].GetTimeVal());
}

float CubicBSplineCurve::GetCurveVal(float time)
{
	bool IsLocated = false, IsError = false;
	unsigned int index = 0;

	while(!IsLocated && !IsError)
	{
		if(time >= pps->pp[index].GetTimeVal() && time <= pps->pp[index + 1].GetTimeVal())
			IsLocated = true;
		
		if(index == pps->numPoints - 1)
			IsError = true;
		
		index++;
	}


	if(!IsError)
	{
		float u;

		index--;
		//Calculate u for this segment of B-Spline using time.
		u = (time - pps->pp[index].GetTimeVal()) / (pps->pp[index + 1].GetTimeVal() - pps->pp[index].GetTimeVal());

		//Row poly matrix of u.
		Matrix poly_mat(1, 4);
		poly_mat.SetVal(0, 0, u * u * u);
		poly_mat.SetVal(0, 1, u * u);
		poly_mat.SetVal(0, 2, u);
		poly_mat.SetVal(0, 3, 1.0f);

		//Column matrices for interpolation.
		Matrix Amp_mat, cpAmp_mat(4, 1);

		cpAmp_mat.SetVal(0, 0, cps->cp[index].GetAmpVal());
		cpAmp_mat.SetVal(1, 0, cps->cp[index + 1].GetAmpVal());
		cpAmp_mat.SetVal(2, 0, cps->cp[index + 2].GetAmpVal());
		cpAmp_mat.SetVal(3, 0, cps->cp[index + 3].GetAmpVal());

		//Multiply different matrix according to different segments.
		if(index > 1 && index < pps->numPoints - 3)
		{
			m3.Product(&cpAmp_mat, &Amp_mat);
			Amp_mat.ScalarProduct(1.0f / 6.0f);
		}
		else
		{
			if(index == 0)
			{
				m1.Product(&cpAmp_mat, &Amp_mat);
			}

			if(index == 1)
			{
				m2.Product(&cpAmp_mat, &Amp_mat);
			}

			if(index == pps->numPoints - 3)
			{
				m4.Product(&cpAmp_mat, &Amp_mat);
			}

			if(index == pps->numPoints - 2)
			{
				m5.Product(&cpAmp_mat, &Amp_mat);
			}
		}

		//Finally multiply polynomal row matrix to Amp_mat.
		Matrix finalAmp_mat, finalTime_mat;
		poly_mat.Product(&Amp_mat, &finalAmp_mat);

		//Return interpolated value.
		return finalAmp_mat.GetVal(0, 0);
	}
	else
		return 0.0f;
}

void CubicBSplineCurve::SetPathPoint(unsigned int pp_index, PathPoint *pp)
{

}

void CubicBSplineCurve::SetControlPoint(unsigned int cp_index, ControlPoint *cp)
{

}

bool CubicBSplineCurve::InsertPathPoint(unsigned int pp_index1, unsigned int pp_index2, PathPoint pp)
{

	return true;
}

bool CubicBSplineCurve::InsertControlPoint(unsigned int cp_index1, unsigned int cp_index2, ControlPoint cp)
{

	return true;
}

bool CubicBSplineCurve::InitializeFromPathPointSet(PathPointSet *pps_in)
{
	pps = pps_in;

	if(!BuildCoefMatrix(pps_in->numPoints))
		return false;

	return true;
}

bool CubicBSplineCurve::FindMaxAndMinPPAmp()
{
	unsigned int i;
	float prev;

	if(pps == NULL)
		return false;

	// Max.
	for(i = 0; i < pps->numPoints; i++)
	{
		if(i == 0)
		{
			prev = pps->pp[i].GetAmpVal();
			pps->maxAmp = pps->pp[i].GetAmpVal();
		}
		else
		{
			if(pps->pp[i].GetAmpVal() > pps->maxAmp)
				pps->maxAmp = pps->pp[i].GetAmpVal();

			prev = pps->pp[i].GetAmpVal();
		}
	}

	// Min.
	for(i = 0; i < pps->numPoints; i++)
	{
		if(i == 0)
		{
			prev = pps->pp[i].GetAmpVal();
			pps->minAmp = pps->pp[i].GetAmpVal();
		}
		else
		{
			if(pps->pp[i].GetAmpVal() < pps->minAmp)
				pps->minAmp = pps->pp[i].GetAmpVal();

			prev = pps->pp[i].GetAmpVal();
		}
	}

	return true;
}

float CubicBSplineCurve::GetMaxAmp()
{
	return pps->maxAmp;
}

float CubicBSplineCurve::GetMinAmp()
{
	return pps->minAmp;
}

// Initialize this CBSC by given coef matrix and its inverse.
bool CubicBSplineCurve::InitializeFromPathPointSet(PathPointSet *pps_in, Matrix *pMat, Matrix *pInvMat)
{
	IsExternalMatrix = true;

	pps = pps_in;

	if(coef_mat != NULL)
		delete coef_mat;

	if(Inverse_mat != NULL)
		delete Inverse_mat;

	coef_mat = pMat;
	Inverse_mat = pInvMat;

	return true;
}

bool CubicBSplineCurve::SetExternalMatricesPointer(Matrix *pMat, Matrix *pInvMat)
{
	if(pMat == NULL || pInvMat == NULL)
		return false;

	coef_mat = pMat;
	Inverse_mat = pInvMat;	

	return true;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      