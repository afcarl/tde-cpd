#ifndef __MODEL_HH__
#define __MODEL_HH__

namespace rlfd {
struct Model {
    /**
     * Compute the distance of the current model to another. 
     * @param other Other model 
     * @return The distance (unspecified if proper distance metric)
     */
    virtual double distance(const Model& other) const = 0;
};

struct DelayEmbedding : Model {
    double distance(const Model& other) const override { return 0; };
};

} // namespace rlfd

#endif
