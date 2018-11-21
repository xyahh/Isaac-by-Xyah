#pragma once
#define DX		DirectX::
#define DX2		DX V2::
#define DX3		DX V3::
#define DX4		DX V4::


inline bool Zero(float fValue)
{
	return  fabsf(fValue) < FLT_EPSILON;
}

inline void Clamp(float lowest, float * n, float highest)
{
	*n = max(lowest, min(*n, highest));
}


namespace DirectX
{
	inline XMVECTOR XM_CALLCONV Swizzle(FXMVECTOR v, uint32_t x, uint32_t y, uint32_t z, uint32_t w)
	{
		return XMVectorSwizzle(v, x, y, z, w);
	}

	inline bool XM_CALLCONV AnyTrue(FXMVECTOR v)
	{
		uint32_t out;
		XMVectorEqualIntR(&out, Swizzle(v, 0, 1, 2, 0), XMVectorTrueInt());
		return XMComparisonAnyTrue(out);
	}

	inline XMVECTOR XM_CALLCONV Or(FXMVECTOR v1, FXMVECTOR v2)
	{
		return XMVectorOrInt(v1, v2);
	}

	inline XMVECTOR XM_CALLCONV Greater(FXMVECTOR v1, FXMVECTOR v2)
	{
		return XMVectorGreater(v1, v2);
	}

	inline float XM_CALLCONV GetX(FXMVECTOR v)
	{
		return XMVectorGetX(v);
	}

	inline float XM_CALLCONV GetY(FXMVECTOR v)
	{
		return XMVectorGetY(v);
	}

	inline float XM_CALLCONV GetZ(FXMVECTOR v)
	{
		return XMVectorGetZ(v);
	}

	inline float XM_CALLCONV GetW(FXMVECTOR v)
	{
		return XMVectorGetW(v);
	}

	inline XMVECTOR XM_CALLCONV Evaluate(FXMVECTOR v1)
	{
		return {
		  (GetX(v1)) ? 1.f : 0.f,
		  (GetY(v1)) ? 1.f : 0.f,
		  (GetZ(v1)) ? 1.f : 0.f,
		  (GetW(v1)) ? 1.f : 0.f, };
	}

	inline void XM_CALLCONV Print(FXMVECTOR v, char end = ' ')
	{
		printf("(%f, %f, %f, %f) %c", GetX(v), GetY(v), GetZ(v), GetW(v), end);
	}

	inline void XM_CALLCONV SetX(XMVECTOR* out, float value)
	{
		*out = XMVectorSetX(*out, value);
	}

	inline void XM_CALLCONV SetY(XMVECTOR* out, float value)
	{
		*out = XMVectorSetY(*out, value);
	}

	inline void XM_CALLCONV SetZ(XMVECTOR* out, float value)
	{
		*out = XMVectorSetZ(*out, value);
	}

	inline void XM_CALLCONV SetW(XMVECTOR* out, float value)
	{
		*out = XMVectorSetW(*out, value);
	}

	inline XMVECTOR XM_CALLCONV Scale(FXMVECTOR v, float scale)
	{
		return XMVectorScale(v, scale);
	}

	inline void XM_CALLCONV Scale(XMVECTOR* out, float scale)
	{
		*out = XMVectorScale(*out, scale);
	}

	inline XMVECTOR XM_CALLCONV Add(FXMVECTOR v1, FXMVECTOR v2)
	{
		return XMVectorAdd(v1, v2);
	}

	inline void XM_CALLCONV Add(XMVECTOR* out, FXMVECTOR v)
	{
		*out = XMVectorAdd(*out, v);
	}


	inline XMVECTOR XM_CALLCONV Subtract(FXMVECTOR v1, FXMVECTOR v2)
	{
		return XMVectorSubtract(v1, v2);
	}

	inline XMVECTOR XM_CALLCONV Multiply(FXMVECTOR v1, FXMVECTOR v2)
	{
		return XMVectorMultiply(v1, v2);
	}

	namespace V2
	{
		inline XMFLOAT2 XM_CALLCONV Store(FXMVECTOR v)
		{
			XMFLOAT2 Storage;
			XMStoreFloat2(&Storage, v);
			return Storage;
		}

		inline XMVECTOR XM_CALLCONV Load(const XMFLOAT2& v)
		{
			return XMLoadFloat2(&v);
		}

		inline float XM_CALLCONV Dot(FXMVECTOR v1, FXMVECTOR v2)
		{
			return XMVectorGetX(XMVector2Dot(v1, v2));
		}

		inline XMVECTOR XM_CALLCONV Cross(FXMVECTOR v1, FXMVECTOR v2)
		{
			return XMVector2Cross(v1, v2);
		}

		inline XMVECTOR XM_CALLCONV Normalize(FXMVECTOR v)
		{
			return XMVector2Normalize(v);
		}

		inline float XM_CALLCONV Magnitude(FXMVECTOR v)
		{
			return XMVectorGetX(XMVector2Length(v));
		}

		inline float XM_CALLCONV MagnitudeSQ(FXMVECTOR v)
		{
			return XMVectorGetX(XMVector2LengthSq(v));
		}

		inline float XM_CALLCONV Distance(FXMVECTOR v1, FXMVECTOR v2)
		{
			return XMVectorGetX(XMVector2Length(XMVectorSubtract(v2, v1)));
		}

		inline float XM_CALLCONV DistanceSQ(FXMVECTOR v1, FXMVECTOR v2)
		{
			return XMVectorGetX(XMVector2LengthSq(XMVectorSubtract(v2, v1)));
		}
	}

	namespace V3
	{
		inline XMFLOAT3 XM_CALLCONV Store(FXMVECTOR v)
		{
			XMFLOAT3 Storage;
			XMStoreFloat3(&Storage, v);
			return Storage;
		}

		inline bool XM_CALLCONV AnyZero(FXMVECTOR v)
		{
			XMFLOAT3 Storage = Store(v);
			return Storage.x*Storage.y*Storage.z == 0.f; //return False if there 
		}

		inline XMVECTOR XM_CALLCONV Load(const XMFLOAT3& v)
		{
			return XMLoadFloat3(&v);
		}

		inline float XM_CALLCONV Dot(FXMVECTOR v1, FXMVECTOR v2)
		{
			return XMVectorGetX(XMVector3Dot(v1, v2));
		}

		inline XMVECTOR XM_CALLCONV Cross(FXMVECTOR v1, FXMVECTOR v2)
		{
			return XMVector3Cross(v1, v2);
		}

		inline XMVECTOR XM_CALLCONV Normalize(FXMVECTOR v)
		{
			return XMVector3Normalize(v);
		}

		inline float XM_CALLCONV Magnitude(FXMVECTOR v)
		{
			return XMVectorGetX(XMVector3Length(v));
		}

		inline float XM_CALLCONV MagnitudeSQ(FXMVECTOR v)
		{
			return XMVectorGetX(XMVector3LengthSq(v));
		}

		inline float XM_CALLCONV Distance(FXMVECTOR v1, FXMVECTOR v2)
		{
			return XMVectorGetX(XMVector3Length(XMVectorSubtract(v2, v1)));
		}

		inline float XM_CALLCONV DistanceSQ(FXMVECTOR v1, FXMVECTOR v2)
		{
			return XMVectorGetX(XMVector3LengthSq(XMVectorSubtract(v2, v1)));
		}
	}

	namespace V4
	{
		inline XMFLOAT4 XM_CALLCONV Store(FXMVECTOR v)
		{
			XMFLOAT4 Storage;
			XMStoreFloat4(&Storage, v);
			return Storage;
		}

		inline XMVECTOR XM_CALLCONV LoadInt(const XMINT4& v)
		{
			return XMLoadSInt4(&v);
		}

		inline XMVECTOR XM_CALLCONV Load(const XMFLOAT4& v)
		{
			return XMLoadFloat4(&v);
		}
	}
}

