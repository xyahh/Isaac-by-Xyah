#pragma once
class BasicCollision
{
public:
	BasicCollision() {}
	virtual ~BasicCollision() {}

	virtual void OnCollision(Physics& MyBody, Physics& CollidingBody);

private:
};

class ActorCollision : public BasicCollision
{
public:
	ActorCollision() {}
	virtual ~ActorCollision() {}

	virtual void OnCollision(Physics& MyBody, Physics& CollidingBody);

};

class VentCollision : public BasicCollision
{
public:
	VentCollision() {}
	virtual ~VentCollision() {}

	virtual void OnCollision(Physics& MyBody, Physics& CollidingBody);
};

class BulletCollision : public BasicCollision
{
public:
	BulletCollision() {}
	virtual	~BulletCollision() {}

	virtual void OnCollision(Physics& MyBody, Physics& CollidingBody);
};



namespace Collision
{
	class Box
	{
	public:
		Box() :
			Size(0.f, 0.f, 0.f) {}

		void XM_CALLCONV SetDimensions(DX FXMVECTOR v);
		DX XMVECTOR XM_CALLCONV GetDimensions() const;

		friend void XM_CALLCONV GetMinMax(DX XMVECTOR* Min, DX XMVECTOR* Max, DX FXMVECTOR position, const Box& box);
		friend DX XMVECTOR XM_CALLCONV HandleCollision(DX FXMVECTOR A_Pos, const Box& A_Box,
			DX FXMVECTOR B_Pos, const Box& B_Box);

		DX XMVECTOR XM_CALLCONV ConvertPosition(DX FXMVECTOR Position) const;

	private:
		DX XMFLOAT3	Size;
	};

	inline void XM_CALLCONV GetMinMax(DX XMVECTOR* Min, DX XMVECTOR* Max, DX FXMVECTOR position, const Box& box)
	{
		DX XMVECTOR Half = DX Scale({ box.Size.x, box.Size.y }, 0.5f);
		*Max = DX Add(position, Half);
		*Min = DX Subtract(position, Half);
		DX Add(Max, { 0.f, 0.f, box.Size.z });
	}

	inline DX XMVECTOR XM_CALLCONV HandleCollision(DX FXMVECTOR A_Pos, const Box& A_Box,
		DX FXMVECTOR B_Pos, const Box& B_Box)
	{
		DX XMVECTOR A_Max, A_Min;
		DX XMVECTOR B_Max, B_Min;

		GetMinMax(&A_Min, &A_Max, A_Pos, A_Box);
		GetMinMax(&B_Min, &B_Max, B_Pos, B_Box);

		return DX Or(
			DX Greater(A_Min, B_Max),
			DX Greater(B_Min, A_Max));
	}

	static BasicCollision	Basic;
	static ActorCollision	Actor;
	static VentCollision	Vent;
	static BulletCollision	Bullet;

}
