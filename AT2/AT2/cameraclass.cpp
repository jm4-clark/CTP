#include "cameraclass.h"

CameraClass::CameraClass()
{
	m_posX = 0.0f;
	m_posY = 0.0f;
	m_posZ = 0.0f;

	m_rotX = 0.0f;
	m_rotY = 0.0f;
	m_rotZ = 0.0f;

	DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_posX = x;
	m_posY = y;
	m_posZ = z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotX = x;
	m_rotY = y;
	m_rotZ = z;
}

XMFLOAT3 CameraClass::GetPosition()
{
	return XMFLOAT3(m_posX, m_posY, m_posZ);
}

XMFLOAT3 CameraClass::GetRotation()
{
	return XMFLOAT3(m_rotX, m_rotY, m_rotZ);
}

void CameraClass::Render()
{
	XMFLOAT3 up, pos, lookAt;
	XMVECTOR upVector, posVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotMatrix;

	//set up vector that points up
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	//load it into XMVECTOR structure
	upVector = XMLoadFloat3(&up);

	//set up position of camera in the world
	pos.x = m_posX;
	pos.y = m_posY;
	pos.z = m_posZ;

	//load into vector
	posVector = XMLoadFloat3(&pos);

	//set up where camera is looking by default
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	//load into vector
	lookAtVector = XMLoadFloat3(&lookAt);

	//set the yaw, pitch & roll in radians
	pitch = m_rotX * 0.0174532925f;
	yaw = m_rotY * 0.0174532925f;
	roll = m_rotZ * 0.0174532925f;

	//create the rotation matrix from yaw, pitch & roll
	rotMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	//transform the lookat & up vector by the rotation matrix so the view is correctly rotated at the origin
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotMatrix);
	upVector = XMVector3Transform(upVector, rotMatrix);

	//translate the rotated cam position to the location of the viewer
	lookAtVector = XMVectorAdd(posVector, lookAtVector);

	//create view matrix
	m_viewMatrix = XMMatrixLookAtLH(posVector, lookAtVector, upVector);
}

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}

//void CameraClass::UpdateCamera()
//{
//	camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
//	camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
//	camTarget = XMVector3Normalize(camTarget);
//
//	XMMATRIX RotateYTempMatrix;
//	RotateYTempMatrix = XMMatrixRotationY(camYaw);
//
//	camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
//	camUp = XMVector3TransformCoord(camUp, RotateYTempMatrix);
//	camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);
//
//	camPosition += moveLeftRight*camRight;
//	camPosition += moveBackForward*camForward;
//
//	moveLeftRight = 0.0f;
//	moveBackForward = 0.0f;
//
//	camTarget = camPosition + camTarget;
//
//	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);
//}