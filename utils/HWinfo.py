import wmi
import sys
from time import sleep


class HWinformation:
    def __init__(self):
        self.w = wmi.WMI(namespace="root/OpenHardwareMonitor")
        self.keys = ["CPU Temp", "CPU Power", "CPU Usage", "GPU Temp", "GPU Power",
                     "GPU Usage", "Memory Usage", "GPU Memory"]
        self.HWI = dict.fromkeys(self.keys)

    def read(self):
        infos = self.w.Sensor()
        for sensor in infos:
            #print(sensor)
            if sensor.SensorType == u'Temperature':
                if sensor.Name == 'CPU Package':
                    result = str("%.2f" % sensor.Value)
                    self.HWI.update({'CPU Temp': result})
                if sensor.Name == 'GPU Core':
                    result = str("%.2f" % sensor.Value)
                    self.HWI.update({'GPU Temp': result})
            if sensor.SensorType == u'Power':
                if sensor.Name == 'CPU Package':
                    result = str("%.1f" % sensor.Value)
                    self.HWI.update({'CPU Power': result})
                if sensor.Name == 'GPU Power':
                    result = str("%.1f" % sensor.Value)
                    self.HWI.update({'GPU Power': result})
            if sensor.SensorType == u'Load':
                if sensor.Name == 'CPU Total':
                    result = str("%.1f" % sensor.Value)
                    self.HWI.update({'CPU Usage': result})
                if sensor.Name == 'GPU Core':
                    result = str("%.1f" % sensor.Value)
                    self.HWI.update({'GPU Usage': result})
                if sensor.Name == 'Memory':
                    result = str("%.1f" % sensor.Value)
                    self.HWI.update({'Memory Usage': result})
                if sensor.Name == 'GPU Memory':
                    result = str("%.1f" % sensor.Value)
                    self.HWI.update({'GPU Memory': result})
        #print("INFO: ", self.HWI)
        return self.HWI
