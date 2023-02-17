#ifndef EXPANDEDLISTCONTROL_H
#define EXPANDEDLISTCONTROL_H

#include <QWidget>
#include <QPushButton>
#include <QTreeWidgetItem>

//class QTreeWidgetItem;

/// источник
/// https://blog.fancyaddress.com/qt-2/create-something-like-the-widget-box-as-in-the-qt-designer/
/// Qt::ToolButtonTextBesideIcon
///
/// /////////////////////////////////////////////////////////////////////////////
/// \brief QtCategoryButton
/// Кнопка для поля категория
/////////////////////////////////////////////////////////////////////////////

class QtCategoryButton : public QPushButton
{
    Q_OBJECT
public:
    QtCategoryButton(const QString& category_name, int category_id, QTreeWidget* a_pParent,
        QTreeWidgetItem* a_pItem);
    virtual ~QtCategoryButton();

private slots:
    void ButtonPressed();
    void onSetExpanded();

private:
    QTreeWidgetItem* m_pItem;
    int m_category_id;
};

////////////////////////////////////////////////////////////////////////////////////////////////
/// Список категорий
/// каждый объект класса представляет собой список категорий с раскрываемыми подкатегориями
/// использование:
/// 1. создать объект класса ExpandedListControl
/// 2. проинициализировать объект с помощью вызова метода ExpandedListControl::set_categories
/// 3. в случае ошибки инициализации вызвать ExpandedListControl::getLastError для информации об ошибки
////////////////////////////////////////////////////////////////////////////////////////////////

namespace Ui {
class ExpandedListControl;
}

class ExpandedListControl : public QWidget
{
    Q_OBJECT

public:
    explicit ExpandedListControl(QWidget *parent = nullptr);
    virtual ~ExpandedListControl();

    ///return true on sucsess, otherwise return false
    /// arr - входные данные для инициализации элемента в JSon формате, образец см. в ресурсах файл :/data/_json_categories.txt
    bool set_categories(const QByteArray &arr);

    const QString &getLastError(){
        return m_error_str;
    }

private:
    Ui::ExpandedListControl *ui;
    QString m_error_str = "";

    void addCategory(const QString& name, const QVariantMap& _props);
    QList< QObject * > m_dym;
};

#endif // EXPANDEDLISTCONTROL_H
