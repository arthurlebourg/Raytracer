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

        // when we are in the ball's center we are above the threshold
        if (den == 0)
            return 100;

        res += num / den;
    }
    return res;
}
