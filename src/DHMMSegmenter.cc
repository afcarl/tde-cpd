#include <rlfd_segmentation/DHMMSegmenter.hh>

namespace rlfd {
DHMMSegmenter::DHMMSegmenter(double varsigma, double k) : varsigma(varsigma), k(k)
{

}

void DHMMSegmenter::AddObservation(const Eigen::VectorXd& x)
{
    // Compute a new model 
    Model* latest_model = new DelayEmbedding(); 
    states.push_front(State(latest_model->distance(*(states[0].model)), latest_model, latest_model));

    // t=0 to t-1
    for (unsigned t = 1; t < states.size()-1; t++) {
        // Cost for a constrained path finishing with the new model r
        // starting at 0 and finishing at time step t 
        states.front().cpath = latest_model->distance(*(states[t].model)) + std::min(states.front().cpath, opaths[t-1] + GetRegularizer()); 
        if (states.front().cpath < opaths[t]) {
            opaths[t] = states.front().cpath;
            states[t].prototype = std::shared_ptr<Model>(latest_model); // Not sure if that makes sense
        } 
    }

    // Compute the minimum cost for each state
    for (unsigned s = 0; s < states.size(); s++) {
        states[s].cpath = latest_model->distance(*(states[s].model)) + std::min(states[s].cpath, opaths.front() + GetRegularizer());
    }

    // Find the mininum cost constrained path
    std::deque<State>::iterator mincost_state = std::min_element(states.begin(), states.end(), [](const State& a, const State& b) { return a.cpath < b.cpath; });
    opaths.push_front(mincost_state->cpath);
    states.front().prototype = mincost_state->model; 
}
} // namespace rlfd
