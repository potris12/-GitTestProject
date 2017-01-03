#include "stdafx.h"
#include "FbxImporter.h"

bool CFbxImporter::Begin(string path){
	m_FileName = path;

	//scene importer�� ���� �Ұ� 
	m_pScene = FbxScene::Create(m_pManager, "tempName");

	m_pImporter = FbxImporter::Create(m_pManager, "");
	if (!m_pImporter->Initialize(path.c_str(), -1, m_pManager->GetIOSettings())){
		//DEBUGER->DebugMessageBox("Begin()", "FBX Import Error");
		return false;
	}
	else {
		//DEBUGER->DebugMessageBox("Begin()", "FBX Import Success");
	}
	m_pImporter->Import(m_pScene);
	m_pImporter->Destroy();
	m_pImporter = nullptr;

	//DirectX����� ������ ��ȯ
	//FbxAxisSystem OurAxisSystem = FbxAxisSystem::DirectX;
	//
	//FbxAxisSystem SceneAxisSystem = m_pScene->GetGlobalSettings().GetAxisSystem();
	//
	//if (SceneAxisSystem != OurAxisSystem){
	//	FbxAxisSystem::DirectX.ConvertScene(m_pScene);
	//}

	FbxSystemUnit SceneSystemUnit = m_pScene->GetGlobalSettings().GetSystemUnit();
	if (SceneSystemUnit.GetScaleFactor() != 1.0){
		FbxSystemUnit::cm.ConvertScene(m_pScene);
	}
	FbxGeometryConverter GeomConverter(m_pManager);
	GeomConverter.Triangulate(m_pScene, true);

	//SetModelScale(modelScale);

	LoadFile();

	return true;
}

bool CFbxImporter::End(){
	
	m_vAnimationData.clear();
	//mesh data
	m_MeshScale = 1;
	m_vMeshData.clear();
	
	m_bHasAnimationData = false;
	m_iMeshCount = 0 ;


	if (m_pImporter){
		m_pImporter->Destroy();
		m_pImporter = nullptr;
	}

	//if (m_pScene){
	//	m_pScene->Destroy();
	//	m_pScene = nullptr;
	//}

	return true;
}

UINT CFbxImporter::GetTotalJointCnt(){
	int nJoint = 0;
	if(false == m_bHasAnimationData)
		return nJoint;
	
	for (auto AnimData : m_vAnimationData) {
		nJoint += AnimData.GetJointCnt();
	}

	return nJoint;
}

void CFbxImporter::LoadFile(){
	//animation name. ������. ������ �Ⱦ�
	SetAnimationNameArray();

	//fbx ���� �б�
	LoadNodeRecursive(m_pScene->GetRootNode());

	//WriteAllData(m_FileName);
}
void CFbxImporter::SetAnimationNameArray(){
	m_pScene->FillAnimStackNameArray(m_sAnimationNames);
}
void CFbxImporter::LoadAnimStack(){

	FbxAnimStack* currAnimStack = m_pScene->GetSrcObject<FbxAnimStack>(0);
	FbxString animStackName = currAnimStack->GetName();
	m_AnimStackData.SetAnimationName(animStackName.Buffer());
	FbxTakeInfo* takeInfo = m_pScene->GetTakeInfo(animStackName);
	FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
	FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
	FbxTime::EMode timeMode = m_pScene->GetGlobalSettings().GetTimeMode();

	m_AnimStackData.SetTimeMode(timeMode);
	m_AnimStackData.SetTimeStart(start);
	m_AnimStackData.SetTimeEnd(end);
	m_AnimStackData.SetAnimationLength(end.GetFrameCount(timeMode) - start.GetFrameCount(timeMode) + 1);

	m_AnimStackData.SetpAnimStack(currAnimStack);

	//m_vAnimationData.push_back(animData);

//DEBUGER->DebugGMessageBox(L"LoadAnimStack()", L"Animation start : %d \n\
//Animation end : %d \n\
//Animation length : %d \n", 
//m_AnimStackData.GetTimeStart(), m_AnimStackData.GetTimeEnd(), m_AnimStackData.GetAnimationLength());

}

void CFbxImporter::LoadNodeRecursive(FbxNode* pNode){
	FbxNodeAttribute* pNodeAttribute = pNode->GetNodeAttribute();

	if (pNodeAttribute){
		if (pNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh){
			FbxMesh* pMesh = pNode->GetMesh();
			if (pMesh){
//				ExportMeshData(pMesh);
				if (false == ExportMeshData(pMesh))
					DEBUGER->DebugMessageBox("LoadNodeRecursive()", "FAIL");

			}//end pMesh if
		}//end if
	}//end if

	const int childCount = pNode->GetChildCount();
	for (int i = 0; i < childCount; ++i)	//Recursive
		LoadNodeRecursive(pNode->GetChild(i));
}
bool CFbxImporter::ExportMeshData(FbxMesh* pMesh)
{
	if (!pMesh->GetNode()) return false;

//	DEBUGER->DebugMessageBox("ExportMeshData()", "Start Export Mesh Data");
	CFbxMeshData MeshData;

	//name set
	//const char* pName = pMesh->GetName();
 	MeshData.SetMeshName(pMesh->GetName());

	FbxGeometryElementNormal* vertexNormal = nullptr;
	FbxGeometryElementUV* vertexUV = nullptr;
	FbxGeometryElement::EMappingMode eNormalMappingMode = FbxGeometryElement::eNone;
	FbxGeometryElement::EMappingMode eUVMappingMode = FbxGeometryElement::eNone;

	MeshData.SetHasNormal(pMesh->GetElementNormalCount() > 0);
	MeshData.SetHasUV(pMesh->GetElementUVCount() > 0);

	if (MeshData.GetHasNormal()){
		vertexNormal = pMesh->GetElementNormal(0);
		eNormalMappingMode = vertexNormal->GetMappingMode();
		if (eNormalMappingMode == FbxGeometryElement::eNone){
			MeshData.SetHasNormal(false);
		}
		if (MeshData.GetHasNormal() && eNormalMappingMode != FbxGeometryElement::eByControlPoint){
			MeshData.SetByControlPoint(false);
		}
	}
	if (MeshData.GetHasUV()){
		vertexUV = pMesh->GetElementUV(0);
		eUVMappingMode = vertexUV->GetMappingMode();
		if (eUVMappingMode == FbxGeometryElement::eNone){
			MeshData.SetHasUV(false);
		}
		if (MeshData.GetHasUV() && eUVMappingMode != FbxGeometryElement::eByControlPoint){
			MeshData.SetByControlPoint(false);
		}
	}

	const int polygonCount = pMesh->GetPolygonCount();
	int polygonVertexCount = 0;

	polygonVertexCount = pMesh->GetPolygonVertexCount();

	if (MeshData.GetByControlPoint())
		polygonVertexCount = pMesh->GetControlPointsCount();

	//DEBUGER->DebugGMessageBox(
	//	L"Export Mesh Data", 
	//	L"Polygon Count : %d \n\
	//	Polygon Vertex Count : %d", polygonCount, polygonVertexCount);

	MeshData.GetVertexDatas().reserve(polygonVertexCount);
	MeshData.GetIndexs().resize(polygonCount * 3);

	if (false == MeshData.GetByControlPoint()){
		//1. ��� controlpoint�� ��´�.
		GetControlPointData(pMesh, MeshData);

		//2. INDEX���� set
		GetIndexData(pMesh, MeshData);
		
		//3. NORMAL���� set
		GetNormalData(pMesh, vertexNormal, MeshData);
		
		//4. UV
		GetUVData(pMesh, vertexUV, MeshData);	

		//DEBUGER->DebugGMessageBox(L"ExportMesh()", L"Success\n MeshIndex : %d\n Vertex Count : %d\n Index Count : %d", m_iMeshCount, MeshData.GetVertexDatas().size(), MeshData.GetIndexCnt());

		m_vMeshData.push_back(MeshData);	
	}

	
	const bool lHasSkin = pMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
	const bool lHasDeformation = lHasSkin;
	
	m_bHasAnimationData = lHasDeformation;
	
	if (m_bHasAnimationData)
	{
		if (m_iMeshCount == 0)//animation Stack ���� ���
			LoadAnimStack();

		if (false == (ExportAnimationData(pMesh) && ReformBlendWeightPairInfo()))
			DEBUGER->DebugGMessageBox(L"ExportAnimationData()\n ReformBlendWeightPairInfo()", L"FAIL");

	}
	
	
	m_iMeshCount++;
	return true;
}

bool CFbxImporter::ExportAnimationData(FbxMesh * pMesh) {
	FbxMesh* currMesh = pMesh;
	unsigned int numOfDeformers = currMesh->GetDeformerCount();
	CFbxAnimationData AnimationData;
	AnimationData.SetGeometryTransform(GetGeometryTransformation(pMesh));

	//deformer 1���� animation���� 1����
	for (unsigned int deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
	{
		FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(currMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
		if (!currSkin) {
			continue;
		}
		//cluster �� �� mesh�� ������ �ִ� joint��
		unsigned int numOfClusters = currSkin->GetClusterCount();
		
		AnimationData.GetJointDatas().resize(numOfClusters);

//		UINT currMeshJointIndexOffset = GetCurrMeshJointIndexOffset();
		//for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
		for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
		{
			FbxCluster* currCluster = currSkin->GetCluster(clusterIndex);
			std::string currJointName = currCluster->GetLink()->GetName();
			//���ο� joint index ���ϱ�
			//UINT currJointIndex = clusterIndex + currMeshJointIndexOffset;
			UINT currJointIndex = clusterIndex;

			// ���� mesh�� ���� Joint�� ��� ������ �޴� ���鿡 ����
			//BlendWeightPair�� ����
			unsigned int numOfIndices = currCluster->GetControlPointIndicesCount();
			for (unsigned int i = 0; i < numOfIndices; ++i){
				CFbxBlendWeightPair currBlendingIndexWeightPair{ currJointIndex , currCluster->GetControlPointWeights()[i] };
				m_vMeshData[m_iMeshCount].GetVertexDatas()[currCluster->GetControlPointIndices()[i]].GetBlendWeightPairs().push_back(currBlendingIndexWeightPair);
			}
			
			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;
			FbxAMatrix globalBindposeInverseMatrix;

			currCluster->GetTransformMatrix(transformMatrix);	// The transformation of the mesh at binding time
			currCluster->GetTransformLinkMatrix(transformLinkMatrix);	// The transformation of the cluster(joint) at binding time from joint space to world space
			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix; //* AnimationData.GetGeometryTransform();

			globalBindposeInverseMatrix.SetT(
				FbxVector4(globalBindposeInverseMatrix.GetT().mData[0] * m_MeshScale,
					globalBindposeInverseMatrix.GetT().mData[1] * m_MeshScale,
					globalBindposeInverseMatrix.GetT().mData[2] * m_MeshScale));


			// ���� Joint�� Offset ��� ���ϱ� 
			AnimationData.GetJointDatas()[clusterIndex].SetOffsetMtx(globalBindposeInverseMatrix);

			for (FbxLongLong i = m_AnimStackData.GetTimeStart(); i <= m_AnimStackData.GetTimeEnd(); ++i) {
				FbxTime currTime;
				currTime.SetFrame(i, m_AnimStackData.GetTimeMode());
				FbxAMatrix currentTransformOffset = pMesh->GetNode()->EvaluateGlobalTransform(currTime);// *AnimationData.GetGeometryTransform();
				FbxAMatrix mGlobalTransform = currentTransformOffset.Inverse() * currCluster->GetLink()->EvaluateGlobalTransform(currTime);

				mGlobalTransform.SetT(
					FbxVector4(mGlobalTransform.GetT().mData[0] * m_MeshScale,
						mGlobalTransform.GetT().mData[1] * m_MeshScale,
						mGlobalTransform.GetT().mData[2] * m_MeshScale));


				CKeyFrame KeyFrame{ (double)i, mGlobalTransform };//�̰� ������ Ÿ��
				
				AnimationData.GetJointDatas()[clusterIndex].GetKeyFrames().push_back(KeyFrame);
			}
			
		}
	}

	//BlendWeightPair�� ���� 4���� joint�� ������ ä���ش�. 0����
	CFbxBlendWeightPair currBlendingIndexWeightPair{ 0,0 };
	for (auto itr = m_vMeshData[m_iMeshCount].GetVertexDatas().begin(); itr != m_vMeshData[m_iMeshCount].GetVertexDatas().end(); ++itr){
		for (size_t i = itr->GetBlendWeightPairs().size(); i <= 4; ++i){
			itr->GetBlendWeightPairs().push_back(currBlendingIndexWeightPair);
		}
	} 

	m_vAnimationData.push_back(AnimationData);
	return true;
}

bool CFbxImporter::ReformBlendWeightPairInfo() {

	for (auto& Data : m_vMeshData[m_iMeshCount].GetVertexDatas()) {
		int boneIndices[4] = { 0, 0, 0, 0 };
		float boneWeights[4] = { 0.0, 0.0, 0.0, 0.0 };
		if (Data.GetBlendWeightPairs().size() > 4){
			//�������� 
			sort(Data.GetBlendWeightPairs().begin(), Data.GetBlendWeightPairs().end(), [](CFbxBlendWeightPair& lhs, CFbxBlendWeightPair& rhs) {
				return lhs.GetWeight() > rhs.GetWeight();
			});

			//nomalize
			float sumWeight = 0.0f, interpolateWeight = 0.0f;
			for (int i = 0; i < 4; ++i) {
				sumWeight += static_cast<float>(Data.GetBlendWeightPairs()[i].GetWeight());
			}

			interpolateWeight = 1.0f - sumWeight;
			//nomalize

			//3.
			auto removeIter = Data.GetBlendWeightPairs().begin() + 4;
			Data.GetBlendWeightPairs().erase(removeIter, Data.GetBlendWeightPairs().end());
			//4.
			Data.GetBlendWeightPairs()[0].SetWeight(Data.GetBlendWeightPairs()[0].GetWeight() + interpolateWeight);
		}
	}
	return true;
}

UINT CFbxImporter::GetCurrMeshJointIndexOffset(){
	int offset = 0;
	for (int i = 0; i < m_iMeshCount; ++i) {
		offset += m_vAnimationData[i].GetJointCnt();
	}
	return offset;
}

FbxAMatrix CFbxImporter::GetGeometryTransformation(FbxMesh* pMesh)
{
	FbxNode* pNode = pMesh->GetNode();
	if (!pNode)
	{
		throw std::exception("Null for mesh geometry");
	}

	const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(lT, lR, lS);
}

void CFbxImporter::GetControlPointData(FbxMesh * pMesh, CFbxMeshData & MeshData){
	int nVertices = pMesh->GetControlPointsCount();
	XMFLOAT3* pVertices = new XMFLOAT3[nVertices];
	//mesh data resize
	MeshData.GetVertexDatas().resize(nVertices);

	for (int j = 0; j < nVertices; ++j) {
		pVertices[j].x = m_MeshScale * static_cast<float>(pMesh->GetControlPointAt(j).mData[0]);
		pVertices[j].y = m_MeshScale * static_cast<float>(pMesh->GetControlPointAt(j).mData[1]);
		pVertices[j].z = m_MeshScale * static_cast<float>(pMesh->GetControlPointAt(j).mData[2]);
		
		MeshData.GetVertexDatas()[j].SetPosition(pVertices[j]);
	}//end for
	delete[] pVertices;
}
void CFbxImporter::GetIndexData(FbxMesh * pMesh, CFbxMeshData & MeshData){
	int nIndices = pMesh->GetPolygonVertexCount();

	UINT* pIndices = new UINT[nIndices];
	pIndices = (UINT*)pMesh->GetPolygonVertices();
	for (int j = 0; j < nIndices; ++j) {
		MeshData.GetIndexs()[j] = pIndices[j];
	}//end for
	 //INDEX

	 //�����ϴ� �༮�� release
	delete[] pIndices;

}
void CFbxImporter::GetNormalData(FbxMesh * pMesh, FbxGeometryElementNormal* vertexNormal, CFbxMeshData & MeshData){
	vector<XMFLOAT3> vNormals;
	int lIndexByPolygonVertex = 0;
	for (int lPolygonIndex = 0; lPolygonIndex < pMesh->GetPolygonCount(); lPolygonIndex++){
		int lPolygonSize = pMesh->GetPolygonSize(lPolygonIndex);
		for (int i = 0; i < lPolygonSize; i++){
			int lNormalIndex = 0;
			if (vertexNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
				lNormalIndex = lIndexByPolygonVertex;

			if (vertexNormal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
				lNormalIndex = vertexNormal->GetIndexArray().GetAt(lIndexByPolygonVertex);

			FbxVector4 lNormal = vertexNormal->GetDirectArray().GetAt(lNormalIndex);
			vNormals.push_back(XMFLOAT3{ static_cast<float>(lNormal[0]), static_cast<float>(lNormal[1]), static_cast<float>(lNormal[2]) });

			lIndexByPolygonVertex++;
		}//end for i //lPolygonSize
	}//end for lPolygonIndex //PolygonCount
	if (vNormals.size() == MeshData.GetVertexCnt()) {
		for (int i = 0; i < vNormals.size(); ++i) {
			MeshData.GetVertexDatas()[i].SetNormal(vNormals[i]);
		}
	}
	else {
		for (int i = 0; i < vNormals.size(); ++i) {
			MeshData.GetVertexDatas()[MeshData.GetIndexs()[i]].SetNormal(vNormals[i]);
		}
	}
	vNormals.clear();
}
void CFbxImporter::GetUVData(FbxMesh * pMesh, FbxGeometryElementUV* vertexUV, CFbxMeshData & MeshData){
	for (int lUVSetIndex = 0; lUVSetIndex < 1; lUVSetIndex++) {
		//uv ������ ä�� ����
		vector<XMFLOAT2> vUV;
		if (!vertexUV)
			continue;
		//index�� ����ϴ��� �˻�.
		// only support mapping mode eByPolygonVertex and eByControlPoint
		if (vertexUV->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
			vertexUV->GetMappingMode() != FbxGeometryElement::eByControlPoint)
			return;

		 
		FbxStringList UVNames;
		pMesh->GetUVSetNames(UVNames);

		const bool lUseIndex = vertexUV->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int lIndexCount = (lUseIndex) ? vertexUV->GetIndexArray().GetCount() : MeshData.GetVertexCnt();
		
		int maxIndex{ 0 };
		//int vertexUVIndex{ 0 };
		const int lPolyCount = pMesh->GetPolygonCount();
		int lPolyIndexCounter = 0;
		
		for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex) {
			const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
			for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
			{
				FbxVector2 lUVValue;
				int lUVIndex = lUseIndex ? vertexUV->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;

				if (maxIndex < lUVIndex) {
					maxIndex = lUVIndex;
				}

				lUVValue = vertexUV->GetDirectArray().GetAt(lUVIndex);
				vUV.push_back(XMFLOAT2(static_cast<float>(lUVValue.mData[0]), static_cast<float>(1 - lUVValue.mData[1])));
				lPolyIndexCounter++;
			}
		}
		
		
		//uv data set
		if (vUV.size() == MeshData.GetVertexCnt()) {
			for (int i = 0; i < vUV.size(); ++i) {
				MeshData.GetVertexDatas()[i].SetUV(vUV[i]);
			}
		}
		else {
			for (int i = 0; i < vUV.size(); ++i) {
				MeshData.GetVertexDatas()[MeshData.GetIndexs()[i]].SetUV(vUV[i]);
			}
		}
		vUV.clear();
	}
}
CFbxImporter::CFbxImporter():CSingleTonBase<CFbxImporter>("fbximportersingleton") {

	m_pManager = FbxManager::Create();//1

	//if (!m_pManager)
	//{
	//	DEBUGER->DebugMessageBox("Create()", "FBX Manager Create Fail");
	//	exit(1);
	//}
	//else {
	//	DEBUGER->DebugMessageBox("Create()", "FBX Manager Create Success");
	//}

	m_pIoSettings = FbxIOSettings::Create(m_pManager, IOSROOT);//2.
}

CFbxImporter::~CFbxImporter(){

	if (m_pManager){
		m_pManager->Destroy();
		m_pManager = nullptr;
	}
	if (m_pIoSettings) {
		m_pIoSettings->Destroy();
		m_pIoSettings = nullptr;
	}
	if (m_pImporter) {
		m_pImporter->Destroy();
		m_pImporter = nullptr;
	}

	if (m_pScene) {
		m_pScene->Destroy();
		m_pScene = nullptr;
	}
}
