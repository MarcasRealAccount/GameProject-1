#include "Engine/Renderer/Renderers.h"
#include "Engine/Renderer/Apis/OpenGL/OpenGLRenderer.h"
#include "Engine/Renderer/Apis/Vulkan/VulkanRenderer.h"
#include "Engine/Utility/Core.h"
#include "Engine/Utility/SmartPointers/SmartPointers.h"

namespace gp1::renderer
{
	Renderers* const Renderers::s_Renderers = new Renderers({
#ifdef RENDERER_OPENGL
	    RendererEntry("opengl", 0, []() -> smart_pointers::shared_ptr<Renderer> {
		    return smart_pointers::make_shared<opengl::OpenGLRenderer>();
	    }),
#endif
#ifdef RENDERER_VULKAN
	    RendererEntry("vulkan", 1, []() -> smart_pointers::shared_ptr<Renderer> {
		    return smart_pointers::make_shared<vulkan::VulkanRenderer>();
	    })
#endif
	});

	RendererEntry::RendererEntry(const std::string& name, uint32_t priority, Function createRenderer)
	    : m_Name(name), m_Priority(priority), m_CreateRenderer(createRenderer) {}

	Renderers::Renderers(const std::initializer_list<RendererEntry>& entries)
	    : m_Renderers(entries) {}

	smart_pointers::shared_ptr<Renderer> Renderers::GetRenderer(const std::string& name)
	{
		for (auto& entry : m_Renderers)
		{
			if (entry.m_Name == name)
			{
				if (!entry.m_Renderer.expired())
					return entry.m_Renderer.lock();

				smart_pointers::shared_ptr<Renderer> renderer = entry.m_CreateRenderer();
				if (renderer->IsCompatible())
				{
					entry.m_Renderer = renderer;
					return renderer;
				}
			}
		}
		return nullptr;
	}

	smart_pointers::shared_ptr<Renderer> Renderers::GetBestRenderer()
	{
		std::vector<RendererEntry*> bestOrder;
		bestOrder.reserve(m_Renderers.size());

		for (auto& entry : m_Renderers)
		{
			bool added        = false;
			auto bestOrderItr = bestOrder.begin();
			for (; bestOrderItr != bestOrder.end(); bestOrderItr++)
			{
				if (entry.m_Priority > (*bestOrderItr)->m_Priority)
				{
					bestOrder.insert(bestOrderItr, &entry);
					added = true;
					break;
				}
			}

			if (!added)
				bestOrder.push_back(&entry);
		}

		for (auto& entry : bestOrder)
		{
			if (!entry->m_Renderer.expired())
				return entry->m_Renderer.lock();

			smart_pointers::shared_ptr<Renderer> renderer = entry->m_CreateRenderer();
			if (renderer->IsCompatible())
			{
				entry->m_Renderer = renderer;
				return renderer;
			}
		}
		return nullptr;
	}

	std::string Renderers::GetName(const smart_pointers::shared_ptr<Renderer>& renderer) const
	{
		for (auto& entry : m_Renderers)
			if (!entry.m_Renderer.expired() && entry.m_Renderer.lock() == renderer)
				return entry.m_Name;
		return "";
	}
} // namespace gp1::renderer