#ifndef CAMERA2D
#define CAMERA2D
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ztime.h"
const float SHAKE_UPDATE_INTERVAL = 0.1f;
//Camera class for 2D games
class Camera2D
{
public:
    Camera2D();
    Camera2D(int screenWidth, int screenHeight);
    Camera2D(int screenWidth, int screenHeight, const glm::vec2 &position, float scale);
    ~Camera2D();

    //sets up the orthographic matrix and screen dimensions
    void init(int screenWidth, int screenHeight);

    //updates the camera matrix if needed
    void update();

    glm::vec2 convertScreenToWorld(glm::vec2 screenCoords)const;

    bool isBoxInView(const glm::vec2& position, const glm::vec2& dimensions)const;
    void shake(float intensity = 1.0f, float time = 1.0f);
    void offsetPosition(const glm::vec2& offset);
    void offsetScale(float offset) { _scale += offset; if (_scale < 0.001f) _scale = 0.001f; _needsMatrixUpdate = true; }

    //setters
    void setActualyPosition(const glm::vec2& actualPosition);
    void setPosition(const glm::vec2& newPosition);
    void lerp(const glm::vec2& position, float lerpValue, float deltaTime);
    void setScale(float newScale) { _scale = newScale; _needsMatrixUpdate = true; }
    void zoom(float zoom) { _scale = _scale + zoom; _needsMatrixUpdate = true; }
    void setBorders(const glm::vec4 &borders);
    void disableBorders() { m_bordred = false; }

    //getters
    inline const glm::vec2& getPosition() const { return _position; }
    inline float getScale() const { return _scale; }
    inline const glm::mat4& getCameraMatrix() const { return _cameraMatrix; }
    inline float getAspectRatio() const { return (float)_screenWidth / (float)_screenHeight; }
    inline glm::vec2 getScreenDimentions() const { return glm::vec2(_screenWidth, _screenHeight); }

private:
    void updateBorders();

    int _screenWidth, _screenHeight;
    bool _needsMatrixUpdate = true;
    float _scale = 1.0f;
    glm::vec2 _position;
    glm::mat4 _cameraMatrix;
    glm::mat4 _orthoMatrix;
    bool m_bordred = false;
    glm::vec4 m_borders;
    bool m_shaking = false;
    Timer m_timer;
    Timer m_shakeTimer;
    float m_shakeIntensity = 1.0f;
    float m_shakeTime = 1.0f;
    glm::vec2 m_shakingPosition;
    glm::vec2 m_actualPosition;
};

#endif // CAMERA2D

