/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_CORE_SECTORBUILDER_FS_HPP
#define CPP_FILECOIN_CORE_SECTORBUILDER_FS_HPP

#include <gsl/span>
#include "sectorbuilder/fs/types.hpp"

using fc::primitives::SectorSize;

namespace fc::sectorbuilder {

  class FS {
   public:
    virtual ~FS() = default;

    virtual outcome::result<SectorPath> findSector(DataType type,
                                                   const Address &miner,
                                                   SectorNumber num) = 0;

    virtual outcome::result<StoragePath> findBestPath(uint64_t qty_bytes_needed,
                                                      bool need_cached,
                                                      bool strict) = 0;

    virtual outcome::result<SectorPath> forceAllocSector(DataType type,
                                                         const Address &miner,
                                                         SectorSize ssize,
                                                         bool need_cached,
                                                         SectorNumber num) = 0;

    virtual outcome::result<SectorPath> allocSector(DataType type,
                                                    const Address &miner,
                                                    SectorSize ssize,
                                                    bool need_cached,
                                                    SectorNumber num) = 0;

    virtual outcome::result<SectorPath> prepareCacheMove(
        const SectorPath &sector, SectorSize ssize, bool need_cached) = 0;

    virtual outcome::result<void> moveSector(const SectorPath &from,
                                             const SectorPath &to) = 0;
  };

  std::shared_ptr<FS> OpenFs(gsl::span<const PathConfig> cfg);

  enum class FSErrors{
      SECTOR_NOT_FOUND = 1,
      NO_CACHED_VALUE,
      NO_AVAILABLE_STORAGE,

  };

}  // namespace fc::sectorbuilder

OUTCOME_HPP_DECLARE_ERROR(fc::sectorbuilder, FSErrors);

#endif  // CPP_FILECOIN_CORE_SECTORBUILDER_FS_HPP
