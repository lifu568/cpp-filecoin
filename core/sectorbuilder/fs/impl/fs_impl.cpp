/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "sectorbuilder/fs/impl/fs_impl.hpp"
#include <boost/filesystem.hpp>

using fc::primitives::BigInt;

namespace fc::sectorbuilder {

  outcome::result<SectorPath> FSImpl::findSector(DataType type,
                                                 const Address &miner,
                                                 SectorNumber num) {
    SectorPath out;
    for (const auto &path : paths) {
      OUTCOME_TRY(p, StoragePath(path.first).sector(type, miner, num));

      if (!boost::filesystem::exists(p)) continue;

      if (!out.empty()) {
        if (paths[p.storage()]->is_cached) {
          return std::move(p);
        }
      }
      out = p;
    }

    if (out.empty()) {
      return FSErrors::SECTOR_NOT_FOUND;
    }

    return out;
  }

  outcome::result<StoragePath> FSImpl::findBestPath(uint64_t qty_bytes_needed,
                                                    bool need_cached,
                                                    bool strict) {
    StoragePath best;
    BigInt bestw(0);

    // If we need cache, only return cache. If we need storage, prefer storage,
    // fall back to cache
    bool bestc = true;

    for (const auto &path : paths) {
      if (path.second->is_cached != need_cached
          && (bestc != path.second->is_cached || strict))
        continue;

      int64_t available_bytes = 0;  // availableBytes

      if (uint64_t(available_bytes) < qty_bytes_needed) continue;

      BigInt w(available_bytes);
      w *= BigInt(path.second->weight);
      if (w >= bestw) {
        if (path.second->is_cached == need_cached) {
          bestw = w;
        }

        best = path.first;
        bestc = path.second->is_cached;
      }
    }

    if (best.empty()) {
      if (need_cached) {
        return FSErrors::NO_CACHED_VALUE;
      }

      return FSErrors::NO_AVAILABLE_STORAGE;
    }
    return best;
  }

  outcome::result<SectorPath> FSImpl::forceAllocSector(DataType type,
                                                       const Address &miner,
                                                       SectorSize ssize,
                                                       bool need_cached,
                                                       SectorNumber num) {
    while (true) {
      auto maybe_sector_path = findSector(type, miner, num);
      if (maybe_sector_path.has_error()) {
        if (maybe_sector_path.error() == FSErrors::SECTOR_NOT_FOUND)
          break;
        else
          return maybe_sector_path.error();
      }

      // clean
    }

    return allocSector(type, miner, ssize, need_cached, num);
  }

  outcome::result<SectorPath> FSImpl::allocSector(DataType type,
                                                  const Address &miner,
                                                  SectorSize ssize,
                                                  bool need_cached,
                                                  SectorNumber num) {
    auto maybe_sector_path = findSector(type, miner, num);
    if (maybe_sector_path.has_value())
      return maybe_sector_path.value();
    else if (maybe_sector_path.error() != FSErrors::SECTOR_NOT_FOUND)
      return maybe_sector_path.error();

    OUTCOME_TRY(overhead, overheadMul(type));
    auto need = overhead * ssize;

    OUTCOME_TRY(p, findBestPath(need, need_cached, false));

    // check reserve

    return p.sector(type, miner, num);
  }

  outcome::result<SectorPath> FSImpl::prepareCacheMove(const SectorPath &sector,
                                                       SectorSize ssize,
                                                       bool need_cached) {
    OUTCOME_TRY(p, findBestPath(ssize, need_cached, true));

    OUTCOME_TRY(miner, sector.miner());
    OUTCOME_TRY(num, sector.num());
    OUTCOME_TRY(type, sector.type());

    // check reserve

    return p.sector(type, miner, num);
  }

  outcome::result<void> FSImpl::moveSector(const SectorPath &from,
                                           const SectorPath &to) {
    if (from == to) {
      return outcome::success();
    }

    // check that is dir

    // clean

    return outcome::success();
  }

  FSImpl::FSImpl() {}

  std::shared_ptr<FS> OpenFs(gsl::span<const PathConfig> cfg) {
    return std::shared_ptr<FSImpl>();
  }

}  // namespace fc::sectorbuilder

OUTCOME_CPP_DEFINE_CATEGORY(fc::sectorbuilder, FSErrors, e) {
  using fc::sectorbuilder::FSErrors;
  switch (e) {
    case (FSErrors::SECTOR_NOT_FOUND):
      return "FS: sector not found";
    case (FSErrors::NO_CACHED_VALUE):
      return "FS: no cached value";
    case (FSErrors::NO_AVAILABLE_STORAGE):
      return "FS: no available storage";
    default:
      return "FS: unknown error";
  }
}
