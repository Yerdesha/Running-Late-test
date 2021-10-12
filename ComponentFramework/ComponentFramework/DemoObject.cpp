#include "DemoObject.h"

#include "MMath.h"

DemoObject::DemoObject(Mesh *mesh_, Shader *shader_, Texture *texture_): 
	mesh(mesh_), shader(shader_), texture(texture_) {	
}

DemoObject::~DemoObject() {}

bool DemoObject::OnCreate() { return true;  } /// Just a stub
void DemoObject::OnDestroy() {}				  /// Just a stub
void DemoObject::Update(float deltaTime_) {} /// Just a stub

void DemoObject::Render() const {

	/// This is some fancy code.  Assigning a 4x4 matrix to a 3x3 matrix
	/// just steals the upper 3x3 of the 4x4 and assigns thoses values 
	/// to the 3x3.  
	Matrix3 normalMatrix = MMath::transpose(MMath::inverse(modelMatrix));

	glUniformMatrix4fv(shader->getUniformID("modelMatrix"), 1, GL_FALSE, modelMatrix);
	glUniformMatrix3fv(shader->getUniformID("normalMatrix"), 1, GL_FALSE, normalMatrix);
	if (texture) {
		glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	}
	mesh->Render();

	/// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void DemoObject::HandleEvents(const SDL_Event &event) {} /// Just a stub