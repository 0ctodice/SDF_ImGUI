#include "GUI.h"

GUI::GUI(GLFWwindow* window, const char* glsl_version) {
	// Setup Dear ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	_io = &ImGui::GetIO();
	(void)_io;
	_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	SetupImGuiStyle();
}

void GUI::clean() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GUI::addPanel(Panel panel) { _panels.insert({ panel.getPanelName(), panel }); }

void GUI::render(GLFWwindow* currentContext) const {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	//DockSpace();

	for (auto it = _panels.begin(); it != _panels.end(); ++it) {
		it->second.renderPanel();
	}

	bool p_open = true;

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

ImGuiIO& GUI::getIo() const {
	return *_io;
}

void GUI::removePanelByName(std::string panelName) {
	auto it = _panels.find(panelName);
	if (it != _panels.end()) {
		it->second.setState(false);
	}
}