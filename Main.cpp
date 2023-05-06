#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include"Model.h"


const unsigned int width = 1000;
const unsigned int height = 1000;

unsigned int samples = 8;

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, samples);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 1000 by 1000 pixels.
	GLFWwindow* window = glfwCreateWindow(width, height, "LaymEngine - Debug", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 1000, y = 1000
	glViewport(0, 0, width, height);

	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	// Enables the Depth Buffer and choses which depth function to use
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);


	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	Model bunny("models/bunny/scene.gltf");
	
	float FOV = 90.0;
	float near = 0.1f;
	float far = 100.0f;
	bool vSync = false;

	//bool frontFaceCull;
	//bool backFaceCull;
	//bool BothCull;

	glUniform1f(glGetUniformLocation(shaderProgram.ID, "near"), near);
	glUniform1f(glGetUniformLocation(shaderProgram.ID, "far"), far);

	glfwSwapInterval(0);
	
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;


	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;
		if (timeDiff >= 1.0 / 30.0)
		{
			prevTime = crntTime;
			camera.Inputs(window);
		}

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		std::cout << "specified bg color (opengl)" << std::endl;
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//ImGui window
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		

		
		camera.updateMatrix(FOV, 0.1f, 100.0f);



		shaderProgram.Activate();
		camera.Matrix(shaderProgram, "camMatrix");
		
		std::cout << "Drew the elements (opengl)" << std::endl;

		ImGui::Begin("Project settings");
		if (ImGui::Button("GUI Mode"))
		{
			camera.guiMode = true;
		}
		if (ImGui::Button("Cam Mode"))
		{
			camera.guiMode = false;
		}
		ImGui::SliderFloat("FOV", &FOV, 25.0f, 120.0f);
		ImGui::SliderFloat("Far Clipping Plane", &far, 0.1f, 1000.0f);
		ImGui::SliderFloat("Near Clipping Plane", &near, 0.01f, 1.0f);

		
	
		ImGui::End();

		ImGui::Begin("Style Settings");
		if (ImGui::Button("Change Style to Default"))
		{
			ImGui::StyleColorsDark();
		}
		if (ImGui::Button("Change Style to ImGuiClassic"))
		{
			ImGui::StyleColorsClassic();
		}
		if (ImGui::Button("Change Style to Light"))
		{
			ImGui::StyleColorsLight();
		}
		ImGui::End();

		ImGui::Begin("Debug");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		bunny.Draw(shaderProgram, camera);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();


	// Delete all the objects we've created
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}