#include "GL/glew.h"
#include "assert.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleProgram.h"

#include "ModuleModel.h"

ModuleModel::ModuleModel() { }

ModuleModel::~ModuleModel() { }

unsigned ModuleModel::Load(CustomFile& path) {

	models.emplace_back(path);

	LOG("New Model loaded: %s", path.name);

	return 1;
}

void ModuleModel::DrawModels() {

	for (auto const& model : models) {
		model.Draw();
	}

}

void ModuleModel::ApplyTexture(Texture& texture) {
	
	for (auto& model : models) {
		model.UpdateTexture(texture);
	}

	/*LOG("New texture loaded: ", texture.fileName);*/

}

void ModuleModel::DeleteModels() {
	models.clear();
}

void ModuleModel::DrawGUI() {

	if (models.size() != 0) {

		if (ImGui::CollapsingHeader("Model")) {

			for (auto& model : models) {
				model.DrawInfo();
			}

		}

		if (ImGui::Checkbox("Checkers Texture", &checkersTexture)) {

			if (checkersTexture && checkTexture.id == 0) {
				checkTexture = App->textures->Load(App->textures->checkersFile);
			}

		}

	}

}
