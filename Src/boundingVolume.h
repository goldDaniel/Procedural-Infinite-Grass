#ifndef BOUNDING_VOLUME_H
#define BOUNDING_VOLUME_H	
 
#include <glm/glm.hpp>
using namespace glm;
#include <memory>
using namespace std;

class BoundingVolume
{
public:

    enum TestResult
    {
            TEST_OUTSIDE,
            TEST_INTERSECT,
            TEST_INSIDE
    };
};
 
class BoundingBox : public BoundingVolume
{
public:

    vec3 minimum;
    vec3 maximum;
    
    vec3 position;

    BoundingBox( vec3 minimum, vec3 maximum, vec3 position = vec3(0.0f) ) : BoundingVolume()
    {
            this->minimum = minimum;
            this->maximum = maximum;
            
            this->position = position;
    }

    virtual vec3 getPositiveVertex( const vec3 &normal ) const;
    virtual vec3 getNegativeVertex( const vec3 &normal ) const;

    TestResult testIntersection(const vec3& point) const;
    TestResult testIntersection(BoundingBox& box) const;
};
 
class Frustum : public BoundingVolume
{
public:

        enum Plane
        {
                PLANE_BACK,
                PLANE_FRONT,
                PLANE_RIGHT,
                PLANE_LEFT,
                PLANE_TOP,
                PLANE_BOTTOM
        };

        Frustum( const mat4 &viewMatrix, const mat4 &projectionMatrix );

        const vec4 &getPlane( const int plane ) const;
        
        TestResult testIntersection( const vec3 &point ) const;
        TestResult testIntersection(  BoundingBox box ) const;

protected:

        vec4 m_planes[6];
};
 
 
 
// compute frustum planes from view and projection matrices
Frustum::Frustum( const mat4 &viewMatrix, const mat4 &projectionMatrix ) : BoundingVolume()
{
        const mat4 &v = viewMatrix;
        const mat4 &p = projectionMatrix;
 
        mat4 clipMatrix;
 
        clipMatrix[0][0] = v[0][0 ]*p[0][0]+v[0][1]*p[1][0]+v[0][2]*p[2][0]+v[0][3]*p[3][0];
        clipMatrix[1][0] = v[0][0]*p[0][1]+v[0][1]*p[1][1]+v[0][2]*p[2][1]+v[0][3]*p[3][1];
        clipMatrix[2][0] = v[0][0]*p[0][2]+v[0][1]*p[1][2]+v[0][2]*p[2][2]+v[0][3]*p[3][2];
        clipMatrix[3][0] = v[0][0]*p[0][3]+v[0][1]*p[1][3]+v[0][2]*p[2][3]+v[0][3]*p[3][3];
        clipMatrix[0][1] = v[1][0]*p[0][0]+v[1][1]*p[1][0]+v[1][2]*p[2][0]+v[1][3]*p[3][0];
        clipMatrix[1][1] = v[1][0]*p[0][1]+v[1][1]*p[1][1]+v[1][2]*p[2][1]+v[1][3]*p[3][1];
        clipMatrix[2][1] = v[1][0]*p[0][2]+v[1][1]*p[1][2]+v[1][2]*p[2][2]+v[1][3]*p[3][2];
        clipMatrix[3][1] = v[1][0]*p[0][3]+v[1][1]*p[1][3]+v[1][2]*p[2][3]+v[1][3]*p[3][3];
        clipMatrix[0][2] = v[2][0]*p[0][0]+v[2][1]*p[1][0]+v[2][2]*p[2][0]+v[2][3]*p[3][0];
        clipMatrix[1][2] = v[2][0]*p[0][1]+v[2][1]*p[1][1]+v[2][2]*p[2][1]+v[2][3]*p[3][1];
        clipMatrix[2][2] = v[2][0]*p[0][2]+v[2][1]*p[1][2]+v[2][2]*p[2][2]+v[2][3]*p[3][2];
        clipMatrix[3][2] = v[2][0]*p[0][3]+v[2][1]*p[1][3]+v[2][2]*p[2][3]+v[2][3]*p[3][3];
        clipMatrix[0][3] = v[3][0]*p[0][0]+v[3][1]*p[1][0]+v[3][2]*p[2][0]+v[3][3]*p[3][0];
        clipMatrix[1][3] = v[3][0]*p[0][1]+v[3][1]*p[1][1]+v[3][2]*p[2][1]+v[3][3]*p[3][1];
        clipMatrix[2][3] = v[3][0]*p[0][2]+v[3][1]*p[1][2]+v[3][2]*p[2][2]+v[3][3]*p[3][2];
        clipMatrix[3][3] = v[3][0]*p[0][3]+v[3][1]*p[1][3]+v[3][2]*p[2][3]+v[3][3]*p[3][3];
 
        m_planes[PLANE_RIGHT].x = clipMatrix[3][0]-clipMatrix[0][0];
        m_planes[PLANE_RIGHT].y = clipMatrix[3][1]-clipMatrix[0][1];
        m_planes[PLANE_RIGHT].z = clipMatrix[3][2]-clipMatrix[0][2];
        m_planes[PLANE_RIGHT].w = clipMatrix[3][3]-clipMatrix[0][3];
 
        m_planes[PLANE_LEFT].x = clipMatrix[3][0]+clipMatrix[0][0];
        m_planes[PLANE_LEFT].y = clipMatrix[3][1]+clipMatrix[0][1];
        m_planes[PLANE_LEFT].z = clipMatrix[3][2]+clipMatrix[0][2];
        m_planes[PLANE_LEFT].w = clipMatrix[3][3]+clipMatrix[0][3];
 
        m_planes[PLANE_BOTTOM].x = clipMatrix[3][0]+clipMatrix[1][0];
        m_planes[PLANE_BOTTOM].y = clipMatrix[3][1]+clipMatrix[1][1];
        m_planes[PLANE_BOTTOM].z = clipMatrix[3][2]+clipMatrix[1][2];
        m_planes[PLANE_BOTTOM].w = clipMatrix[3][3]+clipMatrix[1][3];
 
        m_planes[PLANE_TOP].x = clipMatrix[3][0]-clipMatrix[1][0];
        m_planes[PLANE_TOP].y = clipMatrix[3][1]-clipMatrix[1][1];
        m_planes[PLANE_TOP].z = clipMatrix[3][2]-clipMatrix[1][2];
        m_planes[PLANE_TOP].w = clipMatrix[3][3]-clipMatrix[1][3];
 
        m_planes[PLANE_BACK].x = clipMatrix[3][0]-clipMatrix[2][0];
        m_planes[PLANE_BACK].y = clipMatrix[3][1]-clipMatrix[2][1];
        m_planes[PLANE_BACK].z = clipMatrix[3][2]-clipMatrix[2][2];
        m_planes[PLANE_BACK].w = clipMatrix[3][3]-clipMatrix[2][3];
 
        m_planes[PLANE_FRONT].x = clipMatrix[3][0]+clipMatrix[2][0];
        m_planes[PLANE_FRONT].y = clipMatrix[3][1]+clipMatrix[2][1];
        m_planes[PLANE_FRONT].z = clipMatrix[3][2]+clipMatrix[2][2];
        m_planes[PLANE_FRONT].w = clipMatrix[3][3]+clipMatrix[2][3];
 
        for( int i = 0; i < 6; i++ )
        {
            m_planes[i] = glm::normalize( m_planes[i] );
        }
}
 
// check whether an AABB intersects the frustum
BoundingVolume::TestResult Frustum::testIntersection( BoundingBox box ) const
{
    TestResult result = TEST_INSIDE;

    for( uint i = 0; i < 6; i++ )
    {
        const float pos = m_planes[i].w;
        const vec3 normal = vec3(m_planes[i]);

        if( glm::dot(normal, box.getPositiveVertex(normal))+pos < 0.0f )
        {
                return TEST_OUTSIDE;
        }

        if( glm::dot(normal, box.getNegativeVertex(normal))+pos < 0.0f )
        {
                result = TEST_INTERSECT;
        }
    }

    return result;
}
 
vec3 BoundingBox::getPositiveVertex( const vec3 &normal ) const
{
    vec3 positiveVertex = minimum;

    if( normal.x >= 0.0f ) positiveVertex.x = maximum.x;
    if( normal.y >= 0.0f ) positiveVertex.y = maximum.y;
    if( normal.z >= 0.0f ) positiveVertex.z = maximum.z;

    return position+positiveVertex;
}
 
vec3 BoundingBox::getNegativeVertex( const vec3 &normal ) const
{
    vec3 negativeVertex = maximum;

    if( normal.x >= 0.0f ) negativeVertex.x = minimum.x;
    if( normal.y >= 0.0f ) negativeVertex.y = minimum.y;
    if( normal.z >= 0.0f ) negativeVertex.z = minimum.z;

    return position+negativeVertex;
}
#endif