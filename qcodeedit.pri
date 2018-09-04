INCLUDEPATH += \
    $$PWD \
    $$PWD/document \
    $$PWD/snippets \
    $$PWD/widgets

LIBS += \
    -L../qcodeedit -lqcodeedit

FORMS += $$PWD/widgets/searchreplace.ui \
        $$PWD/widgets/gotoline.ui \
        $$PWD/widgets/gotolinedialog.ui \
        $$PWD/widgets/editconfig.ui \
        $$PWD/widgets/formatconfig.ui \
        $$PWD/snippets/snippetedit.ui

QT += printsupport gui xml

