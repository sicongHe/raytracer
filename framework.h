#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
using namespace std;
//全局变量区
const int rb = 255;
const int gb = 255;
const int bb = 255;
//*******画图函数与alpha混合算法********
int setAlphaR(double tmp, double alpha);

int setAlphaG(double tmp, double alpha);

int setAlphaB(double tmp, double alpha);

void makeImage(ofstream &out, int weight, int height, int maxPixel);
class intersectResult;
//********定义三维向量类********
class Vector3{
public:
	double x,y,z;//xyz三个坐标
	Vector3();//默认构造函数
	Vector3(double x, double y, double z);//自定义构造函数
	double length();//获得长度
	double sqrlength();//长度的平方
	Vector3 normalize();//将该向量单位化
	Vector3 negate();//得到反向量
	Vector3 add(Vector3 tmp);//将向量tmp与本向量相加
	//Vector3 add(double x, double y, double z);
	Vector3 subtract(Vector3 tmp);//减法
	Vector3 multiply(double tmp);//数乘
	Vector3 divide(double tmp);//除法
	double dot(Vector3 tmp);//点积
	Vector3 cross(Vector3 tmp);//叉积
	inline static Vector3 zero(){
		return Vector3(0,0,0);
	}
};
//********定义光线类********
class Ray3{
public:
	Vector3 origin;//光线的起点
	Vector3 direction;//光线的方向
	Ray3();//构造函数
	Ray3(double x, double y, double z, double a, double b, double c);//直接初始化光线
	Ray3(Vector3 origin, Vector3 direction);//自定义构造函数
	Vector3 getPoint(double t);//得到一个点坐标，即改光线传播t距离后的点
};
//颜色类 用(0,1)映射(0,255);
class color{
public:
	double r;
	double g;
	double b;
	color();
	color(double r, double g, double b);
	color add(double r, double g, double b);
	color add(color tmp);
	color multiply(double s);
	color modulate(double r, double g, double b);
	color modulate(color tmp);
	inline static color white(){
	color whiteColor(1,1,1);
	return whiteColor;
	}
	inline static color black(){
	color blackColor(0,0,0);
	return blackColor;
	}
	inline static color red(){
		return color(1,0,0);
	}
	inline static color green(){
		return color(0,1,0);
	}
	inline static color blue(){
		return color(0,0,1);
	}
};

//定义材质类(超类)
class material{
public:
	double reflectiveness;
	inline material(double _reflectiveness = 0): reflectiveness(_reflectiveness){}
	virtual color sample(Vector3 position, Vector3 normal, Ray3 ray, Vector3 lightDir, color lightColor, color environmentLight) = 0;
	virtual ~material(){}
};
//定义Phong材质
class phongMaterial: public material{
public:
	color diffuse;//漫反射系数 反应物体的颜色
	color specular;//镜面反射系数(0-1)
	double shininess;//反射指数 表示了光泽程度(1-2000)
	double environment;//环境光反射系数(0-1)
	inline phongMaterial(color _diffuse, color _specular, double _shininess, double _environment, double _reflectiveness = 0):
	material(_reflectiveness), diffuse(_diffuse), specular(_specular), shininess(_shininess), environment(_environment){}
	virtual color sample(Vector3 position, Vector3 normal, Ray3 ray, Vector3 lightDir, color lightColor, color environmentLight);

};

class checkerMaterial: public material{
public:
	double scale;
	inline checkerMaterial(double _scale, double _reflectiveness = 0):material(_reflectiveness), scale(_scale){}
	virtual color sample(Vector3 position, Vector3 normal, Ray3 ray, Vector3 lightDir, color lightColor, color environmentLight);

};

//定义几何体类(超类)
class geometry{
public:
	material *materialPointer;

	inline geometry():materialPointer(0){}
	// virtual ~geometry(){
	// 	if(this->materialPointer != 0){
	// 		delete this->materialPointer;
	// 		this->materialPointer = 0;
	// 		cout<<"delete!"<<'\n';
	// 	}
	// }
	virtual intersectResult intersect(Ray3 ray) = 0;
	virtual void initialize(){ };
};

class intersectResult{
public:
	geometry* myGeometry;
	double distance;
	Vector3 position;
	Vector3 normal;
	intersectResult();
	static inline intersectResult noHit(){return intersectResult();}
};

//********定义球类********
class Sphere: public geometry{
public:
	Vector3 center;
	double radius;
	Sphere();
	Sphere(Vector3 center, double radius);
	Sphere(double x, double y, double z, double r);
	virtual intersectResult intersect(Ray3 ray);
	//intersectResult intersect(double  x, double y, double z, double a, double b, double c);
};

class scene{
public:
	Vector3 center;
	int weight;
	int height;
	scene();
	scene(Vector3 center, int weigth, int height);
};

class camera{
public:
	Vector3 position;
	camera();
	camera(Vector3 position);
};

class render{
public:
	Sphere mySphere;
	camera myCamera;
	scene myScene;
	render();
	render(Sphere mySphere, camera myCamera, scene myScene);
};

class Plane:public geometry{
public:
	Vector3 normal;
	double d;
	Vector3 position;
	inline Plane(const Plane &p):normal(p.normal),d(p.d),position(Vector3::zero()){}
	inline Plane(const Vector3 &_normal, double _d):normal(_normal),d(_d),position(Vector3::zero()){}
	inline void initialize(){
		this->position = normal.multiply(d);
	}
	virtual intersectResult intersect(Ray3 ray);
};

class Union: public geometry{
public:
	vector<geometry*> geometries;
	inline Union(){}
	void push(geometry* myGeometry){
		geometries.push_back(myGeometry);
	}
	inline void initialize(){
		long size = geometries.size();
		for(long i = 0; i < size; i++){
			geometries[i]->initialize();
		}
	}
	virtual intersectResult intersect(Ray3 ray);
};

class unionRender{
public:
	Union myUnion;
	camera myCamera;
	scene myScene;
	inline unionRender(Union _myUnion, camera _myCamera, scene _myScene):myUnion(_myUnion), myCamera(_myCamera), myScene(_myScene){}
};
//渲染函数
//灰度渲染
void depthImageRender(ofstream &out, render myRender, double maxDepth, double alpha);//灰度渲染
//渲染法向量
void normalRender(ofstream &out, render myRender, double alpha);

void materialRender(ofstream &out, render myRender, double alpha);

void materialRenderForUnion(ofstream &out, unionRender myRender, double alpha);

void reflectRender(ofstream &out, unionRender myRender, double alpha);
color reflect(geometry *geometryPointer, Ray3 ray, int maxReflect);





























