//
//	Created by MarcasRealAccount on 18. Nov. 2020.
//

#pragma once

#include "Engine/Renderer/Mesh/StaticMesh.h"

#include <string>

namespace gp1 {

	namespace meshLoaders {

		bool LoadOBJFile(StaticMesh& mesh, const std::string& fileName);

	} // namespace meshLoaders

} // namespace gp1
