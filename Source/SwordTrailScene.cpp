#include <imgui.h>
#include "Graphics/Graphics.h"
#include "SwordTrailScene.h"
#include "Graphics/PrimitiveRenderer.h"
#include "PlayerManager.h"
#include "Graphics\RenderContext.h"
#include "Graphics\Graphics.h"
#include "RenderState.h"
#include "PlayerDual.h"
#include "EnemyManager.h"
#include "Misc.h"


void SwordTrailScene::Initialize()
{

	ID3D11Device* device = Graphics::Instance().GetDevice();
	//float screenWidth = Graphics::Instance().GetScreenWidth();
	//float screenHeight = Graphics::Instance().GetScreenHeight();

	//// カメラ設定
	//camera.SetPerspectiveFov(
	//	DirectX::XMConvertToRadians(45),	// 画角
	//	screenWidth / screenHeight,			// 画面アスペクト比
	//	0.1f,								// ニアクリップ
	//	1000.0f								// ファークリップ
	//);
	//camera.SetLookAt(
	//	{ 8, 3, 0 },		// 視点
	//	{ 0, 2, 0 },		// 注視点
	//	{ 0, 1, 0 }			// 上ベクトル
	//);


	// 合成なし
	{
		D3D11_BLEND_DESC desc{};
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = false;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		HRESULT hr = device->CreateBlendState(&desc,
			blendStates[0].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}


	// 深度テストあり＆深度書き込みあり
	{
		D3D11_DEPTH_STENCIL_DESC desc{};
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		HRESULT hr = device->CreateDepthStencilState(&desc,
			depthStencilStates[0].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}


	// ベタ塗り＆カリングなし
	{
		D3D11_RASTERIZER_DESC desc{};
		desc.FrontCounterClockwise = false;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.AntialiasedLineEnable = false;
		HRESULT hr = device->CreateRasterizerState(&desc,
			rasterizerStates[0].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	//// ベタ塗り＆裏面カリング
	//{
	//	D3D11_RASTERIZER_DESC desc{};
	//	desc.FrontCounterClockwise = false;
	//	desc.DepthBias = 0;
	//	desc.DepthBiasClamp = 0;
	//	desc.SlopeScaledDepthBias = 0;
	//	desc.DepthClipEnable = true;
	//	desc.ScissorEnable = false;
	//	desc.MultisampleEnable = true;
	//	desc.FillMode = D3D11_FILL_SOLID;
	//	desc.CullMode = D3D11_CULL_BACK;
	//	desc.AntialiasedLineEnable = false;
	//	HRESULT hr = device->CreateRasterizerState(&desc,
	//		rasterizerStates[0].GetAddressOf());
	//	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	//}



}

// 更新処理
void SwordTrailScene::Update(float elapsedTime)
{
	PlayerDual* playerDual = new PlayerDual;

	// TODO②:保存していた頂点バッファを１フレーム分ずらせ
	{

		// 一つ後ろにずらす
		for (int i = MAX_POLYGON - 1; i > -1; --i)
		{
			trailPositions[0][i] = trailPositions[0][i - 1];
	        trailPositions[1][i] = trailPositions[1][i - 1];
		}

		
	}

	// TODO①:剣の根本と先端の座標を取得し、頂点バッファに保存せよ
	// trailPositions[2][MAX_POLYGON] ← 頂点バッファ
	{
		// 剣の原点から根本と先端までのオフセット値
		DirectX::XMVECTOR RootOffset = DirectX::XMVectorSet(0, 0, 0.3f, 0);
		// 先端
		DirectX::XMVECTOR TipOffset = DirectX::XMVectorSet(0, 0, 2.3f, 0);
		
		
		 ////剣手元
   //      Model::Node* nodeRoot = PlayerRobot::Instance().GetModel()->FindNode("player_left_arm2");
   //      //Model::Node* nodeRoot = PlayerRobot::Instance().GetModel()->FindNode("mixamorig:LeftHand");
   //      // 剣先
   //      Model::Node* nodeTip = PlayerRobot::Instance().GetModel()->FindNode("player_right_arm2");
   //      //Model::Node* nodeTip = PlayerRobot::Instance().GetModel()->FindNode("mixamorig:RightHand");




		// 剣手元
		//Model::Node* nodeRoot = PlayerRobot::Instance().GetModel()->FindNode("player_left_arm2");
		////Model::Node* nodeRoot = PlayerRobot::Instance().GetModel()->FindNode("mixamorig:LeftHand");
		//// 剣先
		//Model::Node* nodeTip = PlayerRobot::Instance().GetModel()->FindNode("player_right_arm2");
		////Model::Node* nodeTip = PlayerRobot::Instance().GetModel()->FindNode("mixamorig:RightHand");


		// 剣の手元
		//DirectX::XMFLOAT3 wpos = {
		//		-3,
		//		-8,
		//		-3 };	
		const DirectX::XMFLOAT3& playerPosition = PlayerManager::Instance().GetPlayer(0)->GetPosition();
		DirectX::XMFLOAT3 pos = {
	    playerPosition.x,
		playerPosition.y,
		playerPosition.z };

		
		//const DirectX::XMFLOAT3& playerPosition2 = PlayerManager::Instance().GetPlayer(0)->AddImpulse;
		//DirectX::XMFLOAT3 pos2 = {
  //      playerPosition.x,
  //      playerPosition.y,
  //      playerPosition.z };

		//DirectX::XMFLOAT3 pos = { 
		//	playerDual->GetPosition().x,
		//			playerDual->GetPosition().y,
		//		playerDual->GetPosition().z };
		
		wpos = {
				pos.x,
				pos.y,
				pos.z };
		//DirectX::XMFLOAT3 wpos = {
		//		handNodeR.position.x,
		//		handNodeR.position.y,
		//		handNodeR.position.z };
		//DirectX::XMFLOAT3 wpos = {
		//handNode.position.x,
		//handNode.position.y,
		//handNode.position.z };


		DirectX::XMStoreFloat3(&trailPositions[0][0],DirectX::XMLoadFloat3(&wpos));



		//const DirectX::XMFLOAT3& enemyPosition = EnemyManager::Instance().GetEnemy(0)->GetPosition();
		//DirectX::XMFLOAT3 pose = {
		//enemyPosition.x,
		//enemyPosition.y,
		//enemyPosition.z };

		// 剣先
		wposL = {
				pos.x+3,
				pos.y +3,
				pos.z };
		
		//DirectX::XMFLOAT3 wposL = {
		//		100,
		//		100,
		//		100 };

		//DirectX::XMFLOAT3 wposL = {
		//		nodeTip->worldTransform._41,
		//		nodeTip->worldTransform._42,
		//		nodeTip->worldTransform._43 };
		//DirectX::XMFLOAT3 wposL = {
		//		handNodeL.position.x,
		//		handNodeL.position.y,
		//		handNodeL.position.z };
		
	    DirectX::XMStoreFloat3(&trailPositions[1][0],DirectX::XMLoadFloat3(&wposL));

		
		
	}

	DirectX::XMFLOAT4 color = { 1, 0, 0, 0.5f };

	// ポリゴン作成
	PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();
	if (!spline)
	{
		// 保存していた頂点バッファでポリゴンを作る
		for (int i = 0; i < MAX_POLYGON; ++i)
		{
			primitiveRenderer->AddVertex(trailPositions[0][i], color);
			primitiveRenderer->AddVertex(trailPositions[1][i], color);
		}
	}
	else
	{
		// TODO③:保存していた頂点バッファを用いてスプライン補完処理を行い、
		//        滑らかなポリゴンを描画せよ
		{
			for (int i = 0; i < MAX_POLYGON-3; ++i)
			{
				DirectX::XMVECTOR Pos0;
				DirectX::XMVECTOR Pos1;

				DirectX::XMFLOAT3 pos0f;
				DirectX::XMFLOAT3 pos1f;


				// スプライン曲線 １,２を一つの点にしてカクカク取り消し
				Pos0 = DirectX::XMVectorCatmullRom(
					DirectX::XMLoadFloat3(&trailPositions[0][i]),
					DirectX::XMLoadFloat3(&trailPositions[0][i+1]),
					DirectX::XMLoadFloat3(&trailPositions[0][i+2]), 
					DirectX::XMLoadFloat3(&trailPositions[0][i+3]),
					t);

				Pos1 = DirectX::XMVectorCatmullRom(
					DirectX::XMLoadFloat3(&trailPositions[1][i]),
					DirectX::XMLoadFloat3(&trailPositions[1][i + 1]),
					DirectX::XMLoadFloat3(&trailPositions[1][i + 2]),
					DirectX::XMLoadFloat3(&trailPositions[1][i + 3]),
					t);
				DirectX::XMStoreFloat3(
					&pos0f, Pos0);

				DirectX::XMStoreFloat3(
					&pos1f, Pos1);


				// 手元
				primitiveRenderer->AddVertex(
					pos0f, color);


				// 先端
				primitiveRenderer->AddVertex(
					pos1f, color);

	

			}

		}
	}
}
//Camera& camera, ID3D11DeviceContext* dc
// 描画処理
void SwordTrailScene::Render(Camera& camera, ID3D11DeviceContext* immediate_context/*, ID3D11RenderTargetView* render , ID3D11DepthStencilView* dsv */ )
{
	////this->camera = camera;
	//Graphics& graphics = Graphics::Instance();
	////ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	////ID3D11RenderTargetView* render = Graphics::Instance().GetRenderTargetView();
	//PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();

	//ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	RenderState* renderState = Graphics::Instance().GetRenderState();
	PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();


	


	//// レンダーステート設定
	immediate_context->OMSetBlendState(renderState->GetBlendState(BlendState::Opaque), nullptr, 0xFFFFFFFF);
	immediate_context->OMSetDepthStencilState(renderState->GetDepthStencilState(DepthState::TestAndWrite), 0);
	immediate_context->RSSetState(renderState->GetRasterizerState(RasterizerState::SolidCullNone));


	// レンダーステート設定
	//immediate_context->OMSetBlendState(blendStates[0].Get(), nullptr, 0xFFFFFFFF);
	//immediate_context->OMSetDepthStencilState(depthStencilStates[0].Get(), 0);
	//immediate_context->RSSetState(rasterizerStates[0].Get());


	// ポリゴン描画
	primitiveRenderer->Render(immediate_context, camera.GetView(), camera.GetProjection(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// グリッド描画
	primitiveRenderer->DrawGrid(20, 1);
	primitiveRenderer->Render(immediate_context, camera.GetView(), camera.GetProjection(), D3D11_PRIMITIVE_TOPOLOGY_LINELIST);


}

// GUI描画処理
void SwordTrailScene::DrawGUI()
{
	//ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	//ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y + 10), ImGuiCond_Once);
	//ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Once);

	//if (ImGui::Begin(u8"剣の軌跡処理"))
	//{
	//	ImGui::Checkbox(u8"スプライン補完", &spline);

	//	ImGui::SliderFloat(u8"剣の軌跡の〇部分の綺麗差", &t,0.0f,1.0f);
	//}
	//ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Playerwww", nullptr, ImGuiWindowFlags_None))
	{
		//位置
		ImGui::InputFloat3("Position", &wpos.x);
		ImGui::InputFloat3("Position2", &wposL.x);
	}
	ImGui::End();
}
