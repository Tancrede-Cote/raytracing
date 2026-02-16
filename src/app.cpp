#include "config.h"

float f = 16.67 / 1000.f;
float r = 0.1;
float lr = 0.3; // light radius
int w = 1280;
int h = 720;
vec3 camPos(0.f, 1.f, 3.f);
vec3 plane(.0f, 1.f, .0f);
vec3 pa = vec3(1, 0, -1);
vec3 b = vec3(-1, 0, -1);
vec3 c = vec3(0, 1, -1);
bool jump = false;
Sphere *sphere;

unsigned int camPosLocation;
unsigned int planeLocation;

unsigned int hLocation;
unsigned int wLocation;
unsigned int rLocation;
unsigned int lrLocation;

float g_meshScale = 1.0; // to update based on the mesh size, so that navigation runs at scale
bool g_rotatingP = false;
bool g_panningP = false;
bool g_zoomingP = false;
double g_baseX = 0.0, g_baseY = 0.0;

unsigned int make_module(const std::string &filepath, unsigned int module_type)
{

	std::ifstream file;
	std::stringstream bufferedLines;
	std::string line;

	file.open(filepath);
	while (std::getline(file, line))
	{
		// std::cout << line << std::endl;
		bufferedLines << line << '\n';
	}
	std::string shaderSource = bufferedLines.str();
	const char *shaderSrc = shaderSource.c_str();
	// bufferedLines.str("");
	file.close();

	unsigned int shaderModule = glCreateShader(module_type);
	glShaderSource(shaderModule, 1, &shaderSrc, NULL);
	glCompileShader(shaderModule);

	int success;
	glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char errorLog[1024];
		glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
		std::cout << "Shader Module compilation error:\n"
				  << errorLog << std::endl;
	}

	return shaderModule;
}

unsigned int make_shader(const std::string &vertex_filepath, const std::string &fragment_filepath)
{

	// To store all the shader modules
	std::vector<unsigned int> modules;

	// Add a vertex shader module
	modules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));

	// Add a fragment shader module
	modules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));

	// Attach all the modules then link the program
	unsigned int shader = glCreateProgram();
	for (unsigned int shaderModule : modules)
	{
		glAttachShader(shader, shaderModule);
	}
	glLinkProgram(shader);

	// Check the linking worked
	int success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success)
	{
		char errorLog[1024];
		glGetProgramInfoLog(shader, 1024, NULL, errorLog);
		std::cout << "Shader linking error:\n"
				  << errorLog << '\n';
	}

	// Modules are now unneeded and can be freed
	for (unsigned int shaderModule : modules)
	{
		glDeleteShader(shaderModule);
	}

	return shader;
}

App::App()
{
	set_up_glfw();
}

App::~App()
{
	glDeleteVertexArrays(VAOs.size(), VAOs.data());
	glDeleteBuffers(VBOs.size(), VBOs.data());
	glDeleteTextures(textures.size(), textures.data());

	glfwTerminate();
}

unsigned int App::make_entity()
{
	return entity_count++;
}

void App::run()
{
	float dt = 0.016;
	float time;
	float old;
	TriangleMesh *triangle = new TriangleMesh();
	sphere = new Sphere(vec3(0, 2, 0), shader);
	rLocation = glGetUniformLocation(shader, "r");
	glUniform1f(rLocation, sphere->r);
	lrLocation = glGetUniformLocation(shader, "lr");
	glUniform1f(lrLocation, lr);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);
		glUniform3fv(planeLocation, 1, plane.value_ptr());
		old = time;
		float temp = static_cast<float>(glfwGetTime());
		time = temp;
		dt = time - old;
		glUniform1f(glGetUniformLocation(shader, "time"), time);
		sphere->jump = jump;
		sphere->update(dt);
		jump = false;
		glUniform3fv(glGetUniformLocation(shader, "pos"), 1, (sphere->pos()).value_ptr());
		triangle->draw();
		glfwSwapBuffers(window);
	}
	glDeleteProgram(shader);
	glfwDestroyWindow(window);
	// glfwTerminate();
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
	{
		glfwSetWindowShouldClose(window, true); // Closes the application if the escape key is pressed
	}
	else if (action == GLFW_PRESS && key == GLFW_KEY_SPACE)
	{
		std::cout << "JUMP" << std::endl;
		jump = true;
	}
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		// std::cout << "fdglihdsf" << std::endl;
		if (!g_rotatingP)
		{
			g_rotatingP = true;
			glfwGetCursorPos(window, &g_baseX, &g_baseY);
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		g_rotatingP = false;
	}
}

void cursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	const float normalizer = static_cast<float>((width + height) / 2);
	const float dx = static_cast<float>((g_baseX - xpos) / normalizer);
	const float dy = static_cast<float>((ypos - g_baseY) / normalizer);
	if (g_rotatingP)
	{
		mat3 RotX(vec3(1, 0, 0), vec3(0, cos(-dy * M_PI / 100), -sin(-dy * M_PI / 100)), vec3(0, sin(-dy * M_PI / 100), cos(-dy * M_PI / 100)));
		mat3 RotZ(vec3(cos(-dx * M_PI / 100), -sin(-dx * M_PI / 100), 0), vec3(sin(-dx * M_PI / 100), cos(-dx * M_PI / 100), 0), vec3(0, 0, 1));
		plane = (RotX)*plane;
		sphere->_pos = RotX * sphere->_pos;
	}
}

void App::set_up_glfw()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

	window = glfwCreateWindow(w, h, "Hello Window!", NULL, NULL);
	glfwMakeContextCurrent(window);
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Couldn't load opengl" << std::endl;
		glfwTerminate();
	}
}

void App::set_up_opengl()
{

	glClearColor(0.f, 0.f, 0.f, 1.0f);
	// Set the rendering region to the actual screen size
	glfwGetFramebufferSize(window, &w, &h);
	//(left, top, width, height)
	glViewport(0, 0, w, h);

	// glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	shader = make_shader(
		"../shaders/vertexShader.glsl",
		"../shaders/fragmentShader.glsl");

	planeLocation = glGetUniformLocation(shader, "plane");
	camPosLocation = glGetUniformLocation(shader, "camPos");
	hLocation = glGetUniformLocation(shader, "h");
	wLocation = glGetUniformLocation(shader, "w");
	glUseProgram(shader);
	glUniform3fv(camPosLocation, 1, camPos.value_ptr());
	glUniform1i(hLocation, h);
	glUniform1i(wLocation, w);
	glUniform3fv(glGetUniformLocation(shader, "a"), 1, pa.value_ptr());
	glUniform3fv(glGetUniformLocation(shader, "b"), 1, b.value_ptr());
	glUniform3fv(glGetUniformLocation(shader, "c"), 1, c.value_ptr());
}
