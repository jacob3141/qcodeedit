
#include "formatconfigplugin.h"

#include "qformatconfig.h"

FormatConfigPlugin::FormatConfigPlugin(QObject *p)
 : QObject(p), m_initialized(false)
{
	
}

bool FormatConfigPlugin::isContainer() const
{
	return false;
}

bool FormatConfigPlugin::isInitialized() const
{
	return m_initialized;
}

QIcon FormatConfigPlugin::icon() const
{
	return QIcon();
}

QString FormatConfigPlugin::domXml() const
{
	static const QLatin1String _dom("<widget class=\"QFormatConfig\" name=\"formatConfig\" />");
	return _dom;
}

QString FormatConfigPlugin::group() const
{
	static const QLatin1String _group("QCodeEdit");
	return _group;
}

QString FormatConfigPlugin::includeFile() const
{
	static const QString _include("qformatconfig.h");
	return _include;
}

QString FormatConfigPlugin::name() const
{
	static const QLatin1String _name("QFormatConfig");
	return _name;
}

QString FormatConfigPlugin::toolTip() const
{
	static const QLatin1String _tooltip("A simple config widget for QEditor formattings.");
	return _tooltip;
}

QString FormatConfigPlugin::whatsThis() const
{
	static const QLatin1String _whatthis("A simple config widget for QEditor formattings.");
	return _whatthis;
}

QWidget* FormatConfigPlugin::createWidget(QWidget *p)
{
	return new QFormatConfig(p);
}

void FormatConfigPlugin::initialize(QDesignerFormEditorInterface *core)
{
	if ( m_initialized )
		return;
	
	m_initialized = true;
}
