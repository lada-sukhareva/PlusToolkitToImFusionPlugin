/* Copyright (c) 2012-2019 ImFusion GmbH, Munich, Germany. All rights reserved. */
#pragma once

#include <ImFusion/GUI/AlgorithmController.h>
#include <QtWidgets/QWidget>

class Ui_MhaAdapterController;
namespace ImFusion
{
	class MhaAdapterAlgorithm;

	/// Demonstration of implementing a custom controller using Qt widgets.
	class MhaAdapterController
		: public QWidget
		, public AlgorithmController
	{
		Q_OBJECT

	public:
		/// Constructor with the algorithm instance
		explicit MhaAdapterController(MhaAdapterAlgorithm* algorithm);

		/// Destructor
		~MhaAdapterController() override;

		/// Initializes the widget
		void init() override;

	public slots:
		/// Apply the chosen processing
		void onApply();
        void onFileSelected();

	protected:
		Ui_MhaAdapterController* m_ui;    ///< The actual GUI
		MhaAdapterAlgorithm* m_alg;       ///< The algorithm instance
    };
}
