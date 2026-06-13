QT       += core gui sql printsupport

CONFIG += c++17 cmdline

INCLUDEPATH += .

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    main_2.cpp \
    backend/sqlmanager.cpp \
    backend/transactionsmanager.cpp \
    frontend/categorytreewidgetdialog.cpp \
    frontend/transaccionbasicadialog.cpp \
    frontend/mainwindow.cpp \
    frontend/tableutils.cpp \
    frontend/addderivatetransactiondialog/addderivativetransactionsdialog.cpp \
    frontend/addderivatetransactiondialog/dateeditdelegate.cpp \
    frontend/addderivatetransactiondialog/doublespinboxdelegate.cpp \
    frontend/addderivatetransactiondialog/spinboxdelegate.cpp


HEADERS += \
   commonDataTypes.h \
    backend/DataTypes.h \
    backend/itransactionsmanager.h \
    backend/sqlmanager.h \
    backend/transactionsmanager.h \
    frontend/categorytreewidgetdialog.h \
    frontend/transaccionbasicadialog.h \
    frontend/mainwindow.h \
    frontend/tableutils.h \
    frontend/addderivatetransactiondialog/addderivativetransactionsdialog.h \
    frontend/addderivatetransactiondialog/dateeditdelegate.h \
    frontend/addderivatetransactiondialog/doublespinboxdelegate.h \
    frontend/addderivatetransactiondialog/spinboxdelegate.h


#QMAKE_CXXFLAGS += -std=gnu++11

FORMS += \
    frontend/categorytreewidgetdialog.ui \
    frontend/transaccionbasicadialog.ui \
    frontend/mainwindow.ui \
    frontend/addderivatetransactiondialog/addderivativetransactionsdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target