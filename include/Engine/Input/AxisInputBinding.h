//	
//	Created by MarcasRealAccount on 8. Oct. 2020
//	

#pragma once

#include "Engine/Input/IInputBinding.h"
#include <functional>

namespace gp1 {

	namespace input {

		// Callback for AxisInputBinding
		typedef std::function<void(InputLocation location, uint32_t axis, double value)> AxisCallback;

		struct AxisInputBinding : public IInputBinding {
		public:
			AxisInputBinding(InputGroup* inputGroup, std::string id, InputLocation location, uint32_t axis, AxisCallback callback);

			virtual void HandleEvent(Event& event) override;

		private:
			AxisCallback m_callback;	// The callback to call if the corrent event was passed to HandleEvent()
		};

	} // namespace input

} // namespace gp1
