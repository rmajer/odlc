#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QCloseEvent>

#include "odlc_dimap_gui.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::MenuInfoClicked()
{
	
	this->ui->statusBar->showMessage("Version " + ODLCSymbols::qsProgramName + ODLCSymbols::qsProgramVersion);
	
    QMessageBox msgBox;
	msgBox.setText("Programmversion: " + ODLCSymbols::qsProgramVersion);
	msgBox.setWindowTitle(ODLCSymbols::qsProgramName);
    msgBox.exec();
}


void MainWindow::MenuEndeClicked()
{
	
}

void MainWindow::closeEvent(QCloseEvent *event)  // show prompt when user wants to close app
{
	
	// TODO: Prozess- und Systemlog speichern
	
	event->ignore();
	if (QMessageBox::Yes == QMessageBox::question(this, "Programmende bestaetigen", "Wirklich beenden?", QMessageBox::No | QMessageBox::Yes))
	{
		event->accept();
	}

}