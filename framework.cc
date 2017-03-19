#include "framework.h"
Vector3 lightDir = Vector3(0,-1,0).normalize();
color lightColor(1,1,1);
color environmentLight(1,0,0);
int q = 0;
int setAlphaR(double tmp, double alpha){
	return ((1-alpha)*rb + alpha*tmp);
}

int setAlphaG(double tmp, double alpha){
	return ((1-alpha)*gb + alpha*tmp);
}

int setAlphaB(double tmp, double alpha){
	return ((1-alpha)*bb + alpha*tmp);
}

void makeImage(ofstream &out, int weight, int height, int maxPixel = 255){
	out<<"P3"<<'\n'<<weight<<'\t'<<height<<'\n'<<maxPixel<<endl;
}//IO类不支持拷贝操作，因此不能使用拷贝传参只能使用引用传参;
//对象较大时拷贝操作速度慢，用引用效率更高;

//创建反射函数
color reflect(geometry *geometryPointer, Ray3 ray, int maxReflect){
	intersectResult result = geometryPointer->intersect(ray);
	//测试函数

	if((ray.origin.x != 0) && (ray.origin.y != 0) && (ray.origin.z != 0) && (maxReflect != 5) ){
		if(result.position.y == -50){
			//cout<<"击中平面"<<endl;
			q++;
			color test = result.myGeometry->materialPointer->sample(result.position, result.normal, ray, lightDir, lightColor, environmentLight);
			cout<<test.r*255<<endl;
		}
	}
	//cout<<q<<endl;

	if(result.myGeometry){
		//cout<<"有交点"<<'\n';
		double reflectiveness = result.myGeometry->materialPointer->reflectiveness;
		//cout<<"得到反射度"<<'\n';
		color tmpColor = result.myGeometry->materialPointer->sample(result.position, result.normal, ray, lightDir, lightColor, environmentLight);
		tmpColor = tmpColor.multiply(1 - reflectiveness);
		if((reflectiveness > 0) && (maxReflect > 0)){
			//Vector3 r = result.normal.multiply(-2 * result.normal.dot(ray.direction)).add(ray.direction);
			Vector3 r = ray.direction.subtract(result.normal.multiply(-2).multiply(result.normal.dot(ray.direction))).normalize();
			Ray3 newRay(result.position, r);
			color reColor = reflect(geometryPointer, newRay, maxReflect - 1);
			tmpColor = tmpColor.add(reColor.multiply(reflectiveness));
		}
		else{
			;
		}
		return tmpColor;
	}
	else{
		//cout<<"没有交点"<<'\n';
		return color::black();
	}
}






void depthImageRender(ofstream &out, render myRender, double maxDepth, double alpha){
	int weight = myRender.myScene.weight;
	int height = myRender.myScene.height;
	int sceneX = myRender.myScene.center.x;
	int sceneY = myRender.myScene.center.y;
	int sceneZ = myRender.myScene.center.z;
	cout<<myRender.myCamera.position.x<<' '<<myRender.myCamera.position.y<<' '<<myRender.myCamera.position.z<<'\n';
	cout<<myRender.mySphere.center.x<<' '<<myRender.mySphere.center.y<<' '<<myRender.mySphere.center.z<<' '<<'\n';
	cout<<myRender.myScene.center.x<<' '<<myRender.myScene.center.y<<' '<<myRender.myScene.center.z<<'\n';
	double depth = 0;
	//string empty = "noHit";
	for(int i = 0; i < weight*2; i++){
		for(int j = 0; j< height*2; j++){
			Vector3 tmpPoint(0,0,0);
			Vector3 tmptmp((sceneX - weight + j), (sceneY + height - i), sceneZ);
			tmpPoint = tmptmp;
			Ray3 tmpRay(myRender.myCamera.position,tmpPoint.subtract(myRender.myCamera.position));
			intersectResult tmpResult = myRender.mySphere.intersect(tmpRay);
			if(tmpResult.myGeometry == 0){
				depth = 0;
			}
			else{
				depth = 255 - fmin((tmpResult.distance/maxDepth)*255, 255);
				cout<<"Distance is "<<tmpResult.distance<<'\n';
			}
			
			out<<setAlphaR(depth, alpha) <<'\t'<<setAlphaG(depth, alpha)<<'\t'<<setAlphaB(depth, alpha)<<'\t';
		}
		out<<'\n';
	}
}

void normalRender(ofstream &out, render myRender, double alpha){
	int weight = myRender.myScene.weight;
	int height = myRender.myScene.height;
	int sceneX = myRender.myScene.center.x;
	int sceneY = myRender.myScene.center.y;
	int sceneZ = myRender.myScene.center.z;
	int r,g,b;
	//string empty = "noHit";
	for(int i = 0; i < weight*2; i++){
		for(int j = 0; j < height*2; j++){
			Vector3 tmpPoint(0,0,0);
			Vector3 tmptmp((sceneX - weight + j), (sceneY + height - i), sceneZ);
			tmpPoint = tmptmp;
			Ray3 tmpRay(myRender.myCamera.position,tmpPoint.subtract(myRender.myCamera.position));
			intersectResult tmpResult = myRender.mySphere.intersect(tmpRay);
			if(tmpResult.myGeometry == 0){
				out<<setAlphaR(0,alpha)<<'\t'<<setAlphaG(0,alpha)<<'\t'<<setAlphaB(0,alpha)<<'\t';
			}
			else{
				r = (tmpResult.normal.x - (-1))/2*255;
				g = (tmpResult.normal.y - (-1))/2*255;
				b = (tmpResult.normal.z - (-1))/2*255;
				out<<setAlphaR(r,alpha)<<'\t'<<setAlphaG(g,alpha)<<'\t'<<setAlphaB(b,alpha)<<'\t';

			}
			out<<'\n';
		}
	}
}

void materialRender(ofstream &out, render myRender, double alpha){
	int weight = myRender.myScene.weight;
	int height = myRender.myScene.height;
	int sceneX = myRender.myScene.center.x;
	int sceneY = myRender.myScene.center.y;
	int sceneZ = myRender.myScene.center.z;
	Vector3 tmpPoint(0,0,0);
	Vector3 tmptmp;
	Ray3 tmpRay;
	intersectResult tmpResult;
	color tmpColor;
	double r, g, b;
	for(int i = 0; i < weight*2; i++){
		for(int j = 0; j < height*2; j++){
			tmpPoint = Vector3(0,0,0);
			tmptmp = Vector3((sceneX - weight + j), (sceneY + height - i), sceneZ);
			tmpPoint = tmptmp;
			tmpRay = Ray3(myRender.myCamera.position,tmpPoint.subtract(myRender.myCamera.position));
			tmpResult = myRender.mySphere.intersect(tmpRay);

			if(tmpResult.myGeometry == 0){
				out<<setAlphaR(0,alpha)<<'\t'<<setAlphaG(0,alpha)<<'\t'<<setAlphaB(0,alpha)<<'\t';
			}
			else{
				tmpColor = tmpResult.myGeometry->materialPointer->sample(tmpResult.position, tmpResult.normal, tmpRay, lightDir, lightColor, environmentLight);

				r = min(tmpColor.r * 255, (double)255);
				g = min(tmpColor.g * 255, (double)255);
				b = min(tmpColor.b * 255, (double)255);
				//cout<<r<<' '<<g<<' '<<b<<endl;
				out<<setAlphaR(r,alpha)<<'\t'<<setAlphaG(g,alpha)<<'\t'<<setAlphaB(b,alpha)<<'\t';
			}	
			out<<'\n';
		}
	}
}

void materialRenderForUnion(ofstream &out, unionRender myRender, double alpha){
	int weight = myRender.myScene.weight;
	int height = myRender.myScene.height;
	int sceneX = myRender.myScene.center.x;
	int sceneY = myRender.myScene.center.y;
	int sceneZ = myRender.myScene.center.z;
	Vector3 tmpPoint(0,0,0);
	Vector3 tmptmp;
	Ray3 tmpRay;
	intersectResult tmpResult;
	color tmpColor;
	double r, g, b;
	for(int i = 0; i < weight*2; i++){
		for(int j = 0; j < height*2; j++){
			tmpPoint = Vector3(0,0,0);
			tmptmp = Vector3((sceneX - weight + j), (sceneY + height - i), sceneZ);
			tmpPoint = tmptmp;
			tmpRay = Ray3(myRender.myCamera.position,tmpPoint.subtract(myRender.myCamera.position));
			tmpResult = myRender.myUnion.intersect(tmpRay);

			if(tmpResult.myGeometry == 0){
				out<<setAlphaR(0,alpha)<<'\t'<<setAlphaG(0,alpha)<<'\t'<<setAlphaB(0,alpha)<<'\t';
			}
			else{
				tmpColor = tmpResult.myGeometry->materialPointer->sample(tmpResult.position, tmpResult.normal, tmpRay, lightDir, lightColor, environmentLight);

				r = min(tmpColor.r * 255, (double)255);
				g = min(tmpColor.g * 255, (double)255);
				b = min(tmpColor.b * 255, (double)255);
				//cout<<r<<' '<<g<<' '<<b<<endl;
				out<<setAlphaR(r,alpha)<<'\t'<<setAlphaG(g,alpha)<<'\t'<<setAlphaB(b,alpha)<<'\t';
			}	
			out<<'\n';
		}
	}
}

void reflectRender(ofstream &out, unionRender myRender, double alpha){
	int weight = myRender.myScene.weight;
	int height = myRender.myScene.height;
	int sceneX = myRender.myScene.center.x;
	int sceneY = myRender.myScene.center.y;
	int sceneZ = myRender.myScene.center.z;
	Vector3 tmpPoint(0,0,0);
	Vector3 tmptmp;
	Ray3 tmpRay;
	//intersectResult tmpResult;
	color tmpColor;
	double r, g, b;
	for(int i = 0; i < weight*2; i++){
		for(int j = 0; j < height*2; j++){
			tmpPoint = Vector3(0,0,0);
			tmptmp = Vector3((sceneX - weight + j), (sceneY + height - i), sceneZ);
			tmpPoint = tmptmp;
			tmpRay = Ray3(myRender.myCamera.position,tmpPoint.subtract(myRender.myCamera.position));
			tmpColor = reflect(&(myRender.myUnion), tmpRay, 1);
			r = min(tmpColor.r * 255, (double)255);
			g = min(tmpColor.g * 255, (double)255);
			b = min(tmpColor.b * 255, (double)255);
			out<<setAlphaR(r,alpha)<<'\t'<<setAlphaG(g,alpha)<<'\t'<<setAlphaB(b,alpha)<<'\t';
		}
		out<<'\n';
	}
}





Vector3::Vector3(){
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vector3::Vector3(double x, double y, double z){
	this->x = x;
	this->y = y;
	this->z = z;
}

double Vector3::length(){
	return sqrt(this->x * this-> x +
		this->y * this->y +
		this->z * this->z
		);
}

double Vector3::sqrlength(){
	return (this->x * this-> x +
		this->y * this->y +
		this->z * this->z
		);
}

Vector3 Vector3::normalize(){
	double tmp = 1/(this->length());
	Vector3 normalizeVec(this->x * tmp, this->y *tmp, this->z *tmp);
	return normalizeVec;
}

Vector3 Vector3::negate(){
	Vector3 negateVec(-(this->x), -(this->y), -(this->z));
	return negateVec;
}

Vector3 Vector3::add(Vector3 tmp){
	Vector3 addVec((this->x + tmp.x),
		(this->y + tmp.y),
		(this->z + tmp.z)
		);
	return addVec;
}

Vector3 Vector3::subtract(Vector3 tmp){
	Vector3 subtractVec((this->x - tmp.x),
		(this->y - tmp.y),
		(this->z - tmp.z)
		);
	return subtractVec;
}

Vector3 Vector3::multiply(double tmp){
	Vector3 multiplyVec((this->x) * tmp,
		(this->y) * tmp,
		(this->z) * tmp
		);
	return multiplyVec;
}

Vector3 Vector3::divide(double tmp){
	Vector3 divideVec((this->x) / tmp,
		(this->y) / tmp,
		(this->z) / tmp
		);
	return divideVec;
}

double Vector3::dot(Vector3 tmp){
	return(this->x * tmp.x + this->y * tmp.y +this->z *tmp.z);
}

Vector3 Vector3::cross(Vector3 tmp){
	Vector3 crossVec(-(this->z) * tmp.y + (this->y) * tmp.z,
		(this->z) * tmp.x - (this->x) * tmp.z,
		-(this->y) * tmp.x + (this->x) * tmp.y
		);
	return crossVec;
}

Ray3::Ray3(){
	Vector3 origin(0,0,0);
	Vector3 direction(0,0,0);
	this->origin = origin;
	this->direction = direction;
}

Ray3::Ray3(double x, double y, double z, double a, double b, double c){
	Vector3 origin(x,y,z);
	Vector3 direction(a,b,c);
	this->origin = origin;
	this->direction = direction.normalize();
}

Ray3::Ray3(Vector3 origin, Vector3 direction){
	this->origin = origin;
	this->direction = direction.normalize();
}

Vector3 Ray3::getPoint(double t){
	Vector3 point(0,0,0);
	point = this->origin.add(this->direction.multiply(t));
	return point;
}

Sphere::Sphere(){
	Vector3 center(0,0,0);
	double radius = 0;
	this->center = center;
	this->radius = radius;
}

Sphere::Sphere(Vector3 center, double radius){
	this->center = center;
	this->radius = radius;
}

Sphere::Sphere(double x, double y, double z, double r){
	Vector3 center(x,y,z);
	this->center = center;
	this->radius = r;
}

intersectResult Sphere::intersect(Ray3 ray){
	Vector3 v = ray.origin.subtract(this->center);
	Vector3 d = ray.direction;
	double r = this->radius;
	double tmp = d.dot(v) * d.dot(v) - (v.dot(v) - r*r);
	intersectResult result;
	//cout<<tmp<<'\n';
	if(tmp >= 0){
		result.myGeometry = this;
		result.distance = -d.dot(v) - sqrt(tmp);
		result.position = ray.getPoint(result.distance);
		result.normal = result.position.subtract(this->center).normalize();
		//cout<<result.distance<<'\n';
		return result;
	}
	else{
		//cout<<"noHit"<<'\n';
		return intersectResult::noHit();
	}
}

intersectResult::intersectResult(){
	this->myGeometry = 0;
	this->distance = 0;
	Vector3 zero(0,0,0);
	this->position = zero;
	this->normal = zero;
}

scene::scene(){
	this->weight = 0;
	this->height = 0;
	Vector3 zero(0,0,0);
	this->center = zero;
}

scene::scene(Vector3 center, int weight, int height){
	this->center = center;
	this->weight = weight;
	this->height = height;
}

camera::camera(){
	Vector3 zero(0,0,0);
	this->position = zero;
}

camera::camera(Vector3 tmp){
	this->position = tmp;
}

render::render(){
	Sphere s;
	camera c;
	scene sc;
	this->mySphere = s;
	this->myCamera = c;
	this->myScene = sc;
}

render::render(Sphere mySphere, camera myCamera, scene myScene){
	this->mySphere = mySphere;
	this->myCamera = myCamera;
	this->myScene = myScene;

}

color::color(){
	this->r = 0;
	this->g = 0;
	this->b = 0;
}

color::color(double r, double g, double b){
	this->r = r;
	this->g = g;
	this->b = b;
}

color color::add(double r, double g, double b){
	double tmpR = r + this->r;
	double tmpG = g + this->g;
	double tmpB = b + this->b;
	color newColor(tmpR,tmpG,tmpB);
	return newColor;
}

color color::add(color tmp){
	color newColor(this->r + tmp.r,this->g + tmp.g,this->b + tmp.b);
	return newColor;
}

color color::multiply(double s){
	color newColor(this->r * s,this->g * s,this->b *s);
	return newColor;
}

color color::modulate(double r, double g, double b){
	color newColor(this->r * r,this->g * g,this->b * b);
	return newColor;
}

color color::modulate(color tmp){
	color newColor(this->r * tmp.r,this->g * tmp.g,this->b * tmp.b);
	return newColor;
}

// color phongMaterial::sample(Ray3 ray, Vector3 position, Vector3 normal){
// 	double NdotL = normal.dot(lightDir);
// 	Vector3 H = (lightDir.subtract(ray.direction)).normalize();
// 	double NdotH = normal.dot(H);
// 	color diffuseTerm = (this->diffuse).multiply(max(NdotL, (double)0));
// 	color specularTerm = (this->specular).multiply(pow(max(NdotH,(double)0), this->shininess));
// 	//cout<<max(NdotL, (double)0)<<' '<<pow(max(NdotH,(double)0), this->shininess)<<'\n';
// 	//cout<<lightColor.modulate(diffuseTerm.add(specularTerm)).r<<endl;
// 	return lightColor.modulate(diffuseTerm.add(specularTerm));
// }

//自己实现的phong材质
color phongMaterial::sample(Vector3 position, Vector3 normal, Ray3 ray, Vector3 lightDir, color lightColor, color environmentLight){
	double NdotL = normal.dot(lightDir.normalize().negate());
	color diffuseTerm(max(lightColor.r * (this->diffuse.r) * NdotL, (double)0),
		max(lightColor.g * (this->diffuse.g) * NdotL, (double)0),
		max(lightColor.b * (this->diffuse.b) * NdotL, (double)0)
		);
	Vector3 reflectDir = lightDir.normalize().add(normal.multiply(2*NdotL));
	double RdotR = ray.direction.normalize().negate().dot(reflectDir.normalize());
	color specularTerm(lightColor.r * (this->specular.r) * pow(max(RdotR, (double)0), shininess),
		lightColor.g * (this->specular.g) * pow(max(RdotR, (double)0), shininess),
		lightColor.b * (this->specular.b) * pow(max(RdotR, (double)0), shininess)
		);
	color environmentTerm(environmentLight.r * (this->environment),
		environmentLight.g * (this->environment),
		environmentLight.b * (this->environment)
		);

	color finalTerm((diffuseTerm.r  + specularTerm.r  + environmentTerm.r * 0),
		(diffuseTerm.g  + specularTerm.g  + environmentTerm.g * 0),
		(diffuseTerm.b  + specularTerm.b + environmentTerm.b * 0)
		);
	//if(finalTerm.b > 1 )
		//cout<<finalTerm.b<<'\n';
	return finalTerm;
}


intersectResult Plane::intersect(Ray3 ray){
	double a = ray.direction.dot(this->normal);
	if(a >= 0){
		return intersectResult::noHit();
	}
	else{
		double b = this->normal.dot(ray.origin.subtract(this->position));
		intersectResult result;
		result.myGeometry = this;
		result.distance = -b / a;
		result.position = ray.getPoint(result.distance);
		result.normal = this->normal;
		return result;
	}
}

color checkerMaterial::sample(Vector3 position, Vector3 normal, Ray3 ray, Vector3 lightDir, color lightColor, color environmentLight){
	float d=abs((floor(position.x * this->scale) + floor(position.z * this->scale)));
	d=fmod(d,2);
	return  d < 1 ? color::black() : color::white();
}

intersectResult Union::intersect(Ray3 ray){
	const double Infinity = 1e30;
	double minDistance = Infinity;
	intersectResult minResult = intersectResult::noHit();
	long size = this->geometries.size();
	for(long i = 0; i < size; i++){
		intersectResult result = geometries[i]->intersect(ray);
		if(result.myGeometry && (result.distance < minDistance)){
			minDistance = result.distance;
			minResult = result;
		}
	}
	return minResult;
}






