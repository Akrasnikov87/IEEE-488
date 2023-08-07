#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void InitMeter();
	void WRITE(int,const char *);
	void READ(int,char*,size_t);

private slots:
	void WRITE_READ();
    void WRITE();

private:
	Ui::MainWindow *ui;
	HINSTANCE lib;
	int UD;
	QScrollBar *scrollBar;
};
#endif // MAINWINDOW_H
