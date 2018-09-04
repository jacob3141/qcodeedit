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
	\file qpanel.h
	\brief Definition of the QPanel class
	
	\see QPanel
	
	\defgroup widgets
*/


#include <QHash>
#include <QWidget>
#include <QPointer>

class QPainter;
class QPaintEvent;

class QEditor;
class QPanelCreator;

class  QPanel : public QWidget {
	Q_OBJECT
	
	public:
        QPanel(QWidget *p = nullptr);
		virtual ~QPanel();

		virtual QString type() const = 0;
		
		QEditor* editor();
		void attach(QEditor *e);
		
		virtual bool shallShow() const;
		
		bool defaultVisibility() const;
		void setDefaultVisibility(bool on);
		
	protected:
		virtual bool forward(QMouseEvent *e);
		
		virtual void editorChange(QEditor *e);
		
		virtual void mouseMoveEvent(QMouseEvent *e);
		virtual void mousePressEvent(QMouseEvent *e);
		virtual void mouseReleaseEvent(QMouseEvent *e);
		
		virtual void showEvent(QShowEvent *e);
		virtual void hideEvent(QHideEvent *e);
		virtual void paintEvent(QPaintEvent *e);
		virtual bool paint(QPainter *p, QEditor *e);
		
	private:
		QPointer<QEditor> m_editor;
        bool m_defaultVisibility, m_shownOnce;
};
