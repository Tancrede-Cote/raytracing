#include "config.h"

float f = 16.67/1000.f;
int w = 1280;
int h = 720;
vec3 camPos(0.f,1.f,3.f);
vec3 camOrientation(0.f,0.f,-1.f);// unused for now

unsigned int camPosLocation;

unsigned int hLocation;
unsigned int wLocation;

unsigned int make_module(const std::string& filepath, unsigned int module_type) {
	
	std::ifstream file;
	std::stringstream bufferedLines;
	std::string line;

	file.open(filepath);
	while (std::getline(file, line)) {
		// std::cout << line << std::endl;
		bufferedLines << line << '\n';
	}
	std::string shaderSource = bufferedLines.str();
	const char* shaderSrc = shaderSource.c_str();
	// bufferedLines.str("");
	file.close();

	unsigned int shaderModule = glCreateShader(module_type);
	glShaderSource(shaderModule, 1, &shaderSrc, NULL);
	glCompileShader(shaderModule);

	int success;
	glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
	if (!success) {
		char errorLog[1024];
		glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
		std::cout << "Shader Module compilation error:\n" << errorLog << std::endl;
	}

	return shaderModule;
}

unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath) {

	//To store all the shader modules
	std::vector<unsigned int> modules;

	//Add a vertex shader module
	modules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));

	//Add a fragment shader module
	modules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));

	//Attach all the modules then link the program
	unsigned int shader = glCreateProgram();
	for (unsigned int shaderModule : modules) {
		glAttachShader(shader, shaderModule);
	}
	glLinkProgram(shader);

	//Check the linking worked
	int success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		char errorLog[1024];
		glGetProgramInfoLog(shader, 1024, NULL, errorLog);
		std::cout << "Shader linking error:\n" << errorLog << '\n';
	}

	//Modules are now unneeded and can be freed
	for (unsigned int shaderModule : modules) {
		glDeleteShader(shaderModule);
	}

	return shader;

}

App::App(){
    set_up_glfw();
}

App::~App(){
    glDeleteVertexArrays(VAOs.size(), VAOs.data());
    glDeleteBuffers(VBOs.size(), VBOs.data());
    glDeleteTextures(textures.size(), textures.data());

    glfwTerminate();
}

unsigned int App::make_entity(){
    return entity_count++;
}

void App::run(){
	TriangleMesh* triangle = new TriangleMesh();
    while(!glfwWindowShouldClose(window)){
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

    	glUniform1f(glGetUniformLocation(shader, "time"), static_cast<float>(glfwGetTime()));
		triangle->draw();
		glfwSwapBuffers(window);
        // motionSystem->update(
        //     transformComponents, physicsComponents, f);
        // bool should_close = cameraSystem->update(
        //     transformComponents, cameraID, *cameraComponent, f);
        // if (should_close){
        //     break;
        // }

        // renderSystem->update(transformComponents, renderComponents);
    }
}

unsigned int App::make_texture(const char* filename) {
    return 0;
    // std::cout << filename << std::endl;
    // int width, height, channels;
    // stbi_set_flip_vertically_on_load(true);
	// unsigned char* data = stbi_load(
    //     filename, &width, &height, &channels, STBI_rgb_alpha);

	// //make the texture
    // unsigned int texture;
	// glGenTextures(1, &texture);
    // textures.push_back(texture);
    // glBindTexture(GL_TEXTURE_2D, texture);
	
    // //load data
    // glTexImage2D(GL_TEXTURE_2D, 
    //     0, GL_RGBA, width, height, 0,
    //     GL_RGBA, GL_UNSIGNED_BYTE, data);

    // //free data
	// stbi_image_free(data);

    // //Configure sampler
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // return texture;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if(action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
    	glfwSetWindowShouldClose(window, true); // Closes the application if the escape key is pressed
  	}
}

void App::set_up_glfw() {

    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	
	window = glfwCreateWindow(w, h, "Hello Window!", NULL, NULL);
	glfwMakeContextCurrent(window);
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetKeyCallback(window, keyCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Couldn't load opengl" << std::endl;
		glfwTerminate();
	}

}

void App::set_up_opengl() {

    glClearColor(0.f, 0.f, 0.f, 1.0f);
	//Set the rendering region to the actual screen size
	glfwGetFramebufferSize(window, &w, &h);
	//(left, top, width, height)
	glViewport(0,0,w,h);

	// glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
	shader = make_shader(
		"../shaders/vertexShader.glsl", 
		"../shaders/fragmentShader.glsl");
    
	camPosLocation = glGetUniformLocation(shader, "camPos");
	hLocation = glGetUniformLocation(shader, "h");
	wLocation = glGetUniformLocation(shader, "w");
	glUseProgram(shader);
	glUniform3fv(camPosLocation, 1, camPos.value_ptr());
	glUniform1i(hLocation, h);
	glUniform1i(wLocation, w);
	// unsigned int projLocation = glGetUniformLocation(shader, "projection");
	// mat4 projection = mat4F::perspective(
	// 	45.0f, 1.f, 0.1f, 10.0f);
	// glUniformMatrix4fv(projLocation, 1, GL_FALSE, projection.value_ptr());
}

