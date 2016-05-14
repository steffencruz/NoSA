#include"ConnectedSystem.h"
#include<algorithm>
#include<cmath>

ConnectedSystem::ConnectedSystem(){

	Clear();
}

ConnectedSystem::~ConnectedSystem(){

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
	
	x.erase(x.begin()+obj);
	y.erase(y.begin()+obj);
	m.erase(m.begin()+obj);		
	nmasses--;	
	
	// remove all associated connections with obj
	std::vector<int> indx = GetObjConnections(obj);
	for(int i=0; i<indx.size(); i++){
		RemoveSpring(indx.at(i));
	}
	UpdateSprings(obj);
	
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

void ConnectedSystem::UpdateSprings(int obj){

	for(int i=0; i<nsprings; i++){
		if(s1.at(i)>=obj)
			s1.at(i)--;
		if(s2.at(i)>=obj)
			s2.at(i)--;			
	}
}

std::vector<int> ConnectedSystem::GetObjConnections(int obj){
	
	std::vector<int> indx;
	
	for(int i=0; i<nsprings; i++){

		if(s1.at(i)==obj || s2.at(i)==obj)
			indx.push_back(i);
	}
		
	return indx;
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

		
