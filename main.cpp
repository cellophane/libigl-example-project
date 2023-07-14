#include <igl/opengl/glfw/Viewer.h>
#include <igl/triangle/triangulate.h>
#include <igl/harmonic.h>
#include "json.hpp"
// Input polygon
Eigen::MatrixXd V;
Eigen::MatrixXi E;
Eigen::MatrixXd H;
Eigen::MatrixXd U,U_bc;
// Triangulated interior
Eigen::MatrixXd V2;
Eigen::MatrixXi F2;

int main(int argc, char* argv[])
{
    using namespace Eigen;
    using namespace std;
    nlohmann::json json;
    ifstream ifs;
    ifs.open("tile.json");
    ifs >> json;
    ifs.close();
    int n = json["count"];
    cout << n << endl;
    // Create the boundary of a square
    V.resize(n, 2);
    E.resize(n, 2);
    H.resize(1, 2);
    
    for (int i = 0; i < n; i++) {
     
        V(i,0) = json["point"][i][0];
        V(i,1) = json["point"][i][1];
        E(i,0) = i;
        E(i,1) = (i + 1) % n;
       
    }
   
    /*
    // create two squares, one with edge length of 4,
    // one with edge length of 2
    // both centered at origin
    V << -1, -1, 1, -1, 1, 1, -1, 1,
        -2, -2, 2, -2, 2, 2, -2, 2;

    // add the edges of the squares
    E << 0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4;
*/
    // specify a point that is inside a closed shape
    // where we do not want triangulation to happen
    //H << 100, 100;

    // Triangulate the interior
    // a0.005 means that the area of each triangle should
    // not be greater than 0.005
    // q means that no angles will be smaller than 20 degrees
    // for a detailed set of commands please refer to:
    // https://www.cs.cmu.edu/~quake/triangle.switch.html
    //cout << "hello?";
    igl::triangle::triangulate(V, E, H, "a0.5q", V2, F2);
    Eigen::VectorXi b;
    b.resize(n);
    
    for (int i = 0; i < n; i++) {
        b[i] = i;
    }
    U_bc.resize(n, 2);
    for (int i = 0; i < n; i++) {
        U_bc(i, 0) = cos(i * 2 * 3.142 / n)*4;
        U_bc(i, 1) = sin(i * 2 * 3.142 / n)*4;
    }
    cout << "harmonic" << endl;
    igl::harmonic(V2, F2, b, U_bc, 2, U);
    // Plot the generated mesh
    igl::opengl::glfw::Viewer viewer;
    viewer.data().set_mesh(U, F2);
    viewer.launch();
}