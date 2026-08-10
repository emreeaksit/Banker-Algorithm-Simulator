#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "BankerSolver.h"

using namespace std;


void ApplyCyberTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 12.0f;
    style.FrameRounding = 6.0f;
    style.GrabRounding = 6.0f;
    style.PopupRounding = 8.0f;
    style.ScrollbarRounding = 8.0f;
    style.FramePadding = ImVec2(8, 5);
    style.ItemSpacing = ImVec2(10, 10);
    style.WindowBorderSize = 1.0f;

    ImVec4* colors = style.Colors;
    
    
    colors[ImGuiCol_WindowBg]           = ImVec4(0.08f, 0.04f, 0.12f, 0.88f);
    colors[ImGuiCol_Border]             = ImVec4(0.90f, 0.10f, 0.60f, 0.50f);
    colors[ImGuiCol_Header]             = ImVec4(0.35f, 0.10f, 0.45f, 0.70f);
    colors[ImGuiCol_HeaderHovered]      = ImVec4(0.80f, 0.15f, 0.60f, 0.80f);
    colors[ImGuiCol_HeaderActive]       = ImVec4(0.95f, 0.20f, 0.70f, 1.00f);
    

    colors[ImGuiCol_Button]             = ImVec4(0.60f, 0.10f, 0.55f, 0.85f);
    colors[ImGuiCol_ButtonHovered]      = ImVec4(0.90f, 0.15f, 0.65f, 1.00f);
    colors[ImGuiCol_ButtonActive]       = ImVec4(0.40f, 0.05f, 0.40f, 1.00f);
    
    
    colors[ImGuiCol_FrameBg]            = ImVec4(0.16f, 0.08f, 0.24f, 0.90f);
    colors[ImGuiCol_FrameBgHovered]     = ImVec4(0.28f, 0.12f, 0.40f, 1.00f);
    colors[ImGuiCol_FrameBgActive]      = ImVec4(0.40f, 0.15f, 0.55f, 1.00f);
    
    
    colors[ImGuiCol_TitleBg]            = ImVec4(0.12f, 0.04f, 0.18f, 1.00f);
    colors[ImGuiCol_TitleBgActive]      = ImVec4(0.25f, 0.06f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight]   = ImVec4(0.50f, 0.15f, 0.50f, 0.40f);
    colors[ImGuiCol_TableBorderStrong]  = ImVec4(0.80f, 0.20f, 0.70f, 0.70f);
}


void DrawCyberBackground(float width, float height) {
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    
    drawList->AddRectFilledMultiColor(
        ImVec2(0, 0), ImVec2(width, height),
        IM_COL32(15, 5, 25, 255),
        IM_COL32(35, 10, 50, 255),
        IM_COL32(60, 10, 50, 255),
        IM_COL32(20, 5, 30, 255)
    );

    int gridColor = IM_COL32(230, 30, 150, 25);
    float gridSize = 40.0f;

    for (float x = 0; x < width; x += gridSize) {
        drawList->AddLine(ImVec2(x, 0), ImVec2(x, height), gridColor, 1.0f);
    }
    for (float y = 0; y < height; y += gridSize) {
        drawList->AddLine(ImVec2(0, y), ImVec2(width, y), gridColor, 1.0f);
    }
}


void GenerateRandomScenario(int bosta[3], int maxGereken[5][3], int eldeMevcut[5][3]) {
    for (int j = 0; j < 3; j++) {
        bosta[j] = rand() % 4 + 1; 
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            eldeMevcut[i][j] = rand() % 3; 
            maxGereken[i][j] = eldeMevcut[i][j] + (rand() % 4); 
        }
    }
}

int main() {
    srand((unsigned int)time(NULL)); 

    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(1200, 980, "Banker Algoritmasi Simulatörü", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    ApplyCyberTheme();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    const int gSayisi = 5;
    const int kSayisi = 3;

    int bosta[3];
    int maxGereken[5][3];
    int eldeMevcut[5][3];

    
    GenerateRandomScenario(bosta, maxGereken, eldeMevcut);

    string raporOutput = "Sistem analizini baslatmak icin 'Kilitlenme(Deadlock) Analizini Baslat' butonuna tiklayin.";
    bool sonucGuvenliMi = false;
    bool analizYapildiMi = false;
    vector<int> guvenliSiraGlobal;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        DrawCyberBackground((float)display_w, (float)display_h);

        ImGui::SetNextWindowPos(ImVec2(15, 15), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(display_w - 30.0f, display_h - 30.0f), ImGuiCond_Always);
        ImGui::Begin("GUVENLI BANKACI MOTORU v4.0", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        
        ImGui::TextColored(ImVec4(1.00f, 0.35f, 0.85f, 1.00f), "Sistem Kaynak Tahsisi ve Kilitlenme (Deadlock) Kontrolü");
        
        ImGui::SameLine(ImGui::GetWindowWidth() - 320);
        
        
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.45f, 0.75f, 0.90f));
        if (ImGui::Button("Rastgele Senaryo", ImVec2(140, 25))) {
            GenerateRandomScenario(bosta, maxGereken, eldeMevcut);
            analizYapildiMi = false;
            raporOutput = "Yeni rastgele senaryo yuklendi. Analizi baslatabilirsiniz.";
        }
        ImGui::PopStyleColor();

        ImGui::SameLine();

        
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.70f, 0.20f, 0.20f, 0.90f));
        if (ImGui::Button("Sifirla", ImVec2(100, 25))) {
            for (int j = 0; j < kSayisi; j++) bosta[j] = 0;
            for (int i = 0; i < gSayisi; i++) {
                for (int j = 0; j < kSayisi; j++) {
                    maxGereken[i][j] = 0;
                    eldeMevcut[i][j] = 0;
                }
            }
            analizYapildiMi = false;
            guvenliSiraGlobal.clear();
            raporOutput = "Degerler sifirlandi. Yeni matris degerlerini girip analizi yeniden baslatabilirsiniz.";
        }
        ImGui::PopStyleColor();

        ImGui::Separator();
        ImGui::Spacing();

       
        ImGui::TextColored(ImVec4(0.40f, 0.90f, 1.00f, 1.00f), "1. Available Resources (Bosta Kalan Kaynaklar)");
        ImGui::PushItemWidth(100);
        ImGui::InputInt("Res A", &bosta[0], 0); ImGui::SameLine();
        ImGui::InputInt("Res B", &bosta[1], 0); ImGui::SameLine();
        ImGui::InputInt("Res C", &bosta[2], 0);
        ImGui::PopItemWidth();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

       
        ImGui::Columns(2, "CyberMatrixColumns", false);

        
        ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.80f, 1.00f), "Max Matrix (Maksimum Ihtiyac)");
        if (ImGui::BeginTable("TableMax", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("Process", ImGuiTableColumnFlags_WidthFixed, 60.0f);
            ImGui::TableSetupColumn("A", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("B", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("C", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();

            for (int i = 0; i < gSayisi; i++) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::TextColored(ImVec4(0.80f, 0.50f, 1.00f, 1.00f), "P%d", i);
                for (int j = 0; j < kSayisi; j++) {
                    ImGui::TableSetColumnIndex(j + 1);
                    ImGui::PushID(i * 10 + j);
                    ImGui::PushItemWidth(-FLT_MIN);
                    ImGui::InputInt("##max", &maxGereken[i][j], 0);
                    ImGui::PopItemWidth();
                    ImGui::PopID();
                }
            }
            ImGui::EndTable();
        }

        ImGui::NextColumn();

       
        ImGui::TextColored(ImVec4(0.30f, 0.95f, 0.90f, 1.00f), "Allocation Matrix (Elde Mevcut)");
        if (ImGui::BeginTable("TableAlloc", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("Process", ImGuiTableColumnFlags_WidthFixed, 60.0f);
            ImGui::TableSetupColumn("A", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("B", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("C", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();

            for (int i = 0; i < gSayisi; i++) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::TextColored(ImVec4(0.80f, 0.50f, 1.00f, 1.00f), "P%d", i);
                for (int j = 0; j < kSayisi; j++) {
                    ImGui::TableSetColumnIndex(j + 1);
                    ImGui::PushID(100 + i * 10 + j);
                    ImGui::PushItemWidth(-FLT_MIN);
                    ImGui::InputInt("##alloc", &eldeMevcut[i][j], 0);
                    ImGui::PopItemWidth();
                    ImGui::PopID();
                }
            }
            ImGui::EndTable();
        }

        ImGui::Columns(1);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.75f, 0.10f, 0.65f, 0.90f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.95f, 0.20f, 0.80f, 1.00f));
        if (ImGui::Button("Kilitlenme(Deadlock) Analizini Baslat", ImVec2(-FLT_MIN, 40))) {
            vector<int> vBosta(bosta, bosta + kSayisi);
            vector<vector<int>> vMax(gSayisi, vector<int>(kSayisi));
            vector<vector<int>> vAlloc(gSayisi, vector<int>(kSayisi));

            for (int i = 0; i < gSayisi; i++) {
                for (int j = 0; j < kSayisi; j++) {
                    vMax[i][j] = maxGereken[i][j];
                    vAlloc[i][j] = eldeMevcut[i][j];
                }
            }

            BankerSolver solver(gSayisi, kSayisi, vBosta, vMax, vAlloc);
            guvenliSiraGlobal.clear();
            sonucGuvenliMi = solver.sistemGuvenliMi(guvenliSiraGlobal, raporOutput);
            
            if (!sonucGuvenliMi) {
                raporOutput += "\n\n--- DETAYLI DEADLOCK ANALIZI ---\n";
                raporOutput += "Bosta Kalan Kaynaklar (Available): [ " + to_string(bosta[0]) + " " + to_string(bosta[1]) + " " + to_string(bosta[2]) + " ]\n";
                raporOutput += "Sistemdeki sureclerin Need (Kalan Ihtiyac) degerleri eldeki kaynaklar ile karsilanamadigi icin tikanma olusuyor.";
            }

            analizYapildiMi = true;
        }
        ImGui::PopStyleColor(2);

        ImGui::Spacing();

        
        if (analizYapildiMi) {
            if (sonucGuvenliMi) {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.08f, 0.22f, 0.15f, 0.90f));
                ImGui::BeginChild("StatusBanner", ImVec2(-FLT_MIN, 45), true);
                ImGui::TextColored(ImVec4(0.20f, 1.00f, 0.50f, 1.00f), "GÜVENLI DURUM KILITLENME (DEADLOCK) TESPIT EDILMEDI!");
                ImGui::SameLine(420);
                ImGui::Text("Calistirma Sirasi / Guvenli Dizi: "); ImGui::SameLine();
                for (size_t k = 0; k < guvenliSiraGlobal.size(); k++) {
                    ImGui::TextColored(ImVec4(1.00f, 0.35f, 0.85f, 1.00f), "[ P%d ]", guvenliSiraGlobal[k]);
                    if (k < guvenliSiraGlobal.size() - 1) {
                        ImGui::SameLine();
                        ImGui::TextColored(ImVec4(0.40f, 0.90f, 1.00f, 1.00f), "=>");
                        ImGui::SameLine();
                    }
                }
                ImGui::EndChild();
                ImGui::PopStyleColor();
            } else {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.35f, 0.05f, 0.12f, 0.90f));
                ImGui::BeginChild("StatusBanner", ImVec2(-FLT_MIN, 45), true);
                ImGui::TextColored(ImVec4(1.00f, 0.20f, 0.30f, 1.00f), "GÜVENLI DURUM DEGIL TALEP REDDEDILDI");
                ImGui::EndChild();
                ImGui::PopStyleColor();
            }
        }

        ImGui::Spacing();

       
        ImGui::TextColored(ImVec4(0.70f, 0.70f, 0.85f, 1.00f), "Sistem Analiz Raporu:");
        
        float remainingHeight = ImGui::GetContentRegionAvail().y - 10.0f;
        if (remainingHeight < 150.0f) remainingHeight = 150.0f;

        ImGui::InputTextMultiline(
            "##log", 
            (char*)raporOutput.c_str(), 
            raporOutput.size() + 1, 
            ImVec2(-FLT_MIN, remainingHeight), 
            ImGuiInputTextFlags_ReadOnly
        );

        ImGui::End();

        ImGui::Render();
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.05f, 0.02f, 0.08f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}