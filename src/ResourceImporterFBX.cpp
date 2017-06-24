#include "ResourceImporterFBX.h"
#include <godot.h>

#include <core/Godot.hpp>
#include <core/GodotGlobal.hpp>

#include <ClassDB.hpp>
#include <ArrayMesh.hpp>
#include <ResourceSaver.hpp>
#include <File.hpp>
#include <Mesh.hpp>
#include <SurfaceTool.hpp>
#include <Reference.hpp>
#include <Vector3.hpp>
#include <Ref.hpp>
#include <GlobalConfig.hpp>
#include <PoolArrays.hpp>
#include <fbxsdk.h>
#include <fbxsdk/scene/geometry/fbxnodeattribute.h>

#include <stdio.h>

using namespace godot;

GODOT_NATIVE_INIT(godot_native_init_options *options) {
	register_tool_class<ResourceImporterFBX>();
}

String ResourceImporterFBX::get_importer_name() const
{
	return String("fbx_mesh");
}

String ResourceImporterFBX::get_visible_name() const
{
	return String("FBX As Mesh");
}

int ResourceImporterFBX::get_preset_count() const
{
	return 0;
}

String ResourceImporterFBX::get_preset_name(const int preset) const
{
	return String();
}

Array ResourceImporterFBX::get_recognized_extensions() const
{
	PoolStringArray recognized;
	recognized.push_back(String("fbx"));
	return recognized;
}

Array ResourceImporterFBX::get_import_options(const int preset) const
{
	PoolStringArray options;
	return options;
}

String ResourceImporterFBX::get_save_extension() const
{
	return String("mesh");
}

String ResourceImporterFBX::get_resource_type() const
{
	return String("ArrayMesh");
}

bool ResourceImporterFBX::get_option_visibility(const String option, const Dictionary options) const
{
	return true;
}

// FBX SDK code
void ResourceImporterFBX::InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene)
{
	//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
	pManager = FbxManager::Create();
	if (!pManager)
	{
		FBXSDK_printf("Error: Unable to create FBX Manager!\n");
		exit(1);
	}
	else FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	FbxString lPath = FbxGetApplicationDirectory();
	pManager->LoadPluginsDirectory(lPath.Buffer());

	//Create an FBX scene. This object holds most objects imported/exported from/to files.
	pScene = FbxScene::Create(pManager, "My Scene");
	if (!pScene)
	{
		FBXSDK_printf("Error: Unable to create FBX scene!\n");
		exit(1);
	}
}

void ResourceImporterFBX::DestroySdkObjects(FbxManager* pManager, bool pExitStatus)
{
    //Delete the FBX Manager. All the objects that have been allocated using the FBX Manager and that haven't been explicitly destroyed are also automatically destroyed.
    if( pManager ) pManager->Destroy();
	if( pExitStatus ) FBXSDK_printf("Program Success!\n");
}

// FBX SDK Code

#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(pManager->GetIOSettings()))
#endif

// FBX SDK code
bool ResourceImporterFBX::LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename)
{
	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor, lSDKMinor, lSDKRevision;
	//int lFileFormat = -1;
	int i, lAnimStackCount;
	bool lStatus;
	char lPassword[1024];

	// Get the file version number generate by the FBX SDK.
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(pManager, "");

	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(pFilename, -1, pManager->GetIOSettings());
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if (!lImportStatus)
	{
		FbxString error = lImporter->GetStatus().GetErrorString();
		FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
		FBXSDK_printf("Error returned: %s\n\n", error.Buffer());

		if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
			FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
		}

		return false;
	}

	FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);

	if (lImporter->IsFBX())
	{
		FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);

		// From this point, it is possible to access animation stack information without
		// the expense of loading the entire file.

		FBXSDK_printf("Animation Stack Information\n");

		lAnimStackCount = lImporter->GetAnimStackCount();

		FBXSDK_printf("    Number of Animation Stacks: %d\n", lAnimStackCount);
		FBXSDK_printf("    Current Animation Stack: \"%s\"\n", lImporter->GetActiveAnimStackName().Buffer());
		FBXSDK_printf("\n");

		for (i = 0; i < lAnimStackCount; i++)
		{
			FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

			FBXSDK_printf("    Animation Stack %d\n", i);
			FBXSDK_printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
			FBXSDK_printf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());

			// Change the value of the import name if the animation stack should be imported 
			// under a different name.
			FBXSDK_printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());

			// Set the value of the import state to false if the animation stack should be not
			// be imported. 
			FBXSDK_printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
			FBXSDK_printf("\n");
		}

		// Set the import states. By default, the import states are always set to 
		// true. The code below shows how to change these states.
		IOS_REF.SetBoolProp(IMP_FBX_MATERIAL, true);
		IOS_REF.SetBoolProp(IMP_FBX_TEXTURE, true);
		IOS_REF.SetBoolProp(IMP_FBX_LINK, true);
		IOS_REF.SetBoolProp(IMP_FBX_SHAPE, true);
		IOS_REF.SetBoolProp(IMP_FBX_GOBO, true);
		IOS_REF.SetBoolProp(IMP_FBX_ANIMATION, true);
		IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}

	// Import the scene.
	lStatus = lImporter->Import(pScene);

	if (lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
	{
		FBXSDK_printf("Please enter password: ");

		lPassword[0] = '\0';

		FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
			scanf("%s", lPassword);
		FBXSDK_CRT_SECURE_NO_WARNING_END

			FbxString lString(lPassword);

		IOS_REF.SetStringProp(IMP_FBX_PASSWORD, lString);
		IOS_REF.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

		lStatus = lImporter->Import(pScene);

		if (lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
		{
			FBXSDK_printf("\nPassword is wrong, import aborted.\n");
		}
	}

	// Destroy the importer.
	lImporter->Destroy();

	return lStatus;
}

int ResourceImporterFBX::import(const String p_source_file, const String p_save_path, const Dictionary p_options, const Array p_r_platform_variants, const Array p_r_gen_files)
{	
	FbxManager* lSdkManager = nullptr;
	FbxScene* lScene = nullptr;
	bool lResult;

	InitializeSdkObjects(lSdkManager, lScene);
	
	lResult = LoadScene(lSdkManager, lScene, GlobalConfig::globalize_path(p_source_file).c_string());
	const size_t len = 128;
	char str[len];
    snprintf(str, len, "FBX node count: %d", lScene->GetNodeCount());
	Godot::print(str);
	
	Ref<ArrayMesh> array_mesh = new ArrayMesh;
	
	FbxVector4 pos, nor;
	FbxVector2 uv;

	// https://gamedev.stackexchange.com/q/93935
	FbxNode* rootNode = lScene->GetRootNode();
	if (rootNode == nullptr)
	{
		return true;
	}

	int iChildNodeCount = rootNode->GetChildCount();

	PoolVector3Array vertices;
	PoolVector3Array normals;
	PoolVector2Array uvs;
	PoolIntArray indices;

	for (int i = 0; i < iChildNodeCount; ++i)
	{
		// Child Node -> Object
		FbxNode* fbxChildNode = rootNode->GetChild(i);
		FbxMesh* fbxMesh = fbxChildNode->GetMesh();
		if (!fbxMesh)
		{
			continue;
		}
		if (!fbxMesh->IsTriangleMesh())
		{
			Godot::print("FBX: Not a triangle mesh; Skipping mesh");
			continue;
		}

		char faces[len];
		snprintf(faces, len, "FBX faces: %d", fbxMesh->GetPolygonCount());
		Godot::print(faces);

		for (int j = 0; j < fbxMesh->GetControlPointsCount(); j++)
		{
			int iPolySize = fbxMesh->GetPolygonSize(j);
			FbxVector4 vertex = fbxMesh->GetControlPointAt(j);

			vertices.push_back(Vector3(static_cast<real_t>(vertex[0]),
				static_cast<real_t>(vertex[2]),
				static_cast<real_t>(vertex[1])));
		}

		bool initedNormals = 0;
		if (!initedNormals)
		{
			fbxMesh->InitNormals();
			fbxMesh->GenerateNormals(true, false, true);
		}

		FbxArray<FbxVector4> fbx_normal;

		if (fbxMesh->GetPolygonVertexNormals(fbx_normal))
		{
			for (int l = 0; l < fbx_normal.Size(); ++l)
			{
				normals.push_back(Vector3(static_cast<real_t>(fbx_normal[l].mData[0]), static_cast<real_t>(fbx_normal[l].mData[1]), static_cast<real_t>(fbx_normal[l].mData[2])));
			}
		}

		for (int j = 0; j < fbxMesh->GetPolygonCount(); ++j)
		{
			int lStartIndex = fbxMesh->GetPolygonVertexIndex(j);
			if (lStartIndex == -1)
			{
				return 1;
			}
			int* lVertices = fbxMesh->GetPolygonVertices();
			for (int i = 0; i < fbxMesh->GetPolygonSize(j); ++i)
			{
				indices.push_back(lVertices[lStartIndex + i]);
			}

			if (fbxMesh->GetUVLayerCount() == 0)
			{
				continue;
			}

			FbxLayerElementArrayTemplate<FbxVector2>* uvVertices = 0;			
			if(!fbxMesh->GetTextureUV(&uvVertices))
			{
				return 1;
			}
			for (int i = 0; i < fbxMesh->GetPolygonSize(j); ++i)
			{
				FbxVector2 uv = uvVertices->GetAt(fbxMesh->GetTextureUVIndex(j, i));
				uvs.push_back(Vector2(uv.mData[0], uv.mData[1]));
			}
		}
	}

	PoolVector3Array pool_vertices;
	PoolVector3Array pool_normals;
	// pool_colors;
	PoolVector2Array pool_uvs;
	PoolIntArray pool_indices;

	for(size_t i = 0; i < vertices.size(); ++i) {	
		pool_vertices.push_back(vertices[i]);
		//char vertex_output[len];
		//snprintf(vertex_output, len, "Vertex: %f %f %f at index %zd", pool_vertices[i].x, pool_vertices[i].y, pool_vertices[i].z, i);
		//Godot::print(vertex_output);
	}

	for (size_t i = 0; i < normals.size(); ++i) {
		pool_normals.push_back(normals[i]);
		//char normals_output[len];
		//snprintf(normals_output, len, "Normals: %f %f %f of %zd", pool_normals[i].x, pool_normals[i].y, pool_normals[i].z, i);
		//Godot::print(normals_output);
	}

	for (size_t i = 0; i < indices.size(); ++i) {
		pool_indices.push_back(indices[i]);
		//char index_output[len];
		//snprintf(index_output, len, "Index: %d at index %zd", pool_indices[i], i);
		//Godot::print(index_output);
	}

	for (size_t i = 0; i < uvs.size(); ++i) {
		pool_uvs.push_back(uvs[i]);
		//char index_output[len];
		//snprintf(index_output, len, "Uvs: %f %f at index %zd", pool_uvs[i].x, pool_uvs[i].y, i);
		//Godot::print(index_output);
	}

	Array arrays;
	arrays.resize(ArrayType::ARRAY_MAX);

	arrays[ArrayType::ARRAY_VERTEX] = pool_vertices;
	//arrays[ArrayType::ARRAY_NORMAL] = pool_normals;
	//arrays[ArrayType::ARRAY_TEX_UV2] = pool_uvs;
	arrays[ArrayType::ARRAY_INDEX] = pool_indices;

	array_mesh->add_surface_from_arrays(PrimitiveType::PRIMITIVE_TRIANGLES, arrays);

	char faces[len];

	snprintf(faces, len, "Mesh surface count: %d", array_mesh->get_surface_count());

	Godot::print(faces);

	DestroySdkObjects(lSdkManager, lScene);

	char save_output[len];
	snprintf(save_output, len, "%s%s", p_save_path.c_string(), String(".mesh").c_string());

	return ResourceSaver::save(save_output, array_mesh.ptr());
}

void ResourceImporterFBX::_register_methods() 
{
	register_method("get_importer_name", &ResourceImporterFBX::get_importer_name);
	register_method("get_visible_name", &ResourceImporterFBX::get_visible_name);
	register_method("get_preset_count", &ResourceImporterFBX::get_preset_count);
	register_method("get_preset_name", &ResourceImporterFBX::get_preset_name);
	register_method("get_recognized_extensions", &ResourceImporterFBX::get_recognized_extensions);
	register_method("get_import_options", &ResourceImporterFBX::get_import_options);
	register_method("get_save_extension", &ResourceImporterFBX::get_save_extension);
	register_method("get_resource_type", &ResourceImporterFBX::get_resource_type);
	register_method("get_option_visibility", &ResourceImporterFBX::get_option_visibility);
	register_method("import", &ResourceImporterFBX::import);
}
