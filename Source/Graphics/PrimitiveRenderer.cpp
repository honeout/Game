#include "Misc.h"
#include "GpuResourceUtils.h"
#include "PrimitiveRenderer.h"

// コンストラクタ
PrimitiveRenderer::PrimitiveRenderer(ID3D11Device* device)
{
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	//// 頂点シェーダー
	//	// ファイルを開く
	//FILE* fp = nullptr;
	//fopen_s(&fp, "Shader/PrimitiveRendererVS.cso", "rb");
	//_ASSERT_EXPR_A(fp, "Vertex Shader File not found");

	//// ファイルのサイズを求める
	//fseek(fp, 0, SEEK_END);
	//long size = ftell(fp);
	//fseek(fp, 0, SEEK_SET);

	//// メモリ上に頂点シェーダーデータを格納する領域を用意する
	//std::unique_ptr<u_char[]> data = std::make_unique<u_char[]>(size);
	//fread(data.get(), size, 1, fp);
	//fclose(fp);

	//// 頂点シェーダー生成
	//HRESULT hr = device->CreateVertexShader(
	//	data.get(),
	//	size, 
	//	nullptr, 
	//	vertexShader.GetAddressOf());
	//_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//// 入力レイアウト
	//if (inputLayout.GetAddressOf() != nullptr)
	//{
	//	hr = device->CreateInputLayout(inputElementDesc, _countof(inputElementDesc), data.get(), size, inputLayout.GetAddressOf());
	//	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	//}




	GpuResourceUtils::LoadVertexShader(
		device,
		"Shader/PrimitiveRendererVS.cso",
		inputElementDesc,
		_countof(inputElementDesc),
		inputLayout.GetAddressOf(),
		vertexShader.GetAddressOf());

	// ピクセルシェーダー
	
	GpuResourceUtils::LoadPixelShader(
		device,
		"Shader/PrimitiveRendererPS.cso",
		pixelShader.GetAddressOf());


	//// ピクセルシェーダー
	//const char* filename = "Shader/PrimitiveRendererPS.cso";

	//// ファイルを開く
	///*FILE* */fp = nullptr;
	//fopen_s(&fp, filename, "rb");
	//_ASSERT_EXPR_A(fp, "Pixel Shader File not found");

	//// ファイルのサイズを求める
	//fseek(fp, 0, SEEK_END);
	///*long */size = ftell(fp);
	//fseek(fp, 0, SEEK_SET);

	//// メモリ上に頂点シェーダーデータを格納する領域を用意する
	///*std::unique_ptr<u_char[]> */data = std::make_unique<u_char[]>(size);
	//fread(data.get(), size, 1, fp);
	//fclose(fp);

	//// ピクセルシェーダー生成
	///*HRESULT*/ hr = device->CreatePixelShader(
	//	data.get(), 
	//	size, 
	//	nullptr,
	//	pixelShader.GetAddressOf());
	//_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//// 定数バッファ
	//GpuResourceUtils::CreateConstantBuffer(
	//	device,
	//	sizeof(CbScene),
	//	constantBuffer.GetAddressOf());


	// 定数バッファ
	{
		D3D11_BUFFER_DESC desc{};
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(CbScene);
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, constantBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	}


	// 頂点バッファ
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(Vertex) * VertexCapacity;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	HRESULT hr = device->CreateBuffer(&desc, nullptr, vertexBuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

// 頂点追加
void PrimitiveRenderer::AddVertex(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& color)
{
	Vertex& v = vertices.emplace_back();
	v.position = position;
	v.color = color;
}

// 軸描画
void PrimitiveRenderer::DrawAxis(const DirectX::XMFLOAT4X4& transform, const DirectX::XMFLOAT4& color)
{
	DirectX::XMMATRIX W = DirectX::XMLoadFloat4x4(&transform);
	DirectX::XMFLOAT3 p, x, y, z;
	DirectX::XMStoreFloat3(&p, W.r[3]);
	DirectX::XMStoreFloat3(&x, DirectX::XMVector3Transform(DirectX::XMVectorSet(1, 0, 0, 0), W));
	DirectX::XMStoreFloat3(&y, DirectX::XMVector3Transform(DirectX::XMVectorSet(0, 1, 0, 0), W));
	DirectX::XMStoreFloat3(&z, DirectX::XMVector3Transform(DirectX::XMVectorSet(0, 0, 1, 0), W));
	AddVertex(p, { 1, 0, 0, 1 });
	AddVertex(x, { 1, 0, 0, 1 });
	AddVertex(p, { 0, 1, 0, 1 });
	AddVertex(y, { 0, 1, 0, 1 });
	AddVertex(p, { 0, 0, 1, 1 });
	AddVertex(z, { 0, 0, 1, 1 });
}

// グリッド描画
void PrimitiveRenderer::DrawGrid(int subdivisions, float scale)
{
	int numLines = (subdivisions + 1) * 2;
	int vertexCount = numLines * 2;

	float corner = 0.5f;
	float step = 1.0f / static_cast<float>(subdivisions);

	int index = 0;
	float s = -corner;

	const DirectX::XMFLOAT4 white = DirectX::XMFLOAT4(1, 1, 1, 1);

	// Create vertical lines
	float scaling = static_cast<float>(subdivisions) * scale;
	DirectX::XMMATRIX M = DirectX::XMMatrixScaling(scaling, scaling, scaling);
	DirectX::XMVECTOR V, P;
	DirectX::XMFLOAT3 position;
	for (int i = 0; i <= subdivisions; i++)
	{
		V = DirectX::XMVectorSet(s, 0, corner, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		AddVertex(position, white);

		V = DirectX::XMVectorSet(s, 0, -corner, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		AddVertex(position, white);

		s += step;
	}

	// Create horizontal lines
	s = -corner;
	for (int i = 0; i <= subdivisions; i++)
	{
		V = DirectX::XMVectorSet(corner, 0, s, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		AddVertex(position, white);

		V = DirectX::XMVectorSet(-corner, 0, s, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		AddVertex(position, white);

		s += step;
	}

	// X軸
	{
		const DirectX::XMFLOAT4 red = DirectX::XMFLOAT4(1, 0, 0, 1);
		V = DirectX::XMVectorSet(0, 0, 0, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		AddVertex(position, red);

		V = DirectX::XMVectorSet(corner, 0, 0, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		AddVertex(position, red);
	}

	// Y軸
	{
		const DirectX::XMFLOAT4 green = DirectX::XMFLOAT4(0, 1, 0, 1);
		V = DirectX::XMVectorSet(0, 0, 0, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		AddVertex(position, green);

		V = DirectX::XMVectorSet(0, corner, 0, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		AddVertex(position, green);
	}

	// Z軸
	{
		const DirectX::XMFLOAT4 blue = DirectX::XMFLOAT4(0, 0, 1, 1);
		V = DirectX::XMVectorSet(0, 0, 0, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		AddVertex(position, blue);

		V = DirectX::XMVectorSet(0, 0, corner, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		AddVertex(position, blue);
	}
}

// 描画実行
void PrimitiveRenderer::Render(
	ID3D11DeviceContext* dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection,
	D3D11_PRIMITIVE_TOPOLOGY primitiveTopology)
{
	// シェーダー設定
	dc->VSSetShader(vertexShader.Get(), nullptr, 0);
	dc->PSSetShader(pixelShader.Get(), nullptr, 0);
	dc->IASetInputLayout(inputLayout.Get());

	// 定数バッファ設定
	dc->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

	// ビュープロジェクション行列作成
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX VP = V * P;

	// 定数バッファ更新
	CbScene cbScene;
	DirectX::XMStoreFloat4x4(&cbScene.viewProjection, VP);
	dc->UpdateSubresource(constantBuffer.Get(), 0, 0, &cbScene, 0, 0);

	// 頂点バッファ設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	dc->IASetPrimitiveTopology(primitiveTopology);
	dc->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	// 描画
	UINT totalVertexCount = static_cast<UINT>(vertices.size());
	UINT start = 0;
	UINT count = (totalVertexCount < VertexCapacity) ? totalVertexCount : VertexCapacity;

	while (start < totalVertexCount)
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		HRESULT hr = dc->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		memcpy(mappedSubresource.pData, &vertices[start], sizeof(Vertex) * count);

		dc->Unmap(vertexBuffer.Get(), 0);

		dc->Draw(count, 0);

		start += count;
		if ((start + count) > totalVertexCount)
		{
			count = totalVertexCount - start;
		}
	}

	vertices.clear();
}
