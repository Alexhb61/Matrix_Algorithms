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
    VectorXd* direction;
    double distanceBound;
}

*DistanceResponse simpleOracle(MatrixXd* A, VectorXd* b, VectorXd* x, double r, double hoffmanConst);