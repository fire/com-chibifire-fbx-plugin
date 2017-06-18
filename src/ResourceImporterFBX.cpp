#include "ResourceImporterFBX.h"
#include <godot.h>

#include <core/Godot.hpp>
#include <core/GodotGlobal.hpp>

#include <ClassDB.hpp>
#include <ResourceSaver.hpp>
#include <File.hpp>
#include <Mesh.hpp>
#include <SurfaceTool.hpp>
#include <fbxsdk.h>

#include <stdio.h>

#include <Reference.hpp>

using namespace godot;

String ResourceImporterFBX::get_importer_name()
{
	return "fbx_mesh";
}

String ResourceImporterFBX::get_visible_name()
{
	return "FBX As Mesh";
}

int ResourceImporterFBX::get_preset_count()
{
	return 0;
}

String ResourceImporterFBX::get_preset_name(const int preset)
{
	return String();
}

Array ResourceImporterFBX::get_recognized_extensions()
{
	Array recognized = Array();
	recognized.push_back("fbx");
	return recognized;
}

Array ResourceImporterFBX::get_import_options(const int preset)
{
	return Array();
}

String ResourceImporterFBX::get_save_extension()
{
	return "mesh";
}

String ResourceImporterFBX::get_resource_type()
{
	return "ArrayMesh";
}

bool ResourceImporterFBX::get_option_visibility(const String option, const Dictionary options)
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

// FBX SDK Code

#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(pManager->GetIOSettings()))
#endif

// FBX SDK code
bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename)
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

	Godot::print(lScene->GetNodeCount());

	// Convert the file
	//
	//Ref<ArrayMesh> mesh = Ref<ArrayMesh>(memnew(ArrayMesh));
	//Map<String, Ref<Material> > name_map;

	//bool generate_normals = p_options["generate/normals"];
	//bool generate_tangents = p_options["generate/tangents"];
	//bool flip_faces = p_options["force/flip_faces"];
	//bool force_smooth = p_options["force/smooth_shading"];
	//bool weld_vertices = p_options["force/weld_vertices"];
	//float weld_tolerance = p_options["force/weld_tolerance"];
	//Vector<Vector3> vertices;
	//Vector<Vector3> normals;
	//Vector<Vector2> uvs;
	//String name;

	//Ref<SurfaceTool> surf_tool = memnew(SurfaceTool);
	//surf_tool->begin(Mesh::PRIMITIVE_TRIANGLES);
	//if (force_smooth)
	//	surf_tool->add_smooth_group(true);
	//int has_index_data = false;

	//while (true) {

	//	String l = f->get_line().strip_edges();

	//	if (l.begins_with("v ")) {
	//		//vertex
	//		Vector<String> v = l.split(" ", false);
	//		ERR_FAIL_COND_V(v.size() < 4, ERR_INVALID_DATA);
	//		Vector3 vtx;
	//		vtx.x = v[1].to_float();
	//		vtx.y = v[2].to_float();
	//		vtx.z = v[3].to_float();
	//		vertices.push_back(vtx);
	//	}
	//	else if (l.begins_with("vt ")) {
	//		//uv
	//		Vector<String> v = l.split(" ", false);
	//		ERR_FAIL_COND_V(v.size() < 3, ERR_INVALID_DATA);
	//		Vector2 uv;
	//		uv.x = v[1].to_float();
	//		uv.y = 1.0 - v[2].to_float();
	//		uvs.push_back(uv);

	//	}
	//	else if (l.begins_with("vn ")) {
	//		//normal
	//		Vector<String> v = l.split(" ", false);
	//		ERR_FAIL_COND_V(v.size() < 4, ERR_INVALID_DATA);
	//		Vector3 nrm;
	//		nrm.x = v[1].to_float();
	//		nrm.y = v[2].to_float();
	//		nrm.z = v[3].to_float();
	//		normals.push_back(nrm);
	//	}
	//	else if (l.begins_with("f ")) {
	//		//vertex

	//		has_index_data = true;
	//		Vector<String> v = l.split(" ", false);
	//		ERR_FAIL_COND_V(v.size() < 4, ERR_INVALID_DATA);

	//		//not very fast, could be sped up

	//		Vector<String> face[3];
	//		face[0] = v[1].split("/");
	//		face[1] = v[2].split("/");
	//		ERR_FAIL_COND_V(face[0].size() == 0, ERR_PARSE_ERROR);
	//		ERR_FAIL_COND_V(face[0].size() != face[1].size(), ERR_PARSE_ERROR);
	//		for (int i = 2; i < v.size() - 1; i++) {

	//			face[2] = v[i + 1].split("/");
	//			ERR_FAIL_COND_V(face[0].size() != face[2].size(), ERR_PARSE_ERROR);
	//			for (int j = 0; j < 3; j++) {

	//				int idx = j;

	//				if (!flip_faces && idx < 2) {
	//					idx = 1 ^ idx;
	//				}

	//				if (face[idx].size() == 3) {
	//					int norm = face[idx][2].to_int() - 1;
	//					if (norm < 0)
	//						norm += normals.size() + 1;
	//					ERR_FAIL_INDEX_V(norm, normals.size(), ERR_PARSE_ERROR);
	//					surf_tool->add_normal(normals[norm]);
	//				}

	//				if (face[idx].size() >= 2 && face[idx][1] != String()) {
	//					int uv = face[idx][1].to_int() - 1;
	//					if (uv < 0)
	//						uv += uvs.size() + 1;
	//					ERR_FAIL_INDEX_V(uv, uvs.size(), ERR_PARSE_ERROR);
	//					surf_tool->add_uv(uvs[uv]);
	//				}

	//				int vtx = face[idx][0].to_int() - 1;
	//				if (vtx < 0)
	//					vtx += vertices.size() + 1;
	//				ERR_FAIL_INDEX_V(vtx, vertices.size(), ERR_PARSE_ERROR);

	//				Vector3 vertex = vertices[vtx];
	//				if (weld_vertices)
	//					vertex = vertex.snapped(weld_tolerance);
	//				surf_tool->add_vertex(vertex);
	//			}

	//			face[1] = face[2];
	//		}
	//	}
	//	else if (l.begins_with("s ") && !force_smooth) { //smoothing
	//		String what = l.substr(2, l.length()).strip_edges();
	//		if (what == "off")
	//			surf_tool->add_smooth_group(false);
	//		else
	//			surf_tool->add_smooth_group(true);

	//	}
	//	else if (l.begins_with("o ") || f->eof_reached()) { //new surface or done

	//		if (has_index_data) {
	//			//new object/surface
	//			if (generate_normals || force_smooth)
	//				surf_tool->generate_normals();
	//			if (uvs.size() && (normals.size() || generate_normals) && generate_tangents)
	//				surf_tool->generate_tangents();

	//			surf_tool->index();
	//			mesh = surf_tool->commit(mesh);
	//			if (name == "")
	//				name = vformat(TTR("Surface %d"), mesh->get_surface_count() - 1);
	//			mesh->surface_set_name(mesh->get_surface_count() - 1, name);
	//			name = "";
	//			surf_tool->clear();
	//			surf_tool->begin(Mesh::PRIMITIVE_TRIANGLES);
	//			if (force_smooth)
	//				surf_tool->add_smooth_group(true);

	//			has_index_data = false;

	//			if (f->eof_reached())
	//				break;
	//		}

	//		if (l.begins_with("o ")) //name
	//			name = l.substr(2, l.length()).strip_edges();
	//	}
	//}

	// the whole file is now loaded in the memory buffer. 

	FILE * pSave;
	pSave = fopen("D:/Hobby/ResourceImporterFBX/sample/cube.mesh", "w");
	if (pSave != NULL)
	{
		// Save the file
		//fputs(buffer, pSave);
		fclose(pSave);
	}

	// terminate
	fclose(pSave);

	return 0;

	//File* source = (File *)(Object *)ClassDB::instance("_File");
	//source->open(p_source_file, File::READ);
	//String content = source->get_as_text();
	//source->close();

	//File* save = (File *)(Object *)ClassDB::instance("_File");
	//String save_path;
	//save_path += ".mesh";
	//save->open(save_path, File::WRITE);
	//save->store_string(content);
	//save->close();

	//return 0;

	//FileAccessRef f = File::open(p_source_file, File::READ);
	//ERR_FAIL_COND_V(!f, ERR_CANT_OPEN);

	//Ref<ArrayMesh> mesh = Ref<ArrayMesh>(memnew(ArrayMesh));
	//Map<String, Ref<Material> > name_map;

	//bool generate_normals = p_options["generate/normals"];
	//bool generate_tangents = p_options["generate/tangents"];
	//bool flip_faces = p_options["force/flip_faces"];
	//bool force_smooth = p_options["force/smooth_shading"];
	//bool weld_vertices = p_options["force/weld_vertices"];
	//float weld_tolerance = p_options["force/weld_tolerance"];
	//Vector<Vector3> vertices;
	//Vector<Vector3> normals;
	//Vector<Vector2> uvs;
	//String name;

	//Ref<SurfaceTool> surf_tool = memnew(SurfaceTool);
	//surf_tool->begin(Mesh::PRIMITIVE_TRIANGLES);
	//if (force_smooth)
	//	surf_tool->add_smooth_group(true);
	//int has_index_data = false;

	//while (true) {

	//	String l = f->get_line().strip_edges();

	//	if (l.begins_with("v ")) {
	//		//vertex
	//		Vector<String> v = l.split(" ", false);
	//		ERR_FAIL_COND_V(v.size() < 4, ERR_INVALID_DATA);
	//		Vector3 vtx;
	//		vtx.x = v[1].to_float();
	//		vtx.y = v[2].to_float();
	//		vtx.z = v[3].to_float();
	//		vertices.push_back(vtx);
	//	}
	//	else if (l.begins_with("vt ")) {
	//		//uv
	//		Vector<String> v = l.split(" ", false);
	//		ERR_FAIL_COND_V(v.size() < 3, ERR_INVALID_DATA);
	//		Vector2 uv;
	//		uv.x = v[1].to_float();
	//		uv.y = 1.0 - v[2].to_float();
	//		uvs.push_back(uv);

	//	}
	//	else if (l.begins_with("vn ")) {
	//		//normal
	//		Vector<String> v = l.split(" ", false);
	//		ERR_FAIL_COND_V(v.size() < 4, ERR_INVALID_DATA);
	//		Vector3 nrm;
	//		nrm.x = v[1].to_float();
	//		nrm.y = v[2].to_float();
	//		nrm.z = v[3].to_float();
	//		normals.push_back(nrm);
	//	}
	//	else if (l.begins_with("f ")) {
	//		//vertex

	//		has_index_data = true;
	//		Vector<String> v = l.split(" ", false);
	//		ERR_FAIL_COND_V(v.size() < 4, ERR_INVALID_DATA);

	//		//not very fast, could be sped up

	//		Vector<String> face[3];
	//		face[0] = v[1].split("/");
	//		face[1] = v[2].split("/");
	//		ERR_FAIL_COND_V(face[0].size() == 0, ERR_PARSE_ERROR);
	//		ERR_FAIL_COND_V(face[0].size() != face[1].size(), ERR_PARSE_ERROR);
	//		for (int i = 2; i < v.size() - 1; i++) {

	//			face[2] = v[i + 1].split("/");
	//			ERR_FAIL_COND_V(face[0].size() != face[2].size(), ERR_PARSE_ERROR);
	//			for (int j = 0; j < 3; j++) {

	//				int idx = j;

	//				if (!flip_faces && idx < 2) {
	//					idx = 1 ^ idx;
	//				}

	//				if (face[idx].size() == 3) {
	//					int norm = face[idx][2].to_int() - 1;
	//					if (norm < 0)
	//						norm += normals.size() + 1;
	//					ERR_FAIL_INDEX_V(norm, normals.size(), ERR_PARSE_ERROR);
	//					surf_tool->add_normal(normals[norm]);
	//				}

	//				if (face[idx].size() >= 2 && face[idx][1] != String()) {
	//					int uv = face[idx][1].to_int() - 1;
	//					if (uv < 0)
	//						uv += uvs.size() + 1;
	//					ERR_FAIL_INDEX_V(uv, uvs.size(), ERR_PARSE_ERROR);
	//					surf_tool->add_uv(uvs[uv]);
	//				}

	//				int vtx = face[idx][0].to_int() - 1;
	//				if (vtx < 0)
	//					vtx += vertices.size() + 1;
	//				ERR_FAIL_INDEX_V(vtx, vertices.size(), ERR_PARSE_ERROR);

	//				Vector3 vertex = vertices[vtx];
	//				if (weld_vertices)
	//					vertex = vertex.snapped(weld_tolerance);
	//				surf_tool->add_vertex(vertex);
	//			}

	//			face[1] = face[2];
	//		}
	//	}
	//	else if (l.begins_with("s ") && !force_smooth) { //smoothing
	//		String what = l.substr(2, l.length()).strip_edges();
	//		if (what == "off")
	//			surf_tool->add_smooth_group(false);
	//		else
	//			surf_tool->add_smooth_group(true);

	//	}
	//	else if (l.begins_with("o ") || f->eof_reached()) { //new surface or done

	//		if (has_index_data) {
	//			//new object/surface
	//			if (generate_normals || force_smooth)
	//				surf_tool->generate_normals();
	//			if (uvs.size() && (normals.size() || generate_normals) && generate_tangents)
	//				surf_tool->generate_tangents();

	//			surf_tool->index();
	//			mesh = surf_tool->commit(mesh);
	//			if (name == "")
	//				name = vformat(TTR("Surface %d"), mesh->get_surface_count() - 1);
	//			mesh->surface_set_name(mesh->get_surface_count() - 1, name);
	//			name = "";
	//			surf_tool->clear();
	//			surf_tool->begin(Mesh::PRIMITIVE_TRIANGLES);
	//			if (force_smooth)
	//				surf_tool->add_smooth_group(true);

	//			has_index_data = false;

	//			if (f->eof_reached())
	//				break;
	//		}

	//		if (l.begins_with("o ")) //name
	//			name = l.substr(2, l.length()).strip_edges();
	//	}
	//}

	/*
	TODO, check existing materials and merge?
	//re-apply materials if exist
	for(int i=0;i<mesh->get_surface_count();i++) {

	String n = mesh->surface_get_name(i);
	if (name_map.has(n))
	mesh->surface_set_material(i,name_map[n]);
	}
	*/

	//Error err = ResourceSaver::save(p_save_path + ".mesh", mesh);

	//return err;
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
