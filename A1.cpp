// to run g++ tut1.cpp -lGL -lGLU -lglfw
// ./a.out

//  Code skeleton
//    from tutorial - Troy Alderson

// Older OpenGL
#define _USE_MATH_DEFINES
#define TWO_PI  (2*M_PI)
#define SEGMENTS 1000
#define WINDOWX 640
#define WINDOWY 640
#define BIGR 0
#define SMALLR 1
#define CYCLES 2

#include<GLFW/glfw3.h>	// Windows
#include<GL/gl.h>	     // OpenGL
#include<GL/glu.h>	  // utility library
#include<cmath>
#include<iostream>
using namespace std;

GLFWwindow *window;   // pointer to GLFW window
int w, h;             // width and height values

float small_r = 0.1f;  // small radius
float big_r = 0.3f;    // big radius
int cycles = 1;       // Number of rotations

int parameter = 0;    // toggle type

float scale = 1.0f;
float rotate = 0.0f;
float t = 1.0f;        // time
float dt = 0.0f;

int animationToggle = 0;
int circleToggle = 0;

void drawCircle(float px, float py, float r) {
  int i = 0;
  float theta = TWO_PI/SEGMENTS;
  for(i = 0; i < SEGMENTS+1; i++) {
    float x = r*cosf(theta * i);
    float y = r*sinf(theta * i);
    glVertex2f(px + x, py + y);
  }
}

void drawHypocycloid(float s_r, float b_r, int rotations, float t) {
  int i = 0;
  float theta = t * TWO_PI/SEGMENTS;
  float diff_r = b_r - s_r;
  for (i = 0; i < rotations * SEGMENTS; i++) {
    float x = diff_r*cosf(theta * i) + s_r*cosf((diff_r/s_r) * (theta * i));
    float y = diff_r*sinf(theta * i) - s_r*sinf((diff_r/s_r) * (theta * i));
    glVertex2f(x, y);
  }
}

void render() {
  // OpenGL calls go here
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glScalef(scale, scale, scale);
  glRotatef(rotate, 0.0f, 0.0f, 1.0f);


  glMatrixMode(GL_PROJECTION);  // scene - how it gets projected onto the screen
  glLoadIdentity();
  glOrtho(-(float)w/WINDOWX, (float)w/WINDOWX, -(float)h/WINDOWY, (float)h/WINDOWY, -1, 1); // view volume is a cube, which goes from -1 to 1 in all directions

  // pre computing useful values
  float temp = t * cycles * TWO_PI;
  float bsubs = big_r - small_r;

  glLineWidth(0.1f);

  glClearColor(0.2f, 0.2f, 0.2f, 0.0f);  // background color

  if (circleToggle != 0) {
    // Draw points at the center of the small circle and on the point on the curve
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex2f((bsubs) * cosf(temp), (bsubs) * sinf(temp));
    glVertex2f((bsubs) * cosf(temp) + small_r*cosf(((bsubs)/small_r) * (temp)),
               (bsubs) * sinf(temp) - small_r*sinf(((bsubs)/small_r) * (temp)));
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);  // Draw a line from the center of the small circle to the point on the curve
    glBegin(GL_LINES);
    glVertex2f((bsubs) * cosf(temp), (bsubs) * sinf(temp));
    glVertex2f((bsubs) * cosf(temp) + small_r*cosf(((bsubs)/small_r) * (temp)),
               (bsubs) * sinf(temp) - small_r*sinf(((bsubs)/small_r) * (temp)));
    glEnd();

    if (animationToggle != 0) {
      glColor3f(0.0f, 1.0f, 0.0f);
    } else {
      glColor3f(1.0f, 0.0f, 0.0f);
    }
    // Draw the big circle
    glBegin(GL_LINE_STRIP); // GL_POINTS, GL_QUADS, GL_LINES
    drawCircle(0.0f, 0.0f, big_r);
    glEnd();

    // Draw the small circle
    glBegin(GL_LINE_STRIP);
    drawCircle((bsubs) * cos(temp), (bsubs) * sin(temp), small_r);
    glEnd();
  }

  // Draw the hypocycloid
  glColor3f(0.5f, 0.5f, 1.0f);
  glBegin(GL_LINE_STRIP);
  drawHypocycloid(small_r, big_r, cycles, t);
  glEnd();
}

void keyboard(GLFWwindow *sender, int key, int scancode, int action, int mods) {
  if(key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    rotate += 3.0f;
    cout << "rotation: " << rotate << endl;
  }
  if(key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    rotate -= 3.0f;
    cout << "rotation: " << rotate << endl;
  }
  if(key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    switch(parameter) {
      case 0:
        big_r += 0.01f;
        cout << "big radius: " << big_r << endl;
        break;
      case 1:
        small_r += 0.01f;
        cout << "small radius: " << small_r << endl;
        break;
      case 2:
        cycles += 1;
        cout << "cycles: " << cycles << endl;
        break;
    }
  }
  if(key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    switch(parameter) {
      case 0:
        big_r -= 0.01f;
        if (big_r < 0.0f) {
          big_r = 0.0f;
        }
        cout << "big radius: " << big_r << endl;
        break;
      case 1:
        small_r -= 0.01f;
        if (small_r < 0.0f) {
          small_r = 0.0f;
        }
        cout << "small radius: " << small_r << endl;
        break;
      case 2:
        cycles -= 1;
        if (cycles < 1) {
          cycles = 1;
        }
        cout << "cycles: " << cycles << endl;
        break;
    }
  }
  if(key == GLFW_KEY_1 && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    parameter = BIGR;
    glfwSetWindowTitle(window, "Modifying big radius - R");
  }
  if(key == GLFW_KEY_2 && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    parameter = SMALLR;
    glfwSetWindowTitle(window, "Modifying small radius - r");
  }
  if(key == GLFW_KEY_3 && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    parameter = CYCLES;
    glfwSetWindowTitle(window, "Modifying number of cycles - n");
  }
  if(key == GLFW_KEY_R && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    if (animationToggle != 0) {
      t = 0.0f;
    } else {
      t = 1.0f;
      cycles = 1;
    }
    cout << "Reset" << endl;
  }
  if(key == GLFW_KEY_SPACE && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    if (animationToggle != 0) {
      dt = 0.0f;
      animationToggle = 0;
    } else {
      dt = 0.003f;
      cycles = 1;
      animationToggle = 1;
      circleToggle = 1;
    }
  }
  if(key == GLFW_KEY_F && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    if (circleToggle != 0) {
      circleToggle = 0;
    } else {
      circleToggle = 1;
    }
  }
}

void scroll(GLFWwindow* window, double xoffset, double yoffset) {
  if (yoffset > 0.0f) {
    scale += 0.05f;
    cout << "scale: " << scale << endl;
  } else if (yoffset < 0.0f) {
    scale -= 0.05f;
    cout << "scale: " << scale << endl;
  }
}

int main() {
  if(!glfwInit()) {
    return 1;
  }

  window = glfwCreateWindow(WINDOWX, WINDOWY, "CPSC589 - Assignment 1", NULL, NULL);  // width, height, window title
  if(!window) { // check if window was created properly
    return 1;
  }

  glfwMakeContextCurrent(window); // tell OpenGL what window we're working with
  glfwSetKeyCallback(window, keyboard);
  glfwSetScrollCallback(window, scroll);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  
  while(!glfwWindowShouldClose(window)) {
    glfwGetFramebufferSize(window, &w, &h); // get attributes of the window size and store into w and h
    glViewport(0, 0, w, h); // pixel coordinate of top left --> tell where to draw
    t += dt;
    render();

    glfwSwapBuffers(window);  // double buffer -> work on 1 buffer, display the other
    glfwPollEvents(); // detect input events --> mouse, keyboard. this detects when the mouse clicks the X in the top right
  }

  glfwDestroyWindow(window);  // destroy the window
  glfwTerminate();
  return 0;
}
