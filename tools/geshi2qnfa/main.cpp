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

#include <QDir>
#include <QFile>
#include <QString>
#include <QFileInfo>
#include <QTextStream>
#include <QXmlStreamWriter>

enum Mode
{
	None		= 0,
	Recursive	= 1,
	Tolerant	= 2
};

struct Arguments
{
	int mode;
	QString outputPath;
	QStringList sources;
};

struct PHPArrayNode
{
	PHPArrayNode* node(const QString& s)
	{
		foreach ( PHPArrayNode *c, children )
			if ( c->name == s )
				return c;
		
		return 0;
	}
	
	QString nodeValue(const QString& s)
	{
		PHPArrayNode *n = node(s);
		
		return n ? n->value : QString();
	}
	
	QString name;
	QString value;
	
	QList<PHPArrayNode*> children;
};

int usage();
QString qnfaEscape(const QString& s);
bool parseCommandLine(int argc, char **argv, Arguments& args);
void processArguments(Arguments& args);
void convert(const QString& in, const QString& out);
void dump(const PHPArrayNode *n, QString indent = QString());
void parsePHPFile(PHPArrayNode *n, const QString& s, int start, int end);
void parsePHPArray(PHPArrayNode *n, const QString& s, int start, int end);

int main(int argc, char **argv)
{
	Arguments args;
	
	if ( parseCommandLine(argc, argv, args) )
		return usage();
	
	processArguments(args);
	
	return 0;
}

int usage()
{
	QTextStream o(stdout);
	
	o
		<< "geshi2qnfa (c) 2008 Luc BRUANT aka fullmetalcoder <fullmetalcoder@hotmail.fr>" << endl
		<< "Converts GeSHi syntax files into QCodeEdit syntax files." << endl
		<< "Usage :" << endl
		<< "geshi2qnfa [-h|--hep] [-t|--tolerant] [-r|--recursive] [-o OutputPath] pathes" << endl
		;
	
	return 1;
}

QString qnfaEscape(const QString& s)
{
	QString n;
	static QList<QChar> l = QList<QChar>()
		<< '*'
		<< '+'
		<< '?'
		<< '['
		<< ']'
		<< '('
		<< '$'
		;
	
	for ( int i = 0; i < s.count(); ++i )
	{
		if ( l.contains(s.at(i)) )
		{
			n += QString("\\") + s.at(i);
// 		} else if ( s.at(i) == '\\' ) {
// 			if ( ((i + 1) < s.count()) && (s.at(i + 1) == '\\') )
// 				i += 2;
// 			
// 			n += "\\\\";
// 			
		} else {
			n += s.at(i);
		}
	}
	
	return n;
}


QString qnfaRegex(const QString& s)
{
	QString n;
	bool inClass = false;
	static QList<QChar> cl = QList<QChar>()
		<< 'w'
		<< 'W'
		<< 's'
		<< 'S'
		<< 'd'
		<< 'D'
		;
	
	static QList<QChar> l = QList<QChar>()
		<< '^'
		<< '$'
		;
	
	for ( int i = 0; i < s.count(); ++i )
	{
		if ( inClass )
		{
			if ( s.at(i) == '\\' )
				++i;
			
			if ( s.at(i) == ']' )
				inClass = false;
			
		} else if ( l.contains(s.at(i)) ) {
			continue;
		} else if ( s.at(i) == '\\' ) {
			if ( ((i + 1) < s.count()) && cl.contains(s.at(i + 1)) )
			{
				n += '$';
			} else {
				n += "\\";
			}
			continue;
		} else if ( s.at(i) == '{' ) {
			
			while ( (i < s.count()) && (s.at(i) != '}') )
			{
				++i;
			}
			
			continue;
		} else {
			if ( s.at(i) == '[' )
				inClass = true;
			
		}
		
		n += s.at(i);
	}
	
	return n;
}

bool parseCommandLine(int argc, char **argv, Arguments& args)
{
	if ( argc < 2 )
		return true;
	
	for ( int i = 1; i < argc; ++i )
	{
		QString a = QString::fromLocal8Bit(argv[i]);
		
		if ( a == QLatin1String("-r") || a == QLatin1String("--recursive") )
		{
			args.mode |= Recursive;
		} else if ( a == QLatin1String("-t") || a == QLatin1String("--tolerant") ) {
			args.mode |= Tolerant;
		} else if ( a == QLatin1String("-h") || a == QLatin1String("--help") ) {
			return true;
		} else if ( a == QLatin1String("-o") && ((i + 1) < argc) ) {
			args.outputPath = QString::fromLocal8Bit(argv[++i]);
		} else if ( QFile::exists(a) ) {
			args.sources << a;
		} else {
			qWarning("Invalid parameter : %s", argv[i]);
			
			if ( !(args.mode & Tolerant) )
				return true;
		}
	}
	
	return false;
}

void processArguments(Arguments& args)
{
	QDir out(args.outputPath);
	
	QDir::Filters filters = QDir::Readable | QDir::Files;
	
	if ( args.mode & Recursive )
		filters |= QDir::Dirs | QDir::NoDotAndDotDot;
	
	while ( args.sources.count() )
	{
		QString f = args.sources.takeFirst();
		
		QFileInfo info(f);
		
		if ( info.isDir() )
		{
			QDir d(f);
			QStringList l = d.entryList(filters);
			
			foreach ( const QString& p, l )
				args.sources << d.absoluteFilePath(p);
			
		} else {
			convert(info.absoluteFilePath(), out.absoluteFilePath(info.baseName()) + ".qnfa");
		}
	}
}

void convert(const QString& in, const QString& out)
{
	QFile fin(in), fout(out);
	
	QString definition, convertedDefinition;
	QXmlStreamWriter qnfa(&convertedDefinition);
	qnfa.setAutoFormatting(true);
	
	if ( !fin.open(QFile::ReadOnly | QFile::Text) )
	{
		qWarning("Unable to open %s for reading.", qPrintable(in));
		return;
	}
	
	qDebug("converting %s...", qPrintable(in));
	
	QTextStream sin(&fin);
	definition = sin.readAll();
	
	PHPArrayNode root;
	root.name = "data";
	
	parsePHPFile(
		&root,
		definition,
		definition.indexOf("<?php") + 5,
		definition.lastIndexOf("?>")
	);
	
	//dump(&root);
	//return;
	
	QString lang = root.nodeValue("LANG_NAME");
	
	qnfa.writeStartDocument();
	qnfa.writeStartElement("QNFA");
	qnfa.writeAttribute("language", lang);
	
	PHPArrayNode *k, *c, *n;
	
	n = root.node("COMMENT_SINGLE");
	
	if ( n )
	{
		int idx = 0;
		
		foreach ( c, n->children )
		{
			qnfa.writeStartElement("context");
			qnfa.writeAttribute("id", "comment/single/" + QString::number(idx));
			qnfa.writeAttribute("format", "comment");
			
			qnfa.writeStartElement("start");
			qnfa.writeCharacters(qnfaEscape(c->value));
			qnfa.writeEndElement();
			
			qnfa.writeStartElement("stop");
			qnfa.writeAttribute("exclusive", "false");
			qnfa.writeCharacters("\\n");
			qnfa.writeEndElement();
			
			qnfa.writeEndElement();
			
			++idx;
		}
	}
	
	n = root.node("COMMENT_MULTI");
	
	if ( n )
	{
		int idx = 0;
		
		foreach ( c, n->children )
		{
			QString sidx = QString::number(idx),
					com = lang + "comment" + sidx + ":";
			
			qnfa.writeStartElement("context");
			qnfa.writeAttribute("id", "comment/multi/" + sidx);
			qnfa.writeAttribute("format", "comment");
			
			qnfa.writeStartElement("start");
			qnfa.writeAttribute("parenthesis", com + "open");
			qnfa.writeAttribute("fold", "1");
			qnfa.writeCharacters(qnfaEscape(c->name));
			qnfa.writeEndElement();
			
			qnfa.writeStartElement("stop");
			qnfa.writeAttribute("parenthesis", com + "close");
			qnfa.writeAttribute("fold", "1");
			qnfa.writeCharacters(qnfaEscape(c->value));
			qnfa.writeEndElement();
			
			qnfa.writeEndElement();
			
			++idx;
		}
	}
	
	QString escape = root.nodeValue("ESCAPE_CHAR");
	
	n = root.node("QUOTE_MARKS");
	
	if ( n )
	{
		int idx = 0;
		
		foreach ( c, n->children )
		{
			qnfa.writeStartElement("context");
			qnfa.writeAttribute("id", "data/string/" + QString::number(idx));
			qnfa.writeAttribute("format", "text");
			
			qnfa.writeStartElement("start");
			qnfa.writeCharacters(qnfaEscape(c->name));
			qnfa.writeEndElement();
			
			qnfa.writeStartElement("stop");
			qnfa.writeCharacters(qnfaEscape(c->value));
			qnfa.writeEndElement();
			
			qnfa.writeStartElement("escape");
			qnfa.writeCharacters(escape + "[" + escape + c->value + "]");
			qnfa.writeEndElement();
			
			qnfa.writeEndElement();
			
			++idx;
		}
	}
	
	n = root.node("KEYWORDS");
	
	if ( n )
	{
		foreach ( c, n->children )
		{
			qnfa.writeStartElement("list");
			qnfa.writeAttribute("id", QString("keywords/") + c->name);
			qnfa.writeAttribute("format", "keyword");
			
			foreach ( k, c->children )
				qnfa.writeTextElement("word", k->value);
			
			qnfa.writeEndElement();
		}
	}
	
	n = root.node("REGEXPS");
	
	if ( n )
	{
		foreach ( c, n->children )
		{
			if ( c->value.count() )
			{
				qnfa.writeStartElement("word");
				qnfa.writeAttribute("id", QString("regex/") + c->name);
				qnfa.writeAttribute("format", "keyword");
				qnfa.writeCharacters(qnfaRegex(c->value));
				qnfa.writeEndElement();
			}
		}
	}
	
	qnfa.writeEndDocument();
	
	if ( fout.open(QFile::WriteOnly | QFile::Text) )
	{
		QTextStream sout(&fout);
		sout << convertedDefinition;
	} else {
		qWarning("Unable to open %s for writing.", qPrintable(out));
	}
}

static int fieldEnd(const QString& s, int i, int e)
{
	int nest = 0;
	bool quoted = false, escaped = false;
	
	for ( ; i < e; ++i )
	{
		QChar c = s.at(i);
		
		if ( c == '\'' || c == '\"' )
		{
			++i;
			
			while ( (i < e) && (s.at(i) != c) )
			{
				if ( s.at(i) == '\\' )
					++i;
				
				++i;
			}
			
		} else if ( c == '(' ) {
			++nest;
		} else if ( c == ')' ) {
			if ( nest > 0 )
				--nest;
			else
				return i;
		} else if ( !nest && (c == ',') ) {
			break;
		}
	}
	
	return i;
}

void parsePHPFile(PHPArrayNode *n, const QString& s, int start, int e)
{
	QString buffer;
	QChar lastQuote;
	bool quoted = false, escaped = false;
	
	for ( int i = start; i < e; ++i )
	{
		QChar c = s.at(i);
		
		if ( quoted )
		{
			if ( c == '\\' )
			{
				escaped = !escaped;
			} else if ( !escaped && c == lastQuote ) {
				quoted = false;
				continue;
			} else {
				escaped = false;
			}
		} else if ( (c == '/') && ((i + 1) < e) && ((s.at(i + 1) == '*') || (s.at(i + 1) == '/')) ) {
			++i;
			
			if ( s.at(i) == '*' )
			{
				i = qMin(s.indexOf("*/", i) + 1, e);
			} else {
				i = qMin(s.indexOf("\n", i), e);
			}
			
			continue;
		} else if ( c == ';' ) {
			buffer.clear();
		} else if ( c == '=' ) {
			
			int end = fieldEnd(s, i, e);
			
			//qDebug("buffer = %s", qPrintable(buffer));
			
			if ( buffer == "$language_data" )
			{
				parsePHPArray(
					n,
					s,
					s.indexOf("(", i) + 1,
					end - 1
				);
				
			}
			
			i = end;
			
			buffer.clear();
			continue;
		} else if ( (c == '\"') || (c == '\'') ) {
			quoted = true;
			lastQuote = c;
			continue;
		} else if ( c.isSpace() ) {
			continue;
		}
		
		buffer += c;
	}
}

void parsePHPArray(PHPArrayNode *n, const QString& s, int start, int e)
{
	//qDebug("parsing : \n%s", qPrintable(s.mid(start, e)));
	int index = 0;
	QString buffer;
	QChar lastQuote;
	bool quoted = false, escaped = false;
	
	for ( int i = start; i < e; ++i )
	{
		QChar c = s.at(i);
		
		if ( !quoted && (c == ',') )
		{
			if ( buffer.count() )
			{
				// list...
				//qDebug("field:(value)=(%s)", qPrintable(buffer));
				
				PHPArrayNode *c = new PHPArrayNode;
				c->name = QString::number(index++);
				c->value = buffer;
				n->children << c;
			}
			
			buffer.clear();
			continue;
		} else if ( !quoted && (c == '/') && ((i + 1) < e) && ((s.at(i + 1) == '*') || (s.at(i + 1) == '/')) ) {
			++i;
			
			if ( s.at(i) == '*' )
			{
				i = qMin(s.indexOf("*/", i) + 1, e);
			} else {
				i = qMin(s.indexOf("\n", i), e);
			}
			
			continue;
		} else if ( !quoted && (c == '=') && ((i + 1) < e) && (s.at(i + 1) == '>') ) {
			i += 2;
			
			PHPArrayNode *c = new PHPArrayNode;
			c->name = buffer;
			n->children << c;
			
			int end = fieldEnd(s, i, e), array = s.indexOf("array", i);
			
			while ( s.at(end - 1).isSpace() )
				--end;
			
			//qDebug("field:(name, value)=(%s, %s)", qPrintable(buffer), qPrintable(s.mid(i, end - i)));
			
			if ( array != -1 && array < end )
			{
				parsePHPArray(c, s, s.indexOf("(", i) + 1, end - 1);
			} else {
				QString sub = s.mid(i, end - i).trimmed();
				
				if ( sub.at(0) == '\"' || sub.at(0) == '\'' )
					sub.remove(0, 1);
				
				if ( sub.endsWith('\"') || sub.endsWith('\'') )
					sub.chop(1);
				
				c->value = sub;
			}
			
			i = end;
			buffer.clear();
			continue;
		} else if ( !escaped && quoted &&  (c == lastQuote) ) {
			quoted = false;
			continue;
		} else if ( !quoted && ((c == '\"') || (c == '\'')) ) {
			quoted = true;
			lastQuote = c;
			continue;
		} else if ( quoted && (c == '\\') ) {
			escaped = !escaped;
			continue;
		} else if ( !quoted && c.isSpace() ) {
			continue;
		} else if ( quoted ) {
			escaped = false;
		}
		
		buffer += c;
	}
	
	if ( buffer.isEmpty() )
		return;
	
	PHPArrayNode *c = new PHPArrayNode;
	c->name = QString::number(index);
	c->value = buffer;
	n->children << c;
}

void dump(const PHPArrayNode *n, QString indent)
{
	int cnt = n->children.count();
	
	QString prefix = indent + n->name + ' ';
	
	if ( n->value.count() )
	{
		qDebug("%s= %s\n", qPrintable(prefix), qPrintable(n->value));
		
	} else if ( cnt ) {
		qDebug("%s{", qPrintable(prefix));
		indent += '\t';
		
		foreach ( const PHPArrayNode *c, n->children )
			dump(c, indent);
		
		indent.chop(1);
		qDebug("%s}", qPrintable(indent));
	} else {
		qDebug("%s= ?", qPrintable(prefix));
	}
}
