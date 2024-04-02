#include "MhaAdapterController.h"
#include "MhaAdapterAlgorithm.h"

#include <ImFusion/Base/DataModel.h>
#include <ImFusion/Base/SharedImageSet.h>
#include <ImFusion/GUI/MainWindowBase.h>

#include "ui_MhaAdapterController.h"

#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QTextStream>
#include <QXmlStreamReader>

namespace ImFusion
{
	MhaAdapterController::MhaAdapterController(MhaAdapterAlgorithm* algorithm)
		: AlgorithmController(algorithm)
		, m_alg(algorithm)
	{
        m_ui = new Ui_MhaAdapterController();
		m_ui->setupUi(this);
        connect(m_ui->pushButtonApply, SIGNAL(clicked()), this, SLOT(onApply()));
        connect(m_ui->pushButtonChoose, SIGNAL(clicked()), this, SLOT(onFileSelected()));
	}


	MhaAdapterController::~MhaAdapterController()
	{
		delete m_ui;
	}

	void MhaAdapterController::init()
    {

    }

    void MhaAdapterController::onFileSelected()
    {

        QString filePath = QFileDialog::getOpenFileName(
            this,
            tr("Choose File"),
            QDir::homePath(),
            tr("XML Files (*.xml)")
        );

        if(filePath.isEmpty()) {

            return;
        }

        QFile file(filePath);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return;
        }
        QTextStream in(&file);
        m_alg->configFileContent = in.readAll();
        file.close();

        QXmlStreamReader reader(m_alg->configFileContent);
        while(!reader.atEnd() && !reader.hasError()) {
            auto element = reader.readNext();
            if(element == QXmlStreamReader::StartElement && reader.name() == "Transform") {
                if (reader.attributes().hasAttribute("From")
                    && reader.attributes().hasAttribute("To")
                    && reader.attributes().hasAttribute("Matrix")
                    && reader.attributes().value("From") == "Image"
                    && reader.attributes().value("To") == "Probe"
                ) {
                    QVector<QStringRef> split = reader.attributes().value("Matrix").split(" ", Qt::SkipEmptyParts);
                    std::vector<double> matrixVector(split.size());
                    std::transform(split.begin(), split.end(), matrixVector.begin(), [](QStringRef i) {  return i.toDouble(); });
                    mat4 tmpMatrix(matrixVector.data());
                    m_alg->imageToProbTransformMat = tmpMatrix.transpose();
                }
                if (reader.attributes().hasAttribute("From")
                    && reader.attributes().hasAttribute("To")
                    && reader.attributes().hasAttribute("Matrix")
                    && reader.attributes().value("From") == "Reference"
                    && reader.attributes().value("To") == "Bed"
                        ) {
                    QVector<QStringRef> split = reader.attributes().value("Matrix").split(" ", Qt::SkipEmptyParts);
                    std::vector<double> matrixVector(split.size());
                    std::transform(split.begin(), split.end(), matrixVector.begin(), [](QStringRef i) {  return i.toDouble(); });
                    mat4 tmpMatrix(matrixVector.data());
                    m_alg->referenceToBedTransformMat = tmpMatrix.transpose();
                }
            }

        }
        if (reader.hasError()) {
            LOG_ERROR("ERROR: " << reader.errorString().toStdString());
        }
    }

	void MhaAdapterController::onApply()
	{
		m_alg->compute();
		m_main->dataModel()->add(m_alg->takeOutput());
	}
}
