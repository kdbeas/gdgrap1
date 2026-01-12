#include <GLFW/glfw3.h>
#include <cmath>

constexpr float PI = 3.14159265358979323846f;

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    // Square window with your name
    window = glfwCreateWindow(600, 600, "Kaeden Tiu", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // 2D projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    const int sides = 9;
    const float radius = 0.8f;

    const float angleStep = 2.0f * PI / sides;
    const float rotationOffset = -PI / 2.0f + angleStep / 2.0f;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < sides; i++)
        {
            float angle = i * angleStep + rotationOffset;
            float x = radius * cosf(angle);
            float y = radius * sinf(angle);
            glVertex2f(x, y);
        }
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
