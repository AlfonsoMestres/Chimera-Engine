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

		//TODO: First we will require to have our own materials loaded from .dds
		//if (ImGui::CollapsingHeader("Diffuse")) {
		//	ImGui::ColorEdit3("Diffuse color", (float*)&diffuseColor);
		//	/*DrawComboBoxMaterials("DiffuseComboTextures", MaterialTypeSelected::DIFFUSE_MAP, labelDiffuseCurrentFileTextureSelected);*/
		//	ImGui::Text("Dimensions: %dx%d", diffuseWidth, diffuseHeight);
		//	ImGui::Image((ImTextureID)diffuseMap, ImVec2(200, 200));
		//	ImGui::SliderFloat("K diffuse", &diffuseK, 0.0f, 1.0f);
		//}

		//if (ImGui::CollapsingHeader("Ambient")) {
		//	/*DrawComboBoxMaterials("OcclusionComboTextures", MaterialTypeSelected::OCCLUSION_MAP, labelOcclusionCurrentFileTextureSelected);*/
		//	ImGui::Text("Dimensions: %dx%d", ambientWidth, ambientHeight);
		//	ImGui::Image((ImTextureID)occlusionMap, ImVec2(200, 200));
		//	ImGui::SliderFloat("K ambient", &ambientK, 0.0f, 1.0f);
		//}

		//if (ImGui::CollapsingHeader("Specular")) {
		//	ImGui::ColorEdit3("Specular color", (float*)&specularColor);
		//	/*DrawComboBoxMaterials("SpecularComboTextures", MaterialTypeSelected::SPECULAR_MAP, labelSpecularCurrentFileTextureSelected);*/
		//	ImGui::Text("Dimensions: %dx%d", specularWidth, specularHeight);
		//	ImGui::Image((ImTextureID)specularMap, ImVec2(200, 200));
		//	ImGui::SliderFloat("K specular", &specularK, 0.0f, 1.0f);
		//	ImGui::SliderFloat("K shininess", &shininess, 0.0f, 128.0f);
		//}

		//if (ImGui::CollapsingHeader("Emissive")) {
		//	ImGui::ColorEdit3("Emissive color", (float*)&emissiveColor);
		//	/*DrawComboBoxMaterials("EmissiveComboTextures", MaterialTypeSelected::EMISSIVE_MAP, labelEmissiveCurrentFileTextureSelected);*/
		//	ImGui::Text("Dimensions: %dx%d", emissiveWidth, emissiveHeight);
		//	ImGui::Image((ImTextureID)emissiveMap, ImVec2(200, 200));
		//}
	}
	ImGui::PopID();
}