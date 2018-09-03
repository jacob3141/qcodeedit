
#include "editorconfigplugin.h"

#include "qeditconfig.h"

EditorConfigPlugin::EditorConfigPlugin(QObject *p)
 : QObject(p), m_initialized(false)
{
	
}

bool EditorConfigPlugin::isContainer() const
{
	return false;
}

bool EditorConfigPlugin::isInitialized() const
{
	return m_initialized;
}

QIcon EditorConfigPlugin::icon() const
{
	return QIcon();
}

QString EditorConfigPlugin::domXml() const
{
	static const QLatin1String _dom("<widget class=\"QEditConfig\" name=\"editorConfig\" />");
	return _dom;
}

QString EditorConfigPlugin::group() const
{
	static const QLatin1String _group("QCodeEdit");
	return _group;
}

QString EditorConfigPlugin::includeFile() const
{
	static const QString _include("qeditconfig.h");
	return _include;
}

QString EditorConfigPlugin::name() const
{
	static const QLatin1String _name("QEditConfig");
	return _name;
}

QString EditorConfigPlugin::toolTip() const
{
	static const QLatin1String _tooltip("Simple configuration widget for QEditor.");
	return _tooltip;
}

QString EditorConfigPlugin::whatsThis() const
{
	static const QLatin1String _whatthis("Simple configuration widget for QEditor.");
	return _whatthis;
}

QWidget* EditorConfigPlugin::createWidget(QWidget *p)
{
	return new QEditConfig(p);
}

void EditorConfigPlugin::initialize(QDesignerFormEditorInterface *core)
{
	if ( m_initialized )
		return;
	
	m_initialized = true;
}
