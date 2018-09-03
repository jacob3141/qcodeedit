TEMPLATE = lib
TARGET = qcodeedit

DEPENDPATH += . document language widgets qnfa
INCLUDEPATH += . document language widgets qnfa

CONFIG += staticlib
QT += xml widgets printsupport gui


DEFINES += _QCODE_EDIT_BUILD_

HEADERS += qce-config.h \
        qeditor.h \
        qeditorfactory.h \
        qeditorinputbindinginterface.h \
        qeditorinputbinding.h \
        qeditsession.h \
        qcodeedit.h \
        qpanellayout.h \
        qformatfactory.h \
        qformatscheme.h \
        qlinemarksinfocenter.h \
        qreliablefilewatch.h \
        document/qdocument.h \
        document/qdocument_p.h \
        document/qdocumentcommand.h \
        document/qdocumentcursor.h \
        document/qdocumentline.h \
        document/qdocumentsearch.h \
        qcodecompletionengine.h \
        qlanguagedefinition.h \
        qlanguagefactory.h \
        widgets/qpanel.h \
        widgets/qlinenumberpanel.h \
        widgets/qlinemarkpanel.h \
        widgets/qlinechangepanel.h \
        widgets/qfoldpanel.h \
        widgets/qstatuspanel.h \
        widgets/qgotolinepanel.h \
        widgets/qsearchreplacepanel.h \
        widgets/qgotolinedialog.h \
        widgets/qeditconfig.h \
        widgets/qformatconfig.h \
        widgets/qsimplecolorpicker.h \
        widgets/qcalltip.h \
        snippets/qsnippet.h \
        snippets/qsnippet_p.h \
        snippets/qsnippetpatternloader.h \
        snippets/qsnippetmanager.h \
        snippets/qsnippetedit.h \
        snippets/qsnippetbinding.h

SOURCES += qeditor.cpp \
        qeditorfactory.cpp \
        qeditorinputbinding.cpp \
        qeditsession.cpp \
        qcodeedit.cpp \
        qpanellayout.cpp \
        qformatscheme.cpp \
        qlinemarksinfocenter.cpp \
        qreliablefilewatch.cpp \
        document/qdocument.cpp \
        document/qdocumentcommand.cpp \
        document/qdocumentcursor.cpp \
        document/qdocumentline.cpp \
        document/qdocumentsearch.cpp \
        qcodecompletionengine.cpp \
        qlanguagedefinition.cpp \
        qlanguagefactory.cpp \
        widgets/qpanel.cpp \
        widgets/qlinenumberpanel.cpp \
        widgets/qlinemarkpanel.cpp \
        widgets/qlinechangepanel.cpp \
        widgets/qfoldpanel.cpp \
        widgets/qstatuspanel.cpp \
        widgets/qgotolinepanel.cpp \
        widgets/qsearchreplacepanel.cpp \
        widgets/qgotolinedialog.cpp \
        widgets/qeditconfig.cpp \
        widgets/qformatconfig.cpp \
        widgets/qsimplecolorpicker.cpp \
        widgets/qcalltip.cpp \
        snippets/qsnippet.cpp \
        snippets/qsnippetmanager.cpp \
        snippets/qsnippetedit.cpp \
        snippets/qsnippetbinding.cpp

FORMS += widgets/searchreplace.ui \
        widgets/gotoline.ui \
        widgets/gotolinedialog.ui \
        widgets/editconfig.ui \
        widgets/formatconfig.ui \
        snippets/snippetedit.ui

