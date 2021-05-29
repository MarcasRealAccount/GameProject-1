//
//	Created by MarcasRealAccount on 13. May. 2021
//
//	Implements a system for storing all compiled renderers.
//

#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace gp1::renderer
{
	class Renderer;

	struct RendererEntry
	{
	public:
		using Function = std::function<Renderer*()>;

		RendererEntry(const std::string& name, uint32_t priority, Function createRenderer);

	public:
		std::string m_Name;
		uint32_t    m_Priority;
		Function    m_CreateRenderer;
		Renderer*   m_Renderer = nullptr;
	};

	class Renderers
	{
	public:
		static Renderers* const s_Renderers;

		inline const std::vector<RendererEntry>& GetEntries() const
		{
			return m_Renderers;
		}

		Renderer* GetRenderer(const std::string& name);
		Renderer* GetBestRenderer();
		void      DestroyRenderer(Renderer* renderer);

		std::string GetName(Renderer* renderer) const;

	private:
		Renderers(const std::initializer_list<RendererEntry>& entries);

		std::vector<RendererEntry> m_Renderers;
	};
} // namespace gp1::renderer