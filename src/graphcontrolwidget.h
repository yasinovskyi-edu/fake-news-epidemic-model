#pragma once

#include <QWidget>

class QGroupBox;
class QLineEdit;

class GraphControlWidget : public QWidget {
    Q_OBJECT

public:
    explicit GraphControlWidget(QWidget* parent = nullptr);

signals:
    void createGraphRequested();
    void runSimulationRequested();

private:
    QGroupBox* createInputFieldsGroup();
    QGroupBox* createButtonsGroup();
    QGroupBox* createProgressBarGroup();
    QGroupBox* createNodeLegendGroup();

    QLineEdit* createDoubleInput(double defaultValue);

private slots:
    void onCreateGraphClicked();

private:
    QLineEdit* _numInterestsInput;
    QLineEdit* _interestsMeanInput;
    QLineEdit* _interestsStdInput;
    QLineEdit* _reshareRateMeanInput;
    QLineEdit* _reshareRateStdInput;
    QLineEdit* _recoverRateMeanInput;
    QLineEdit* _recoverRateStdInput;
    QLineEdit* _vulnerabilityMeanInput;
    QLineEdit* _vulnerabilityStdInput;
    QLineEdit* _numRegularInput;
    QLineEdit* _numInfluencersInput;
    QLineEdit* _numBotsInput;
    QLineEdit* _numFactCheckersInput;
    QLineEdit* _spatialThresholdInput;
    QLineEdit* _attrThresholdInput;
};
