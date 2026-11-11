#pragma once
#include "QwEpicChannel.h"
#include "QwAtomicEpicTypes.h"
#include "QwObserver.h"

namespace BMW
{
	class MasterSwitch : public EPICSObserver<int>
	{
		static char const* EPIC_PV;
		static constexpr int ENABLE  = 1;
		static constexpr int DISABLE = 0;

		EpicChannel* channel_{nullptr};
		AtomicEpicsType<int> status_{static_cast<int>(ENABLE)};

		MasterSwitch();
		~MasterSwitch();
	public:
		// Only reason this is a singleton is to make the C-API easier
		// And I guess because the constructor blocks the thread for ~0.75sec
		// so preventing excessive ctor calls is preferred
		static MasterSwitch& Instance();
		MasterSwitch(MasterSwitch const&) = delete;
		MasterSwitch& operator=(MasterSwitch const&) = delete;
		bool Enable();
		bool Disable();
		bool isEnabled() const;


		// No-op
		void Update(int const& data) override;
	};
};
