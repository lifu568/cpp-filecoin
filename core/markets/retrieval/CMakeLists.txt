add_subdirectory(network)

add_library(retrieval_market_client
    client/retrieval_client_impl.cpp
    )
target_link_libraries(retrieval_market_client
    p2p::p2p_basic_host
    )

add_executable(dev dev.cpp)
target_link_libraries(dev
    outcome
    )
