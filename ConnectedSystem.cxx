#include "ConnectedSystem.h"
#include <algorithm>
#include <cmath>

using namespace arma;


ConnectedSystem::ConnectedSystem(){
	Clear();
}

ConnectedSystem::~ConnectedSystem(){

}

void ConnectedSystem::SetupMatrix(){
	int n = nmasses;
	
	K = zeros<mat>(n,n);	
	L = zeros<mat>(n,n);	
	for(int i=0; i<nsprings; i++){
		K(s1.at(i),s2.at(i)) = k.at(i)/m.at(s1.at(i));
		K(s2.at(i),s1.at(i)) = k.at(i)/m.at(s2.at(i));	
		L(s1.at(i),s2.at(i)) = l.at(i);	
		L(s2.at(i),s1.at(i)) = l.at(i);	
	}

	K.print("K = ");
	L.print("L = ");	
		
	F = zeros<mat>(2*n,2*n);
	
	double valx, valy, dx, dy;
	int i1,i2,j2;
	std::vector<int> indx;
	
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){	
			
			valx = 0;
			valy = 0;
			if(i==j){
				//printf("\n\t [%i,%i] Diagonal :  ",i,j);
			
				indx = GetObjConnections(i);
				for(int k=0; k<indx.size(); k++){
					GetSpringObjs(indx.at(k),i1,i2);
					if(i1==i){
						j2 = i2;
					} else if(i2==i){
						j2 = i1;														
					} else continue;						
	
					dx = x.at(j2)-x.at(i);
					dy = y.at(j2)-y.at(i);
						
					//printf("\n\t\t%i - %i  dx = %.2f  dy = %.2f  l0 = %.2f  k = %.2f",i1,i2,dx,dy,L(i,j2),K(i,j2));

					valx += K(i,j2)*dx/(L(i,j2)*m.at(i));
					valy += K(i,j2)*dy/(L(i,j2)*m.at(i));
				}
			} else {
				if(K(i,j)==0)
					continue;					
				dx = x.at(j)-x.at(i);
				dy = y.at(j)-y.at(i);
				
				valx  = K(i,j)*dx/(L(i,j)*m.at(i));
				valy  = K(i,j)*dy/(L(i,j)*m.at(i));
			}
			
			F(i,j)     = valx;
			F(i+n,j+n) = valy;
		}
	}
	
	F.print("\nF = ");		
	return;
}

void ConnectedSystem::SolveMatrix(){
	
	SetupMatrix();

	cx_vec W = zeros<cx_vec>(nmasses);
	cx_mat V = zeros<cx_mat>(nmasses,nmasses);	
	
	bool success = arma::eig_gen(W,V,F);
	if(!success){
		printf("\n\t Error [matrix solver]:  eigen decomposition failed!\n");
		return;
	}

	W.print("\nEigenvalues ^ 2 [all] = ");
	V.print("\nEigenvectors [all] = ");		
	
	double tol = 1e-10;
	uvec indx = find(W>tol);
	nmodes = indx.n_elem; // set number of modes
	
	E = sqrt(W.elem(indx)); // locate all eigenvalues^2>1e-10 and sqrt them
	E.print("\nEigenvalues = ");
	
	Ax = V.submat(span(0,nmasses-1),span(indx.at(0),indx.at(indx.n_elem-1)));
	Ay = V.submat(span(nmasses,2*nmasses-1),span(indx.at(0),indx.at(indx.n_elem-1)));

	Ax.print("\nX Eigenvectors = ");
	Ay.print("\nY Eigenvectors = ");	
}

void ConnectedSystem::SetMotionScale(double scale){
	motionscale = scale;
}

void ConnectedSystem::GetMassEigenMotion(int obj, int mode, double t, double &xx, double &yy){

	if(!CheckMassObj(obj))
		return;
	
	if(!CheckEigenMode(mode))
		return;
		
	xx = x.at(obj) + motionscale*Ax.at(obj)*sin(E(mode)*t);
	yy = y.at(obj) + motionscale*Ay.at(obj)*sin(E(mode)*t);
}

void ConnectedSystem::GetSpringEigenMotion(int obj, int mode, double t, double &xx1, double &yy1, double &xx2, double &yy2){

	if(!CheckSpringObj(obj))
		return;
	
	if(!CheckEigenMode(mode))
		return;
	
	int obj1, obj2;
	GetSpringObjs(obj,obj1,obj2);
	
	GetMassEigenMotion(obj1,mode,t,xx1,yy1);
	GetMassEigenMotion(obj2,mode,t,xx2,yy2);
}		

void ConnectedSystem::AddMass(double xx, double yy, double mm){

	x.push_back(xx);
	y.push_back(yy);	
	m.push_back(mm);
	nmasses++;
}

void ConnectedSystem::RemoveMass(int obj){
	
	if(!CheckMassObj(obj))
		return;
		
	ClearMatrices();	
			
	// remove all associated connections with obj
	std::vector<int> indx = GetObjConnections(obj);
	std::sort(indx.begin(),indx.end(), std::greater<int>()); // descending order sort
	for(int i=0; i<indx.size(); i++)
		RemoveSpring(indx.at(i));

	// now remove mass
	x.erase(x.begin()+obj);
	y.erase(y.begin()+obj);
	m.erase(m.begin()+obj);		
	nmasses--;	
	
	return;	
}

void ConnectedSystem::RemoveSpring(int obj){

	if(!CheckSpringObj(obj))
		return;
		
	ClearMatrices();			
	
	// the mass indices also change when we remove an element.
	// spring [2-4] becomes [1-3] if we remove element 0.
	s1.erase(s1.begin()+obj);
	s2.erase(s2.begin()+obj);
	k.erase(k.begin()+obj);		
	l.erase(l.begin()+obj);	
	nsprings--;	
	
	return;
}

std::vector<int> ConnectedSystem::GetObjConnections(int obj){
	
	std::vector<int> indx;
	// returns the spring indices, not the mass indices
	for(int i=0; i<nsprings; i++)
		if(s1.at(i)==obj || s2.at(i)==obj)
			indx.push_back(i);
		
	return indx;
}

void ConnectedSystem::GetMassXYM(int obj, double &xx, double &yy, double &mm){
	
	if(!CheckMassObj(obj))
		return;
	
	xx = x.at(obj);
	yy = y.at(obj);
	mm = m.at(obj);
	return ;
}

double ConnectedSystem::GetMassX(int obj){
	
	if(!CheckMassObj(obj))
		return 0;
	
	return x.at(obj);
}

double ConnectedSystem::GetMassY(int obj){

	if(!CheckMassObj(obj))
		return 0;
	
	return y.at(obj);
}

double ConnectedSystem::GetMassM(int obj){

	if(!CheckMassObj(obj))
		return 0;
	
	return m.at(obj);
}

bool ConnectedSystem::CheckMassObj(int obj){

	if(obj<0 || obj>=nmasses){
		printf("\n\t Error [Mass] :  index %i out of range. Must be in range 0-%i!\n",obj,nmasses-1);
		return false;
	} else{
		return true;
	}
}

bool ConnectedSystem::CheckSpringObj(int obj){

	if(obj<0 || obj>=nsprings){
		printf("\n\t Error [Spring] :  index %i out of range. Must be in range 0-%i!\n",obj,nsprings-1);
		return false;
	} else{
		return true;
	}
}

bool ConnectedSystem::CheckEigenMode(int mode){

	if(mode<0 || mode>=nmodes){
		printf("\n\t Error [Mode] :  index %i out of range. Must be in range 0-%i!\n",mode,nmodes-1);
		return false;
	} else{
		return true;
	}
}

void ConnectedSystem::AddSpring(int obj1, int obj2, double kk){

	if(!CheckMassObj(obj1) || !CheckMassObj(obj2))
		return;
		
	if(obj1 == obj2){
		printf("\n\t Error [Spring]:  A spring must connect two different masses, %i-%i is invalid!\n",obj1,obj2);
		return;
	}
	
	if(kk<=0){
		printf("\n\t Error [Spring]:  Spring must have a positive non-zero spring constant, %f is invalid!\n",kk);
		return;
	}

	s1.push_back(obj1);
	s2.push_back(obj2);	
	k.push_back(kk);
	l.push_back(GetDist(obj1,obj2));
	nsprings++;
	
	return;
}

double ConnectedSystem::GetDist(int obj1, int obj2){

	if(!CheckMassObj(obj1) || !CheckMassObj(obj2))
		return 0;

	return sqrt(pow(x.at(obj2)-x.at(obj1),2.0)+pow(y.at(obj2)-y.at(obj1),2.0));
}

void ConnectedSystem::GetSpringObjs(int obj, int &obj1, int &obj2){

	if(!CheckSpringObj(obj))
		return;
		
	obj1 = s1.at(obj);
	obj2 = s2.at(obj);
	return;		
}

double ConnectedSystem::GetSpringL(int obj){

	if(!CheckSpringObj(obj))
		return 0;

	return l.at(obj);
}

double ConnectedSystem::GetSpringK(int obj){

	if(!CheckSpringObj(obj))
		return 0;

	return k.at(obj);
}		

void ConnectedSystem::Print(){

	printf("\n\n- - - - - - - - - - - - - - - - - - - - - - - - - - -");
	printf("\n Masses [total = %i] :-",nmasses);
	for(int i=0; i<nmasses; i++){
		if(i==0)	printf("\n\n\t obj\t x\t y\t m  ");
		printf("\n\t %4i\t %3.1f\t %3.1f\t %3.1f",i,x.at(i),y.at(i),m.at(i));
	}
	
	printf("\n\n\n Springs [total = %i] :-",nsprings);
	for(int i=0; i<nsprings; i++){
		if(i==0)	printf("\n\n\t obj\t   masses \t l\t  k  ");
		printf("\n\t %3i\t %3i -%3i\t %3.1f\t %3.1f",i,s1.at(i),s2.at(i),l.at(i),k.at(i));	
	}
	
	printf("\n\n\n Modes [total = %i] :-",nmodes);
	for(int i=0; i<nmodes; i++){
		if(i==0)	printf("\n\n\t mode\t freq \t");
		printf("\n\t %3i\t %5.3f",i,E(i));	
	}
	
	printf("\n- - - - - - - - - - - - - - - - - - - - - - - - - - - -\n\n");
	return;
}

void ConnectedSystem::Clear(){

	nmasses  = 0;	
	x.clear();
	y.clear();
	m.clear();
	
	nsprings = 0;	
	s1.clear();
	s2.clear();
	k.clear();
	l.clear();
	
	ClearMatrices();
	motionscale = 1;	

	return;
}

void ConnectedSystem::ClearMatrices(){
	nmodes = 0;
	Z.reset(); 
	K.reset(); 
	L.reset(); 
	F.reset(); 
	E.reset(); 
	Ax.reset();
	Ay.reset();
}
		
