#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "FormSystemLog.h"
#include <QMessageBox>
#include <QCloseEvent>

#include "odlc_dimap_gui.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	
	this->log = new FormSystemLog();
	
	// Logging anbinden
	QObject::connect(this, SIGNAL(lineToLog(const QString&)), odlc_logger, SLOT(Log(const QString&)));
	
	// Systemlog-Form und Statusbar anbinden
	QObject::connect(odlc_logger, SIGNAL(lineLogged(const QString&)), this->log, SLOT(addLogLine(const QString&)));
	QObject::connect(daten_logger, SIGNAL(lineLogged(const QString&)), this->log, SLOT(addLogLine(const QString&)));
	
	QObject::connect(odlc_logger, SIGNAL(lineLogged(const QString&)), this->ui->statusBar, SLOT(showMessage(const QString&)));
	
	// an sich selbst Signale senden, wenn der Laser aus irgendeinem Grund de-initialisiert wurde
	QObject::connect(this, SIGNAL(sigLaserInitialized(void)), this, SLOT(LaserInitialized(void)));
	QObject::connect(this, SIGNAL(sigLaserDeInitialized(void)), this, SLOT(LaserDeInitialized(void)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::MenuInfoClicked()
{
	
	//this->ui->statusBar->showMessage("Version " + ODLCSymbols::qsProgramName +" "+ ODLCSymbols::qsProgramVersion);
	
	//odlc_logger->Log("Versionsinformation : " + ODLCSymbols::qsProgramName +" "+ ODLCSymbols::qsProgramVersion);
	
	emit this->lineToLog("Program version : " + ODLCSymbols::qsProgramName + " " + ODLCSymbols::qsProgramVersion);
	
    QMessageBox msgBox;
	msgBox.setText("Program version: " + ODLCSymbols::qsProgramVersion);
	msgBox.setWindowTitle(ODLCSymbols::qsProgramName);
    msgBox.exec();
}


void MainWindow::MenuEndeClicked()
{
	
}


void MainWindow::ButtonInitialisierenClicked()
{
	
	emit this->sigLaserDeInitialized();
	
	// damit man den button nicht beliebig oft clicken kann
	this->ui->pbMCP2210Init->setDisabled(true);
	
	delete myMCP2210;
	
	myMCP2210 = new MCP2210Device();
	
	QObject::connect(myMCP2210, SIGNAL(lineToLog(const QString &)), odlc_logger, SLOT(Log(const QString &)));
	
	myMCP2210->readDeviceConfig();
	
	// button wieder aktivieren
	this->ui->pbMCP2210Init->setDisabled(false);
	
	emit this->sigLaserInitialized();
}


void MainWindow::MenuSystemLogClicked()
{
	
	if (NULL != odlc_logger)
	{
		this->log->setLogContent(odlc_logger->getLogContent());
	}
	
	this->log->show();
	
}


void MainWindow::closeEvent(QCloseEvent *event)  // show prompt when user wants to close app
{
	
	event->ignore();
	if (QMessageBox::Yes == QMessageBox::question(this, tr("Quitting program"), tr("Do you want to quit?"), QMessageBox::No | QMessageBox::Yes))
	{
		
		this->cleanup();
		
		event->accept();
	}

}


void MainWindow::cleanup()
{
	
	if (NULL != log)
	{
		log->close();
	}
	
}


void MainWindow::LaserInitialized()
{
	this->_laserInitialized = true;
	
	// startbutton etc freigeben
	
	// und fertig melden
	emit this->lineToLog("Laser subsystem initialized.");
}


void MainWindow::LaserDeInitialized()
{
	this->_laserInitialized = false;
	// buttons etc sperren
	
	// und fertig
	emit this->lineToLog("Laser subsystem de-initialized.");
}