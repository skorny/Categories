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
    m_elem = new (ExpandedListControl);
    ui->verticalLayout_2->replaceWidget(ui->widget, m_elem);

    QFile file(":/data/_json_categories.txt");
    file.open(QIODevice::ReadOnly);
    QByteArray arr = file.readAll();
    if( !m_elem->set_categories(arr))
        qDebug() << m_elem->getLastError();
}

MainWidget::~MainWidget()
{
    delete m_elem;
    delete ui;
}

