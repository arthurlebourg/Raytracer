#include "smooth_triangle.hh"

Smooth_Triangle::Smooth_Triangle(Vector3 A, Vector3 B, Vector3 C,
                                 std::shared_ptr<Texture_Material> texture)
    : Triangle(A, B, C, texture)
    , normalA_(Triangle::normal(A))
    , normalB_(Triangle::normal(A))
    , normalC_(Triangle::normal(A))
{}

Vector3 Smooth_Triangle::normal(Vector3 point)
{
    // source:
    // https://flipcode.com/archives/Interpolating_Normals_For_Ray-Tracing.shtml
    // find the vertex farthest to hit point
    auto distA = squaredDistance(point, A_);
    auto distB = squaredDistance(point, B_);
    auto distC = squaredDistance(point, C_);

    // n stands for normal
    Vector3 far_vertex;
    Vector3 far_vertex_n;
    Vector3 close_vertex1;
    Vector3 close_vertex1_n;
    Vector3 close_vertex2;
    Vector3 close_vertex2_n;

    if (distA > distB && distA > distC)
    {
        far_vertex = A_;
        far_vertex_n = normalA_;
        close_vertex1 = B_;
        close_vertex1_n = normalB_;
        close_vertex2 = C_;
        close_vertex2_n = normalC_;
    }

    else if (distB > distA && distB > distC)
    {
        far_vertex = B_;
        far_vertex_n = normalB_;
        close_vertex1 = A_;
        close_vertex1_n = normalA_;
        close_vertex2 = C_;
        close_vertex2_n = normalC_;
    }

    else
    {
        far_vertex = C_;
        far_vertex_n = normalC_;
        close_vertex1 = A_;
        close_vertex1_n = normalA_;
        close_vertex2 = B_;
        close_vertex2_n = normalB_;
    }

    // first interpolation on the edge farthest from point
    Plane edge(close_vertex2,
               cross(close_vertex2 - close_vertex1, Triangle::normal(point)),
               texture_);

    // point on closest edge for 1st interpolation
    auto interpolation_point =
        edge.hit(Ray(far_vertex, point - far_vertex)).value();

    auto dist1 = distance(close_vertex1, interpolation_point);
    auto dist2 = distance(close_vertex1, close_vertex2);
    // normal at the 1st interpolation
    auto normal1 =
        close_vertex1_n + (close_vertex2_n - close_vertex1_n) * (dist1 / dist2);

    auto dist3 = distance(interpolation_point, point);
    auto dist4 = distance(interpolation_point, far_vertex);

    return normal1 + (far_vertex_n - normal1) * (dist3 / dist4);
}
