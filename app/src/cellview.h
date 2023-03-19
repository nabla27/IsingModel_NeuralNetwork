#ifndef CELLVIEW_H
#define CELLVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QWidget>
#include "metropolismethod.h"
#include "isingmodel.h"
#include "isingheatbathmethod.h"

class QLineEdit;


class CellScene : public QGraphicsScene
{
    Q_OBJECT
public:
    CellScene(QObject *parent);
};

class CellItem;

class CellView : public QGraphicsView
{
    Q_OBJECT
public:
    CellView(QWidget *parent);
};

class CellViewSettingWidget : public QWidget
{
    Q_OBJECT
public:
    CellViewSettingWidget(QWidget *parent, CellItem *cell);

signals:
    void saveRequested();
};

class ParameterSettingWidget : public QWidget
{
    Q_OBJECT
public:
    ParameterSettingWidget(QWidget *parent, CellItem *cell);
};

class CellItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    CellItem(QObject *obj, QGraphicsItem *parent);

public:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    int maxCount() const { return _maxCount; }
    double paramKbT() const { return ising.kbT(); }
    double paramJ() const { return ising.param.J; }

    enum class Method { Metropolis, HeatBath } method = Method::Metropolis;
    Q_ENUM(Method)

public slots:
    void setCellWidth(const int width) { cellSize = width; }
    void setInterval(const int msec);
    void setMaxCount(int count) { _maxCount = count; }
    void startUpdate();
    void stopUpdate();
    void initCell();
    void updateCell();

    void setParamKbT(const double& value) { ising.param.T = value / ising.param.kb; }
    void setParamJ(const double& value) { ising.param.J = value; }
    void setMethod(const int index) { method = Method(index); }

private:
    int cellSize = 5;
    QTimer *timer;
    int stepCount = 0;
    int _maxCount = 10000;

    typedef State<50, 50, bool> StateType;
    StateType state;
    IsingModel ising;
    typedef MetropolisMethod<IsingModel, StateType, &IsingModel::energy, &IsingModel::randomAction, &IsingModel::randomAccept> MetropolisType;
    MetropolisType metropolis;
    IsingHeatBathMethod headBath;

signals:
    void stepChanged(const int step);
};

#endif // CELLVIEW_H
