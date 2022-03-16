#include "metaball.hh"

double Metaball::evaluate_potential(Vector3 point)
{
    double res = 0;
    for (auto ball : balls)
    {
        double num = ball.get_radius() * ball.get_radius();
        auto ball_center = ball.get_center();

        double x = point.x() - ball_center.x();
        double y = point.y() - ball_center.y();
        double z = point.z() - ball_center.z();

        double den = x * x + y * y + z * z;

        res += num / den;
    }
    return res;
}
