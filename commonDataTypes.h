#ifndef COMMONDATATYPES_H
#define COMMONDATATYPES_H

#include <string>
#include <array>

enum class UpdateResult {
    Success,
    EmptyValues,
    SumMismatch,
    NoChanges,
    DatabaseError,
    ParentNotFound
};

// Tamaños fijos
constexpr size_t N_FIELDS_T = 5;
constexpr size_t N_FIELDS_DT = 5;

enum t_FIELDS{t_DATE, t_CONCEPT, t_AMOUNT, t_CURRENCY, t_TIPO};
enum dt_FIELDS{dt_DATE, dt_CONCEPT, dt_AMOUNT, dt_CATEGORY, dt_TIPO};


struct T_Structure {
    int id;
    std::array<std::string, N_FIELDS_T> values;
    bool processed;
    std::string tipo;
};

struct DT_Structure {
    int id;
    std::array<std::string, N_FIELDS_DT> values;
    int id_T;
    std::string tipo;
};

struct Category_Structure {
    int id;
    std::string name;
    int id_parent;
    std::string tipo;
};

struct Currency_Structure {
    std::string code;
    std::string name;
    std::string symbol;
};

#endif
