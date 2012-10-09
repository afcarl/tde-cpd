#ifndef __DHMMSEGMENTER_H__
#define __DHMMSEGMENTER_H__
#include <numeric>
#include <Eigen/Core>
#include <rlfd_segmentation/Model.hh>
#include <deque>
#include <memory>

namespace rlfd {
/**
 * A class to perform time series segmentation.
 *
 * Implements a variant of:
 *
 * Kohlmorgen, J., & Lemm, S. (2001). A dynamic hmm
 * for on-line segmentation of sequential data. Advances in Neural Information Processing Systems
 * 14 (NIPS 2001) (pp. 793–800). Vancouver, British
 * Columbia, Canada: MIT Press.
 */
class DHMMSegmenter
{
public:
    DHMMSegmenter(double varsigma=0, double k=0);
    virtual ~DHMMSegmenter() {};

    /**
     * Add an observation (delay-embedded). 
     * When the number of observations is sufficient
     * to build a model, the segmentation will get 
     * updated automatically
     * @param x An observation vector 
     */
    void AddObservation(const Eigen::VectorXd& x);

    /**
     * Regularization constant C that subsumes varsigma and k
     */
    double GetRegularizer() { return 2*std::pow(varsigma, 2)*std::log(k); }

private:
    struct State {
        State(double cpath, Model* model, Model* prototype): 
            cpath(cpath), model(model), prototype(prototype) {}

        // Cost of the c-path finishing at this state
        double cpath;

        // Model computed for this state. 
        // Can be a PDF or delay embedding 
        std::shared_ptr<Model> model;

        // The prototype assigned to this state
        std::shared_ptr<Model> prototype;
    };

    std::deque<State> states;

    // Maintain a history of the optimal paths costs
    std::deque<double> opaths;

    double varsigma;
    double k;
};

} // namespace rlfd

#endif
