#include <QPixmap>
#include <QFile>
#include <QDebug>
#include <QMessageBox>

#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "expandedlistcontrol.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    auto elem = new (ExpandedListControl);
    ui->verticalLayout_2->replaceWidget(ui->widget, elem);

    QFile file(":/data/_json_categories.txt");
    file.open(QIODevice::ReadOnly);
    QByteArray arr = file.readAll();
    if( !elem->set_categories(arr))
        qDebug() << elem->getLastError();
}

MainWidget::~MainWidget()
{
    delete ui;
}

