/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_FS_IMPL_HPP
#define CPP_FILECOIN_FS_IMPL_HPP

#include "sectorbuilder/fs/fs.hpp"

#include <unordered_map>

namespace fc::sectorbuilder {
  class FSImpl : public FS {

      FSImpl();

    outcome::result<SectorPath> findSector(DataType type,
                                           const Address &miner,
                                           SectorNumber num) override;

    outcome::result<StoragePath> findBestPath(uint64_t qty_bytes_needed,
                                              bool need_cached,
                                              bool strict) override;

    outcome::result<SectorPath> forceAllocSector(DataType type,
                                                 const Address &miner,
                                                 SectorSize ssize,
                                                 bool need_cached,
                                                 SectorNumber num) override;

    outcome::result<SectorPath> allocSector(DataType type,
                                            const Address &miner,
                                            SectorSize ssize,
                                            bool need_cached,
                                            SectorNumber num) override;

    outcome::result<SectorPath> prepareCacheMove(const SectorPath &sector,
                                                 SectorSize ssize,
                                                 bool need_cached) override;

    outcome::result<void> moveSector(const SectorPath &from,
                                     const SectorPath &to) override;

   private:


    std::unordered_map<std::string, std::shared_ptr<PathInfo>> paths;
    std::unordered_map<std::string, std::unordered_map<DataType, uint64_t>>
        reserved;

    // TODO locks with channels

    std::mutex lk;
  };
}  // namespace fc::sectorbuilder

#endif  // CPP_FILECOIN_FS_IMPL_HPP
