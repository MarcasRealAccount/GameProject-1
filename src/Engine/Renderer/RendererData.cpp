//
//	Created by MarcasRealAccount on 13. May. 2021
//

#include "Engine/Renderer/RendererData.h"

#include "Engine/Renderer/Renderer.h"

namespace gp1::renderer
{
	RendererData::~RendererData()
	{
		if (m_Renderer)
			m_Renderer->RemoveRendererData(this);
	}
} // namespace gp1::renderer