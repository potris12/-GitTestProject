#include "stdafx.h"
#include "UseFBXMesh.h"

static bool gVerbose = true;

bool CUseFBXMesh::Begin(UINT index) {
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_nIndex = index;

	CreateVertexBuffer();
	CreateIndexBuffer();
	//create vertex buffer
	return true;
}

bool CUseFBXMesh::End() {

	return CMesh::End();
}

bool CUseFBXMesh::CreateVertexBuffer() {
	m_nMeshCnt = FBXIMPORTER->GetMeshCnt();

	//1. ��ü ������ ���Ѵ�.
	m_nVertices = FBXIMPORTER->GetMeshDatas()[m_nIndex].GetVertexCnt();
	
	m_pVertices = new XMFLOAT3[m_nVertices];
	m_pNormals = new XMFLOAT3[m_nVertices];
	m_pUVs = new XMFLOAT2[m_nVertices];
	//������ ���� �ε���
	int nVertex{ 0 };
	
	//mesh�� ��� ������ ���ؼ�
	for (UINT j = 0; j < FBXIMPORTER->GetMeshDatas()[m_nIndex].GetVertexCnt(); ++j) {
		//���� ���� ���´�.
		m_pVertices[nVertex] = FBXIMPORTER->GetMeshDatas()[m_nIndex].GetVertexDatas()[j].GetPosition();
		m_pNormals[nVertex] = FBXIMPORTER->GetMeshDatas()[m_nIndex].GetVertexDatas()[j].GetNormal();
		m_pUVs[nVertex++] = FBXIMPORTER->GetMeshDatas()[m_nIndex].GetVertexDatas()[j].GetUV();
	}
	//create space mesh aabb
	BoundingBox::CreateFromPoints(m_BoundingBox, (size_t)m_nVertices, m_pVertices, (size_t)sizeof(XMFLOAT3));


	m_pd3dPositionBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pVertices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dNormalBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dUVBuffer = CreateBuffer(sizeof(XMFLOAT2), m_nVertices, m_pUVs, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	//-----------------------------------���� ����-----------------------------------
	ID3D11Buffer *pd3dBuffers[3] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dUVBuffer };
	UINT pnBufferStrides[3] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[3] = { 0, 0, 0 };
	AssembleToVertexBuffer(3, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	//-----------------------------------���� ����-----------------------------------
	
	if (m_ppd3dVertexBuffers)	return true;

	return false;
}
bool CUseFBXMesh::CreateIndexBuffer() {
	//�߿��߿� offset�� �� ���� �ؾ��Ѵ�. 
	//ó�� mesh�� �ε����� �Ǿ��ִٸ� ��� mesh�� index�� �Ǿ��ִٰ� �����Ѵ�.
	if (!FBXIMPORTER->GetMeshDatas()[0].GetByControlPoint()) {
		//1. ��� �ε��� ���� ����
		
		m_nIndices = FBXIMPORTER->GetMeshDatas()[m_nIndex].GetIndexCnt();
		
		//2. �Ҵ�
		m_pnIndices = new UINT[m_nIndices];

		//3. offset�� ���ذ��� index������ �ϼ�
		//������ �ε��� 
//		int nIndex{ 0 };
		int offset{ 0 };
		for (UINT j = 0; j < m_nIndices; ++j) {
			//mesh�� index�� 0�� �ƴ� ��� offset�� ���ذ���.
			//offset�� ���� index�� �����Ѵ�. offest�� 0��mesh�� 0/ 1�� mesh�� 0�� mesh�� ���� ����ŭ offset�� ������.
			m_pnIndices[j] = FBXIMPORTER->GetMeshDatas()[m_nIndex].GetIndexs()[j] + offset;
		}

		m_pd3dIndexBuffer = CreateBuffer(sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	}
	return true;

	////�ε���
	//FbxNode *pChildNode = nullptr;
	////��� child �� ��ŭ ������ ���鼭
	//for (int i = 0; i < m_pRootNode->GetChildCount(); i++) {
	//
	//	pChildNode = m_pRootNode->GetChild(i);
	//	//mesh�� ���´�.
	//	FbxMesh *pChildMesh = pChildNode->GetMesh();
	//	if (pChildMesh != nullptr) {
	//
	//		m_nIndices = pChildMesh->GetPolygonVertexCount();
	//		m_pnIndices = new UINT[m_nIndices];
	//		m_pnIndices = (UINT*)pChildMesh->GetPolygonVertices();
	//	}
	//}
	//
	//m_pd3dIndexBuffer = CreateBuffer(sizeof(int), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	//
	//if (m_pd3dIndexBuffer) return true;
	//
	//return false;
}


void CUseFBXMesh::ProcessGetVertexBuffer(FbxNode * pNode){
	if (!pNode) return;

	FbxMesh* pMesh = pNode->GetMesh();
	GetPositionData(pMesh);
	GetNormalData(pMesh);
	GetUVData(pMesh);
	int i, lCount = pNode->GetChildCount();
	for (i = 0; i < lCount; i++) {
		ProcessGetVertexBuffer(pNode->GetChild(i));
	}

	
}
void CUseFBXMesh::GetPositionData(FbxMesh* pMesh){
	if (pMesh) {
		//get index
		int nIndices = pMesh->GetPolygonVertexCount();
		m_pnIndices = new UINT[nIndices];
		m_pnIndices = (UINT*)pMesh->GetPolygonVertices();
		//get vertex
		int nVertices = pMesh->GetControlPointsCount();
		XMFLOAT3* pVertices = new XMFLOAT3[nVertices];
		
		//��� ���� �� ��ŭ
		for (int j = 0; j < nVertices; ++j) {
			pVertices[j].x = static_cast<float>(pMesh->GetControlPointAt(j).mData[0]);
			pVertices[j].y = static_cast<float>(pMesh->GetControlPointAt(j).mData[1]);
			pVertices[j].z = static_cast<float>(pMesh->GetControlPointAt(j).mData[2]);
		}//end for

		 //����
		m_nVertices = nIndices;
		m_pVertices = new XMFLOAT3[nIndices];
		//��� ���� �� �ε����� ���� �ε��� ������ ������ �� �����
		for (int i = 0; i < nIndices; ++i) {
			//�ε��� ������� ���ؽ����� ���ġ + Ȯ��
			m_pVertices[i] = pVertices[m_pnIndices[i]];
		}

		//create space mesh aabb
		BoundingBox::CreateFromPoints(m_BoundingBox, (size_t)nVertices, pVertices, (size_t)sizeof(XMFLOAT3));

		//�����ϴ� �༮�� release
		//delete[] m_pnIndices;
		delete[] pVertices;

	}//end if lMesh

}

//get mesh normals info
void CUseFBXMesh::GetNormalData(FbxMesh* pMesh)
{
	//get mesh
	if (pMesh)
	{
		vector<XMFLOAT3> vNormals;

		//get the normal element
		FbxGeometryElementNormal* lNormalElement = pMesh->GetElementNormal();
		if (lNormalElement)
		{
			//mapping mode is by control points. The mesh should be smooth and soft.
			//we can get normals by retrieving each control point
			if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				//Let's get normals of each vertex, since the mapping mode of normal element is by control point
				for (int lVertexIndex = 0; lVertexIndex < pMesh->GetControlPointsCount(); lVertexIndex++)
				{
					int lNormalIndex = 0;
					//reference mode is direct, the normal index is same as vertex index.
					//get normals by the index of control vertex
					if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
						lNormalIndex = lVertexIndex;

					//reference mode is index-to-direct, get normals by the index-to-direct
					if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
						lNormalIndex = lNormalElement->GetIndexArray().GetAt(lVertexIndex);

					//Got normals of each vertex.
					FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
					if (gVerbose) FBXSDK_printf("normals for vertex[%d]: %f %f %f %f \n", lVertexIndex, lNormal[0], lNormal[1], lNormal[2], lNormal[3]);
					vNormals.push_back(XMFLOAT3{ (float)(lNormal[0]), (float)(lNormal[1]), (float)(lNormal[2]) });
					//add your custom code here, to output normals or get them into a list, such as KArrayTemplate<FbxVector4>
					//. . .
				}//end for lVertexIndex
			}//end eByControlPoint
			 //mapping mode is by polygon-vertex.
			 //we can get normals by retrieving polygon-vertex.
			else if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				//�̰����� ���´�.
				int lIndexByPolygonVertex = 0;
				//Let's get normals of each polygon, since the mapping mode of normal element is by polygon-vertex.
				for (int lPolygonIndex = 0; lPolygonIndex < pMesh->GetPolygonCount(); lPolygonIndex++)
				{
					//get polygon size, you know how many vertices in current polygon.
					int lPolygonSize = pMesh->GetPolygonSize(lPolygonIndex);
					//retrieve each vertex of current polygon.
					for (int i = 0; i < lPolygonSize; i++)
					{
						int lNormalIndex = 0;
						//reference mode is direct, the normal index is same as lIndexByPolygonVertex.
						if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
							lNormalIndex = lIndexByPolygonVertex;

						//reference mode is index-to-direct, get normals by the index-to-direct
						if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
							lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndexByPolygonVertex);

						//Got normals of each polygon-vertex.
						FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
						if (gVerbose) FBXSDK_printf("normals for polygon[%d]vertex[%d]: %f %f %f %f \n",
							lPolygonIndex, i, lNormal[0], lNormal[1], lNormal[2], lNormal[3]);
						vNormals.push_back(XMFLOAT3{ static_cast<float>(lNormal[0]), static_cast<float>(lNormal[1]), static_cast<float>(lNormal[2]) });
						//add your custom code here, to output normals or get them into a list, such as KArrayTemplate<FbxVector4>
						//. . .

						lIndexByPolygonVertex++;
					}//end for i //lPolygonSize
				}//end for lPolygonIndex //PolygonCount

			}//end eByPolygonVertex
		}//end if lNormalElement
		//Let's get normals of each vertex, since the mapping mode of normal element is by control point
			 //normal buffer data ����
		m_nNormals = static_cast<int>(vNormals.size());
		m_pNormals = new XMFLOAT3[m_nNormals];
		for (int i = 0; i < m_nNormals; ++i) {
			m_pNormals[i] = vNormals[i];
		}
	}
}


void CUseFBXMesh::GetUVData(FbxMesh* pMesh){
	
	if (pMesh)
	{
		FbxStringList lUVSetNameList;
		pMesh->GetUVSetNames(lUVSetNameList);

		//iterating over all uv sets
		//for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
		//only use defuse uv
		for (int lUVSetIndex = 0; lUVSetIndex < 1; lUVSetIndex++){
			//uv ������ ä�� ����
			vector<XMFLOAT2> vUV;
			//UV���� ������ �ִ� fbx��ü ��������
			const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
			const FbxGeometryElementUV* lUVElement = pMesh->GetElementUV(lUVSetName);
			if (!lUVElement)
				continue;

			//���������� ������ �ָ� ������.
			if (lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
				lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
				return;

			//index array, where holds the index referenced to the uv data
			//index�� ����ϴ��� �˻�.
			const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
			const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

			int maxIndex{ 0 };

			//iterating through the data by polygon
			const int lPolyCount = pMesh->GetPolygonCount();

			if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
				{
					// build the max index array that we need to pass into MakePoly
					const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
					for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
					{
						FbxVector2 lUVValue;

						//get the index of the current vertex in control points array
						int lPolyVertIndex = pMesh->GetPolygonVertex(lPolyIndex, lVertIndex);

						//the UV index depends on the reference mode
						int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;

						if (maxIndex < lUVIndex) {
							maxIndex = lUVIndex;
						}

						lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
						vUV.push_back(XMFLOAT2(static_cast<float>(lUVValue.mData[0]), static_cast<float>(lUVValue.mData[1])));
					}
				}
			}
			else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				//����� ���´�. 
				int lPolyIndexCounter = 0;
				for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
				{
					// build the max index array that we need to pass into MakePoly
					const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
					for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
					{
						if (lPolyIndexCounter < lIndexCount)
						{
							FbxVector2 lUVValue;

							//the UV index depends on the reference mode
							int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;

							if (maxIndex < lUVIndex) {
								maxIndex = lUVIndex;
							}

							lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
							vUV.push_back(XMFLOAT2(static_cast<float>(lUVValue.mData[0]), static_cast<float>(lUVValue.mData[1])));
							lPolyIndexCounter++;
						}
					}
				}
			}
			m_nUVs = static_cast<int>(vUV.size());
			m_pUVs = new XMFLOAT2[m_nUVs];
			for (int i = 0; i < m_nUVs; ++i) {
				m_pUVs[i] = vUV[i];
			}
			
		}
	}

}

CUseFBXMesh::CUseFBXMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CMesh(pd3dDevice, pd3dDeviceContext) {

}
CUseFBXMesh::~CUseFBXMesh(){

}