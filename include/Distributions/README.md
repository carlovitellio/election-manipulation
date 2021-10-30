*Not using MuParser*
If it is not intended to use MuParser for reading a generic Distribution and
you don't want to download the MuParser library, the only thing needed is to
uncomment few lines in DistributionFactories.hpp. In particular, the ones including
the ParsedDistributions .hpp files and the lines adding the methods to the factory
in the loadFactory functions

*Adding new Distributions*
For implementing a new Distribution, it is requested to follow the
ProbabilityDistribution class and inherits from it.
If new Distributions are implemented, they have to be included in the 
DistributionFactories.hpp file. Furthermore, they have to be added in the
corresponding loadFactory function.
