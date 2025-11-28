
#ifndef DATATYPES_H
#define DATATYPES_H

#include <string>

struct estructuraTB {
    int id;
    double amount;
    std::string comment;
    std::string date;
    std::string currency;
    bool processed;
};

struct estructuraTN {
    int id;
    double amount;
    std::string comment;
    std::string date;
    int id_TB;
    std::string category_name;
    int category_id;
};

struct estructuraCategoria {
    int id;
    std::string nombre;
    int id_padre;
};

struct estructuraDivisa {
    std::string codigo;
    std::string nombre;
    std::string simbolo;
};

#endif
