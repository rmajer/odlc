#include "FormSystemLog.h"
#include "ui_SystemLog.h"
#include "odlc_dimap_gui.h"

FormSystemLog::FormSystemLog(QWidget *parent)
	: QWidget(parent),	
	  ui(new Ui::FormSystemLog)
{
	ui->setupUi(this);
	
	
}

FormSystemLog::~FormSystemLog()
{
	delete this->ui;
}

void FormSystemLog::setLogContent(const QString &entry)
{
	
	this->ui->pteSystemLog->clear();
	this->ui->pteSystemLog->appendPlainText(entry);
	
}


void FormSystemLog::addLogLine(const QString &line)
{
	this->ui->pteSystemLog->appendPlainText(line);
}


void FormSystemLog::syslog_AktualisierenClicked()
{
	
	this->setLogContent(odlc_logger->getLogContent());
	
}

void FormSystemLog::syslog_ResetClicked()
{
	this->ui->pteSystemLog->clear();
	odlc_logger->clear();
	odlc_logger->Log("Logansicht zurueckgesetzt");
	this->setLogContent(odlc_logger->getLogContent());
	
}

void FormSystemLog::syslog_ExportierenClicked()
{
	
}