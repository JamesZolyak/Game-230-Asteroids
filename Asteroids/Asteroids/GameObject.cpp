#include "GameObject.h"



GameObject::GameObject()
{
}

float GameObject::Length() {
	return sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
}

Vector2f GameObject::Normalize() {
	Vector2f vector;
	float length = this->Length();

	if (length != 0) {
		vector.x = velocity.x / length;
		vector.y = velocity.y / length;
	}

	return vector;
}


GameObject::~GameObject()
{
}
