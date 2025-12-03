#include "categorytreewidgetdialog.h"
#include "ui_categorytreewidgetdialog.h"
#include <QTreeWidget>
#include <QMessageBox>

void inicializarTreeWidget(QTreeWidget *tree,
                           const std::vector<Category_Structure> &categorias);

categoryTreeWidgetDialog::categoryTreeWidgetDialog(
    QWidget *parent,
    const std::vector<Category_Structure> &categorias)
    : QDialog(parent)
    , ui(new Ui::categoryTreeWidgetDialog)
    , m_categorias(categorias)
    , m_selectedName("")
    , m_selectedId(-1)  // Valor por defecto indicando que no hay selección
{
    ui->setupUi(this);

    inicializarTreeWidget(ui->treeWidget, m_categorias);

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
    Q_UNUSED(column); // No usamos el parámetro de columna

    if (item) {
        // Obtener el nombre y el ID del item
        m_selectedName = item->text(0);
        m_selectedId = item->data(0, Qt::UserRole).toInt();

        // Cerrar el diálogo con resultado aceptado
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

        if (cat.id_parent == 0 || mapa.find(cat.id_parent) == mapa.end())
            tree->addTopLevelItem(item);
        else
            mapa[cat.id_parent]->addChild(item);
    }

    tree->expandAll();
}
