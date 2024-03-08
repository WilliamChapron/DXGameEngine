#pragma once

class CubeMesh {
public:
    Vertex* cubeVertices;
    UINT* cubeIndices;
    int numElementsV;
    int numElementsI;

    float aabbSizeX;
    float aabbSizeY;
    float aabbSizeZ;

    CubeMesh() {
        // Initialisation des pointeurs
        cubeVertices = new Vertex[8]{
            { {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },
            { {-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
            { {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },
            { {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
            { { 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },
            { { 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
            { { 0.5f,  0.5f, -0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 1.0f} },
            { { 0.5f,  0.5f,  0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 0.0f} }
        };

        cubeIndices = new UINT[36]{
            0, 1, 2,
            2, 1, 3,
            4, 6, 5,
            6, 7, 5,
            0, 2, 4,
            2, 6, 4,
            1, 5, 3,
            3, 5, 7,
            2, 3, 6,
            3, 7, 6,
            0, 4, 1,
            1, 4, 5
        };

        numElementsV = 8;
        numElementsI = 36;
    }

    ~CubeMesh() {
        delete[] cubeVertices;
        delete[] cubeIndices;
    }
};

class TriangleMesh {
public:
    Vertex* cubeVertices;
    UINT* cubeIndices;
    int numElementsV;
    int numElementsI;

    TriangleMesh() {
        // Initialisation des pointeurs
        cubeVertices = new Vertex[8]{
            { {0.5f, 0.9f, 0.9f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },
            { {0.9f, 0.9f,  -0.9f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
            { {0.9f,  0.9f, 0.9f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },
            { {0.9f,  0.9f,  0.9f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
            { { 0.9f, 0.9f, 0.9f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },
            { { 0.9f, 0.9f,  0.9f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
            { { 0.9f,  0.9f, 0.9f}, {0.9f, 0.5f, 0.5f, 1.0f}, {1.0f, 1.0f} },
            { { 0.5f,  0.5f,  0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 0.0f} }
        };

        cubeIndices = new UINT[36]{
            0, 1, 2,
            2, 1, 3,
            4, 6, 5,
            6, 7, 5,
            0, 2, 4,
            2, 6, 4,
            1, 5, 3,
            3, 5, 7,
            2, 3, 6,
            3, 7, 6,
            0, 4, 1,
            1, 4, 5
        };

        numElementsV = 8;
        numElementsI = 36;
    }

    ~TriangleMesh() {
        delete[] cubeVertices;
        delete[] cubeIndices;
    }
};