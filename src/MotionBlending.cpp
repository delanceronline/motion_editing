// MotionBlending.cpp: implementation of the MotionBlending class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "KungfuMotion.h"
#include "MotionBlending.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MotionBlending::MotionBlending()
{
}

MotionBlending::~MotionBlending()
{
}

bool MotionBlending::LinearBlendFrames(MotionData *md1, MotionData *md2,  unsigned int StartFrame, unsigned int EndFrame, float t, MotionData *pMotionData)
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

bool MotionBlending::IsIdenticalJoint(MotionMarker *mk1, MotionMarker *mk2)
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

bool MotionBlending::HasIdenticalName(MotionData *pMD)
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

bool MotionBlending::IsSketelonIdentical(MotionData *pMD1, MotionData *pMD2)
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

unsigned int MotionBlending::GetMarkerIndex(MotionData *pMD, const char *MarkerName)
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
