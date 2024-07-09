#include "Mathf.h"
#include <stdlib.h>

//���`���
float Mathf::Lerp(float a, float b, float t)
{
    return a * (1.0f - t) + (b * t);
}

//�w��͈͂̃����_���l���v�Z����
float Mathf::RandomRange(float min, float max)
{
    //0.0~1.0�̊Ԃ܂ł̃����_���l
    float value = static_cast<float>(rand()) / RAND_MAX;

    //min~max�܂ł̃����_���l�ɕϊ�
    return min + (max - min) * value;
}

//int Mathf::RandomRangeInt(float min, float max)
//{
//    //0.0~1.0�̊Ԃ܂ł̃����_���l
//    int value = rand() / RAND_MAX;
//
//    //min~max�܂ł̃����_���l�ɕϊ�
//    return (int)min + (max - min) * value;
//}
