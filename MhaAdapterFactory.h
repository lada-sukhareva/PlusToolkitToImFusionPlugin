/* Copyright (c) 2012-2019 ImFusion GmbH, Munich, Germany. All rights reserved. */
#pragma once

#include <ImFusion/Base/AlgorithmControllerFactory.h>
#include <ImFusion/Base/AlgorithmFactory.h>

namespace ImFusion
{
	class Algorithm;

	/// AlgorithmFactory for plugin
	class MhaAdapterAlgorithmFactory : public AlgorithmFactory
	{
	public:
        MhaAdapterAlgorithmFactory();
	};

	/// AlgorithmControllerFactory for plugin
	class MhaAdapterControllerFactory : public AlgorithmControllerFactory
	{
	public:
		AlgorithmController* create(Algorithm* a) const override;
	};
}
