#ifndef SCENE0_H
#define SCENE0_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class DemoObject;
class Mesh;
class Shader;
class Texture;

class Scene0 : public Scene {
private:
	DemoObject *demoObject;
	DemoObject* demoObject2;
	Vec3 lightSource;
	Mesh *meshPtr;
	Mesh* meshPtr2;
	Shader *shaderPtr;
	Texture *texturePtr;
	Texture* texturePtr2;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
public:
	explicit Scene0();
	virtual ~Scene0();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
};


#endif // SCENE0_H