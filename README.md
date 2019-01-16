# Chimera Engine

Engine created by [Alfonso Mestres Muñoz](https://github.com/AlfonsoMestres) in the UPC Master's degree in
game programming and development.

![ChimeraEnginev2](preview.gif)

For more visual examples visit the [Last Release Notes](https://github.com/AlfonsoMestres/Chimera-Engine/releases/tag/2.0)

##  Controls:

### Scene
* Right click + “WASD” to move camera around.
* Right click to rotate the camera.
* Alt + left click to orbit the camera.
* Mouse wheel to zoom in/out.
* F to center the camera into the last loaded object.
* SHIFT + Movement to duplicate the movement speed.
* You can move, rotate and scale selected elements using ImGuizmo in the scene viewport.

### Assets
* Drag and drop .fbx to generate a .HEAD file. This file will be read by the engine to load the mesh tree correctly.
* Drag and drop .png .tiff to import them as .dds files, this files will be used by chimera to set up materials.
* You can also copy your files into the Assets folder and Chimera will import them automatically.

### Inspector
* Each element selected will contain diferent components:
  - Info: This will display the current uuid of this element and his GameObject father.
  - Transform: The location of the element in the world, among other properties (and ImGuizmo edit menu)
  - Camera: Here you can select if the camera is rendering in debug mode, wireframe, select as Primary, etc.
  - Mesh: It will display a dropdown with the current loaded .heads in Chimera. FEED CHIMERA! DRAG and DROP your .Fbx!
  - Material: It contains a dropdown for the Diffuse/Ambient/Specular/Emissive Map, can be removed.

### Hierarchy
* You can select every object in the scene here, it will display their configuration in the inspector.
* Right button to select one of the options.
- Add Empty Object
- Duplicate
- Remove
- Move up/down

### Renderer
* PhongBlinn shader is used in the scene
* You can load a Diffuse , Ambient, Specular and Emissive Maps with the .dds files imported.

### Light
* Edit the intensity of the light 
* Move it around the World

### QuadTree
* Select the Checkbox Draw to render the QuadTree in the scene.
* Once selected it will appear an Orthographic camera displaying the current mesh.

### Configuration
* Framerate, Vsync, Scene camera configuration, etc

## Libraries

* STL.
* [SDL](https://www.libsdl.org/index.php).
* [ImGui](https://github.com/ocornut/imgui).
* [OpenGL](https://www.opengl.org/).
* [MathGeolib](https://github.com/juj/MathGeoLib).
* [Assimp](http://www.assimp.org/).
* [DevIL](http://openil.sourceforge.net/).
* [mmgr](http://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml).
* [CrossGuid](https://github.com/graeme-hill/crossguid)
* [DebugDraw](https://github.com/glampert/debug-draw)
* [PhysicsFS](https://icculus.org/physfs/)
* [RapidJson](https://github.com/Tencent/rapidjson)
* [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo)
* [Brofiler](https://github.com/bombomby/brofiler)

## License

MIT License

Copyright (c) 2018 Alfonso Mestres Muñoz

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.