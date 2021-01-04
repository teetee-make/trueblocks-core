#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "etherlib.h"
#include "transaction.h"
#include "reconciliation.h"
#include "ethstate.h"

namespace qblocks {

// EXISTING_CODE
typedef enum { FM_PRODUCTION, FM_STAGING } freshen_e;
// EXISTING_CODE

//--------------------------------------------------------------------------
class CMonitor : public CAccountName {
  public:
    CAbi abi_spec;
    CReconciliation summaryStatement;
    CEthStateArray stateHistory;
    wei_t curBalance;
    bool enabled;
    freshen_e fm_mode;

  public:
    CMonitor(void);
    CMonitor(const CMonitor& mo);
    virtual ~CMonitor(void);
    CMonitor& operator=(const CMonitor& mo);

    DECLARE_NODE(CMonitor);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    bloom_t bloom;
    bloom_t getBloom(void);
    uint64_t cntBefore;
    bool needsRefresh;
    CMonitor(const string_q& _addr, const string_q& _name, blknum_t fB, blknum_t lB);
    CMonitor(const address_t& _addr, const string_q& _name);
    bool inBlock;
    CArchive* tx_cache;
    void writeLastBlock(blknum_t bn);
    void updateLastExport(blknum_t bn);
    void writeAnArray(const CAppearanceArray_base& array);
    void writeARecord(blknum_t bn, blknum_t tx_id);
    bool openCacheFile1(void);
    blknum_t nextBlockAsPerMonitor(void) const;
    bool isLocked(string_q& msg) const;
    bool clearLocks(void);
    void moveToProduction(void);
    bool loadAndSort(CAppearanceArray& items);
    uint64_t getRecordCount(void) const;
    bool exists(void) const;
    blknum_t getLastVisitedBlock(void) const;
    blknum_t getLastExportedBlock(void) const;
    bool isDeleted(void) const;
    void undeleteMonitor(void);
    void deleteMonitor(void);
    void removeMonitor(void);
    // EXISTING_CODE
    bool operator==(const CMonitor& it) const;
    bool operator!=(const CMonitor& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CMonitor& v1, const CMonitor& v2);
    friend ostream& operator<<(ostream& os, const CMonitor& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMonitor& mo);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMonitor::CMonitor(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMonitor::CMonitor(const CMonitor& mo) {
    // EXISTING_CODE
    tx_cache = NULL;
    // EXISTING_CODE
    duplicate(mo);
}

// EXISTING_CODE
//--------------------------------------------------------------------------
inline CMonitor::CMonitor(const string_q& _addr, const string_q& _name, blknum_t fB, blknum_t lB) {
    initialize();
    address = toLower(_addr);
    name = _name;
    firstAppearance = fB;
    latestAppearance = lB;
}

//--------------------------------------------------------------------------
inline CMonitor::CMonitor(const address_t& _addr, const string_q& _name) {
    initialize();
    address = toLower(_addr);
    name = _name;
    firstAppearance = 0;
    latestAppearance = 0;
}
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMonitor::~CMonitor(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMonitor::clear(void) {
    // EXISTING_CODE
    if (tx_cache) {
        tx_cache->Release();
        delete tx_cache;
    }
    tx_cache = NULL;
    cntBefore = 0;
    needsRefresh = false;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMonitor::initialize(void) {
    CAccountName::initialize();

    abi_spec = CAbi();
    summaryStatement = CReconciliation();
    stateHistory.clear();
    curBalance = 0;
    enabled = true;
    fm_mode = FM_PRODUCTION;

    // EXISTING_CODE
    bloom = bloom_t();
    latestAppearance = UINT_MAX;
    inBlock = false;
    tx_cache = NULL;
    cntBefore = 0;
    needsRefresh = false;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMonitor::duplicate(const CMonitor& mo) {
    clear();
    CAccountName::duplicate(mo);

    abi_spec = mo.abi_spec;
    summaryStatement = mo.summaryStatement;
    stateHistory = mo.stateHistory;
    curBalance = mo.curBalance;
    enabled = mo.enabled;
    fm_mode = mo.fm_mode;

    // EXISTING_CODE
    bloom = mo.bloom;
    latestAppearance = mo.latestAppearance;
    inBlock = mo.inBlock;
    tx_cache = NULL;  // we do not copy the tx_cache
    cntBefore = mo.cntBefore;
    needsRefresh = mo.needsRefresh;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMonitor& CMonitor::operator=(const CMonitor& mo) {
    duplicate(mo);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMonitor::operator==(const CMonitor& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMonitor& v1, const CMonitor& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMonitor> CMonitorArray;
extern CArchive& operator>>(CArchive& archive, CMonitorArray& array);
extern CArchive& operator<<(CArchive& archive, const CMonitorArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_MONITOR;

//---------------------------------------------------------------------------
// EXISTING_CODE
extern string_q getMonitorPath(const address_t& addr, freshen_e mode = FM_PRODUCTION);
extern string_q getMonitorLast(const address_t& addr, freshen_e mode = FM_PRODUCTION);
extern string_q getMonitorExpt(const address_t& addr, freshen_e mode = FM_PRODUCTION);
extern string_q getMonitorDels(const address_t& addr, freshen_e mode = FM_PRODUCTION);
extern string_q getMonitorCach(const address_t& addr, freshen_e mode = FM_PRODUCTION);
extern void cleanMonitors(const CAddressArray& addrs);
extern void cleanMonitor(const address_t& addr);
extern void establishTestMonitors(void);
typedef map<address_t, CMonitor> CMonitorMap;  // NOLINT
extern void establishMonitorFolders(void);
extern void cleanMonitorStage(void);
extern const char* STR_DISPLAY_TOKENBALANCERECORD2;
extern string_q getTokenBalanceOf(const CMonitor& token, const address_t& holder, blknum_t blockNum);
extern string_q getTokenState(const string_q& what, const CMonitor& token, blknum_t blockNum);
// EXISTING_CODE
}  // namespace qblocks
