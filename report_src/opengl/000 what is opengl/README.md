# 1) ¿Qué es y qué no es OpenGL?
OpenGL es principalmente la especificación de una API para **generar** o **manipular** imágenes usando la CPU y la GPU. Esta especificacion es mantenida y gestionada por el [Grupo Khronos](https://www.khronos.org/).

Las especificaciones de OpenGL no proveen detalles de implementación, sino solamente descriptivos, los fabricantes de cada tarjeta grafica y algunos "hobbistas" son los encargados de implementar las librerías de OpenGL a partir de las especificaciones.

El capitulo 1 de la especificación nos dice lo siguiente:

> **1.2 What is the OpenGL Graphics System?**
> OpenGL (for “Open Graphics Library”) is a **software interface to graphics hardware**. The interface consists of a set of several hundred procedures and functions that allow a programmer to specify the objects and operations involved in producing high-quality graphical images, specifically color images of three-dimensional objects.
 
> **1.3 Programmer’s View of OpenGL**
> To the programmer, OpenGL is a set of commands that allow the specification of geometric objects in two or three dimensions, together with commands that control how these objects are rendered into the framebuffer.

> **1.4 Implementor’s View of OpenGL**
> To the implementor, OpenGL is a set of commands that affect the operation of graphics hardware. [...] The OpenGL implementor’s task is to provide the CPU software interface while dividing the work for each OpenGL command between the CPU and the graphics hardware. This division must be tailored to the available graphics hardware to obtain optimum performance in carrying out OpenGL calls.

# 2) ¿Cómo es la especificación de OpenGL 3.3?

Como se mencionó anteriormente las especificaciones de las distintas versiones de OpenGL proveen principalmente descripciones de las funciones pero no detalles de implementación y OpenGL 3.3 no es la excepción.

La [especificacion de OpenGL 3.3](https://www.khronos.org/registry/OpenGL/specs/gl/glspec33.core.pdf) fue publicada en marzo 11 del 2010 y consta de 6 capítulos y 10 apéndices.

El **Capitulo 1** es una **Introducción** que explica brevemente que es OpenGL y su lenguaje de sombreado y nos introduce en las herramientas que podría usar un desarrollador para hacer aplicaciones OpenGL.

El **Capitulo 2** explica la **operación** de OpenGL, describe el sistema de estados, los tipos de datos, la sintaxis de los comandos así como los vertex buffer object, vertex arrays object, buffer objects, fragment shader objects, vertex shader objects y geometry shader objects (no describe el lenguaje de sombreado sino la estructura y funcionamiento de estos objetos que actúan como un tipo de interfaz entre la CPU y el lenguaje de sombreado en la GPU).

El **Capitulo 3** describe la etapa de **rasterización** de OpenGL, en esta etapa se describen la interfaz para convertir vértices en fragments asi como de la API de texturizado.

El **Capitulo 4** describe los tests que debe realizar OpenGL a cada fragment **(Per-Fragment Operations and the Framebuffer)** generado durante la etapa de rasterizacion  de OpenGL, entre otras cosas se describe el Scissor Test, Stencil Test y Depth Buffer Test,

El **Capitulo 5** describe **funciones especiales** relacionadas a sincronizacion así como instrucciones flush y finish.

El **Capitulo 6** describe **estados y solicitudes de estado (State and State Requests)**  de todos los objetos de OpenGL, ademas en la sección 6.2 provee una tabla de todos los estados de OpenGL.

# 3) ¿Qué son las extensiones de OpenGL?

Las extensiones son un medio para que agregar funcionalidades a OpenGL que el `core` de este no tiene por defecto, algunas extensiones exponen características que soportan solo algunos fabricantes de hardware.

Todas las extensiones conocidas pueden ser encontradas en el [OpenGL Extension Registry](https://www.khronos.org/registry/OpenGL/index_gl.php) que es mantenido por el `Architectural Review Board` ([ARB](https://www.khronos.org/opengl/wiki/OpenGL_Architectural_Review_Board)) y provee la especificación de cada extensión.

> **Extensions fall into 3 broad categories:** `vendor-specific`, `generic`, and `ARB-approved`. The name of each extension starts with a prefix, specifying what type it is.
Extensions that start with **GL_EXT are generic extensions**. They can be implemented by any implementation. **This does not ensure that they are implemented by all IHVs**, but it means that the extension is available to be implemented by multiple vendors. It is also a signal to the user of OpenGL that this extension may find its way into other implementations.
**There is a higher level of EXT extensions, called GL_ARB**. These extensions **are multi-vendor** as well, but they **are specifically approved by the OpenGL ARB**. These are extensions that are most likely to enter later versions of the core OpenGL, and if they do, there will likely be few changes made to them.
**All other prefixes are vendor-specific**. **GL_NV means that only NVIDIA hardware will implement this extension**. **GL_APPLE means** that this **extension will only be found on implementations that are on Macintoshes**. And so forth.

Para mas información en los prefijos de las extensiones y como usarlas revisar la [documentación oficial](https://www.khronos.org/opengl/wiki/OpenGL_Extension).

# 4) Enliste tres extensiones de OpenGL y explique brevemente su funcionamiento

### [ARB_get_proc_address](https://www.khronos.org/registry/OpenGL/extensions/ARB/GLX_ARB_get_proc_address.txt)

> **Status**
    Complete. Approved by ARB on 12/8/1999
**Version**
    Last Modified Date: January 3, 2000
**Overview**
    This extension adds a function to return the address of GLX
    and GL extension functions, given the function name. This is
    necessary with (for example) heterogenous implementations where
    hardware drivers may implement extension functions not known to the
    link library; a similar situation on Windows implementations
    resulted in the wglGetProcAddress function.
   **Notes**
   La especificación de esta extensión fue agregada a la especificación de OpenGL 1.2.1


### [ARB_cull_distance](https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_cull_distance.txt)

> **Status**
Complete. Approved by the ARB on June 26, 2014.
Ratified by the Khronos Board of Promoters on August 7, 2014.
**Version**
Date: June 17, 2014
Revision: 9
**Overview**
This extension adds a new GLSL gl_CullDistance shader output, similar to gl_ClipDistance, but used for whole primitive culling. This new stage in the pipeline is added as part of the primitive clipping stage.
**Notes**
La especificación de esta extensión fue agregada a OpenGL 4.4 (Core Profile) Specification (Programmable Vertex Processing)


### [ARB_copy_image](https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_copy_image.txt)

> **Status**
    Complete. Approved by the ARB on 2012/06/12.
    **Version**
    Last Modified Date:         May 10, 2012
    Revision:                   3
    **Overview**
    This extension enables efficient image data transfer between image
    objects (i.e. textures and renderbuffers) without the need to bind
    the objects or otherwise configure the rendering pipeline.
    This is accomplised by adding a new entry-point CopyImageSubData,
    which takes a named source and destination.
    CopyImageSubData does not perform general-purpose conversions
    such as scaling, resizing, blending, color-space, or format 
    conversions. It should be considered to operate in a manner 
    similar to a CPU memcpy, but using the GPU for the copy.
    CopyImageSubData supports copies between images with different
    internal formats, if the formats are compatible for TextureViews.
    CopyImageSubData also supports copying between compressed and
    uncompressed images if the compressed block / uncompressed texel
    sizes are the same.
    **Notes**
    La especificación de esta extensión fue agregada a Chapter 4 of the OpenGL 4.2 (Core Profile) Specification (Per-Fragment Operations and the Frame Buffer).

# 5) Una [pequeña referencia en diagramas de máquinas de estado](http://tecbolivia.com/index.php/articulos-y-tutoriales-microcontroladores/13-introduccion-a-las-maquinas-de-estado-finito)

# 6)  [Diagrama de máquina de estados para OpenGL 1.1](https://www.lri.fr/~mbl/ENS/IG2/docs/opengl-stm.pdf)

### sobre la máquina de estados OpenGL core-profile
> **OpenGL is by itself a large state machine**: a collection of variables that define how OpenGL should currently operate. The state of OpenGL is commonly referred to as the OpenGL context. When using OpenGL, we often change its state by setting some options, manipulating some buffers and then render using the current context.
Whenever we tell OpenGL that we now want to draw lines instead of triangles for example, we change the state of OpenGL by changing some context variable that sets how OpenGL should draw. As soon as we changed the state by telling OpenGL it should draw lines, the next drawing commands will now draw lines instead of triangles.
When working in OpenGL we will come across several state-changing functions that change the context and several state-using functions that perform some operations based on the current state of OpenGL. As long as you keep in mind that OpenGL is basically one large state machine, most of its functionality will make more sense.
Extracted from [LearnOpenGL site](https://learnopengl.com/#!Getting-started/OpenGL).

# 7) sobre los objetos, Extracted from [LearnOpenGL site](https://learnopengl.com/#!Getting-started/OpenGL).

The OpenGL libraries are written in C and allows for many derivations in other languages, but in its core it remains a C-library. Since many of C's language-constructs do not translate that well to other higher-level languages, OpenGL was developed with several abstractions in mind. One of those abstractions are objects in OpenGL.
An object in OpenGL is a collection of options that represents a subset of OpenGL's state. For example, we could have an object that represents the settings of the drawing window; we could then set its size, how many colors it supports and so on. One could visualize an object as a C-like struct:

	struct object_name {
	    float  option1;
	    int    option2;
	    char[] name;
	};
Whenever we want to use objects it generally looks something like this (with OpenGL's context visualized as a large struct):

	// The State of OpenGL
	struct OpenGL_Context {
	  	...
	  	object* object_Window_Target;
	  	...  	
	};

example:

	// create object
	unsigned int objectId = 0;
	glGenObject(1, &objectId);
	// bind object to context
	glBindObject(GL_WINDOW_TARGET, objectId);
	// set options of object currently bound to GL_WINDOW_TARGET
	glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_WIDTH, 800);
	glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_HEIGHT, 600);
	// set context target back to default
	glBindObject(GL_WINDOW_TARGET, 0);

This little piece of code is a workflow you'll frequently see when working in OpenGL. We first create an object and store a reference to it as an id (the real object data is stored behind the scenes). Then we bind the object to the target location of the context (the location of the example window object target is defined as GL_WINDOW_TARGET). Next we set the window options and finally we un-bind the object by setting the current object id of the window target to 0. The options we set are stored in the object referenced by objectId and restored as soon as we bind the object back to GL_WINDOW_TARGET.

The great thing about using these objects is that we can define more than one object in our application, set their options and whenever we start an operation that uses OpenGL's state, we bind the object with our preferred settings. There are objects for example that act as container objects for 3D model data (a house or a character) and whenever we want to draw one of them, we bind the object containing the model data that we want to draw (we first created and set options for these objects). Having several objects allows us to specify many models and whenever we want to draw a specific model, we simply bind the corresponding object before drawing without setting all their options again.

# 8) cuáles son las diferencias entre OpenGL inmediate mode y core profile, Extracted from [LearnOpenGL site](https://learnopengl.com/#!Getting-started/OpenGL).

In the old days, using OpenGL meant developing in immediate mode (often referred to as the fixed function pipeline) which was an easy-to-use method for drawing graphics. Most of the functionality of OpenGL was hidden in the library and developers did not have much freedom at how OpenGL does its calculations. Developers eventually got hungry for more flexibility and over time the specifications became more flexible; developers gained more control over their graphics. The immediate mode is really easy to use and understand, but it is also extremely inefficient. For that reason the specification started to deprecate immediate mode functionality from version 3.2 and started motivating developers to develop in OpenGL's core-profile mode which is a division of OpenGL's specification that removed all old deprecated functionality.

When using OpenGL's core-profile, OpenGL forces us to use modern practices. Whenever we try to use one of OpenGL's deprecated functions, OpenGL raises an error and stops drawing. The advantage of learning the modern approach is that it is very flexible and efficient, but unfortunately is also more difficult to learn. The immediate mode abstracted quite a lot from the actual operations OpenGL performed and while it was easy to learn, it was hard to grasp how OpenGL actually operates. The modern approach requires the developer to truly understand OpenGL and graphics programming and while it is a bit difficult, it allows for much more flexibility, more efficiency and most importantly a much better understanding of graphics programming.

This is also the reason why our tutorials are geared at Core-Profile OpenGL version 3.3. Although it is more difficult, it is greatly worth the effort.

As of today, much higher versions of OpenGL are published (at the time of writing 4.5) at which you might ask: why do I want to learn OpenGL 3.3 when OpenGL 4.5 is out? The answer to that question is relatively simple. All future versions of OpenGL starting from 3.3 basically add extra useful features to OpenGL without changing OpenGL's core mechanics; the newer versions just introduce slightly more efficient or more useful ways to accomplish the same tasks. The result is that all concepts and techniques remain the same over the modern OpenGL versions so it is perfectly valid to learn OpenGL 3.3. Whenever you're ready and/or more experienced you can easily use specific functionality from more recent OpenGL versions.