#include "assert.h"
#include "par_shapes.h"
#include "Application.h"
#include "ModuleScene.h"
#include "MeshImporter.h"
#include "ComponentMesh.h"
#include "ModuleLibrary.h"
#include "ComponentMaterial.h"
#include "Math/float3.h"
#include "Math/float2.h"

ComponentMesh::ComponentMesh(GameObject* goContainer, Mesh* mesh) : Component(goContainer, ComponentType::MESH) { }

ComponentMesh::ComponentMesh(const ComponentMesh& duplicatedComponent) : Component(duplicatedComponent) {
	mesh = duplicatedComponent.mesh;
}

ComponentMesh::~ComponentMesh() {
	CleanUp();
}

Component* ComponentMesh::Duplicate() {
	return new ComponentMesh(*this);
}

void ComponentMesh::CleanUp() {
	//TODO!: when deleted we should remove the component mesh element instead of ignoring it
	goContainer->mesh = nullptr;
	delete this;
}

void ComponentMesh::Draw(unsigned shaderProgram, const ComponentMaterial* material) const {
	glActiveTexture(GL_TEXTURE0);

	glUniform4f(glGetUniformLocation(shaderProgram, "diffuseColor"), material->material.diffuseColor.x, material->material.diffuseColor.y, material->material.diffuseColor.z, 1.0f);
	glUniform4f(glGetUniformLocation(shaderProgram, "emissiveColor"), material->material.emissiveColor.x, material->material.emissiveColor.y, material->material.emissiveColor.z, 1.0f);
	glUniform4f(glGetUniformLocation(shaderProgram, "specularColor"), material->material.specularColor.x, material->material.specularColor.y, material->material.specularColor.z, 1.0f);
	glUniform3fv(glGetUniformLocation(shaderProgram, "light_pos"), 1, (float*)&App->scene->lightPosition);
	glUniform1f(glGetUniformLocation(shaderProgram, "ambient"), App->scene->ambientLight);
	glUniform1f(glGetUniformLocation(shaderProgram, "shininess"), material->material.shininess);
	glUniform1f(glGetUniformLocation(shaderProgram, "k_ambient"), material->material.ambientK);
	glUniform1f(glGetUniformLocation(shaderProgram, "k_diffuse"), material->material.diffuseK);
	glUniform1f(glGetUniformLocation(shaderProgram, "k_specular"), material->material.specularK);
	glUniform4fv(glGetUniformLocation(shaderProgram, "newColor"), 1, (float*)&material->material.color);

	if (material->material.diffuseMap != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material->material.diffuseMap);
		glUniform1i(glGetUniformLocation(shaderProgram, "diffuseMap"), 0);
		glUniform1i(glGetUniformLocation(shaderProgram, "useDiffuseMap"), 1);
	}
	else {
		glUniform1i(glGetUniformLocation(shaderProgram, "useDiffuseMap"), 0);
	}

	if (material->material.emissiveMap != 0) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, material->material.emissiveMap);
		glUniform1i(glGetUniformLocation(shaderProgram, "emissiveMap"), 1);
		glUniform1i(glGetUniformLocation(shaderProgram, "useEmissiveMap"), 1);
	}
	else {
		glUniform1i(glGetUniformLocation(shaderProgram, "useEmissiveMap"), 0);
	}

	if (material->material.occlusionMap != 0) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, material->material.occlusionMap);
		glUniform1i(glGetUniformLocation(shaderProgram, "occlusionMap"), 2);
		glUniform1i(glGetUniformLocation(shaderProgram, "useOcclusionMap"), 1);
	}
	else {
		glUniform1i(glGetUniformLocation(shaderProgram, "useOcclusionMap"), 0);
	}

	if (material->material.specularMap != 0) {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, material->material.specularMap);
		glUniform1i(glGetUniformLocation(shaderProgram, "specularMap"), 3);
		glUniform1i(glGetUniformLocation(shaderProgram, "useSpecularMap"), 1);
	}
	else {
		glUniform1i(glGetUniformLocation(shaderProgram, "useSpecularMap"), 0);
	}

	glBindVertexArray(mesh.vao);
	glDrawElements(GL_TRIANGLES, mesh.indicesNumber, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ComponentMesh::DrawProperties() {

	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("Mesh")) {

		bool removed = Component::DrawComponentState();
		if (removed) {
			ImGui::PopID();
			return;
		}

		ImGui::Button("Mesh options");

		if (ImGui::IsItemClicked(0)) {
			ImGui::OpenPopup("MeshOptionsContextualMenu");
		}

		if (ImGui::BeginPopup("MeshOptionsContextualMenu")) {
			ImGui::PopID();
			ImGui::PushID("DeleteMesh");
			if (ImGui::Button("Delete mesh")) {}
			ImGui::PopID();
			ImGui::EndPopup();
		}

		ImGui::Separator();

		std::vector<std::string> fileMeshesList = App->library->fileMeshesList;
		currentMesh;
		fileMeshesList.insert(fileMeshesList.begin(), "Select mesh");

		if (fileMeshesList.size() > 0) {
			if (ImGui::BeginCombo("##meshCombo", currentMesh.c_str())) {
				for (std::vector<std::string>::iterator iterator = fileMeshesList.begin(); iterator != fileMeshesList.end(); ++iterator) {
					bool isSelected = (currentMesh == (*iterator));
					if (ImGui::Selectable((*iterator).c_str(), isSelected)) {
						currentMesh = (*iterator);

						LoadMesh(currentMesh.c_str());
						if (isSelected) {
							ImGui::SetItemDefaultFocus();
						}
					}
				}
				ImGui::EndCombo();
			}
		}

		ImGui::Separator();

		ImGui::Text("Triangles count: %d", mesh.verticesNumber / 3);
		ImGui::Text("Vertices count: %d", mesh.verticesNumber);
	}

	ImGui::PopID();
}

void ComponentMesh::LoadMesh(const char* name) {
	if (mesh.vbo != 0) {
		glDeleteBuffers(1, &mesh.vbo);
	}

	if (mesh.ibo != 0) {
		glDeleteBuffers(1, &mesh.ibo);
	}

	MeshImporter::Load(&mesh, name);
	ComputeMesh();
	goContainer->ComputeBBox();
}

void ComponentMesh::ComputeMesh() { 
	glGenBuffers(1, &mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

	unsigned offset = sizeof(math::float3);

	if (mesh.normals != nullptr) {
		mesh.normalsOffset = offset;
		offset += sizeof(math::float3);
	}

	if (mesh.uvs != nullptr) {
		mesh.texturesOffset = offset;
		offset += sizeof(math::float2);
	}

	mesh.vertexSize = offset;

	glBufferData(GL_ARRAY_BUFFER, mesh.vertexSize * mesh.verticesNumber, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * mesh.verticesNumber, mesh.vertices);

	if (mesh.normals != nullptr) {
		glBufferSubData(GL_ARRAY_BUFFER, mesh.normalsOffset * mesh.verticesNumber, sizeof(float) * 3 * mesh.verticesNumber, mesh.normals);
	}

	if (mesh.uvs != nullptr) {
		glBufferSubData(GL_ARRAY_BUFFER, mesh.texturesOffset * mesh.verticesNumber, sizeof(float2)*mesh.verticesNumber, mesh.uvs);
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &mesh.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indicesNumber * sizeof(unsigned), mesh.indices, GL_STATIC_DRAW);

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &mesh.vao);

	glBindVertexArray(mesh.vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	if (mesh.normalsOffset != 0) {
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(mesh.normalsOffset * mesh.verticesNumber));
	}

	if (mesh.texturesOffset != 0) {
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(mesh.texturesOffset * mesh.verticesNumber));
	}

	glBindVertexArray(0);

	mesh.bbox.SetNegativeInfinity();
	mesh.bbox.Enclose((float3*)mesh.vertices, mesh.verticesNumber);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ComponentMesh::ComputeMesh(par_shapes_mesh_s* parMesh) {
	assert(parMesh != nullptr);

	glGenBuffers(1, &mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

	unsigned offset = sizeof(math::float3);

	if (parMesh->normals) {
		mesh.normalsOffset = offset;
		offset += sizeof(math::float3);
	}

	mesh.vertexSize = offset;

	glBufferData(GL_ARRAY_BUFFER, mesh.vertexSize * parMesh->npoints, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(math::float3) * parMesh->npoints, parMesh->points);

	if (parMesh->normals) {
		glBufferSubData(GL_ARRAY_BUFFER, mesh.normalsOffset * parMesh->npoints, sizeof(math::float3) * parMesh->npoints, parMesh->normals);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &mesh.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * parMesh->ntriangles * 3, nullptr, GL_STATIC_DRAW);

	unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned) * parMesh->ntriangles * 3, GL_MAP_WRITE_BIT);

	for (unsigned i = 0; i< unsigned(parMesh->ntriangles * 3); ++i) {
		*(indices++) = parMesh->triangles[i];
	}

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	mesh.verticesNumber = parMesh->npoints;
	mesh.indicesNumber = parMesh->ntriangles * 3;

	glGenVertexArrays(1, &mesh.vao);

	glBindVertexArray(mesh.vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	if (mesh.normalsOffset != 0) {
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(mesh.normalsOffset * mesh.verticesNumber));
	}

	if (mesh.texturesOffset != 0) {
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(mesh.texturesOffset * mesh.verticesNumber));
	}

	glBindVertexArray(0);

	mesh.bbox.SetNegativeInfinity();
	mesh.bbox.Enclose((float3*)parMesh->points, parMesh->npoints);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
