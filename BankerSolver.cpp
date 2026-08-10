#include "BankerSolver.h"
#include <iostream>

using namespace std; 

void BankerSolver::ihtiyacHesapla() {
    ihtiyac.resize(gorevSayisi, vector<int>(kaynakTuruSayisi, 0));

    for (int i = 0; i < gorevSayisi; ++i) {
        for (int j = 0; j < kaynakTuruSayisi; ++j) {
            ihtiyac[i][j] = maxGereken[i][j] - gorevEldeMevcut[i][j];
        }
    }
}

BankerSolver::BankerSolver(int g, int k, 
                             const vector<int>& bAlan, 
                             const vector<vector<int>>& maxGerek, 
                             const vector<vector<int>>& eMevcut) 
    : gorevSayisi(g), kaynakTuruSayisi(k), bostaAlan(bAlan), maxGereken(maxGerek), gorevEldeMevcut(eMevcut) {
    
    ihtiyacHesapla();
}

bool BankerSolver::sistemGuvenliMi(vector<int>& guvenliDizi, string& cikti) {
    vector<int> kopyaBostaAlan = bostaAlan;
    vector<bool> bittiMi(gorevSayisi, false);
    
    guvenliDizi.clear();

   
    cikti = "\n--- KALAN IHTIYAC (NEED) MATRISI HESAPLANDI ---\n";
    for (int i = 0; i < gorevSayisi; ++i) {
        cikti += "  P" + to_string(i) + " Need: [ ";
        for (int j = 0; j < kaynakTuruSayisi; ++j) {
            cikti += to_string(ihtiyac[i][j]) + " ";
        }
        cikti += "]\n";
    }

    
    cikti += "\n--- GUVENLIK KONTROLU BASLADI ---\n";

    int bitenGorevSayac = 0;
    
    while (bitenGorevSayac < gorevSayisi) {
        bool gorevBulundu = false;

        for (int i = 0; i < gorevSayisi; ++i) {
            if (!bittiMi[i]) {
                bool tahsisEdilen = true;
                for (int j = 0; j < kaynakTuruSayisi; ++j) {
                    if (ihtiyac[i][j] > kopyaBostaAlan[j]) {
                        tahsisEdilen = false;
                        break;
                    }
                }

                if (tahsisEdilen) {
                    cikti += "P" + to_string(i) + " icin kaynaklar yeterli. Calistiriliyor...\n";
                    
                    for (int j = 0; j < kaynakTuruSayisi; ++j) {
                        kopyaBostaAlan[j] += gorevEldeMevcut[i][j];
                    }
                    
                    bittiMi[i] = true;
                    guvenliDizi.push_back(i);
                    gorevBulundu = true;
                    bitenGorevSayac++;
                    
                    cikti += "P" + to_string(i) + " bitti ve kaynaklarini iade etti.\n";
                    cikti += "   -> YENI BOSTA KALAN KAYNAK (Available): [ ";
                    for (size_t k = 0; k < kopyaBostaAlan.size(); ++k) {
                        cikti += to_string(kopyaBostaAlan[k]) + " ";
                    }
                    cikti += "]\n\n";
                }
            }
        }

        if (!gorevBulundu) {
            cikti += "HATA: Tikanma (Deadlock) riski var! Guvenli bir sira bulunamadi.\n";
            return false;
        }
    }

    cikti += "BASARI: Sistem guvende! Sureclerin kilitlenmeden calisma sirasi: ";
    for (size_t i = 0; i < guvenliDizi.size(); ++i) {
        cikti += "P" + to_string(guvenliDizi[i]) + " ";
    }
    cikti += "\n";
    
    return true;
}

bool BankerSolver::kaynakIste(int gorevId, const vector<int>& istek, string& cikti) {
    cikti = "\n=== P" + to_string(gorevId) + " ICIN KAYNAK ISTEGI GELDI ===\n";

    for (int j = 0; j < kaynakTuruSayisi; ++j) {
        if (istek[j] > ihtiyac[gorevId][j]) {
            cikti += "REDDEDILDI: P" + to_string(gorevId) + " maksimum ihtiyacindan fazlasini istedi!\n";
            return false;
        }
    }

    for (int j = 0; j < kaynakTuruSayisi; ++j) {
        if (istek[j] > bostaAlan[j]) {
            cikti += "BEKLETILIYOR: Kasada yeterli kaynak yok. P" + to_string(gorevId) + " beklemeli.\n";
            return false;
        }
    }

   
    for (int j = 0; j < kaynakTuruSayisi; ++j) {
        bostaAlan[j] -= istek[j];
        gorevEldeMevcut[gorevId][j] += istek[j];
        ihtiyac[gorevId][j] -= istek[j];
    }

    cikti += "Gecici tahsisat yapildi. Yeni durum icin guvenlik testi baslatiliyor...\n";

    vector<int> dummySira;
    string guvenlikLog;
    bool guvenliMi = sistemGuvenliMi(dummySira, guvenlikLog);

    cikti += guvenlikLog;

    if (guvenliMi) {
        cikti += "\nONAYLANDI: Istek guvenli. Kaynaklar kalici olarak P" + to_string(gorevId) + " surecine tahsis edildi.\n";
        return true;
    } else {
        cikti += "\nREDDEDILDI: Bu istek sistemi kilitliyor! Degisiklikler geri aliniyor (Rollback).\n";
        
        for (int j = 0; j < kaynakTuruSayisi; ++j) {
            bostaAlan[j] += istek[j];
            gorevEldeMevcut[gorevId][j] -= istek[j];
            ihtiyac[gorevId][j] += istek[j];
        }
        return false;
    }
}