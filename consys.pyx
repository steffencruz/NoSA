
from libcpp.vector cimport vector
from libcpp cimport bool



cdef extern from "ConnectedSystem.h":
    cdef cppclass ConnectedSystem:
        ConnectedSystem() except +
        
        void SetupMatrix();
        void SolveMatrix();
        
        void GetMassEigenMotion(int, int, double, double, double)
        void GetSpringEigenMotion(int, int, double, double, double, double, double)

        void AddMass(double, double, double)
        void RemoveMass(int)
        int GetNumberOfMasses()
        double GetMassX(int)
        double GetMassY(int)
        double GetMassM(int)
        void GetMassXYM(int, double, double, double);      
        vector[int] GetObjConnections(int)
    
        void AddSpring(int, int, double)
        void RemoveSpring(int)
        int GetNumberOfSprings()
        void GetSpringObjs(int, int, int) 
        double GetSpringL(int)
        double GetSpringK(int)      
        double GetDist(int, int)

        bool CheckMassObj(int)
        bool CheckSpringObj(int)       
        void Print()
        void Clear()





cdef class consys:
    cdef ConnectedSystem *csptr      # hold a C++ instance which we're wrapping
    def __cinit__(self):
        self.csptr = new ConnectedSystem()
    def __dealloc__(self):
        del self.csptr


    def SetupMatrix(self):
        self.csptr.SetupMatrix()
    def SolveMatrix(self):
        self.csptr.SolveMatrix();
        
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
