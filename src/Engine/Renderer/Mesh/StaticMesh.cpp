//
//	Created by MarcasRealAccount on 13. May. 2021
//

#include "Engine/Renderer/Mesh/StaticMesh.h"
#include "Engine/Application.h"
#include "Engine/Utility/SmartPointers/SmartPointers.h"

namespace gp1::renderer
{
	smart_pointers::shared_ptr<StaticMesh> StaticMesh::Create()
	{
		return Application::GetInstance()->GetRenderer()->CreateStaticMesh();
	}
} // namespace gp1::renderer