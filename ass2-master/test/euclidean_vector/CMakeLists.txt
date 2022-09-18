cxx_test(
   TARGET euclidean_vector_test1
   FILENAME "euclidean_vector_test1.cpp"
   LINK euclidean_vector
)

cxx_test(
   TARGET constructor_tests
   FILENAME "constructor_tests.cpp"
   LINK euclidean_vector
)

cxx_test(
   TARGET operator_tests
   FILENAME "operator_tests.cpp"
   LINK euclidean_vector
)

cxx_test(
   TARGET friend_test
   FILENAME "friend_test.cpp"
   LINK euclidean_vector
)

cxx_test(
   TARGET member_test
   FILENAME "member_test.cpp"
   LINK euclidean_vector
)

cxx_test(
   TARGET utility_test
   FILENAME "utility_test.cpp"
   LINK euclidean_vector
)