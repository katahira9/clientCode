#pragma once
#include "Arithmetic.h"
#include <Windows.h>

class CameraControl
{
private:
	FLOAT3 pos;
	FLOAT3 target;
	FLOAT3 up_vector;

	float fov;
	float zfar;
	float znear;
	float width;
	float height;

	POINT l_fulcrum;
	POINT r_fulcrum;
	POINT c_fulcrum;
	FLOAT3 attitude;
public:
	CameraControl();

	FLOAT3* getPos()	{ return &pos; }
	FLOAT3* getTarget()	{ return &target; }
	FLOAT3* getUp()		{ return &up_vector; }

	float* getFov()		{ return &fov; }
	float* getFar()		{ return &zfar; }
	float* getNear()	{ return &znear; }
	float* getWidth()	{ return &width; }
	float* getHeight()	{ return &height; }

	FLOAT4X4 getView()const;
	FLOAT4X4 getProjection()const;
	FLOAT4X4 getOrthographic()const;

	/// <summary>
	/// �J�����̍X�V�����܂��B
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	/// <param name="move_speed">�ړ����x</param>
	/// <param name="rotation_speed">�p���x</param>
	void update(float elapsed_time, 
		float move_speed = 32.0f, 
		float rotation_speed = (__pi_ * 2.0f));

};