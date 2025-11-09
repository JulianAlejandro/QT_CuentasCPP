QT       += core gui sql printsupport

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        gestor_sql.cpp \
        main.cpp \
        personadialog.cpp \
        transaccionbase.cpp \
        transaccionbruta.cpp \
        transaccionneta.cpp \
        widget.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    gestor_sql.h \
    personadialog.h \
    transaccionbase.h \
    transaccionbruta.h \
    transaccionneta.h \
    widget.h


QMAKE_CXXFLAGS += -std=gnu++11

FORMS += \
    personadialog.ui \
    widget.ui
