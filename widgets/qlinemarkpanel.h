/****************************************************************************
**
** Copyright (C) 2006-2009 fullmetalcoder <fullmetalcoder@hotmail.fr>
**
** This file is part of the Edyuk project <http://edyuk.org>
** 
** This file may be used under the terms of the GNU General Public License
** version 3 as published by the Free Software Foundation and appearing in the
** file GPL.txt included in the packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#pragma once

/*!
	\file qlinemarkpanel.h
	\brief Definition of the QLineMarkPanel class.
	
	\see QLineMarkPanel
*/

#include "qpanel.h"

#include <QHash>
#include <QIcon>

class QDocumentLine;

class QLineMarkPanel : public QPanel
{
	Q_OBJECT
	
    public:
        QLineMarkPanel(QWidget *p = nullptr);
		virtual ~QLineMarkPanel();
		
		virtual QString type() const;
		
	protected:
		virtual bool paint(QPainter *p, QEditor *e);
		virtual void mousePressEvent(QMouseEvent *e);
		virtual void mouseReleaseEvent(QMouseEvent *e);
		virtual void contextMenuEvent(QContextMenuEvent *e);
		
	private:
		QList<QRect> m_rects;
		QList<int> m_lines;
};

