#ifndef ORACLE_H
#include <Eigen/Dense>
#include <functional>

enum class WithinX {
    NO,
    MAYBE,
    ABSOLUTELY,
    YES
};

struct DistanceResponse {
    WithinX wx;
    Eigen::VectorXd* direction;
    double distanceBound;
};

typedef std::function<DistanceResponse* (Eigen::MatrixXd, Eigen::VectorXd, Eigen::VectorXd, double, double)> oracleFunction;

#define ORACLE_H
#endif // !ORACLE_H