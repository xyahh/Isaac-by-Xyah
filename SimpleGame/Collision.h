#pragma once
#define NS_COLLISION_START namespace Collision {
#define NS_COLLISION_END }

class BasicCollision
{
public:
	BasicCollision() {}
	virtual ~BasicCollision() {}

	virtual void SSE_CALLCONV OnCollision
	(
		size_t MyID,
		Physics* MyBody,
		size_t CollidingID,
		Physics* CollidingBody,
		SSE_VECTOR_PARAM1 CollisionNormal
	);

private:
};

class ActorCollision : public BasicCollision
{
public:
	ActorCollision() {}
	virtual ~ActorCollision() {}

	virtual void SSE_CALLCONV OnCollision
	(
		size_t MyID,
		Physics* MyBody,
		size_t CollidingID,
		Physics* CollidingBody,
		SSE_VECTOR_PARAM1 CollisionNormal
	);
};

class ProjectileCollision : public BasicCollision
{
public:
	ProjectileCollision() {}
	virtual	~ProjectileCollision() {}

	virtual void SSE_CALLCONV OnCollision
	(
		size_t MyID,
		Physics* MyBody,
		size_t CollidingID,
		Physics* CollidingBody,
		SSE_VECTOR_PARAM1 CollisionNormal
	);
};

class StructureCollision : public BasicCollision
{
public:
	StructureCollision() {}
	virtual	~StructureCollision() {}

	virtual void SSE_CALLCONV OnCollision
	(
		size_t MyID,
		Physics* MyBody, 
		size_t CollidingID,
		Physics* CollidingBody, 
		SSE_VECTOR_PARAM1 CollisionNormal
	);
};


class ExplosionCollision : public BasicCollision
{
public:
	ExplosionCollision() {}
	virtual ~ExplosionCollision() {}

	virtual void SSE_CALLCONV OnCollision
	(
		size_t MyID,
		Physics* MyBody,
		size_t CollidingID,
		Physics* CollidingBody,
		SSE_VECTOR_PARAM1 CollisionNormal
	);

private:
};

NS_COLLISION_START

static BasicCollision		Basic;
static ActorCollision		Actor;
static ProjectileCollision	Projectile;
static StructureCollision	Structure;
static ExplosionCollision	Explosion;

class BBox
	{
	public:
		BBox() :
			Size(0.f, 0.f, 0.f) {}

		void SSE_CALLCONV SetDimensions(SSE_VECTOR_PARAM1 v);
		SSE_VECTOR SSE_CALLCONV GetDimensions() const;
		friend void SSE_CALLCONV GetExtents(SSE_VECTOR* Min,  SSE_VECTOR* Max,  SSE_VECTOR_PARAM1 position, const BBox& box);

	private:
		 FLOAT3	Size;
	};

inline  SSE_VECTOR SSE_CALLCONV GetBBoxCenter(SSE_VECTOR_PARAM1 ObjectPosition, float BBoxHeight)
{
	return  Add(ObjectPosition, {0.f, 0.f, BBoxHeight * 0.5f});
}

inline void SSE_CALLCONV GetExtents( SSE_VECTOR* Min,  SSE_VECTOR* Max,  SSE_VECTOR_PARAM1 position, const BBox& box)
{
	 SSE_VECTOR Half =  Scale({ box.Size.x, box.Size.y }, 0.5f);
	*Max =  Add(position, Half);
	*Min =  Subtract(position, Half);
	*Max =  Add(*Max, { 0.f, 0.f, box.Size.z });
}

inline bool SSE_CALLCONV HandleCollision( SSE_VECTOR_PARAM1 A_Min,  SSE_VECTOR_PARAM1 A_Max,  SSE_VECTOR_PARAM1 B_Min,  SSE_VECTOR_PARAM2 B_Max)
{
	return !( AnyTrue( Or( Greater(A_Min, B_Max),  Greater(B_Min, A_Max))));
}

inline  SSE_VECTOR SSE_CALLCONV GetNormal( SSE_VECTOR_PARAM1 A_Min,  SSE_VECTOR_PARAM1 A_Max,  SSE_VECTOR_PARAM1 B_Min,  SSE_VECTOR_PARAM2 B_Max)
{
	 SSE_VECTOR DiffA =  Subtract(A_Max, B_Min);
	 SSE_VECTOR DiffB =  Subtract(B_Max, A_Min);

	 SSE_VECTOR MinA = VectorOne();
	 SSE_VECTOR MinB = VectorOne();
	 SetW(&DiffA, (float)0xFFFFFFFF); //Max Value To Check for Min
	 SetW(&DiffB, (float)0xFFFFFFFF); // Max Value To Check for Min

	//Find Min of Both Vectors
	for (int i = 1; i <= 3; ++i)
	{
		MinA =  Multiply(MinA,  Evaluate( Less(DiffA,  ShiftLeft(DiffA, i))));
		MinB =  Multiply(MinB,  Evaluate( Less(DiffB,  ShiftLeft(DiffB, i))));
	}

	float FA = MagnitudeSq4( Multiply(DiffA, MinA));
	float FB = MagnitudeSq4( Multiply(DiffB, MinB));
	if (FA < FB) return 
		MinA;	
	else return 
		MinB;
}

NS_COLLISION_END

