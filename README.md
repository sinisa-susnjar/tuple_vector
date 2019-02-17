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

