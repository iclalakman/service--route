#include "deneme.hpp"

#include <locale> //precisionı 6 basamak yapmak için
int main(){

    std::locale::global(std::locale("C"));
    servis servis;

    servis.readCSV("C:\\Users\\sugra\\OneDrive\\Belgeler\\Servis\\veri.csv");

    servis.writeCSV("C:\\Users\\sugra\\OneDrive\\Belgeler\\Servis\\cikanDosya.csv");

    servis.rotaYazdir("C:\\Users\\sugra\\OneDrive\\Belgeler\\Servis\\rota.csv");


    return 0;
}