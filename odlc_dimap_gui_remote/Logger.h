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
	
	Logger(const QString &filename, const QString prefix = "");
	~Logger();
	
	QString getLogContent();
	void clear();
	
	// soll vor dem Logeintrag ein Prefix stehen?
	void setLogPrefix(const QString &prefix);
	
	
signals:
	void lineLogged(const QString &line);
	
	// um ein Signal an eine andere Log-Instanz zu schicken und dort Status-MSGs zu deponieren
	void externalLog(const QString &line);
	
public slots :
	
	void Log(const QString &entry);

	
private:
	
	QString _filename;
	QFile *_file;
	QString _logPrefix;
	
	bool _fileOpened = false;
	
	//std::map<QDateTime, QString> _log;
	std::vector<QString> _log;
};


//#endif
