#pragma once

//#ifndef MAINWINDOW_H
//#define MAINWINDOW_H

#include <QMainWindow>
#include "FormSystemLog.h"

#include "mcp2210device.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
	
	void sigLaserInitialized();
	void sigLaserDeInitialized();
	
	void lineToLog(const QString &line);
	
protected slots :
	void MenuInfoClicked();
	void MenuEndeClicked();
	
	void MenuSystemLogClicked();

	void ButtonInitialisierenClicked();
	
	// alle Systeme wurden korrekt initialisiert; es kann gestartet werden
	void LaserInitialized();
	
	// es wurde etwas geaendert, Neu-Initialisierung erforderlich
	void LaserDeInitialized();
	
private:
    
	bool _laserInitialized = false;
	
	Ui::MainWindow *ui;
	
	FormSystemLog *log;
	
	void closeEvent(QCloseEvent *event);
	
	// aufraeumen und alles noetige schliessen
	void cleanup();	
	
	MCP2210Device *myMCP2210;

};

//#endif // MAINWINDOW_H
