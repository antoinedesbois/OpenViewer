
#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Scene.h"
#include "Sphere.h"
#include "Light.h"
#include "ModelLoader.h"

#include "GLM/glm/glm.hpp"
#include "GLM/glm/gtc/matrix_transform.hpp"
#include "GLM/glm/gtc/type_ptr.hpp"

Scene::Scene()
	: m_levelIsDone(false)
	, m_renderQuad(nullptr)
	, m_objectsToBeCreated()
{
}

Scene::~Scene()
{
	sceneTearDown(); 
}

void Scene::Initialize()
{
	createShaderPrograms();
	createMaterials();
	buffersSetup();
	levelSetup();
	lightSetup();
}

void Scene::sceneTearDown()
{
	for (auto it = m_shaderPrograms.begin(); it != m_shaderPrograms.end(); ++it)
	{
		delete it->second; it->second = nullptr;
	}
	m_shaderPrograms.clear();

	for (auto it = m_materials.begin(); it != m_materials.end(); ++it)
	{
		delete it->second; it->second = nullptr;
	}
	m_materials.clear();

	for (auto it = m_frameBuffers.begin(); it != m_frameBuffers.end(); ++it)
	{
		delete it->second; it->second = nullptr;
	}

	for (int i = 0; i < m_objects.size(); ++i)
	{
		delete m_objects[i]; m_objects[i] = nullptr;
	}

	for (int i = 0; i < m_lights.size(); ++i)
	{
		delete m_lights[i]; m_lights[i] = nullptr;
	}

	if (m_renderQuad)
	{
		delete m_renderQuad; m_renderQuad = nullptr;
		for each (Object* obj in m_objects)
		{
			delete m_renderQuad; m_renderQuad = nullptr;
		}
	}
}

void Scene::drawAllLights(bool allLightsAreDrawn)
{
	m_allLightsAreDrawn = allLightsAreDrawn;
	for each (auto light in m_lights)
	{
		light->setVisible(allLightsAreDrawn);
	}
}

void Scene::loadModel(const ModelImportOptions* options)
{
	m_objectsToBeCreated.push(options);
}

void Scene::loadPendingModels()
{
	while (!m_objectsToBeCreated.empty())
	{
		const ModelImportOptions* importOptions = m_objectsToBeCreated.front();
		ModelContainer* model = ModelLoader::loadModel(
			importOptions->path, 
			m_materials["default"], 
			m_shaderPrograms[importOptions->shader]->getId(), 
			std::vector<Vec3>());

		if (importOptions->smoothNormal)
			model->smoothNormals();

		model->translate(importOptions->position);
		model->setVisible(true);
		
		m_objectVectorMutex.lock();
		{
			m_objects.push_back(model);
			m_objectsToBeCreated.pop();
			delete importOptions;
		}
		m_objectVectorMutex.unlock();
	}

}

void Scene::removePendingBackgrounds()
{
	while (!m_backgroundToBeRemoved.empty())
	{
		ModelContainer* model = m_backgroundToBeRemoved.front().first;
		Vec3 color = m_backgroundToBeRemoved.front().second;
		std::vector<Vec3> colors;
		colors.push_back(color);
		model->removeBackground(colors);
		m_backgroundToBeRemoved.pop();
	}
}

void Scene::addBackgroundToBeRemoved(ModelContainer* obj, Vec3 color)
{
	m_backgroundToBeRemoved.push(std::pair<ModelContainer*, Vec3>(obj, color));
}

std::vector<std::string> Scene::getShaderList() const
{
	std::vector<std::string> shaders;

	for (auto i = m_shaderPrograms.begin(); i != m_shaderPrograms.end(); ++i)
	{
		shaders.push_back(i->first);
	}
	return shaders;
}

void Scene::preDraw()
{
	loadPendingModels();
	removePendingBackgrounds();
	m_objectVectorMutex.lock();
}

void Scene::postDraw()
{
	m_objectVectorMutex.unlock();
}

void Scene::removeObject(Object* obj)
{
	for (int i = 0; i < m_objects.size(); ++i)
	{
		if (m_objects[i] == obj)
			m_objects[i]->setVisible(false);
	}
}