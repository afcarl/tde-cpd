#ifndef __DHMMSEGMENTER_H__
#define __DHMMSEGMENTER_H__

#include <Eigen/Core>

namespace rlfd {

class DHMMSegmenter
{
public:
    DHMMSegmenter();
    virtual ~DHMMSegmenter() {};

    /**
     * Add an observation (delay-embedded). 
     * When the number of observations is sufficient
     * to build a model, the segmentation will get 
     * updated automatically
     * @param x An observation vector 
     */
    void addObservation(const Eigen::VectorXd& x);


};

} // namespace rlfd

#endif
