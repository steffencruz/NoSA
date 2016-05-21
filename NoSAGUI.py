"""NoSA GUI"""

import sys
from consys import consys
import numpy as np
from PyQt4 import QtCore, QtGui


class NoSAGUI(QtGui.QMainWindow):
    
    def __init__(self):
        super(NoSAGUI, self).__init__()
        
        self.initUI()
        
        
    def initUI(self):    

        self.abox = AnimationBox(self)
        self.setCentralWidget(self.abox)

                
        self.resize(500, 500)
        self.setWindowTitle('NoSA')        
        self.show()


# class NoSAGUI(QtGui.QMainWindow):
    
#     def __init__(self):
#         super(NoSAGUI, self).__init__() #call init of QtGui.QWidget
#         self.initUI() #call init function of this class
    
#     def initUI(self):
#         #build window
        
#         button1 = QtGui.QPushButton("Add Mass")
#         #button2 = QtGui.QPushButton("Sine")
#         self.label1 = QtGui.QLabel("")
        
#         button1.clicked.connect(self.buttonClicked)
#         #button2.clicked.connect(self.buttonClicked)


#         #hbox = QtGui.QHBoxLayout()
#         #hbox.addStretch(1)
#         #hbox.addLayout(button1)
#         #hbox.addLayout(button2)        
        
        
#         hbox = QtGui.QHBoxLayout()
#         buttonbox = QtGui.QVBoxLayout()
#         imbox = QtGui.QVBoxLayout()
#         hbox.addStretch(1)
#         ##vbox.addLayout(hbox)
#         #vbox.addWidget(imbox)
#         #vbox.addWidget(buttonbox)
#         buttonbox.addWidget(button1)
#         #vbox.addWidget(button2)  
#         #buttonbox.addWidget(self.label1)
#         imbox.addWidget(self.label1)
        
#         hbox.addLayout(imbox)        
#         hbox.addLayout(buttonbox)  

#         self.setLayout(hbox)      
        
#         self.setGeometry(0,0,500,500) #setGeometry is inherited from QWidget
#         self.setWindowTitle('NoSA GUI') #set title
#         self.show() #show window
#         self.label1.setText("asdf2")
        
#     def buttonClicked(self):
#         sender = self.sender()
#         if sender.text() == "Add Mass":
#             self.label1.setText("Add Mass")
 

class AnimationBox(QtGui.QFrame):
    BoxWidth = 400
    BoxHeight = 400

    MassList = []
    SpringList = []

    def __init__(self, parent):
        super(AnimationBox, self).__init__(parent)

        self.initAnimationBox()

    def initAnimationBox(self):
        self.MassList.append((100.,100., 20.))
        self.MassList.append((350.,120., 25.))
        self.MassList.append((100.,120., 14.))
        self.MassList.append((130.,100., 12.))
        self.SpringList.append((1,2,1.))

    def paintEvent(self, event):
        painter = QtGui.QPainter(self)
        
        self.drawSpring(painter,100., 100., 350., 120, 1.)

        for mass in self.MassList:
            self.drawMass(painter, *mass)

        #path = QtGui.QPainterPath()
        #path.moveTo(0,0) #move to start
        #path.lineTo(100,100)
        #painter.setPen(QtGui.QPen(QtGui.QColor(QtGui.darkGreen), 3))
        #painter.drawPath(path)
        #draw springs




    def drawMass(self, painter, x, y, m): #draw a mass
    
        MassColor = 0xCC6666
        # add function that translates from x, y, m to pix_x, pix_y, size
        size = m
        color = QtGui.QColor(MassColor)
        path = QtGui.QPainterPath()
        path.addEllipse(x-size/2, y-size/2, size, size)
        painter.fillPath(path, color)

    def drawSpring(self, painter, x0, y0, x1, y1, k): #draw a spring
        """Draw a spring from (X0,Y0) to (X1,Y1) with strenght K"""
        SrpingColor = 0xAAAAAA
        color = QtGui.QColor(SrpingColor)
       

        #SrpingColor = 0xCC6666
        #color = QtGui.QColor(SrpingColor)

        loopspacing = int(30./k)
        if loopspacing < 3: # minimal 3 pix loop spacing
            loopspacing = 3

        path = QtGui.QPainterPath()
        path.moveTo(x0,y0) #move to start
        #path.lineTo(x1,y1)
        #path.moveTo(x0,y0) #move to start
        #path.lineTo(x1,y1)

        xpath,ypath = self.MakeSpringPath(x0, y0, x1, y1, loopspacing)

        for (x,y) in zip(xpath[1:],ypath[1:]): #skip the first point, we already entered it
            path.lineTo(x,y)
        pen = QtGui.QPen(color)
        pen.setWidth(3.)
        painter.setPen(pen)
        painter.drawPath(path)


    def MakeSpringPath(self, x0, y0, x1, y1, spacing):
        """Function to calculate the spring shape path and return a np array containing the path"""
        totlen = np.sqrt((x1-x0)**2+(y1-y0)**2)
        
        prelen = 10. #lenght before loops at each side of the srping in pix
        
        sprlen = totlen-2*prelen
        if sprlen > 0:  
            loops = int(sprlen/spacing)
            excess = sprlen%spacing
            
            xpts = np.zeros(5 + loops)
            ypts = np.zeros(5 + loops)
            yoffset = 10. #the deviation to both sides in y
            #start
            xpts[2] = spacing/2
            ypts[2] = yoffset
        
            for i in range(loops):
                xpts[i+3] = (0.5 + i)*spacing
                if i%2 == 1: #odd
                    ypts[i+3] = -yoffset
                else:
                    ypts[i+3] = +yoffset
        
            #end
            xpts[loops+3] = spacing*loops
            ypts[loops+3] = 0.
            
            xpts[1:] = xpts[1:]+ excess/2.+prelen
            
            xpts[loops+4] = totlen
            ypts[loops+4] = 0.
        else: #just a straight line
            xpts = np.zeros(2)
            ypts = np.zeros(2)
            xpts[1] = totlen
        
        
        #rotate
        th = np.arctan2((y1-y0),(x1-x0))

        
        xptsrot = xpts*np.cos(th) - ypts*np.sin(th)
        yptsrot = xpts*np.sin(th) + ypts*np.cos(th)
        xptsrot += x0
        yptsrot += y0
        
        return (xptsrot, yptsrot)











def main():


    #test system
    cs = consys()

    aa = 2.0 #mass spacing
    mm = 1.0 #mass
    kk = 3.0 #spring const
    nmass = 10

    #add masses
    for ii in range(nmass):
        cs.AddMass(ii*aa, 0., mm)


    for ii in range(nmass-1):
        cs.AddSpring(ii, ii+1, kk)

    #show initialized system
    cs.Print()

    app = QtGui.QApplication(sys.argv)
    NoSA = NoSAGUI()
    sys.exit(app.exec_())
    
    
if __name__ == '__main__':
    main()

