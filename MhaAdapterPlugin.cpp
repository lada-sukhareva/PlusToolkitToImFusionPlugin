#include "MhaAdapterPlugin.h"
#include "MhaAdapterFactory.h"

// Export free factory function to instantiate plugin
#ifdef WIN32
extern "C" __declspec(dllexport) ImFusion::ImFusionPlugin* createPlugin()
{
	return new ImFusion::MhaAdapterPlugin;
}
#else
extern "C" ImFusion::ImFusionPlugin* createPlugin()
{
	return new ImFusion::MhaAdapterPlugin;
}
#endif

namespace ImFusion
{
	MhaAdapterPlugin::MhaAdapterPlugin() = default;
    MhaAdapterPlugin::~MhaAdapterPlugin() = default;

    /**
     * Returns factory for Algorithm.
     * @return
     */
    const ImFusion::AlgorithmFactory* MhaAdapterPlugin::getAlgorithmFactory() {
        /// defined in MhaAdapterFactory file
        return new MhaAdapterAlgorithmFactory;
    }

    /**
     * Returns factory for Controller (UI).
     * @return
     */
    const ImFusion::AlgorithmControllerFactory* MhaAdapterPlugin::getAlgorithmControllerFactory() {
        /// defined in MhaAdapterFactory file
        return new MhaAdapterControllerFactory;
    }
}
