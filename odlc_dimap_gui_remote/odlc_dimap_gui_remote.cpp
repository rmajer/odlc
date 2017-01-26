#include "MainWindow.h"
#include <QApplication>
#include <QSettings>
#include <QString>

#include "odlc_dimap_gui.h"

QSettings *settings;


// allgemeine Initialisierungen
void init(void)
{
	
	QCoreApplication::setOrganizationName(ODLCSymbols::qsOrganizationName);
	QCoreApplication::setOrganizationDomain(ODLCSymbols::qsOrganizationDomain);
	QCoreApplication::setApplicationName(ODLCSymbols::qsProgramName);
	
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	settings = new QSettings();
	
	MainWindow w;
	w.setWindowTitle(ODLCSymbols::qsProgramName + " (" + ODLCSymbols::qsProgramVersion + ")");
    w.show();
    
	a.exec();
	
	
    return 0;
}
