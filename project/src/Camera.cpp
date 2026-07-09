#include "Camera.h"
#include <iostream>

Camera::Camera(float aspectRatio , dae::Vector3 _location, float _yaw, float _pitch, float _fovAngle):
	location(_location), totalYaw(_yaw), totalPitch(_pitch), fovAngle(_fovAngle), fov(tanf((fovAngle * 3.1415f / 180.f) / 2.f)),aspectRatio(aspectRatio)
{
}


dae::Matrix Camera::GetViewMatrix()
{
	return dae::Matrix::CreateLookAtLH(location,forward,up);
}

dae::Matrix Camera::GetProjectionMatrix()
{
	return  dae::Matrix::CreatePerspectiveFovLH(fov,aspectRatio,nearPlane,farPlane);
}


dae::Vector3 Camera::GetPosition()
{
	return location;
}

int Camera::GetSamplingTechniqueIndex()
{
	return currentTechniqueIndex;
}

void Camera::Update(const dae::Timer* ptimer)
{

	const uint8_t* keyBoardPtr = SDL_GetKeyboardState(nullptr);

	if (keyBoardPtr[SDL_SCANCODE_W] || keyBoardPtr[SDL_SCANCODE_UP]) {
		location += forward * movementSpeed * ptimer->GetElapsed();
	}
	if (keyBoardPtr[SDL_SCANCODE_S] || keyBoardPtr[SDL_SCANCODE_DOWN]) {
		location -= forward * movementSpeed * ptimer->GetElapsed();
	}
	
	if (keyBoardPtr[SDL_SCANCODE_A] || keyBoardPtr[SDL_SCANCODE_LEFT]) {
		location -= right * movementSpeed * ptimer->GetElapsed();
	}
	if (keyBoardPtr[SDL_SCANCODE_D] || keyBoardPtr[SDL_SCANCODE_RIGHT]) {
		location += right * movementSpeed * ptimer->GetElapsed();
	}
	if (keyBoardPtr[SDL_SCANCODE_LSHIFT])
	{
		movementSpeed += 5.0f * ptimer->GetElapsed();
	}
	if (keyBoardPtr[SDL_SCANCODE_F4]) {
		currentTechniqueIndex = (currentTechniqueIndex + 1)%3;
	}

	int x{}, y{};
	uint32_t mouseState = SDL_GetRelativeMouseState(&x,&y);

	if (mouseState == SDL_BUTTON_RMASK){
		totalYaw -= x * rotationSensitivity * .001; //* ptimer->GetElapsed();
		totalPitch += y * rotationSensitivity *.001; //* ptimer->GetElapsed();
		//std::cout << "Yaw: " << totalYaw << " Pitch: " << totalPitch << std::endl;
	}
	if (mouseState == SDL_BUTTON_LMASK) {

		if (y != 0) {
			float direction{ float(y) / std::abs(y) };
			location += forward * movementSpeed * direction * ptimer->GetElapsed();
		}
	}
	if (mouseState & SDL_BUTTON_LMASK && mouseState & SDL_BUTTON_RMASK) {
		if (y != 0) {
			float direction{ float(y) / std::abs(y) };
			location += up * movementSpeed * direction *ptimer->GetElapsed();
		}
	}
	
	const dae::Matrix rotMat{ dae::Matrix::CreateRotation(totalPitch, totalYaw, 0) };
	forward = rotMat.TransformVector(dae::Vector3::UnitZ).Normalized();
	right = rotMat.TransformVector(dae::Vector3::UnitX).Normalized();
	up = rotMat.TransformVector(dae::Vector3::UnitY).Normalized();


	static bool f5Pressed = false;
	if (keyBoardPtr[SDL_SCANCODE_F5]) {
		if (!f5Pressed) {
			const static std::string renderingModeString[4]{ std::string("Diffuse"),std::string("ObservedArea"),std::string("Specular"),std::string("Combined")};
			const static int renderingModesCount{ (int)RenderingMode::Combined };
			renderingmode = (RenderingMode)(((int)renderingmode + 1) % (renderingModesCount + 1));
			std::cout << "RenderingMode : " << renderingModeString[(int)renderingmode % renderingModeString->length()] << std::endl;
		}
		f5Pressed = true;
	}
	else {
		f5Pressed = false;
	}

}
