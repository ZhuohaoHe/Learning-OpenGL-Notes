#pragma once
#include <GLFW/glfw3.h>
#include <string>

class Texture {
    private:
        GLuint m_RendererID;
        const std::string m_TextureType;
        const std::string m_FilePath;
        unsigned char* m_LocalBuffer;
        int m_Width, m_Height, m_BPP;

    public:
        Texture(const std::string& path);
        ~Texture();

        void Bind(const unsigned int slot = 0) const;
        void Unbind() const;

        inline int GetWidth() const { return m_Width; }
        inline int GetHeight() const { return m_Height; }

        inline std::string GetTextureType() const { return m_TextureType; }
};