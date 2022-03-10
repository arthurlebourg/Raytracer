#include "metaball.hh"

float Metaball::evaluate_potential(Vector3 point)
{
    float res = 0;
    for (auto ball : balls)
    {
        float num = ball.get_radius() * ball.get_radius();
        auto ball_center = ball.get_center();

        float x = point.x() - ball_center.x();
        float y = point.y() - ball_center.y();
        float z = point.z() - ball_center.z();

        float den = x * x + y * y + z * z;

        res += num / den;
    }
    return res;
}
