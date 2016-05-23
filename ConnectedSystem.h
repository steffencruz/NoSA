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
		
		void SetMotionScale(double scale);
		void GetMassEigenMotion(int obj, int mode, double t, double &xx, double &yy);
		void GetSpringEigenMotion(int obj, int mode, double t, double &xx1, double &yy1,
																													double &xx2, double &yy2);																						
		
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
		int GetNumberOfSprings(){ return nsprings; }		
		void GetSpringObjs(int obj, int &obj1, int &obj2);
		double GetSpringL(int obj);
		double GetSpringK(int obj);			
		double GetDist(int obj1, int obj2);

		bool CheckMassObj(int obj);
		bool CheckSpringObj(int obj);		
		bool CheckEigenMode(int mode);		
		void Print();
		void Clear();
		void ClearMatrices();
		
	private: 
		int nmasses;
		std::vector<double> x;
		std::vector<double> y;
		std::vector<double> m;
		
		int nsprings;	
		std::vector<int>    s1;
		std::vector<int>    s2;	
		std::vector<double> l;
		std::vector<double> k;
		
	private: // Armadillo
		int nmodes;
		double motionscale;
		arma::vec Z; // combined position vector of masses
		arma::mat K; // reduced stiffness / inertia matrix
		arma::mat L; // length matrix
		arma::mat F; // force matrix
		
		arma::vec E; // eigenvalues vector
		arma::mat Ax; // X eigenvectors matrix	
		arma::mat Ay; // Y eigenvectors matrix	
};

#endif