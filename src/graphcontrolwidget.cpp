#include "graphcontrolwidget.h"

#include "graphsettings.h"

#include <QDoubleValidator>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>

GraphControlWidget::GraphControlWidget(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);

    QGroupBox* inputFieldsGroup = createInputFieldsGroup();
    QGroupBox* buttonsGroup = createButtonsGroup();
    QGroupBox* progressBarGroup = createProgressBarGroup();
    QGroupBox* legendGroup = createNodeLegendGroup();

    mainLayout->addWidget(inputFieldsGroup);
    mainLayout->addWidget(buttonsGroup);
    mainLayout->addWidget(progressBarGroup);
    mainLayout->addWidget(legendGroup);

    setFixedWidth(300);
}

QGroupBox* GraphControlWidget::createInputFieldsGroup()
{
    QGroupBox* inputFieldsGroup = new QGroupBox("Parameters");
    QFormLayout* formLayout = new QFormLayout;

    const GraphSettings& settings = GraphSettings::instance();

    _numInterestsInput = new QLineEdit;
    _numInterestsInput->setValidator(new QIntValidator(0, 1000, this));
    _numInterestsInput->setText(QString::number(settings.numInterests));

    _interestsMeanInput = createDoubleInput(settings.interestsMean);
    _interestsStdInput = createDoubleInput(settings.interestsStd);
    _reshareRateMeanInput = createDoubleInput(settings.reshareRateMean);
    _reshareRateStdInput = createDoubleInput(settings.reshareRateStd);
    _recoverRateMeanInput = createDoubleInput(settings.recoverRateMean);
    _recoverRateStdInput = createDoubleInput(settings.recoverRateStd);
    _vulnerabilityMeanInput = createDoubleInput(settings.vulnerabilityMean);
    _vulnerabilityStdInput = createDoubleInput(settings.vulnerabilityStd);

    _numRegularInput = new QLineEdit;
    _numRegularInput->setValidator(new QIntValidator(0, 1000, this));
    _numRegularInput->setText(QString::number(settings.numRegular));

    _numInfluencersInput = new QLineEdit;
    _numInfluencersInput->setValidator(new QIntValidator(0, 1000, this));
    _numInfluencersInput->setText(QString::number(settings.numInfluencers));

    _numBotsInput = new QLineEdit;
    _numBotsInput->setValidator(new QIntValidator(0, 1000, this));
    _numBotsInput->setText(QString::number(settings.numBots));

    _numFactCheckersInput = new QLineEdit;
    _numFactCheckersInput->setValidator(new QIntValidator(0, 100, this));
    _numFactCheckersInput->setText(QString::number(settings.numFactCheckers));

    _spatialThresholdInput = createDoubleInput(settings.spatialThreshold);
    _attrThresholdInput = createDoubleInput(settings.attrThreshold);

    formLayout->addRow("Num Interests:", _numInterestsInput);
    formLayout->addRow("Interests Mean:", _interestsMeanInput);
    formLayout->addRow("Interests Std:", _interestsStdInput);
    formLayout->addRow("Reshare Rate Mean:", _reshareRateMeanInput);
    formLayout->addRow("Reshare Rate Std:", _reshareRateStdInput);
    formLayout->addRow("Recover Rate Mean:", _recoverRateMeanInput);
    formLayout->addRow("Recover Rate Std:", _recoverRateStdInput);
    formLayout->addRow("Vulnerability Mean:", _vulnerabilityMeanInput);
    formLayout->addRow("Vulnerability Std:", _vulnerabilityStdInput);
    formLayout->addRow("Num Regular:", _numRegularInput);
    formLayout->addRow("Num Influencers:", _numInfluencersInput);
    formLayout->addRow("Num Bots:", _numBotsInput);
    formLayout->addRow("Num Fact-Checkers", _numFactCheckersInput);
    formLayout->addRow("Spatial Threshold:", _spatialThresholdInput);
    formLayout->addRow("Attr Threshold:", _attrThresholdInput);

    inputFieldsGroup->setLayout(formLayout);

    return inputFieldsGroup;
}

QGroupBox* GraphControlWidget::createButtonsGroup()
{
    QGroupBox* buttonsGroup = new QGroupBox("Actions");
    QVBoxLayout* buttonsLayout = new QVBoxLayout;

    QPushButton* generateGraphButton = new QPushButton("Generate Graph");
    QPushButton* saveGraphButton = new QPushButton("Save Graph");
    QPushButton* loadGraphButton = new QPushButton("Load Graph");
    QPushButton* runSimulationButton = new QPushButton("Run Simulation");
    QPushButton* runNSimulationsButton = new QPushButton("Run N Simulations");

    buttonsLayout->addWidget(generateGraphButton);
    buttonsLayout->addWidget(saveGraphButton);
    buttonsLayout->addWidget(loadGraphButton);
    buttonsLayout->addWidget(runSimulationButton);
    buttonsLayout->addWidget(runNSimulationsButton);

    buttonsGroup->setLayout(buttonsLayout);

    connect(generateGraphButton, &QPushButton::clicked,
            this, &GraphControlWidget::onCreateGraphClicked);
    connect(runSimulationButton, &QPushButton::clicked,
            this, [this]() { emit runSimulationRequested(); });

    return buttonsGroup;
}

QGroupBox* GraphControlWidget::createProgressBarGroup()
{
    QGroupBox* progressBarGroup = new QGroupBox("Progress");
    QVBoxLayout* progressLayout = new QVBoxLayout;

    QProgressBar* progressBar = new QProgressBar;

    progressLayout->addWidget(progressBar);

    progressBarGroup->setLayout(progressLayout);

    return progressBarGroup;
}

QGroupBox* GraphControlWidget::createNodeLegendGroup()
{
    constexpr size_t numStates = 5;

    const char* labels[numStates] = {
        "Regular",
        "Influencer",
        "Bot",
        "Believer",
        "Fact Checker"
    };

    const QColor colors[numStates] = {
        QColor(0x0B3D91),
        QColor(0xF1C40F),
        QColor(0x7D3C98),
        QColor(0xC0392B),
        QColor(0x228B22)
    };

    constexpr int radius = 10;

    QGroupBox* legendGroup = new QGroupBox("Node Legend");
    QVBoxLayout* legendLayout = new QVBoxLayout;
    legendLayout->setAlignment(Qt::AlignTop);

    for (size_t i = 0; i < numStates; ++i) {
        QHBoxLayout* rowLayout = new QHBoxLayout;

        QFrame* circle = new QFrame;
        circle->setFixedSize(radius * 2, radius * 2);
        circle->setStyleSheet(QString("background-color: %1; border: 1px solid black; border-radius: %2px").arg(colors[i].name()).arg(radius));

        QLabel* text = new QLabel(labels[i]);

        rowLayout->addWidget(circle);
        rowLayout->addWidget(text);

        legendLayout->addLayout(rowLayout);
    }

    legendGroup->setLayout(legendLayout);

    return legendGroup;
}

QLineEdit* GraphControlWidget::createDoubleInput(double defaultValue)
{
    QLineEdit* input = new QLineEdit;
    input->setValidator(new QDoubleValidator(-1000.0, 1000.0, 4, this));
    input->setText(QString::number(defaultValue, 'f', 4));
    return input;
}

void GraphControlWidget::onCreateGraphClicked()
{
    GraphSettings& settings = GraphSettings::instance();

    settings.numInterests = _numInterestsInput->text().toULongLong();
    settings.interestsMean = _interestsMeanInput->text().toDouble();
    settings.interestsStd = _interestsStdInput->text().toDouble();
    settings.recoverRateMean = _recoverRateMeanInput->text().toDouble();
    settings.recoverRateStd = _recoverRateStdInput->text().toDouble();
    settings.vulnerabilityMean = _vulnerabilityMeanInput->text().toDouble();
    settings.vulnerabilityStd = _vulnerabilityStdInput->text().toDouble();
    settings.reshareRateMean = _reshareRateMeanInput->text().toDouble();
    settings.reshareRateStd = _reshareRateStdInput->text().toDouble();
    settings.numRegular = _numRegularInput->text().toULongLong();
    settings.numInfluencers = _numInfluencersInput->text().toULongLong();
    settings.numBots = _numBotsInput->text().toULongLong();
    settings.numFactCheckers = _numFactCheckersInput->text().toULongLong();
    settings.spatialThreshold = _spatialThresholdInput->text().toDouble();
    settings.attrThreshold = _attrThresholdInput->text().toDouble();

    emit createGraphRequested();
}
