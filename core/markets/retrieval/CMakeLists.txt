add_subdirectory(network)

add_library(retrieval_market_provider
    provider/retrieval_provider_impl.cpp
    provider/query_responder/query_responder_impl.cpp
    )
target_link_libraries(retrieval_market_provider
    retrieval_market_network
    )

add_executable(retrieval_market_query_demo
    demo/retrieval_query.cpp
    )
target_link_libraries(retrieval_market_query_demo
    retrieval_market_provider
    p2p::p2p
    p2p::p2p_literals
    )
