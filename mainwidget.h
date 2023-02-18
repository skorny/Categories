#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class ExpandedListControl;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    Ui::MainWidget *ui;
    ExpandedListControl *m_elem = nullptr;
};
#endif // MAINWIDGET_H
