#include "categorytreewidgetdialog.h"
#include "ui_categorytreewidgetdialog.h"
#include <QTreeWidget>

void inicializarTreeWidget(QTreeWidget *tree,
                           const std::vector<estructuraCategoria> &categorias);

/*
struct Categoria {
    int id;
    QString nombre;
    int id_padre;
};


std::vector<Categoria> categorias =
    {
        {0,  "Raíz",         0},

        {1,  "Ingresos",      0},
        {2,  "Sueldo",        1},
        {3,  "Inversiones",   1},
        {4,  "Intereses",     3},
        {5,  "Dividendos",    3},
        {6,  "Ventas",        1},

        {7,  "Gastos",         0},
        {8,  "Vivienda",       7},
        {9,  "Alquiler",       8},
        {10, "Hipoteca",       8},
        {11, "Servicios",      8},
        {12, "Electricidad",   11},
        {13, "Agua",           11},
        {14, "Internet",       11},

        {15, "Transporte",     7},
        {16, "Combustible",    15},
        {17, "Mantenimiento",  15},

        {18, "Comida",         7},
        {19, "Supermercado",   18},
        {20, "Restaurantes",   18},

        {21, "Salud",          7},
        {22, "Seguros",        7},

        {23, "Ocio",           7},
        {24, "Viajes",         23},
        {25, "Cine/Museos",    23},

        {26, "Tecnología",     7}
};
*/

categoryTreeWidgetDialog::categoryTreeWidgetDialog(
    QWidget *parent,
    const std::vector<estructuraCategoria> &categorias)
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
                           const std::vector<estructuraCategoria> &categorias)
{

    tree->clear();
    tree->setColumnCount(1);
    tree->setHeaderLabel("Categorías");

    std::map<int, QTreeWidgetItem*> mapa;

    for (const auto &cat : categorias) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, QString::fromStdString(cat.nombre));
        item->setData(0, Qt::UserRole, cat.id);
        mapa[cat.id] = item;
    }

    for (const auto &cat : categorias) {
        QTreeWidgetItem *item = mapa[cat.id];

        if (cat.id == 0)
            tree->addTopLevelItem(item);
        else
            mapa[cat.id_padre]->addChild(item);
    }

    tree->expandAll();

}

