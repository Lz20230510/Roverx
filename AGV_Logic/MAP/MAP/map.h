#include "sys.h"

//////////////////////////////////////////////////////////
//
//
//存储地图的途径信息
//其中OA存储东北坐标系的北方向坐标
//其中OB存储东北坐标系的东方向坐标
//OAOB_angle 存储路径点的航向角
//thresh_posi 为标定值
////////////////////////////////////////////////////////////
extern  float OAOB_angle[5200];		//路径上的OA OB 点的航向角
extern   double OA[5200];
extern  double OB[5200];
extern float thresh_posi;
