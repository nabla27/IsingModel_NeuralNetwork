#include "mainwindow.h"

#include <QVBoxLayout>
#include "cellview.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *vLayout = new QVBoxLayout(centralWidget);
    QWidget *contentsWidget = new QWidget(centralWidget);
    QHBoxLayout *contentsLayout = new QHBoxLayout(contentsWidget);
    CellView *cellView = new CellView(centralWidget);
    CellItem *cellItem = new CellItem(cellView, nullptr);
    ParameterSettingWidget *paramSetting = new ParameterSettingWidget(contentsWidget, cellItem);
    CellViewSettingWidget *settingWidget = new CellViewSettingWidget(centralWidget, cellItem);

    cellView->scene()->addItem(cellItem);
    contentsLayout->setContentsMargins(0, 0, 0, 0);
    contentsWidget->setContentsMargins(0, 0, 0, 0);

    setCentralWidget(centralWidget);
    centralWidget->setLayout(vLayout);
    vLayout->addWidget(contentsWidget);
    contentsWidget->setLayout(contentsLayout);
    contentsLayout->addWidget(cellView);
    contentsLayout->addWidget(paramSetting);
    vLayout->addWidget(settingWidget);

    connect(settingWidget, &CellViewSettingWidget::saveRequested, [=]()
    {
        const QString path = QFileDialog::getSaveFileName(this);

        if(path.isEmpty()) return;

        const QPixmap pixmap = cellView->grab();

        pixmap.save(path, "png");
    });
}

MainWindow::~MainWindow()
{
}

