#include <GL/glew.h>
#include <glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/* Function called will be different for each compiler */
#define ASSERT(x) if(!(x)) __debugbreak(); // compiler intrinsic means specific to MSBC functions instead of clang/gcc
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR); // since = 0 can write -> while (!glGetError());
}

//static void GLCheckError()
static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError()) 
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

/* Return 2 Variables */
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath); 
    
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    /* check the file line by line */
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                // set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                // set mode to fragment
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() }; // return 2 variables - tuple/pair

}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // a pointer to beginning of the data - return pointer to immutable array so cannot be changed
    // OR = &source[0]; - looking out first char in the string return a memory address
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // TODO: Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) // (!result)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char)); // a function allocate size bytes of space on the stack dynamically
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program); // links the program object specified by program
    glValidateProgram(program); // checks to see whether the executables contained in program can execute given the current OpenGL state.

    // can delete those shader since linked to the program alr & its immediate
    glDeleteShader(vs); 
    glDeleteShader(fs);

    return program;
}

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

    /* Square */
    float positions[] = {
        // x, y
       -0.5f, -0.5f, // 0
        0.5f, -0.5f, // 1
        0.5f,  0.5f, // 2
       -0.5f,  0.5f  // 3

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


    /* Vertex Buffer */
    unsigned int buffer;
    glGenBuffers(1, &buffer); // generate a buffer and giving back an ID
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // select which buffer you want to use
    //glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); // put data into that buffer
                                    // ^ in bytes (see docs.gl)

    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW); // SQUARE

    // Need to enable this vertex attribute if not will black screen - nothing will be render
    glEnableVertexAttribArray(0); // type this after your actual buffer is bound - glBindBuffer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
                                                        // ^ or 8

    /* Index Buffer */
    unsigned int ibo; // index buffer object
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW); // MUST be unsigned


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

    /* Relative Path - inside properties > Debugging > Working Directory > $(ProjectDir) so is the dir contains our project file */
    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
    //std::cout << "VERTEX" << std::endl;
    //std::cout << source.VertexSource << std::endl;
    //std::cout << "FRAGMENT" << std::endl;
    //std::cout << source.FragmentSource << std::endl;

    //unsigned int shader = CreateShader(vertexShader, fragmentShader);
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader); // installs the program object specified by program as part of current rendering state. 
    //// now the triangle is RED colour (or the color u indicate in the fragment shader)


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

        //glDrawArrays(GL_TRIANGLES, 0, 3); // TRIANGLE
        //glDrawArrays(GL_TRIANGLES, 0, 6); // SQUARE

        // indices == vertices  
        //-0.5f, -0.5f,  - 1
        // 0.0f, 0.5f,   - 2
        // 0.5f, -0.5f   - 3

        /* Clear Error First */
        GLClearError();

        /* Use with index buffers */
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); // since we bound our ibo so just put nullptr
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr)); // Debugging purpose

        /* Then Check Errors */
        //ASSERT(GLLogCall());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /* Clean up the shaders once done */
    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}