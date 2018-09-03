
TARGET = qcodeedit-plugin
TEMPLATE = lib

CONFIG += designer plugin release

LIBS += -L.. -lqcodeedit

INCLUDEPATH += . ../lib ../lib/widgets ../lib/document

HEADERS += collection.h \
	editorplugin.h \
	colorpickerplugin.h \
	editorconfigplugin.h \
	formatconfigplugin.h

SOURCES += collection.cpp \
	editorplugin.cpp \
	colorpickerplugin.cpp \
	editorconfigplugin.cpp \
	formatconfigplugin.cpp

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS += target
