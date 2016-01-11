/****************************************************************************
 Copyright (C) 2013 Henry van Merode. All rights reserved.
 Copyright (c) 2015 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/


#ifndef __CC_PU_PARTICLE_MESH_SURFACE_EMITTER_H__
#define __CC_PU_PARTICLE_MESH_SURFACE_EMITTER_H__

#include "extensions/Particle3D/PU/CCPUEmitter.h"

NS_CC_BEGIN

/**
* @addtogroup _3d
* @{
*/

/** @class PUTriangle
*@brief @~english Definition of a Triangle.
@~chinese 定义一个三角形类。
*/
class PUTriangle
{
public:
    /** The struct is used to return both the position and the normal
    */
    struct PositionAndNormal
    {
        Vec3 position;
        Vec3 normal;
    };

    /** Public attributes **/
    float squareSurface;
    Vec3 surfaceNormal;	/** Normal of triangle v1-v2-v3 */
    Vec3 v1;				/** Vertex v1 */
    Vec3 v2;				/** Vertex v2 */
    Vec3 v3;				/** Vertex v3 */
    Vec3 vn1;			/** Normal of vertex v1 */
    Vec3 vn2;			/** Normal of vertex v2 */
    Vec3 vn3;			/** Normal of vertex v3 */
    Vec3 en1;			/** Normal of edge v1-v2 */
    Vec3 en2;			/** Normal of edge v2-v3 */
    Vec3 en3;			/** Normal of edge v3-v1 */

    /**
    * @~english Constructor.
    * @~chinese 构造器。
    */
    PUTriangle(void){};

    /**
    * @~english Calculate the (square) surface of the triangle.
    * @~chinese 计算三角形的表面。
    */
    void calculateSquareSurface (void);

    /**
    * @~english Calculate the surface normal of the triangle.
    * @~chinese 计算三角形的表面法线。
    */
    void calculateSurfaceNormal (void);

    /**
    * @~english Calculate the edge normals of the 3 edges.
    * @~chinese 计算三角形的边法线。
    */
    void calculateEdgeNormals (void);

    /**
    * @~english Determine a random position on this triangle.
    * @~chinese 获取三角形内的一个随机点。
    * @return   @~english A random position on this triangle. @~chinese 三角形的随机点。
    */
    const Vec3 getRandomTrianglePosition (void);

    /**
    * @~english Determine a random position including its normal on a one of the edges.
    * @~chinese 获取三角形内的一个随机点包含一条边的法线。
    * @return   @~english Position and normal. @~chinese 位置和法线。
    */
    const PositionAndNormal getRandomEdgePositionAndNormal (void);

    /**
    * @~english Determine a random vertex including its normal of this triangle.
    * @~chinese 获取三角形内的一个随机顶点和法线。
    * @return   @~english Position and normal. @~chinese 位置和法线。
    */
    const PositionAndNormal getRandomVertexAndNormal (void);
};

/** @class PUSortAscending
*@brief @~english Comparer used for sorting vector in ascending order.
@~chinese 升序比较容器用以排序三角形。
*/
struct PUSortAscending
{
    bool operator() (const PUTriangle& a, const PUTriangle& b)
    {
        return a.squareSurface < b.squareSurface;
    }
};

/** @class PUSortDescending
*@brief @~english Comparer used for sorting vector in descending order.
@~chinese 降序比较容器用以排序三角形。
*/
struct PUSortDescending
{
    bool operator() (const PUTriangle& a, const PUTriangle& b)
    {
        return a.squareSurface > b.squareSurface;
    }
};


typedef std::vector<PUTriangle> Triangles;

/** @class MeshInfo
*@brief @~english Definition of a Mesh.
@~chinese 定义一个网格类。
*/
class MeshInfo
{
public:
    /** Defining several methods to emit particles on the mesh surface
    @remarks
        Sometimes the difference is not always visible, for example if the mesh contains triangles with more or
        less the same size. Only in case a mesh contains both small and large triangles the difference between
        the various distribution methods is more obvious.
    */
    enum MeshSurfaceDistribution
    { 
        MSD_HOMOGENEOUS,		/** Distribute particles homogeneous (random) on the mesh surface */
        MSD_HETEROGENEOUS_1,	/** Distribute more particles on the smaller faces  */
        MSD_HETEROGENEOUS_2,	/** Same as above, but now more particles are emitting from the larger faces  */
        MSD_VERTEX,				/** Particles only emit from the vertices  */
        MSD_EDGE				/** Particles emit random on the edges  */
    };

    /** Constructor **/
    MeshInfo (const std::string& meshName, 
        const MeshSurfaceDistribution distribution = MSD_HOMOGENEOUS,
        const Quaternion& orientation = Quaternion(),
        const Vec3& scale = Vec3::ZERO);

    /** Destructor **/
    ~MeshInfo (void);

    /**
    * @~english Generate a random number.
    * @~chinese 生成高斯随机数。
    * @param high   @~english The high argument determines that numbers are returned between [0..high]. @~chinese 最大的范围。
    * @param cutoff   @~english The cutoff. @~chinese 截断值。
    * @return   @~english The value. @~chinese 随机值。
    */
    float getGaussianRandom (float high, float cutoff = 4);

    ///** Retrieve vertex info **/
    //void getMeshInformation(Ogre::MeshPtr mesh,
    //						const Vec3& position = Vec3::ZERO,
    //						const Quaternion& orient = Quaternion(),
    //						const Vec3& scale = Vec3::ONE);

    /**
    * @~english Get one triangle.
    * @~chinese 获取一个三角形。
    * @param triangleIndex   @~english The index of triangle. @~chinese 三角形索引。
    * @return   @~english The triangle. @~chinese 三角形。
    */
    const PUTriangle& getTriangle (size_t triangleIndex);

    /**
    * @~english Get a random triangle (index) from the mesh.
    * @~chinese 获取一个网格中的随机三角形索引。
    * @return   @~english The index of triangle. @~chinese 三角形索引。
    */
    const size_t getRandomTriangleIndex (void);
    
    /**
    * @~english Get triangle number.
    * @~chinese 获取网格中三角形数目。
    * @return   @~english The number of triangle. @~chinese 三角形数目。
    */
    size_t getTriangleCount() const { return _triangles.size(); }

    /**
    * @~english Returns both a random point on a given triangle and its normal vector.
    *           How the random point and the normal are determined depends on the distribution type.
    * @~chinese 返回指定三角形的所有随机点和法线。
    * @param triangleIndex   @~english The index of triangle. @~chinese 三角形索引。
    * @return   @~english Position and normal. @~chinese 位置和法线。
    */
    const PUTriangle::PositionAndNormal getRandomPositionAndNormal (const size_t triangleIndex);

protected:
    Triangles _triangles;
    MeshSurfaceDistribution mDistribution;
};

/** The MeshSurfaceEmitter is a ParticleEmitter that emits particles on the surface of a mesh.
@remarks
    There are several ways of emitting it on the surface, from the vertices, edges and faces of a mesh.
    It is also possible to define whether more particles emit on larger faces.
*/

class CC_DLL PUMeshSurfaceEmitter : public PUEmitter
{
public:
    // Constants
    static const Vec3 DEFAULT_SCALE;
    static const MeshInfo::MeshSurfaceDistribution DEFAULT_DISTRIBUTION;

    static PUMeshSurfaceEmitter* create();

    /**
    * @~english Returns the mesh name.
    * @~chinese 返回网格名称。
    * @return   @~english The mesh nam. @~chinese 网格名称。
    */
    const std::string& getMeshName(void) const;

    /**
    * @~english Sets the mesh name.
    * @~chinese 设置网格名称。
    * @param meshName   @~english The index of triangle. @~chinese 网格名称。
    * @param doBuild   @~english If need build mesh. @~chinese 是否构建网格。
    */
    void setMeshName(const std::string& meshName, bool doBuild = true);

    /**
    * @~english Returns whether use normal.
    * @~chinese 返回是否使用法线。
    * @return   @~english Whether use normal. @~chinese 是否使用法线。
    */
    bool useNormals (void) const;

    /**
    * @~english Set indication whether normals are used for the particle direction.
    * @~chinese 设置法线是否用于粒子方向。
    * @param useNormals   @~english Whether use normal. @~chinese 是否使用法线。
    */
    void setUseNormals (bool useNormals);

    /**
    * @~english Returns the type of particle distribution.
    * @~chinese 返回分布类型。
    * @return   @~english  the type of particle distribution. @~chinese 粒子分布类型。
    * @remarks
    * There are several ways to emit particles on the surface of a mesh. This attribute indicates
    * the type of distribution on the surface.
    */
    const MeshInfo::MeshSurfaceDistribution getDistribution (void) const;

    /**
    * @~english Set the type of particle distribution on the surface of a mesh.
    * @~chinese 设置网格的粒子分布类型。
    * @param distribution   @~english the type of particle distribution. @~chinese 粒子分布类型。
    */
    void setDistribution(MeshInfo::MeshSurfaceDistribution distribution);

    /**
    * @~english Returns the scale of the mesh.
    * @~chinese 获取网格缩放。
    * @return   @~english  the scale. @~chinese 缩放。
    */
    const Vec3& getScale (void) const;

    /**
    * @~english Set the scale of the mesh.
    * @~chinese 设置网格缩放。
    * @return   @~english  the scale. @~chinese 缩放。
    */
    void setScale (const Vec3& scale);

    /**
    * @~english Build all the data needed to generate the particles.
    * @~chinese 构建粒子所需的数据。
    */
    void build(void);

    /** override. */
    virtual void prepare() override;

    /** override. */
    virtual void unPrepare() override;

    /** override. */
    virtual void initParticlePosition(PUParticle3D* particle) override;

    /** override. */
    virtual unsigned short calculateRequestedParticles(float timeElapsed) override;

    /** override. */
    virtual void initParticleDirection(PUParticle3D* particle) override;

    /** override. */
    virtual PUMeshSurfaceEmitter* clone() override;

    /** override. */
    virtual void copyAttributesTo (PUEmitter* emitter) override;

CC_CONSTRUCTOR_ACCESS:
    PUMeshSurfaceEmitter(void);
    virtual ~PUMeshSurfaceEmitter(void);

protected:
    std::string _meshName;
    Quaternion _orientation;
    Vec3 _scale;
    MeshInfo::MeshSurfaceDistribution _distribution;
    MeshInfo* _meshInfo;
    size_t _triangleIndex;
    bool _directionSet;
};

// end of _3d group
/// @}

NS_CC_END

#endif