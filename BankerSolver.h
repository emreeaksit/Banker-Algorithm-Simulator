#ifndef BANKERSOLVER_H
#define BANKERSOLVER_H

#include <vector>
#include <string>
using namespace std;
class BankerSolver {
private:
    int gorevSayisi;
    int kaynakTuruSayisi;
    
    vector<int> bostaAlan;
    vector<std::vector<int>> maxGereken;
    vector<std::vector<int>> gorevEldeMevcut;
    vector<std::vector<int>> ihtiyac;

    void ihtiyacHesapla();

public:
    BankerSolver(int g, int k, 
                 const vector<int>& bAlan, 
                 const vector<std::vector<int>>& maxGerek, 
                 const vector<std::vector<int>>& eMevcut);

    bool sistemGuvenliMi(vector<int>& guvenliDizi, string& cikti);
    bool kaynakIste(int gorevId, const vector<int>& istek, string& cikti);
};

#endif 