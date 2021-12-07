#ifndef OBJLOADER2_H
#define OBJLOADER2_H
#include <vector>
#include "Vector.h"

using namespace MATH;

class ObjLoader2 {
public:

	/// C11 precautions 
	ObjLoader2(const ObjLoader2&) = delete;  /// Copy constructor
	ObjLoader2(ObjLoader2&&) = delete;       /// Move constructor
	ObjLoader2& operator=(const ObjLoader2&) = delete; /// Copy operator
	ObjLoader2& operator=(ObjLoader2&&) = delete;      /// Move operator

	static std::vector<Vec3> vertices;
	static std::vector<Vec2> uvCoords;
	static std::vector<Vec3> normals;
	static bool loadOBJ2(const char* path);

};

#endif

