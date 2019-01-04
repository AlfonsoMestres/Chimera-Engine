#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleLibrary.h"
#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(GameObject* goContainer) : Component(goContainer, ComponentType::MATERIAL) { }

ComponentMaterial::ComponentMaterial(GameObject* goContainer, const aiMaterial* material) : Component(goContainer, ComponentType::MATERIAL) { }

ComponentMaterial::ComponentMaterial(const ComponentMaterial& duplicatedComponent) : Component(duplicatedComponent) {
	material = duplicatedComponent.material;
}

ComponentMaterial::~ComponentMaterial() { 
	UnloadMaterial();
}

Component* ComponentMaterial::Duplicate() {
	return new ComponentMaterial(*this);
}

void ComponentMaterial::DeleteTexture(unsigned id) {
	if (id != 0u) {
		glDeleteTextures(1, &id);
	}
}

void ComponentMaterial::DrawProperties() {
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("Material")) {
		bool removed = Component::DrawComponentState();
		if (removed) {
			ImGui::PopID();
			return;
		}

		ImGui::Button("Material options");

		if (ImGui::IsItemClicked(0)) {
			ImGui::OpenPopup("MaterialOptionsContextualMenu");
		}

		if (ImGui::BeginPopup("MaterialOptionsContextualMenu")) {
			ImGui::PushID("AddMaterial");
			if (ImGui::MenuItem("Add material")) { }
			ImGui::PopID();
			ImGui::PushID("DeleteMaterial");
			if (ImGui::MenuItem("Remove materials")) {
				UnloadMaterial();
			}
			ImGui::PopID();
			ImGui::EndPopup();
		}

		ImGui::Separator();

		if (ImGui::CollapsingHeader("Diffuse")) {
			ImGui::ColorEdit3("Diffuse color", (float*)&material.diffuseColor);
			DrawComboBoxMaterials("DiffuseComboTextures", MaterialType::DIFFUSE_MAP, diffuseSelected);
			ImGui::Text("Dimensions: %dx%d", material.diffuseWidth, material.diffuseHeight);
			ImGui::Image((ImTextureID)material.diffuseMap, ImVec2(200, 200));
			ImGui::SliderFloat("K diffuse", &material.diffuseK, 0.0f, 1.0f);
		}

		if (ImGui::CollapsingHeader("Ambient")) {
			DrawComboBoxMaterials("OcclusionComboTextures", MaterialType::OCCLUSION_MAP, occlusionSelected);
			ImGui::Text("Dimensions: %dx%d", material.ambientWidth, material.ambientHeight);
			ImGui::Image((ImTextureID)material.occlusionMap, ImVec2(200, 200));
			ImGui::SliderFloat("K ambient", &material.ambientK, 0.0f, 1.0f);
		}

		if (ImGui::CollapsingHeader("Specular")) {
			ImGui::ColorEdit3("Specular color", (float*)&material.specularColor);
			DrawComboBoxMaterials("SpecularComboTextures", MaterialType::SPECULAR_MAP, specularSelected);
			ImGui::Text("Dimensions: %dx%d", material.specularWidth, material.specularHeight);
			ImGui::Image((ImTextureID)material.specularMap, ImVec2(200, 200));
			ImGui::SliderFloat("K specular", &material.specularK, 0.0f, 1.0f);
			ImGui::SliderFloat("K shininess", &material.shininess, 0.0f, 128.0f);
		}

		if (ImGui::CollapsingHeader("Emissive")) {
			ImGui::ColorEdit3("Emissive color", (float*)&material.emissiveColor);
			DrawComboBoxMaterials("EmissiveComboTextures", MaterialType::EMISSIVE_MAP, emissiveSelected);
			ImGui::Text("Dimensions: %dx%d", material.emissiveWidth, material.emissiveHeight);
			ImGui::Image((ImTextureID)material.emissiveMap, ImVec2(200, 200));
		}
	}
	ImGui::PopID();
}

void ComponentMaterial::DrawComboBoxMaterials(const char* id, MaterialType matType, static std::string& currentTexture) {
	std::vector<std::string> fileTexturesList = App->library->fileTexturesList;
	fileTexturesList.insert(fileTexturesList.begin(), "Select a Texture");

	if (fileTexturesList.size() > 0) {
		ImGui::PushID(id);
		if (ImGui::BeginCombo("##", currentTexture.c_str())) {
			for (std::vector<std::string>::iterator iterator = fileTexturesList.begin(); iterator != fileTexturesList.end(); ++iterator) {
				bool isSelected = (currentTexture == (*iterator).c_str());
				if (ImGui::Selectable((*iterator).c_str(), isSelected)) {
					currentTexture = (*iterator).c_str();
					App->textures->LoadMaterial(currentTexture.c_str(), this, matType);
					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
			}

			ImGui::EndCombo();
		}

		ImGui::PopID();
	}
}

void ComponentMaterial::UnloadMaterial() {
	DeleteTexture(material.diffuseMap);
	DeleteTexture(material.specularMap);
	DeleteTexture(material.occlusionMap);
	DeleteTexture(material.emissiveMap);

	Material emptyMaterial;
	material = emptyMaterial;
}