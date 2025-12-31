#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "osoba.h"
#include <QTextStream>
#include <QMessageBox>
#include <QtAlgorithms>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->Search, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::wyswietlDane);
    connect(ui->SearchByLetters, &QLineEdit::textChanged, this, &MainWindow::SearchByLetters);
    connect(ui->Malejaco, &QPushButton::clicked, this, &MainWindow::SortujMalejaco);
    connect(ui->Rosnaco, &QPushButton::clicked, this, &MainWindow::SortujRosnaco);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::saveToFile()
{
    if (currentFileName.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Nie wczytano pliku!");
        return false;
    }

    QFile file(currentFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Błąd", "Nie udało się zapisać pliku!");
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    for (const Osoba &o : listaOsob)
        out << o.toString() << ";";

    file.close();
    return true;
}


void MainWindow::SearchRefresh()
{
    ui->Search->clear();
    for (int i = 0; i < listaOsob.size(); ++i) {
        const Osoba &o = listaOsob[i];
        ui->Search->addItem(o.getImie() + " " + o.getNazwisko(), i);
    }
}

void MainWindow::on_Load_clicked()
{
    currentFileName = QFileDialog::getOpenFileName(this, tr("Otwórz Plik"), "", tr("Plik tekstowy (*.txt)"));
    QFile file(currentFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Błąd", "Nie udało się otworzyć pliku!");
        return;
    }
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString dane = in.readAll();
    file.close();
    QStringList rekordy = dane.split(';', Qt::SkipEmptyParts);
    listaOsob.clear();

    for (const QString &rekord : rekordy) {
        QStringList pola = rekord.split(',', Qt::SkipEmptyParts);
        if (pola.size() == 6) {
            listaOsob.append(Osoba(
                pola[0].trimmed(),      // imie
                pola[1].trimmed(),      //nazwisko
                pola[4].toUInt(),       // rok
                pola[3].toUInt(),     // miesiąc
                pola[2].toUInt(),     // dzień
                pola[5].toFloat()       // stan konta
                ));
        }
    }
    SearchRefresh();
}
void MainWindow::wyswietlDane(int index)
{
    if (index < 0 || index >= ui->Search->count()){
        ui->Imie->clear();
        ui->Nazwisko->clear();
        ui->DzienUrodzenia->clear();
        ui->MiesiacUrodzenia->clear();
        ui->RokUrodzenia->clear();
        ui->StanKonta->clear();
        return;
    }
    int realIndex = ui->Search->itemData(index).toInt();
    if (realIndex < 0 || realIndex >= listaOsob.size()) return;
    const Osoba &o = listaOsob[realIndex];

    ui->Imie->setText(o.getImie());
    ui->Nazwisko->setText(o.getNazwisko());
    ui->DzienUrodzenia->setText(QString::number(o.getDzienUrodzenia()));
    ui->MiesiacUrodzenia->setText(QString::number(o.getMiesiacUrodzenia()));
    ui->RokUrodzenia->setText(QString::number(o.getRokUrodzenia()));
    ui->StanKonta->setText(QString::number(o.getStanKonta(), 'f', 2));
}

void MainWindow::on_Save_clicked()
{
    int comboIndex = ui->Search->currentIndex();
    if (comboIndex < 0) return;
    int realIndex = ui->Search->itemData(comboIndex).toInt();
    if (realIndex < 0 || realIndex >= listaOsob.size()) return;
    Osoba &o = listaOsob[realIndex];

    o.setImie(ui->Imie->toPlainText());
    o.setNazwisko(ui->Nazwisko->toPlainText());
    o.setDzienUrodzenia(ui->DzienUrodzenia->toPlainText().toUInt());
    o.setMiesiacUrodzenia(ui->MiesiacUrodzenia->toPlainText().toUInt());
    o.setRokUrodzenia(ui->RokUrodzenia->toPlainText().toUInt());
    o.setStanKonta(ui->StanKonta->toPlainText().toFloat());

    if (saveToFile()) {
        QMessageBox::information(this, "Sukces", "Dane zapisane pomyślnie!");
        SearchRefresh();
    }
}

void MainWindow::SearchByLetters(const QString &text)
{
    ui->Search->clear();

    for (int i = 0; i < listaOsob.size(); ++i) {
        const Osoba &o = listaOsob[i];
        QString imie = o.getImie();

        if (text.isEmpty() || imie.startsWith(text, Qt::CaseSensitive)) {
            ui->Search->addItem(o.getImie() + " " + o.getNazwisko(), i);
        }
    }

    if (ui->Search->count() > 0) {
        ui->Search->setCurrentIndex(0);
        wyswietlDane(0);
    } else {
        ui->Imie->clear();
        ui->Nazwisko->clear();
        ui->DzienUrodzenia->clear();
        ui->MiesiacUrodzenia->clear();
        ui->RokUrodzenia->clear();
        ui->StanKonta->clear();
    }
}
void MainWindow::SortujMalejaco()
{
    qSort(listaOsob.begin(), listaOsob.end(), [](const Osoba &a, const Osoba &b){
        return a.getStanKonta() > b.getStanKonta();
    });

    if (saveToFile()) {
        SearchRefresh();
        QMessageBox::information(this, "Sukces", "Dane zostały posortowane malejąco i zapisane!");
    }
}

void MainWindow::SortujRosnaco()
{
    qSort(listaOsob.begin(), listaOsob.end(), [](const Osoba &a, const Osoba &b){
        return a.getStanKonta() < b.getStanKonta();
    });

    if (saveToFile()) {
        SearchRefresh();
        QMessageBox::information(this, "Sukces", "Dane zostały posortowane rosnąco i zapisane!");
    }
}

void MainWindow::on_Add_clicked()
{
    QString imie = ui->Imie->toPlainText().trimmed();
    QString nazwisko = ui->Nazwisko->toPlainText().trimmed();
    ushort dzien = ui->DzienUrodzenia->toPlainText().toUInt();
    ushort miesiac = ui->MiesiacUrodzenia->toPlainText().toUInt();
    uint rok = ui->RokUrodzenia->toPlainText().toUInt();
    float stanKonta = ui->StanKonta->toPlainText().toFloat();

    if (imie.isEmpty() || nazwisko.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Uzupełnij wszystkie pola przed dodaniem nowej osoby!");
        return;
    }

    listaOsob.append(Osoba(imie, nazwisko, rok, miesiac, dzien, stanKonta));

    if (saveToFile()) {
        SearchRefresh();
        QMessageBox::information(this, "Sukces", "Nowa osoba została dodana!");
    }
}

void MainWindow::on_Delete_clicked()
{
    int comboIndex = ui->Search->currentIndex();
    if (comboIndex < 0) {
        QMessageBox::warning(this, "Błąd", "Nie wybrano osoby do usunięcia!");
        return;
    }

    int realIndex = ui->Search->itemData(comboIndex).toInt();

    if (realIndex < 0 || realIndex >= listaOsob.size()) return;

    auto confirm = QMessageBox::question(this, "Potwierdź", "Czy na pewno chcesz usunąć tę osobę?", QMessageBox::Yes | QMessageBox::No);
    if (confirm != QMessageBox::Yes)
        return;

    listaOsob.removeAt(realIndex);

    if (saveToFile()) {
        SearchRefresh();
        ui->Imie->clear();
        ui->Nazwisko->clear();
        ui->DzienUrodzenia->clear();
        ui->MiesiacUrodzenia->clear();
        ui->RokUrodzenia->clear();
        ui->StanKonta->clear();
        QMessageBox::information(this, "Sukces", "Osoba została usunięta!");
    }
}
