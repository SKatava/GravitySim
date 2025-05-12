#include "GUI.h"

#include <string>
#include <iostream>

#include <implot.h>

#include "IconCodes.h"
#include "Colors.h"
#include "Hardware.h"
#include "Timer.h"
#include "Settings.h"
#include "PhysicsEngine.h"


const char* GUI::EntityTypes[] = {"Object", "Particle"};

ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoSavedSettings | 
                                ImGuiWindowFlags_NoTitleBar | 
                                ImGuiWindowFlags_NoResize | 
                                ImGuiWindowFlags_NoMove |
                                ImGuiWindowFlags_NoScrollbar | 
                                ImGuiWindowFlags_NoScrollWithMouse |
                                ImGuiWindowFlags_NoBringToFrontOnFocus;

GUI::GUI() : CALL_ADDENTITY_FORM(false), CALL_ENTITY_SEARCH(false){
    m_NewColor[0] = 0.f;
    m_NewColor[1] = 0.f;
    m_NewColor[2] = 1.f;
    m_EntityTypeSelected = 0;
    m_NumToAdd = 1;
    m_NewSize = 10;
    m_TableRowSelected = 0;
    int m_EntitySelected = 0;
    m_MaxForce = 0; 
    float m_NewDeltaTime = 0;
    ResetPositionPlotData();
    ResetForcePlotData();
}

GUI::~GUI(){
    if(m_window){
        glfwDestroyWindow(m_window);
    }
    ImGui_ImplOpenGL3_Shutdown(); 
	ImGui_ImplGlfw_Shutdown(); 
    ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

void GUI::InitGUI(GLFWwindow* window){
    IMGUI_CHECKVERSION(); 
    ImGui::CreateContext(); 
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); 
    (void)io; 
    io.Fonts->Clear();
    ImFont* myFont = io.Fonts->AddFontFromFileTTF("assets/fonts/inter.ttf", 18.0f);
    io.FontDefault = myFont;

    static const ImWchar icons_ranges[] = { 0xf000, 0xf8ff, 0 };
    ImFontConfig config;
    config.MergeMode = true;  
    config.PixelSnapH = true; 
    ImFont* iconFont = io.Fonts->AddFontFromFileTTF("assets/fonts/icon_font.otf", 16.0f, &config, icons_ranges);
    io.Fonts->Build();
    
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = WINDOW_BG;
    style.Colors[ImGuiCol_Button] = BUTTON_BG;
    style.Colors[ImGuiCol_FrameBg] = INPUT_BG; 
    style.Colors[ImGuiCol_ChildBg] = CHILD_WINDOW_BG;
    style.Colors[ImGuiCol_TableHeaderBg] = TABLE_HEADER_ROW_BG;
    style.FrameBorderSize = 1.0f;  
    
    ImPlotStyle& plotStyle = ImPlot::GetStyle(); 
    plotStyle.Colors[ImPlotCol_FrameBg] = PLOT_BG;
    plotStyle.Colors[ImPlotCol_AxisBgHovered] = PLOT_BG;
    plotStyle.Colors[ImPlotCol_AxisBgActive] = PLOT_BG;
    
    ImGui_ImplGlfw_InitForOpenGL(window, true); 
    ImGui_ImplOpenGL3_Init("#version 130");

    m_window = window;
}

void GUI::CreateContext(){
    TitleBar();
    EntitiesList();
    AddEntityBtn();
    DeleteEntityBtn();
    SearchEntityBtn();
    TimeInfo();
    TimeStateButton();
    ResetButton();
    TimeModeButton();
    AccelerationOptions();
    TimePlot();
    UsageAnalytics();
    CPUInfo();
    EntityAnalytics();
    AddEntityForm();
    EntitySearch();
    WindowBorder();
}

void GUI::Render(){
    ImGui_ImplOpenGL3_NewFrame(); 
	ImGui_ImplGlfw_NewFrame(); 
	ImGui::NewFrame();

    CreateContext();

    ImGui::Render(); 
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::TitleBar(){
    ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
    ImGui::SetNextWindowSize(ImVec2(1720.f, 20.f));                                 
    ImGui::Begin("##TITLEBAR", NULL, window_flags);

    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0)) {
        dragging = true;

        double mouseX, mouseY;
        glfwGetCursorPos(m_window, &mouseX, &mouseY);

        int winX, winY;
        glfwGetWindowPos(m_window, &winX, &winY);

        dragOffsetX = mouseX;
        dragOffsetY = mouseY;
    }

    if (dragging) {
        if (ImGui::IsMouseDown(0)) {
            double mouseX, mouseY;
            glfwGetCursorPos(m_window, &mouseX, &mouseY);

            int winX, winY;
            glfwGetWindowPos(m_window, &winX, &winY);

            int newX = winX + (int)(mouseX - dragOffsetX);
            int newY = winY + (int)(mouseY - dragOffsetY);

            glfwSetWindowPos(m_window, newX, newY);
        } else {
            dragging = false;
        }
    }

    ImGui::Text(ROCKET_ICON " Gravity Sim");
    ImGui::SameLine();
    ImGui::SetCursorPosX(1685);
    ImGui::SetCursorPosY(0);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, NEGATIVE_BUTTON_HOVER);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, NEGATIVE_BUTTON_ACTIVE); 
    if(ImGui::Button(CLOSE_ICON, ImVec2(35, 35))){
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }
    ImGui::PopStyleColor(2);
    ImGui::End();
}

void GUI::WindowBorder(){
    ImGui::GetForegroundDrawList()->AddRect(
        ImVec2(0, 0),
        ImVec2(1720 - 1, 1040 - 1),
        IM_COL32(50, 50, 50, 255), 
        0.0f,                         
        0,                            
        2.0f                         
    );    
}

void GUI::EntitiesList(){
    ImGui::SetNextWindowPos(ImVec2(10, 45));
    ImGui::SetNextWindowSize(ImVec2(450, 310));
    ImGui::Begin("##ENTITIES_LIST", NULL, window_flags & ~ImGuiWindowFlags_NoScrollbar);
    ImGui::SetCursorPos(ImVec2(0,0));
    ImGui::BeginTable("Entity Table", 5, 0, ImVec2(460, 100));
    ImGui::TableSetupColumn(" Name");
    ImGui::TableSetupColumn("Type");
    ImGui::TableSetupColumn("Mass");
    ImGui::TableSetupColumn("Number");
    ImGui::TableSetupColumn("Color");
    ImGui::TableHeadersRow();
    size_t row = 0;
    for(const auto& group : Entities::groups){
        const Entity& entity = Entities::entities[group[0]];
        ImGui::TableNextRow();
        
        ImGui::TableSetColumnIndex(0);
        ImGui::PushID(row);
        if (ImGui::Selectable("##select", m_TableRowSelected == row, ImGuiSelectableFlags_SpanAllColumns)){
            m_TableRowSelected = row;
        }
        ImGui::PopID();
        ImGui::SameLine();
        ImGui::Text("%s", entity.GetName().c_str());

        ImGui::TableSetColumnIndex(1);
        if(entity.GetType() == EntityType::OBJECT){
            ImGui::Text("Object");
        }
        else if(entity.GetType() == EntityType::PARTICLE){
            ImGui::Text("Particle");
        }

        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%.2e", entity.GetMass());

        ImGui::TableSetColumnIndex(3);
        ImGui::Text("%d", group[1]);

        ImGui::TableSetColumnIndex(4);
        ImGui::ColorButton(("##COLOR" + std::to_string(row)).c_str(), ImVec4(entity.GetColor().x, entity.GetColor().y, entity.GetColor().z, 1.f), 0, ImVec2(18,18)); 
        
        row++;
    }
    ImGui::EndTable();
    ImGui::End();
}

void GUI::TimeInfo(){
    ImGui::SetNextWindowPos(ImVec2(10, 440));
    ImGui::SetNextWindowSize(ImVec2(450, 95));
    ImGui::Begin("Simulation Settings", NULL, window_flags);
    
    ImGui::SetCursorPos(ImVec2(0, 0));
    ImGui::BeginTable("##TIME_INFO", 2, ImGuiTableFlags_BordersInnerH, ImVec2(580, 0));

    ImGui::TableSetupColumn("left", ImGuiTableColumnFlags_WidthFixed, 325.0f);  
    ImGui::TableSetupColumn("right");    

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0); 
    ImGui::Text("Total Real Time");
    ImGui::TableSetColumnIndex(1); 
    ImGui::Text("%s", Timer::FormatTime(Timer::TotalRealTime).c_str());
    
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("Total simulation time");
    ImGui::TableSetColumnIndex(1);
    ImGui::Text("%s", Timer::FormatTime(Timer::TotalSimTime).c_str());
    
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("FPS");
    ImGui::TableSetColumnIndex(1);
    ImGui::Text("%d", Timer::FPS);
    
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("Delta Time");
    ImGui::TableSetColumnIndex(1);
    if(Settings::time_mode == TimeMode::REAL_TIME){
        m_NewDeltaTime = Timer::deltaTime;
        ImGui::BeginDisabled();
        ImGui::InputFloat("INPUT_DELTA_TIME", &m_NewDeltaTime, 0, 0, "%.4f");
        ImGui::EndDisabled();
    }
    else{
        if(ImGui::InputFloat("##INPUT_DELTA_TIME", &m_NewDeltaTime, 0, 0, "%.4f")){
            Timer::deltaTime = m_NewDeltaTime;   
        }
    }
    
    ImGui::EndTable();
    ImGui::End();
}

void GUI::TimeStateButton(){
    ImGui::SetNextWindowPos(ImVec2(30, 555));
    ImGui::SetNextWindowSize(ImVec2(125, 40));
    ImGui::Begin("##TIME_STATE", NULL, window_flags);
    ImGui::SetCursorPos(ImVec2(0, 0));
    if(Settings::state == State::STOPPED){
        if(ImGui::Button(CONTINIUE_ICON,  ImVec2(125, 40))){
            Settings::state = State::RUNNING;
        }
    }
    else{
        if(ImGui::Button(PAUSE_ICON,  ImVec2(125, 40))){
            Settings::state = State::STOPPED;
        }
    }
    ImGui::End();
}

void GUI::ResetButton(){
    ImGui::SetNextWindowPos(ImVec2(170, 555));
    ImGui::SetNextWindowSize(ImVec2(125, 40));
    ImGui::Begin("##RESET_TIME", NULL, window_flags);
    ImGui::SetCursorPos(ImVec2(0, 0));

    if(ImGui::Button(RESET_ICON, ImVec2(125, 40))){
        Timer::TotalRealTime = 0;
        Timer::TotalSimTime = 0;
        Settings::state = State::STOPPED;
        Settings::mode = Mode::NORMAL;
        Settings::time_mode = TimeMode::REAL_TIME;
    }
    ImGui::End();
}

void GUI::TimeModeButton(){
    ImGui::SetNextWindowPos(ImVec2(310, 555));
    ImGui::SetNextWindowSize(ImVec2(125, 40));
    ImGui::Begin("##TIME_MODE", NULL, window_flags);
    ImGui::SetCursorPos(ImVec2(0, 0));
    if(Settings::time_mode == TimeMode::REAL_TIME){
        if(ImGui::Button("REAL", ImVec2(125, 40))){
            Settings::time_mode = TimeMode::SIM_TIME;
        }
    }
    else{
        if(ImGui::Button("SIM", ImVec2(125, 40))){
            Settings::time_mode = TimeMode::REAL_TIME;
        }
    }
    ImGui::End();
}

void GUI::AccelerationOptions(){
    ImGui::SetNextWindowPos(ImVec2(10, 935));
    ImGui::SetNextWindowSize(ImVec2(450, 95));
    ImGui::Begin("##ACC_OPTIONS", NULL, window_flags);
    ImGui::Indent(75);
    ImGui::Dummy(ImVec2(0,10));
    static int current = 0;
    const char* categories[] = { "Normal", "High", "Ultra" };
    ImGui::Text("Perfomance: %s", categories[current]);
    if(ImGui::SliderInt("##quality_slider", &current, 0, IM_ARRAYSIZE(categories) - 1, "")){
       if(current == 0){Settings::mode = Mode::NORMAL;}
       else if(current == 1){Settings::mode = Mode::CPU_MULTITHREAD;}
       else if(current == 2){
            Settings::mode = Mode::GPU_ACCELERATION;
            PhysicsEngine::UpdateSSBO = true;
       }
    }
    ImGui::End();
}

void GUI::UsageAnalytics(){
    ImGui::SetNextWindowPos(ImVec2(925, 840));
    ImGui::SetNextWindowSize(ImVec2(310, 190));
    ImGui::Begin("##USAGE_ANALYTICS", NULL, window_flags);
    ImPlot::SetNextAxesLimits(0, 9, 0, 100, ImPlotCond_Always);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
    if(ImPlot::BeginPlot("##CPU_USAGE_PLOT", ImVec2(280, 180))){
        ImPlot::SetupAxis(ImAxis_X1, NULL, ImPlotAxisFlags_NoDecorations);
        ImPlot::SetupAxis(ImAxis_Y1, NULL, ImPlotAxisFlags_None);
        ImPlot::PlotShaded("##USAGE", &Hardware::cpuHistory[0], 10);
        ImPlot::EndPlot();
    }
    ImGui::PopStyleVar();
    ImGui::End();
}

void GUI::CPUInfo(){
    ImGui::SetNextWindowPos(ImVec2(485, 840));
    ImGui::SetNextWindowSize(ImVec2(430, 190));
    ImGui::Begin("##CPU_INFO", NULL, window_flags);
    ImGui::SetCursorPos(ImVec2(7.5, 20));
    ImGui::Text("CPU model: %s", Hardware::cpuModel.c_str());
    ImGui::Separator();
    ImGui::Text("Usage: %.0f%%", Hardware::cpuHistory[9]);
    ImGui::Separator();
    ImGui::Text("Cores: %s", Hardware::cpuCores.c_str());
    ImGui::Separator();
    ImGui::Text("Threads: %s", Hardware::cpuThreads.c_str());
    ImGui::Separator();
    ImGui::Text("Frequency: %s", Hardware::cpuFrequency.c_str());
    ImGui::End();
}

void GUI::EntityAnalytics(){
    UpdateEntityAnalytics();
    ImGui::SetNextWindowPos(ImVec2(1265, 45));
    ImGui::SetNextWindowSize(ImVec2(450, 985));
    ImGui::Begin("##ENTITY_OPTIONS", NULL, window_flags);
    if(Entities::entities.empty()){
        ImGui::End();
        return;
    }
    Entity entity = Entities::entities[m_EntitySelected];
    ImGui::BeginTable("##ENTITY_INFO", 2, ImGuiTableFlags_Borders);

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0); 
    ImGui::Text("Index:");
    ImGui::TableSetColumnIndex(1); 
    ImGui::Text("%d", m_EntitySelected);

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0); 
    ImGui::Text("Name:");
    ImGui::TableSetColumnIndex(1); 
    ImGui::Text("%s", entity.GetName().c_str());

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0); 
    ImGui::Text("Type:");
    ImGui::TableSetColumnIndex(1); 
    if(entity.GetType() == EntityType::OBJECT){
        ImGui::Text("Object");
    }
    else if(entity.GetType() == EntityType::PARTICLE){
        ImGui::Text("Particle");
    }

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0); 
    ImGui::Text("Mass:");
    ImGui::TableSetColumnIndex(1); 
    ImGui::Text("%.2e kg", entity.GetMass());

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0); 
    ImGui::Text("Position:");
    ImGui::TableSetColumnIndex(1); 
    ImGui::Text("(%.0f, %.0f)", entity.GetPosition().x, entity.GetPosition().y);

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0); ImGui::Text("Color:");
    ImGui::TableSetColumnIndex(1); 
    ImGui::ColorButton("ColorPreview", ImVec4(entity.GetColor().x, entity.GetColor().y, entity.GetColor().z, 1.0f));
    ImGui::EndTable();
    PositionPlot();
    ForcePlot();
    ShowDirections();
    ImGui::End();
}

void GUI::PositionPlot(){
    ImGui::Indent(50);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
    ImPlot::SetNextAxesLimits(0, 800, 0, 800, ImPlotCond_Always);
    if (ImPlot::BeginPlot("##POS_PLOT", ImVec2(300, 300))) { 
        ImPlot::SetupAxis(ImAxis_Y1, "", ImPlotAxisFlags_Invert);
        ImPlot::PlotScatter("", &m_PositionPlotDataX[0], &m_PositionPlotDataY[0], 128); 
        ImPlot::EndPlot(); 
    }
    ImGui::Unindent(50);
    ImGui::PopStyleVar();
}

void GUI::ForcePlot(){
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
    ImPlot::SetNextAxesLimits(0, 128, 0, m_MaxForce*2+1 , ImPlotCond_Always);
    if(ImPlot::BeginPlot("##FORCE_PLOT", ImVec2(425, 250))){
        ImPlot::SetupAxis(ImAxis_X1, NULL, ImPlotAxisFlags_NoDecorations);
        ImPlot::PlotShaded("Force" , &m_ForcePlotData[0], 128);
        ImPlot::EndPlot();
    }
    ImGui::PopStyleVar();
}

void GUI::TimePlot(){
    ImGui::SetNextWindowPos(ImVec2(10, 615));
    ImGui::SetNextWindowSize(ImVec2(450, 300));
    ImGui::Begin("##TIME_ANALYTICS", NULL, window_flags);
    ImGui::SetCursorPos(ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
    ImPlot::SetNextAxisLimits(ImAxis_Y1, 0, 0.1);
    if (ImPlot::BeginPlot("##TIME_CHART", ImVec2(400, 300))) {
    static const char* labels[] = { "Physics", "Integrator", "Events", "Rendering" };
    static float values[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    static double positions[] = { 0, 1, 2, 3 };

    values[0] = Timer::PhysicsEngineTime;
    values[1] = Timer::IntegratorTime;
    values[2] = Timer::EventProcessingTime;
    values[3] = Timer::RenderingTime;

    ImPlot::SetupAxis(ImAxis_X1, "Module"); 
    ImPlot::SetupAxis(ImAxis_Y1, "Time(s)");
    ImPlot::SetupAxisFormat(ImAxis_Y1, "%.1e"); 
    ImPlot::SetupAxisTicks(ImAxis_X1, positions, 4, labels); 
    ImPlot::PlotBars("Data", values, 4, 0.5f);
    ImPlot::EndPlot();
    ImGui::PopStyleVar();
    ImGui::End();
}
}

void GUI::UpdateEntityAnalytics(){
    if(Entities::entities.empty() || Settings::state == State::STOPPED){
        return;
    }
    if(Timer::ShouldUpdateAnalytics()){
        for(int i = 1; i < 128; ++i){
            m_PositionPlotDataX[i - 1] = m_PositionPlotDataX[i]; 
            m_PositionPlotDataY[i - 1] = m_PositionPlotDataY[i];
            m_ForcePlotData[i - 1] = m_ForcePlotData[i];
        }
        m_PositionPlotDataX[127] = Entities::entities[m_EntitySelected].GetPosition().x;
        m_PositionPlotDataY[127] = Entities::entities[m_EntitySelected].GetPosition().y;
        m_ForcePlotData[127] = Entities::entities[m_EntitySelected].GetForce();
        m_MaxForce = m_ForcePlotData[127] > m_MaxForce ? m_ForcePlotData[127] : m_MaxForce;
    }
}

void GUI::ResetPositionPlotData(){
    for(int i = 0; i < 128; i++){
        m_PositionPlotDataX[i] = -10;
        m_PositionPlotDataY[i] = -10;
    }
}

void GUI::ResetForcePlotData(){
    for(int i = 0; i < 128; i++){
        m_ForcePlotData[i] = 0;
    }
    m_MaxForce = 0;
}

void GUI::AddEntityBtn(){
    ImGui::SetNextWindowPos(ImVec2(30, 375));
    ImGui::SetNextWindowSize(ImVec2(125, 40));
    ImGui::Begin("##ADDENTITY", NULL, window_flags);
    ImGui::SetCursorPos(ImVec2(0, 0));
    if(ImGui::Button(ADD_ICON,  ImVec2(125, 40))){
        CALL_ADDENTITY_FORM ^= true;
    }
    ImGui::End();
}

void GUI::AddEntityForm(){
    if(CALL_ADDENTITY_FORM){
        ImGui::SetNextWindowPos(ImVec2(410, 50));
        ImGui::SetNextWindowSize(ImVec2(900, 900));

        ImGui::Begin("##ADDENTITY_FORM", NULL, window_flags & ~ImGuiWindowFlags_NoBringToFrontOnFocus);
        ImGui::Dummy(ImVec2(0, 40));
        ImGui::Indent(100);
        
        ImGui::BeginChild("##POSITION_INPUT", ImVec2(275, 135), true);
        ImGui::Text("Position Input");
        ImGui::Dummy(ImVec2(0, 12));
        ImGui::SetNextItemWidth(75);
        ImGui::InputInt("X-Axis (meters)", &m_NewPosX, 0);
        ImGui::SetNextItemWidth(75);
        ImGui::InputInt("Y-Axis (meters)", &m_NewPosY, 0);
        ImGui::SliderInt("Size", &m_NewSize, 10, 400);
            
        ImGui::EndChild();
        
        ImGui::Dummy(ImVec2(0, 20));

        ImGui::BeginChild("##SIMVALUES_INPUT", ImVec2(275, 235), true);
        ImGui::Text("Simulation values input");
        ImGui::Dummy(ImVec2(0, 20));
        ImGui::SetNextItemWidth(75);
        ImGui::InputFloat("Mass (kg)", &m_NewMass, 0, 0, "%.2e");
        ImGui::SetNextItemWidth(75);
        ImGui::InputFloat("Velocity on X-Axis (m/s)", &m_InitVelocityX);
        ImGui::SetNextItemWidth(75);
        ImGui::InputFloat("Velocity on Y-Axis (m/s)", &m_InitVelocityY);
        ImGui::Dummy(ImVec2(0, 20));
        ImGui::SetNextItemWidth(125);
        ImGui::Combo("Entity type", &m_EntityTypeSelected, EntityTypes, 2);
        ImGui::Dummy(ImVec2(0, 5));
        ImGui::SetNextItemWidth(200);
        ImGui::InputText("Name", m_newName, (int)(sizeof(m_newName) / sizeof(*m_newName)));
        
        ImGui::EndChild();

        ImGui::Dummy(ImVec2(0, 20));
        ImGui::BeginChild("##NUM_TO_ADD", ImVec2(275, 100), true);
        ImGui::Text("Number of entities to add");
        ImGui::Dummy(ImVec2(0, 20));
        if(ImGui::InputInt("##NUM_TO_ADD_INPUT", &m_NumToAdd)){
            if(m_NumToAdd < 1){m_NumToAdd = 1;}
        }
        ImGui::EndChild();

        ImGui::SameLine();
        
        ImGui::SetCursorPosX(405);
        ImGui::BeginChild("##COLOR", ImVec2(400, 100), true);
        ImGui::Text("Color Picker");
        ImGui::ColorEdit3("Pick Color", m_NewColor);
        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(315, 700));
       
        ImGui::BeginChild("##CONFIRM_BTN_ADD", ImVec2(125, 40), true, window_flags);
        ImGui::SetCursorPos(ImVec2(0,0));
        if(ImGui::Button("Confirm", ImVec2(125, 40))){
            if(m_NumToAdd == 1){
                if(m_EntityTypeSelected == 0){
                    Entities::CreateEntity(m_NewMass, m_NewSize, Vector2f(m_NewPosX, m_NewPosY), EntityType::OBJECT, Vector3f(m_NewColor[0], m_NewColor[1], m_NewColor[2]), m_newName, Vector2f(m_InitVelocityX, m_InitVelocityY));
                }
                else if(m_EntityTypeSelected == 1){
                    Entities::CreateEntity(m_NewMass, m_NewSize, Vector2f(m_NewPosX, m_NewPosY), EntityType::PARTICLE, Vector3f(m_NewColor[0], m_NewColor[1], m_NewColor[2]), m_newName, Vector2f(m_InitVelocityX, m_InitVelocityY));
                }
            }
            else{
                if(m_EntityTypeSelected == 0){
                    Entities::CreateBachOfEntities(m_NewMass, m_NewSize, Vector2f(m_NewPosX, m_NewPosY), EntityType::OBJECT, Vector3f(m_NewColor[0], m_NewColor[1], m_NewColor[2]), m_newName, m_NumToAdd, Vector2f(m_InitVelocityX, m_InitVelocityY));
                }
                else if(m_EntityTypeSelected == 1){
                    Entities::CreateBachOfEntities(m_NewMass, m_NewSize, Vector2f(m_NewPosX, m_NewPosY), EntityType::PARTICLE, Vector3f(m_NewColor[0], m_NewColor[1], m_NewColor[2]), m_newName, m_NumToAdd, Vector2f(m_InitVelocityX, m_InitVelocityY));
                }
            }
            CALL_ADDENTITY_FORM = false;
        }
        ImGui::EndChild();
        
        ImGui::SameLine();
        ImGui::Dummy(ImVec2(20, 0));
        ImGui::SameLine();

        ImGui::BeginChild("##CANCLE_BTN_ADD", ImVec2(125, 40), true, window_flags);
        ImGui::SetCursorPos(ImVec2(0,0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, NEGATIVE_BUTTON_HOVER);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, NEGATIVE_BUTTON_ACTIVE);
        if(ImGui::Button("Cancel", ImVec2(125, 40))){
            CALL_ADDENTITY_FORM = false;
        }
        ImGui::PopStyleColor(2);
        ImGui::EndChild();
        ImGui::SetCursorPos(ImVec2(0, 50));
        PositionPicker();
        ImGui::End();
    }

}

void GUI::DeleteEntityBtn(){
    ImGui::SetNextWindowPos(ImVec2(170, 375));
    ImGui::SetNextWindowSize(ImVec2(125, 40));
    ImGui::Begin("##DELETEENTITY", NULL, window_flags);
    ImGui::SetCursorPos(ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, NEGATIVE_BUTTON_HOVER);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, NEGATIVE_BUTTON_ACTIVE);
    if(ImGui::Button(TRASH_ICON,  ImVec2(125, 40))){
        if(Entities::groups.size() != 0){
            if(Entities::groups[m_TableRowSelected][1] == 1){
                Entities::DeleteEntity(m_TableRowSelected); 
            }
            else{
                Entities::DeleteBachOfEntities(m_TableRowSelected);
            }
            m_TableRowSelected = 0;
        }
    }
    ImGui::PopStyleColor(2);
    ImGui::End();
}

void GUI::SearchEntityBtn(){ 
    ImGui::SetNextWindowPos(ImVec2(310, 375));
    ImGui::SetNextWindowSize(ImVec2(125, 40));
    ImGui::Begin("##SEARCHENTITY", NULL, window_flags);
    ImGui::SetCursorPos(ImVec2(0, 0));
    if(ImGui::Button(SEARCH_ICON,  ImVec2(125, 40) ) && !Entities::entities.empty()){
        if(Entities::groups[m_TableRowSelected][1] == 1){
            m_EntitySelected = Entities::groups[m_TableRowSelected][0];
            ResetPositionPlotData();
            ResetForcePlotData();
        }
        else{
            CALL_ENTITY_SEARCH ^= true;
        }
    }
    ImGui::End();
}

void GUI::EntitySearch(){
    if(CALL_ENTITY_SEARCH){
        ImGui::SetNextWindowPos(ImVec2(410, 50));
        ImGui::SetNextWindowSize(ImVec2(900, 900));

        ImGui::Begin("##ENTITY_SEARCH", NULL, window_flags & ~ImGuiWindowFlags_NoBringToFrontOnFocus & ~ImGuiWindowFlags_NoScrollbar);
        ImGui::SetCursorPos(ImVec2(0,0));
        ImGui::BeginTable("Entity Search Table", 5, 0, ImVec2(890, 100));
        ImGui::TableSetupColumn(" Index");
        ImGui::TableSetupColumn("Position");
        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Mass");
        ImGui::TableSetupColumn("Color");
        ImGui::TableHeadersRow();
        size_t row = 0;
        for(int i = Entities::groups[m_TableRowSelected][0]; i < Entities::groups[m_TableRowSelected][1]; i++){
            const Entity& entity = Entities::entities[i];
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::PushID(row);
            if (ImGui::Selectable("##select", m_EntitySelected == row, ImGuiSelectableFlags_SpanAllColumns)){
                m_EntitySelected= row;
            }

            if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)){
                    CALL_ENTITY_SEARCH = false;
                    ResetPositionPlotData();
                    ResetForcePlotData();
            }

            ImGui::PopID();
            ImGui::SameLine();
            ImGui::Text("%d", i);

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("(%.0f, %.0f)", entity.GetPosition().x, entity.GetPosition().y);

            ImGui::TableSetColumnIndex(2);
            if(entity.GetType() == EntityType::OBJECT){
                ImGui::Text("Object");
            }
            else if(entity.GetType() == EntityType::PARTICLE){
                ImGui::Text("Particle");
            }

            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%.2e", entity.GetMass());


            ImGui::TableSetColumnIndex(4);
            ImGui::ColorButton(("##COLOR" + std::to_string(row)).c_str(), ImVec4(entity.GetColor().x, entity.GetColor().y, entity.GetColor().z, 1.f), 0, ImVec2(18,18)); 

            row++;
        }
        ImGui::EndTable();
        ImGui::End();
    } 
}

void GUI::PositionPicker(){
    ImVec2 pickerSize(400, 400);
    ImGui::Dummy(pickerSize); 
    ImGui::SameLine();
    ImGui::BeginChild("Canvas", pickerSize, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();

    
    if (ImGui::InvisibleButton("##canvas", pickerSize))
    {
        ImVec2 mousePos = ImGui::GetMousePos();
        m_NewPosX = 2.25*(mousePos.x - canvasPos.x);
        m_NewPosY = 2.25*(mousePos.y - canvasPos.y);
    }

    ImVec2 markerPos = ImVec2(canvasPos.x + m_NewPosX/2.25, canvasPos.y + m_NewPosY/2.25);
    if(m_EntityTypeSelected == 0){
        float padding = m_NewSize/2.25 + m_NewSize / 4.5;
        drawList->AddCircleFilled(markerPos, m_NewSize/2.25, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
        if(m_NumToAdd == 2){
            drawList->AddCircleFilled(ImVec2(markerPos.x - padding, markerPos.y + m_NewSize/2.25 + 5), m_NewSize/2.25, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
        }
        else if(m_NumToAdd == 3){
            drawList->AddCircleFilled(ImVec2(markerPos.x - padding, markerPos.y + padding), m_NewSize/2.25, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
            drawList->AddCircleFilled(ImVec2(markerPos.x + padding, markerPos.y - padding), m_NewSize/2.25, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
        }
        else if(m_NumToAdd == 4){
            drawList->AddCircleFilled(ImVec2(markerPos.x - padding, markerPos.y + padding), m_NewSize/2.25, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
            drawList->AddCircleFilled(ImVec2(markerPos.x + padding, markerPos.y - padding), m_NewSize/2.25, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
            drawList->AddCircleFilled(ImVec2(markerPos.x + padding, markerPos.y + padding), m_NewSize/2.25, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
        }
        else if(m_NumToAdd > 4){
            drawList->AddCircleFilled(ImVec2(markerPos.x - padding, markerPos.y + padding), m_NewSize/2.25, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
            drawList->AddCircleFilled(ImVec2(markerPos.x + padding, markerPos.y - padding), m_NewSize/2.25, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
            drawList->AddCircleFilled(ImVec2(markerPos.x + padding, markerPos.y + padding), m_NewSize/2.25, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
            drawList->AddCircleFilled(ImVec2(markerPos.x - padding, markerPos.y - padding), m_NewSize/2.25, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
        }
    }
    else{
        drawList->AddCircleFilled(markerPos, 5, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
        if(m_NumToAdd == 2){
            drawList->AddCircleFilled(ImVec2(markerPos.x - m_NewSize/2.25 - 10, markerPos.y + m_NewSize/2.25 + 5), 5, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
        }
        else if(m_NumToAdd == 3){
            drawList->AddCircleFilled(ImVec2(markerPos.x - m_NewSize/2.25 - 5, markerPos.y + m_NewSize/2.25 + 5), 5, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
            drawList->AddCircleFilled(ImVec2(markerPos.x + m_NewSize/2.25 + 5, markerPos.y - m_NewSize/2.25 - 5), 5, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
        }
        else if(m_NumToAdd == 4){
            drawList->AddCircleFilled(ImVec2(markerPos.x - m_NewSize/2.25 - 5, markerPos.y + m_NewSize/2.25 + 5), 5, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
            drawList->AddCircleFilled(ImVec2(markerPos.x + m_NewSize/2.25 + 5, markerPos.y - m_NewSize/2.25 - 5), 5, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
            drawList->AddCircleFilled(ImVec2(markerPos.x + m_NewSize/2.25 + 5, markerPos.y + m_NewSize/2.25 + 5), 5, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
        }
        else if(m_NumToAdd > 4){
            drawList->AddCircleFilled(ImVec2(markerPos.x - m_NewSize/2.25 - 5, markerPos.y + m_NewSize/2.25 + 5), 5, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
            drawList->AddCircleFilled(ImVec2(markerPos.x + m_NewSize/2.25 + 5, markerPos.y - m_NewSize/2.25 - 5), 5, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
            drawList->AddCircleFilled(ImVec2(markerPos.x + m_NewSize/2.25 + 5, markerPos.y + m_NewSize/2.25 + 5), 5, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
            drawList->AddCircleFilled(ImVec2(markerPos.x - m_NewSize/2.25 - 5, markerPos.y - m_NewSize/2.25 - 5), 5, IM_COL32(m_NewColor[0]*255, m_NewColor[1]*255, m_NewColor[2]*255, 255));
        }
    }

    ImGui::EndChild();
}

void GUI::ShowDirections(){
    float length = 100;
    float arrowheadLength = 15; 
    float arrowheadAngle = 30.0f; 

    Vector2f velocity = Vector2f(Entities::entities[m_EntitySelected].GetData()->velX,
                                 Entities::entities[m_EntitySelected].GetData()->velY);

    float total_velocity = velocity(); 

    float velocity_ratioX = 0.0f;
    float velocity_ratioY = 0.0f;

    if (total_velocity >= 0.01f) {
        velocity_ratioX = velocity.x / total_velocity;
        velocity_ratioY = velocity.y / total_velocity;
    }

    Vector2f force = Entities::entities[m_EntitySelected].GetStoredForce();

    float total_force = force();

    float force_ratioX = 0.f;
    float force_ratioY = 0.f;

    if(total_force >= 0.01f){
        force_ratioX = force.x/total_force;
        force_ratioY = force.y/total_force;
    }

    ImGui::SetCursorPos(ImVec2(100, 720));      
    ImVec2 size = ImVec2(250, 250); 
    ImGui::BeginChild("CanvasTwo", size, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();  

    ImVec2 startPos = ImVec2(canvasPos.x + 125, canvasPos.y + 125);
    ImVec2 endPos = ImVec2(startPos.x + length * velocity_ratioX, startPos.y + length * velocity_ratioY);

    drawList->AddLine(startPos, endPos, IM_COL32(255, 0, 0, 255), 2.0f);

    float angle = arrowheadAngle * 3.14159265f / 180.0f; 
    float cosA = cos(angle);
    float sinA = sin(angle);

    ImVec2 left = ImVec2(
        endPos.x - arrowheadLength * (cosA * velocity_ratioX - sinA * velocity_ratioY),
        endPos.y - arrowheadLength * (sinA * velocity_ratioX + cosA * velocity_ratioY)
    );

    ImVec2 right = ImVec2(
        endPos.x - arrowheadLength * (cosA * velocity_ratioX + sinA * velocity_ratioY),
        endPos.y - arrowheadLength * (-sinA * velocity_ratioX + cosA * velocity_ratioY)
    );

    drawList->AddTriangleFilled(endPos, left, right, IM_COL32(255, 0, 0, 255));

    endPos = ImVec2(startPos.x + length * force_ratioX, startPos.y + length * force_ratioY);

    drawList->AddLine(startPos, endPos, IM_COL32(0, 0, 255, 255), 2.0f);

    left = ImVec2(
        endPos.x - arrowheadLength * (cosA * force_ratioX - sinA * force_ratioY),
        endPos.y - arrowheadLength * (sinA * force_ratioX + cosA * force_ratioY)
    );

    right = ImVec2(
        endPos.x - arrowheadLength * (cosA * force_ratioX + sinA * force_ratioY),
        endPos.y - arrowheadLength * (-sinA * force_ratioX + cosA * force_ratioY)
    );

    drawList->AddTriangleFilled(endPos, left, right, IM_COL32(0, 0, 255, 255));

    ImGui::EndChild();
}