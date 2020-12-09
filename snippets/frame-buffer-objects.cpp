...
// create a texture object
GLuint textureId;
glGenTextures(1, &textureId);
glBindTexture(GL_TEXTURE_2D, textureId);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
glBindTexture(GL_TEXTURE_2D, 0);

// create a renderbuffer object to store depth info
GLuint rboId;
glGenRenderbuffers(1, &rboId);
glBindRenderbuffer(GL_RENDERBUFFER, rboId);
glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
glBindRenderbuffer(GL_RENDERBUFFER, 0); // un-bind, no longer required.

// create a framebuffer object
GLuint fboId;
glGenFramebuffers(1, &fboId);
glBindFramebuffer(GL_FRAMEBUFFER, fboId);

// attach the texture to FBO color attachment point
glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER
                       GL_COLOR_ATTACHMENT0,  // 2. attachment point
                       GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
                       textureId,             // 4. tex ID
                       0);                    // 5. mipmap level: 0(base)

// attach the renderbuffer to depth attachment point
glFramebufferRenderbuffer(GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
                          GL_DEPTH_ATTACHMENT, // 2. attachment point
                          GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
                          rboId);              // 4. rbo ID

// check FBO status
// This is required, to check the FBO was correctly setup.
GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
if(status != GL_FRAMEBUFFER_COMPLETE)
    fboUsed = false;

// Render the scene to textures in the created FBO.
glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
glEnable(GL_DEPTH_TEST);
DrawScene();	

// switch back to window-system-provided (default) framebuffer
glBindFramebuffer(GL_FRAMEBUFFER, 0);

// (Exercise)
// Maybe add a quad that overlaps all the screen and render the generated texture to it.
// Or whatever you need to do post-processing effects.
// second pass
glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
glClear(GL_COLOR_BUFFER_BIT);

glBindVertexArray(quadVAO);
glDisable(GL_DEPTH_TEST); // To make sure the quad is over all previously rendered.
glBindTexture(GL_TEXTURE_2D, textureColorbuffer); // The same texture attached to the created Frame Buffer Object
glDrawArrays(GL_TRIANGLES, 0, 6); // Draw the rectangle.
...

// Source from: http://www.songho.ca/opengl/gl_fbo.html
// Source from: https://learnopengl.com/Advanced-OpenGL/Framebuffers

float ambientStrength = 0.1;
vec3 ambient = ambientStrength * lightColor;

float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColor;

// Where FragPos is the World Space fragment position.
vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
vec3 specular = specularStrength * spec * lightColor;  