#include "odlc_dimap_gui_remote.cpp"
#include "Logger.h"
#include "symbols.h"

#include <QTextStream>

Logger::Logger(const QString &filename, const QString prefix)
{
	this->_logPrefix = prefix;
	
	this->Log("Start logging.");
	
	this->_filename = filename;
	if (!_filename.isEmpty())
	{
		this->_file = new QFile();
		this->_file->setFileName(this->_filename);
		this->_file->open(QIODevice::Append | QIODevice::Text);
	}
	
	if (_file != NULL)
	{
		this->_fileOpened = true;
		this->Log("Opened logfile " + _filename + ".");
		
		// Datei-Operationen an externen Logger melden
		QString tmp = "Opened logfile " + _filename + ".";
		if (!this->_logPrefix.isEmpty())
		{
			tmp = this->_logPrefix + ": " + tmp;
		}
		emit this->externalLog(tmp);
	}
	
}

Logger::~Logger()
{
		
	if (_file != NULL)
	{
		this->Log("Closing logfile " + _filename + ".");
	
		_file->close();
	}
	
	this->Log("Log closed.");
}


void Logger::Log(const QString &entry)
{
	
	QDateTime t = QDateTime::currentDateTime();
	QString val = t.toString("yyyy-MM-dd hh:mm:ss\t") + entry;
	
	if (!this->_logPrefix.isEmpty())
	{
		val = this->_logPrefix + "\t" + val;
	}
	
	_log.push_back(val);
	if (this->_fileOpened)
	{
		QTextStream out(_file);
		out << val << "\n";
		out.flush();
	}
	emit lineLogged(val);
}


QString Logger::getLogContent()
{

	QString tmp;
	
	//std::map<QDateTime, QString>::iterator itr;
	std::vector<QString>::iterator itr;
	
	for (itr = _log.begin(); itr != _log.end(); ++itr)
	{
		//tmp += itr->first.toString("yyyy-MM-dd hh:mm:ss") + "\t" + itr->second + "\n";
		tmp += *itr + "\n";
	}
	
	return tmp;
}

void Logger::clear()
{
	this->_log.clear();
}

void Logger::setLogPrefix(const QString &prefix)
{
	this->_logPrefix = prefix;
}