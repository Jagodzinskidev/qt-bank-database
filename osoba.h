#ifndef OSOBA_H
#define OSOBA_H

#include <QString>

class Osoba {
private:
    QString imie;
    QString nazwisko;
    unsigned int rok_urodzenia;
    unsigned char miesiac_urodzenia;
    unsigned char dzien_urodzenia;
    float stan_konta;

public:
    Osoba()
        : rok_urodzenia(0)
        , miesiac_urodzenia(0)
        , dzien_urodzenia(0)
        , stan_konta(0.0f)
    {}
    Osoba(const QString &i, const QString &n, unsigned int rok,
          int miesiac, int dzien, float stan)
        : imie(i)
        , nazwisko(n)
        , rok_urodzenia(rok)
        , miesiac_urodzenia(miesiac)
        , dzien_urodzenia(dzien)
        , stan_konta(stan)
    {}

    QString getImie() const { return imie; }
    QString getNazwisko() const { return nazwisko; }
    unsigned int getRokUrodzenia() const { return rok_urodzenia; }
    unsigned char getMiesiacUrodzenia() const { return miesiac_urodzenia; }
    unsigned char getDzienUrodzenia() const { return dzien_urodzenia; }
    float getStanKonta() const { return stan_konta; }

    void setImie(const QString &v) { imie = v; }
    void setNazwisko(const QString &v) { nazwisko = v; }
    void setRokUrodzenia(unsigned int v) { rok_urodzenia = v; }
    void setMiesiacUrodzenia(unsigned char v) { miesiac_urodzenia = v; }
    void setDzienUrodzenia(unsigned char v) { dzien_urodzenia = v; }
    void setStanKonta(float v) { stan_konta = v; }

    QString toString() const {
        return QString("%1,%2,%3,%4,%5,%6")
        .arg(imie)
            .arg(nazwisko)
            .arg(dzien_urodzenia)
            .arg(miesiac_urodzenia)
            .arg(rok_urodzenia)
            .arg(stan_konta, 0, 'f', 2);
    }
};

#endif // OSOBA_H

