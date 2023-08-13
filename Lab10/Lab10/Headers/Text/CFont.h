#ifndef __TEXT_FT
#define __TEXT_FT

#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class CFont 
{
private:
	glm::mat4 transform;
	GLuint tTexture;
	GLuint tVAO, tVBO;
	std::map<GLchar, Character> t_Characters;
    Shader tShader;
    bool defaultProj = true;
    float texWidth;
public:
	CFont(bool defaultProj = true)
    {
        this->defaultProj = defaultProj;
    }

    void updateProjection2D()
    {
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(glutGet(GLUT_WINDOW_WIDTH)), 0.0f, static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT)));
        this->tShader.use();
        this->tShader.setMat4(projection, "matProj");
    }

	void init(const char* font_filename, int font_size, Shader tShader = Shader("Shaders/Text/text-ft-vertex.glsl", "Shaders/Text/text-ft-fragment.glsl"))
    {
	
        this->transform = glm::mat4(1.f);

        this->tShader = tShader;

		if(defaultProj)
            this->updateProjection2D();

        FT_Library ft;
        // All functions return a value different than 0 whenever an error occurred
        if (FT_Init_FreeType(&ft))
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

        // load font as face
        FT_Face face;
        if (FT_New_Face(ft, font_filename, 0, &face))
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, font_size);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            t_Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        // destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        glGenVertexArrays(1, &this->tVAO);
        glBindVertexArray(this->tVAO);
        glGenBuffers(1, &this->tVBO);
        glBindBuffer(GL_ARRAY_BUFFER, this->tVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

	}
    void renderText(std::string text, float x, float y, float scale = 1.0f, glm::vec3 color = glm::vec3(1.0, 1.0, 1.0))
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // activate corresponding render state
        this->tShader.use();
        this->tShader.setVec3(color, "textColor");

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(this->tVAO);
        this->texWidth = 0.f;
        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = t_Characters[*c];

            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, this->tVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
            this->texWidth += (ch.Advance >> 6) * scale;
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glDisable(GL_BLEND);
    }
    
    void renderText3D(std::string text, glm::mat4 matView, glm::mat4 matProj, float scale = 1.f, glm::vec3 color = glm::vec3(1.0, 1.0, 1.0))
    {
        this->tShader.use();
        this->tShader.setMat4(this->transform, "matModel");
        this->tShader.setMat4(matView, "matView");
        this->tShader.setMat4(matProj, "matProj");
        this->renderText(text, -this->texWidth / 2, 0, scale, color);

    }

    void translate(glm::vec3 vec) 
    {
        this->transform = glm::translate(this->transform, vec);
    }
    void setTransform(glm::mat4 transform) 
    {
        this->transform = transform;
    }
};
#endif // __TEXT_FT