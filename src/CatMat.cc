#include <rlfd/utils/ImportExport.hh>
#include <iostream>

int main(int argc, char** argv)
{
  Eigen::MatrixXd out;
  rlfd::utils::Import(argv[1], out); 
  std::cout << out << std::endl;
  return 0;
}
