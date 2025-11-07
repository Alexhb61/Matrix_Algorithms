#include <hyper_ball_fetch.h>
#include "oracle.h"

HyperBallFetchResponse* hyperBallFetch(oracleFunction oracle, MatrixXd A, Eigen::VectorXd b, Eigen::VectorXd startingCenter, double startingRadius, double minRadius, double hoffmanConst) {
    Eigen::VectorXd center = Eigen::VectorXd(startingCenter); 
	double radius_squared = startingRadius * startingRadius;
    while (radius_squared >= minRadius * minRadius) {
        DistanceResponse* response = oracle(A, b, center, sqrt(radius_squared), hoffmanConst);
        if (response->wx == WithinX::ABSOLUTELY) {
            return new HyperBallFetchResponse{ new Eigen::VectorXd(center), true};
        }
        if (response->wx == WithinX::YES) {
            radius_squared = response->distanceBound * response->distanceBound;
        }
        if (response->wx == WithinX::MAYBE) {
            center = center - ( *(response->direction) * response->distanceBound);
        }
        if (response->wx == WithinX::NO) {
            return new HyperBallFetchResponse{ NULL, false };
        }
    }
    return new HyperBallFetchResponse{ NULL, false };

}