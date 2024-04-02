#include "MhaAdapterFactory.h"
#include "MhaAdapterAlgorithm.h"
#include "MhaAdapterController.h"

namespace ImFusion
{
	MhaAdapterAlgorithmFactory::MhaAdapterAlgorithmFactory()
	{
		/// register the MhaAdapterAlgorithm
		registerAlgorithm<MhaAdapterAlgorithm>("Ultrasound;Plus Toolkit Configuration File");
	}

	AlgorithmController* MhaAdapterControllerFactory::create(Algorithm* a) const
	{
		/// register the MhaAdapterController for the MhaAdapterAlgorithm
		if (auto* alg = dynamic_cast<MhaAdapterAlgorithm*>(a))
			return new MhaAdapterController(alg);
		return nullptr;
	}
}
