#include "DataProcesser.h"
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>
#include "../Components/Game.h"

bool DataProcesser::isInitialized_ = false;
Game* DataProcesser::activeGame_;
std::unordered_map<std::string, DataProcesser::VertexShaderInfo> DataProcesser::vShaders_{};
std::unordered_map<std::string, DataProcesser::PixelShaderInfo> DataProcesser::pShaders_{};
std::unordered_map<const wchar_t*, DataProcesser::TextureInfo> DataProcesser::textures_{};
std::unordered_map<std::string, DataProcesser::GeometryInfo> DataProcesser::meshes_{};

void DataProcesser::LoadTexture(const wchar_t* name)
{
    auto res = DirectX::CreateDDSTextureFromFile(activeGame_->device_.Get(), name, &textures_[name].TextureBuffer, &textures_[name].TextureView);
}

void DataProcesser::LoadMesh(const std::string name)
{

    meshes_.insert({ name, { {}, {} } });

    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(name.c_str(), aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

    ProcessNode(name, pScene->mRootNode, pScene);
}

void DataProcesser::ProcessNode(const std::string name, aiNode* node, const aiScene* scene)
{
    for (UINT i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(name, mesh, scene);
    }
    for (UINT i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(name, node->mChildren[i], scene);
    }
}

void DataProcesser::ProcessMesh(const std::string name, aiMesh* mesh, const aiScene* scene)
{
    for (UINT i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex point;

        point.pos.x = mesh->mVertices[i].x;
        point.pos.y = mesh->mVertices[i].y;
        point.pos.z = mesh->mVertices[i].z;
        point.pos.w = 1.0f;

        if (mesh->mTextureCoords[0])
        {
            point.tex.x = mesh->mTextureCoords[0][i].x;
            point.tex.y = mesh->mTextureCoords[0][i].y;
        }

        point.normal.x = mesh->mNormals[i].x;
        point.normal.y = mesh->mNormals[i].y;
        point.normal.z = mesh->mNormals[i].z;
        point.normal.w = 0.0f;

        meshes_[name].Points.push_back(point);
    }

    for (UINT i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace face = mesh->mFaces[i];

        for (UINT j = 0; j < face.mNumIndices; j++)
            meshes_[name].Indices.push_back(face.mIndices[j]);
    }
}

void DataProcesser::Initialize(Game* game)
{
    if (isInitialized_)
        return;

    activeGame_ = game;
    isInitialized_ = true;

    vShaders_.insert({ "base", {nullptr, nullptr} });

    pShaders_.insert({ "base", {nullptr, nullptr} });

    ID3DBlob* errorVertexCode = nullptr;
    auto res = D3DCompileFromFile(L"./Shaders/Base3dShader.hlsl",
        nullptr /*macros*/,
        nullptr /*include*/,
        "VSMain",
        "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &(vShaders_["base"].Bc),
        &errorVertexCode);

    if (FAILED(res)) {
        // If the shader failed to compile it should have written something to the error message.
        if (errorVertexCode) {
            const char* compileErrors = static_cast<char*>(errorVertexCode->GetBufferPointer());

            std::cout << compileErrors << std::endl;
        }
        // If there was  nothing in the error message then it simply could not find the shader file itself.
        else
        {
            MessageBox(game->display_->hwnd_, L"Base3dShader.hlsl", L"Missing Shader File", MB_OK);
        }
        //return;
    }

    ID3DBlob* errorPixelCode = nullptr;
    res = D3DCompileFromFile(L"./Shaders/Base3dShader.hlsl",
        nullptr /*macros*/,
        nullptr /*include*/,
        "PSMain",
        "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &(pShaders_["base"].Bc),
        &errorPixelCode);

    res = game->device_->CreateVertexShader(
        vShaders_["base"].Bc->GetBufferPointer(),
        vShaders_["base"].Bc->GetBufferSize(),
        nullptr, &(vShaders_["base"].Shader));

    res = game->device_->CreatePixelShader(
        pShaders_["base"].Bc->GetBufferPointer(),
        pShaders_["base"].Bc->GetBufferSize(),
        nullptr, &(pShaders_["base"].Shader));

    vShaders_.insert({ "spinny", {nullptr, nullptr} });

    pShaders_.insert({ "spinny", {nullptr, nullptr} });

    errorVertexCode = nullptr;
    res = D3DCompileFromFile(L"./Shaders/SpinnyShader.hlsl",
        nullptr /*macros*/,
        nullptr /*include*/,
        "VSMain",
        "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &(vShaders_["spinny"].Bc),
        &errorVertexCode);

    if (FAILED(res)) {
        // If the shader failed to compile it should have written something to the error message.
        if (errorVertexCode) {
            const char* compileErrors = static_cast<char*>(errorVertexCode->GetBufferPointer());

            std::cout << compileErrors << std::endl;
        }
        // If there was  nothing in the error message then it simply could not find the shader file itself.
        else
        {
            MessageBox(game->display_->hwnd_, L"SpinnyShader.hlsl", L"Missing Shader File", MB_OK);
        }
        //return;
    }

    errorPixelCode = nullptr;
    res = D3DCompileFromFile(L"./Shaders/SpinnyShader.hlsl",
        nullptr /*macros*/,
        nullptr /*include*/,
        "PSMain",
        "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &(pShaders_["spinny"].Bc),
        &errorPixelCode);

    res = game->device_->CreateVertexShader(
        vShaders_["spinny"].Bc->GetBufferPointer(),
        vShaders_["spinny"].Bc->GetBufferSize(),
        nullptr, &(vShaders_["spinny"].Shader));

    res = game->device_->CreatePixelShader(
        pShaders_["spinny"].Bc->GetBufferPointer(),
        pShaders_["spinny"].Bc->GetBufferSize(),
        nullptr, &(pShaders_["spinny"].Shader));
}

void DataProcesser::DestroyResources()
{
    if (!isInitialized_)
        return;

    for (const auto t : textures_)
    {
        t.second.TextureBuffer->Release();
        t.second.TextureView->Release();
    }
    textures_.clear();
    for (const auto vs : vShaders_)
    {
        vs.second.Bc->Release();
        vs.second.Shader->Release();
    }
    vShaders_.clear();
    for (const auto ps : pShaders_)
    {
        ps.second.Bc->Release();
        ps.second.Shader->Release();
    }
    pShaders_.clear();
    meshes_.clear();

    isInitialized_ = false;
}

ID3D11Resource* DataProcesser::GetTextureBuffer(const wchar_t* name)
{
    if (textures_.find(name) != textures_.end())
        return textures_[name].TextureBuffer;

    LoadTexture(name);

    return textures_[name].TextureBuffer;
}

ID3D11ShaderResourceView* DataProcesser::GetTextureView(const wchar_t* name)
{
    if (textures_.find(name) != textures_.end())
        return textures_[name].TextureView;

    LoadTexture(name);

    return textures_[name].TextureView;
}

ID3D11VertexShader* DataProcesser::GetVertexShader(const std::string name)
{
    return vShaders_[name].Shader;
}

ID3D11PixelShader* DataProcesser::GetPixelShader(const std::string name)
{
    return pShaders_[name].Shader;
}

ID3DBlob* DataProcesser::GetVertexShaderBC(std::string name)
{
    return vShaders_[name].Bc;
}

ID3DBlob* DataProcesser::GetPixelShaderBC(std::string name)
{
    return pShaders_[name].Bc;
}

const std::vector<Vertex>& DataProcesser::GetPoints(std::string name)
{
    if (meshes_.find(name) != meshes_.end())
        return meshes_[name].Points;

    LoadMesh(name);

    return meshes_[name].Points;
}

const std::vector<UINT>& DataProcesser::GetIndices(std::string name)
{
    if (meshes_.find(name) != meshes_.end())
        return meshes_[name].Indices;

    LoadMesh(name);

    return meshes_[name].Indices;
}
