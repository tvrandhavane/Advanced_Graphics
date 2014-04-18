#!/usr/bin/python
import os
import sys

print 'Display "cube_of_cubes" "framebuffer" "rgb"'
print 'Projection "perspective" "fov" 40'
print 'Format 640 480 1'
print ''
print 'Translate 0 0 5'
print 'Rotate 0 1 0 0'
print 'Rotate 90 0 1 0'
print 'Scale 1 1 1'
print ''
print 'WorldBegin'
print '  Surface "shaders/constant"'
  
count = int(sys.argv[1])
scale = 0.5 * 1.0/float(count)
for x in range(0, count):
	xOffset = 2.0 * x * scale
	for y in range (0, count):
		yOffset = 2.0 * y * scale
		for z in range (0, count):
			zOffset = 2.0 * z * scale
			print "  TransformBegin"
			print "    Translate " + str(xOffset) + " " + str(yOffset) + " " + str(zOffset)
			# print "    Sphere " + str(scale) + " -" + str(scale) + " " + str(scale) + " 360"
			print '    AttributeBegin  #begin unit cube'
			print '      Attribute "identifier" "name" "unitcube"'
			print '      Bound -.5 .5 -.5 .5 -.5 .5'
			print '      TransformBegin'
			print '        # far face'
			print '        Polygon "P" [.5 .5 .5  -.5 .5 .5  -.5 -.5 .5  .5 -.5 .5]'
			print '        Rotate 90  0 1 0'
			print '        # right face'
			print '        Polygon "P" [.5 .5 .5  -.5 .5 .5  -.5 -.5 .5  .5 -.5 .5]'
			print '        # near face'
			print '        Rotate 90  0 1 0'
			print '        Polygon "P" [.5 .5 .5  -.5 .5 .5  -.5 -.5 .5  .5 -.5 .5]'
			print '        # left face'
			print '        Rotate 90  0 1 0'
			print '        Polygon "P" [.5 .5 .5  -.5 .5 .5  -.5 -.5 .5  .5 -.5 .5]'
			print '      TransformEnd'
			print '      TransformBegin'
			print '        # bottom face'
			print '        Rotate 90  1 0 0'
			print '        Polygon "P" [.5 .5 .5  -.5 .5 .5  -.5 -.5 .5  .5 -.5 .5]'
			print '      TransformEnd'
			print '      TransformBegin'
			print '        # top face'
			print '        Rotate -90  1 0 0'
			print '        Polygon "P" [.5 .5 .5  -.5 .5 .5  -.5 -.5 .5  .5 -.5 .5]'
			print '      TransformEnd'
			print '    AttributeEnd  #end unit cube'
			print "  TransformEnd"

print 'WorldEnd'
