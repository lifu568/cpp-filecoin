add_subdirectory(network)

add_library(retrieval_market_provider
    provider/retrieval_provider_impl.cpp
    )
target_link_libraries(retrieval_market_provider
    retrieval_market_network
    )

add_executable(dev dev.cpp)
target_link_libraries(dev
    outcome
    )
