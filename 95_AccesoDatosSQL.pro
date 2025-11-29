QT       += core gui sql printsupport

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        frontend/addderivatetransactiondialog/addderivativetransactionsdialog.cpp \
        frontend/addderivatetransactiondialog/dateeditdelegate.cpp \
        frontend/addderivatetransactiondialog/doublespinboxdelegate.cpp \
        frontend/categorytreewidgetdialog.cpp \
        backend/categorymodel/categorynode.cpp \
        backend/transactionmodel/basetransaction.cpp \
        backend/transactionmodel/derivativetransaction.cpp \
        backend/sqlmanager.cpp \
        backend/transactionmodel/transaction.cpp \
        backend/transactionsmanager.cpp \
        frontend/addderivatetransactiondialog/spinboxdelegate.cpp \
        frontend/transaccionbasicadialog.cpp \
        main.cpp \
        main_2.cpp \
        frontend/mainwindow.cpp \
        frontend/tableutils.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    backend/DataTypes.h \
    commonDataTypes.h \
    frontend/addderivatetransactiondialog/addderivativetransactionsdialog.h \
    frontend/addderivatetransactiondialog/dateeditdelegate.h \
    frontend/addderivatetransactiondialog/doublespinboxdelegate.h \
    frontend/categorytreewidgetdialog.h \
    backend/categorymodel/categorynode.h \
    backend/transactionmodel/basetransaction.h \
    backend/transactionmodel/derivativetransaction.h \
    backend/itransactionsmanager.h \
    backend/sqlmanager.h \
    backend/transactionmodel/transaction.h \
    backend/transactionsmanager.h \
    frontend/mainwindow.h \
    frontend/addderivatetransactiondialog/spinboxdelegate.h \
    frontend/transaccionbasicadialog.h \
    frontend/tableutils.h


QMAKE_CXXFLAGS += -std=gnu++11

FORMS += \
    frontend/addderivatetransactiondialog/addderivativetransactionsdialog.ui \
    frontend/categorytreewidgetdialog.ui \
    frontend/mainwindow.ui \
    frontend/transaccionbasicadialog.ui
