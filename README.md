# OpenGL-Renderer
![](./doc/demo.gif)
## Model Renderer using OpenGL and GLFW
### Features:
* Geometric shape rendering
* Model loading using [Assimp](http://assimp.org/)
* Directional and Point Light Sources
* Blinn-Phong lighting model with linear gamma correction
* Directional shadow mapping
* Omnidirectional shadow maps
* Normal mapping
* Parallax mapping
* HDR and Bloom
### Implementation:
This renderer uses [OpenGL version 3.3](https://registry.khronos.org/OpenGL/specs/gl/glspec33.core.pdf) which is very widely supported. [GLAD](https://github.com/Dav1dde/glad) is used to handle the locating of driver-specific functions, enabling interoperability between different systems. [GLFW](https://www.glfw.org/) is used to provide easy functionality for creating an OpenGL instance, handling user input, and displaying our buffer to a window. 
### Structure:
The renderer is structured as follows:
```
engine ┰ camera
       ┃
       ┠ scene ┰ shaders ┰ lightingShader
       ┃       ┃         ┠ directionalShadowShader
       ┃       ┃         ┠ pointShadowShader
       ┃       ┃         ┖ skyboxShader
       ┃       ┃
       ┃       ┠ objects ┰ texture
       ┃       ┃         ┖ vertices
       ┃       ┃
       ┃       ┠ models ─ meshs ┰ diffuseTexture
       ┃       ┃                ┠ specularTexture
       ┃       ┃                ┠ normalTexture
       ┃       ┃                ┖ vertices
       ┃       ┃
       ┃       ┠ depthMaps ┰ directionalDepthMap
       ┃       ┃           ┖ pointDepthMap
       ┃       ┃
       ┃       ┖ lights ┰ directionalLight
       ┃                ┖ pointLights
       ┃
       ┖ inputs ┰ keyboard
                ┖ mouse
```
#### Overview:
The engine handles the inputs from the keyboard and mouse, and therefore also handles the control of the camera. The camera and inputs are passed to the scene, which is rendered every frame. The scene contains a mixture of objects, models, and lights. In addition, there are various shaders attached to the scene that are used for different rendering conditions - like the generation of shadows.
#### Shaders
There are four types of shaders used in the main rendering loop. The `directionalShadowShader` performs the rendering for the depth map used in generating directional shadows. The `pointShadowShader` performs the rendering for the depth cube map used in generating point shadows. The `skyboxShader` is used to render the optional skybox, to add some more life to the scene. And finally, the `lightingShader` does most of the heavy lifting, implementing the Blinn-Phong lighting model.
#### Shadow Mapping
To generate both directional and point shadows, a shadow map is used - the scene is rendered from the point of view of the light source, and only the depth buffer is rendered. This allows each fragment to determine whether or not it is blocked from the light source. 

For directional light sources, an orthographic projection matrix is used to render the depth buffer of a scene. For point sources, a cube map is used instead. 
### Model Loading
Model loading is performed used [Assimp](http://assimp.org/), which allows for the importing of models in a multitude of formats. 
## Building and Running:
To - Do, for now just enjoy the cool GIF :)
