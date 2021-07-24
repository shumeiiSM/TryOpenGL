#include <GL/glew.h>
#include <glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shaders.h"
#include "Texture.h"


/* ALL THESE WRITTEN IN SHADERS.CPP */

///* Return 2 Variables */
//struct ShaderProgramSource
//{
//    std::string VertexSource;
//    std::string FragmentSource;
//};
//
//static ShaderProgramSource ParseShader(const std::string& filepath)
//{
//    std::ifstream stream(filepath); 
//    
//    enum class ShaderType
//    {
//        NONE = -1, VERTEX = 0, FRAGMENT = 1
//    };
//
//    /* check the file line by line */
//    std::string line;
//    std::stringstream ss[2];
//    ShaderType type = ShaderType::NONE;
//
//    while (getline(stream, line))
//    {
//        if (line.find("#shader") != std::string::npos)
//        {
//            if (line.find("vertex") != std::string::npos)
//            {
//                // set mode to vertex
//                type = ShaderType::VERTEX;
//            }
//            else if (line.find("fragment") != std::string::npos)
//            {
//                // set mode to fragment
//                type = ShaderType::FRAGMENT;
//            }
//        }
//        else
//        {
//            ss[(int)type] << line << "\n";
//        }
//    }
//
//    return { ss[0].str(), ss[1].str() }; // return 2 variables - tuple/pair
//
//}
//
//static unsigned int CompileShader(unsigned int type, const std::string& source)
//{
//    unsigned int id = glCreateShader(type);
//    const char* src = source.c_str(); // a pointer to beginning of the data - return pointer to immutable array so cannot be changed
//    // OR = &source[0]; - looking out first char in the string return a memory address
//    glShaderSource(id, 1, &src, nullptr);
//    glCompileShader(id);
//
//    // TODO: Error handling
//    int result;
//    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
//    if (result == GL_FALSE) // (!result)
//    {
//        int length;
//        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
//        char* message = (char*)alloca(length * sizeof(char)); // a function allocate size bytes of space on the stack dynamically
//        glGetShaderInfoLog(id, length, &length, message);
//        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
//        std::cout << message << std::endl;
//        glDeleteShader(id);
//        return 0;
//    }
//
//    return id;
//}
//
//static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
//{
//    unsigned int program = glCreateProgram();
//    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
//    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
//
//    glAttachShader(program, vs);
//    glAttachShader(program, fs);
//    glLinkProgram(program); // links the program object specified by program
//    glValidateProgram(program); // checks to see whether the executables contained in program can execute given the current OpenGL state.
//
//    // can delete those shader since linked to the program alr & its immediate
//    glDeleteShader(vs); 
//    glDeleteShader(fs);
//
//    return program;
//}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Write this after having OpenGL context */
    glfwSwapInterval(1); // make the screen updates slower, higher no. = slower

    /* First you need to create a valid OpenGL rendering context - glfwMakeContextCurrent(window) */
    /* before calling glewInit to initialize the extension entry points */
    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }

    /* Print the OpenGL version we are using */
    // 4.6.0 - Build 27.20.100.9415
    std::cout << glGetString(GL_VERSION) << std::endl;

    /* Triangle */
    //float positions[6] = {
    //    // x, y
    //   -0.5f, -0.5f,
    //    0.0f, 0.5f,
    //    0.5f, -0.5f
    //    // OpenGL doesn't know how many float per vertices so need to specific your layout using glVertexAttribPointer
    //};

    { // start of scope

        /* Square */
        float positions[] = {
            // x, y  // texture coordinate for last 2 
           -0.5f, -0.5f, 0.0f, 0.0f, // 0 - BOTTOM LEFT 
            0.5f, -0.5f, 1.0f, 0.0f, // 1 - BOTTOM RIGHT 
            0.5f,  0.5f, 1.0f, 1.0f, // 2 - TOP RIGHT 
           -0.5f,  0.5f, 0.0f, 1.0f  // 3 - TOP LEFT

           /* remove duplicates */
           //-0.5f, -0.5f, 
           // 0.5f, -0.5f,
           // 0.5f,  0.5f,
           // 
           // 0.5f,  0.5f,
           //-0.5f,  0.5f,
           //-0.5f, -0.5f
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };


        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        /* After adding Vertex Array cpp, generate & bind is done there so this few line of code dont need */
        /* Explicitly creating a vao & how to use it */
        /*
        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao)); // stored in ID vao
        GLCall(glBindVertexArray(vao)); // no actual target so just specify the ID we want to bind 
        */

        /* Vertex Array */
        VertexArray va;

        /* Vertex Buffer */
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
                                    // ^ from  2 change to 4 since now got 4 floats per row
        /* Vertex Buffer Layout */
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);


        /* Vertex Buffer Replaced
        unsigned int buffer;
        GLCall(glGenBuffers(1, &buffer)); // generate a buffer and giving back an ID
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer)); // select which buffer you want to use
        //glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); // put data into that buffer
                                        // ^ in bytes (see docs.gl)

        GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW)); // SQUARE
        */

        /* Vertex Buffer Layout Replaced
        // Need to enable this vertex attribute if not will black screen - nothing will be render
        GLCall(glEnableVertexAttribArray(0)); // type this after your actual buffer is bound - glBindBuffer
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
                                                                    // ^ or 8
        */


        /* Index Buffer */
        IndexBuffer ib(indices, 6); // stack allocated object

        /*
        unsigned int ibo; // index buffer object
        GLCall(glGenBuffers(1, &ibo));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW)); // MUST be unsigned
        */

        //glBindBuffer(GL_ARRAY_BUFFER, 0); // if bind (select) something else then glDrawArrays cannot draw 


        //std::string vertexShader = 
        //    "#version 330 core\n"
        //    "\n"
        //    "layout(location = 0) in vec4 position;"
        //    "\n"
        //    "void main()\n"
        //    "{\n"
        //    "   gl_Position = position;\n"
        //    "}\n";

        //std::string fragmentShader =
        //    "#version 330 core\n"
        //    "\n"
        //    "layout(location = 0) out vec4 color;"
        //    "\n"
        //    "void main()\n"
        //    "{\n"
        //    "   color = vec4(1.0, 0.0, 0.0, 1.0);\n" // RGBA:  0 - black (0), 1 - white (255)
        //    "}\n";

        /* Shader Abstraction SA 1 */
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        /* Relative Path - inside properties > Debugging > Working Directory > $(ProjectDir) so is the dir contains our project file */
        // replaced sa1.1 - ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
        
        //std::cout << "VERTEX" << std::endl;
        //std::cout << source.VertexSource << std::endl;
        //std::cout << "FRAGMENT" << std::endl;
        //std::cout << source.FragmentSource << std::endl;

        //unsigned int shader = CreateShader(vertexShader, fragmentShader);
        // replaced sa1.2 - unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        // replaced sa1.3 - GLCall(glUseProgram(shader)); // installs the program object specified by program as part of current rendering state. 
        //// now the triangle is RED colour (or the color u indicate in the fragment shader)

        /* Shader Abstraction SA 2*/
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        // replaced sa2.1 - GLCall(int location = glGetUniformLocation(shader, "u_Color"));
        // replaced sa2.2 - ASSERT(location != -1); // -1 means cannot find the location
        // replaced sa2.3 - //GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f)); // PINK

        /* Texture */
        Texture texture("res/textures/ChernoLogo.png");
        texture.Bind(); // default = 0 so no need pass anything
        shader.SetUniform1i("u_Texture", 0); // since we bound that texture to GL_TEXTURE0 (slot zero) so must match that 0

        /* Unbound all the buffers */
        va.Unbind();  // replaced GLCall(glBindVertexArray(0));
        vb.Unbind(); // replaced GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        ib.Unbind(); // replaced GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        shader.Unbind(); // replaced GLCall(glUseProgram(0));

        /* Renderer */
        Renderer renderer;

        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();
            //glClear(GL_COLOR_BUFFER_BIT); // handle by the renderer

            ///* Drawing a triangle */
            //glBegin(GL_TRIANGLES);
            //glVertex2f(-0.5f, -0.5f);
            //glVertex2f(0.0f, 0.5f);
            //glVertex2f(0.5f, -0.5f);
            //glEnd();

            //glDrawArrays(GL_TRIANGLES, 0, 3); // TRIANGLE
            //glDrawArrays(GL_TRIANGLES, 0, 6); // SQUARE

            // indices == vertices  
            //-0.5f, -0.5f,  - 1
            // 0.0f, 0.5f,   - 2
            // 0.5f, -0.5f   - 3

            /* Clear Error First */
            //GLClearError();

            shader.Bind(); // replaced GLCall(glUseProgram(shader));
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f); // replaced GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

            /* After adding Vertex Array cpp, bind is done there so this line of code dont need */
            //GLCall(glBindVertexArray(vao)); // linking vertex buffer to vertex array object

            //GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));

            /* Done in Renderer.cpp so dont need to bind here again */
            //va.Bind();
            //ib.Bind(); // replaced glBindBuffer
            //GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo)); // index buffer object

            ///* Need to do these 2 code every times if we draw another object with diff layout */
            //GLCall(glEnableVertexAttribArray(0));
            //GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

            /* Renderer replaced glDrawElements */
            renderer.Draw(va, ib, shader); // telling renderer to draw these on screen by providing Vertex Array, Index Buffer and Shader

            /* Use with index buffers */
            //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // since we bound our ibo so just put nullptr
            //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr)); // Debugging purpose

            /* Then Check Errors */
            //ASSERT(GLLogCall());

            /* Bounce between 1 and 0 for RED in RGBA */
            if (r > 1.0f)
            {
                increment = -0.05f;
            }
            else if (r < 0.0f)
            {
                increment = 0.05f;
            }
            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        /* Clean up the shaders once done */
        /* After adding Shaders.cpp don't need to manually once reach end of the scope will be deleted by the destructor */
        //GLCall(glDeleteProgram(shader));

    } // end of scope

    glfwTerminate();
    return 0;
}