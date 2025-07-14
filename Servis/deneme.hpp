#pragma once

#include <Windows.h> // microsaniye olayi 

#include <sstream>
#include <string>

#include <fstream>
#include <iomanip>

#include <vector>
#include <iostream>

double uzaklik_hesapla(double x1,double y1,double merkezx,double merkezy);

class servis {
public:

    void readCSV(std::string dosya_yer);

    void writeCSV(std::string dosyaYaratmaYeri);

    void rotaYazdir(std::string cikis_dosyasi);

    std::vector<double> latitude;
    std::vector<double> longitude;
};