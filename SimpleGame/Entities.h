#pragma once
class Actor
{
public:
	u_int		GraphicsIndex;
	u_int		PhysicsIndex;
	u_int		StateIndex;
	u_int		Team; //Placed here  cause no other place to put it for now :)
};

class Object
{
public:
	u_int GraphicsIndex;
	u_int PhysicsIndex;
	u_int Team;
};
