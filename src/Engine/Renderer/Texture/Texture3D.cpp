//
//	Created by MarcasRealAccount on 18. May. 2021.
//

#include "Engine/Renderer/Texture/Texture3D.h"
#include "Engine/Application.h"
#include "Engine/Utility/SmartPointers/SmartPointers.h"

namespace gp1::renderer
{
	smart_pointers::shared_ptr<Texture3D> Texture3D::Create()
	{
		return Application::GetInstance()->GetRenderer()->CreateTexture3D();
	}
} // namespace gp1::renderer