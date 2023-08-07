#include "mainwindow.h"
#include "ui_mainwindow.h"

#define BUF_SIZE 256

static char VoltsString[BUF_SIZE + 1];

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	setWindowTitle("GPIB");
	connect(ui->pushButton,SIGNAL(clicked()),SLOT(InitMeter()));
	connect(ui->pushButton_2,SIGNAL(clicked()),SLOT(WRITE_READ()));
    connect(ui->pushButton_3,SIGNAL(clicked()),SLOT(WRITE()));
    scrollBar = ui->textEdit->verticalScrollBar();
}
MainWindow::~MainWindow() {
	delete ui;
}
void MainWindow::InitMeter() {
    lib = LoadLibrary(L"gpib488.dll");

	typedef int (WINAPI MES1)(int,int,int,int,int,int);
    MES1 *fibdev;

    if (!lib) {
		QMessageBox::critical(this,"Ошибка","Библиотека не загружена.\nПрограмма будет завершена.");
		qApp->exit();
		return;
	}

    fibdev = (MES1*)::GetProcAddress((HMODULE)lib,"ibdev");//Получаем указания на функции

    if (!fibdev)
		QMessageBox::critical(this,"Ошибка","Устройство не найдено.");

    UD = (*fibdev)(ui->spinBox->value(),ui->spinBox_2->value(),0,15,1,0);//Open

    if (UD > 0) {
        for (int i = 0; i < BUF_SIZE; i++)
            VoltsString[i] = 0;
	}
}
void MainWindow::WRITE(int device,const char *command) {
	typedef unsigned long(WINAPI MES2) (int ud,const char *command,unsigned long lenght);
    MES2 *fibwrt;

    fibwrt = (MES2*)::GetProcAddress((HMODULE)lib,"ibwrt");

    if (!fibwrt)
		QMessageBox::critical(this,"Ошибка","Невозножно записать.");

	size_t cmdlength = strlen(command);
    fibwrt(device,command,unsigned(cmdlength));//Send the instrument command
}
void MainWindow::READ(int device,char *bufer,size_t bufsize) {
    typedef unsigned long(WINAPI MES3) (int ud,void *buf, size_t cnt );
    MES3 *fibrd;

	fibrd = (MES3*)::GetProcAddress((HMODULE)lib,"ibrd");

    if (!fibrd)
		QMessageBox::critical(this,"Ошибка","Невозножно считать.");

    fibrd(device,bufer,bufsize);//Read the data back from the device

    ui->textEdit->setText(ui->textEdit->toPlainText() + bufer);

	scrollBar->setValue(scrollBar->maximum());

    //QString value = (const char*)bufer;//Преобразовываю char в string
}
void MainWindow::WRITE_READ() {
	QByteArray array = ui->lineEdit->text().toUtf8();
	WRITE(UD,array);
    READ(UD,VoltsString,BUF_SIZE);
}
void MainWindow::WRITE() {
    QByteArray array = ui->lineEdit->text().toUtf8();
    WRITE(UD,array);
}
