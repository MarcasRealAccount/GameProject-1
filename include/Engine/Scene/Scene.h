//
//	Created by MarcasRealAccount on 7. Nov. 2020.
//

#pragma once

#include "Engine/Scene/Entity.h"
#include "Engine/Utility/SmartPointers/SmartPointers.h"

#include <vector>

namespace gp1::scene
{
	class Scene
	{
	public:
		// Attach an entity to this scene.
		void AttachEntity(const smart_pointers::shared_ptr<Entity>& entity);
		// Detach an entity from this scene.
		void DetachEntity(const smart_pointers::shared_ptr<Entity>& entity);

		void Update(float deltaTime);

		// Get all entities this scene holds.
		inline const std::vector<smart_pointers::weak_ptr<Entity>>& GetEntities()
		{
			return m_Entities;
		}

	private:
		std::vector<smart_pointers::weak_ptr<Entity>> m_Entities; // The entities this scene holds.
	};
} // namespace gp1::scene
