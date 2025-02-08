#include "GUI.h"

int GUI::selected = 0;

int GUI::new_mass = 0;
int GUI::new_posX = 0;
int GUI::new_posY = 0;
float GUI::new_velX = 0;
float GUI::new_velY = 0;

float GUI::DT = deltaTime;

//Initialize ImGui and ImPlot
void GUI::Initialize(GLFWwindow* window){
    IMGUI_CHECKVERSION(); 
	ImGui::CreateContext(); 
    ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); 
	(void)io; 
	io.Fonts->Clear();
    ImFont* myFont = io.Fonts->AddFontFromFileTTF("../fonts/Inter_18pt-Black.ttf", 18.0f);
    io.FontDefault = myFont;
    io.Fonts->Build();
    ImGui::StyleColorsDark(); 
    ImPlotStyle& style = ImPlot::GetStyle(); 
    style.Colors[ImPlotCol_FrameBg] = ImVec4(0, 0, 0, 0);
    style.Colors[ImPlotCol_AxisBgHovered] = ImVec4(0, 0, 0, 0);
    style.Colors[ImPlotCol_AxisBgActive] = ImVec4(0, 0, 0, 0);
	ImGui_ImplGlfw_InitForOpenGL(window, true); 
	ImGui_ImplOpenGL3_Init("#version 130");
   
}

//Create context and render data
void GUI::Draw(){
    ImGui_ImplOpenGL3_NewFrame(); 
	ImGui_ImplGlfw_NewFrame(); 
	ImGui::NewFrame();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoSavedSettings | 
                                    ImGuiWindowFlags_NoTitleBar | 
                                    ImGuiWindowFlags_NoResize | 
                                    ImGuiWindowFlags_NoMove;

    ImGui::Begin("Main Window", NULL, window_flags);
    ImGui::SetWindowPos(ImVec2(1000.f, 0.f));
    ImGui::SetWindowSize(ImVec2(780.f, 1000.f));
    ObjectList();
    InfoGraphics();
    ObjectCreator();
    ImGui::SameLine();
    TimeInfo();
    ClearButton();
    ImGui::End();
	ImGui::Render(); 
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

//Display info of the selected object
void GUI::InfoGraphics(){
    ImGui::BeginChild("Objects Info", ImVec2(770, 425), true);
    PositionPlot();
    ImGui::SameLine(0, 100);
    ForcePlot();
    ImGui::Indent(50.f);
    ImGui::Text("OBJECT ID: %d", OBJMG::objects[selected].GetID());
    ImGui::Text("OBJECT MASS: %.2f", OBJMG::objects[selected].GetMass());
    ImGui::Text("OBJECT ACC: %.4f", OBJMG::objects[selected].GetAcc()());
    ImGui::Text("OBJECT VELOCITY: %.4f", OBJMG::objects[selected].GetVelocity()());
    ImGui::SetCursorPos(ImVec2(475, 325));
    if(ImGui::Button("DELETE", ImVec2(200, 50))){
        OBJMG::Delete(selected);
        selected = 0;
    }
    ImGui::EndChild();
}

//Show list of all existing objects
void GUI::ObjectList(){
    ImGui::BeginChild("Object List", ImVec2(770.f, 250.f), true);
    ImGui::BeginTable("Objects list", 6, ImGuiTableFlags_RowBg);
    ImGui::TableSetupColumn("ID");
    ImGui::TableSetupColumn("MASS");
    ImGui::TableSetupColumn("POS X");
    ImGui::TableSetupColumn("POS Y");
    ImGui::TableSetupColumn("VEL X");
    ImGui::TableSetupColumn("VEL Y");
    ImGui::TableHeadersRow();
    for(int i = 0; i < OBJMG::objects.size(); i++){
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        if (ImGui::Selectable(std::to_string(OBJMG::objects[i].GetID()).c_str(), selected == i, ImGuiSelectableFlags_SpanAllColumns)) {
            selected = i;
        }
        
        ImGui::TableNextColumn();
        ImGui::Text("%.1f", OBJMG::objects[i].GetMass());
        ImGui::TableNextColumn();
        ImGui::Text("%.2f", OBJMG::objects[i].GetPos().x);
        ImGui::TableNextColumn();
        ImGui::Text("%.2f", OBJMG::objects[i].GetPos().y);
        ImGui::TableNextColumn();
        ImGui::Text("%.4f", OBJMG::objects[i].GetVelocity().x);
        ImGui::TableNextColumn();
        ImGui::Text("%.4f", OBJMG::objects[i].GetVelocity().y);
    }

    ImGui::EndTable();
    ImGui::EndChild();
}

//Show time info of the simulation
void GUI::TimeInfo(){
    ImGui::BeginChild("Time Info", ImVec2(400, 175), true);
    ImGui::Text("RUNTIME: %.2f", RUNTIME);            
    ImGui::SameLine(250.f);
    if(ImGui::Button("RESET ##RUNTIME_RESET", ImVec2(130, 25))){
        RUNTIME = 0;
    } 
    if(!SIM_MODE){
        ImGui::PushItemWidth(100);
        ImGui::BeginDisabled();
        ImGui::InputFloat("DELTA TIME ##DT_ENABLED", &DT, 0, 0, "%.3f");
        ImGui::EndDisabled();
        ImGui::PopItemWidth();
        ImGui::SameLine(250.f);
        if(ImGui::Button("SIM MODE ##ENTER_SIM_MODE", ImVec2(130, 25))){
            SIM_MODE = 1;
            deltaTime = 0.001;
        }
        DT = deltaTime;
    }       
    else{
        ImGui::PushItemWidth(100);
        ImGui::InputFloat("DELTA TIME ##DT_DISABLED", &DT, 0, 0, "%.3f");
        ImGui::PopItemWidth();
        ImGui::SameLine(250.f);
        if(ImGui::Button("REAL TIME ##ENTER_REAL_MODE", ImVec2(130, 25))){
            SIM_MODE = 0;
        }
        
        deltaTime = DT;
    }
    ImGui::Text("SIMULATION TIME: %.2f", SIM_RUNTIME);
    ImGui::SameLine(250.f);
    if(ImGui::Button("RESET ##SIM_RUNTIME_RESET", ImVec2(130, 25))){
        SIM_RUNTIME = 0;
    }         
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Indent(75.f);
    StopButton();
    ImGui::Unindent(75.f);
    ImGui::EndChild();
}

//Button for stopping simulation
void GUI::StopButton(){
    if(STOP_SIM){
        if(ImGui::Button("RESUME", ImVec2(200, 50))){STOP_SIM = 0;};
    }
    else{
        if(ImGui::Button("STOP", ImVec2(200, 50))){STOP_SIM = 1;};
    }
}

//Button for clearing all the objects from simulation(from the OBJMG)
void GUI::ClearButton(){
    ImGui::SetCursorPos(ImVec2(460, 900));
    if(ImGui::Button("CLEAR", ImVec2(200, 50))){
        OBJMG::objects.clear();
    }
}

//Creates the interface for user to create an object
void GUI::ObjectCreator(){
    ImGui::BeginChild("Creation tool", ImVec2(360, 300), true);
    ImGui::InputInt("MASS", &new_mass, 0);
    ImGui::InputInt("POS X", &new_posX, 0);
    ImGui::InputInt("POS Y", &new_posY, 0);
    ImGui::InputFloat("VEL X", &new_velX, 0, 0, "%.4f");
    ImGui::InputFloat("VEL Y", &new_velY, 0, 0, "%.4f");
    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    ImGui::Indent(75.f);
    if(ImGui::Button("CREATE", ImVec2(200, 50)) && new_mass > 0 && new_posX >= 0 && new_posY >= 0){
        OBJMG::AddObject(new_mass, Vector2f(new_posX, new_posY), Vector3f(1.f, 1.f, 1.f));
        OBJMG::objects.back().SetInitialVelocity(Vector2f(new_velX, new_velY));
    }
    ImGui::Unindent(75.f);
    ImGui::Text("The mass can't be less or equal to zero AND the ");
    ImGui::Text("position of the object must be in the [0, 1000]");
    ImGui::EndChild();
};

//Creates Position Plot/Graph of seleted object
void GUI::PositionPlot(){
    ImPlot::SetNextAxesLimits(0, 1000, 0, 1000, ImPlotCond_Always);
    if (ImPlot::BeginPlot("##POS_PLOT", ImVec2(300, 300))) { 
        ImPlot::SetupAxis(ImAxis_Y1, "", ImPlotAxisFlags_Invert);
        ImPlot::PlotScatter("Positions", OBJMG::objects[selected].GetTraceX(), OBJMG::objects[selected].GetTraceY(), 64); 
        ImPlot::EndPlot(); 
    }
}

//Creates the Force Plot/Graph of selected object
void GUI::ForcePlot(){
    //ImPlot::SetNextAxesToFit();
    ImPlot::SetNextAxesLimits(0, 16, 0, OBJMG::objects[selected].GetMaxForce() * 2 + 1, ImPlotCond_Always);
    if(ImPlot::BeginPlot("##FORCE_PLOT", ImVec2(300, 278))){
        ImPlot::SetupAxis(ImAxis_X1, NULL, ImPlotAxisFlags_NoDecorations);
        ImPlot::PlotShaded("Force" , OBJMG::objects[selected].GetForceHistory(), 16);
        ImPlot::EndPlot();
    }
}

//Cleant the ImGui and Implot
void GUI::Delete(){
    ImGui_ImplOpenGL3_Shutdown(); 
	ImGui_ImplGlfw_Shutdown(); 
    ImPlot::DestroyContext();
	ImGui::DestroyContext();
}


