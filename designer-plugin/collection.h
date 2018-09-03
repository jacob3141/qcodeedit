
#ifndef _COLLECTION_H_
#define _COLLECTION_H_

#include <QtCore/qplugin.h>
#include <QtDesigner/QtDesigner>

class Collection : public QObject, public QDesignerCustomWidgetCollectionInterface
{
	Q_OBJECT
	Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)
	
	public:
		Collection(QObject *p = 0);
		
		virtual QList<QDesignerCustomWidgetInterface*> customWidgets() const;
		
	private:
		QList<QDesignerCustomWidgetInterface*> widgets;
};

#endif // _COLLECTION_H_
