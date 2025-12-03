
#include "categorytreewidgetdialog.h"
#include "ui_categorytreewidgetdialog.h"
#include <QTreeWidget>

void inicializarTreeWidget(QTreeWidget *tree,
                           const std::vector<Category_Structure> &categorias);



categoryTreeWidgetDialog::categoryTreeWidgetDialog(
    QWidget *parent,
    const std::vector<Category_Structure> &categorias)
    : QDialog(parent)
    , ui(new Ui::categoryTreeWidgetDialog)
    , m_categorias(categorias)                // <-- guardamos copia
{
    ui->setupUi(this);

    inicializarTreeWidget(ui->treeWidget, m_categorias);
}


categoryTreeWidgetDialog::~categoryTreeWidgetDialog()
{
    delete ui;
}


void inicializarTreeWidget(QTreeWidget *tree,
                           const std::vector<Category_Structure> &categorias)
{

    tree->clear();
    tree->setColumnCount(1);
    tree->setHeaderLabel("Categorías");

    std::map<int, QTreeWidgetItem*> mapa;

    for (const auto &cat : categorias) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, QString::fromStdString(cat.name));
        item->setData(0, Qt::UserRole, cat.id);
        mapa[cat.id] = item;
    }

    for (const auto &cat : categorias) {
        QTreeWidgetItem *item = mapa[cat.id];

        if (cat.id == 0)
            tree->addTopLevelItem(item);
        else
            mapa[cat.id_parent]->addChild(item);
    }

    tree->expandAll();

}


