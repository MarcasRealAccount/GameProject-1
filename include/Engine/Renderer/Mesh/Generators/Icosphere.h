//
//	Created by MarcasRealAccount on 16. May. 2021
//

#pragma once

#include "Engine/Renderer/Mesh/StaticMesh.h"
#include "Engine/Utility/SmartPointers/SmartPointers.h"

namespace gp1::renderer::meshGenerators
{
	void GenerateIcosphere(const smart_pointers::shared_ptr<StaticMesh>& mesh, uint32_t subdivisions);
} // namespace gp1::renderer::meshGenerators