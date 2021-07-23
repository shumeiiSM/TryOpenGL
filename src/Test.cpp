#include <GL/glew.h>
#include <glfw3.h>

#include <iostream>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* First you need to create a valid OpenGL rendering context - line 23 */
    /* before calling glewInit to initialize the extension entry points */
    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }

    /* Print the OpenGL version we are using */
    // 4.6.0 - Build 27.20.100.9415
    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[6] = {
       -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer); // generate a buffer and giving back an ID
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // select which buffer you want to use
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); // put data into that buffer
                                    // ^ in bytes (see docs.gl)

    //glBindBuffer(GL_ARRAY_BUFFER, 0); // if bind (select) something else then glDrawArrays cannot draw 

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
     
        ///* Drawing a triangle */
        //glBegin(GL_TRIANGLES);
        //glVertex2f(-0.5f, -0.5f);
        //glVertex2f(0.0f, 0.5f);
        //glVertex2f(0.5f, -0.5f);
        //glEnd();

        glDrawArrays(GL_TRIANGLES, 0, 3); 
        // indices == vertices  
        //-0.5f, -0.5f,  - 1
        // 0.0f, 0.5f,   - 2
        // 0.5f, -0.5f   - 3
        
        /* Use with index buffers */
        //glDrawElements();
       

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}