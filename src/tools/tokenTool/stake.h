#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
#include <vector>
#include <map>
#include "etherlib.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CStake : public CBaseNode {
public:
    address_t address;
    uint64_t lastPos ;
    wei_t stake;

public:
    CStake(void);
    CStake(const CStake& st);
    virtual ~CStake(void);
    CStake& operator=(const CStake& st);

    DECLARE_NODE(CStake);

    // EXISTING_CODE
    string_q display(uint64_t pos);
    CStake *pMapItem = NULL;
    // EXISTING_CODE
    bool operator==(const CStake& item) const;
    bool operator!=(const CStake& item) const { return !operator==(item); }
    friend bool operator<(const CStake& v1, const CStake& v2);
    friend ostream& operator<<(ostream& os, const CStake& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CStake& st);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CStake::CStake(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CStake::CStake(const CStake& st) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(st);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CStake::~CStake(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStake::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStake::initialize(void) {
    CBaseNode::initialize();

    address = "";
    lastPos  =  NOPOS;
    stake = 0;

    // EXISTING_CODE
    pMapItem = NULL;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStake::duplicate(const CStake& st) {
    clear();
    CBaseNode::duplicate(st);

    address = st.address;
    lastPos  = st.lastPos ;
    stake = st.stake;

    // EXISTING_CODE
    pMapItem = st.pMapItem;
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CStake& CStake::operator=(const CStake& st) {
    duplicate(st);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CStake::operator==(const CStake& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CStake& v1, const CStake& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    if (v1.stake == v2. stake)
        return v1.address < v2.address;
    return v1.stake < v2.stake;
}

//---------------------------------------------------------------------------
typedef vector<CStake> CStakeArray;
extern CArchive& operator>>(CArchive& archive, CStakeArray& array);
extern CArchive& operator<<(CArchive& archive, const CStakeArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CStake& sta);
extern CArchive& operator>>(CArchive& archive, CStake& sta);

//---------------------------------------------------------------------------
// EXISTING_CODE
//-------------------------------------------------------------------------
inline bool reverseSort(const CStake& v1, const CStake& v2) {
    if (v1.stake == v2. stake)
        return v2.address < v1.address;
    return v2.stake < v1.stake; // reverse sort
}
// EXISTING_CODE

