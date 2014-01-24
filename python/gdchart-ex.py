# encoding: utf-8
import cgitb; cgitb.enable()
import gdchart
import sys
gd = gdchart.Bar3D()
gd.width = 250
gd.height = 250
gd.xtitle = "Weekday"
gd.ytitle = "Percentage"
gd.title = "Example Graph"
gd.ext_color = [ "white", "yellow", "red", "blue", "green"]
gd.setData([20, 100, 80, 30, 50])
gd.setLabels(["Mon", "Tue", "Wed", "Thu", "Fri"])
print 'Content-Type: image/png\n'
sys.stdout.flush()
gd.draw(sys.stdout)
sys.stdout.flush()
