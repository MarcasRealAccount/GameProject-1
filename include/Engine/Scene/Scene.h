//
//	Created by MarcasRealAccount on 7. Nov. 2020.
//

#pragma once

#include "Engine/Scene/Entity.h"

#include <memory>
#include <vector>

namespace gp1::scene
{
	class Scene
	{
	public:
		// Attach an entity to this scene.
		void AttachEntity(Entity* entity);
		// Detach an entity from this scene.
		void DetachEntity(Entity* entity);

		void Update(float deltaTime);

		// Get all entities this scene holds.
		inline const std::vector<Entity*>& GetEntities()
		{
			return m_Entities;
		}

	private:
		std::vector<Entity*> m_Entities; // The entities this scene holds.
	};
} // namespace gp1::scene
