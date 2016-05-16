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
	
	double val1, val2, val3, dx, dy, l0;
	int i1,i2;
	std::vector<int> indx;
	
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){	
			
			val1 = 0;
			val2 = 0;
			val3 = 0;			
			if(i==j){
				printf("\n\t [%i,%i] Diagonal :  ",i,j);
			
				indx = GetObjConnections(i);
				for(int k=0; k<indx.size(); k++){
					GetSpringObjs(indx.at(k),i1,i2);
					dx = x.at(i1)-x.at(i2);
					dy = y.at(i1)-y.at(i2);					
					l0 = L(i1,i2);
					printf("\n\t\t%i - %i  dx = %.2f  dy = %.2f  l0 = %.2f  k = %.2f",i1,i2,dx,dy,l0,K(i1,i2));
					
					val1 += -K(i1,i2)*dx*dx/(l0*l0);
					val2 += -K(i1,i2)*dx*dy/(l0*l0);
					val3 += -K(i1,i2)*dy*dy/(l0*l0);
				}
			} else {
				if(K(i,j)==0)
					continue;					
				dx = x.at(i)-x.at(j);
				dy = y.at(i)-y.at(j);
				
				val1  = K(i,j)*dx*dx/(L(i,j)*L(i,j));
				val2  = K(i,j)*dx*dx/(L(i,j)*L(i,j));
				val3  = K(i,j)*dx*dx/(L(i,j)*L(i,j));	
			}
			
			F(i,j)     = val1;
			F(i+n,j)   = val2;
			F(i,j+n)   = val2;
			F(i+n,j+n) = val3;
		}
	}
	
	F.print("\nF = ");		
	return;
}

void ConnectedSystem::SolveMatrix(){
	
	SetupMatrix();

	W = arma::zeros<arma::vec>(nmasses);
	V = arma::zeros<arma::mat>(nmasses,nmasses);	
	
	bool success = arma::eig_sym(W,V,F);
	
	W.print("\nEigenvalues = ");
	
	V.print("\nEigenvectors = ");	
	
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

	return;
}

		
