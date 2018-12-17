#pragma once
#include <float.h>
#define SSE_CALLCONV XM_CALLCONV

inline bool Zero(float fValue)
{
	return  fabsf(fValue) < FLT_EPSILON;
}

inline int Less(float fA, float fB)
{
	return fA - fB < -FLT_EPSILON;
}

inline void Clamp(float lowest, float * n, float highest)
{
	*n = max(lowest, min(*n, highest));
}

using SSE_VECTOR = DirectX::XMVECTOR;
using SSE_VECTOR_PARAM1 = DirectX::FXMVECTOR;
using SSE_VECTOR_PARAM2 = DirectX::GXMVECTOR;
using SSE_VECTOR_PARAM3 = DirectX::HXMVECTOR;
using SSE_VECTOR_PARAM4 = DirectX::CXMVECTOR;

using FLOAT2 = DirectX::XMFLOAT2;
using FLOAT3 = DirectX::XMFLOAT3;
using FLOAT4 = DirectX::XMFLOAT4;

using UINT2 = DirectX::XMUINT2;

/* Setters & Getters */

inline float SSE_CALLCONV GetX(SSE_VECTOR_PARAM1 v)
{
	return DirectX::XMVectorGetX(v);
}

inline float SSE_CALLCONV GetY(SSE_VECTOR_PARAM1 v)
{
	return DirectX::XMVectorGetY(v);
}

inline float SSE_CALLCONV GetZ(SSE_VECTOR_PARAM1 v)
{
	return DirectX::XMVectorGetZ(v);
}

inline float SSE_CALLCONV GetW(SSE_VECTOR_PARAM1 v)
{
	return DirectX::XMVectorGetW(v);
}

inline void SSE_CALLCONV SetX(SSE_VECTOR* out, float value)
{
	*out = DirectX::XMVectorSetX(*out, value);
}

inline void SSE_CALLCONV SetY(SSE_VECTOR* out, float value)
{
	*out = DirectX::XMVectorSetY(*out, value);
}

inline void SSE_CALLCONV SetZ(SSE_VECTOR* out, float value)
{
	*out = DirectX::XMVectorSetZ(*out, value);
}

inline void SSE_CALLCONV SetW(SSE_VECTOR* out, float value)
{
	*out = DirectX::XMVectorSetW(*out, value);
}

inline STD ostream& SSE_CALLCONV operator<<(STD ostream& out, SSE_VECTOR_PARAM1 v)
{
	out << "(" << GetX(v) << ", " << GetY(v) << ", " << GetZ(v) << ", " << GetW(v) << ")";
	return out;
}

/* BOOLEAN, DEF VECTORS */

inline SSE_VECTOR SSE_CALLCONV VectorZero()
{
	return DirectX::XMVectorZero();
}

inline SSE_VECTOR SSE_CALLCONV VectorOne()
{
	return DirectX::XMVectorSplatOne();
}

inline SSE_VECTOR SSE_CALLCONV Abs(SSE_VECTOR_PARAM1 v)
{
	return DirectX::XMVectorAbs(v);
}

inline SSE_VECTOR SSE_CALLCONV ShiftLeft(SSE_VECTOR_PARAM1 v, int Amount)
{
	return DirectX::XMVectorShiftLeft(v, v, Amount);
}

inline SSE_VECTOR SSE_CALLCONV Swizzle(SSE_VECTOR_PARAM1 v, uint32_t x, uint32_t y, uint32_t z, uint32_t w)
{
	return DirectX::XMVectorSwizzle(v, x, y, z, w);
}

inline bool SSE_CALLCONV AnyTrue(SSE_VECTOR_PARAM1 v)
	{
		uint32_t out;
		DirectX::XMVectorEqualIntR(&out, Swizzle(v, 0, 1, 2, 0), DirectX::XMVectorTrueInt());
		return DirectX::XMComparisonAnyTrue(out);
	}

inline SSE_VECTOR SSE_CALLCONV Or(SSE_VECTOR_PARAM1 v1, SSE_VECTOR_PARAM1 v2)
	{
		return DirectX::XMVectorOrInt(v1, v2);
	}

inline SSE_VECTOR SSE_CALLCONV Greater(SSE_VECTOR_PARAM1 v1, SSE_VECTOR_PARAM1 v2)
{
	return DirectX::XMVectorGreater(v1, v2);
}

inline SSE_VECTOR SSE_CALLCONV GreaterOrEqual(SSE_VECTOR_PARAM1 v1, SSE_VECTOR_PARAM1 v2)
{
	return DirectX::XMVectorGreaterOrEqual(v1, v2);
}

inline SSE_VECTOR SSE_CALLCONV Less(SSE_VECTOR_PARAM1 v1, SSE_VECTOR_PARAM1 v2)
{
	return DirectX::XMVectorLess(v1, v2);
}

/* OPERATORS */

inline SSE_VECTOR SSE_CALLCONV Scale(SSE_VECTOR_PARAM1 v, float scale)
{
	return DirectX::XMVectorScale(v, scale);
}

inline SSE_VECTOR SSE_CALLCONV Add(SSE_VECTOR_PARAM1 v1, SSE_VECTOR_PARAM1 v2)
{
	return DirectX::XMVectorAdd(v1, v2);
}

inline SSE_VECTOR SSE_CALLCONV Subtract(SSE_VECTOR_PARAM1 v1, SSE_VECTOR_PARAM1 v2)
{
	return DirectX::XMVectorSubtract(v1, v2);
}

inline SSE_VECTOR SSE_CALLCONV Multiply(SSE_VECTOR_PARAM1 v1, SSE_VECTOR_PARAM1 v2)
{
	return DirectX::XMVectorMultiply(v1, v2);
}

inline SSE_VECTOR SSE_CALLCONV Evaluate(SSE_VECTOR_PARAM1 v1) //Checks if each component is NOT A Zero
{
	return DirectX::XMVectorClamp
	(
		DirectX::XMVectorNotEqual(v1, DirectX::XMVectorZero()), 
		DirectX::XMVectorZero(), 
		DirectX::XMVectorSplatOne()
	);
}

inline float SSE_CALLCONV MagnitudeSq2(SSE_VECTOR_PARAM1 v)
{
	return DirectX::XMVectorGetX(DirectX::XMVector2LengthSq(v));
}

inline float SSE_CALLCONV MagnitudeSq3(SSE_VECTOR_PARAM1 v)
{
	return DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(v));
}

inline float SSE_CALLCONV MagnitudeSq4(SSE_VECTOR_PARAM1 v)
{
	return DirectX::XMVectorGetX(DirectX::XMVector4LengthSq(v));
}

inline float SSE_CALLCONV Magnitude2(SSE_VECTOR_PARAM1 v)
{
	return DirectX::XMVectorGetX(DirectX::XMVector2Length(v));
}

inline float SSE_CALLCONV Magnitude3(SSE_VECTOR_PARAM1 v)
{
	return DirectX::XMVectorGetX(DirectX::XMVector3Length(v));
}

inline float SSE_CALLCONV Magnitude4(SSE_VECTOR_PARAM1 v)
{
	return DirectX::XMVectorGetX(DirectX::XMVector4Length(v));
}

inline SSE_VECTOR SSE_CALLCONV Normalize2(SSE_VECTOR_PARAM1 v)
{
	return DirectX::XMVector2Normalize(v);
}

inline SSE_VECTOR SSE_CALLCONV Normalize3(SSE_VECTOR_PARAM1 v)
{
	return DirectX::XMVector3Normalize(v);
}

inline FLOAT2 SSE_CALLCONV StoreFloat2(SSE_VECTOR_PARAM1 v)
{
	FLOAT2 Storage;
	DirectX::XMStoreFloat2(&Storage, v);
	return Storage;
}

inline UINT2 SSE_CALLCONV StoreUint2(SSE_VECTOR_PARAM1 v)
{
	UINT2 Storage;
	DirectX::XMStoreUInt2(&Storage, v);
	return Storage;
}

inline FLOAT3 SSE_CALLCONV StoreFloat3(SSE_VECTOR_PARAM1 v)
{
	FLOAT3 Storage;
	DirectX::XMStoreFloat3(&Storage, v);
	return Storage;
}

inline FLOAT4 SSE_CALLCONV StoreFloat4(SSE_VECTOR_PARAM1 v)
{
	FLOAT4 Storage;
	DirectX::XMStoreFloat4(&Storage, v);
	return Storage;
}

inline SSE_VECTOR SSE_CALLCONV LoadFloat2(const FLOAT2& v)
{
	return DirectX::XMLoadFloat2(&v);
}

inline SSE_VECTOR SSE_CALLCONV LoadUint2(const UINT2& v)
{
	return DirectX::XMLoadUInt2(&v);
}

inline SSE_VECTOR SSE_CALLCONV LoadFloat3(const FLOAT3& v)
{
	return DirectX::XMLoadFloat3(&v);
}

inline SSE_VECTOR SSE_CALLCONV LoadFloat4(const FLOAT4& v)
{
	return DirectX::XMLoadFloat4(&v);
}

inline float SSE_CALLCONV Dot2(SSE_VECTOR_PARAM1 v1, SSE_VECTOR_PARAM1 v2)
{
	return DirectX::XMVectorGetX(DirectX::XMVector2Dot(v1, v2));
}

inline float SSE_CALLCONV Dot3(SSE_VECTOR_PARAM1 v1, SSE_VECTOR_PARAM1 v2)
{
	return DirectX::XMVectorGetX(DirectX::XMVector3Dot(v1, v2));
}

inline SSE_VECTOR SSE_CALLCONV Cross2(SSE_VECTOR_PARAM1 v1, SSE_VECTOR_PARAM1 v2)
{
	return DirectX::XMVector2Cross(v1, v2);
}

inline SSE_VECTOR SSE_CALLCONV Cross3(SSE_VECTOR_PARAM1 v1, SSE_VECTOR_PARAM1 v2)
{
	return DirectX::XMVector3Cross(v1, v2);
}

inline float SSE_CALLCONV Distance2(SSE_VECTOR_PARAM1 v1, SSE_VECTOR_PARAM1 v2)
{
	return DirectX::XMVectorGetX(DirectX::XMVector2Length(DirectX::XMVectorSubtract(v2, v1)));
}

inline float SSE_CALLCONV Distance3(SSE_VECTOR_PARAM1 v1, SSE_VECTOR_PARAM1 v2)
{
	return DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(v2, v1)));
}

inline float SSE_CALLCONV DistanceSq2(SSE_VECTOR_PARAM1 v1, SSE_VECTOR_PARAM1 v2)
{
	return DirectX::XMVectorGetX(DirectX::XMVector2LengthSq(DirectX::XMVectorSubtract(v2, v1)));
}

inline float SSE_CALLCONV DistanceSq3(SSE_VECTOR_PARAM1 v1, SSE_VECTOR_PARAM1 v2)
{
	return DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(DirectX::XMVectorSubtract(v2, v1)));
}

inline bool SSE_CALLCONV AnyZero3(SSE_VECTOR_PARAM1 v)
{
	FLOAT3 Storage = StoreFloat3(v);
	return Zero(Storage.x*Storage.y*Storage.z);
}


