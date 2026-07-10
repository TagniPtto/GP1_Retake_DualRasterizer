

#include "Math/Matrix.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

#include "Timer.h"
#include "math/Math.h"
#include "math/Matrix.h"

#include <SDL_keyboard.h>
#include <SDL_mouse.h>
//#include <cstdint>


class Camera {

public:
	Camera(float aspectRatio , dae::Vector3 _location = { 0, 0, -50}, float _yaw = {}, float _pitch = {}, float _fovAngle = 45);

	dae::Matrix GetViewMatrix();
	dae::Matrix GetProjectionMatrix();
	dae::Vector3 GetPosition();
	int GetSamplingTechniqueIndex();


	void Update(const dae::Timer* ptimer);
private:

	dae::Vector3 location{0,0,-0.f};
	dae::Vector3 forward{ 0.f ,0.f, 1.f};
	dae::Vector3 right{ 1.f ,0.f, 0.f };
	dae::Vector3 up{ 0.f, 1.f , 0.f};

	//dae::Matrix invViewMatrix{};
	//dae::Matrix viewMatrix{};

	//dae::Matrix projectionMatrix{};


	float totalPitch{};
	float totalYaw{};
	float totalRoll{};
	float fovAngle{90};
	float fov{};
	float aspectRatio;

	float nearPlane{1};
	float farPlane{100};

	float movementSpeed{20.0f};
	float rotationSensitivity{1.0f};

	int currentTechniqueIndex{};

	//software rasterizer
public:
	enum class RenderingMode {
		Diffuse,
		ObservedArea,
		Specular,
		Combined
	};
	RenderingMode renderingmode;
};