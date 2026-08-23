#pragma once
#include "Mesh.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(QuadMesh)

	class QuadMesh : public Mesh
	{
		MGSL_DISABLE_COPY(QuadMesh)
		MGSL_DISABLE_MOVE(QuadMesh)
		using Super = Mesh;

	public:
		virtual ~QuadMesh() override;
		static QuadMeshPtr Create();

	/*=================================//
	//   OpenGL object setup methods   //
	//=================================*/
	private:
		QuadMesh();
		void SetVertexAttributes();
	};
}

