#ifndef TYPES_H
#define TYPES_H

#ifdef WIN32
#include <gl/glew.h>
#endif

#include <GL/glfw.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/random.hpp>
#include <omp.h>

#ifndef MAXFLOAT
#define MAXFLOAT 1e+37f
#endif

#ifndef PI
#define PI 3.1415926536f
#endif

// refractive index constants for materials
#ifndef AIR
#define AIR 1.000293
#endif
#ifndef GLASS
#define GLASS 1.52
#endif

using glm::vec2;
using glm::vec3;
using glm::vec4;

using glm::uvec2;
using glm::uvec3;
using glm::uvec4;

using glm::ivec2;
using glm::ivec3;
using glm::ivec4;

using glm::mat3;
using glm::mat4;

using glm::quat;

typedef glm::highp_vec3         hpvec3;

typedef unsigned char 	        u8;
typedef unsigned short 	        u16;
typedef unsigned int 	        u32;
typedef unsigned long	        u64;

typedef signed char		        i8;
typedef signed short 	        i16;
typedef signed int		        i32;
typedef signed long		        i64;

typedef glm::detail::float16	f16;
typedef float		 	        f32;
typedef double			        f64;

#endif
