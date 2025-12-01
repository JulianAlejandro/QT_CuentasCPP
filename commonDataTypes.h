#ifndef COMMONDATATYPES_H
#define COMMONDATATYPES_H

#include <string>
#include <array>

// Tamaños fijos
constexpr size_t N_FIELDS_T = 4;
constexpr size_t N_FIELDS_DT = 4;

enum t_FIELDS{t_DATE, t_CONCEPT, t_AMOUNT, t_CURRENCY};
enum dt_FIELDS{dt_DATE, dt_CONCEPT, dt_AMOUNT, dt_CATEGORY};

// Arrays globales para títulos
inline const std::array<std::string, N_FIELDS_T> T_FIELD_TITLES = {
    "Date", "Concept", "Amount", "Currency"
};

inline const std::array<std::string, N_FIELDS_DT> DT_FIELD_TITLES = {
    "Date", "Concept", "Amount", "Category"
};

struct T_Structure {
    int id;
    std::array<std::string, N_FIELDS_T> values;
    bool processed;
};

struct DT_Structure {
    int id;
    std::array<std::string, N_FIELDS_DT> values;
    int id_T;
};

struct Category_Structure {
    int id;
    std::string name;
    int id_parent;
};

struct Currency_Structure {
    std::string code;
    std::string name;
    std::string symbol;
};

#endif
