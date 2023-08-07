#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	MainWindow gpib;
	gpib.show();

	return a.exec();
}
