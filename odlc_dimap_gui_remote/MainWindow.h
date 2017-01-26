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

	
protected slots :
	void MenuInfoClicked();
	void MenuEndeClicked();
	
	void MenuSystemLogClicked();

	void ButtonInitialisierenClicked();
	
private:
    
	Ui::MainWindow *ui;
	
	FormSystemLog *log;
	
	void closeEvent(QCloseEvent *event);
	
	// aufraeumen und alles noetige schliessen
	void cleanup();	
	
	MCP2210Device *myMCP2210;

};

//#endif // MAINWINDOW_H
