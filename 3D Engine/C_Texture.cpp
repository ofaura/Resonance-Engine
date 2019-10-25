#include "C_Texture.h"

C_Texture::C_Texture(GameObject * object) : Component(COMPONENT_TYPE::TEXTURE, object)
{
	name = "Texture";
}

C_Texture::~C_Texture() {}
