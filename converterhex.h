#ifndef CONVERTERHEX_H
#define CONVERTERHEX_H

#include <QMainWindow>
#include <QMainWindow>
#include <QFileDialog>
#include <QLabel>
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QString>


QT_BEGIN_NAMESPACE
namespace Ui { class ConverterHex; }
QT_END_NAMESPACE

class ConverterHex : public QMainWindow
{
    Q_OBJECT

public:
    ConverterHex(QWidget *parent = nullptr);
    ~ConverterHex();

private slots:
    void on_ButOpenFile_clicked();

    void on_pushButton_clicked();

private:
    Ui::ConverterHex *ui;
};
#endif // CONVERTERHEX_H
