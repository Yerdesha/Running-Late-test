#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

union SDL_Event; /// A forward declaration 

class GameObject {
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const = 0;
	virtual void HandleEvents(const SDL_Event &event) = 0;	
};
#endif