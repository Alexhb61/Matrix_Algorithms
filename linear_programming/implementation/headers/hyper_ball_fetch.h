#ifndef HYPERBALLFETCH_H



#include <Eigen/Dense>
#include "oracle.h"

using Eigen::MatrixXd;

struct HyperBallFetchResponse {
	Eigen::VectorXd* point;
	bool isFound;
};

HyperBallFetchResponse* hyperBallFetch(oracleFunction oracle, MatrixXd A, Eigen::VectorXd b, Eigen::VectorXd startingCenter, double startingRadius, double minRadius, double hoffmanConst);

#define HYPERBALLFETCH_H

#endif // !HYPERBALLFETCH_H