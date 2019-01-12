#include "assert.h"
#include "Config.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleLibrary.h"
#include "imgui_internal.h"
#include "ModuleTextures.h"
#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(GameObject* goContainer) : Component(goContainer, ComponentType::MATERIAL) { 

}

ComponentMaterial::ComponentMaterial(GameObject* goContainer, const aiMaterial* material) : Component(goContainer, ComponentType::MATERIAL) { 

}

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

void ComponentMaterial::DrawProperties(bool staticGo) {
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("Material")) {

		if (staticGo) {
			ImGui::PushItemFlag({ ImGuiButtonFlags_Disabled | ImGuiItemFlags_Disabled | ImGuiSelectableFlags_Disabled }, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

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

		if (staticGo) {
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
	}
	ImGui::PopID();
}

void ComponentMaterial::DrawComboBoxMaterials(const char* id, MaterialType matType, static std::string& currentTexture) {

		ImGui::PushID(id);
		if (ImGui::BeginCombo("##", currentTexture.c_str())) {
			for (std::vector<std::string>::iterator iterator = App->library->fileTexturesList->begin(); iterator != App->library->fileTexturesList->end(); ++iterator) {
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

void ComponentMaterial::UnloadMaterial() {
	DeleteTexture(material.diffuseMap);
	DeleteTexture(material.specularMap);
	DeleteTexture(material.occlusionMap);
	DeleteTexture(material.emissiveMap);

	Material emptyMaterial;
	material = emptyMaterial;
}

void ComponentMaterial::Save(Config* config) {
	config->StartObject();

	config->AddComponentType("componentType", componentType);
	config->AddString("parent", goContainer->uuid);

	config->AddString("diffuseSelected", diffuseSelected.c_str());
	config->AddFloat4("diffuseColor", material.diffuseColor);
	config->AddFloat("diffuseK", material.diffuseK);

	config->AddString("occlusionSelected", occlusionSelected.c_str());
	config->AddFloat("ambientK", material.ambientK);

	config->AddString("specularSelected", specularSelected.c_str());
	config->AddFloat4("specularColor", material.specularColor);
	config->AddFloat("specularK", material.specularK);
	config->AddFloat("shininess", material.shininess);

	config->AddString("emissiveSelected", emissiveSelected.c_str());
	config->AddFloat4("emissiveColor", material.emissiveColor);

	config->EndObject();
}

void ComponentMaterial::Load(Config* config, rapidjson::Value& value) {
	diffuseSelected = config->GetString("diffuseSelected", value);
	material.diffuseColor = config->GetFloat4("diffuseColor", value);
	material.diffuseK = config->GetFloat("diffuseK", value);
	App->textures->LoadMaterial(diffuseSelected.c_str(), this, MaterialType::DIFFUSE_MAP);

	occlusionSelected = config->GetString("occlusionSelected", value);
	material.ambientK = config->GetFloat("ambientK", value);
	App->textures->LoadMaterial(occlusionSelected.c_str(), this, MaterialType::OCCLUSION_MAP);

	specularSelected = config->GetString("specularSelected", value);
	material.specularColor = config->GetFloat4("specularColor", value);
	material.specularK = config->GetFloat("specularK", value);
	material.shininess = config->GetFloat("shininess", value);
	App->textures->LoadMaterial(specularSelected.c_str(), this, MaterialType::SPECULAR_MAP);

	emissiveSelected = config->GetString("emissiveSelected", value);
	material.emissiveColor = config->GetFloat4("emissiveColor", value);
	App->textures->LoadMaterial(emissiveSelected.c_str(), this, MaterialType::EMISSIVE_MAP);
}
