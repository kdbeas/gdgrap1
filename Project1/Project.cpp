#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float x = 0.0f, y = 0.0f, z = 0.0f;

float x_scale = 1.0f, y_scale = 1.0f, z_scale = 1.0f;

float theta = 0.0f;
float x_axis = 0.0f, y_axis = 1.0f, z_axis = 0.0f;

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
    else
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        float moveSpeed = 0.05f;
        float scaleSpeed = 0.05f;
        float rotateSpeed = 5.0f;

        if (key == GLFW_KEY_D) x += moveSpeed;
        if (key == GLFW_KEY_A) x -= moveSpeed;
        if (key == GLFW_KEY_W) y += moveSpeed;
        if (key == GLFW_KEY_S) y -= moveSpeed;
        if (key == GLFW_KEY_C) z += moveSpeed;
        if (key == GLFW_KEY_Z) z -= moveSpeed;

        if (key == GLFW_KEY_E)
        {
            x_scale += scaleSpeed;
            y_scale += scaleSpeed;
            z_scale += scaleSpeed;
        }

        if (key == GLFW_KEY_Q)
        {
            x_scale -= scaleSpeed;
            y_scale -= scaleSpeed;
            z_scale -= scaleSpeed;
        }

        if (key == GLFW_KEY_RIGHT) theta += rotateSpeed;
        if (key == GLFW_KEY_LEFT)  theta -= rotateSpeed;
    }
}

int main(void)
{
    GLFWwindow* window;

    
    GLfloat UV [] = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
	};
    

    glm::mat3 identity_matrix3 = glm::mat3(1.0f);
    glm::mat4 identity_matrix4 = glm::mat4(1.0f);

    glm::mat4 translation_matrix = glm::translate(identity_matrix4, glm::vec3(x, y, z));
    glm::mat4 scale_matrix = glm::scale(identity_matrix4, glm::vec3(x_scale, y_scale, z_scale));
    glm::mat4 rotation_matrix = glm::rotate(identity_matrix4, glm::radians(theta), glm::vec3(x_axis, y_axis, z_axis));

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    


	float windowWidth = 800.0f;
	float windowHeight = 800.0f;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

	int img_width, img_height, colorChannels;

	//glViewport(0, 0, 320, 480);

    glfwSetKeyCallback(window, Key_Callback);

    //load the shader file into a string stream
    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    //add the file stream into the string stream
    vertBuff << vertSrc.rdbuf();

    //convert the stream to a character array
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    //load the shader file into a string stream
    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    //add the file stream into the string stream
    fragBuff << fragSrc.rdbuf();

    //convert the stream to a character array
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &v, NULL);
	glCompileShader(vertexShader);

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &f, NULL);
	glCompileShader(fragShader);

	GLuint shaderProg = glCreateProgram();
	glAttachShader(shaderProg, vertexShader);
	glAttachShader(shaderProg, fragShader);

	glLinkProgram(shaderProg);


    std::string path = "3D/myCube.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* tex_bytes = stbi_load("3D/ayaya.png", &img_width, &img_height, &colorChannels, 0);

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(&attributes, &shapes, &material, &warning, &error, path.c_str());

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }

    GLuint texture;

    glGenTextures(1, &texture);

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_bytes);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(tex_bytes);

    glEnable(GL_DEPTH_TEST);

	GLuint VAO, VBO, EBO, VBO_UV;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO_UV);
    glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * attributes.vertices.size(), &attributes.vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh_indices.size(), mesh_indices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (sizeof(UV) / sizeof(UV[0])), &UV[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 1.0f);

	glm::mat4 proj = glm::perspective(glm::radians(60.0f), windowWidth / windowHeight, 0.1f, 100.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLuint tex0Address = glGetUniformLocation(shaderProg, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(tex0Address, 0);
        
        glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 5.0f);
        glm::mat4 cameraPosMatrix = glm::translate(glm::mat4(1.f), cameraPos * -1.f);

        glm::vec3 WorldUp = glm::vec3(0.f, 1.f, 0.f);
        glm::vec3 cameraCenter = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::vec3 F = glm::normalize(cameraCenter - cameraPos);
        glm::vec3 R = glm::normalize(glm::cross(F, WorldUp));
        glm::vec3 U = glm::cross(R, F);

        glm::mat4 cameraRotMatrix = glm::mat4(1.f);
        cameraRotMatrix[0][0] = R.x;
        cameraRotMatrix[1][0] = R.y;
        cameraRotMatrix[2][0] = R.z;

        cameraRotMatrix[0][1] = U.x;
        cameraRotMatrix[1][1] = U.y;
        cameraRotMatrix[2][1] = U.z;

        cameraRotMatrix[0][2] = -F.x;
        cameraRotMatrix[1][2] = -F.y;
        cameraRotMatrix[2][2] = -F.z;

        glm::mat4 view = cameraRotMatrix * cameraPosMatrix;
        
        glm::mat4 transformation_matrix = glm::translate(identity_matrix4, glm::vec3(x, y, z));

        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(x_scale, y_scale, z_scale));

        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(theta), glm::vec3(x_axis, y_axis, z_axis));

		glUseProgram(shaderProg);

        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        unsigned int projLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}