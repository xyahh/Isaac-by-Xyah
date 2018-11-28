#pragma once
#define NS_COLLISION_START namespace Collision {
#define NS_COLLISION_END }

class BasicCollision
{
public:
	BasicCollision() {}
	virtual ~BasicCollision() {}

	virtual void XM_CALLCONV OnCollision(Physics& MyBody, Physics& CollidingBody, DX FXMVECTOR CollisionNormal);

private:
};

class ActorCollision : public BasicCollision
{
public:
	ActorCollision() {}
	virtual ~ActorCollision() {}

	virtual void XM_CALLCONV OnCollision(Physics& MyBody, Physics& CollidingBody, DX FXMVECTOR CollisionNormal);

};
class BulletCollision : public BasicCollision
{
public:
	BulletCollision() {}
	virtual	~BulletCollision() {}

	virtual void XM_CALLCONV OnCollision(Physics& MyBody, Physics& CollidingBody, DX FXMVECTOR CollisionNormal);
};


NS_COLLISION_START

static BasicCollision	Basic;
static ActorCollision	Actor;
static BulletCollision	Bullet;

class Box
	{
	public:
		Box() :
			Size(0.f, 0.f, 0.f) {}

		void XM_CALLCONV SetDimensions(DX FXMVECTOR v);
		DX XMVECTOR XM_CALLCONV GetDimensions() const;

		friend void XM_CALLCONV GetExtents(DX XMVECTOR* Min, DX XMVECTOR* Max, DX FXMVECTOR position, const Box& box);
		DX XMVECTOR XM_CALLCONV ConvertPosition(DX FXMVECTOR Position) const;

	private:
		DX XMFLOAT3	Size;
	};

inline void XM_CALLCONV GetExtents(DX XMVECTOR* Min, DX XMVECTOR* Max, DX FXMVECTOR position, const Box& box)
{
	DX XMVECTOR Half = DX Scale({ box.Size.x, box.Size.y }, 0.5f);
	*Max = DX Add(position, Half);
	*Min = DX Subtract(position, Half);
	DX Add(Max, { 0.f, 0.f, box.Size.z });
}

inline bool XM_CALLCONV HandleCollision(DX FXMVECTOR A_Min, DX FXMVECTOR A_Max, DX FXMVECTOR B_Min, DX GXMVECTOR B_Max)
{
	return !(DX AnyTrue(DX Or(DX Greater(A_Min, B_Max), DX Greater(B_Min, A_Max))));
}

inline DX XMVECTOR XM_CALLCONV GetNormal(DX FXMVECTOR A_Min, DX FXMVECTOR A_Max, DX FXMVECTOR B_Min, DX GXMVECTOR B_Max)
{
	DX XMVECTOR DiffA = DX Subtract(A_Max, B_Min);
	DX XMVECTOR DiffB = DX Subtract(B_Max, A_Min);

	DX XMVECTOR MinA = DX XMVectorSplatOne();
	DX XMVECTOR MinB = DX XMVectorSplatOne();
	DX SetW(&DiffA, (float)0xFFFFFFFF); //Max Value To Check for Min
	DX SetW(&DiffB, (float)0xFFFFFFFF); // Max Value To Check for Min

	//Find Min of Both Vectors
	for (int i = 1; i <= 3; ++i)
	{
		MinA = DX Multiply(MinA, DX Evaluate(DX Less(DiffA, DX ShiftLeft(DiffA, i))));
		MinB = DX Multiply(MinB, DX Evaluate(DX Less(DiffB, DX ShiftLeft(DiffB, i))));
	}
	float FA = DX4 MagnitudeSQ(DX Multiply(DiffA, MinA));
	float FB = DX4 MagnitudeSQ(DX Multiply(DiffB, MinB));
	if (FA < FB) return 
		MinA;	
	else return 
		MinB;
}

NS_COLLISION_END

