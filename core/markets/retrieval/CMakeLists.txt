add_subdirectory(network)

add_library(retrieval_market_provider
    provider/retrieval_provider_impl.cpp
    provider/query_responder/query_responder_impl.cpp
    )
target_link_libraries(retrieval_market_provider
    retrieval_market_network
    )

add_library(retrieval_market_client
    client/retrieval_client_impl.cpp
    )
target_link_libraries(retrieval_market_client
    retrieval_market_network
    cbor
    )

add_executable(retrieval_market_query_demo
    demo/retrieval_query.cpp
    )
target_link_libraries(retrieval_market_query_demo
    retrieval_market_provider
    retrieval_market_client
    p2p::p2p
    p2p::p2p_literals
    )

add_executable(dev
    dev.cpp
    )
target_link_libraries(dev
    host_crypto_context
    )
