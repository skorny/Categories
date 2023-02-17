#include <QSettings>
#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariantMap>
#include <QLineEdit>

#include "expandedlistcontrol.h"
#include "ui_expandedlistcontrol.h"

/// источник
/// https://blog.fancyaddress.com/qt-2/create-something-like-the-widget-box-as-in-the-qt-designer/
/// Qt::ToolButtonTextBesideIcon
///
/// /////////////////////////////////////////////////////////////////////////////
/// \brief QtCategoryButton
/// Список категорий
/////////////////////////////////////////////////////////////////////////////

QtCategoryButton::QtCategoryButton( const QString& category_name, int category_id,
        QTreeWidget* a_pParent, QTreeWidgetItem* a_pItem )
    : QPushButton(category_name, a_pParent)
    , m_pItem(a_pItem)
    , m_category_id( category_id )
{
    this->setAttribute(Qt::WA_WindowPropagation,false);
    setStyleSheet(QString::fromUtf8("text-align: left;"));
    setSizePolicy( QSizePolicy::Preferred,  QSizePolicy::Preferred);

    setIconSize(QSize(40,40));
    QPixmap pix(QString::fromUtf8(":/images/arrow_right.png"));
    setIcon(QIcon(pix));
    QString str_temp =  QString::fromUtf8(" QPushButton {"
                                              "text-align: left;"
                                              "background: #dadbde;"
                                              "border: none;"
                                              "}"
                                              "QPushButton:hover { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa); }"                                     );
    setStyleSheet(str_temp);

    // раскрываем список категоий
    connect(this, SIGNAL(pressed()), this, SLOT(ButtonPressed()));
}

QtCategoryButton::~QtCategoryButton()
{
}

void QtCategoryButton::ButtonPressed()
{
    m_pItem->setExpanded( !m_pItem->isExpanded() );
    if(m_pItem->isExpanded()){
        QPixmap pix(QString::fromUtf8(":/images/arrow_bottom.png"));
        setIcon(QIcon(pix));
    }
    else{
        QPixmap pix(QString::fromUtf8(":/images/arrow_right.png"));
        setIcon(QIcon(pix));
    }
    return;
}

void QtCategoryButton::onSetExpanded()
{
    m_pItem->setExpanded( !m_pItem->isExpanded() );
}


/////////////////////////////////////////////////////////////////////////////
/// \brief ExpandedListControl
/// Список подкатегорий со значениями
/////////////////////////////////////////////////////////////////////////////

ExpandedListControl::ExpandedListControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExpandedListControl)
{
    ui->setupUi(this);
    //ui->treeWidget->installEventFilter(this);
    ui->treeWidget->setRootIsDecorated(false);
    ui->treeWidget->setIndentation(0);
    ui->treeWidget->setHeaderHidden(true);
}

ExpandedListControl::~ExpandedListControl()
{
    foreach( auto it, m_dym){
        it->deleteLater();
    }
    delete ui;
}

bool ExpandedListControl::set_categories(const QByteArray &arr)
{
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    QJsonParseError parseError;
    jsonDoc = QJsonDocument::fromJson(arr, &parseError);
    if(parseError.error != QJsonParseError::NoError){
        m_error_str = "Json parse error:" + parseError.errorString();
        return false;
    }
    jsonObj = jsonDoc.object();
    bool res = true;
    try {
        if(jsonObj[QString::fromUtf8("categories")].isNull() || jsonObj[QString::fromUtf8("categories")].isUndefined()) throw(0);
        QJsonArray jsarr = jsonObj[QString::fromUtf8("categories")].toArray();
        auto it = jsarr.begin();

        while(it != jsarr.end()){
            if( !(*it).isObject() ) throw(1);
            QJsonObject obj = (*it).toObject();
            // map key : "name" это имя категории
            // map key : "powers" это массив свойст вида(ключ, значение)
            if( obj[QString::fromUtf8("name")].isNull() || obj[QString::fromUtf8("name")].isUndefined()) throw(2);
            QString caterory_namestr = obj[QString::fromUtf8("name")].toString();

            if( obj[QString::fromUtf8("powers")].isNull() || obj[QString::fromUtf8("powers")].isUndefined()) throw(3);
            if( !obj[QString::fromUtf8("powers")].isObject()) throw(4);

            QVariantMap _props = obj[QString::fromUtf8("powers")].toObject().toVariantMap();

            // тут создаем список подкатегорий со значениями
            addCategory(caterory_namestr, _props);
            it++;
        }
    }  catch (int code) {
        res = false;
        switch (code) {
        case 0:
            m_error_str ="JSon document does not contain -categories- keyword";
            break;
        case 1:
            m_error_str ="One or more Elements of categoies is not a JSon Object";
            break;
        case 2:
            m_error_str ="JSon document does not contain -name- keyword";
            break;
        case 3:
            m_error_str ="JSon document does not contain -powers- keyword";
            break;
        case 4:
            m_error_str ="powers is not a JSon Object";
            break;
        default:
            m_error_str ="WHAT?! You must dont be here!";
            break;
        }
    }
    return res;
}

void ExpandedListControl::addCategory(const QString& name, const QVariantMap& _props)
{
    // каждый из создаваенмых объектов методом new, имеет родителя и помещен в ui
    // следовательно аккуратно зачистится при уничтожении ui

    // создание категории
    QTreeWidgetItem* pCategory = new QTreeWidgetItem();
    ui->treeWidget->addTopLevelItem(pCategory);
    QtCategoryButton *catbut = new QtCategoryButton(name, 1, ui->treeWidget, pCategory);
    ui->treeWidget->setItemWidget(pCategory, 0,catbut);
    m_dym.push_back(catbut);

    // создание списка подкатегорий
    QWidget *sub = new QWidget(catbut);
    QGridLayout* main_layout = new QGridLayout();
    m_dym.push_back(sub);
    m_dym.push_back(main_layout);
    main_layout->setHorizontalSpacing(0);
    main_layout->setVerticalSpacing(0);
    main_layout->setContentsMargins(0,0,0,0);
    main_layout->setMargin(0);
    auto it = _props.begin();
    while(it != _props.end()){
        QLineEdit *propname = new QLineEdit(it.key());
        QLineEdit *propvalue = new QLineEdit(it.value().toString());
        m_dym.push_back(propname);
        m_dym.push_back(propvalue);
        // поле с названием подкатегории, только для чтения
        propname->setReadOnly(true);
        int row = main_layout->rowCount();
        main_layout->addWidget(propname, row, 0);
        main_layout->addWidget(propvalue, row, 1);
        it++;
    }
    sub->setLayout(main_layout);
    QTreeWidgetItem* pContainer = new QTreeWidgetItem();
    pCategory->addChild(pContainer);
    ui->treeWidget->setItemWidget(pContainer, 0, sub);
}
