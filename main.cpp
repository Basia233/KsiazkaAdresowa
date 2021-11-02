#include <iostream>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct Adresat
{
    int id = 0, idUzytkownika = 0;
    string imie = "", nazwisko = "", telefon = "", mail = "", adres = "";
};

struct Uzytkownik
{
    int idUzytkownika=0;
    string nazwa = "", haslo = "";
};


string zamienIntNaString(int idOsoby)
{
    ostringstream ss;
    ss << idOsoby;
    string str = ss.str();
    return str;
}


int rejestracja (vector <Uzytkownik> &uzytkownicy)
{
    Uzytkownik osobyDoEksportu;
    int idOsoby=0;
    string nazwa, haslo;

    idOsoby = uzytkownicy.size()+1;

    system("cls");
    cout << "Podaj nazwe uzytkownika: ";
    cin >> nazwa;

    int i=0;

    while (i<uzytkownicy.size())
    {
        if (uzytkownicy[i].nazwa == nazwa)
        {
            cout << "Taki uzytkownik istnieje. Wpisz inna nazwe uzytkownika: ";
            cin >> nazwa;
            i = 0;
        }
        else
        {
            i++;
        }
    }

    cout << "Podaj haslo: ";
    cin >> haslo;

    osobyDoEksportu.idUzytkownika = idOsoby;
    osobyDoEksportu.nazwa = nazwa;
    osobyDoEksportu.haslo = haslo;

    uzytkownicy.push_back(osobyDoEksportu);

    fstream plik;
    plik.open("Uzytkownicy.txt", ios::out | ios::app);

    if (plik.good()==true)
    {

        plik << zamienIntNaString(idOsoby) + "|" +
             nazwa + "|" + haslo + "|" << endl;


        plik.close();

        cout << "Uzytkownik zostal zarejestrowany." << endl;
        Sleep(1500);
    }

    else
    {
        cout << "Nie udalo sie otworzyc pliku" << endl;
    }
}


void wczytajUzytkownikowZPliku(vector <Uzytkownik> &uzytkownicy)
{
    Uzytkownik osoby;
    int id;
    string nazwa, haslo;
    string linia = "";
    int nrLinii=1;
    int nrOsoby=0;

    fstream plik;
    plik.open("Uzytkownicy.txt", ios::in);

    if(plik.good()==true)
    {
        while(getline(plik, linia, '|'))
        {
            switch (nrLinii)
            {
            case 1:
                osoby.idUzytkownika = atoi(linia.c_str());
                break;
            case 2:
                osoby.nazwa = linia;
                break;
            case 3:
                osoby.haslo = linia;
                break;
            }
            if (nrLinii==3)
            {
                nrLinii=0;
                nrOsoby++;
                uzytkownicy.push_back(osoby);
            }
            nrLinii++;
        }
        plik.close();
    }
}


int logowanie(vector <Uzytkownik> uzytkownicy)
{
    string nazwa, haslo;
    cout << "Podaj nazwe: ";
    cin >>  nazwa;
    int i=0;

    while (i<uzytkownicy.size())
    {
        if (uzytkownicy[i].nazwa == nazwa)
        {
            for (int proby=0; proby<3; proby++)
            {
                cout << "Podaj haslo. Pozostalo prob " << 3-proby << ": ";
                cin >> haslo;
                if (uzytkownicy[i].haslo == haslo)
                {
                    cout << "Zalogowales sie. " << endl;
                    Sleep(1000);
                    return uzytkownicy[i].idUzytkownika;
                }

            }
            cout << "Podales 3 razy bledne haslo. Poczekaj 3 sekundy przed kolejna proba" << endl;
            Sleep(3000);
            return 0;
        }
        i++;
    }
    cout << "Nie ma uzytkownika z takim loginem" << endl;
    Sleep(1500);
    return 0;
}


void zapiszDaneUzytkownikowWPliku(vector <Uzytkownik> &uzytkownicy)
{
    fstream plik;
    plik.open("Uzytkownicy.txt", ios::out);


    if(plik.good()== true)
    {
        for(int i =0; i<uzytkownicy.size(); i++)
        {

            plik << zamienIntNaString(uzytkownicy[i].idUzytkownika) + "|" +
                 uzytkownicy[i].nazwa + "|" + uzytkownicy[i].haslo + "|" << endl;
        }
        plik.close();
    }
}


void zmianaHasla(vector <Uzytkownik> uzytkownicy, int idZalogowanegoUzytkownika)
{
    string haslo;
    cout << "Podaj nowe haslo: ";
    cin >> haslo;

    for (int i=0; i<uzytkownicy.size(); i++)
    {
        if (uzytkownicy[i].idUzytkownika == idZalogowanegoUzytkownika)
        {
            uzytkownicy[i].haslo = haslo;

            zapiszDaneUzytkownikowWPliku(uzytkownicy);
            cout << "Haslo zostalo zmienione " << endl;
            Sleep(1500);
        }
    }
}


int sprawdzLiczbeKontaktowWPliku()
{
    int calkowitaLiczbaKontaktow;
    int numerLinii = 0;
    string linia;

    fstream plik;
    plik.open("Kontakty.txt", ios::in);

    if(plik.good()== false)
    {
        calkowitaLiczbaKontaktow =0;
    }
    else
    {
        while (getline(plik, linia))
        {
            numerLinii++;
        }
        calkowitaLiczbaKontaktow = numerLinii;
    }
     plik.close();

    return calkowitaLiczbaKontaktow;
}


int nadajKolejnyNumerIdKontaktu(int liczbaWszystkichKontaktow)
{
    string pobranaLinia;
    int idKontaktuWOstatniejLinii = 0;

    fstream plik;

    plik.open("Kontakty.txt", ios::in);

    for (int i=0; i < liczbaWszystkichKontaktow; i++)
    {
        getline(plik, pobranaLinia);

        char szukanyZnak = '|';
        int pozycjaZnaku = pobranaLinia.find(szukanyZnak);
        idKontaktuWOstatniejLinii = atoi((pobranaLinia.substr(0,pozycjaZnaku)).c_str());
    }

    plik.close();

    return idKontaktuWOstatniejLinii;

}



void wczytajKontaktyZPliku(vector <Adresat> &adresaci, int idZalogowanegoUzytkownika)
{
    Adresat osoby;
    int id=0, idUzytkownika=0;

    idUzytkownika = idZalogowanegoUzytkownika;

    string imie, nazwisko, telefon, mail, adres;
    string linia = "";
    int nrLinii=1;
    int nrOsoby=0;

    adresaci.clear();

    fstream plik;
    plik.open("Kontakty.txt", ios::in);

    if(plik.good()== true)
    {
        while(getline(plik, linia, '|'))
        {
            switch (nrLinii)
            {
            case 1:
                osoby.id = atoi(linia.c_str());
                break;
            case 2:
                osoby.idUzytkownika = atoi(linia.c_str());
                break;
            case 3:
                osoby.imie = linia;
                break;
            case 4:
                osoby.nazwisko = linia;
                break;
            case 5:
                osoby.telefon = linia;
                break;
            case 6:
                osoby.mail = linia;
                break;
            case 7:
                osoby.adres = linia;
                break;
            }

            if (nrLinii==7)
            {
                nrLinii=0;
                nrOsoby++;

                if(osoby.idUzytkownika == idZalogowanegoUzytkownika)
                {
                    adresaci.push_back(osoby);
                }
            }
            nrLinii++;
        }

        plik.close();
    }
}


void dodajOsobe(vector <Adresat> &adresaci, int idZalogowanegoUzytkownika)
{
    Adresat osoby;
    int idOsoby=0, idUzytkownika=0, liczbaWszystkichKontaktow=0;
    string imie, nazwisko, telefon, mail, adres;

    idUzytkownika = idZalogowanegoUzytkownika;
    liczbaWszystkichKontaktow = sprawdzLiczbeKontaktowWPliku();


    system("cls");
    cout << "Dodaj osobe do ksiazki adresowej" << endl;
    cout << "Podaj imie: ";
    cin >> imie;
    cout << "Podaj nazwisko: ";
    cin >> nazwisko;
    cout << "Podaj numer telefonu: ";
    cin.sync();
    getline(cin, telefon);
    cout << "Podaj adres email: ";
    cin >> mail;
    cout << "Podaj adres osoby: ";
    cin.sync();
    getline(cin, adres);


    adresaci.push_back(osoby);

  /*  osoby.id = idOsoby;
    osoby.idUzytkownika = idZalogowanegoUzytkownika;
    osoby.imie = imie;
    osoby.nazwisko = nazwisko;
    osoby.telefon = telefon;
    osoby.mail = mail;
    osoby.adres = adres;*/

    fstream plik;
    plik.open("Kontakty.txt", ios::out | ios::app);

    if (plik.good()== false)
        idOsoby = 1;
    else
        idOsoby = nadajKolejnyNumerIdKontaktu(liczbaWszystkichKontaktow)+1;


    if (plik.good()==true)
    {
        plik << zamienIntNaString(idOsoby) + "|" +
             zamienIntNaString(idUzytkownika) + "|" +
             imie + "|" + nazwisko + "|" + telefon + "|"+
             mail + "|" + adres + "|" << endl;

        plik.close();

        cout << "Osoba zostala dodana do ksiazki adresowej." << endl;
        Sleep(1500);
    }

    else
    {
        cout << "Nie udalo sie otworzyc pliku" << endl;
    }


}


void zapiszDaneKontaktowWPliku(vector <Adresat> &adresaci)
{
    fstream plik;
    plik.open("Kontakty.txt", ios::out);


    if(plik.good()== true)
    {
        for(int i =0; i< adresaci.size(); i++)
        {
            plik << adresaci[i].id << "|" << adresaci[i].idUzytkownika << "|" <<
                 adresaci[i].imie + "|" + adresaci[i].nazwisko << "|" + adresaci[i].telefon << "|"+
                 adresaci[i].mail + "|" + adresaci[i].adres + "|" << endl;
        }
        plik.close();
    }
}


void wyszukajOsobePoImieniu(vector <Adresat> adresaci)
{
    string szukaneImie;
    int liczbaOsobZSzukanymImieniem =0;

    system("cls");
    cout << "Wyszukaj osobe po imieniu"<< endl;
    cout << "Podaj imie: ";
    cin >> szukaneImie;


    for (int i=0; i<adresaci.size(); i++)
    {
        if (adresaci[i].imie == szukaneImie)
        {
            cout <<"Osoby o szukanym imieniu: ";
            cout << adresaci[i].id << ". " << adresaci[i].imie << " " << adresaci[i].nazwisko << endl;
            liczbaOsobZSzukanymImieniem++;
            Sleep(500);
        }
    }
    if (liczbaOsobZSzukanymImieniem == 0)
        cout << "Nie znaleziono osoby o takim imieniu" << endl;

    system("pause");
}


void wyszukajOsobePoNazwisku(vector <Adresat> adresaci)
{
    string szukaneNazwisko;
    int liczbaOsobZSzukanymNazwiskiem =0;

    system("cls");
    cout << "Wyszukaj osobe po nazwisku"<< endl;
    cout << "Podaj nazwisko: ";
    cin >> szukaneNazwisko;
    int i=0;


    while (i<adresaci.size())
    {
        if (adresaci[i].nazwisko == szukaneNazwisko)
        {
            cout <<"Osoby o szukanym nazwisku: ";
            cout << adresaci[i].id << ". " << adresaci[i].imie << " " <<adresaci[i].nazwisko << endl;
            liczbaOsobZSzukanymNazwiskiem++;
            Sleep(500);
        }
        i++;
    }
    if (liczbaOsobZSzukanymNazwiskiem == 0)
        cout << "Nie znaleziono osoby o takim nazwisku" << endl;

    system("pause");
}


void wyswietlWszystkieKontakty(vector <Adresat> adresaci)
{

    if (!adresaci.empty())
    {
        for (int i=0; i<adresaci.size(); i++)
        {
            cout << endl;
            cout << "ID: " << adresaci[i].id << endl;
            cout << adresaci[i].imie << endl;
            cout << adresaci[i].nazwisko << endl;
            cout << "Telefon: " << adresaci[i].telefon << endl;
            cout << "Email: " << adresaci[i].mail << endl;
            cout << "Adres: " << adresaci[i].adres << endl;
        }
        system("pause");
    }

}



void edytujDaneKontaktu(vector <Adresat> &adresaci, int idZalogowanegoUzytkownika)
{
    string noweImie, noweNazwisko, nowyNumerTelefonu, nowyEmail, nowyAdres;
    int idEdytowanejOsoby = 0;
    int wybranaOpcjaDoEdycji = 0;

    cout << "Podaj numer ID osoby, ktora chcesz edytowac: ";
    cin >> idEdytowanejOsoby;

    int i=0;

    while (i<adresaci.size())
    {
        if (adresaci[i].id == idEdytowanejOsoby)
        {
            system("cls");
            cout <<"Wybierz dane do edycji" << endl;
            cout<< "1 - imie" <<endl;
            cout<< "2 - nazwisko" <<endl;
            cout<< "3 - numer telefonu" <<endl;
            cout<< "4 - email" <<endl;
            cout<< "5 - adres" <<endl;
            cout<< "6 - powrot do menu" <<endl;

            cin>> wybranaOpcjaDoEdycji;

            switch (wybranaOpcjaDoEdycji)
            {
            case 1:
            {
                cout << "Podaj zmienione imie: ";
                cin.sync();
                getline (cin, noweImie);
                adresaci[i].imie = noweImie;
            }
            break;
            case 2:
            {
                cout << "Podaj zmienione nazwisko: ";
                cin.sync();
                getline(cin, noweNazwisko);
                adresaci[i].nazwisko = noweNazwisko;
            }
            break;
            case 3:
            {
                cout << "Podaj nowy numer telefonu: ";
                cin.sync();
                getline (cin, nowyNumerTelefonu);
                adresaci[i].telefon = nowyNumerTelefonu;
            }
            break;
            case 4:
            {
                cout << "Podaj nowy adres mailowy: ";
                cin.sync();
                getline (cin, nowyEmail);
                adresaci[i].mail = nowyEmail;
            }
            break;
            case 5:
            {
                cout << "Podaj nowy adres: ";
                cin.sync();
                getline(cin, nowyAdres);
                adresaci[i].adres = nowyAdres;
            }
            case 6:
                break;

                Sleep(500);
            }
        }
        i++;
    }


    string pobranaLinia = "";
    int liczbaWszystkichKontaktow = sprawdzLiczbeKontaktowWPliku();

    fstream plikDoOdczytu, plikDoZapisu;
    plikDoOdczytu.open("Kontakty.txt", ios::in);
    plikDoZapisu.open("Kontakty_tymczasowy.txt", ios::out);

    for (int i=0; i < liczbaWszystkichKontaktow; i++)
    {
           getline(plikDoOdczytu, pobranaLinia);

            char szukanyZnak = '|';
            int pozycjaZnaku = pobranaLinia.find(szukanyZnak);
            int idKontaktuWPobranejLinii = atoi((pobranaLinia.substr(0,pozycjaZnaku)).c_str());

        if (idKontaktuWPobranejLinii == idEdytowanejOsoby)
        {
            for(int pozycjaWektora=0; pozycjaWektora<adresaci.size(); pozycjaWektora++)
            {
                if (adresaci[pozycjaWektora].id == idEdytowanejOsoby)
                {
                    plikDoZapisu << adresaci[pozycjaWektora].id << '|' << adresaci[pozycjaWektora].idUzytkownika << '|'<<
                    adresaci[pozycjaWektora].imie << '|' << adresaci[pozycjaWektora].nazwisko << '|' <<
                    adresaci[pozycjaWektora].telefon << '|' << adresaci[pozycjaWektora].mail << '|' <<
                    adresaci[pozycjaWektora].adres << '|' << endl;
                }
            }
        }
        else
            plikDoZapisu << pobranaLinia << endl;
    }

    plikDoOdczytu.close();
    plikDoZapisu.close();

    remove("Kontakty.txt");
    rename("Kontakty_tymczasowy.txt", "Kontakty.txt" );

}



void usunKontakt(vector <Adresat> adresaci)
{
    int idUsuwanegoKontaktu=0;
    string pobranaLinia = "";
    char potwierdzenieUsunieciaKontaktu;

    cout << "Podaj ID adresata, ktorego chcesz usunac: ";
    cin >> idUsuwanegoKontaktu;

    cout << "Czy na pewno chcesz usunac tego Adresata? Jesli tak, wybierz t" << endl;
    cin >> potwierdzenieUsunieciaKontaktu;

    vector <string> tymczasowy;
    int i =0;


    fstream plikDoOdczytu, plikDoZapisu;
    plikDoOdczytu.open("Kontakty.txt", ios::in);


    if (potwierdzenieUsunieciaKontaktu == 't')
    {
        while (i<adresaci.size())
        {
            if (adresaci[i].id == idUsuwanegoKontaktu)
            {
                while (getline(plikDoOdczytu, pobranaLinia))
                    tymczasowy.push_back(pobranaLinia);

                plikDoOdczytu.close();

                plikDoZapisu.open("Kontakty.txt", ios::out);

                for (int j = 0; j < tymczasowy.size(); j++)
                {
                    if (j+1 != idUsuwanegoKontaktu)
                    {
                        plikDoZapisu << tymczasowy[j] << endl;
                    }
                }

                plikDoZapisu.close();
            }
            i++;
        }

        cout << "Adresat zostal usuniety" << endl;
        Sleep(800);
    }
    else
    {
        cout << "Adresat nie zostal usuniety" << endl;
        Sleep(800);
    }

}


int main()
{
    vector <Uzytkownik> uzytkownicy;
    vector <Adresat> adresaci;
    int idZalogowanegoUzytkownika = 0;
    int liczbaWszystkichKontaktow = 0;

    char wybor;

    wczytajUzytkownikowZPliku(uzytkownicy);


    while(true)
    {
        if (idZalogowanegoUzytkownika == 0)
        {
            system ("cls");
            cout << "1. Logowanie" << endl;
            cout << "2. Rejestracja" << endl;
            cout << "9. Zamknij program" << endl;
            cin >> wybor;

            if(wybor =='1')
            {
                idZalogowanegoUzytkownika = logowanie(uzytkownicy);
            }
            else if (wybor =='2')
            {
                rejestracja(uzytkownicy);
            }
            else if (wybor == '9')
            {
                exit(0);
            }
        }

        else
        {
            liczbaWszystkichKontaktow = sprawdzLiczbeKontaktowWPliku();
            wczytajKontaktyZPliku(adresaci, idZalogowanegoUzytkownika);

            system ("cls");
            cout << "1. Dodaj adresata" << endl;
            cout << "2. Wyszukaj po imieniu" << endl;
            cout << "3. Wyszukaj po nazwisku" << endl;
            cout << "4. Wyswietl wszystkich adresatow" << endl;
            cout << "5. Usun adresata" << endl;
            cout << "6. Edytuj adresata" << endl;
            cout << "7. Zmien haslo" << endl;
            cout << "9. Wyloguj sie " << endl;
            cout << "Twoj wybor: " << endl;
            cout <<endl;


            wybor = getchar();

            switch (wybor)
            {
            case '1':
                dodajOsobe(adresaci, idZalogowanegoUzytkownika);
                break;
            case '2':
                wyszukajOsobePoImieniu(adresaci);
                break;
            case '3':
                wyszukajOsobePoNazwisku(adresaci);
                break;
            case '4':
                wyswietlWszystkieKontakty(adresaci);
                break;
            case '5':
                usunKontakt(adresaci);
                break;
            case '6':
                edytujDaneKontaktu(adresaci, idZalogowanegoUzytkownika);

                break;
            case '7':
                zmianaHasla(uzytkownicy, idZalogowanegoUzytkownika);
                break;
            case '9':
                idZalogowanegoUzytkownika = 0;
            }
            cout << endl;
        }

    }

    return 0;
}
