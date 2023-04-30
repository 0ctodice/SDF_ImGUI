#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GUI.h"
#include "Program.h"
#include <vector>
#include "main.h"

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

struct Sphere {
	std::string name;
	float position[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	Sphere() : name { "" } {}
	Sphere(std::string n) : name {n} {}
};

std::string computeFS(std::vector<Sphere> spheres) {
	std::string FS = "#version 330 core\nout vec4 FragColor;\n";
	for (int i = 0; i < spheres.size(); i++) {
		FS += "uniform vec4 " + spheres[i].name + "POS;\n";
		FS += "uniform vec4 " + spheres[i].name + "COL;\n";
	}
	FS += "vec4 futureColor = vec4(1.0, 1.0, 1.0, 1.0);\n";
	FS += "float sdf_sphere(vec3 p, vec3 c, float r){ return distance(p,c) - r; }\n";
	FS += "float sdf_smin(float a, float b, float k, vec4 colA, vec4 colB){\n";
	FS += "float h = max( k-abs(a-b), 0.0 )/k;\n";
	FS += "float blend = pow(h,6.0)*0.5;\n";
	FS += "if(a < b) { futureColor = mix(colA, colB, blend * 0.001);} else { futureColor = mix(colA, colB, 1.0 - (blend * 0.001)); }\n";
	FS += "return min( a, b ) - blend*k*(1.0/6.0);\n";
	FS += "}\n";
	FS += "void main(){\n";
	FS += "float test = 1;\n";
	FS += "vec3 frag = vec3(gl_FragCoord.xyz * gl_FragCoord.w);\n";
	for (int i = 0; i < spheres.size(); i++) {
		FS += "test = sdf_smin(test, sdf_sphere(" + spheres[i].name + "POS.xyz, frag," + spheres[i].name + "POS.w), 32, futureColor, " + spheres[i].name + "COL);\n";
	}
	FS += "FragColor = futureColor;\n";
	FS += "}\0";
	/*FS += "if (test < 0){\n";
	FS += "	FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n";
	FS += "}else{\n";
	FS += "	FragColor = vec4(1.0, 1.0, 1.0, 1.0);}\n";
	FS += "}\0";*/

	std::cout << FS << std::endl;

	return FS;
}

const char* VS = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main(){\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"}\0";

int main(int, char**) {

	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	const char* glsl_version = "#version 330 core";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
	if (window == nullptr)
		return 1;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable
	gladLoadGL();

	GLfloat vertices[] = {
	-1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
	};

	GLuint vao, vbo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	std::vector<Sphere> spheres;
	bool shouldRecompFS = false;

	GUI gui{window, glsl_version};


	gui.addPanel({ "SDF Manager", [&]() {
		ImGui::Begin("SDF Manager");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / gui.getIo().Framerate, gui.getIo().Framerate);
		if (ImGui::Button("ADD SPHERE")) {
			std::string title = "SP" + std::to_string(spheres.size() + 1);
			spheres.emplace_back( title );
			shouldRecompFS = true;
		}

		for (int i = 0; i < spheres.size(); i++) {
			ImGui::Text(spheres[i].name.c_str());
			std::string labelPos = spheres[i].name + "POS";
			std::string labelCol = spheres[i].name + "COL";
			ImGui::DragFloat4(labelPos.c_str(), spheres[i].position);
			ImGui::ColorEdit4(labelCol.c_str(), spheres[i].color);
			ImGui::Separator();
		}

		ImGui::End();
	}});

	Program shaderProgram;

	shaderProgram.createProgram(VS, computeFS(spheres).c_str());

	shaderProgram.useProgram();

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h); 
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		if (shouldRecompFS) {
			shaderProgram.createProgram(VS, computeFS(spheres).c_str());
			shaderProgram.useProgram();
			shouldRecompFS = false;
		}

		for (int i = 0; i < spheres.size(); i++) {
			float shaderPoint[4]{ spheres[i].position[0], spheres[i].position[1], spheres[i].position[2], spheres[i].position[3] };
			shaderPoint[0] += display_w / 2.0f;
			shaderPoint[1] += display_h / 2.0f;
			std::string pos = spheres[i].name + "POS";
			std::string col = spheres[i].name + "COL";
			glUniform4fv(glGetUniformLocation(shaderProgram.getGLid(), pos.c_str()), 1, shaderPoint);
			glUniform4fv(glGetUniformLocation(shaderProgram.getGLid(), col.c_str()), 1, spheres[i].color);
		}

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		gui.render(glfwGetCurrentContext());

		glfwSwapBuffers(window);
	}
	// Cleanup

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	gui.clean();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}