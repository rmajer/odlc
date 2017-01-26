#include "odlc_dimap_gui_remote.cpp"
#include "Logger.h"
#include "symbols.h"

#include <QTextStream>

Logger::Logger(const QString &filename)
{
	this->Log("Log gestartet.");
	
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
		this->Log("Logfile " + _filename + " geoeffnet.");
	}
}

Logger::~Logger()
{
		
	if (_file != NULL)
	{
		this->Log("Logfile " + _filename + " wird geschlossen");
	
		_file->close();
	}
	
	this->Log("Log geschlossen.");
}


void Logger::Log(const QString &entry)
{
	
	QDateTime t = QDateTime::currentDateTime();
	QString val = t.toString("yyyy-MM-dd hh:mm:ss\t") + entry;
	
	
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