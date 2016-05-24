#include <iostream>
#include "ConnectedSystem.h"

int main(){
	
	ConnectedSystem sys;
	
	sys.AddMass(0.0,0.0,1.0);	
	sys.AddMass(1.0,0.0,1.0);	
	sys.AddMass(2.0,0.0,1.0);	

//	sys.AddMass(5.0,5.0,100.0);	
//	sys.AddMass(10.0,5.0,100.0);
//	sys.AddMass(5.0,10.0,100.0);
//	sys.AddMass(10.0,10.0,100.0);	
		
	sys.AddSpring(0,1,10.0);
	sys.AddSpring(1,2,10.0);

//	sys.AddSpring(0,2,30.0);
//	sys.AddSpring(3,2,100.0);	
//	sys.AddSpring(2,1,60.0);
//	sys.AddSpring(3,0,10.0);	
		
	sys.Print();
	
//	sys.RemoveMass(1);
			
//	sys.Print();
	
	sys.SolveMatrix();
			
	sys.Print();

/*
	double t,x,y;
	for(int i=0; i<50; i++){
		t = (double)i*2*3.1415/50.0;
		sys.GetMassEigenMotion(0,0,t,x,y);
		printf("\n\t time = %5.3f\t x[0] = %5.3f\t y[0] = %5.3f",t,x,y);
	}
*/
	
	sys.SetDebug(true);
	sys.BuildMassGrid(3,2,50,100,50,100,true);
	sys.SolveMatrix();
	
	sys.Print();
	
	return 0;
	sys.BuildMassPoly(4,50,50,50,1);
	sys.Print();
		
//	sys.SolveMatrix();
//	sys.Print();
	
	sys.BuildMassRand(10,0,100,0,100,0);
	sys.SolveMatrix();
	sys.Print();	
	
	printf("\n\t Complete! \n\n");
	return 0;
}


