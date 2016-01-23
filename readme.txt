VU COMPUTERGRAPHICS LAB4B
Andreas Fuchs: a1106307


SOURCECODE:
This code is my own work excluding the used libraries
- TinyXML-2 (www.grinninglizard.com/tinyxml2/index.html )
- OpenGL Mathematics ( http://glm.g-truc.net/0.9.7/index.html )
All libraries are located in src/lib/


CODESTRUCTURE:
src/ -> source code
obj/ -> compiled object files
bin/ -> executable
scenes/ -> example scenes


COMPILATION AND RUNNING THE EXECUTABLE:
To compile this program simply use the provided Makefile. You may also want to clean before compiling (make clean).
No libraries that are not included are required so it should compile on any system.
The executable a1106307_lab4 can be found in the bin directory.
The run the program pass the location of the .xml to the program as commandline argument, eg: ./a1106307_lab4 example.xml.
The created image will be saved as an ascii ppm image. Note that the filesize can be quite immense.


IMPLEMENTATION:

Supersampling:
Supersampling uses the Jitter algorithm. To use supersampling insert <supersampling rate="x"/> into the camera node in the xml file. A samplerate < 1 will be interpreted as to not use supersampling. 
Note: When not using super sampling rays are shot through the center of the pixel while super sampling shoots the ray through a random position within the grid. Therefore the image may look slightly different even when using a sample rate of 1 where the grid equals the pixels.

Spot Light:
Spot Lights are used whenever a spot light is specified in the xml.


IMPORTANT REFERENCES:
- Materials from the VU Computergraphics course
- Angel & Shreiner: Interactive Computer Graphics
- https://steveharveynz.wordpress.com/2012/12/20/ray-tracer-part-two-creating-the-camera/
- http://geomalgorithms.com/a06-_intersect-2.html
- http://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf
- https://en.wikipedia.org/wiki/Supersampling
- cplusplus.com, stackoverflow.com and Breymann: Der c++ Programmierer 	used whenever i forgot how to programm - which was a lot

