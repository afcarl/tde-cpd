#ifndef __GNUPLOT_H__
#define __GNUPLOT_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <stdexcept>

namespace rlfd {
namespace utils {
class Gnuplot 
{
 public:
  Gnuplot()
  {
    gp_ = popen("gnuplot -persist","w"); 
    if (gp_ == NULL) {
      throw std::runtime_error(strerror(errno));
    }
  }

  ~Gnuplot() 
  {
    fflush(gp_);
    fclose(gp_);
  }

  void operator()(const std::vector<double>& ind, const std::vector<double>& dep)
  {
    // TODO Check if dim is the same
    fprintf(gp_, "plot '-' with points\n");
    for (unsigned i = 0; i < ind.size(); i++) {
      fprintf(gp_, "%.15le %.15le\n", ind[i], dep[i]); 
      fflush(gp_);
    }
    fprintf(gp_, "\ne\n");
  }

 private:
  FILE* gp_;
};
} // namespace utils
} // namespace rlfd
#endif 
