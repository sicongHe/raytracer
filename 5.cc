#include "framework.h"
int main(){
	ofstream out("5.ppm");
	double alpha = 1;
	makeImage(out,400,400,255);
	Vector3 tmp(0,0,0);
	camera myCamera(tmp);
	Vector3 tmp2(0,0,100);
	Vector3 planeNormal(0,1,0);
	Vector3 planeNormal2(1,0,0);
	Vector3 planeNormal3(0,0,-1);
	scene myScene(tmp2,200,200);
	Sphere mySphere(49,0,100,50);
	Sphere mySphere2(-49,0,100,50);
	Sphere mySphere3(0,-1000,100,1000);	
	Plane myPlane(planeNormal.normalize(),-100);//要传入一个单位向量
	Plane testPlane(planeNormal2.normalize(),100);//平面相交测试
	Plane myPlane2(planeNormal2.normalize(),-300);
	Plane myPlane3(planeNormal3.normalize(),-500);
	//myPlane.materialPointer = new phongMaterial(color(0,1,1), color::blue(), 5, 0);
	myPlane.materialPointer = new checkerMaterial(0.1, 0.5 );
	myPlane2.materialPointer = new checkerMaterial(0.1, 0.2 );
	testPlane.materialPointer = new phongMaterial(color::red(), color::white(), 2, 0, 0.1);
	myPlane3.materialPointer = new phongMaterial(color(1,0,1), color::white(), 5, 0, 0);
	//mySphere.materialPointer = new checkerMaterial(0.1, 0.9);
	mySphere.materialPointer = new phongMaterial(color::red(), color::white(), 2, 0, 0.5);
	mySphere2.materialPointer = new phongMaterial(color::blue(), color::white(), 2, 0, 0.5);
	mySphere3.materialPointer = new phongMaterial(color::blue(), color::white(), 2, 0, 0.5);
	Union myUnion;
	myUnion.push(&myPlane);
	myUnion.push(&testPlane);
	//myUnion.push(&mySphere);
	//myUnion.push(&mySphere2);
	//myUnion.push(&myPlane);
	//myUnion.push(&mySphere3);
	//myUnion.push(&myPlane2);
	//myUnion.push(&myPlane3);
	myUnion.initialize();	
	unionRender myRender(myUnion, myCamera, myScene);
	//normalRender(out, myRender, alpha);
	//depthImageRender(out,myRender,250,alpha);
	//materialRender(out, myRender, alpha);
	reflectRender(out, myRender, alpha);
	delete mySphere.materialPointer;
	delete mySphere2.materialPointer;
	delete myPlane.materialPointer;
	delete myPlane2.materialPointer;
	delete myPlane3.materialPointer;
	delete mySphere3.materialPointer;
	delete testPlane.materialPointer;
	out.close();
	return 0;
}