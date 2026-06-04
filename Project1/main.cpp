#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <algorithm>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "P6/P6Particle.h"

using namespace std;
using namespace std::chrono_literals;

int main()
{
    constexpr std::chrono::nanoseconds timestep(16ms);

    if (!glfwInit())
        return -1;

    float windowWidth = 700;
    float windowHeight = 700;

    GLFWwindow* window = glfwCreateWindow(
        windowWidth,
        windowHeight,
        "PC01 Kaeden Tiu",
        NULL,
        NULL
    );

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    gladLoadGL();

    glEnable(GL_DEPTH_TEST);

    std::fstream vertSrc("Shaders/sample.vert");

    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();

    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc("Shaders/sample.frag");

    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();

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

    string path = "3D/sphere.obj";

    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> material;

    tinyobj::attrib_t attributes;

    string warning;
    string error;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    std::vector<GLfloat> fullVertexData;

    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        tinyobj::index_t vData =
            shapes[0].mesh.indices[i];

        fullVertexData.push_back(
            attributes.vertices[(vData.vertex_index * 3)]
        );

        fullVertexData.push_back(
            attributes.vertices[(vData.vertex_index * 3) + 1]
        );

        fullVertexData.push_back(
            attributes.vertices[(vData.vertex_index * 3) + 2]
        );

        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3)]
        );

        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3) + 1]
        );

        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3) + 2]
        );

        fullVertexData.push_back(
            attributes.texcoords[(vData.texcoord_index * 2)]
        );

        fullVertexData.push_back(
            attributes.texcoords[(vData.texcoord_index * 2) + 1]
        );
    }

    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * fullVertexData.size(),
        fullVertexData.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    GLintptr normalPtr = 3 * sizeof(float);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)normalPtr
    );

    glEnableVertexAttribArray(1);

    GLintptr uvPtr = 6 * sizeof(float);

    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)uvPtr
    );

    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm::mat4 view = glm::mat4(1.0f);

    glm::mat4 projection = glm::ortho(
        0.0f,
        windowWidth,
        0.0f,
        windowHeight,
        -500.0f,
        500.0f
    );

    std::vector<glm::vec3> spherePositions =
    {
        glm::vec3(0.f, 700.f, 201.f),
        glm::vec3(700.f, 700.f, 173.f),
        glm::vec3(700.f, 0.f, -300.f),
        glm::vec3(0.f, 0.f, -150.f)
    };

    std::vector<glm::vec3> sphereColors =
    {
        glm::vec3(1.f, 0.f, 0.f),
        glm::vec3(0.f, 1.f, 0.f),
        glm::vec3(0.f, 0.f, 1.f),
        glm::vec3(1.f, 1.f, 0.f)
    };

    glm::vec3 center =
        glm::vec3(350.f, 350.f, 0.f);

    P6::P6Particle redParticle;
    P6::P6Particle greenParticle;
    P6::P6Particle blueParticle;
    P6::P6Particle yellowParticle;

    redParticle.Position = spherePositions[0];
    greenParticle.Position = spherePositions[1];
    blueParticle.Position = spherePositions[2];
    yellowParticle.Position = spherePositions[3];

    glm::vec3 redDir =
        glm::normalize(center - spherePositions[0]);

    glm::vec3 greenDir =
        glm::normalize(center - spherePositions[1]);

    glm::vec3 blueDir =
        glm::normalize(center - spherePositions[2]);

    glm::vec3 yellowDir =
        glm::normalize(center - spherePositions[3]);

    redParticle.Velocity = redDir * 80.f;
    greenParticle.Velocity = greenDir * 90.f;
    blueParticle.Velocity = blueDir * 130.f;
    yellowParticle.Velocity = yellowDir * 110.f;

    redParticle.Acceleration = redDir * 14.5f;
    greenParticle.Acceleration = greenDir * 8.f;
    blueParticle.Acceleration = blueDir * 1.f;
    yellowParticle.Acceleration = yellowDir * 3.f;

    bool redFinished = false;
    bool greenFinished = false;
    bool blueFinished = false;
    bool yellowFinished = false;

    float redFinishTime = 0.f;
    float greenFinishTime = 0.f;
    float blueFinishTime = 0.f;
    float yellowFinishTime = 0.f;

    float redFinishVelocity = 0.f;
    float greenFinishVelocity = 0.f;
    float blueFinishVelocity = 0.f;
    float yellowFinishVelocity = 0.f;

    glm::vec3 redAverageVelocity;
    glm::vec3 greenAverageVelocity;
    glm::vec3 blueAverageVelocity;
    glm::vec3 yellowAverageVelocity;

    float elapsedTime = 0.f;

    float finishRadius = 10.f;

    bool rankingsPrinted = false;

    using clock = std::chrono::high_resolution_clock;

    auto curr_time = clock::now();
    auto prev_time = curr_time;

    std::chrono::nanoseconds curr_ns(0);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        curr_time = clock::now();

        auto dur =
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                curr_time - prev_time
            );

        prev_time = curr_time;

        curr_ns += dur;

        while (curr_ns >= timestep)
        {
            constexpr float timestep_sec =
                timestep.count() / (float)(1E9);

            elapsedTime += timestep_sec;

            if (!redFinished)
                redParticle.Update(timestep_sec);

            if (!greenFinished)
                greenParticle.Update(timestep_sec);

            if (!blueFinished)
                blueParticle.Update(timestep_sec);

            if (!yellowFinished)
                yellowParticle.Update(timestep_sec);

            if (!redFinished &&
                glm::distance(redParticle.Position, center)
                <= finishRadius)
            {
                redFinished = true;

                redParticle.Position = center;
                redParticle.Velocity = glm::vec3(0.f);
                redParticle.Acceleration = glm::vec3(0.f);

                redFinishTime = elapsedTime;

                redFinishVelocity =
                    glm::length(redDir * 80.f + redDir * 14.5f * redFinishTime);

                glm::vec3 displacement =
                    center - spherePositions[0];

                redAverageVelocity =
                    displacement / redFinishTime;
            }

            if (!greenFinished &&
                glm::distance(greenParticle.Position, center)
                <= finishRadius)
            {
                greenFinished = true;

                greenParticle.Position = center;
                greenParticle.Velocity = glm::vec3(0.f);
                greenParticle.Acceleration = glm::vec3(0.f);

                greenFinishTime = elapsedTime;

                greenFinishVelocity =
                    glm::length(greenDir * 90.f + greenDir * 8.f * greenFinishTime);

                glm::vec3 displacement =
                    center - spherePositions[1];

                greenAverageVelocity =
                    displacement / greenFinishTime;
            }

            if (!blueFinished &&
                glm::distance(blueParticle.Position, center)
                <= finishRadius)
            {
                blueFinished = true;

                blueParticle.Position = center;
                blueParticle.Velocity = glm::vec3(0.f);
                blueParticle.Acceleration = glm::vec3(0.f);

                blueFinishTime = elapsedTime;

                blueFinishVelocity =
                    glm::length(blueDir * 130.f + blueDir * 1.f * blueFinishTime);

                glm::vec3 displacement =
                    center - spherePositions[2];

                blueAverageVelocity =
                    displacement / blueFinishTime;
            }

            if (!yellowFinished &&
                glm::distance(yellowParticle.Position, center)
                <= finishRadius)
            {
                yellowFinished = true;

                yellowParticle.Position = center;
                yellowParticle.Velocity = glm::vec3(0.f);
                yellowParticle.Acceleration = glm::vec3(0.f);

                yellowFinishTime = elapsedTime;

                yellowFinishVelocity =
                    glm::length(yellowDir * 110.f + yellowDir * 3.f * yellowFinishTime);

                glm::vec3 displacement =
                    center - spherePositions[3];

                yellowAverageVelocity =
                    displacement / yellowFinishTime;
            }

            curr_ns -= timestep;
        }

        glUseProgram(shaderProg);

        glUniformMatrix4fv(
            glGetUniformLocation(shaderProg, "view"),
            1,
            GL_FALSE,
            glm::value_ptr(view)
        );

        glUniformMatrix4fv(
            glGetUniformLocation(shaderProg, "projection"),
            1,
            GL_FALSE,
            glm::value_ptr(projection)
        );

        GLint objectColorLoc =
            glGetUniformLocation(shaderProg, "objectColor");

        glm::mat4 identity =
            glm::mat4(1.0f);

        std::vector<P6::P6Particle*> particles =
        {
            &redParticle,
            &greenParticle,
            &blueParticle,
            &yellowParticle
        };

        for (int i = 0; i < particles.size(); i++)
        {
            glUniform3fv(
                objectColorLoc,
                1,
                glm::value_ptr(sphereColors[i])
            );

            glm::mat4 transform =
                glm::translate(
                    identity,
                    particles[i]->Position
                );

            transform = glm::scale(
                transform,
                glm::vec3(25.0f)
            );

            glUniformMatrix4fv(
                glGetUniformLocation(shaderProg, "transform"),
                1,
                GL_FALSE,
                glm::value_ptr(transform)
            );

            glBindVertexArray(VAO);

            glDrawArrays(
                GL_TRIANGLES,
                0,
                fullVertexData.size() / 8
            );

            glBindVertexArray(0);
        }

        if (!rankingsPrinted &&
            redFinished &&
            greenFinished &&
            blueFinished &&
            yellowFinished)
        {
            rankingsPrinted = true;

            std::cout << std::fixed
                << std::setprecision(2);

            float times[4] =
            {
                redFinishTime,
                greenFinishTime,
                blueFinishTime,
                yellowFinishTime
            };

            string names[4] =
            {
                "Red",
                "Green",
                "Blue",
                "Yellow"
            };

            float finishVelocities[4] =
            {
                redFinishVelocity,
                greenFinishVelocity,
                blueFinishVelocity,
                yellowFinishVelocity
            };

            glm::vec3 averageVelocities[4] =
            {
                redAverageVelocity,
                greenAverageVelocity,
                blueAverageVelocity,
                yellowAverageVelocity
            };

            int rankings[4] = { 0, 1, 2, 3 };

            for (int i = 0; i < 4; i++)
            {
                for (int j = i + 1; j < 4; j++)
                {
                    if (times[rankings[j]] < times[rankings[i]])
                    {
                        std::swap(rankings[i], rankings[j]);
                    }
                }
            }

            for (int i = 0; i < 4; i++)
            {
                int idx = rankings[i];

                std::string place;

                if (i == 0)
                    place = "1st";
                else if (i == 1)
                    place = "2nd";
                else if (i == 2)
                    place = "3rd";
                else
                    place = "4th";

                std::cout
                    << names[idx]
                    << ": "
                    << place
                    << "\n";

                std::cout
                    << "Mag. of Velocity: "
                    << round(finishVelocities[idx] * 100.0f) / 100.0f
                    << " m/s\n";

                std::cout
                    << "Average Velocity: ("
                    << round(averageVelocities[idx].x * 100.0f) / 100.0f
                    << ", "
                    << round(averageVelocities[idx].y * 100.0f) / 100.0f
                    << ", "
                    << round(averageVelocities[idx].z * 100.0f) / 100.0f
                    << ") m/s\n";

                std::cout
                    << round(times[idx] * 100.0f) / 100.0f
                    << " secs\n\n";
            }
        }

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);

    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}