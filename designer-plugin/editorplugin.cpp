
#include "editorplugin.h"

#include "qeditor.h"

EditorPlugin::EditorPlugin(QObject *p)
 : QObject(p), m_initialized(false)
{
	
}

bool EditorPlugin::isContainer() const
{
	return false;
}

bool EditorPlugin::isInitialized() const
{
	return m_initialized;
}

QIcon EditorPlugin::icon() const
{
	return QIcon();
}

QString EditorPlugin::domXml() const
{
	static const QLatin1String _dom("<widget class=\"QEditor\" name=\"editor\" />");
	return _dom;
}

QString EditorPlugin::group() const
{
	static const QLatin1String _group("QCodeEdit");
	return _group;
}

QString EditorPlugin::includeFile() const
{
	static const QString _include("qeditor.h");
	return _include;
}

QString EditorPlugin::name() const
{
	static const QLatin1String _name("QEditor");
	return _name;
}

QString EditorPlugin::toolTip() const
{
	static const QLatin1String _tooltip("A powerful source code editor widget.");
	return _tooltip;
}

QString EditorPlugin::whatsThis() const
{
	static const QLatin1String _whatthis("A powerful source code editor widget.");
	return _whatthis;
}

QWidget* EditorPlugin::createWidget(QWidget *p)
{
	return new QEditor(p);
}

void EditorPlugin::initialize(QDesignerFormEditorInterface *core)
{
	if ( m_initialized )
		return;
	
	m_initialized = true;
}
