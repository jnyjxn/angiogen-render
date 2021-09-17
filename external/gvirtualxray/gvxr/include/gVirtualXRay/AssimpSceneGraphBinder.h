#ifndef __AssimpSceneGraphBinder_h
#define __AssimpSceneGraphBinder_h

#include <string>

// ASSIMP to load Collada files
#ifdef HAS_ASSIMP
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#endif

#ifndef __SceneGraphBinder_h
#include "gVirtualXRay/SceneGraphBinder.h"
#endif

#ifndef __Types_h
#include "gVirtualXRay/Types.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


class AssimpSceneGraphBinder: public SceneGraphBinder
{
public:
	AssimpSceneGraphBinder();
	AssimpSceneGraphBinder(const AssimpSceneGraphBinder& aSceneGraph);
	AssimpSceneGraphBinder(const std::string& aFileName, RATIONAL_NUMBER aUnitOfLength);
	AssimpSceneGraphBinder(const char* aFileName, RATIONAL_NUMBER aUnitOfLength);
	virtual ~AssimpSceneGraphBinder();

	AssimpSceneGraphBinder& operator=(const AssimpSceneGraphBinder& aSceneGraph);


	virtual void loadSceneGraph(const char* aFileName, RATIONAL_NUMBER aUnitOfLength);


#ifdef HAS_ASSIMP
	void setSceneGraph(const aiScene* apSceneGraph, RATIONAL_NUMBER aUnitOfLength);
#endif



private:

#ifdef HAS_ASSIMP
	void recursivePolygonMeshBuild (const aiNode* nd, SceneGraphNode* apParent, aiMatrix4x4 tStack);

	Assimp::Importer m_importer;
	aiScene* m_p_scene;
#endif
};


} // namespace gVirtualXRay


#endif // __AssimpSceneGraphBinder_h
