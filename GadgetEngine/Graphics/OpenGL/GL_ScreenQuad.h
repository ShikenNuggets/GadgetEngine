#ifndef GL_SCREEN_QUAD_H
#define GL_SCREEN_QUAD_H

#include <glad/glad.h>

namespace Gadget{
	class GL_ScreenQuad{
	public:
		GL_ScreenQuad();
		~GL_ScreenQuad();

		void Bind();
		void Unbind();

		inline constexpr GLuint GetVAO() const{ return vao; }
		inline constexpr GLuint GetVBO() const{ return vbo; }
		inline constexpr GLuint GetEBO() const{ return ebo; }

	private:
		GLuint vao;
		GLuint vbo;
		GLuint ebo;
	};
}

#endif //!GL_SCREEN_QUAD_H