#include "Config.h"
#include "Component.h"
#include "Application.h"
#include "ModuleFileSystem.h"

Config::Config() {
	stringBuffer = new rapidjson::StringBuffer();
	writer = new rapidjson::PrettyWriter<rapidjson::StringBuffer>(*stringBuffer);
	writer->StartObject();
}

Config::~Config() {
	delete[] writer;
	delete [] stringBuffer;

	writer = nullptr;
	stringBuffer = nullptr;
}

void Config::AddComponentType(const char* name, ComponentType componentType) {
	writer->String(name);

	switch (componentType) {
		case ComponentType::CAMERA:
			writer->String("CAMERA");
			break;
		case ComponentType::MATERIAL:
			writer->String("MATERIAL");
			break;
		case ComponentType::MESH:
			writer->String("MESH");
			break;
		case ComponentType::TRANSFORM:
			writer->String("TRANSFORM");
			break;
	}

}

void Config::AddInt(const char* name, int value) {
	writer->String(name);
	writer->Int(value);
}

void Config::AddUnsigned(const char* name, unsigned value) {
	writer->String(name);
	writer->Uint(value);
}

void Config::AddFloat(const char* name, float value) {
	writer->String(name);
	writer->Double(value);
}

void Config::AddString(const char* name, const char* value) {
	writer->String(name);
	writer->String(value);
}

void Config::AddBool(const char* name, bool value) {
	writer->String(name);
	writer->Bool(value);
}

void Config::AddFloat3(const char* name, math::float3 value) {
	StartObject(name);
	writer->String("x");
	writer->Double(value.x);
	writer->String("y");
	writer->Double(value.y);
	writer->String("z");
	writer->Double(value.z);
	EndObject();
}

void Config::AddFloat4(const char* name, math::float4 value) {
	StartObject(name);
	writer->String("x");
	writer->Double(value.x);
	writer->String("y");
	writer->Double(value.y);
	writer->String("z");
	writer->Double(value.z);
	writer->String("w");
	writer->Double(value.w);
	EndObject();
}

void Config::AddQuat(const char* name, math::Quat value) {
	StartObject(name);
	writer->String("x");
	writer->Double(value.x);
	writer->String("y");
	writer->Double(value.y);
	writer->String("z");
	writer->Double(value.z);
	writer->String("w");
	writer->Double(value.w);
	EndObject();
}

ComponentType Config::GetComponentType(const char* name, rapidjson::Value& value) {
	const char* stringComponentType = value[name].GetString();

	if (strcmp(stringComponentType, "TRANSFORM") == 0) {
		return ComponentType::TRANSFORM;
	}
	if (strcmp(stringComponentType, "MESH") == 0) {
		return ComponentType::MESH;
	}
	if (strcmp(stringComponentType, "MATERIAL") == 0) {
		return ComponentType::MATERIAL;
	}
	if (strcmp(stringComponentType, "CAMERA") == 0) {
		return ComponentType::CAMERA;
	}
}

int Config::GetInt(const char* name, rapidjson::Value& value) {
	return value[name].GetInt();
}

unsigned Config::GetUnsigned(const char* name, rapidjson::Value& value) {
	return value[name].GetUint();
}

float Config::GetFloat(const char* name, rapidjson::Value& value) {
	return value[name].GetFloat();
}

const char* Config::GetString(const char* name, rapidjson::Value& value) {
	return value[name].GetString();
}

bool Config::GetBool(const char* name, rapidjson::Value& value) {
	return value[name].GetBool();
}

math::float3 Config::GetFloat3(const char* name, rapidjson::Value& value) {
	math::float3 result = {
		value[name]["x"].GetFloat(),
		value[name]["y"].GetFloat(),
		value[name]["z"].GetFloat()
	};

	return result;
}

math::float4 Config::GetFloat4(const char* name, rapidjson::Value& value) {
	math::float4 result = {
		value[name]["x"].GetFloat(),
		value[name]["y"].GetFloat(),
		value[name]["z"].GetFloat(),
		value[name]["w"].GetFloat()
	};

	return result;
}

math::Quat Config::GetQuat(const char* name, rapidjson::Value& value) {
	math::Quat result = {
		value[name]["x"].GetFloat(),
		value[name]["y"].GetFloat(),
		value[name]["z"].GetFloat(),
		value[name]["w"].GetFloat()
	};

	return result;
}

void Config::AddName(const char* name) {
	writer->String(name);
}

void Config::StartObject(const char* name) {
	writer->String(name);
	writer->StartObject();
}

void Config::StartObject() {
	writer->StartObject();
}

void Config::EndObject() {
	writer->EndObject();
}

void Config::StartArray(const char* name) {
	writer->String(name);
	writer->StartArray();
}

void Config::EndArray() {
	writer->EndArray();
}

void Config::WriteToDisk() {
	writer->EndObject();
	App->fileSystem->Save("/Library/Scene/scene.json", stringBuffer->GetString(), strlen(stringBuffer->GetString()), false);
}

rapidjson::Document Config::LoadFromDisk() {
	rapidjson::Document result = nullptr;

	char* fileBuffer;
	unsigned lenghBuffer = App->fileSystem->Load("/Library/Scene/scene.json", &fileBuffer);

	if (fileBuffer) {
		if (result.Parse<rapidjson::kParseStopWhenDoneFlag>(fileBuffer).HasParseError()) {
			result = nullptr;
		}
	}

	return result;
}
