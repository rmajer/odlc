#pragma once

//#ifndef _LOGGER_H
//#define _LOGGER_H

#include <QObject>
#include <QString>
#include <QTime>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

#include <map>
#include <vector>

#include "symbols.h"

using namespace std;


class Logger : public QObject
{
	Q_OBJECT
		
public:
	
	Logger(const QString &filename);
	~Logger();
	
	QString getLogContent();
	void clear();
	
signals:
	void lineLogged(const QString &line);
	
public slots :
	
	void Log(const QString &entry);

	
private:
	
	QString _filename;
	QFile *_file;

	bool _fileOpened = false;
	
	//std::map<QDateTime, QString> _log;
	std::vector<QString> _log;
};


//#endif
