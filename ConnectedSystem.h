#include<iostream>
#include<vector>

#ifndef CONSYS_H
#define CONSYS_H

class ConnectedSystem 
{
	public:
		ConnectedSystem();
		virtual ~ConnectedSystem();
		
		void AddMass(double xx, double yy, double mm);
		void RemoveMass(int obj);
		double GetMassX(int obj);
		double GetMassY(int obj);
		double GetMassM(int obj);		
		std::vector<int> GetObjConnections(int obj);
	
		void AddSpring(int obj1, int obj2, double kk);	
		void RemoveSpring(int obj);
		void GetSpringObjs(int obj, int &obj1, int &obj2);
		double GetSpringL(int obj);
		double GetSpringK(int obj);			
		double GetDist(int obj1, int obj2);

		bool CheckMassObj(int obj);
		bool CheckSpringObj(int obj);		
		void Print();
		void Clear();
		
	private:
		int nmasses;
		std::vector<double> x;
		std::vector<double> y;
		std::vector<double> m;
		
	private:
	
		void UpdateSprings(int obj);
		int nsprings;	
		std::vector<int>    s1;
		std::vector<int>    s2;	
		std::vector<double> l;
		std::vector<double> k;
		
};

#endif