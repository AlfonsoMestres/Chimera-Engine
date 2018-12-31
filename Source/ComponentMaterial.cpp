#include "Application.h"
#include "ModuleProgram.h"
#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(GameObject* goContainer) : Component(goContainer, ComponentType::MATERIAL) {
	shader = App->program->textureProgram;
}

ComponentMaterial::ComponentMaterial(GameObject* goContainer, const aiMaterial* material) : Component(goContainer, ComponentType::MATERIAL) {
	shader = App->program->textureProgram;
	ComputeMaterial(material);
}

ComponentMaterial::ComponentMaterial(const ComponentMaterial& duplicatedComponent) : Component(duplicatedComponent) {
	shader = duplicatedComponent.shader;
	texture = duplicatedComponent.texture;
}

ComponentMaterial::~ComponentMaterial() { 
	DeleteTexture();
}

Component* ComponentMaterial::Duplicate() {
	return new ComponentMaterial(*this);
}

void ComponentMaterial::ComputeMaterial(const aiMaterial* material) {
	std::string texturePath;
	if (material != nullptr) {
		aiTextureMapping mapping = aiTextureMapping_UV;
		aiString file;
		unsigned uvindex = 0u;

		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, &uvindex) == AI_SUCCESS) {
			texturePath = goContainer->GetFileFolder() + file.C_Str();
			texture = App->textures->Load(texturePath.c_str());
		} else {
			texture = App->textures->defaultTexture;
		}
	} else {
		texture = App->textures->defaultTexture;
	}

	/*DeleteTexture();*/
}


void ComponentMaterial::DeleteTexture() {
	//TODO: Ask to resource manager if texture is being used by other GO
	if (texture != nullptr) {
		glDeleteTextures(1, (GLuint*)& texture->id);
	}

	delete texture;
	texture = nullptr;
}

void ComponentMaterial::DrawProperties() {
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("Material")) {
		bool removed = Component::DrawComponentState();
		if (removed) {
			ImGui::PopID();
			return;
		}

		if (texture == nullptr) {
			float colors[3] = { color.x, color.y, color.z };
			if (ImGui::ColorPicker3("", colors, ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_HEX)) {
				color = { colors[0], colors[1], colors[2] , color.w };
			}
		}

		if (texture != nullptr) {
			ImGui::Text("Texture width:%d height:%d", texture->width, texture->height);
			float size = ImGui::GetWindowWidth();
			ImGui::Image((ImTextureID)texture->id, { size,size });
			ImGui::Separator();
		}
	}
	ImGui::PopID();
}