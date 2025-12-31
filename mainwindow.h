#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QFile>
#include "osoba.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Load_clicked();
    void on_Save_clicked();
    void on_Add_clicked();
    void on_Delete_clicked();
    void SearchRefresh();
    void wyswietlDane(int index);
    void SearchByLetters(const QString &text);
    void SortujMalejaco();
    void SortujRosnaco();
private:
    Ui::MainWindow *ui;
    QList<Osoba> listaOsob;
    QString currentFileName;
    bool saveToFile();
};

#endif // MAINWINDOW_H
