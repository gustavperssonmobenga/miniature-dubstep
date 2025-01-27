/**
 * @file
 * @author Martin <mail@mail.nu>, Victor Lundberg <lundberg.victor@gmail.com>
 * @version 1.0
 * Copyright (�) A-Team.
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 */

#include "DX10Module.h"

using namespace gfx;

DX10Module* DX10Module::mpsThreadInstance = NULL;

DX10Module::DX10Module() : Thread(), mWindowHandler(), mRenderHandler(), mShaderHandler(),
						   mObjectHandler(), mMeshLoader()
{
	mCamera = NULL;
	mpTextureHandler = NULL;
	InitVertexStructures();
}

DX10Module::DX10Module(const DX10Module &module) : mWindowHandler(), mRenderHandler(),
					   mShaderHandler(), mObjectHandler(), mMeshLoader()
{
	module;
}

DX10Module::~DX10Module()
{
	if(mCamera)
	{
		delete mCamera;
		mCamera = NULL;
	}
	if(mpTextureHandler)
	{
		delete mpTextureHandler;
		mpTextureHandler = NULL;
	}
	DestroyVertexStructures();
}

const DX10Module &DX10Module::operator=(const DX10Module &module)
{
	if(this != &module)
	{

	}
	return *this;
}

HRESULT DX10Module::init(const char *title, long width, long height)
{
	if(FAILED(mWindowHandler.initWindow(title, width, height)))
	{
		return E_FAIL;
	}
	if(FAILED(mWindowHandler.initDevice()))
	{
		return E_FAIL;
	}
	if(FAILED(mRenderHandler.initRenderer(mWindowHandler.getHWND(), mWindowHandler.getDevice())))
	{
		return E_FAIL;
	}
	mShaderHandler.init(mWindowHandler.getDevice());
	if(FAILED(loadShaders()))
	{
		return E_FAIL;
	}
	mCamera = myNew Camera(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	mpTextureHandler = myNew TextureHandler(mWindowHandler.getDevice());
	mpTextureHandler->addTexture("../../ModelFiles/TankGround/textureTank.png", "TextureTank");
	mpTextureHandler->addTexture("../../ModelFiles/TankAir/textureTankAir.png", "TextureTankAir");
	mShaderHandler.getEffect("BasicColor")->updateTextureArray(mpTextureHandler->getTextureArray("TextureTank,TextureTankAir"));

	//Animation tests.
	/*mpTextureHandler->addTexture("../../ModelFiles/TankGround/n00b.png", "n00b");
	mShaderHandler.getEffect("BasicColor")->updateAnimations(mpTextureHandler->getTextureArray("n00b"));*/

	mObjectHandler.setCamera(mCamera);

	return S_OK;
}

DX10Module* DX10Module::getInstance()
{
	if(mpsThreadInstance == NULL)
	{
		mpsThreadInstance = myNew DX10Module();
	}

	return mpsThreadInstance;
}

void DX10Module::mainLoop()
{
	if(FAILED(init("BugTanks", 1240, 800)))
	{
		DEBUG_MESSAGE("Error: DX10Module::init().")
	}

	setRunFlag(true);

	while(getRunFlag())
	{
		MSG msg = {0};
		while(WM_QUIT != msg.message)
		{
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				lock(0);
				update();
				render();
				unlock(0);

				Sleep(10);
			}
		}
		exit();
		break;
	}
}

void DX10Module::update()
{
	mCamera->update();
	updateShaderPerFrameBuffers();
	mObjectHandler.updateObjects();
	mObjectHandler.updateCulling();
}

void DX10Module::render()
{
	mRenderHandler.start();
	mRenderHandler.renderVector(mObjectHandler.getCulledObjects());
	mRenderHandler.end();
}

void DX10Module::loadMeshModel(const char *filePath, int meshID, const char *meshName)
{
	Mesh *meshModel = mMeshLoader.importMesh(filePath, mWindowHandler.getDevice(), mShaderHandler.getEffect("BasicColor"));
	meshModel->setID(meshID);
	meshModel->setName(meshName);
	mObjectHandler.addMeshModel(meshModel);
}

void DX10Module::loadMeshInstance(const char *filePath, int instanceID, const char *instanceName, unsigned int maxInstances)
{
	InstanceMesh *meshInstance = mMeshLoader.importInstanceMesh(filePath, mWindowHandler.getDevice(), mShaderHandler.getEffect("BasicColor"));
	meshInstance->setID(instanceID);
	meshInstance->setName(instanceName);
	meshInstance->setInstanceCountMax(maxInstances);
	meshInstance->setFlagUpdateFromBase(true);
	mObjectHandler.addMeshInstanceBase(meshInstance);
}

void DX10Module::loadObjectShapeInstance(ShapeTypes::Types type, int instanceID, const char *instanceName, unsigned int maxInstances)
{
	InstanceObject *objectInstanceBase = NULL;

	switch(type)
	{
		case ShapeTypes::TRIANGLE:
			objectInstanceBase = myNew InstanceShapeTriangle(mWindowHandler.getDevice(), mShaderHandler.getEffect("BasicColor"));
			objectInstanceBase->setFlagInstanceBase(true);
			objectInstanceBase->setID(instanceID);
			objectInstanceBase->setName(instanceName);
			objectInstanceBase->setInstanceCountMax(maxInstances);
			objectInstanceBase->setFlagUpdateFromBase(true);
			((InstanceShapeCube *)objectInstanceBase)->loadBuffers();
			mObjectHandler.addObjectInstanceBase(objectInstanceBase);
			break;

		case ShapeTypes::CUBE:
			objectInstanceBase = myNew InstanceShapeCube(mWindowHandler.getDevice(), mShaderHandler.getEffect("BasicColor"));
			objectInstanceBase->setFlagInstanceBase(true);
			objectInstanceBase->setID(instanceID);
			objectInstanceBase->setName(instanceName);
			objectInstanceBase->setInstanceCountMax(maxInstances);
			objectInstanceBase->setFlagUpdateFromBase(true);
			((InstanceShapeCube *)objectInstanceBase)->loadBuffers();
			mObjectHandler.addObjectInstanceBase(objectInstanceBase);
			break;

		default:
			break;
	}
}

TransformObject *DX10Module::createObjectShape(ShapeTypes::Types type)
{
	TransformObject *object = NULL;

	switch(type)
	{
		case ShapeTypes::TRIANGLE:
			object = myNew ShapeTriangle(mWindowHandler.getDevice(), mShaderHandler.getEffect("BasicColor"));
			mObjectHandler.addObject(object);
			break;

		case ShapeTypes::CUBE:
			object = myNew ShapeCube(mWindowHandler.getDevice(), mShaderHandler.getEffect("BasicColor"));
			mObjectHandler.addObject(object);
			break;

		default:
			break;
	}

	return object;
}

TransformObject *DX10Module::createObjectInstanced(int instanceID)
{
	return mObjectHandler.addObjectFromInstance(instanceID);
}

TransformObject *DX10Module::createObjectInstanced(const char *instanceName)
{
	return mObjectHandler.addObjectFromInstance(instanceName);
}

Mesh *DX10Module::createMesh(int modelID)
{
	return mObjectHandler.addMeshFromModel(modelID);
}

Mesh *DX10Module::createMesh(const char *modelName)
{
	return mObjectHandler.addMeshFromModel(modelName);
}

InstanceMesh *DX10Module::createMeshInstanced(int instanceID)
{
	return mObjectHandler.addMeshFromInstance(instanceID);
}

InstanceMesh *DX10Module::createMeshInstanced(const char *instanceName)
{
	return mObjectHandler.addMeshFromInstance(instanceName);
}

RenderObject *DX10Module::getRenderObject(int objectID)
{
	return mObjectHandler.getObject(objectID);
}

RenderObject *DX10Module::getRenderObject(const char *objectName)
{
	return mObjectHandler.getObject(objectName);
}

void DX10Module::removeRenderObject(RenderObject *object)
{
	mObjectHandler.removeObject(object);
}

void DX10Module::removeRenderObject(int objectID)
{
	mObjectHandler.removeObject(objectID);
}

void DX10Module::removeRenderObject(const char *objectName)
{
	mObjectHandler.removeObject(objectName);
}

void DX10Module::copyBoundingVolumes(std::vector<CollisionObject *> *pBVs)
{
	mObjectHandler.copyBoundingVolumes(pBVs);
}

HRESULT DX10Module::loadShaders()
{
	if(FAILED(mShaderHandler.addEffect("../../Shaders/BasicCel.fx", "BasicColor")))
	{
		return E_FAIL;
	}
	mShaderHandler.getEffect("BasicColor")->createInputLayout(0, VertexTypes::VERTEX_TRANSFORM);

	return S_OK;
}

void DX10Module::updateShaderOnceBuffers()
{

}

void DX10Module::updateShaderPerFrameBuffers()
{
	D3DXMATRIX MV;
	D3DXMATRIX MP;
	mCamera->GetVPMatrices(MV, MP);
	mShaderHandler.getEffect("BasicColor")->updateMVP(MV * MP);
	mShaderHandler.getEffect("BasicColor")->updateVCameraPosition(mCamera->getPosition());
	mShaderHandler.getEffect("BasicColor")->updateVCameraDirection(mCamera->getTarget() - mCamera->getPosition());
}