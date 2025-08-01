#include <Eigen/Dense>
 
using Eigen::MatrixXd;

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

DistanceResponse* simpleOracle(MatrixXd* A, Eigen::VectorXd* b, Eigen::VectorXd* x, double r, double hoffmanConst);