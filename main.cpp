#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

//GLuint VAO = 0;
//GLuint VBO[2] = {0, 0};

GLuint program;
GLint attribute_coord2d;

std::string fileToBuf(std::string file){
  std::ifstream in(file);
  std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

  return content;
}


int main(int argc, char **argv) {

  SDL_Window* window = SDL_CreateWindow("SDL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
					640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  SDL_SetWindowIcon(window, IMG_Load("nvidia.png"));
  SDL_GLContext gl_context = SDL_GL_CreateContext(window);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

  std::cout << std::setw(20) << std::left << "OpenGL Version:" << glGetString(GL_VERSION) << "\n";
  std::cout << std::setw(20) << std::left << "GLSL Version:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
  std::cout << std::setw(20) << std::left << "Vendor:" << glGetString(GL_VENDOR) << "\n";
  std::cout << std::setw(20) << std::left << "Renderer:" << glGetString(GL_RENDERER) << "\n";

  glewExperimental = GL_TRUE;
  GLenum err = glewInit();

  if(err != GLEW_OK){
    std::cerr << "ERROR: GLEW could not initialize.\n";
    return 1;
  }

  if(!GLEW_VERSION_4_5){
    std::cerr << "ERROR: OpenGL 4.5 API is not available.\n";
    return 1;
  }

  GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

  std::string vertex_source_string = fileToBuf("VertexShader.glsl");
  std::string fragment_source_string = fileToBuf("FragmentShader.glsl");

  const char *vertex_source, *fragment_source;
  vertex_source = vertex_source_string.c_str();
  fragment_source = fragment_source_string.c_str();

  glShaderSource(vertex_shader, 1, &vertex_source, NULL);
  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_ok);
  if(!compile_ok){
    std::cout << "Error vertex shader.\n";
    return 1;
  }

  glShaderSource(fragment_shader, 1, &fragment_source, NULL);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_ok);
  if(!compile_ok){
    std::cout << "Error fragment shader.\n";
    return 1;
  }

  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if(!link_ok){
    std::cout << "Error program.\n";
    return 1;
  }

  const char* attribute_name = "coord2d";
  attribute_coord2d  = glGetAttribLocation(program, attribute_name);
  if(attribute_coord2d == -1){
    std::cout << "Could not bind attribute " << attribute_name << "\n";
    return 1;
  }

  glClearColor(1.0, 1.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(program);
  glEnableVertexAttribArray(attribute_coord2d);

  GLfloat vertices[] = {
     0.0,  0.8,
    -0.8, -0.8,
     0.8, -0.8,
  };

  glVertexAttribPointer(attribute_coord2d,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			vertices);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(attribute_coord2d);
  SDL_GL_SwapWindow(window);

  glDeleteProgram(program);

  SDL_Delay(5000);
  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
