// MotionData.cpp: implementation of the MotionData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MotionData.h"
#include "Vector3D.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MotionData::MotionData()
{
	pMarkerHead = NULL;
	next = NULL;
	m_pCoefMat = NULL;
	m_pInvMat = NULL;
	m_pMSeq = NULL;
	m_MotionDataType = UNKNOWN_MOTION_TYPE;

	numFrames = 0;
	numMarkers = 0;
	m_nMSeq = 0;
	FrameTime = 0.0f;

	m_IsMatricesReady = false;
	m_IsAllMarkersFitted = false;
}

MotionData::~MotionData()
{
	if(pMarkerHead != NULL)
	{
		MotionMarker *temp1, *temp2;

		temp1 = pMarkerHead;
		while(temp1 != NULL)
		{
			temp2 = temp1->next;
			delete temp1;
			temp1 = temp2;
		}
	}

	if(m_pCoefMat != NULL)
	{
		delete m_pCoefMat;
		m_pCoefMat = NULL;
	}

	if(m_pInvMat != NULL)
	{
		delete m_pInvMat;
		m_pInvMat = NULL;
	}

	if(m_pMSeq != NULL)
	{
		delete[] m_pMSeq;
		m_pMSeq = NULL;
	}
}

unsigned int MotionData::FileType()
{
	return m_MotionDataType;
}

bool MotionData::LoadTRCFile(const char *filename)
{
	FILE *stream;

	stream = fopen(filename, "r");
	if(stream != NULL)
	{
		unsigned int i, j;
		float amp, time;
		char line[2048], word[256];

		fseek(stream, 0L, SEEK_SET);
		fgets(line, 2048, stream);
		fgets(line, 2048, stream);
		fgets(line, 2048, stream);
		sscanf(line, "%f\t%f\t%d\t%d\t%f\t%d\t%d\t%s\n", &DataRate, &CameraRate, &numFrames, &numMarkers, Units, &time, &i, &j);

		// Allocate markers.
		MotionMarker *pMK = NULL, *pPrevMK = NULL;

		fscanf(stream, "%s", word);
		fscanf(stream, "%s", word);
		for(i = 0; i < numMarkers; i++)
		{
			pMK = new MotionMarker;
			
			if(i == 0)
				pMarkerHead = pMK;
			else
				pPrevMK->next = pMK;

			// Read marker's name.
			fscanf(stream, "%s", word);

			// Initialize markers
			pMK->Initialize(3, numFrames, word, TRC_MARKER);
			pMK->nPosition = i;

			pPrevMK = pMK;
		}

		// Set names for all DOFs.
		for(i = 0; i < numMarkers; i++)
		{
			MotionMarker *pMK = GetMarker(i);
			for(j = 0; j < pMK->numDOFs; j++)
			{
				if(fscanf(stream, "%s", word) != 1)
					return false;
				pMK->SetDOFName(j, word);
			}
		}
		
		// Jump down one line.
		while(fgetc(stream) != '\n');

		for(i = 0; i < numFrames; i++)
		{
			fscanf(stream, "%s", word);
			fscanf(stream, "%f", &time);

			pMK = pMarkerHead;
			for(j = 0; j < numMarkers; j++)
			{
				fscanf(stream, "%f", &amp);
				pMK->pps[0].pp[i].SetAmpVal(amp);
				fscanf(stream, "%f", &amp);
				pMK->pps[1].pp[i].SetAmpVal(amp);
				fscanf(stream, "%f", &amp);
				pMK->pps[2].pp[i].SetAmpVal(amp);

				pMK->pps[0].pp[i].SetTimeVal(time);
				pMK->pps[1].pp[i].SetTimeVal(time);
				pMK->pps[2].pp[i].SetTimeVal(time);

				pMK = pMK->next;
			}
		}

	}
	else
		return false;

	fclose(stream);

	SetMotionFileType(TRC_MOTION_TYPE);

	return true;
}

bool MotionData::GetPointX(unsigned int markerIndex, unsigned int frame, PathPoint *pp)
{
	if(m_MotionDataType == TRC_MOTION_TYPE)
	{
		pp->SetAmpVal(GetMarker(markerIndex)->pps[0].pp[frame].GetAmpVal());
		pp->SetTimeVal(GetMarker(markerIndex)->pps[0].pp[frame].GetTimeVal());

		return true;
	}
	else
		return false;
}

bool MotionData::GetPointY(unsigned int markerIndex, unsigned int frame, PathPoint *pp)
{
	if(m_MotionDataType == TRC_MOTION_TYPE)
	{
		pp->SetAmpVal(GetMarker(markerIndex)->pps[1].pp[frame].GetAmpVal());
		pp->SetTimeVal(GetMarker(markerIndex)->pps[1].pp[frame].GetTimeVal());

		return true;
	}
	else
		return false;
}

bool MotionData::GetPointZ(unsigned int markerIndex, unsigned int frame, PathPoint *pp)
{
	if(m_MotionDataType == TRC_MOTION_TYPE)
	{
		pp->SetAmpVal(GetMarker(markerIndex)->pps[2].pp[frame].GetAmpVal());
		pp->SetTimeVal(GetMarker(markerIndex)->pps[2].pp[frame].GetTimeVal());

		return true;
	}
	else
		return false;
}

bool MotionData::SaveTRCFile(const char *filename)
{

	return true;
}

char * MotionData::GetMarkerName(unsigned int index)
{
	if(m_MotionDataType != UNKNOWN_MOTION_TYPE)
		return GetMarker(index)->name;

	return NULL;
}

PathPointSet * MotionData::GetPathPointSetX(unsigned int markerIndex)
{
	if(m_MotionDataType == TRC_MOTION_TYPE)
		return &GetMarker(markerIndex)->pps[0];
	else
		return NULL;
}

PathPointSet * MotionData::GetPathPointSetY(unsigned int markerIndex)
{
	if(m_MotionDataType == TRC_MOTION_TYPE)
		return &GetMarker(markerIndex)->pps[1];
	else
		return NULL;
}

PathPointSet * MotionData::GetPathPointSetZ(unsigned int markerIndex)
{
	if(m_MotionDataType == TRC_MOTION_TYPE)
		return &GetMarker(markerIndex)->pps[2];
	else
		return NULL;
}

void MotionData::SetMotionFileType(unsigned int type)
{
	m_MotionDataType = type;

	if(	m_MotionDataType == TRC_MOTION_TYPE)
		m_maxDOFs = 3;

	if(	m_MotionDataType == BVH_MOTION_TYPE)
		m_maxDOFs = 6;
}

bool MotionData::AllocateControlPoints(unsigned int nMarker, unsigned int numCPs)
{
	if(m_MotionDataType != UNKNOWN_MOTION_TYPE)
	{
		MotionMarker *pMK;

		pMK = GetMarker(nMarker);

		if(pMK != NULL)
		{
			if(!pMK->IsEndSite)
			{
				unsigned int i;

				pMK->cps = new ControlPointSet[pMK->numDOFs];
				for(i = 0; i < pMK->numDOFs; i++)
					pMK->cps[i].Reset(numCPs);
				pMK->numCPs = numCPs;
			}
		}
		else
			return false;
	}
	else
		return false;

	return true;
}

ControlPointSet * MotionData::GetControlPointSetFromMarker(unsigned int nMarker, unsigned int nDOF)
{
	if(m_MotionDataType != UNKNOWN_MOTION_TYPE)
	{
		MotionMarker *pMK = GetMarker(nMarker);

		if(pMK != NULL)
			if(!pMK->IsEndSite)
				return &GetMarker(nMarker)->cps[nDOF];
			else
				return NULL;
	}

	return NULL;
}

// This function is to initialize cubic B-Spline curve objects of marker.
bool MotionData::InitializeAllMotionCurves(void)
{
	if(m_MotionDataType != UNKNOWN_MOTION_TYPE)
	{
		unsigned int i, j;
		MotionMarker *pMK;

		pMK = pMarkerHead;
		for(i = 0; i < numMarkers; i++)
		{
			if(!pMK->IsEndSite)
			{
				for(j = 0; j < pMK->numDOFs; j++)
				{
					pMK->m_cbsc[j].InitializeFromPathPointSet(&pMK->pps[j], NULL, NULL);
					pMK->m_cbsc[j].FindMaxAndMinPPAmp();
				}
			}
			pMK = pMK->next;
		}
	}

	return true;
}

// When this function is called, it is assumed that coef matrix and its inverse exist.
bool MotionData::FitMarkerToUCBS(unsigned int nMarker)
{
	// Allocate space for control point sets in marker.
	AllocateControlPoints(nMarker, numFrames + 2);

	if(m_MotionDataType != UNKNOWN_MOTION_TYPE)
	{
		MotionMarker *pMK = GetMarker(nMarker);

		if(pMK != NULL)
			if(!pMK->IsEndSite)
			{
				unsigned int i;

				for(i = 0; i < pMK->numDOFs; i++)
					pMK->m_cbsc[i].CalculateControlPointsFromPathPoints(&pMK->cps[i]);
			}
	}

	return true;
}

float MotionData::GetCurveVal(unsigned int nMarker, unsigned int nDOF, float time)
{
	if(m_MotionDataType != UNKNOWN_MOTION_TYPE)
	{
		MotionMarker *pMK = GetMarker(nMarker);

		if(pMK != NULL)
			if(!pMK->IsEndSite)
			return pMK->m_cbsc[nDOF].GetCurveVal(time);
	}

	return 0.0f;
}

PathPoint MotionData::GetPathPoint(unsigned int nMarker, unsigned int nFrame, unsigned int nDOF)
{
	PathPoint pp;

	if(m_MotionDataType != UNKNOWN_MOTION_TYPE)
	{
		MotionMarker *pMK = GetMarker(nMarker);

		if(pMK != NULL)
			if(!pMK->IsEndSite)
				pMK->m_cbsc[nDOF].GetPathPoint(nFrame, &pp);
	}

	return pp;
}

float MotionData::GetMaxAmp(unsigned int nMarker, unsigned int nDOF)
{
	if(m_MotionDataType != UNKNOWN_MOTION_TYPE)
	{
		MotionMarker *pMK = GetMarker(nMarker);

		if(pMK != NULL)
			if(!pMK->IsEndSite)
				return GetMarker(nMarker)->m_cbsc[nDOF].GetMaxAmp();
	}
	
	return 0.0f;
}

float MotionData::GetMinAmp(unsigned int nMarker, unsigned int nDOF)
{
	if(m_MotionDataType != UNKNOWN_MOTION_TYPE)
	{
		MotionMarker *pMK = GetMarker(nMarker);

		if(pMK != NULL)
			if(!pMK->IsEndSite)
				return pMK->m_cbsc[nDOF].GetMinAmp();
	}

	return 0.0f;
}

// This function does not work.
bool MotionData::NodeToArray()
{
	/*
	unsigned int i, count = 0;
	MotionMarker *temp;

	temp = pMarkerHead;
	while(temp != NULL)
	{
		count++;
		temp = temp->next;
	}

	if(count != 0)
	{
		temp = pMarkerHead;
		marker = new MotionMarker[count];
		for(i = 0; i < count; i++)
		{
			marker[i] = *temp;
			temp = temp->next;
		}
	}
*/
	return true;
}

// This function is to get address of marker in marker linked list.
MotionMarker *MotionData::GetMarker(unsigned int nMarker)
{
	if(pMarkerHead != NULL)
	{
		MotionMarker *pMK;

		pMK = pMarkerHead;
		while(pMK != NULL)
		{
			if(pMK->nPosition == nMarker)
				return pMK;
			pMK = pMK->next;
		}
		return NULL;
	}
	else
		return NULL;
}

char * MotionData::GetDOFName(unsigned int nMarker, unsigned int nDOF)
{
	return GetMarker(nMarker)->CurveName[nDOF].name;
}

unsigned int MotionData::GetNumDOFs()
{
	unsigned int count = 0;
	MotionMarker *pMK = pMarkerHead;

	while(pMK != NULL)
	{
		count += pMK->numDOFs;

		pMK = pMK->next;
	}

	return count;
}

bool MotionData::IsMarkerEndSite(unsigned int nMarker)
{
	return GetMarker(nMarker)->IsEndSite;
}

bool MotionData::BuildCoefMatrix(unsigned int N)
{
	//Setup coefficient matrix with N x N dimension.
	if(N >= 4)
	{
		m_pCoefMat = new Matrix(N, N);

		//coef_mat.ResetDimension(N, N);

		m_pCoefMat->SetVal(0, 0, 9.0f); m_pCoefMat->SetVal(0, 1, -3.0f);
		m_pCoefMat->SetVal(1, 0, 0.25f); m_pCoefMat->SetVal(1, 1, 7.0f / 12.0f); m_pCoefMat->SetVal(1, 2, 1.0f / 6.0f);

		for(unsigned int i = 2; i < N - 2; i++)
		{
			m_pCoefMat->SetVal(i, i - 1, 1.0f / 6.0f);
			m_pCoefMat->SetVal(i, i, 2.0f / 3.0f);
			m_pCoefMat->SetVal(i, i + 1, 1.0f / 6.0f);
		}

		m_pCoefMat->SetVal(N - 2, N - 3, 1.0f / 6.0f); m_pCoefMat->SetVal(N - 2, N - 2, 7.0f / 12.0f); m_pCoefMat->SetVal(N - 2, N - 1, 0.25f);
		m_pCoefMat->SetVal(N - 1, N - 2, -3.0f); m_pCoefMat->SetVal(N - 1, N - 1, 9.0f); 

		return true;
	}
	else
		return false;
}

bool MotionData::SetMatricesPointersToCurves()
{
	if(m_MotionDataType != UNKNOWN_MOTION_TYPE)
	{
		unsigned int i, j;
		MotionMarker *pMK;

		pMK = pMarkerHead;
		for(i = 0; i < numMarkers; i++)
		{
			if(!pMK->IsEndSite)
			{
				for(j = 0; j < pMK->numDOFs; j++)
					pMK->m_cbsc[j].SetExternalMatricesPointer(m_pCoefMat, m_pInvMat);
			}
			pMK = pMK->next;
		}
	}

	return true;
}

bool MotionData::LoadBVHFile(const char *filename)
{
	FILE *stream;

	stream = fopen(filename, "r");
	if(stream != NULL)
	{
		unsigned int i, j;
		float amp, time;
		char line[4096], word1[256], word2[256];

		// Read number of frames first.
		while(strcmp(word1, "MOTION") != 0)
		{
			if(fgets(line, 4096, stream) == NULL)
				return false;
			sscanf(line, "%s", word1);
		}
		fgets(line, 4096, stream);
		sscanf(line, "%s %d", word1, &numFrames);
		if(strcmp(word1, "Frames:") != 0)
			return false;

		// Read from beginning again now.
		fseek(stream, 0L, SEEK_SET);

		// Check hierarchy header.
		fgets(line, 4096, stream);
		sscanf(line, "%s", word1);
		if(strcmp(word1, "HIERARCHY") != 0)
			return false;

		// Read all markers and their relation.
		bool flag;
		if(!LoadBVHMarker(NULL, NULL, stream, &flag))
		{
			fclose(stream);
			return false;
		}

		// Check motion data section.
		fgets(line, 4096, stream);
		sscanf(line, "%s", word1);
		if(strcmp(word1, "MOTION") != 0)
			return false;

		// Read frame time.
		fgets(line, 4096, stream);
		fgets(line, 4096, stream);
		sscanf(line, "%s %s %f", word1, word2, &FrameTime);
		
		MotionMarker *temp;
		temp = pMarkerHead;
		time = 0.0f;

		for(i = 0; i < numFrames; i++)
		{
			while(temp != NULL)
			{
				if(!temp->IsEndSite)
				{
					for(j = 0; j < temp->numDOFs; j++)
					{
						fscanf(stream, "%f", &amp);
						temp->pps[j].pp[i].SetAmpVal(amp);
						temp->pps[j].pp[i].SetTimeVal(time);
					}
				}
				temp = temp->next;
			}
			
			time += FrameTime;
			temp = pMarkerHead;
		}
		
	}
	else
		return false;

	fclose(stream);

	//NodeToArray();
	// Find number of markers.
	MotionMarker *temp;
	numMarkers = 0;
	temp = pMarkerHead;
	while(temp != NULL)
	{
		numMarkers++;
		temp = temp->next;
	}

	SetMotionFileType(BVH_MOTION_TYPE);

	return true;
}

bool MotionData::LoadBVHMarker(MotionMarker *parent, MotionMarker *prev, FILE *stream, bool *IsNoMoreChild)
{
	bool IsRoot;
	unsigned int i, numDOFs;
	char line[4096], word1[256], name[256];
	MotionMarker *temp = NULL, *pMK = NULL;

	if(parent == NULL)
		IsRoot = true;
	else
		IsRoot = false;

	fgets(line, 4096, stream);
	sscanf(line, "%s %s", word1, name);

	// Add new node to chain.
	if(IsRoot)
	{
		if(strcmp(word1, "ROOT") != 0)
			return false;

		// Allocate first node.
		pMarkerHead = new MotionMarker;
		pMarkerHead->nPosition = 0;
		pMarkerHead->IsEndSite = false;
		pMK = pMarkerHead;
	}
	else
	{
		if(strcmp(word1, "JOINT") != 0 && strcmp(word1, "End") != 0 && strcmp(word1, "}") != 0)
			return false;

		// Exit peacefully.
		if(strcmp(word1, "}") == 0)
		{
			*IsNoMoreChild = true;
			return true;
		}

		// Allocate new node.
		temp = new MotionMarker;
		temp->prev = prev;
		if(prev != NULL)
		{
			prev->next = temp;
			temp->nPosition = prev->nPosition + 1;
			temp->parent = parent;

			// Determine whether END SITE JOINT.
			if(strcmp(word1, "JOINT") == 0)
				temp->IsEndSite = false;
			else
				temp->IsEndSite = true;
		}
		pMK = temp;
	}

	// Read "{".
	fgets(line, 4096, stream);
	sscanf(line, "%s", word1);
	if(strcmp(word1, "{") != 0)
		return false;
	
	// Read offset.
	fgets(line, 4096, stream);
	sscanf(line, "%s %f %f %f", word1, &pMK->offset.x, &pMK->offset.y, &pMK->offset.z);
	if(strcmp(word1, "OFFSET") != 0)
		return false;

	if(!pMK->IsEndSite)
	{
		// Read channel information.
		fscanf(stream, "%s", word1);
		if(strcmp(word1, "CHANNELS") != 0)
			return false;
		fscanf(stream, "%d", &numDOFs);

		// Initialize newly allocated marker.
		pMK->Initialize(numDOFs, numFrames, name, BVH_MARKER);

		// Set names to all DOFs (curves).
		for(i = 0; i < pMK->numDOFs; i++)
		{
			fscanf(stream, "%s", word1);
			pMK->SetDOFName(i, word1);
		}
		fgets(line, 4096, stream);
		
		// Recursively load markers.
		bool flag = false;
		while(!flag)
		{
			// Find for last node.
			temp = pMarkerHead;
			while(temp->next != NULL)
				temp = temp->next;

			// Run recurive function and flag is for notifing whether there are no more children.
			if(!LoadBVHMarker(pMK, temp, stream, &flag))
				return false;
		}
	}
	else
	{
		// End Site marker doesn't have motion data and is not initialized.
		// It has offset data and the marker name of "End Site".
		strcpy(pMK->name, "End Site");
		// Read "}".
		fgets(line, 4096, stream);
		sscanf(line, "%s", word1);
		if(strcmp(word1, "}") != 0)
			return false;
	}

	// Exit recurive function.
	*IsNoMoreChild = false;
	return true;
}

bool MotionData::SaveBVHFile(const char *filename)
{
	if(m_MotionDataType == BVH_MOTION_TYPE && pMarkerHead != NULL)
	{
		FILE *stream;

		stream = fopen(filename, "w");
		if(stream != NULL)
		{
			PathPoint pp;
			MotionMarker *pMK = pMarkerHead;
			m_pMSeq = new MarkerSeq[numMarkers];
			m_nMSeq = 0;

			while(pMK->parent != NULL)
				pMK = pMK->next;

			fprintf(stream, "HIERARCHY\n");
			SaveBVHMarker(pMK, 0, stream);
			fprintf(stream, "MOTION\n");
			fprintf(stream, "Frames:	%u\n", numFrames);
			fprintf(stream, "Frame Time:	%f\n", FrameTime);

			for(unsigned int i = 0; i < numFrames; i++)
			{
				for(unsigned int j = 0; j < numMarkers; j++)
				{
					pMK = m_pMSeq[j].pMK;
					for(unsigned int k = 0; k < pMK->numDOFs; k++)
					{
						pMK->m_cbsc[k].GetPathPoint(i, &pp);
						fprintf(stream, "%0.2f	", pp.GetAmpVal());
					}
				}
				fprintf(stream, "\n");
			}

			delete[] m_pMSeq;
			m_pMSeq = NULL;
		}
		fclose(stream);
	}
	else
		return false;

	return true;
}

bool MotionData::SaveBVHMarker(MotionMarker *pParentMK, int level, FILE *stream)
{
	MotionMarker *pMK = pMarkerHead;

	if(pParentMK->IsEndSite)
	{
		// Output End Site.
		PrintTab(level, stream);
		fprintf(stream, "End Site\n");
		
		PrintTab(level, stream);
		fprintf(stream, "{\n");
		
		PrintTab(level, stream);
		fprintf(stream, "\tOFFSET\t%0.2f\t%0.2f\t%0.2f\n", pParentMK->offset.x, pParentMK->offset.y, pParentMK->offset.z);

		PrintTab(level, stream);
		fprintf(stream, "}\n");

		m_pMSeq[m_nMSeq].pMK = pParentMK;
		m_nMSeq++;
	}
	else
	{
		// Output child joint.
		if(level == 0)
			fprintf(stream, "ROOT %s\n", pParentMK->name);
		else
		{
			PrintTab(level, stream);
			fprintf(stream, "JOINT %s\n", pParentMK->name);
		}

		PrintTab(level, stream);
		fprintf(stream, "{\n");

		PrintTab(level, stream);
		fprintf(stream, "\tOFFSET\t%0.2f\t%0.2f\t%0.2f\n", pParentMK->offset.x, pParentMK->offset.y, pParentMK->offset.z);

		PrintTab(level, stream);
		fprintf(stream, "\tCHANNELS %u", pParentMK->numDOFs);
		for(unsigned int i = 0; i < pParentMK->numDOFs; i++)
			fprintf(stream, " %s", pParentMK->CurveName[i].name);
		fprintf(stream, "\n");

		m_pMSeq[m_nMSeq].pMK = pParentMK;
		m_nMSeq++;

		while(pMK != NULL)
		{
			if(pMK->parent == pParentMK)
				SaveBVHMarker(pMK, level + 1, stream);

			pMK = pMK->next;
		}

		PrintTab(level, stream);
		fprintf(stream, "}\n");
	}

	return true;
}

bool MotionData::PrintTab(int numTabs, FILE *stream)
{
	for(int i = 0; i < numTabs; i++)
	{
		if(fprintf(stream, "\t") < 0)
			return false;
	}

	return true;
}

bool MotionData::SaveBVHFile(const char *filename, float _TimeLength, float _FrameTime)
{
	if(m_MotionDataType == BVH_MOTION_TYPE && pMarkerHead != NULL && m_IsAllMarkersFitted)
	{
		FILE *stream;

		stream = fopen(filename, "w");
		if(stream != NULL)
		{
			float var;
			MotionMarker *pMK = pMarkerHead;
			m_pMSeq = new MarkerSeq[numMarkers];
			m_nMSeq = 0;

			while(pMK->parent != NULL)
				pMK = pMK->next;

			fprintf(stream, "HIERARCHY\n");
			SaveBVHMarker(pMK, 0, stream);
			fprintf(stream, "MOTION\n");
			fprintf(stream, "Frames:	%u\n", (unsigned int)(_TimeLength / _FrameTime));
			fprintf(stream, "Frame Time:	%f\n", FrameTime);

			for(float i = 0; i < _TimeLength; i += _FrameTime)
			{
				for(unsigned int j = 0; j < numMarkers; j++)
				{
					pMK = m_pMSeq[j].pMK;
					for(unsigned int k = 0; k < pMK->numDOFs; k++)
					{
						var = pMK->m_cbsc[k].GetCurveVal(i);
						fprintf(stream, "%0.2f	", var);
					}
				}
				fprintf(stream, "\n");
			}

			delete[] m_pMSeq;
			m_pMSeq = NULL;
		}
		fclose(stream);
	}
	else
		return false;

	return true;
}

unsigned int MotionData::GetMarkerIndex(const char *MarkerName)
{
	MotionMarker *pMK;

	for(pMK = pMarkerHead; pMK != NULL; pMK = pMK->next)
	{
		if(strcmp(pMK->name, MarkerName) == 0)
			return pMK->nPosition;
	}

	return 0;
}

ControlPoint MotionData::GetControlPoint(unsigned int nMarker, unsigned int nFrame, unsigned int nDOF)
{
	ControlPoint cp;

	if(m_MotionDataType != UNKNOWN_MOTION_TYPE)
	{
		MotionMarker *pMK = GetMarker(nMarker);

		if(pMK != NULL)
			if(!pMK->IsEndSite)
				pMK->m_cbsc[nDOF].GetControlPoint(nFrame, &cp);
	}

	return cp;
}

PathPointSet * MotionData::GetPathPointSet(unsigned int nMarker, unsigned int nDOF)
{
	if(m_MotionDataType != UNKNOWN_MOTION_TYPE)
		return &GetMarker(nMarker)->pps[nDOF];
	else
		return NULL;
}

bool MotionData::RenewMaxAndMinPPAmp(unsigned int nMarker, unsigned int nDOF)
{
	GetMarker(nMarker)->m_cbsc[nDOF].FindMaxAndMinPPAmp();

	return true;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      