#ifndef SIMPLE_ORACLE_H

#include <Eigen/Dense>
#include "oracle.h"
 
using Eigen::MatrixXd;

DistanceResponse* simpleOracle(MatrixXd A, Eigen::VectorXd b, Eigen::VectorXd x, double r, double hoffmanConst);

#define SIMPLE_ORACLE_H
#endif // !SIMPLE_ORACLE_H