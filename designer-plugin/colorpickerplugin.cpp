
#include "colorpickerplugin.h"

#include "qsimplecolorpicker.h"

ColorPickerPlugin::ColorPickerPlugin(QObject *p)
 : QObject(p), m_initialized(false)
{
	
}

bool ColorPickerPlugin::isContainer() const
{
	return false;
}

bool ColorPickerPlugin::isInitialized() const
{
	return m_initialized;
}

QIcon ColorPickerPlugin::icon() const
{
	return QIcon();
}

QString ColorPickerPlugin::domXml() const
{
	static const QLatin1String _dom("<widget class=\"QSimpleColorPicker\" name=\"colorPicker\" />");
	return _dom;
}

QString ColorPickerPlugin::group() const
{
	static const QLatin1String _group("QCodeEdit");
	return _group;
}

QString ColorPickerPlugin::includeFile() const
{
	static const QString _include("qsimplecolorpicker.h");
	return _include;
}

QString ColorPickerPlugin::name() const
{
	static const QLatin1String _name("QSimpleColorPicker");
	return _name;
}

QString ColorPickerPlugin::toolTip() const
{
	static const QLatin1String _tooltip("A simple color selection widget.");
	return _tooltip;
}

QString ColorPickerPlugin::whatsThis() const
{
	static const QLatin1String _whatthis("A simple color selection widget.");
	return _whatthis;
}

QWidget* ColorPickerPlugin::createWidget(QWidget *p)
{
	return new QSimpleColorPicker(p);
}

void ColorPickerPlugin::initialize(QDesignerFormEditorInterface *core)
{
	if ( m_initialized )
		return;
	
	m_initialized = true;
}
