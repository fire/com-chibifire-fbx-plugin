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

#include <fbxsdk.h>
#include <fbxsdk/scene/geometry/fbxnodeattribute.h>

#include <stdio.h>


using namespace godot;

String ResourceImporterFBX::get_importer_name() const
{
	return "fbx_mesh";
}

String ResourceImporterFBX::get_visible_name() const
{
	return "FBX As Mesh";
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
	Array recognized = Array();
	recognized.push_back("fbx");
	return recognized;
}

Array ResourceImporterFBX::get_import_options(const int preset) const
{
	return Array();
}

String ResourceImporterFBX::get_save_extension() const
{
	return "mesh";
}

String ResourceImporterFBX::get_resource_type() const
{
	return "ArrayMesh";
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

	// Prepare the FBX SDK.
	InitializeSdkObjects(lSdkManager, lScene);

	lResult = LoadScene(lSdkManager, lScene, "D:/Hobby/ResourceImporterFBX/sample/cube.fbx");
	const size_t len = 128;
	char str[len];

    snprintf(str, len, "FBX node count: %d", lScene->GetNodeCount());

	Godot::print(str);

	// Convert the file

	Ref<ArrayMesh> array_mesh = new ArrayMesh;

	//Ref<SurfaceTool> surf_tool = new SurfaceTool;

	//surf_tool->begin(PrimitiveType::PRIMITIVE_TRIANGLES);

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

	for (int i = 0; i < iChildNodeCount; i++)
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

		int iVertexCount = fbxMesh->GetControlPointsCount();

		if (iVertexCount > 0)
		{
			int indicesIndex = 0;

			// Get all positions
			FbxVector4* pVertices = fbxMesh->GetControlPoints();
			int iPolyCount = fbxMesh->GetPolygonCount();
			for (int j = 0; j < iPolyCount; j++)
			{
				// The poly size should be 3 since it's a triangle
				int iPolySize = fbxMesh->GetPolygonSize(j);
				// Get 3 vertices of the triangle
				for (int k = 0; k < iPolySize; ++k)
				{
					int index = fbxMesh->GetPolygonVertex(j, k);

					// Get normal
					fbxMesh->GetPolygonVertexNormal(j, k, nor);
					normals.push_back(Vector3(nor[0], nor[1], nor[2]));

					// Insert pos and nor data
					vertices.push_back(Vector3(pVertices[index].mData[0],
						pVertices[index].mData[1],
						pVertices[index].mData[2]));

					// TODO uv... but will be!
				}
			}
		}

		int* lVertices = fbxMesh->GetPolygonVertices();
		for (int j = 0; j < fbxMesh->GetPolygonCount(); ++j)
		{
			size_t start_index = fbxMesh->GetPolygonVertexIndex(j);
			int a = lVertices[start_index + 0];
			int b = lVertices[start_index + 1];
			int c = lVertices[start_index + 2];
			int d = 0;
			if (b < 0)
			{
				b = abs(b) - 1;
				d = b;
				b = c;
				c = d;
			}
			if (c < 0)
			{
				c = abs(c) - 1;
			}
			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(c);
		}

	}

	Array arrays;
	arrays.resize(ArrayType::ARRAY_MAX);
	arrays[ArrayType::ARRAY_VERTEX] = vertices;
	arrays[ArrayType::ARRAY_NORMAL] = normals;
	//arrays[ArrayType::ARRAY_TEX_UV2] = uvs;
	arrays[ArrayType::ARRAY_INDEX] = indices;

	array_mesh->add_surface_from_arrays(PrimitiveType::PRIMITIVE_TRIANGLES, arrays);

	char faces[len];

	snprintf(faces, len, "Mesh faces: %d", array_mesh->get_surface_count());

	Godot::print(faces);

	DestroySdkObjects(lSdkManager, lScene);

	// TODO SAVE WITH .mesh extension
	return ResourceSaver::save("res://main.mesh", array_mesh.ptr());
}

void ResourceImporterFBX::_register_methods() {
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
