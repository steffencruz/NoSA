#include<iostream>
#include "ConnectedSystem.h"

int main(){
	
	ConnectedSystem sys;
	
	sys.AddMass(10.0,5.0,100.0);
	sys.AddMass(5.0,10.0,100.0);
	sys.AddMass(5.0,5.0,100.0);
	sys.AddMass(10.0,10.0,100.0);	
	
	sys.Print();
	
	sys.AddSpring(0,1,30.0);
	sys.AddSpring(0,2,30.0);
	sys.AddSpring(3,2,100.0);	
	sys.AddSpring(2,1,60.0);
	sys.AddSpring(3,0,10.0);	
		
	sys.Print();
	
	sys.RemoveMass(2);
			
	sys.Print();
			
	return 0;
}


