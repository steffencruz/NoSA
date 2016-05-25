
from libcpp.vector cimport vector
from libcpp cimport bool



cdef extern from "ConnectedSystem.h":
    cdef cppclass ConnectedSystem:
        ConnectedSystem() except +
        
        void SetupMatrix();
        void SolveMatrix();
        
        void SetMotionScale(double)
        void GetMassEigenMotion(int, int, double, double, double)
        void GetSpringEigenMotion(int, int, double, double, double, double, double)

        void AddMass(double, double, double)
        void RemoveMass(int)
        int GetNumberOfMasses()
        double GetMassX(int)
        double GetMassY(int)
        double GetMassM(int)
        void SetMassX(int, double)
        void SetMassY(int, double)
        void SetMassM(int, double)
        
        void GetMassXYM(int, double, double, double);      
        vector[int] GetObjConnections(int)
    
        void AddSpring(int, int, double)
        void RemoveSpring(int)
        int GetNumberOfSprings()
        void GetSpringObjs(int, int, int) 
        double GetSpringL(int)
        double GetSpringK(int)      
        double GetDist(int, int)

        void BuildMassGrid(int, int, double, double, double, double, bool)
#        void BuildMassRand(int, double, double, double, double, int)
        void BuildMassPoly(int, double, double, double, int)
    
        void BuildSpringChain(double, int , int , int )
        void BuildSpringNest(double )

        bool CheckMassObj(int)
        bool CheckSpringObj(int)   
        void SetDebug(bool)    
        void Print()
        void Clear()





cdef class consys:
    cdef ConnectedSystem *csptr      # hold a C++ instance which we're wrapping
    def __cinit__(self):
        self.csptr = new ConnectedSystem()
    def __dealloc__(self):
        del self.csptr


    def SolveMatrix(self):
        self.csptr.SolveMatrix();
        
    def BuildMassGrid(self, nrows, ncols, xmin, xmax, ymin, ymax, connect):
        self.csptr.BuildMassGrid(nrows, ncols, xmin, xmax, ymin, ymax, connect);
#    def BuildMassRand(self, nparticles, xmin, xmax, ymin, ymax, spr_type):
#        self.csptr.BuildMassRand(nparticles,xmin,max,ymin,ymax,spr_type);
    def BuildMassPoly(self, nsides, length, xmid, ymid, spr_type):
        self.csptr.BuildMassPoly(nsides,length,xmid,ymid,spr_type);

		        
    def BuildSpringChain(self, kval, rule, obj_from, ntimes):
        self.csptr.BuildSpringChain(kval,rule,obj_from,ntimes);
    def BuildSpringNest(self,kval):
        self.csptr.BuildSpringNest(kval);   
                
    def SetMotionScale(self, scale):
        self.csptr.SetMotionScale(scale)
    def GetMassEigenMotion(self, obj, mode, t):
        cdef double xx, yy
        self.csptr.GetMassEigenMotion(obj, mode, t, xx, yy)
        return (xx, yy)
    def GetSpringEigenMotion(self, obj, mode, t):
        cdef double xx1, yy1, xx2, yy2
        self.csptr.GetSpringEigenMotion(obj, mode, t, xx1, yy1, xx2, yy2)
        return (xx1, yy1, xx2, yy2)


    def AddMass(self, xx, yy, mm):
        self.csptr.AddMass(xx, yy, mm)
    def RemoveMass(self, obj):
        self.csptr.RemoveMass(obj)

    def GetNumberOfMasses(self):
        return self.csptr.GetNumberOfMasses()
    def GetMassX(self, obj):
        return self.csptr.GetMassX(obj)
    def GetMassY(self, obj):
        return self.csptr.GetMassY(obj)
    def GetMassM(self, obj):
        return self.csptr.GetMassM(obj)
    def SetMassX(self, obj,xx):
        self.csptr.SetMassX(obj, xx)                
    def SetMassY(self, obj, yy):
        self.csptr.SetMassY(obj, yy)        
    def SetMassM(self, obj, mm):
        self.csptr.SetMassM(obj, mm)        
    def GetMassXYM(self, obj):
        cdef double xx, yy, mm
        self.csptr.GetMassXYM(obj, xx, yy, mm);
        return (xx,yy,mm)


    def GetObjConnections(self, obj):
        return self.csptr.GetObjConnections(obj)
    def AddSpring(self, obj1, obj2, kk):
        self.csptr.AddSpring(obj1, obj2, kk)
    def RemoveSpring(self, obj):
        self.csptr.RemoveSpring(obj)
    def GetNumberOfSprings(self):
        return self.csptr.GetNumberOfSprings()     
    def GetSpringObjs(self, spring):
        cdef int obj1, obj2
        self.csptr.GetSpringObjs(spring, obj1, obj2) 
        return (obj1, obj2)
    def GetSpringL(self, obj):
        return self.csptr.GetSpringL(obj)
    def GetSpringK(self, obj):
        return self.csptr.GetSpringK(obj)
    def GetDist(self, obj1, obj2):    
        return self.csptr.GetDist(obj1, obj2)
    def GetSpringXYXYK(self, obj):
        (mobj1, mobj2) = self.GetSpringObjs(obj)
        (x1, y1, m1) = self.GetMassXYM(mobj1)
        (x2, y2, m2) = self.GetMassXYM(mobj2)
        k = self.csptr.GetSpringK(obj)
        return (x1, y1, x2, y2, k)

    def CheckMassObj(self, obj):
        return self.csptr.CheckMassObj(obj)
    def CheckSpringObj(self, obj):
        return self.csptr.CheckSpringObj(obj)
    def Print(self):   
        self.csptr.Print()
    def Clear(self):
        self.csptr.Clear()
    def SetDebug(self,flag):
        self.csptr.SetDebug(flag)    
