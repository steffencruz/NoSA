# distutils: language = c++
# distutils: sources = ConnectedSystem.cxx


from libcpp.vector cimport vector



cdef extern from "ConnectedSystem.h":
    cdef cppclass ConnectedSystem:
        ConnectedSystem() except +
        void AddMass(double, double, double)
        void RemoveMass(int)
        double GetMassX(int)
        double GetMassY(int)
        double GetMassM(int)      
        vector[int] GetObjConnections(int)
    
        void AddSpring(int, int, double)
        void RemoveSpring(int)
        #void GetSpringObjs(int, &int, &int) #test later
        double GetSpringL(int)
        double GetSpringK(int)      
        double GetDist(int, int)

        bool CheckMassObj(int)
        bool CheckSpringObj(int)       
        #void Print()
        #void Clear()





cdef class consys:
    cdef ConnectedSystem *csptr      # hold a C++ instance which we're wrapping
    def __cinit__():
        self.csptr = new ConnectedSystem()
    def __dealloc__(self):
        del self.csptr


    def AddMass(xx, yy, mm):
        self.csptr.AddMass(xx, yy, mm)
    def RemoveMass(obj):
        self.csptr.RemoveMass(obj)

    def GetMassX(obj):
        return self.csptr.GetMassX(obj)
    def GetMassY(obj):
        return self.csptr.GetMassY(obj)
    def GetMassM(obj):
        return self.csptr.GetMassM(obj)

    def GetObjConnections(obj):
        return self.csptr.GetObjConnections(obj)
    def AddSpring(obj1, obj2, kk):
        self.csptr.AddSpring(obj1, obj2, kk)
    def RemoveSpring(obj):
        self.csptr.RemoveSpring(obj)
    
        #void self.csptr.GetSpringObjs(int, &int, &int) #test later

    def GetSpringL(obj):
        return self.csptr.GetSpringL(obj)
    def GetSpringK(obj):
        return self.csptr.GetSpringK(obj)
    def GetDist(obj1, obj2):    
        return self.csptr.GetDist(obj1, obj2)

    def CheckMassObj(obj):
        return self.csptr.CheckMassObj(obj)
    def CheckSpringObj(obj):
        bool self.csptr.CheckSpringObj(obj)       
        #void self.csptr.Print()
        #void self.csptr.Clear()
