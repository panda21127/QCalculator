#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Calculator.h"

CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CMainWindow)
{
    ui->setupUi(this);
}

CMainWindow::~CMainWindow()
{
    delete ui;
}


void CMainWindow::on_btnCalc_clicked()
{
    CCalculator calc;
    double result = 0.0;

    QString strInput = ui->textInput->toPlainText();

    if (strInput.isEmpty())
    {
        ui->textResult->setText("Empty");
        return;
    }

    try
    {
        QString reversePolish = calc.createReversePolish(strInput);

        ui->textReversePoland->setText(reversePolish);

        result = calc.math(reversePolish);
    }
    catch (const char* errorMessage)
    {
        ui->textReversePoland->setText(errorMessage);
        ui->textResult->setText(errorMessage);
        return;
    }

    ui->textResult->setText(QString::number(result));
}

