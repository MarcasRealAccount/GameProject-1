#include "Engine/Renderer/Renderers.h"
#include "Engine/Renderer/Apis/OpenGL/OpenGLRenderer.h"
#include "Engine/Renderer/Apis/Vulkan/VulkanRenderer.h"
#include "Engine/Utility/Core.h"

namespace gp1::renderer
{
	Renderers* const Renderers::s_Renderers = new Renderers({
#ifdef RENDERER_OPENGL
	    RendererEntry("opengl", 0, []() -> Renderer* {
		    return new opengl::OpenGLRenderer();
	    }),
#endif
#ifdef RENDERER_VULKAN
	    RendererEntry("vulkan", 1, []() -> Renderer* {
		    return new vulkan::VulkanRenderer();
	    })
#endif
	});

	RendererEntry::RendererEntry(const std::string& name, uint32_t priority, Function createRenderer)
	    : m_Name(name), m_Priority(priority), m_CreateRenderer(createRenderer) {}

	Renderers::Renderers(const std::initializer_list<RendererEntry>& entries)
	    : m_Renderers(entries) {}

	Renderer* Renderers::GetRenderer(const std::string& name)
	{
		for (auto& entry : m_Renderers)
		{
			if (entry.m_Name == name)
			{
				if (entry.m_Renderer)
					return entry.m_Renderer;

				Renderer* renderer = entry.m_CreateRenderer();
				if (renderer->IsCompatible())
				{
					entry.m_Renderer = renderer;
					return renderer;
				}
			}
		}
		return nullptr;
	}

	Renderer* Renderers::GetBestRenderer()
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
			if (entry->m_Renderer)
				return entry->m_Renderer;

			Renderer* renderer = entry->m_CreateRenderer();
			if (renderer->IsCompatible())
			{
				entry->m_Renderer = renderer;
				return renderer;
			}
		}
		return nullptr;
	}

	void Renderers::DestroyRenderer(Renderer* renderer)
	{
		if (renderer)
		{
			for (auto& entry : m_Renderers)
			{
				if (entry.m_Renderer == renderer)
				{
					entry.m_Renderer = nullptr;
					delete renderer;
				}
			}
		}
	}

	std::string Renderers::GetName(Renderer* renderer) const
	{
		for (auto& entry : m_Renderers)
			if (entry.m_Renderer == renderer)
				return entry.m_Name;
		return "";
	}
} // namespace gp1::renderer