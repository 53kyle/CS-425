
#include <sys/mman.h>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <span>
#include <vector>
#include <thread>

typedef float floatVec4 __attribute__ (( vector_size(4*(sizeof(float))) )) __attribute__(( aligned(64) ));
typedef float floatVec16 __attribute__ (( vector_size(16*(sizeof(float))) )) __attribute__(( aligned(64) ));

using namespace std;

//---------------------------------------------------------------------------
//
//  Data types and typedefs (C++ "using" clauses)
//

using Index = int;
using Distance = float;

//---------------------------------------------------------------------------

struct Vertex {
    float x;
    float y;
    float z;
    
    floatVec4 posVec() const {
        return floatVec4{x, y, z, 1.0};
    }
    
    floatVec16 dotVec() const {
        return floatVec16{x, y, z, 1.0, x, y, z, 1.0, x, y, z, 1.0, x, y, z, 1.0};
    }

    friend std::ostream& operator << (std::ostream& os, const Vertex& p) {
        return os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    }
};

using Vertices = std::span<Vertex>;

//---------------------------------------------------------------------------

struct Face {
    unsigned int numIndices;
    Index* indices;

    friend std::ostream& operator << (std::ostream& os, const Face& f) {
        os << f.numIndices << ": ";
        for (auto i = 0; i < f.numIndices; ++i) {
            os << f.indices[i] << " ";
        }
        return os;
    }
};

using Faces = std::vector<Face>;

//---------------------------------------------------------------------------

struct Transform {
    
    struct float4 {
        floatVec4 posVec;
        floatVec16 dotVec;

        float4() = default;
        float4(float x, float y, float z, float w) {
            posVec = floatVec4{x, y, z, w};
        }
        float4(floatVec4 posVec) : posVec(posVec) { }
        float4(floatVec16 dotVec) : dotVec(dotVec) { }
        
        floatVec4 dot(const floatVec16& v) const {
            floatVec16 resultVec = dotVec*v;
            
            return floatVec4{(resultVec[0] + resultVec[1] + resultVec[2] + resultVec[3]), (resultVec[4] + resultVec[5] + resultVec[6] + resultVec[7]), (resultVec[8] + resultVec[9] + resultVec[10] + resultVec[11]), (resultVec[12] + resultVec[13] + resultVec[14] + resultVec[15])};
        }

        Vertex perspectiveDivide() const {
            floatVec4 resultVec = posVec/posVec[3];
            
            return Vertex{ resultVec[0], resultVec[1], resultVec[2] };
        }
    };

    floatVec16 dotVec;

    Transform(float4 r0, float4 r1, float4 r2, float4 r3) {
        dotVec = floatVec16{r0.posVec[0], r0.posVec[1], r0.posVec[2], r0.posVec[3], r1.posVec[0], r1.posVec[1], r1.posVec[2], r1.posVec[3], r2.posVec[0], r2.posVec[1], r2.posVec[2], r2.posVec[3], r3.posVec[0], r3.posVec[1], r3.posVec[2], r3.posVec[3]};
    }

    Vertex operator * (const Vertex& v) {
        float4 r(dotVec);
        
        floatVec4 dot = r.dot(v.dotVec());
        
        float4 s(dot[0], dot[1], dot[2], dot[3]);

        return s.perspectiveDivide();
    }
};

//---------------------------------------------------------------------------
//
//  readData - read data from input file.  In this case, we memory-map
//    the file in for better performance, and map various arrays of
//    data (i.e., the vertices, and how they're connected together [their
//    faces]) into a few C++ data structures.
//
//  This routine uses some hard-coded "magic numbers" related to sizes
//    in this file to simplify the program.  While this is usually a
//    frowned upon approach, it save a lot of parsing code.  In this
//    input file, there are:
//
//      14027872 vertices, each containing three floating-point values
//      28055742 faces, each with an unsigned interger count, followed
//        by <count> indices    
//

void readData(const char* filename, Vertices& vertices, Faces& faces) {

    // Open the file, determine its size, and "memory map".  A memory
    //   map makes the contents of the file avilable as an array of
    //   bytes, which we can cast to the right types to use.
    //
    FILE* file = fopen(filename, "rb");
    if (!file) {
        std::cerr << "Unable to open '" << filename << "' ... exiting\n";
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);

    void* memory = mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fileno(file), 0);
    
    if ((long) memory < 0 ) {
        std::cerr << "Unable to read '" << filename << "' ... exiting\n";
        exit(EXIT_FAILURE);
    }

    // Use a C++ std::span container to present the raw memory of the memory
    //   map as a useful type.  In this case, the variable "vertices"
    //   will look a lot like a std::vector, but not requiring all of the
    //   allocations
    constexpr size_t numVertices = 14027872;
    vertices = Vertices{reinterpret_cast<Vertex*>(memory), numVertices};

    size_t bytesForVertices = numVertices * sizeof(Vertex);

    // Faces are recorded in the file as a <count>, followed by <count>
    //   integers that index into the list of vertices.  Since there
    //   could be faces with different numbers of vertices, we need to
    //   process each face separately
    constexpr size_t numFaces = 28055742;
    faces.reserve(numFaces);
    auto data = reinterpret_cast<unsigned int*>((char*)memory + bytesForVertices);

    for (auto i = 0; i < numFaces; ++i) {
        unsigned int numIndices = *data++;
        faces.emplace_back(Face{numIndices, reinterpret_cast<Index*>(data)});
        data += numIndices;
    }
}

//----------------------------------------------------------------------------
//
//  transform - transforms a vertex by a 4x4 tranformation matrix using
//    the Transform class's operator *.  
//

Transform  xform {
    { 0.1, 0.0, 0.0, 0.0 },
    { 0.0, 0.1, 0.0, 0.0 },
    { 0.0, 0.0, -0.02, -1.01 },
    { 0.0, 0.0, 0.0, 1.0 },            
};

Vertex transform(const Vertex& v) {
    return xform * v;
}

//----------------------------------------------------------------------------
//
//  distance - computes the distance between two vertices
//

Distance distance(const Vertex& p, const Vertex& q) {
    floatVec4 subResult = p.posVec() - q.posVec();
    floatVec4 sqrResult = subResult*subResult;

    return std::sqrt(sqrResult[0] + sqrResult[1] + sqrResult[2]);
}

//----------------------------------------------------------------------------
//
//  computePerimeter - computes the distance around all of the vertices
//    in a face.  The distance between each pair of vertices is computed
//    using the distance function, and then summed.
//

Distance computePerimeter(const Face& face, const Vertices& vertices) {
    Distance perimeter = 0.0;

    Index* indices = face.indices;

    auto i = 0;
    while (i < face.numIndices - 1) {
        auto p = transform(vertices[indices[i]]);
        auto q = transform(vertices[indices[++i]]);
        perimeter += distance(p, q);
    }

    auto p = transform(vertices[indices[i]]);
    auto q = transform(vertices[indices[0]]);
    perimeter += distance(p, q);

    return perimeter;
}

struct Min {
    Distance perimeter = std::numeric_limits<Distance>::infinity();
    Index index = 0;
};

void computePerimiterForSection(const Faces &faces, const Vertices &vertices, int section, Min *minFace) {
    
    auto i = section*(faces.size()/4);
    auto limit = faces.size();
    
    if (section < 3) {
        limit = (section + 1)*(faces.size()/4) - 1;
    }
    
    //    cout << "Thread " << section << ": " << endl << "Min: " << i << " Max: " << limit << endl;
    while (i < limit) {
        auto perimeter = computePerimeter(faces[i], vertices);

        if (perimeter < minFace -> perimeter) {
            minFace -> perimeter = perimeter;
            minFace -> index = i;
        }
        
        i++;
    }
}

//----------------------------------------------------------------------------
//
//  main - loads the data for the model and then computes the perimeter
//    of each face in the model, reporting the index of the face with the
//    smallest perimeter under the given transformation
//

int main() {
    Vertices vertices;
    Faces    faces;
    
    readData("lucy.bin", vertices, faces);

    Min minFace;
    Min minFaces[4];

    /*for (auto i = 0; i < faces.size(); ++i) {
        auto perimeter = computePerimeter(faces[i], vertices);

        if (perimeter < minFace.perimeter) {
            minFace.perimeter = perimeter;
            minFace.index = i;
        }
    }*/
    
    //    cout << "Total number of faces: " << faces.size() << endl;
    
    thread a[4];
    
    for (auto i = 0; i < 4; i++) {
        a[i] = thread(&computePerimiterForSection, faces, vertices, i, &minFaces[i]);
    }
    
    for (auto i = 0; i < 4; i++) {
        a[i].join();
    }
    
    for (auto i = 0; i < 4; i++) {
        if (minFaces[i].perimeter < minFace.perimeter) {
            minFace = minFaces[i];
        }
    }
    
    std::cout << "The smallest triangle is " << minFace.index << "\n";
}
