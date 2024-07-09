#include <filesystem>
#include <wrl.h>
#include <WICTextureLoader.h>
//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Misc.h"
#include "GpuResourceUtils.h"

// ���_�V�F�[�_�[�ǂݍ���
HRESULT GpuResourceUtils::LoadVertexShader(
	ID3D11Device* device,
	const char* filename,
	const D3D11_INPUT_ELEMENT_DESC inputElementDescs[],
	UINT inputElementCount,
	ID3D11InputLayout** inputLayout,
	ID3D11VertexShader** vertexShader)
{
	// �t�@�C�����J��
	FILE* fp = nullptr;
	fopen_s(&fp, filename, "rb");
	_ASSERT_EXPR_A(fp, "Vertex Shader File not found");

	// �t�@�C���̃T�C�Y�����߂�
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// ��������ɒ��_�V�F�[�_�[�f�[�^���i�[����̈��p�ӂ���
	std::unique_ptr<u_char[]> data = std::make_unique<u_char[]>(size);
	fread(data.get(), size, 1, fp);
	fclose(fp);

	// ���_�V�F�[�_�[����
	HRESULT hr = device->CreateVertexShader(data.get(), size, nullptr, vertexShader);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// ���̓��C�A�E�g
	if (inputLayout != nullptr)
	{
		hr = device->CreateInputLayout(inputElementDescs, inputElementCount, data.get(), size, inputLayout);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	return hr;
}

// �s�N�Z���V�F�[�_�[�ǂݍ���
HRESULT GpuResourceUtils::LoadPixelShader(
	ID3D11Device* device,
	const char* filename,
	ID3D11PixelShader** pixelShader)
{
	// �t�@�C�����J��
	FILE* fp = nullptr;
	fopen_s(&fp, filename, "rb");
	_ASSERT_EXPR_A(fp, "Pixel Shader File not found");

	// �t�@�C���̃T�C�Y�����߂�
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// ��������ɒ��_�V�F�[�_�[�f�[�^���i�[����̈��p�ӂ���
	std::unique_ptr<u_char[]> data = std::make_unique<u_char[]>(size);
	fread(data.get(), size, 1, fp);
	fclose(fp);

	// �s�N�Z���V�F�[�_�[����
	HRESULT hr = device->CreatePixelShader(data.get(), size, nullptr, pixelShader);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	return hr;
}

//HRESULT ModelResource::LoadTexture(ID3D11Device* device, const char* filename, const char* suffix, bool dummy, ID3D11ShaderResourceView** srv, UINT dummy_color)
//{
//	// �p�X�𕪉�
//	char drive[256], dirname[256], fname[256], ext[256];
//	::_splitpath_s(filename, drive, sizeof(drive), dirname, sizeof(dirname), fname, sizeof(fname), ext, sizeof(ext));
//
//	// ����������ǉ�
//	if (suffix != nullptr)
//	{
//		::strcat_s(fname, sizeof(fname), suffix);
//	}
//	// �p�X������
//	char filepath[256];
//	::_makepath_s(filepath, 256, drive, dirname, fname, ext);
//
//	// �}���`�o�C�g�������烏�C�h�����֕ϊ�
//	wchar_t wfilepath[256];
//	::MultiByteToWideChar(CP_ACP, 0, filepath, -1, wfilepath, 256);
//
//	// �e�N�X�`���ǂݍ���
//	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
//	HRESULT hr = DirectX::CreateWICTextureFromFile(device, wfilepath, resource.GetAddressOf(), srv);
//	if (FAILED(hr))
//	{
//		// WIC�ŃT�|�[�g����Ă��Ȃ��t�H�[�}�b�g�̏ꍇ�iTGA�Ȃǁj��
//		// STB�ŉ摜�ǂݍ��݂����ăe�N�X�`���𐶐�����
//		int width, height, bpp;
//		unsigned char* pixels = stbi_load(filepath, &width, &height, &bpp, STBI_rgb_alpha);
//		if (pixels != nullptr)
//		{
//			D3D11_TEXTURE2D_DESC desc = { 0 };
//			desc.Width = width;
//			desc.Height = height;
//			desc.MipLevels = 1;
//			desc.ArraySize = 1;
//			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//			desc.SampleDesc.Count = 1;
//			desc.SampleDesc.Quality = 0;
//			desc.Usage = D3D11_USAGE_DEFAULT;
//			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//			desc.CPUAccessFlags = 0;
//			desc.MiscFlags = 0;
//			D3D11_SUBRESOURCE_DATA data;
//			::memset(&data, 0, sizeof(data));
//			data.pSysMem = pixels;
//			data.SysMemPitch = width * 4;
//
//			Microsoft::WRL::ComPtr<ID3D11Texture2D>	texture;
//			hr = device->CreateTexture2D(&desc, &data, texture.GetAddressOf());
//			_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//
//			hr = device->CreateShaderResourceView(texture.Get(), nullptr, srv);
//			_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//
//			// ��n��
//			stbi_image_free(pixels);
//		}
//		else if (dummy)
//		{
//			// �ǂݍ��ݎ��s������_�~�[�e�N�X�`�������
//			LOG("load failed : %s\n", filepath);
//
//			const int width = 8;
//			const int height = 8;
//			UINT pixels[width * height];
//			for (int yy = 0; yy < height; ++yy)
//			{
//				for (int xx = 0; xx < width; ++xx)
//				{
//					pixels[yy * width + xx] = dummy_color;
//				}
//			}
//
//			D3D11_TEXTURE2D_DESC desc = { 0 };
//			desc.Width = width;
//			desc.Height = height;
//			desc.MipLevels = 1;
//			desc.ArraySize = 1;
//			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//			desc.SampleDesc.Count = 1;
//			desc.SampleDesc.Quality = 0;
//			desc.Usage = D3D11_USAGE_DEFAULT;
//			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//			desc.CPUAccessFlags = 0;
//			desc.MiscFlags = 0;
//			D3D11_SUBRESOURCE_DATA data;
//			::memset(&data, 0, sizeof(data));
//			data.pSysMem = pixels;
//			data.SysMemPitch = width;
//
//			Microsoft::WRL::ComPtr<ID3D11Texture2D>	texture;
//			hr = device->CreateTexture2D(&desc, &data, texture.GetAddressOf());
//			_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//
//			hr = device->CreateShaderResourceView(texture.Get(), nullptr, srv);
//			_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//		}
//	}
//	return hr;
//}
//// �e�N�X�`���ǂݍ���
//HRESULT GpuResourceUtils::LoadTexture(
//	ID3D11Device* device,
//	const char* filename,
//	ID3D11ShaderResourceView** shaderResourceView,
//	D3D11_TEXTURE2D_DESC* texture2dDesc)
//{
//	// �g���q���擾
//	std::filesystem::path filepath(filename);
//	std::string extension = filepath.extension().string();
//	std::transform(extension.begin(), extension.end(), extension.begin(), tolower);	// ��������
//
//	// ���C�h�����ɕϊ�
//	std::wstring wfilename = filepath.wstring();
//
//	// �t�H�[�}�b�g���ɉ摜�ǂݍ��ݏ���
//	HRESULT hr;
//	DirectX::TexMetadata metadata;
//	DirectX::ScratchImage scratch_image;
//	if (extension == ".tga")
//	{
//		hr = DirectX::GetMetadataFromTGAFile(wfilename.c_str(), metadata);
//		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//
//		hr = DirectX::LoadFromTGAFile(wfilename.c_str(), &metadata, scratch_image);
//		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//	}
//	else if (extension == ".dds")
//	{
//		hr = DirectX::GetMetadataFromDDSFile(wfilename.c_str(), DirectX::DDS_FLAGS_NONE, metadata);
//		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//
//		hr = DirectX::LoadFromDDSFile(wfilename.c_str(), DirectX::DDS_FLAGS_NONE, &metadata, scratch_image);
//		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//	}
//	else if (extension == ".hdr")
//	{
//		hr = DirectX::GetMetadataFromHDRFile(wfilename.c_str(), metadata);
//		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//
//		hr = DirectX::LoadFromHDRFile(wfilename.c_str(), &metadata, scratch_image);
//		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//	}
//	else
//	{
//		hr = DirectX::GetMetadataFromWICFile(wfilename.c_str(), DirectX::WIC_FLAGS_NONE, metadata);
//		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//
//		hr = DirectX::LoadFromWICFile(wfilename.c_str(), DirectX::WIC_FLAGS_NONE, &metadata, scratch_image);
//		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//	}
//
//	// �V�F�[�_�[���\�[�X�r���[�쐬
//	hr = DirectX::CreateShaderResourceView(device, scratch_image.GetImages(), scratch_image.GetImageCount(),
//		metadata, shaderResourceView);
//	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//
//	// �e�N�X�`�����擾
//	if (texture2dDesc != nullptr)
//	{
//		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
//		(*shaderResourceView)->GetResource(resource.GetAddressOf());
//
//		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
//		hr = resource->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
//		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//		texture2d->GetDesc(texture2dDesc);
//	}
//	return hr;
//}
//
//// �e�N�X�`���ǂݍ���
//HRESULT GpuResourceUtils::LoadTexture(
//	ID3D11Device* device,
//	const void* data,
//	size_t size,
//	ID3D11ShaderResourceView** shaderResourceView,
//	D3D11_TEXTURE2D_DESC* texture2dDesc)
//{
//	// �t�H�[�}�b�g���ɉ摜�ǂݍ��ݏ���
//	HRESULT hr = E_FAIL;
//	DirectX::TexMetadata metadata;
//	DirectX::ScratchImage scratch_image;
//
//	// .tga
//	{
//		hr = DirectX::GetMetadataFromTGAMemory(data, size, metadata);
//		if (SUCCEEDED(hr))
//		{
//			hr = DirectX::LoadFromTGAMemory(data, size, &metadata, scratch_image);
//		}
//	}
//	// .dds
//	if (FAILED(hr))
//	{
//		hr = DirectX::GetMetadataFromDDSMemory(data, size, DirectX::DDS_FLAGS_NONE, metadata);
//		if (SUCCEEDED(hr))
//		{
//			hr = DirectX::LoadFromDDSMemory(data, size, DirectX::DDS_FLAGS_NONE, &metadata, scratch_image);
//		}
//	}
//	// .hdr
//	if (FAILED(hr))
//	{
//		hr = DirectX::GetMetadataFromHDRMemory(data, size, metadata);
//		if (SUCCEEDED(hr))
//		{
//			hr = DirectX::LoadFromHDRMemory(data, size, &metadata, scratch_image);
//		}
//	}
//	if (FAILED(hr))
//	{
//		hr = DirectX::GetMetadataFromWICMemory(data, size, DirectX::WIC_FLAGS_NONE, metadata);
//		if (SUCCEEDED(hr))
//		{
//			hr = DirectX::LoadFromWICMemory(data, size, DirectX::WIC_FLAGS_NONE, &metadata, scratch_image);
//		}
//	}
//	if (FAILED(hr))
//	{
//		return hr;
//	}
//
//	// �V�F�[�_�[���\�[�X�r���[�쐬
//	hr = DirectX::CreateShaderResourceView(device, scratch_image.GetImages(), scratch_image.GetImageCount(),
//		metadata, shaderResourceView);
//	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//
//	// �e�N�X�`�����擾
//	if (texture2dDesc != nullptr)
//	{
//		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
//		(*shaderResourceView)->GetResource(resource.GetAddressOf());
//
//		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
//		hr = resource->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
//		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//		texture2d->GetDesc(texture2dDesc);
//	}
//	return hr;
//}

// �_�~�[�e�N�X�`���쐬
HRESULT GpuResourceUtils::CreateDummyTexture(
	ID3D11Device* device,
	UINT color,
	ID3D11ShaderResourceView** shaderResourceView,
	D3D11_TEXTURE2D_DESC* texture2dDesc)
{
	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = 1;
	desc.Height = 1;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = &color;
	data.SysMemPitch = desc.Width;

	Microsoft::WRL::ComPtr<ID3D11Texture2D>	texture;
	HRESULT hr = device->CreateTexture2D(&desc, &data, texture.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	hr = device->CreateShaderResourceView(texture.Get(), nullptr, shaderResourceView);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// �e�N�X�`�����擾
	if (texture2dDesc != nullptr)
	{
		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
		(*shaderResourceView)->GetResource(resource.GetAddressOf());

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
		hr = resource->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		texture2d->GetDesc(texture2dDesc);
	}

	return hr;
}

// �萔�o�b�t�@�쐬
HRESULT GpuResourceUtils::CreateConstantBuffer(
	ID3D11Device* device,
	UINT bufferSize,
	ID3D11Buffer** constantBuffer)
{
	D3D11_BUFFER_DESC desc{};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.ByteWidth = bufferSize;
	desc.StructureByteStride = 0;

	HRESULT hr = device->CreateBuffer(&desc, 0, constantBuffer);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	return hr;
}
