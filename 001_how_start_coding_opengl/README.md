# 1) ¿Qué es un contexto OpenGL?

Un contexto OpenGL son muchas cosas se concidera la parte principal de una aplicación OpenGL, el contexto es el encargado de almacenar todos los estados asociados a la instancia actual de OpenGL, se puede pensar en un contexto OpenGL como el objeto que almacena a toda la aplicación OpenGL.

from [docs](https://www.khronos.org/opengl/wiki/OpenGL_Context):

> Contexts are localized within a particular process of execution (an application, more or less) on an operating system. A process can create multiple OpenGL contexts. Each context can represent a separate viewable surface, like a window in an application.
> In order for any OpenGL commands to work, a context must be current; all OpenGL commands affect the state of whichever context is current. The current context is a thread-local variable, so a single process can have several threads, each of which has its own current context. However, a single context cannot be current in multiple threads at the same time.

# 2) ¿Por qué se dice que crear un contexto OpenGL y una aplicación de venta para dibujar en ella es una tarea especifica de cada sistema operativo?
**Hipótesis**
Dado que cada implementacion de OpenGL depende del hardware disponible en el sistema así como del sistema operativo instalado, se hace obligatorio escribir codigo diferente para cada sistema.
> **Notas del estudiante:** La respuesta anterior posee grandes nubes de ignorancia, aun no puedo responder esta pregunta con seguridad,  se recomienda leer con urgencia el capitulo 1 de la especificación 3.3.
>  - es claro que crear una aplicación de ventana es `OS Specific` pero porque el contexto OpenGL tambien es OS Specific?
>  - Otra táctica viable para resolver el problema podría ser estudiar más a profundidad el funcionamiento de GLFW y sus algoritmos.

# 3) ¿Qué hace GLFW por nosotros?

Es una libreria escrita en C, que nos facilita el proceso de crear Contextos OpenGL, ventanas y manejar entradas del usuario.

# 4) ¿Qué otras librerías como GLFW existen?

> **GLUT - The OpenGL Utility Toolkit:** GLUT (pronounced like the glut in gluttony) is the OpenGL Utility Toolkit, a window system independent toolkit for writing OpenGL programs. It implements a simple windowing application programming interface (API) for OpenGL. GLUT makes it considerably easier to learn about and explore OpenGL programming. GLUT provides a portable API so you can write a single OpenGL program that works across all PC and workstation OS platforms.
> GLUT is not open source. Mark Kilgard maintains the copyright. There are a number of newer and open source alternatives.

> **SDL - Simple DirectMedia Layer:** Is a cross-platform development library designed to provide low level access to audio, keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D. It is used by video playback software, emulators, and popular games including Valve's award winning catalog and many Humble Bundle games.

> **SFML - Simple and Fast Multimedia Library:** SFML provides a simple interface to the various components of your PC, to ease the development of games and multimedia applications. It is composed of five modules: system, window, graphics, audio and network.

> **GLFW** GLFW is an Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan development on the desktop. It provides a simple API for creating windows, contexts and surfaces, receiving input and events.

# 5) ¿Como hacer un build desde el source package de GLFW?

**Paso 1:** ir al [sitio oficial de GLFW](http://www.glfw.org/)
**Paso 2:**  click en download GLFW
![enter image description here](https://i.imgur.com/N0m7dj2.jpg)
**Paso 3:** extract `glfw-3.2.1.zip--`
**Paso 4:** Hacer `Build` de `GLFW` para `VisualStudio v15 2017` usando `CMake`
![enter image description here](https://i.imgur.com/w0DP1hP.jpg)
**Paso 5:** abrir el proyecto para `visual studio v15 2017` llamado `GLFW.sln` que se generó en el paso anterior.
**Paso 6:** `Compilar > Compilar solución`
compiled library can be found in src/Debug named glfw3.lib

# 6) ¿Qué hace GLAD por nosotros?

Nos carga en tiempo de ejecución las funciones de la librería de enlace dinámico  OpenGL de nuestros drivers particulares (esto es una tarea OS-Specific).

# 7) - ¿Como agregar GLFW a mi proyecto en Visual Studio 2017?

**Paso 1:** Copiamos glfw3.lib en una carpeta de nuestro gusto, en mi caso `thirdParty/OpenGL/lib` y los include en `thirdParty/OpenGL/include`

**Paso 2:** Creamos una variable de entorno `OPENGL_DIR` para el directorio `thirdParty/OpenGL/`

![enter image description here](https://i.imgur.com/hv2S4LS.jpg)

**Paso 3:** Habilitamos la ventana `administrador de propiedades`

![enter image description here](https://i.imgur.com/SFnbvfI.jpg)

**Paso 4:** Agregamos una nueva hoja de propiedades al proyecto, en mi caso lo llamaré `opengl_dependencies`

![enter image description here](https://i.imgur.com/2EaxisO.jpg)

**Paso 5:** Le indicamos a nuestro proyecto donde va a poder encontrar la libreria `glfw3.lib` y las respectivas declaraciones de sus funciones (include files).

![enter image description here](https://i.imgur.com/OsDNqgv.jpg)

**Paso 6:** Tell to compiler to link `glfw3.lib` and `opengl32.lib` to our binary result.
note that OpenGL library `opengl32.lib`comes whit the Microsoft SDK which is installed by default when installing `c++ development tools for visual studio`

![enter image description here](https://i.imgur.com/ZcfaFWt.jpg)

# 8) ¿Qué otras librerías como GLAD existen?

> An **OpenGL Loading Library** is a library that loads pointers to OpenGL functions at runtime, core as well as extensions. This is required to access functions from OpenGL versions above 1.1 on most platforms. Extension loading libraries also abstracts away the difference between the loading mechanisms on different platforms.
Most extension loading libraries override the need to include gl.h at all. Instead, they provide their own header that must be used. Most extension loading libraries use code generation to construct the code that loads the function pointers and the included headers. Information is available if you wish to perform this manually, but you are encouraged to use one of these libraries yourself.
extracted from: [enter link description here](https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library)


> **GLEW - The OpenGL Extension Wrangler:** library provides access to all GL entrypoints. It supports Windows, MacOS X, Linux, and FreeBSD.
As with most other loaders, you should not include gl.h, glext.h, or any other gl related header file before glew.h, otherwise you'll get an error message that you have included gl.h before glew.h. In fact, you shouldn't be including gl.h at all; glew.h replaces it.

> **GL3W - The GL3W library-** focuses on the core profile of OpenGL 3 and 4. It only loads the core entrypoints for these OpenGL versions. It supports Windows, Mac OS X, Linux, and FreeBSD.
> GL3W relies on a Python script for its code generation. Unlike other extension loaders, GL3W actually does the code generation on your machine. This is based on downloading and parsing the glcorearb.h file from the OpenGL Registry website.

> **glLoadGen (OpenGL Loader Generator):** The OpenGL Loader Generator This tool is similar to GL3W in several respects. It generates GL loaders, rather than being a library you download and compile. The difference is that it is less specialized; GL3W will only generate core OpenGL functions/enumerators, so no extensions. And it only does so for the most recent version of OpenGL (currently 4.6).
The GL Loader Generator is much more generalized. You can generate a header/source pair for any version of OpenGL, from 1.1 to 4.6. You can include whichever OpenGL extensions you so desire extensions or none at all. And it has the ability to generate source code in a variety of styles, from the standard C-style that most users are familiar with, to C++ functions/enums wrapped in namespaces, and so forth. The system is also user-extensible.
Like GL3W, the loader generator is built in a scripting language. Unlike GL3W, this tool is written in Lua, which is downloadable for a variety of platforms (and has a much smaller install package than Python, if you care about that sort of thing).

> **glad (Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator):**
Glad is pretty similiar to glLoadGen, it generates a loader for your exact needs based on the official specifications from the Khronos SVN. 
This means they are always up to date! It was written in a way that you can easily extend it to other languages (e.g. at the date of writing this there are 4 different code generating backends).
You can use the glad website to generate a loader for your needs, download it and use it in your project. Another method of using glad is cloning/downloading the repository and generating your own loader. The tool itself is pretty easy to use and works with any Python version above 2.6, you can also include the source directly into your CMake project.

	// glad, include glad *before* glfw
	#include <glad/glad.h>
	// GLFW
	#include <GLFW/glfw3.h>

	// ... some code ...

	int main()
	{
	    // Init GLFW
	    glfwInit();
	    // ... <snip> ... setup a window and a context
 
	    // Load all OpenGL functions using the glfw loader function
	    // If you use SDL you can use: https://wiki.libsdl.org/SDL_GL_GetProcAddress
	    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
	        std::cout << "Failed to initialize OpenGL context" << std::endl;
	        return -1;
	    }
	    // Alternative use the builtin loader, e.g. if no other loader function is available
	    /*
	    if (!gladLoadGL()) {
	        std::cout << "Failed to initialize OpenGL context" << std::endl;
	        return -1;
	    }
	    */
  
	    // glad populates global constants after loading to indicate,
	    // if a certain extension/version is available.
	    printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

	    if(GLAD_GL_EXT_framebuffer_multisample) {
	        /* GL_EXT_framebuffer_multisample is supported */ 
	    }
	    if(GLAD_GL_VERSION_3_0) {
	        /* We support at least OpenGL version 3 */
	    }

	    // ... <snip> ... more code
	}

> **glbinding (C++): ** glbinding is a new, generated, cross-platform C++ binding for OpenGL which is solely based on the new xml-based OpenGL API specification (gl.xml). It leverages modern C++11 features like enum classes, lambdas, and variadic templates, instead of relying on macros (all OpenGL symbols are real functions and variables). It provides type-safe parameters, per feature API header, lazy function resolution, multi-context and multi-thread support, global function callbacks, meta information about the generated OpenGL binding and the OpenGL runtime, as well as multiple examples for quick-starting your projects.


	#include <glbinding/gl/gl.h>
	#include <glbinding/Binding.h>

	using namespace gl;

	int main()
	{
	  // create context, e.g. using GLFW, Qt, SDL, GLUT, ...

	  glbinding::Binding::initialize();

	  glBegin(GL_TRIANGLES);
	  // ...
	  glEnd();
	}	

> **libepoxy:**  libepoxy requires no initialization code. The only thing you have to do is:

	#include <epoxy/gl.h>
	#include <epoxy/glx.h>

# 9) - ¿Como agregar GLAD a mi proyecto en Visual Studio 2017?

**Paso 1:** Ir al servicio [web](http://glad.dav1d.de/)

**Paso 2:** Establecemos `C/C++` como lenguaje, version de `API` 3.3 o superior y definimos `core profile`, no incluimos extensiones por ahora y marcamos el cuadro `generate a loader`

**Paso 3:** Agregamos la carpeta include a nuestra carpeta de includes en `thirdParty/OpenGL/include`

**Paso 4:** Agregamos glad.c a nuestro directorio de trabajo en visual studio