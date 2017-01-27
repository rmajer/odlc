#include "MainWindow.h"
#include <QApplication>
#include <QSettings>
#include <QString>

#include "odlc_dimap_gui.h"


Logger *odlc_logger;
Logger *daten_logger;
QSettings *settings;

// allgemeine Initialisierungen
void init(void)
{
	
	QCoreApplication::setOrganizationName(ODLCSymbols::qsOrganizationName);
	QCoreApplication::setOrganizationDomain(ODLCSymbols::qsOrganizationDomain);
	QCoreApplication::setApplicationName(ODLCSymbols::qsProgramName);
	
	settings = new QSettings();
	
	odlc_logger = new Logger(settings->value("logger/filename", ODLCSymbols::qsLoggerDefaultFilename).toString(), "SYSTEM");
	daten_logger = new Logger(settings->value("daten/filename", ODLCSymbols::qsDatenDefaultFilename).toString(), "LASERS");
}

// initialisierungen nachdem die app und das mainwindow erstellt wurden
void initApp(void)
{
	
	QObject::connect(daten_logger, SIGNAL(externalLog(const QString&)), odlc_logger, SLOT(Log(const QString&)));
	
}

void cleanup(void)
{
	
	if (NULL != odlc_logger)
	{
		delete odlc_logger;
	}
	
}


int main(int argc, char *argv[])
{
	// initialisieren
	init();
	
	
	QApplication a(argc, argv);
	MainWindow w;
	
	
	w.setWindowTitle(ODLCSymbols::qsProgramName + " (" + ODLCSymbols::qsProgramVersion + ")");
    w.show();
    
	// Signale/Slots etc
	initApp();
	
	
	a.exec();
	
	
	
	// aufraeumen
	cleanup();
	
    return 0;
}
