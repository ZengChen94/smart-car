import matplotlib.pyplot as plt
import math

plt.ion()
class DynamicUpdate():
    #Suppose we know the x range
    min_x = -100
    max_x = 100

    def on_launch(self):
        #Set up plot
        self.figure, self.ax = plt.subplots()
        self.lines, = self.ax.plot([],[], 'o-')
        #Autoscale on unknown axis and known lims on the other
        self.ax.set_autoscaley_on(True)
        self.ax.set_xlim(self.min_x, self.max_x)
        #Other stuff
        self.ax.grid()
        ...

    def on_running(self, xdata, ydata):
        #Update data (with the new _and_ the old points)
        self.lines.set_xdata(xdata)
        self.lines.set_ydata(ydata)
        #Need both of these in order to rescale
        self.ax.relim()
        self.ax.autoscale_view()
        #We need to draw *and* flush
        self.figure.canvas.draw()
        self.figure.canvas.flush_events()

    #Example
    def __call__(self):
        import numpy as np
        import time

        self.on_launch()

        f = open("test.txt")

        line = f.readline()
        line = line[0:-1]
        acceleration = line.split(" ")
        #print (acceleration)
        line = f.readline()
        line = line[0:-1]
        angular = line.split(" ")
        #print (angle)

        xvelocity = []
        yvelocity = []
        xvelocity.append(10*float(acceleration[0]))
        yvelocity.append(10*float(acceleration[1]))
        #print (float(acceleration[0]), type(float(acceleration[0])))

        angle = []
        angle.append(float(angular[2]))

        xdata = []
        ydata = []
        xdata.append(0)
        ydata.append(0)

        while line:
            #refresh xdata and ydata
            xdatatmp = xdata[-1] + xvelocity[-1] * math.cos(angle[-1]/math.pi)
            ydatatmp = ydata[-1] + yvelocity[-1] * math.sin(angle[-1]/math.pi)
            xdata.append(xdatatmp)
            ydata.append(ydatatmp)

            self.on_running(xdata, ydata)
            time.sleep(0.5)

            line = f.readline()
            line = line[0:-1]
            acceleration = line.split(" ")
            line = f.readline()
            line = line[0:-1]
            angular = line.split(" ")

            #refresh velocity / angle
            xvelocity.append(xvelocity[-1]+10*float(acceleration[0]))
            yvelocity.append(yvelocity[-1]+10*float(acceleration[1]))
            angle.append(angle[-1]+float(angular[2]))
            print (float(angular[2]))
        #return xdata, ydata

d = DynamicUpdate()
d()