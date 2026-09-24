#include "categorytreewidgetdialog.h"
#include "ui_categorytreewidgetdialog.h"
#include <QTreeWidget>
#include <QMessageBox>

void inicializarTreeWidget(QTreeWidget *tree,
                           const std::vector<Category_Structure> &categorias,
                           const QString &filterTipo);

categoryTreeWidgetDialog::categoryTreeWidgetDialog(
    QWidget *parent,
    const std::vector<Category_Structure> &categorias,
    const QString &filterTipo)
    : QDialog(parent)
    , ui(new Ui::categoryTreeWidgetDialog)
    , m_categorias(categorias)
    , m_filterTipo(filterTipo)
    , m_selectedName("")
    , m_selectedId(-1)
{
    ui->setupUi(this);

    inicializarTreeWidget(ui->treeWidget, m_categorias, m_filterTipo);

    // Conectar la señal de doble clic
    connect(ui->treeWidget, &QTreeWidget::itemDoubleClicked,
            this, &categoryTreeWidgetDialog::onItemDoubleClicked);
}

categoryTreeWidgetDialog::~categoryTreeWidgetDialog()
{
    delete ui;
}

void categoryTreeWidgetDialog::onItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    if (item) {
        m_selectedName = item->text(0);
        m_selectedId = item->data(0, Qt::UserRole).toInt();
        accept();
    }
}

QString categoryTreeWidgetDialog::getSelectedCategoryName() const
{
    return m_selectedName;
}

int categoryTreeWidgetDialog::getSelectedCategoryId() const
{
    return m_selectedId;
}

void inicializarTreeWidget(QTreeWidget *tree,
                           const std::vector<Category_Structure> &categorias,
                           const QString &filterTipo)
{
    tree->clear();
    tree->setColumnCount(1);
    tree->setHeaderLabel("Categorías");

    std::map<int, QTreeWidgetItem*> mapa;
    std::set<int> categoriasAMostrar;

    if (filterTipo.isEmpty()) {
        for (const auto &cat : categorias) {
            categoriasAMostrar.insert(cat.id);
        }
    } else {
        for (const auto &cat : categorias) {
            if (QString::fromStdString(cat.tipo).toLower() == filterTipo.toLower()) {
                categoriasAMostrar.insert(cat.id);
            }
        }

        for (const auto &cat : categorias) {
            if (categoriasAMostrar.count(cat.id) > 0) {
                int parentId = cat.id_parent;
                while (parentId != 0) {
                    categoriasAMostrar.insert(parentId);
                    int currentParentId = parentId;
                    auto it = std::find_if(categorias.begin(), categorias.end(),
                        [currentParentId](const Category_Structure& c){ return c.id == currentParentId; });
                    if (it != categorias.end()) {
                        parentId = it->id_parent;
                    } else {
                        break;
                    }
                }
            }
        }
    }

    for (const auto &cat : categorias) {
        if (categoriasAMostrar.count(cat.id) == 0) {
            continue;
        }
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, QString::fromStdString(cat.name));
        item->setData(0, Qt::UserRole, cat.id);
        mapa[cat.id] = item;
    }

    for (const auto &cat : categorias) {
        if (categoriasAMostrar.count(cat.id) == 0) {
            continue;
        }
        QTreeWidgetItem *item = mapa[cat.id];

        if (cat.id_parent == 0 || mapa.find(cat.id_parent) == mapa.end()) {
            tree->addTopLevelItem(item);
        } else {
            mapa[cat.id_parent]->addChild(item);
        }
    }

    tree->expandAll();
}
