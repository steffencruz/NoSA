#include <iostream>
#include <vector>
#include <armadillo>

#ifndef CONSYS_H
#define CONSYS_H

class ConnectedSystem 
{
	public:
		ConnectedSystem();
		virtual ~ConnectedSystem();
		
		void SetupMatrix();
		void SolveMatrix();
		
		void AddMass(double xx, double yy, double mm);
		void RemoveMass(int obj);
		int GetNumberOfMasses(){ return nmasses; }
		void GetMassXYM(int obj, double &xx, double &yy, double &mm);
		double GetMassX(int obj);
		double GetMassY(int obj);
		double GetMassM(int obj);		
		std::vector<int> GetObjConnections(int obj);
	
		void AddSpring(int obj1, int obj2, double kk);	
		void RemoveSpring(int obj);
		int GetNumberOfSprings(){ return nsrings; }		
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
		int nsprings;	
		std::vector<int>    s1;
		std::vector<int>    s2;	
		std::vector<double> l;
		std::vector<double> k;
		
	private:
		arma::vec Z; // combined position vector of masses
		arma::mat K; // reduced stiffness / inertia matrix
		arma::mat L; // length matrix
		arma::mat F; // force matrix
		
		arma::vec W ; // eigenvalues vector
		arma::mat V;  // eigenvectors matrix
		
};

#endif