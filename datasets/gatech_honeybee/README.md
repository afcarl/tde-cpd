This dataset comes from:

Learning and Inferring Motion Patterns using Parametric Segmental Switching Linear Dynamic Systems
Sang Min Oh, James M. Rehg, Tucker Balch, Frank Dellaert
International Journal of Computer Vision (IJCV) Special Issue on Learning for Vision, May 2008. Vol.77(1-3). Pages 103-124.

and is available at [http://www.cc.gatech.edu/~borg/ijcv_psslds/]

We simplified the original .btf format to a simple 5-columns table format.

The sequence files can be produced in one line with:

$ paste label0.btf ximage.btf yimage.btf timage.btf <(cat timage.btf | \
Rscript -e 'cat(sin(as.numeric(readLines("stdin"))), sep="\n")') > sequence1.dat
