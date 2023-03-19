#include "cellview.h"

#include <random>
#include <QTimer>
#include <QFormLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QLineEdit>

CellScene::CellScene(QObject *parent)
    : QGraphicsScene(parent)
{

}

CellView::CellView(QWidget *parent)
    : QGraphicsView(new CellScene(nullptr), parent)
{

}
#include <QComboBox>
CellViewSettingWidget::CellViewSettingWidget(QWidget *parent, CellItem *cell)
    : QWidget(parent)
{
    QFormLayout *fLayout = new QFormLayout(this);
    QSpinBox *intervalSpin = new QSpinBox(this);
    QSpinBox *maxCountSpin = new QSpinBox(this);
    QComboBox *methodCombo = new QComboBox(this);
    QPushButton *startButton = new QPushButton("Start", this);
    QPushButton *stopButton = new QPushButton("Stop", this);
    QPushButton *initButton = new QPushButton("Init", this);
    QPushButton *saveButton = new QPushButton("Save", this);

    setLayout(fLayout);
    fLayout->addRow("Interval", intervalSpin);
    fLayout->addRow("Max Count", maxCountSpin);
    fLayout->addRow("Method", methodCombo);
    fLayout->addRow("", startButton);
    fLayout->addRow("", stopButton);
    fLayout->addRow("", initButton);
    fLayout->addRow("", saveButton);

    intervalSpin->setMaximum(100000);
    maxCountSpin->setMaximum(10000000000);
    maxCountSpin->setValue(cell->maxCount());
    methodCombo->addItems(QStringList() << "Metropolis" << "HeadBath");
    stopButton->hide();

    connect(intervalSpin, &QSpinBox::valueChanged, cell, &CellItem::setInterval);
    connect(maxCountSpin, &QSpinBox::valueChanged, cell, &CellItem::setMaxCount);
    connect(methodCombo, &QComboBox::currentIndexChanged, cell, &CellItem::setMethod);
    connect(startButton, &QPushButton::released, cell, &CellItem::startUpdate);
    connect(startButton, &QPushButton::released, startButton, &QPushButton::hide);
    connect(startButton, &QPushButton::released, stopButton, &QPushButton::show);
    connect(stopButton, &QPushButton::released, cell, &CellItem::stopUpdate);
    connect(stopButton, &QPushButton::released, stopButton, &QPushButton::hide);
    connect(stopButton, &QPushButton::released, startButton, &QPushButton::show);
    connect(initButton, &QPushButton::released, cell, &CellItem::initCell);
    connect(initButton, &QPushButton::released, startButton, &QPushButton::show);
    connect(initButton, &QPushButton::released, stopButton, &QPushButton::hide);
    connect(saveButton, &QPushButton::released, this, &CellViewSettingWidget::saveRequested);

    setContentsMargins(0, 0, 0, 0);
}

ParameterSettingWidget::ParameterSettingWidget(QWidget *parent, CellItem *cell)
    : QWidget(parent)
{
    QFormLayout *fLayout = new QFormLayout(this);
    QLineEdit *stepLineEdit = new QLineEdit(this);
    QDoubleSpinBox *kbTDoubleSpin = new QDoubleSpinBox(this);
    QDoubleSpinBox *JDoubleSpin = new QDoubleSpinBox(this);

    setLayout(fLayout);
    fLayout->addRow("Step", stepLineEdit);
    fLayout->addRow("kT", kbTDoubleSpin);
    fLayout->addRow("J", JDoubleSpin);

    stepLineEdit->setReadOnly(true);
    kbTDoubleSpin->setMaximum(1000);
    kbTDoubleSpin->setMinimum(0.00001);
    kbTDoubleSpin->setDecimals(5);
    kbTDoubleSpin->setValue(cell->paramKbT());
    JDoubleSpin->setMaximum(1000);
    JDoubleSpin->setMinimum(-1000);
    JDoubleSpin->setValue(cell->paramJ());

    connect(cell, &CellItem::stepChanged, [stepLineEdit](const int step){ stepLineEdit->setText(QString::number(step)); });
    connect(kbTDoubleSpin, &QDoubleSpinBox::valueChanged, cell, &CellItem::setParamKbT);
    connect(JDoubleSpin, &QDoubleSpinBox::valueChanged, cell, &CellItem::setParamJ);

    setContentsMargins(0, 0, 0, 0);
}

CellItem::CellItem(QObject *obj, QGraphicsItem *parent)
    : QObject(obj)
    , QGraphicsItem(parent)
    , state(StateType(0))
    , timer(new QTimer(this))
    , ising(IsingModel())
    , metropolis(&ising)
    , headBath(&ising)
{
    initCell();
    setInterval(0);
    timer->setTimerType(Qt::PreciseTimer);

    connect(timer, &QTimer::timeout, this, &CellItem::updateCell);
}

QRectF CellItem::boundingRect() const
{
    return QRectF(0, 0, state.rows() * cellSize, state.cols() * cellSize);
}

void CellItem::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    for(size_t i = 0; i < state.rows(); ++i)
    {
        for(size_t j = 0; j < state.cols(); ++j)
        {
            const size_t x = i * cellSize;
            const size_t y = j * cellSize;

            painter->fillRect(x, y, cellSize, cellSize, (state.at(i, j)) ? Qt::black : Qt::white);
        }
    }
}

void CellItem::setInterval(const int msec)
{
    timer->setInterval(msec);
}

void CellItem::startUpdate()
{
    timer->start();
}

void CellItem::stopUpdate()
{
    timer->stop();
}

void CellItem::initCell()
{
    stepCount = 0;
    timer->stop();
    emit stepChanged(stepCount);

    for(size_t i = 0; i < state.rows(); ++i)
        for(size_t j = 0; j < state.cols(); ++j)
            state[i][j] = (rand() % 2 == 0);

    update();
}

void CellItem::updateCell()
{
    if(stepCount < _maxCount)
    {
        switch(method)
        {
        case Method::Metropolis: metropolis.update(state); break;
        case Method::HeatBath: headBath.update(state); break;
        default: break;
        }

        update();

        stepChanged(++stepCount);
    }
}
