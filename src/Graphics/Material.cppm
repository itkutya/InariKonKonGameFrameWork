export module Material;

import Shader;

export namespace ikk
{
    struct Material
    {
        const Shader<ShaderType::Vertex>* vertexShader = nullptr;
        const Shader<ShaderType::Fragment>* fragmentShader = nullptr;
    };
}