QT       += core gui sql printsupport

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        backend/gestor_sql.cpp \
        backend/gestortransacciones.cpp \
        main.cpp \
        main_2.cpp \
        frontend/mainwindow.cpp \
        frontend/personadialog.cpp \
        backend/transaccionbase.cpp \
        frontend/transaccionbasicadialog.cpp \
        backend/transaccionbruta.cpp \
        backend/transaccionneta.cpp \
        frontend/widget.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    backend/gestor_sql.h \
    backend/gestortransacciones.h \
    backend/itransactionmanager.h \
    frontend/mainwindow.h \
    frontend/personadialog.h \
    backend/transaccionbase.h \
    frontend/transaccionbasicadialog.h \
    backend/transaccionbruta.h \
    backend/transaccionneta.h \
    frontend/widget.h


QMAKE_CXXFLAGS += -std=gnu++11

FORMS += \
    frontend/mainwindow.ui \
    frontend/personadialog.ui \
    frontend/transaccionbasicadialog.ui \
    frontend/widget.ui
