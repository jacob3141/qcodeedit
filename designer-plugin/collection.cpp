
#include "collection.h"

#include "editorplugin.h"
#include "colorpickerplugin.h"
#include "editorconfigplugin.h"
#include "formatconfigplugin.h"

Collection::Collection(QObject *p)
 : QObject(p)
{
	widgets << new EditorPlugin(this);
	widgets << new ColorPickerPlugin(this);
	widgets << new EditorConfigPlugin(this);
	widgets << new FormatConfigPlugin(this);
}

QList<QDesignerCustomWidgetInterface*> Collection::customWidgets() const
{
	return widgets;
}

Q_EXPORT_PLUGIN2(qcodeeditcollection, Collection);
