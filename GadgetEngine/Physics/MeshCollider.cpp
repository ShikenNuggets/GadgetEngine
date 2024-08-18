#include "MeshCollider.h"

#include "App.h"
#include "Graphics/Mesh.h"

using namespace Gadget;

MeshCollider::MeshCollider(GameObject* parent_, StringID modelName_, ColliderShape convexOrConcave_, bool isTrigger_) : Collider(SID("MeshCollider"), parent_, convexOrConcave_, isTrigger_), modelName(modelName_){
	GADGET_BASIC_ASSERT(parent != nullptr);
	GADGET_BASIC_ASSERT(modelName != StringID::None);
	GADGET_BASIC_ASSERT(shape == ColliderShape::ConcaveMesh || shape == ColliderShape::ConvexMesh);
	GADGET_BASIC_ASSERT(isTrigger == true || isTrigger == false);

	RecalculateCollider();
}

MeshCollider::MeshCollider(GUID parentGuid_, StringID modelName_, ColliderShape convexOrConcave_, bool isTrigger_) : Collider(SID("MeshCollider"), parentGuid_, convexOrConcave_, isTrigger_), modelName(modelName_){
	GADGET_BASIC_ASSERT(parent != nullptr);
	GADGET_BASIC_ASSERT(modelName != StringID::None);
	GADGET_BASIC_ASSERT(shape == ColliderShape::ConcaveMesh || shape == ColliderShape::ConvexMesh);
	GADGET_BASIC_ASSERT(isTrigger == true || isTrigger == false);

	RecalculateCollider();
}

MeshCollider::MeshCollider(const ComponentProperties& props_) : Collider(props_, ColliderShape::ConvexMesh){
	MeshCollider::Deserialize(props_);

	GADGET_BASIC_ASSERT(parent != nullptr);
	GADGET_BASIC_ASSERT(modelName != StringID::None);
	GADGET_BASIC_ASSERT(shape == ColliderShape::ConcaveMesh || shape == ColliderShape::ConvexMesh);
	GADGET_BASIC_ASSERT(isTrigger == true || isTrigger == false);

	RecalculateCollider();
}

//TODO - The ownership semantics here are *really weird*
btCollisionShape* MeshCollider::CreateCollisionShape() const{
	btCompoundShape* shape = new btCompoundShape(true, triMeshes.Size());
	for(auto* mesh : triMeshes){
		shape->addChildShape(btTransform(btQuaternion::getIdentity()), new btBvhTriangleMeshShape(mesh, false));
	}

	return shape;
}

void MeshCollider::RecalculateCollider(){
	Mesh* mesh = App::GetResourceManager().LoadResource<Mesh>(modelName);
	GADGET_BASIC_ASSERT(mesh != nullptr);
	if(mesh == nullptr){
		Debug::ThrowFatalError(SID("PHYSICS"), "", ErrorCode::Invalid_Args, __FILE__, __LINE__);
	}

	for(const auto& subMesh : mesh->submeshes){
		btTriangleMesh* triMesh = new btTriangleMesh();

		for(size_t i = 0; i < subMesh.indices.size(); i += 3){
			const Vertex& v0 = subMesh.vertices[subMesh.indices[i]];
			const Vertex& v1 = subMesh.vertices[subMesh.indices[i + 1]];
			const Vertex& v2 = subMesh.vertices[subMesh.indices[i + 2]];

			triMesh->addTriangle(BulletHelper::ConvertVector3(v0.position), BulletHelper::ConvertVector3(v1.position), BulletHelper::ConvertVector3(v2.position));
		}

		triMeshes.Add(triMesh);
	}

	App::GetResourceManager().UnloadResource(modelName);
}

void MeshCollider::SetModel(StringID newModel_){
	modelName = newModel_;
	RecalculateCollider();
}

ComponentProperties MeshCollider::Serialize() const{
	GADGET_ASSERT_NOT_IMPLEMENTED;
	return ComponentProperties(SID("MeshCollider"), GetGUID(), parent->GetGUID());
}

void MeshCollider::Deserialize(const ComponentProperties& props_){
	GADGET_ASSERT_NOT_IMPLEMENTED
}