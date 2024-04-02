#include "MhaAdapterAlgorithm.h"

#include <ImFusion/Base/DataList.h>

#include <ImFusion/US/UltrasoundSweep.h>
#include <ImFusion/GL/TrackedSharedImageSet.h>


#include <ImFusion/Base/TrackedSharedImageSetPropertiesAlgorithm.h>

#include <ImFusion/Core/Log.h>

#include <ImFusion/GUI/TransformationMatrixWidget.h>

#include <ImFusion/Base/Pose.h>

#undef IMFUSION_LOG_DEFAULT_CATEGORY
#define IMFUSION_LOG_DEFAULT_CATEGORY "MhaAdapter"

namespace ImFusion
{
    MhaAdapterAlgorithm::MhaAdapterAlgorithm(std::vector<UltrasoundSweep*> sweeps)
            : m_sweeps(std::move(sweeps))
    {
    }

	bool MhaAdapterAlgorithm::createCompatible(const DataList& data, Algorithm** a)
	{
		// check requirements to create the algorithm
        // we accept only one sweep selected to process.
		if (data.size() != 1) return false;

        // check if selected item is ultrasound sweep.
        auto img = data.getImages(Data::IMAGESET,Data::ULTRASOUND);
        if (img.size() != data.size())
            return false;

        // requirements are met, create the algorithm if asked
        std::vector<UltrasoundSweep*> sweeps(img.size(), nullptr);
        for (int i = 0; i < data.size(); i++) {
            if (auto sweep = dynamic_cast<UltrasoundSweep*>(img[i])) {
                sweeps[i] = sweep;
            } else {
                return false;
            }
        }

        if (a) {
            // create algorithm adapter and pass slices to it.
            *a = new MhaAdapterAlgorithm(sweeps);
        }

        return true;
	}

	void MhaAdapterAlgorithm::compute()
	{
		m_status = static_cast<int>(Status::Error);                 // set generic error status until we have finished
        if (m_sweeps.empty()) return;

        for (int i = 0; i < m_sweeps.size(); i++) {
            auto sweep = m_sweeps[i];

            // Temporary Tranducer Matrix to convert the plus coordinate system to ImFusion and back
            static vec3 tranducerTrans(
                    1. * sweep->get(0)->width() / 2,
                    1. * sweep->get(0)->height() / 2,
                    0);
            static vec3 rotTranducerTemp(0, 0, 0);
            static mat4 tranducerTempMatrix = Pose::eulerToMat(tranducerTrans, rotTranducerTemp);

            //extract two tracking streams
            auto newTs = std::make_unique<TrackingSequence>("ImageToReferenceCalculated");
            //build in a check
            auto ts0 = sweep->tracking(0);  //probe to tracker
            auto ts1 = sweep->tracking(1); //reference to tracker

            for (int i = 0; i < ts0->size(); i++) {
                auto ts0Matrix = ts0->rawMatrix(ts0->timestamp(i));
                auto ts1Matrix = ts1->rawMatrix(ts1->timestamp(i));
                auto ts1InverseMatrix = ts1Matrix.inverse();  //


if (!this->referenceToBedTransformMat.isIdentity()) {
//in case of prostate sweeps
    auto imageToReferenceCalculatedMatrix = this->referenceToBedTransformMat * (ts1InverseMatrix * (ts0Matrix * (this->imageToProbTransformMat)));
    newTs->add(imageToReferenceCalculatedMatrix, ts0->timestamp(i));
} else {
//in case of spinal sweeps
    auto imageToReferenceCalculatedMatrix = ts1InverseMatrix * (ts0Matrix * (this->imageToProbTransformMat));
    newTs->add(imageToReferenceCalculatedMatrix, ts0->timestamp(i));
}
            }
            sweep->addTracking(std::move(newTs));
            sweep->setUseTracking(sweep->tracking()->size()-1);

            /**
            // Remove Tracking Streams which are not used.
            std::vector<int> tsToRemove(0);
            for (int t = 0; t < sweep->numTracking(); t++) {
                auto ts = sweep->tracking(t);
                if (t != sweep->trackingUsed()) {
                    tsToRemove.push_back(t);
                }
            }
            int currIndex = 0;
            while (1 != sweep->numTracking()) {
                if (currIndex != sweep->trackingUsed()) {
                    sweep->removeTracking(currIndex);
                    currIndex = 0;
                } else {
                    currIndex++;
                }
            }
            */
            // Set transformation and calibration matrixes.
            auto currentTs = sweep->tracking(sweep->trackingUsed());
            currentTs->setCalibration(tranducerTempMatrix);
            currentTs->emitTrackingChanged();

        }

        this->signalParametersChanged.emitSignal();
        this->signalParametersChanged.emitSignalRecursive();

        // set algorithm status to success
        m_status = static_cast<int>(Status::Success);

        LOG_INFO("MhaAdapterAlgorithm: Compute is done.");
	}

	OwningDataList MhaAdapterAlgorithm::takeOutput()
	{
		// if we have produced some output, add it to the list
		return OwningDataList(std::move(m_out));
	}
}
