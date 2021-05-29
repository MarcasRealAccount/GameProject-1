//
//	Created by MarcasRealAccount on 7. Nov. 2020.
//

#include "Engine/Scene/Scene.h"

namespace gp1::scene
{
	void Scene::AttachEntity(Entity* entity)
	{
		if (entity->m_Scene)
			entity->m_Scene->DetachEntity(entity);
		this->m_Entities.push_back(entity);
		entity->m_Scene = this;
	}

	void Scene::DetachEntity(Entity* entity)
	{
		if (entity->m_Scene != this)
			return;

		for (auto itr = m_Entities.begin(); itr != m_Entities.end();)
		{
			if (*itr == entity)
			{
				m_Entities.erase(itr);
				entity->m_Scene = nullptr;
				break;
			}
			itr++;
		}
	}

	void Scene::Update(float deltaTime)
	{
		for (auto entity : m_Entities)
			entity->Update(deltaTime);
	}
} // namespace gp1::scene
