#include "VertexArray.hpp"
#include <iostream>

VertexArray::VertexArray() {
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray() {
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    vb.Bind();
    Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i ++) {
        const auto& element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, 
            layout.GetStride(), (const void*)offset));
        std::cout << "i: " << i << " count: " << element.count << " type: " << element.type << " normalized: " << element.normalized << " stride: " << layout.GetStride() << " offset: " << offset << std::endl;
        offset += element.count * VertexBufferLayoutElement::GetSizeofType(element.type);
    }
    UnBind();
    vb.UnBind();
}

void VertexArray::Bind() const {
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const {
    GLCall(glBindVertexArray(0));
}