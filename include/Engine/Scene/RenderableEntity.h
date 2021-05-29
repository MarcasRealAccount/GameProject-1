//
//	Created by MarcasRealAccount on 16. May. 2021
//

#pragma once

#include "Engine/Renderer/Material/Material.h"
#include "Engine/Renderer/Mesh/Mesh.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Utility/SmartPointers/SmartPointers.h"

namespace gp1::scene
{
	class RenderableEntity : public Entity
	{
	public:
		inline virtual bool IsRenderable() const override
		{
			return true;
		}

		inline virtual smart_pointers::shared_ptr<renderer::Mesh>     GetMesh() const     = 0;
		inline virtual smart_pointers::shared_ptr<renderer::Material> GetMaterial() const = 0;
	};
} // namespace gp1::scene