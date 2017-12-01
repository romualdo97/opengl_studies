# ¿Qué es un contexto OpenGL?

Un contexto OpenGL son muchas cosas se concidera la parte principal de una aplicación OpenGL, el contexto es el encargado de almacenar todos los estados asociados a la instancia actual de OpenGL, se puede pensar en un contexto OpenGL como el objeto que almacena a toda la aplicación OpenGL.

from [docs](https://www.khronos.org/opengl/wiki/OpenGL_Context):

> Contexts are localized within a particular process of execution (an application, more or less) on an operating system. A process can create multiple OpenGL contexts. Each context can represent a separate viewable surface, like a window in an application.
> In order for any OpenGL commands to work, a context must be current; all OpenGL commands affect the state of whichever context is current. The current context is a thread-local variable, so a single process can have several threads, each of which has its own current context. However, a single context cannot be current in multiple threads at the same time.

# ¿Por qué se dice que crear un contexto OpenGL y una aplicación de venta para dibujar en ella es una tarea especifica de cada sistema operativo?