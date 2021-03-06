#
# Copyright Soramitsu Co., Ltd. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0
#

add_library(hexutil
    hexutil.hpp
    hexutil.cpp
    )
target_link_libraries(hexutil
    Boost::boost
    outcome
    )

add_library(blob
    blob.hpp
    blob.cpp
    )
target_link_libraries(blob
    hexutil
    )

add_library(outcome INTERFACE)
target_link_libraries(outcome INTERFACE
    Boost::boost
    p2p::p2p
    )

add_library(buffer
    buffer.hpp
    buffer.cpp
    buffer_back_insert_iterator.cpp
    )
target_link_libraries(buffer
    hexutil
    )

add_library(logger
    logger.cpp
    )
target_link_libraries(logger
    spdlog::spdlog
    )

add_subdirectory(libp2p)

add_library(todo_error
    todo_error.hpp
    todo_error.cpp
    )
target_link_libraries(todo_error
    outcome
    )
