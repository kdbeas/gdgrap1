#include <GLFW/glfw3.h>
#include <cmath>

constexpr float PI = 3.14159265358979323846f;

int main()
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(600, 600, "Kaeden Tiu", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    const float r = 0.8f;
    const float step = 2.0f * PI / 9.0f;
    const float offset = -PI / 2.0f + step / 2.0f;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        // Draw nonagon
        glBegin(GL_LINE_LOOP);
            glVertex2f(r * cosf(0 * step + offset), r * sinf(0 * step + offset));
            glVertex2f(r * cosf(1 * step + offset), r * sinf(1 * step + offset));
            glVertex2f(r * cosf(2 * step + offset), r * sinf(2 * step + offset));
            glVertex2f(r * cosf(3 * step + offset), r * sinf(3 * step + offset));
            glVertex2f(r * cosf(4 * step + offset), r * sinf(4 * step + offset));
            glVertex2f(r * cosf(5 * step + offset), r * sinf(5 * step + offset));
            glVertex2f(r * cosf(6 * step + offset), r * sinf(6 * step + offset));
            glVertex2f(r * cosf(7 * step + offset), r * sinf(7 * step + offset));
            glVertex2f(r * cosf(8 * step + offset), r * sinf(8 * step + offset));
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
