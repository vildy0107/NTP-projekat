#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

// Struktura Dionica
struct Dionica {
    string puniNaziv;
    string skracenica;
    double vrijednost;
};


// Struktura Korisnik
struct Korisnik {
    string korisnickoIme;
    string lozinka;
    double kapital;
    vector<string> dionice;
};


// Funkcija za unos novog korisnika
Korisnik unosKorisnika() {
    Korisnik korisnik;
    cout << "Unesite korisnicko ime: ";
    cin >> korisnik.korisnickoIme;
    cout << "Unesite lozinku: ";
    cin >> korisnik.lozinka;
    cout << "Unesite pocetni kapital: ";
    cin >> korisnik.kapital;
    return korisnik;
}
Dionica unosDionice() {
    Dionica novaDionica;
    cout << "Unesite puni naziv dionice: ";
    cin.ignore();
    getline(cin, novaDionica.puniNaziv);

    cout << "Unesite skracenicu dionice: ";
    cin >> novaDionica.skracenica;

    cout << "Unesite vrijednost dionice: ";
    cin >> novaDionica.vrijednost;

    return novaDionica;
}



// Funkcija za ispis dionica
void Pregled(const Korisnik& korisnik) {
    cout << "Vas kapital je:  " << korisnik.kapital;
    cout << "   Dionice korisnika " << korisnik.korisnickoIme << ":" << endl;
    if (korisnik.dionice.empty()) {
        cout << "Nemate dionica." << endl;
    } else {
        for (const auto& dionica : korisnik.dionice) {
            cout << dionica << endl;
        }
    }
}

// Funkcija za dodavanje kapitala korisniku
void dodajKapital(Korisnik& korisnik) {
    double dodatniKapital;
    cout << "Unesite iznos dodatnog kapitala: ";
    cin >> dodatniKapital;
    korisnik.kapital += dodatniKapital;
    cout << "Kapital je uspjesno dodan. Novi iznos kapitala: " << korisnik.kapital << endl;
}

// Funkcija za provjeru korisnika
bool provjeriKorisnika(const Korisnik& korisnik, const string& korisnickoIme, const string& lozinka) {
    return (korisnik.korisnickoIme == korisnickoIme && korisnik.lozinka == lozinka);
}


bool prijavaKorisnika(Korisnik& korisnik) {
    string korisnickoIme;
    string lozinka;

    cout << "Unesite korisnicko ime: ";
    cin >> korisnickoIme;

    cout << "Unesite lozinku: ";
    cin >> lozinka;

    ifstream datoteka("korisnici.txt");
    if (datoteka.is_open()) {
        string linija;
        while (getline(datoteka, linija)) {
            if (linija == korisnickoIme) {
                getline(datoteka, linija);
                if (linija == lozinka) {
                    korisnik.korisnickoIme = korisnickoIme;
                    korisnik.lozinka = lozinka;
                    getline(datoteka, linija);
                    korisnik.kapital = stod(linija);
                    datoteka.close();
                    cout << "Uspjesna prijava. Dobrodosli, " << korisnik.korisnickoIme << "!" << endl;
                    return true;
                } else {
                    cout << "Pogresna lozinka. Prijavite se ponovno." << endl;
                    datoteka.close();
                    return false;
                }
            }
        }
        cout << "Korisnicko ime nije pronadjeno. Prijavite se ponovno." << endl;
        datoteka.close();
        return false;
    } else {
        cout << "Greska pri otvaranju datoteke." << endl;
        return false;
    }
}void kupiDionicu(Korisnik& korisnik, const Dionica& dionica) {
    if (dionica.vrijednost <= korisnik.kapital) {
       // korisnik.dionice.push_back(dionica);
        korisnik.kapital -= dionica.vrijednost;
        cout << "Kupljena je dionica " << dionica.puniNaziv << "." << endl;

        // Ažuriraj vrijednost dionice u datoteci
        ifstream datoteka("dionice.txt");
        ofstream temp("temp.txt");
        if (datoteka.is_open() && temp.is_open()) {
            string linija;
            while (getline(datoteka, linija)) {
                if (linija.find("Puni naziv: " + dionica.puniNaziv) != string::npos) {
                    temp << linija << endl;  // Puni naziv
                    getline(datoteka, linija); // Skracenica
                    temp << linija << endl;
                    getline(datoteka, linija); // Vrijednost
                    double novaVrijednost = stod(linija.substr(12)) - dionica.vrijednost;
                    temp << "Vrijednost: " << novaVrijednost << endl;
                    getline(datoteka, linija); // Prazna linija
                    temp << endl;
                } else {
                    temp << linija << endl;
                }
            }
            datoteka.close();
            temp.close();

            // Zamijeni originalnu datoteku s temp datotekom
            remove("dionice.txt");
            rename("temp.txt", "dionice.txt");
        } else {
            cout << "Greska pri otvaranju datoteka." << endl;
        }
    } else {
        cout << "Nemate dovoljno sredstava za kupnju dionice." << endl;
    }
}


int main() {
    int opcija;
    Korisnik korisnik;

    do {
        cout << "----- Meni -----" << endl;
        cout << "1. Registracija" << endl;
        cout << "2. Prijava" << endl;
        cout << "3. Pregled" << endl;
        cout << "4. Kupi dionicu" << endl;
        cout << "5. Dodaj kapital" << endl;
        cout << "0. Izlaz" << endl;
        cout << "----------------" << endl;
        cout << "Unesite broj opcije: ";
        cin >> opcija;

        switch (opcija) {
            case 1: {
                Korisnik noviKorisnik = unosKorisnika();

                ofstream datoteka("korisnici.txt", ios::app);
                if (datoteka.is_open()) {
                    datoteka << noviKorisnik.korisnickoIme << endl;
                    datoteka << noviKorisnik.lozinka << endl;
                    datoteka << noviKorisnik.kapital << endl;
                    datoteka.close();
                    cout << "Uspjesno ste se registrovali. Mozete se prijaviti sa unesenim podacima." << endl;
                } else {
                    cout << "Greska pri otvaranju datoteke." << endl;
                }
                break;
            }
            case 2: {
                if (prijavaKorisnika(korisnik)) {
                    // Uspjesna prijava, nastavite sa radom korisnika
                } else {
                    // Neuspjesna prijava, vracanje na meni
                }
                break;
            }
            case 3: {
                if (korisnik.korisnickoIme.empty()) {
                    cout << "Niste prijavljeni. Prijavite se prije dodavanja kapitala." << endl;
                } else {
                Pregled(korisnik);}
                break;
            }
            case 4: {
                if (korisnik.korisnickoIme.empty()) {
                    cout << "Niste prijavljeni. Prijavite se prije kupovine dionica." << endl;
                    break;
                } else
                if (korisnik.kapital <= 0) {
                    cout << "Nemate vise sredstava za kupnju dionica." << endl;
                    break;
                }

                ifstream datoteka("dionice.txt");
                if (datoteka.is_open()) {
                    string linija;
                    vector<Dionica> dostupneDionice;
                    while (getline(datoteka, linija)) {
                        if (linija.find("Puni naziv: ") != string::npos) {
                            Dionica dionica;
                            dionica.puniNaziv = linija.substr(12);
                            getline(datoteka, linija); // Preskoci liniju "Skracenica: "
                            getline(datoteka, linija); // Vrijednost: "
                            dionica.vrijednost = stod(linija.substr(12));
                            dostupneDionice.push_back(dionica);
                        }
                    }
                    datoteka.close();

                    if (dostupneDionice.empty()) {
                        cout << "Nema dostupnih dionica." << endl;
                        break;
                    }

                    cout << "Dostupne dionice:" << endl;
                    for (int i = 0; i < dostupneDionice.size(); i++) {
                        cout << i + 1 << ". " << left << dostupneDionice[i].skracenica<< setw(32)<< dostupneDionice[i].puniNaziv << setw(20)  << dostupneDionice[i].vrijednost  << endl;
                    }
//                    cout << "Dostupne dionice:" << endl;
//                    cout << "-------------------------------" << endl;
//                    cout << left << setw(12) << "Skracenica" << setw(40) << "Puni naziv" << setw(42) << "Vrijednost" << endl;
//                    cout << "-------------------------------" << endl;
//
//                    for (const Dionica& dionica : dostupneDionice) {
//                        cout << left << setw(12) << dionica.skracenica << setw(40) << dionica.puniNaziv << setw(42) << dionica.vrijednost << endl;
//                    }
//
                    cout << "-------------------------------" << endl;

                    cout << "Odaberite broj dionice koju želite kupiti: ";
                    int odabir;
                    cin >> odabir;

                    if (odabir >= 1 && odabir <= dostupneDionice.size()) {
                        kupiDionicu(korisnik, dostupneDionice[odabir - 1]);
                    } else {
                        cout << "Neispravan odabir dionice." << endl;
                    }
                } else {
                    cout << "Greska pri otvaranju datoteke." << endl;
                }

                break;
            }

            case 5: {if (korisnik.korisnickoIme.empty()) {
                    cout << "Niste prijavljeni. Prijavite se prije dodavanja kapitala." << endl;
                } else {
                dodajKapital(korisnik);
                break;
            }
            }
            case 0: {
                cout << "Hvala na koristenju. Dovidjenja!" << endl;
                break;
            }
            default: {
                cout << "Nepostojeca opcija. Pokusajte ponovo." << endl;
                break;
            }
        }

        cout << endl;

    } while (opcija != 0);

    return 0;
}