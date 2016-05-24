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
		
		int GetNumberOfMasses()	{ return nmasses;  }
		int GetNumberOfSprings(){ return nsprings; }		
		
		void SolveMatrix();	
		void SetMotionScale(double scale);
		void GetMassEigenMotion(int obj, int mode, double t, double &xx, double &yy);
		void GetSpringEigenMotion(int obj, int mode, double t, double &xx1, double &yy1, double &xx2, double &yy2);																						
		
		void AddMass(double xx, double yy, double mm);
		void RemoveMass(int obj);
		void GetMassXYM(int obj, double &xx, double &yy, double &mm);
		double GetMassX(int obj);
		double GetMassY(int obj);
		double GetMassM(int obj);						
		void SetMassX(int obj, double xx);
		void SetMassY(int obj, double yy);
		void SetMassM(int obj, double mm);			
		std::vector<int> GetObjConnections(int obj);
	
		void AddSpring(int obj1, int obj2, double kk);	
		void RemoveSpring(int obj);
		void GetSpringObjs(int obj, int &obj1, int &obj2);
		double GetSpringL(int obj);
		double GetSpringK(int obj);			
		double GetDist(int obj1, int obj2);

		// quick fillers:
		// 	BuildMassGrid -  makes a rectangular lattice which can be filled with springs
		// 	BuildMassRand -  makes a random distribution which can be filled with random springs
		// 	BuildMassPoly -  makes a polygon distribution which can be filled with springs

		void BuildMassGrid(int nrows, int ncols, double xmin, double xmax, double ymin, double ymax, bool connect);
	//	void BuildMassRand(int nparticles, double xmin, double xmax, double ymin, double ymax, int spr_type);		
		void BuildMassPoly(int nsides, double length, double xmid, double ymid, int spr_type);
		
		void BuildSpringChain(double kval, int rule=1, int obj_from=0, int ntimes=0); 
		void BuildSpringNest(double kval);	

		void SetDebug(bool flag = true) { debug = flag; }
		bool CheckMassObj(int obj);
		bool CheckSpringObj(int obj);		
		bool CheckEigenMode(int mode);
				
		void Print();
		void Clear();				
		
	private:
		void SetupMatrix();
		void ClearMatrices();		
		
	private: 
		bool debug;
	
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