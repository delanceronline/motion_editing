// MotionEditing.cpp: implementation of the MotionEditing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MotionEditing.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MotionEditing::MotionEditing()
{

}

MotionEditing::~MotionEditing()
{

}

bool MotionEditing::LinearBlendFrames(MotionData *md1, MotionData *md2,  unsigned int StartFrame, unsigned int EndFrame, float t, MotionData *pMotionData)
{
	unsigned int i, j, k, numFrames = EndFrame - StartFrame + 1;
	float t_inv = 1.0f - t;
	float var;
	MotionMarker *mk1, *mk2, *mk3;
	bool IsSimJoint = false;
	PathPoint pp1, pp2;

	if(md1 != NULL && md2 != NULL && md1->numMarkers == md2->numMarkers && pMotionData != NULL)
	{
		MotionMarker *pMK = NULL, *pPrevMK = NULL;

		pMotionData->numFrames = numFrames;
		pMotionData->numMarkers = md1->numMarkers;
		pMotionData->FrameTime = md1->FrameTime;
		pMotionData->m_MotionDataType = md1->m_MotionDataType;

		if(IsSketelonIdentical(md1, md2))
		{
			// Sketelons are identical, so we blend all joints now.
			for(i = 0; i < md1->numMarkers; i++)
			{
				mk1 = md1->GetMarker(i);

				pMK = new MotionMarker;
				if(i == 0)
					pMotionData->pMarkerHead = pMK;
				else
					pPrevMK->next = pMK;

				pMK->nPosition = mk1->nPosition;

				if(!mk1->IsEndSite)
				{
					pMK->Initialize(mk1->numDOFs, numFrames, mk1->name, BVH_MARKER);
					pMK->IsEndSite = mk1->IsEndSite;
					pMK->offset = mk1->offset;
					for(k = 0; k < mk1->numDOFs; k++)
					{
						pMK->SetDOFName(k, mk1->CurveName[k].name);
						for(j = 0; j < numFrames; j++)
						{
							pp1 = md1->GetPathPoint(i, StartFrame + j, k);
							pp2 = md2->GetPathPoint(GetMarkerIndex(md2, mk1->name), StartFrame + j, k);

							// Blend two motion curves linearly.
							var = pp1.GetAmpVal() * t + pp2.GetAmpVal() * t_inv;
							pMK->pps[k].pp[j].SetAmpVal(var);
							pMK->pps[k].pp[j].SetTimeVal(pp1.GetTimeVal());
						}
					}
				}
				else
				{
					// For End Site, just don't blend anything.
					strcpy(pMK->name, mk1->name);
					pMK->IsEndSite = mk1->IsEndSite;
					pMK->offset = mk1->offset;
				}

				pPrevMK = pMK;
			}

			// Set parent pointers of joints in resulting motion buffer after blending.
			char parent_name[256];

			mk1 = md1->pMarkerHead;
			mk2 = pMotionData->pMarkerHead;
			for(i = 0; i < md1->numMarkers; i++)
			{
				if(mk1->parent != NULL)
				{
					strcpy(parent_name, mk1->parent->name);

					mk3 = pMotionData->pMarkerHead;
					while(strcmp(parent_name, mk3->name) != 0)
						mk3 = mk3->next;
					mk2->parent = mk3;

				}
				mk1 = mk1->next;
				mk2 = mk2->next;
			}
			
		}// End linear motion blending.
		else
			return false;
	}// End if pointers are null.
	else
		return false;

	return true;
}

bool MotionEditing::IsIdenticalJoint(MotionMarker *mk1, MotionMarker *mk2)
{
	char name1[256], name2[256], parent_name1[256], parent_name2[256];

	if(mk1->numDOFs == mk2->numDOFs)
	{
		if(mk1 != NULL && mk2 != NULL)
		{
			strcpy(name1, mk1->name);
			strcpy(name2, mk2->name);
			if(mk1->parent == NULL)
			{
				if(strcmp(name1, name2) == 0 && mk2->parent == NULL)
					return true;
			}
			else
			{
				if(mk2->parent != NULL)
				{
					strcpy(parent_name1, mk1->parent->name);
					strcpy(parent_name2, mk2->parent->name);
					if(strcmp(name1, name2) == 0 && strcmp(parent_name1, parent_name2) == 0)
						return true;
				}
			}
		}
	}

	return false;
}

bool MotionEditing::HasIdenticalName(MotionData *pMD)
{
	MotionMarker *pMK1, *pMK2;

	for(pMK1 = pMD->pMarkerHead; pMK1 != NULL; pMK1 = pMK1->next)
	{
		for(pMK2 = pMD->pMarkerHead; pMK2 != NULL; pMK2 = pMK2->next)
		{
			if(pMK1 != pMK2)
			{
				if(strcmp(pMK1->name, pMK2->name) == 0 && !pMK1->IsEndSite && !pMK2->IsEndSite)
					return true;
			}
		}
	}

	return false;
}

bool MotionEditing::IsSketelonIdentical(MotionData *pMD1, MotionData *pMD2)
{
	MotionMarker *pMK1, *pMK2;

	if(pMD1 != NULL && pMD2 != NULL)
	{
		// Check whether marker names are unique for each sketelon.
		if(HasIdenticalName(pMD1) || HasIdenticalName(pMD2))
			return false;

		if(pMD1->numMarkers != pMD2->numMarkers)
			return false;

		for(pMK1 = pMD1->pMarkerHead; pMK1 != NULL; pMK1 = pMK1->next)
		{
			for(pMK2 = pMD2->pMarkerHead; pMK2 != NULL; pMK2 = pMK2->next)
			{
				if(IsIdenticalJoint(pMK1, pMK2))
					break;
			}
			
			if(pMK2 == NULL)
				return false;
		}
	}
	else
		return false;

	return true;
}

unsigned int MotionEditing::GetMarkerIndex(MotionData *pMD, const char *MarkerName)
{
	if(pMD->pMarkerHead != NULL)
	{
		MotionMarker *pMK;

		for(pMK = pMD->pMarkerHead; pMK != NULL; pMK = pMK->next)
		{
			if(strcmp(pMK->name, MarkerName) == 0)
				return pMK->nPosition;
		}
	}

	return 0;
}

bool MotionEditing::LinearBlendFrames(MotionData *pMB1, MotionData *pMB2, MotionEditingInfo *pMBInfo, MotionData *pMotionData)
{
	unsigned int i, j, k, numFrames;
	float var, time;
	float amp1, amp2;
	MotionMarker *mk1, *mk2, *mk3;
	PathPoint pp1, pp2;

	if(pMB1 != NULL && pMB2 != NULL && pMB1->numMarkers == pMB2->numMarkers && pMotionData != NULL)
	{
		MotionMarker *pMK = NULL, *pPrevMK = NULL;

		numFrames = pMBInfo->numFrames;

		pMotionData->numFrames = pMBInfo->numFrames;
		pMotionData->numMarkers = pMBInfo->numMarkers;
		pMotionData->FrameTime = pMBInfo->FrameTime;
		pMotionData->m_MotionDataType = pMBInfo->MotionDataType;


		if(IsSketelonIdentical(pMB1, pMB2))
		{
			// Sketelons are identical, so we blend all joints now.
			
			for(i = 0; i < pMB1->numMarkers; i++)
			{
				mk1 = pMB1->GetMarker(i);

				pMK = new MotionMarker;
				if(i == 0)
					pMotionData->pMarkerHead = pMK;
				else
					pPrevMK->next = pMK;

				pMK->nPosition = mk1->nPosition;

				if(!mk1->IsEndSite)
				{
					// Initialize new marker.
					pMK->Initialize(mk1->numDOFs, pMotionData->numFrames, mk1->name, BVH_MARKER);
					pMK->IsEndSite = mk1->IsEndSite;
					pMK->offset = mk1->offset;

					for(k = 0; k < mk1->numDOFs; k++)
					{
						pMK->SetDOFName(k, mk1->CurveName[k].name);
						time = 0.0f;
						for(j = 0; j < numFrames; j++)
						{
							// Blend two motion curves linearly.
							pp1 = pMB1->GetPathPoint(i, pMBInfo->StartFrame_M1 + j, k);
							pp2 = pMB2->GetPathPoint(GetMarkerIndex(pMB2, mk1->name), pMBInfo->StartFrame_M2 + j, k);

							amp1 = pp1.GetAmpVal();
							amp2 = pp2.GetAmpVal();

							var = amp1 * (1.0f - pMBInfo->pSimVal[pMK->nPosition]) + amp2 * pMBInfo->pSimVal[pMK->nPosition];
							//var = pMB1->GetCurveVal(i, k, pMBInfo->StartTime_M1 + t1) * 1.0f + pMB2->GetCurveVal(i, k, pMBInfo->StartTime_M2 + t2) * 0.0f;
							pMK->pps[k].pp[j].SetAmpVal(var);
							pMK->pps[k].pp[j].SetTimeVal(time);
							time += pMBInfo->FrameTime;
						}
					}
				}
				else
				{
					// For End Site, just don't blend anything.
					strcpy(pMK->name, mk1->name);
					pMK->IsEndSite = mk1->IsEndSite;
					pMK->offset = mk1->offset;
				}

				pPrevMK = pMK;
			}

			


			// Set parent pointers of joints in resulting motion buffer after blending.
			char parent_name[256];

			mk1 = pMB1->pMarkerHead;
			mk2 = pMotionData->pMarkerHead;
			for(i = 0; i < pMB1->numMarkers; i++)
			{
				if(mk1->parent != NULL)
				{
					strcpy(parent_name, mk1->parent->name);

					mk3 = pMotionData->pMarkerHead;
					while(strcmp(parent_name, mk3->name) != 0)
						mk3 = mk3->next;
					mk2->parent = mk3;

				}
				mk1 = mk1->next;
				mk2 = mk2->next;
			}
			
		}// End linear motion blending.
		else
			return false;
	}// End if pointers are null.
	else
		return false;

	return true;
}

bool MotionEditing::LinearBlendFramesByCurve(MotionData *pMB1, MotionData *pMB2, MotionEditingInfo *pMBInfo, MotionData *pMotionData)
{
	unsigned int i, j, k;
	float var, time, length, t1, t2;
	float amp1, amp2;
	MotionMarker *mk1, *mk2, *mk3;
	PathPoint pp1, pp2;

	if(pMB1 != NULL && pMB2 != NULL && pMB1->numMarkers == pMB2->numMarkers && pMotionData != NULL)
	{
		MotionMarker *pMK = NULL, *pPrevMK = NULL;

		pMotionData->numFrames = pMBInfo->numFrames;
		pMotionData->numMarkers = pMBInfo->numMarkers;
		pMotionData->FrameTime = pMBInfo->FrameTime;
		pMotionData->m_MotionDataType = pMBInfo->MotionDataType;

		float Length_M1, Length_M2;
		Length_M1 = pMBInfo->EndTime_M1 - pMBInfo->StartTime_M1;
		Length_M2 = pMBInfo->EndTime_M2 - pMBInfo->StartTime_M2;

		float AdvTime_M1, AdvTime_M2;
		AdvTime_M1 = Length_M1 / (float)pMotionData->numFrames;
		AdvTime_M2 = Length_M2 / (float)pMotionData->numFrames;

		length = pMotionData->FrameTime * pMotionData->numFrames;

		if(IsSketelonIdentical(pMB1, pMB2))
		{
			// Sketelons are identical, so we blend all joints now.
			
			for(i = 0; i < pMB1->numMarkers; i++)
			{
				mk1 = pMB1->GetMarker(i);

				pMK = new MotionMarker;
				if(i == 0)
					pMotionData->pMarkerHead = pMK;
				else
					pPrevMK->next = pMK;

				pMK->nPosition = mk1->nPosition;

				if(!mk1->IsEndSite)
				{
					// Initialize new marker.
					pMK->Initialize(mk1->numDOFs, pMotionData->numFrames, mk1->name, BVH_MARKER);
					pMK->IsEndSite = mk1->IsEndSite;
					pMK->offset = mk1->offset;

					for(k = 0; k < mk1->numDOFs; k++)
					{
						pMK->SetDOFName(k, mk1->CurveName[k].name);
						//j = 0;
						t1 = 0.0f;
						t2 = 0.0f;
						time = 0.0f;
						//for(time = 0.0f; time < length; time += pMotionData->FrameTime)
						for(j = 0; j < pMotionData->numFrames; j++)
						{
							// Blend two motion curves linearly.
							amp1 = pMB1->GetCurveVal(i, k, pMBInfo->StartTime_M1 + t1);
							amp2 = pMB2->GetCurveVal(i, k, pMBInfo->StartTime_M2 + t2);

							if(j >= pMotionData->numFrames)
								MessageBox(NULL, "Error!", "Error", MB_OK);

							var = amp1 * (1.0f - pMBInfo->pSimVal[pMK->nPosition]) + amp2 * pMBInfo->pSimVal[pMK->nPosition];
							pMK->pps[k].pp[j].SetAmpVal(var);
							pMK->pps[k].pp[j].SetTimeVal(time);

							t1 += AdvTime_M1;
							t2 += AdvTime_M2;
							time += pMotionData->FrameTime;
						}
					}
				}
				else
				{
					// For End Site, just don't blend anything.
					strcpy(pMK->name, mk1->name);
					pMK->IsEndSite = mk1->IsEndSite;
					pMK->offset = mk1->offset;
				}

				pPrevMK = pMK;
			}

			// Set parent pointers of joints in resulting motion buffer after blending.
			char parent_name[256];

			mk1 = pMB1->pMarkerHead;
			mk2 = pMotionData->pMarkerHead;
			for(i = 0; i < pMB1->numMarkers; i++)
			{
				if(mk1->parent != NULL)
				{
					strcpy(parent_name, mk1->parent->name);

					mk3 = pMotionData->pMarkerHead;
					while(strcmp(parent_name, mk3->name) != 0)
						mk3 = mk3->next;
					mk2->parent = mk3;

				}
				mk1 = mk1->next;
				mk2 = mk2->next;
			}
			
		}// End linear motion blending.
		else
			return false;
	}// End if pointers are null.
	else
		return false;

	return true;
}

bool MotionEditing::LinearExaggeratingFramesByCurve(MotionData *pMB1, MotionData *pMB2, MotionEditingInfo *pMBInfo, MotionData *pMotionData)
{
	unsigned int i, j, k;
	float var, time, length, t1, t2;
	float amp1, amp2;
	MotionMarker *mk1, *mk2, *mk3;
	PathPoint pp1, pp2;

	if(pMB1 != NULL && pMB2 != NULL && pMB1->numMarkers == pMB2->numMarkers && pMotionData != NULL)
	{
		MotionMarker *pMK = NULL, *pPrevMK = NULL;

		pMotionData->numFrames = pMBInfo->numFrames;
		pMotionData->numMarkers = pMBInfo->numMarkers;
		pMotionData->FrameTime = pMBInfo->FrameTime;
		pMotionData->m_MotionDataType = pMBInfo->MotionDataType;

		float Length_M1, Length_M2;
		Length_M1 = pMBInfo->EndTime_M1 - pMBInfo->StartTime_M1;
		Length_M2 = pMBInfo->EndTime_M2 - pMBInfo->StartTime_M2;

		float AdvTime_M1, AdvTime_M2;
		AdvTime_M1 = Length_M1 / (float)pMotionData->numFrames;
		AdvTime_M2 = Length_M2 / (float)pMotionData->numFrames;

		length = pMotionData->FrameTime * pMotionData->numFrames;

		if(IsSketelonIdentical(pMB1, pMB2))
		{
			// Sketelons are identical, so we blend all joints now.
			
			for(i = 0; i < pMB1->numMarkers; i++)
			{
				mk1 = pMB1->GetMarker(i);

				pMK = new MotionMarker;
				if(i == 0)
					pMotionData->pMarkerHead = pMK;
				else
					pPrevMK->next = pMK;

				pMK->nPosition = mk1->nPosition;

				if(!mk1->IsEndSite)
				{
					// Initialize new marker.
					pMK->Initialize(mk1->numDOFs, pMotionData->numFrames, mk1->name, BVH_MARKER);
					pMK->IsEndSite = mk1->IsEndSite;
					pMK->offset = mk1->offset;

					for(k = 0; k < mk1->numDOFs; k++)
					{
						pMK->SetDOFName(k, mk1->CurveName[k].name);
						t1 = 0.0f;
						t2 = 0.0f;
						time = 0.0f;
						for(j = 0; j < pMotionData->numFrames; j++)
						{
							// Blend two motion curves linearly.
							amp1 = pMB1->GetCurveVal(i, k, pMBInfo->StartTime_M1 + t1);
							amp2 = pMB2->GetCurveVal(i, k, pMBInfo->StartTime_M2 + t2);

							if(j >= pMotionData->numFrames)
								MessageBox(NULL, "Error!", "Error", MB_OK);

							var = amp1 + (amp2 - amp1) * pMBInfo->pSimVal[pMK->nPosition];
							pMK->pps[k].pp[j].SetAmpVal(var);
							pMK->pps[k].pp[j].SetTimeVal(time);

							t1 += AdvTime_M1;
							t2 += AdvTime_M2;
							time += pMotionData->FrameTime;
						}
					}
				}
				else
				{
					// For End Site, just don't blend anything.
					strcpy(pMK->name, mk1->name);
					pMK->IsEndSite = mk1->IsEndSite;
					pMK->offset = mk1->offset;
				}

				pPrevMK = pMK;
			}

			// Set parent pointers of joints in resulting motion buffer after blending.
			char parent_name[256];

			mk1 = pMB1->pMarkerHead;
			mk2 = pMotionData->pMarkerHead;
			for(i = 0; i < pMB1->numMarkers; i++)
			{
				if(mk1->parent != NULL)
				{
					strcpy(parent_name, mk1->parent->name);

					mk3 = pMotionData->pMarkerHead;
					while(strcmp(parent_name, mk3->name) != 0)
						mk3 = mk3->next;
					mk2->parent = mk3;

				}
				mk1 = mk1->next;
				mk2 = mk2->next;
			}
			
		}// End linear motion blending.
		else
			return false;
	}// End if pointers are null.
	else
		return false;

	return true;
}

bool MotionEditing::TransiteMotionsByCurve(MotionData *pMB1, MotionData *pMB2, MotionEditingInfo *pMEInfo, MotionData *pFinalMotionData)
{
	if(pMB1 != NULL && pMB2 != NULL && pMB1->numMarkers == pMB2->numMarkers && pFinalMotionData != NULL)
	{
		MotionData *pMotionData = new MotionData;
		float t, var;
		PathPoint pp;
		unsigned int i, j, k, l;
		MotionMarker *pMK = NULL, *pPrevMK = NULL;
		MotionMarker *pMK1;
		float t_offset = 0.0f, p_offset = 0.0f, t_step;
		float length = 0.0f;

		pMotionData->numFrames = pMEInfo->numFrames;
		pMotionData->numMarkers = pMEInfo->numMarkers;
		pMotionData->FrameTime = pMEInfo->FrameTime;
		pMotionData->m_MotionDataType = pMEInfo->MotionDataType;
		t = pMEInfo->TransitingTime;

		// Connect two motions.
		for(i = 0; i < pMB1->numMarkers; i++)
		{
			pMK = new MotionMarker;
			if(i == 0)
				pMotionData->pMarkerHead = pMK;
			else
				pPrevMK->next = pMK;

			pMK1 = pMB1->GetMarker(i);
			pMK->nPosition = pMK1->nPosition;

			if(!pMK1->IsEndSite)
			{
				pMK->Initialize(pMK1->numDOFs, pMotionData->numFrames, pMK1->name, BVH_MARKER);
				pMK->IsEndSite = pMK1->IsEndSite;
				pMK->offset = pMK1->offset;
				for(k = 0; k < pMK1->numDOFs; k++)
				{
					pMK->SetDOFName(k, pMK1->CurveName[k].name);
					// Motion 1
					l = 0;
					t_step = 0.0f;
					for(j = pMEInfo->StartFrame_M1; j < pMEInfo->EndFrame_M1; j++)
					{
						pp = pMB1->GetPathPoint(i, j, k);

						pMK->pps[k].pp[l].SetAmpVal(pp.GetAmpVal());
						//pMK->pps[k].pp[l].SetTimeVal(pp.GetTimeVal());
						pMK->pps[k].pp[l].SetTimeVal(t_step);
						//t_offset = pp.GetTimeVal();
						t_offset = t_step;
						l++;
						t_step += pMEInfo->FrameTime;
					}
					// Motion 2
					t_step = 0.0f;
					for(j = pMEInfo->StartFrame_M2; j < pMEInfo->EndFrame_M2; j++)
					{
						// Compensate displacement of motion 1.
						if(pMK1->parent == NULL && (k == 0 || k == 1 || k == 2))
						{
							float temp;

							pp = pMB1->GetPathPoint(i, pMEInfo->EndFrame_M1 - 1, k);
							p_offset = pp.GetAmpVal();
							pp = pMB2->GetPathPoint(pMB2->GetMarkerIndex(pMK1->name), pMEInfo->StartFrame_M2, k);
							temp = pp.GetAmpVal();

							pp = pMB2->GetPathPoint(pMB2->GetMarkerIndex(pMK1->name), j, k);

							pMK->pps[k].pp[l].SetAmpVal((pp.GetAmpVal() - temp) + p_offset);
							pMK->pps[k].pp[l].SetTimeVal(t_offset + t + t_step);
						}
						else
						{
							pp = pMB2->GetPathPoint(pMB2->GetMarkerIndex(pMK1->name), j, k);

							pMK->pps[k].pp[l].SetAmpVal(pp.GetAmpVal());
							pMK->pps[k].pp[l].SetTimeVal(t_offset + t + t_step);
						}
						l++;
						t_step += pMEInfo->FrameTime;
					}
					
				}
			}
			else
			{
				// For End Site, just don't blend anything.
				strcpy(pMK->name, pMK1->name);
				pMK->IsEndSite = pMK1->IsEndSite;
				pMK->offset = pMK1->offset;
			}

			pPrevMK = pMK;
		}

		// Set parent pointers of joints in resulting motion buffer.
		LinkToParent(pMB1, pMotionData);

		// Fit newly combined motion.
		MotionCurves_UCBS MC(pMotionData);		
		if(!MC.FitAllMarkersToCurves())
			return false;
		pMotionData->m_IsAllMarkersFitted = true;
		// Release coef matrix and inverse matrix of motion buffer.
		MC.ReleaseMatrices();

		// Create newly combined motion.
		length = pMEInfo->EndTime_M1 - pMEInfo->StartTime_M1 + pMEInfo->EndTime_M2 - pMEInfo->StartTime_M2 + t;
		pFinalMotionData->numFrames = (unsigned int)(length / pMotionData->FrameTime);
		pFinalMotionData->numMarkers = pMotionData->numMarkers;
		pFinalMotionData->FrameTime = pMotionData->FrameTime;
		pFinalMotionData->m_MotionDataType = pMotionData->m_MotionDataType;

		for(i = 0; i < pMotionData->numMarkers; i++)
		{
			pMK = new MotionMarker;
			if(i == 0)
				pFinalMotionData->pMarkerHead = pMK;
			else
				pPrevMK->next = pMK;

			pMK1 = pMotionData->GetMarker(i);
			pMK->nPosition = pMK1->nPosition;

			if(!pMK1->IsEndSite)
			{
				pMK->Initialize(pMK1->numDOFs, pFinalMotionData->numFrames, pMK1->name, BVH_MARKER);
				pMK->IsEndSite = pMK1->IsEndSite;
				pMK->offset = pMK1->offset;
				for(k = 0; k < pMK1->numDOFs; k++)
				{
					pMK->SetDOFName(k, pMK1->CurveName[k].name);
					
					t_step = 0.0f;
					for(j = 0; j < pMotionData->numFrames; j++)
					{
						var = pMotionData->GetCurveVal(i, k, t_step);

						pMK->pps[k].pp[j].SetAmpVal(var);
						pMK->pps[k].pp[j].SetTimeVal(t_step);
						t_step += pMotionData->FrameTime;
					}

				}
			}
			else
			{
				// For End Site, just don't blend anything.
				strcpy(pMK->name, pMK1->name);
				pMK->IsEndSite = pMK1->IsEndSite;
				pMK->offset = pMK1->offset;
			}

			pPrevMK = pMK;
		}

		// Set parent pointers of joints in resulting motion buffer.
		LinkToParent(pMotionData, pFinalMotionData);

		// Clean temporary motion data.
		delete pMotionData;
	}
	else
		return false;

	return true;
}

bool MotionEditing::LinkToParent(MotionData *pMD1, MotionData *pMD2)
{
	MotionMarker *pMK1, *pMK2, *pMK3;
	char parent_name[256];

		pMK1 = pMD1->pMarkerHead;
		pMK2 = pMD2->pMarkerHead;
		for(unsigned int i = 0; i < pMD1->numMarkers; i++)
		{
			if(pMK1->parent != NULL)
			{
				strcpy(parent_name, pMK1->parent->name);

				pMK3 = pMD2->pMarkerHead;
				while(strcmp(parent_name, pMK3->name) != 0)
					pMK3 = pMK3->next;
				pMK2->parent = pMK3;

			}
			pMK1 = pMK1->next;
			pMK2 = pMK2->next;
		}

	return true;
}

bool MotionEditing::ExtractMotionByCurve(MotionData *pMB, MotionEditingInfo *pMEInfo, MotionData *pMotionData)
{
	if(pMB != NULL && pMotionData != NULL && pMEInfo != NULL)
	{
		MotionMarker *pMK = NULL, *pPrevMK = NULL;
		unsigned int i, j, k, l;
		MotionMarker *pMK1;
		float t_step, var;

		pMotionData->numFrames = pMEInfo->numFrames;
		pMotionData->numMarkers = pMEInfo->numMarkers;
		pMotionData->FrameTime = pMEInfo->FrameTime;
		pMotionData->m_MotionDataType = pMEInfo->MotionDataType;

		for(i = 0; i < pMB->numMarkers; i++)
		{
			pMK = new MotionMarker;
			if(i == 0)
				pMotionData->pMarkerHead = pMK;
			else
				pPrevMK->next = pMK;

			pMK1 = pMB->GetMarker(i);
			pMK->nPosition = pMK1->nPosition;

			if(!pMK1->IsEndSite)
			{
				pMK->Initialize(pMK1->numDOFs, pMotionData->numFrames, pMK1->name, BVH_MARKER);
				pMK->IsEndSite = pMK1->IsEndSite;
				pMK->offset = pMK1->offset;
				for(k = 0; k < pMK1->numDOFs; k++)
				{
					pMK->SetDOFName(k, pMK1->CurveName[k].name);
					l = 0;
					t_step = pMEInfo->StartFrame_M1 * pMEInfo->FrameTime;
					for(j = pMEInfo->StartFrame_M1; j < pMEInfo->EndFrame_M1; j++)
					{
						var = pMB->GetCurveVal(i, k, t_step);

						pMK->pps[k].pp[l].SetAmpVal(var);
						pMK->pps[k].pp[l].SetTimeVal(t_step);
						l++;
						t_step += pMEInfo->FrameTime;
					}
				}
			}
			else
			{
				// For End Site, just don't blend anything.
				strcpy(pMK->name, pMK1->name);
				pMK->IsEndSite = pMK1->IsEndSite;
				pMK->offset = pMK1->offset;
			}

			pPrevMK = pMK;
		}

		// Set parent pointers of joints in resulting motion buffer.
		LinkToParent(pMB, pMotionData);
	}
	else
		return false;

	return true;
}

bool MotionEditing::WarpMotionByCurve(MotionData *pMB, MotionEditingInfo *pMEInfo, MotionData *pMotionData)
{
	if(pMB != NULL && pMotionData != NULL && pMEInfo != NULL)
	{
		MotionMarker *pMK = NULL, *pPrevMK = NULL;
		unsigned int i, k, l;
		MotionMarker *pMK1;
		float t_step, var;

		pMotionData->numFrames = pMEInfo->numFrames;
		pMotionData->numMarkers = pMEInfo->numMarkers;
		pMotionData->FrameTime = pMEInfo->FrameTime;
		pMotionData->m_MotionDataType = pMEInfo->MotionDataType;

		for(i = 0; i < pMB->numMarkers; i++)
		{
			pMK = new MotionMarker;
			if(i == 0)
				pMotionData->pMarkerHead = pMK;
			else
				pPrevMK->next = pMK;

			pMK1 = pMB->GetMarker(i);
			pMK->nPosition = pMK1->nPosition;

			if(!pMK1->IsEndSite)
			{
				pMK->Initialize(pMK1->numDOFs, pMotionData->numFrames, pMK1->name, BVH_MARKER);
				pMK->IsEndSite = pMK1->IsEndSite;
				pMK->offset = pMK1->offset;
				for(k = 0; k < pMK1->numDOFs; k++)
				{
					pMK->SetDOFName(k, pMK1->CurveName[k].name);
					t_step = 0.0f;
					for(l = 0; l < pMEInfo->numFrames; l++)
					{
						var = pMB->GetCurveVal(i, k, t_step * (pMEInfo->EndTime_M1 / pMEInfo->Duration));

						pMK->pps[k].pp[l].SetAmpVal(var);
						pMK->pps[k].pp[l].SetTimeVal(t_step);
						t_step += pMEInfo->FrameTime;
					}
				}
			}
			else
			{
				// For End Site, just don't blend anything.
				strcpy(pMK->name, pMK1->name);
				pMK->IsEndSite = pMK1->IsEndSite;
				pMK->offset = pMK1->offset;
			}

			pPrevMK = pMK;
		}

		// Set parent pointers of joints in resulting motion buffer.
		LinkToParent(pMB, pMotionData);
	}
	else
		return false;

	return true;
}

bool MotionEditing::FlipFilter(PathPointSet *pps, float tol, bool IsIgnoreSign)
{
	unsigned int i, j, k;
	float amp, amp_next, var;
	float amp1, amp2;
	PathPoint pp;

	for(i = 0; i < pps->numPoints; i++)
	{
		if(i != pps->numPoints - 1)
		{
			amp = pps->pp[i].GetAmpVal();
			amp_next = pps->pp[i + 1].GetAmpVal();
			
			var = amp - amp_next;
			
			if(IsIgnoreSign)
			{
				if(fabs(var) > tol)
				{
					for(j = i + 1; j < pps->numPoints - 1; j++)
					{
						amp1 = pps->pp[j].GetAmpVal();
						amp2 = pps->pp[j + 1].GetAmpVal();

						if(fabs(amp1 - amp2) > tol)
							break;
					}
					
					for(k = i + 1; k <= j; k++)
					{
						amp = pps->pp[k].GetAmpVal();
						pps->pp[k].SetAmpVal(amp + var);
					}
				}
			}
			else
			{
				if(fabs(var) > tol && amp * amp_next < 0.0f)
				{
					for(j = i + 1; j < pps->numPoints - 1; j++)
					{
						amp1 = pps->pp[j].GetAmpVal();
						amp2 = pps->pp[j + 1].GetAmpVal();

						if(fabs(amp1 - amp2) > tol)
							break;
					}
					
					for(k = i + 1; k <= j; k++)
					{
						amp = pps->pp[k].GetAmpVal();
						pps->pp[k].SetAmpVal(amp + var);
					}
				}
			}
		}
	}

	return true;
}
