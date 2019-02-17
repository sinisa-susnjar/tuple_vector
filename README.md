# tuple_vector

A specialised container class based on std::vector for *fast* find() and lower_bound()
operations on strictly increasing timeseries.

# Use-cases

The typical use-case is to read strictly increasing timeseries data once, and then
only append to it. At times you also need to find a value for a given key using find()
or lower_bound().
The performance for appending, array operations and iterations is in the same ballpark as
std::vector, while the find() and lower_bound() performance is 10x and 5x faster than std::map.

# Operation

The find() and lower_bound() methods piggyback on the properties of strictly increasing
timeseries by implementing an interpolation search with ~O(log log n) complexity.

# Sample

    RUNNING TESTS FOR time_t
    emplace()
                 runtime    vector     tuple       map
        vector     19185        --    28.68%    93.19%
         tuple     26902   -40.22%        --    90.45%
           map    281797 -1368.82%  -947.49%        --

    operator []
                 runtime    vector     tuple       map
        vector      6808        --    24.57%    99.66%
         tuple      9026   -32.57%        --    99.55%
           map   2015667-29504.05%-22231.50%        --

    iterator
                 runtime    vector     tuple       map
        vector      6481        --    28.20%    95.18%
         tuple      9026   -39.27%        --    93.29%
           map    134510 -1975.44% -1390.24%        --

    find()
                 runtime     tuple       map
         tuple     43860        --    91.84%
           map    537419 -1125.29%        --

    lower_bound()
                 runtime     tuple       map
         tuple    106517        --    80.10%
           map    535223  -402.47%        --

    RUNNING TESTS FOR boost::posix_time::ptime
    emplace()
                 runtime    vector     tuple       map
        vector     19073        --    28.99%    93.25%
         tuple     26858   -40.82%        --    90.50%
           map    282718 -1382.28%  -952.60%        --

    operator []
                 runtime    vector     tuple       map
        vector      6540        --    28.07%    99.67%
         tuple      9092   -39.02%        --    99.54%
           map   1987052-30280.80%-21754.27%        --

    iterator
                 runtime    vector     tuple       map
        vector      6744        --    24.91%    95.01%
         tuple      8981   -33.17%        --    93.36%
           map    135225 -1905.03% -1405.57%        --

    find()
                 runtime     tuple       map
         tuple    163989        --    69.19%
           map    532240  -224.56%        --

    lower_bound()
                 runtime     tuple       map
         tuple    375379        --    50.55%
           map    759051  -102.21%        --

