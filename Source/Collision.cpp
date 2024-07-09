#include "Collision.h"

//球と球の交差判定
bool Collision::IntersectSphereVsSphere(const DirectX::XMFLOAT3& positionA, float radiusA, const DirectX::XMFLOAT3& positionB, float radiusB, DirectX::XMFLOAT3& outPositionB)
{
    //A->Bの単位ベクトルを算出
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
    DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    //距離判定
    float range = radiusA + radiusB;
    if (lengthSq > range * range)
    {
        return false;
    }

    //AがBを押し出す
    Vec = DirectX::XMVector3Normalize(Vec);
    Vec = DirectX::XMVectorScale(Vec, range);
    PositionB = DirectX::XMVectorAdd(PositionA, Vec);
    DirectX::XMStoreFloat3(&outPositionB, PositionB);

    return true;
}

bool Collision::IntersectSphereVsSphere(const DirectX::XMFLOAT3& positionA, float radiusA, const DirectX::XMFLOAT3& positionB, float radiusB, DirectX::XMFLOAT3& outPositionA, DirectX::XMFLOAT3& outPositionB)
{
    // B→Aの単位ベクトルを算出
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
    DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    // 距離判定
    float range = radiusA + radiusB;
    if (lengthSq > range * range)
    {
        return false;
    }

    Vec = DirectX::XMVector3Normalize(Vec);
    // 当たり判定に質量等を付与して重み判定を行うと重さを表現しやすい
    Vec = DirectX::XMVectorScale(Vec, (range - lengthSq) * 0.5f);
    PositionA = DirectX::XMVectorSubtract(PositionA, Vec);
    PositionB = DirectX::XMVectorAdd(PositionB, Vec);
    DirectX::XMStoreFloat3(&outPositionA, PositionA);
    DirectX::XMStoreFloat3(&outPositionB, PositionB);
    return true;
}

//円柱同士の交差判定
bool Collision::IntersectCylinderVsCylinder(const DirectX::XMFLOAT3& positionA, float radiusA, float heightA, const DirectX::XMFLOAT3& positionB, float radiusB, float heightB, DirectX::XMFLOAT3& outPositionB)
{
    if (positionA.y > positionB.y + heightB)
    {
        return false;
    }

    //Aの頭がBの足元より下なら当たっていない
    if (positionA.y + heightB < positionB.y)
    {
        return false;
    }

    //XZ平面での範囲チェック
    //positionB - positionAまでの距離が
    DirectX::XMFLOAT2 posA(positionA.x, positionA.z);
    DirectX::XMFLOAT2 posB(positionB.x, positionB.z);

    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat2(&posA);
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat2(&posB);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
    DirectX::XMVECTOR LengthSq = DirectX::XMVector2Length(Vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    //radiusAとradiusBの長さ
    float range = radiusA + radiusB;

    //radiusA + radiusB より大きかったら
    if (lengthSq > range)
    {
        return false;
    }


    //AがBを押し出す(Bは下に押し出してはいけない)

    //当たっている場合

    Vec = DirectX::XMVector3Normalize(Vec);
    Vec = DirectX::XMVectorScale(Vec, range);
    Vec = DirectX::XMVectorAdd(PositionA, Vec);

    DirectX::XMFLOAT2 resultPos;
    DirectX::XMStoreFloat2(&resultPos, Vec);

    outPositionB.x = resultPos.x;
    outPositionB.y = positionB.y;
    outPositionB.z = resultPos.y;

    return true;
    ////Aの足元がBの頭より上ならあたっていない
    //if (positionA.y > positionB.y + heightB)
    //{
    //    return false;
    //}
    ////Aの頭がBの足元より下ならあたっていない
    //if (positionA.y + heightA < positionB.y)
    //{
    //    return false;
    //}
    ////XZ平面での範囲チェック
    //float vx = positionB.x - positionA.x;
    //float vz = positionB.z - positionA.z;
    //float range = radiusA + radiusB;
    //float distXZ = sqrtf(vx * vx + vz * vz);
    //if (distXZ > range)
    //{
    //    return false;
    //}
    ////AがBを押し出す
    //vx /= distXZ;
    //vz /= distXZ;
    //outPositionB.x = positionA.x +(vx * range);
    //outPositionB.y = positionB.y;
    //outPositionB.z = positionA.z +(vz * range);

    //return true;
}

//球と円柱の交差判定
bool Collision::IntersectSphereVsCylinder(const DirectX::XMFLOAT3& spherePosition, float sphereRadius, const DirectX::XMFLOAT3& cylinderPosition, float cylinderRadius, float cylinderHeight, DirectX::XMFLOAT3& outCylinderPosition)
{
    //高さチェック
    if (spherePosition.y + sphereRadius < cylinderPosition.y) return false;
    if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderHeight) return false;

    //XZ平面での範囲チェック
    float vx = cylinderPosition.x - spherePosition.x;
    float vz = cylinderPosition.z - spherePosition.z;
    float range = sphereRadius + cylinderRadius;
    float distXZ = sqrtf(vx * vx + vz * vz);
    if (distXZ > range) return false;

    //球が円柱を押し出す
    vx /= distXZ;
    vz /= distXZ;
    outCylinderPosition.x = spherePosition.x + (vx * range);
    outCylinderPosition.y = cylinderPosition.y;
    outCylinderPosition.z = spherePosition.z + (vz * range);

    return true;
}

//レイとモデルの交差判定
bool Collision::IntersectRayVsModel(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const Model* model, HitResult& result)
{
    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
    DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
    DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

    //ワールド空間のレイの長さ
    DirectX::XMStoreFloat(&result.distance, WorldRayLength);

    bool hit = false;
    const ModelResource* resource = model->GetResource();
    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
    {
        //メッシュノード取得
        const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);

        //レイをワールド空間からローカル空間へ変換
        DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
        DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

        DirectX::XMVECTOR Start = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
        DirectX::XMVECTOR End = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
        DirectX::XMVECTOR Dir = DirectX::XMVector3Normalize(Vec);
        DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);

        //レイの長さ
        float neart;
        DirectX::XMStoreFloat(&neart, Length);

        //三角形との交差判定
        const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
        const std::vector <UINT> indices = mesh.indices;

        int materialIndex = -1;
        DirectX::XMVECTOR HitPosition;
        DirectX::XMVECTOR HitNormal;
        for (const ModelResource::Subset& subset : mesh.subsets)
        {
            for (UINT i = 0; i < subset.indexCount; i += 3)
            {
                UINT index = subset.startIndex + i;

                //三角形の頂点を抽出
                const ModelResource::Vertex& a = vertices.at(indices.at(index));
                const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
                const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

                DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
                DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
                DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

                //三角形の三辺ベクトルを算出
                DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
                DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
                DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

                //三角形の法線ベクトルを算出
                DirectX::XMVECTOR Normal = DirectX::XMVector3Cross(AB, BC);

                //内積の結果がプラスならば裏向き
                DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Dir, Normal);
                float dot;
                DirectX::XMStoreFloat(&dot, Dot);
                if (dot >= 0) continue;

                //レイと平面の交点を算出
                DirectX::XMVECTOR V = DirectX::XMVectorSubtract(A, Start);
                DirectX::XMVECTOR T = DirectX::XMVectorDivide(DirectX::XMVector3Dot(Normal, V), Dot);
                float t;
                DirectX::XMStoreFloat(&t, T);
                if (t < .0f || t > neart) continue;     //交点までの距離が今までに計算した最近距離より
                                                        //大きいときはスキップ
                DirectX::XMVECTOR Position = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(Dir, T), Start);

                //交点が三角形の内側にあるか判定
                //１つめ
                DirectX::XMVECTOR V1 = DirectX::XMVectorSubtract(A, Position);
                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(V1, AB);
                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, Normal);
                float dot1;
                DirectX::XMStoreFloat(&dot1, Dot1);
                if (dot1 < 0.0f) continue;
                //２つめ
                DirectX::XMVECTOR V2 = DirectX::XMVectorSubtract(B, Position);
                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(V2, BC);
                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, Normal);
                float dot2;
                DirectX::XMStoreFloat(&dot2, Dot2);
                if (dot2 < 0.0f) continue;
                //３つめ
                DirectX::XMVECTOR V3 = DirectX::XMVectorSubtract(C, Position);
                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(V3, CA);
                DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, Normal);
                float dot3;
                DirectX::XMStoreFloat(&dot3, Dot3);
                if (dot3 < 0.0f) continue;

                //最近距離を異更新
                neart = t;

                //交点と法線を更新
                HitPosition = Position;
                HitNormal = Normal;
                materialIndex = subset.materialIndex;
            }
        }
        if (materialIndex >= 0)
        {
            //ローカル空間からワールド空間へ変換
            DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
            DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
            DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
            float distance;
            DirectX::XMStoreFloat(&distance, WorldCrossLength);

            //ヒット情報保存
            if (result.distance > distance)
            {
                DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);
                result.distance = distance;
                result.materialIndex = materialIndex;
                DirectX::XMStoreFloat3(&result.position, WorldPosition);
                DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
                hit = true;
            }
        }
    }

    return hit;
}
