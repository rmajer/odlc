#pragma once

//#ifndef _FORMSYSTEMLOG_H
//#define _FORMSYSTEMLOG_H

#include <QWidget>

namespace Ui
{
	class FormSystemLog;
}


class FormSystemLog : public QWidget
{
	
	Q_OBJECT
		
public :
	
	explicit FormSystemLog(QWidget *parent = 0);
	~FormSystemLog();
	
public slots :
	
	void setLogContent(const QString &entry);
	void syslog_AktualisierenClicked();
	void syslog_ResetClicked();
	void syslog_ExportierenClicked();
	
	void addLogLine(const QString &line);
	
private:
	
	Ui::FormSystemLog *ui;
	
	
};


//#endif // !_FORMSYSTEMLOG_H
