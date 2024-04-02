/* Copyright (c) 2012-2019 ImFusion GmbH, Munich, Germany. All rights reserved. */
#pragma once

#include <ImFusion/Base/Algorithm.h>
#include <ImFusion/US/UltrasoundSweep.h>

#include <memory>
#include <unordered_map>
#include <QString>
#include <QVector>

namespace ImFusion
{
	class SharedImageSet;

	class MhaAdapterAlgorithm : public Algorithm
	{
	public:
		/// Creates the algorithm instance with an image
        explicit MhaAdapterAlgorithm(std::vector<UltrasoundSweep*> sweeps);

		/// Factory method to check for applicability or to create the algorithm
		static bool createCompatible(const DataList& data, Algorithm** a = nullptr);


		/// Applies the processing
		void compute() override;

		/// If new data was created, make it available here
		OwningDataList takeOutput() override;

        QString configFileContent;                                  // Content of Plus configuration file.
        mat4 imageToProbTransformMat = mat4::Identity();            // ImageToProb matrix from Plus configuration file.
        mat4 referenceToBedTransformMat = mat4::Identity();         // ReferenceToBed matrix from Plus configuration file.
        vec3 volumeReconstructionSpacing = {1., 1., 1.};   // Spacing from Plus configuration file

	private:
        std::vector<UltrasoundSweep *> m_sweeps;    /// Sweeps form Data View.
        std::unique_ptr<UltrasoundSweep> m_out;
        mat4 calibration;                           /// Calibration matrix
    };
}
