#include <stdlib.h>
#include <stdio.h>
#include "window.h"
#include "inputmanager.h"
#include "iomanager.h"
#include "camera2d.h"
#include <glm/glm.hpp>
#include "resourcemanager.h"
#include "batchrenderer.h"
#include "zgui.h"
#include "PushButton.h"
#include "Slider.h"
#include "ZWindow.h"


#ifdef EMSCRIPTEN
#include <emscripten/emscripten.h>
#include <GLES3/gl3.h>
#define SIMPLE_VERTEX_SHADER_PATH "Shaders/textureShading_es.vert"
#define SIMPLE_FRAGMENT_SHADER_PATH "Shaders/textureShading_es.frag"
#else
#include <GL/glew.h>
#define SIMPLE_VERTEX_SHADER_PATH "Shaders/textureShading.vert"
#define SIMPLE_FRAGMENT_SHADER_PATH "Shaders/textureShading.frag"
#endif
using namespace std;
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

static InputManager inputManager;
static Window window;
static GLSLProgram program;
static SpriteBatch batch;
static SpriteFont* spriteFont;
static DebugRenderer debugRenderer;
static Camera2D camera;
static BatchRenderer batchrenderer;
static ZGUI gui;

Slider *Rslider;
Slider *Gslider;
Slider *Bslider;

void mainLoopFn()
{
    //inputManager.update();

      SDL_Event evnt;
      while(SDL_PollEvent(&evnt)){
       switch (evnt.type) {
          case SDL_QUIT:
              exit(0);
              break;
          case SDL_MOUSEMOTION:
              inputManager.setMouseCoords((float)evnt.motion.x, (float)window.getHeight() - (float)evnt.motion.y);
              break;
          case SDL_KEYDOWN:
              inputManager.pressKey(evnt.key.keysym.sym);
              break;
          case SDL_KEYUP:
              inputManager.releaseKey(evnt.key.keysym.sym);
              break;
          case SDL_MOUSEBUTTONDOWN:
              inputManager.pressKey(evnt.button.button);
              break;
          case SDL_MOUSEBUTTONUP:
              inputManager.releaseKey(evnt.button.button);
              break;

        }
        gui.onSDLEvent(evnt);
      }
      camera.update();



      static float r = 0.f, g = 0.f, b = 1.f, a = 1.f;
      if (inputManager.isKeyDown(SDLK_e)){
          r = 1.0f;
      } else r = 0.f;
      static const GLTexture* image = ResourceManager::getSingelton().getTexture("Resources/rock.png", true);

      glViewport(0, 0, window.getWidth(), window.getHeight());
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glClearColor(Rslider->getCurrentValue(), Gslider->getCurrentValue(), Bslider->getCurrentValue(), a);


      program.use();

      GLint textureUniform = program.getUniformLocation("mySampler");
      glUniform1i(textureUniform, 0);
      glActiveTexture(GL_TEXTURE0);

      glm::mat4 projectionMatrix = camera.getCameraMatrix();
      GLuint pUniform = program.getUniformLocation("P");
      glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

      batchrenderer.begin();
      batchrenderer.draw(glm::vec4(25, 200, 200, 200), glm::vec4(0, 0, 1, 1), image->id, 0, &program, WhiteRGBA8);
      batchrenderer.render(projectionMatrix);

     /* batch.begin();
      batch.draw(glm::vec4(0, 0, 400, 400), glm::vec4(0, 0, 1, 1), image->id, 0.f, White);
      spriteFont->draw(batch, "m_renderedText", glm::vec2(400, 200), glm::vec2(1.f), 0.f, Justification::LEFT, 255);
      batch.end();
      batch.renderBatch();
      program.unuse();

      debugRenderer.drawLine(glm::vec2(0, 0), glm::vec2(300, 300), Red);
      debugRenderer.drawLine(glm::vec2(0, 0), glm::vec2(1, 300), Red);
      debugRenderer.drawLine(glm::vec2(0, 0), glm::vec2(300, 300), Red);
      debugRenderer.end();
      debugRenderer.render(projectionMatrix, 2);
*/

      gui.update(inputManager);
      gui.draw();

      window.swapBuffer();

}

int main()
{    
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    // 3D
   /* glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);*/

    window.create("Qt Opengl", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    program.compileShaders(SIMPLE_VERTEX_SHADER_PATH, SIMPLE_FRAGMENT_SHADER_PATH);
    program.addAttribute("vertexPosition");
    program.addAttribute("vertexColor");
    program.addAttribute("vertexUV");
    program.linkShaders();

    batch.init();
    batchrenderer.init();
    spriteFont = new SpriteFont("Font/arial.ttf", 32);
    debugRenderer.init();
    camera.init(WINDOW_WIDTH, WINDOW_HEIGHT);

    ZGUI::load();
    gui.init(&window);
    UiResources::loadScheme("Scripts/scheme.json");
    UiResources::loadImageSets("Scripts/imageSets.json");
  /*  ZLabel* label = new ZLabel(glm::vec4(0.1, 0.1, 0.6, 0.5), gui.getRoot());
    label->setText("label is it true ?");*/

    const char *sliderTexturs[] = {"sWindow", "defaultButton", "defaultProgressBar"};
    SpriteFont *font = FontManager::get("Font/chintzy.ttf", 24);;

    ZWindow *window = new ZWindow(glm::vec4(0.55, 0.6, 0.4, 0.3), "sWindow", gui.getRoot());
    window->setTitle("Window : Colors");
    window->setStatic(false);

    Rslider = new Slider(glm::vec4(0.1, 0.5f, 0.55f, 0.2f), sliderTexturs, window);
    Rslider->setMinValue(0.f);
    Rslider->setMaxValue(1.0f);
    Rslider->setCurrentValue(0.8f);

    Gslider = new Slider(glm::vec4(0.1, 0.3f, 0.55f, 0.2f), sliderTexturs, window);
    Gslider->setMinValue(0.f);
    Gslider->setMaxValue(1.0f);

    Bslider = new Slider(glm::vec4(0.1, 0.1f, 0.55f, 0.2f), sliderTexturs, window);
    Bslider->setMinValue(0.f);
    Bslider->setMaxValue(1.0f);



#ifdef EMSCRIPTEN
    emscripten_set_main_loop(mainLoopFn, 60, 1);
#else
    while(1){
       mainLoopFn();      
       SDL_Delay(20);
    }
#endif



    return 0;
}

