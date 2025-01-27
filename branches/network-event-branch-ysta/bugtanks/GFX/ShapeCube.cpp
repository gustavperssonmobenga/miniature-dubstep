/**
 * @file
 * @author Victor Lundberg <lundberg.victor@gmail.com>
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

#include "ShapeCube.h"

using namespace gfx;

ShapeCube::ShapeCube(ID3D10Device *pDevice, Effect *pEffect) : BufferObject(pDevice, pEffect)
{
	createVertexBufferStatic();
	createVertexBufferDynamic();
	createIndexBuffer();
	setBoundingVolume(myNew Sphere(0.0f, 0.0f, 0.0f, 1.0f, -1));
}

ShapeCube::ShapeCube(const ShapeCube &cube) : BufferObject(cube)
{
	setBoundingVolume(myNew Sphere(0.0f, 0.0f, 0.0f, 1.0f, -1));
}

ShapeCube::~ShapeCube()
{

}
	
const ShapeCube &ShapeCube::operator=(const ShapeCube &cube)
{
	if(this != &cube)
	{
		BufferObject::operator=(cube);
	}
	return *this;
}

RenderObject *ShapeCube::clone()
{
	return myNew ShapeCube(*this);
}

void ShapeCube::update()
{
	BufferObject::update();
}

bool ShapeCube::updateVertexBufferDynamic()
{
	VertexTransformDynamic *pData;
	getVertexBufferDynamic()->Map(D3D10_MAP_WRITE_DISCARD, 0, (void **)&pData);
	for(unsigned int n = 0; n < getVertexCount(); ++n)
	{
		pData[n].textureID = getTextureID();
		pData[n].color = getColor();
		pData[n].transform = getMatrixTransform();
	}
	getVertexBufferDynamic()->Unmap();
	return true;
}

void ShapeCube::createVertexBufferStatic()
{
	setVertexCount(24);

	VertexTransformStatic vertices[24];

	//Front faces.
	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	vertices[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertices[0].tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertices[0].UV = D3DXVECTOR2(0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(-1.0f,  1.0f, -1.0f);
	vertices[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertices[1].tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertices[1].UV = D3DXVECTOR2(0.0f, 0.0f);

	vertices[2].position = D3DXVECTOR3(1.0f,  1.0f, -1.0f);
	vertices[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertices[2].tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertices[2].UV = D3DXVECTOR2(1.0f, 0.0f);

	vertices[3].position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	vertices[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertices[3].tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertices[3].UV = D3DXVECTOR2(1.0f, 1.0f);

	//Back faces
	vertices[4].position = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	vertices[4].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertices[4].tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertices[4].UV = D3DXVECTOR2(1.0f, 1.0f);

	vertices[5].position = D3DXVECTOR3(1.0f,  -1.0f, 1.0f);
	vertices[5].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertices[5].tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertices[5].UV = D3DXVECTOR2(0.0f, 1.0f);

	vertices[6].position = D3DXVECTOR3(1.0f,  1.0f, 1.0f);
	vertices[6].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertices[6].tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertices[6].UV = D3DXVECTOR2(0.0f, 0.0f);

	vertices[7].position = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	vertices[7].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertices[7].tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertices[7].UV = D3DXVECTOR2(1.0f, 0.0f);

	//Bottom faces.
	vertices[8].position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	vertices[8].normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	vertices[8].tangent = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	vertices[8].UV = D3DXVECTOR2(1.0f, 1.0f);

	vertices[9].position = D3DXVECTOR3(1.0f,  -1.0f, -1.0f);
	vertices[9].normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	vertices[9].tangent = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	vertices[9].UV = D3DXVECTOR2(0.0f, 1.0f);

	vertices[10].position = D3DXVECTOR3(1.0f,  -1.0f, 1.0f);
	vertices[10].normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	vertices[10].tangent = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	vertices[10].UV = D3DXVECTOR2(0.0f, 0.0f);

	vertices[11].position = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	vertices[11].normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	vertices[11].tangent = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	vertices[11].UV = D3DXVECTOR2(1.0f, 0.0f);

	//Top faces.
	vertices[12].position = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	vertices[12].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertices[12].tangent = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertices[12].UV = D3DXVECTOR2(0.0f, 1.0f);

	vertices[13].position = D3DXVECTOR3(-1.0f,  1.0f, 1.0f);
	vertices[13].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertices[13].tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertices[13].UV = D3DXVECTOR2(0.0f, 0.0f);

	vertices[14].position = D3DXVECTOR3(1.0f,  1.0f, 1.0f);
	vertices[14].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertices[14].tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertices[14].UV = D3DXVECTOR2(1.0f, 0.0f);

	vertices[15].position = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	vertices[15].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertices[15].tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertices[15].UV = D3DXVECTOR2(1.0f, 1.0f);

	//Left faces.
	vertices[16].position = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	vertices[16].normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	vertices[16].tangent = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	vertices[16].UV = D3DXVECTOR2(0.0f, 1.0f);

	vertices[17].position = D3DXVECTOR3(-1.0f,  1.0f, 1.0f);
	vertices[17].normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	vertices[17].tangent = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	vertices[17].UV = D3DXVECTOR2(0.0f, 0.0f);

	vertices[18].position = D3DXVECTOR3(-1.0f,  1.0f, -1.0f);
	vertices[18].normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	vertices[18].tangent = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	vertices[18].UV = D3DXVECTOR2(1.0f, 0.0f);

	vertices[19].position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	vertices[19].normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	vertices[19].tangent = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	vertices[19].UV = D3DXVECTOR2(1.0f, 1.0f);

	//Right faces.
	vertices[20].position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	vertices[20].normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertices[20].tangent = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertices[20].UV = D3DXVECTOR2(0.0f, 1.0f);

	vertices[21].position = D3DXVECTOR3(1.0f,  1.0f, -1.0f);
	vertices[21].normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertices[21].tangent = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertices[21].UV = D3DXVECTOR2(0.0f, 0.0f);

	vertices[22].position = D3DXVECTOR3(1.0f,  1.0f, 1.0f);
	vertices[22].normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertices[22].tangent = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertices[22].UV = D3DXVECTOR2(1.0f, 0.0f);

	vertices[23].position = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	vertices[23].normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertices[23].tangent = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertices[23].UV = D3DXVECTOR2(1.0f, 1.0f);

	D3D10_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = getEffect()->getInputLayoutHandler().getInputLayout(getEffectTechIndex())->getLayoutSizeStatic() * getVertexCount();
	bufferDesc.Usage = D3D10_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA bufferData;
	bufferData.pSysMem = vertices;
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;

	if(FAILED(getDevice()->CreateBuffer(&bufferDesc, &bufferData, getVertexBufferStaticPointer())))
	{
		DEBUG_MESSAGE("Error: GFX: ShapeCube: createVertexBufferStatic(). Error creating vertex buffer.")
	}
}

void ShapeCube::createVertexBufferDynamic()
{
	setVertexCount(24);

	VertexTransformDynamic vertices[24];

	//Front faces.
	vertices[0].textureID = -1;
	vertices[0].color = getColor();
	vertices[0].transform = getMatrixTransform();

	vertices[1].textureID = -1;
	vertices[1].color = getColor();
	vertices[1].transform = getMatrixTransform();

	vertices[2].textureID = -1;
	vertices[2].color = getColor();
	vertices[2].transform = getMatrixTransform();

	vertices[3].textureID = -1;
	vertices[3].color = getColor();
	vertices[3].transform = getMatrixTransform();

	//Back faces
	vertices[4].textureID = -1;
	vertices[4].color = getColor();
	vertices[4].transform = getMatrixTransform();

	vertices[5].textureID = -1;
	vertices[5].color = getColor();
	vertices[5].transform = getMatrixTransform();

	vertices[6].textureID = -1;
	vertices[6].color = getColor();
	vertices[6].transform = getMatrixTransform();

	vertices[7].textureID = -1;
	vertices[7].color = getColor();
	vertices[7].transform = getMatrixTransform();

	//Bottom faces.
	vertices[8].textureID = -1;
	vertices[8].color = getColor();
	vertices[8].transform = getMatrixTransform();

	vertices[9].textureID = -1;
	vertices[9].color = getColor();
	vertices[9].transform = getMatrixTransform();

	vertices[10].textureID = -1;
	vertices[10].color = getColor();
	vertices[10].transform = getMatrixTransform();

	vertices[11].textureID = -1;
	vertices[11].color = getColor();
	vertices[11].transform = getMatrixTransform();

	//Top faces.
	vertices[12].textureID = -1;
	vertices[12].color = getColor();
	vertices[12].transform = getMatrixTransform();

	vertices[13].textureID = -1;
	vertices[13].color = getColor();
	vertices[13].transform = getMatrixTransform();

	vertices[14].textureID = -1;
	vertices[14].color = getColor();
	vertices[14].transform = getMatrixTransform();

	vertices[15].textureID = -1;
	vertices[15].color = getColor();
	vertices[15].transform = getMatrixTransform();

	//Left faces.
	vertices[16].textureID = -1;
	vertices[16].color = getColor();
	vertices[16].transform = getMatrixTransform();

	vertices[17].textureID = -1;
	vertices[17].color = getColor();
	vertices[17].transform = getMatrixTransform();

	vertices[18].textureID = -1;
	vertices[18].color = getColor();
	vertices[18].transform = getMatrixTransform();

	vertices[19].textureID = -1;
	vertices[19].color = getColor();
	vertices[19].transform = getMatrixTransform();

	//Right faces.
	vertices[20].textureID = -1;
	vertices[20].color = getColor();
	vertices[20].transform = getMatrixTransform();

	vertices[21].textureID = -1;
	vertices[21].color = getColor();
	vertices[21].transform = getMatrixTransform();

	vertices[22].textureID = -1;
	vertices[22].color = getColor();
	vertices[22].transform = getMatrixTransform();

	vertices[23].textureID = -1;
	vertices[23].color = getColor();
	vertices[23].transform = getMatrixTransform();

	D3D10_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = getEffect()->getInputLayoutHandler().getInputLayout(getEffectTechIndex())->getLayoutSizeDynamic() * getVertexCount();
	bufferDesc.Usage = D3D10_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA bufferData;
	bufferData.pSysMem = vertices;
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;

	if(FAILED(getDevice()->CreateBuffer(&bufferDesc, &bufferData, getVertexBufferDynamicPointer())))
	{
		DEBUG_MESSAGE("Error: GFX: ShapeCube: createVertexBufferDynamic(). Error creating vertex buffer.")
	}
}

void ShapeCube::createIndexBuffer()
{
	setIndexCount(72);

	unsigned int indices[72];

	//Front faces.
	indices[0] = 0;
	indices[1] = 16;
	indices[2] = 1;
	indices[3] = 14;
	indices[4] = 2;
	indices[5] = 3;

	indices[6] = 0;
	indices[7] = 1;
	indices[8] = 2;
	indices[9] = 22;
	indices[10] = 3;
	indices[11] = 10;

	//Back faces.
	indices[12] = 4;
	indices[13] = 8;
	indices[14] = 5;
	indices[15] = 20;
	indices[16] = 6;
	indices[17] = 7;

	indices[18] = 4;
	indices[19] = 5;
	indices[20] = 6;
	indices[21] = 12;
	indices[22] = 7;
	indices[23] = 18;

	//Bottom faces.
	indices[36] = 8;
	indices[37] = 2;
	indices[38] = 9;
	indices[39] = 22;
	indices[40] = 10;
	indices[41] = 11;

	indices[42] = 8;
	indices[43] = 9;
	indices[44] = 10;
	indices[45] = 6;
	indices[46] = 11;
	indices[47] = 18;

	//Top faces.
	indices[24] = 12;
	indices[25] = 16;
	indices[26] = 13;
	indices[27] = 4;
	indices[28] = 14;
	indices[29] = 15;

	indices[30] = 12;
	indices[31] = 13;
	indices[32] = 14;
	indices[33] = 20;
	indices[34] = 15;
	indices[35] = 0;

	//Left faces.
	indices[48] = 16;
	indices[49] = 6;
	indices[50] = 17;
	indices[51] = 14;
	indices[52] = 18;
	indices[53] = 19;

	indices[54] = 16;
	indices[55] = 17;
	indices[56] = 18;
	indices[57] = 2;
	indices[58] = 19;
	indices[59] = 10;

	//Right faces.
	indices[60] = 20;
	indices[61] = 0;
	indices[62] = 21;
	indices[63] = 12;
	indices[64] = 22;
	indices[65] = 23;

	indices[66] = 20;
	indices[67] = 21;
	indices[68] = 22;
	indices[69] = 4;
	indices[70] = 23;
	indices[71] = 8;

	D3D10_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(unsigned int) * getIndexCount();
	bufferDesc.Usage = D3D10_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA bufferData;
	bufferData.pSysMem = indices;
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;

	if(FAILED(getDevice()->CreateBuffer(&bufferDesc, &bufferData, getIndexBufferPointer())))
	{
		DEBUG_MESSAGE("Error: GFX: ShapeCube: createIndexBuffer(). Error creating index buffer.")
	}
}