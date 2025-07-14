#include "deneme.hpp"

#include <cstring>
#include <math.h>
#include <filesystem>
#include <stdexcept>
#include <algorithm> //sorting için


double uzaklik_hesapla(double x1, double y1, double merkezx, double merkezy) {
    return sqrt(pow(x1 - merkezx,2) + pow(y1 - merkezy,2));
}

void servis::readCSV(std::string dosya_yer){

    std::ifstream dosya(dosya_yer);

    if (!dosya.is_open()) {
        std::cerr << "Dosya acilamadi: " << dosya_yer << std::endl;
        std::cerr << "Hata: " << strerror(errno) << std::endl;
        return;
    }

    std::string satir;
    while (std::getline(dosya, satir)) {
        // UTF-8 BOM temizliği
        if (!satir.empty() && static_cast<unsigned char>(satir[0]) == 0xEF) {
            satir.erase(0, 3);
        }

        // Virgüle göre ayır
        size_t virgülPozisyonu = satir.find(',');
        if (virgülPozisyonu != std::string::npos) {
            std::string lat_str = satir.substr(0,virgülPozisyonu);
            std::string lon_str = satir.substr(virgülPozisyonu + 1);

            try {
                double lat = std::stod(lat_str);
                double lon = std::stod(lon_str);

                latitude.push_back(lat);
                longitude.push_back(lon);

            } catch (const std::exception& e) {
                std::cerr << "Satır işlenemedi: " << satir << "\nHata: " << e.what() << std::endl;
            }
        }
    }

    dosya.close();
}

void servis::writeCSV(std::string dosyaYaratmaYeri) {
    std::filesystem::path dosyaYolu(dosyaYaratmaYeri);

    std::ofstream dosya(dosyaYaratmaYeri);
    if (!dosya.is_open()) {
        std::cerr << "Dosya açılamadı: " << dosyaYaratmaYeri << std::endl;
        return;
    }

    if (latitude.empty() || longitude.empty()) {
        std::cerr << "Koordinat verileri yok.\n";
        return;
    }

    // Merkez koordinatları (ilk satır)
    double merkezX = latitude[0];
    double merkezY = longitude[0];

    std::vector<double> uzaklik_sonuclar;

    for (size_t i = 1; i < latitude.size(); ++i) {
        double x = latitude[i];
        double y = longitude[i];
        double uzaklik_sonuc = uzaklik_hesapla(x, y, merkezX, merkezY);
        uzaklik_sonuclar.push_back(uzaklik_sonuc);
    }
    // sorting ->kucukten buyuge
    std::sort(uzaklik_sonuclar.begin(), uzaklik_sonuclar.end());

    for (double deger:uzaklik_sonuclar) {
        dosya << std::fixed << std::setprecision(6) << deger << "\n";
    }
    dosya.close();
    std::cout << "\nVeri yazildi: " << dosyaYaratmaYeri << std::endl;
}

void rotaOlusturRecursive(double merkezX,double merkezY,std::vector<std::pair<double, double>>& kalanNoktalar,std::vector<std::pair<double, double>>& rota){
    if (kalanNoktalar.empty())
        return;

    // En yakın noktayı bul
    double minUzaklik = std::numeric_limits<double>::max();
    int enYakinIndex = -1;

    for (size_t i = 0; i < kalanNoktalar.size(); ++i) {
        double uzak = uzaklik_hesapla(merkezX, merkezY, kalanNoktalar[i].first, kalanNoktalar[i].second);
        if (uzak < minUzaklik) {
            minUzaklik = uzak;
            enYakinIndex = i;
        }
    }

    // Seçilen noktayı rotaya ekle
    auto enYakinNokta = kalanNoktalar[enYakinIndex];
    rota.push_back(enYakinNokta);

    // Kalan noktalar listesinden çıkar
    kalanNoktalar.erase(kalanNoktalar.begin() + enYakinIndex);

    // Recursive çağrı
    rotaOlusturRecursive(enYakinNokta.first, enYakinNokta.second, kalanNoktalar, rota);
}

void servis::rotaYazdir(std::string cikis_dosyasi) {
    if (latitude.empty() || longitude.empty()) return;

    std::vector<std::tuple<double, double, int>> kalanNoktalar;

    // İlk satır merkez, geri kalanları listele
    for (size_t i = 1; i < latitude.size(); ++i) {
        kalanNoktalar.emplace_back(latitude[i],longitude[i], i);
    }

    std::vector<int> rotaIndex; // Sadece index tutacağız
    rotaIndex.push_back(0); // Merkez koordinatın index'i her zaman 0

    // Recursive rota -> merkezler
    double merkezX = latitude[0];
    double merkezY = longitude[0];

    while (!kalanNoktalar.empty()) {
        double minUzaklik = std::numeric_limits<double>::max();
        int enYakinIndex = -1;

        for (size_t i = 0; i < kalanNoktalar.size(); ++i) {
            double lat = std::get<0>(kalanNoktalar[i]);
            double lon = std::get<1>(kalanNoktalar[i]);
            double uzak = uzaklik_hesapla(merkezX, merkezY, lat, lon);

            if (uzak < minUzaklik) {
                minUzaklik = uzak;
                enYakinIndex = i;
            }
        }

        // Yeni merkez: en yakın nokta
        auto secilen = kalanNoktalar[enYakinIndex];
        merkezX = std::get<0>(secilen);
        merkezY = std::get<1>(secilen);
        rotaIndex.push_back(std::get<2>(secilen)); // orijinal index

        // Kaldır
        kalanNoktalar.erase(kalanNoktalar.begin() + enYakinIndex);
    }

    // Yazdır
    std::ofstream dosya(cikis_dosyasi);
    if (!dosya.is_open()) {
        std::cerr << "Cikis dosyasi acilamadi: " << cikis_dosyasi << std::endl;
        return;
    }

    // alt alta
    /*for (int index : rotaIndex) {
        dosya << index << "\n";
    }*/
    // yan yana  0 -> 4 -> 2 -> ...
    for (size_t i = 0; i < rotaIndex.size(); ++i) {
        dosya << rotaIndex[i];
        if (i != rotaIndex.size() - 1) {
            dosya << " -> ";
        }
    }
    dosya << std::endl;

    dosya.close();
}





