add_library(retrieval_market_network
    impl/network_stream_impl.cpp
    )
target_link_libraries(retrieval_market_network
    p2p::p2p_basic_host
    outcome
    buffer
    )
