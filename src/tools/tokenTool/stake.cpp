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
#include <algorithm>
#include "stake.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CStake, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextStakeChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextStakeChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CStake::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    string_q fmt = fmtIn;
    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextStakeChunk, this);
}

//---------------------------------------------------------------------------
string_q nextStakeChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CStake *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CStake::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) { address = str_2_Addr(fieldValue); return true; }
            break;
        case 'l':
            if ( fieldName % "lastPos " ) { lastPos  = str_2_Uint(fieldValue); return true; }
            break;
        case 's':
            if ( fieldName % "stake" ) { stake = str_2_Wei(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CStake::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CStake::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> address;
    archive >> lastPos ;
    archive >> stake;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CStake::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << address;
    archive << lastPos ;
    archive << stake;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CStakeArray& array) {
    uint64_t count;
    archive >> count;
    array.resize(count);
    for (size_t i = 0 ; i < count ; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CStakeArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CStake::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    size_t fieldNum = 1000;
    ADD_FIELD(CStake, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CStake, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CStake, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CStake, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CStake, "address", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CStake, "lastPos ", T_NUMBER, ++fieldNum);
    ADD_FIELD(CStake, "stake", T_WEI, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CStake, "schema");
    HIDE_FIELD(CStake, "deleted");
    HIDE_FIELD(CStake, "showing");
    HIDE_FIELD(CStake, "cname");

    builtIns.push_back(_biCStake);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextStakeChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CStake *sta = reinterpret_cast<const CStake *>(dataPtr);
    if (sta) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, sta);
                // EXISTING_CODE
                // EXISTING_CODE
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CStake::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CStake& sta) {
    sta.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CStake& sta) {
    sta.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
string_q CStake::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextStakeChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) return addr_2_Str(address);
            break;
        case 'l':
            if ( fieldName % "lastPos " ) return uint_2_Str(lastPos );
            break;
        case 's':
            if ( fieldName % "stake" ) return wei_2_Str(stake);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CStake& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
string_q CStake::display(uint64_t pos) {
    ASSERT(pMapItem);
    ostringstream os;
    if (pMapItem->lastPos == NOPOS)  // new to the list
        os << cYellow;
    else if (pos > pMapItem->lastPos)  // moved down the list
        os << cRed;
    else if (pos < pMapItem->lastPos)  // moved up the list
        os << cGreen;
    else  // stayed the same
        os << cTeal;
    os << pos << "\t" << (pMapItem->lastPos == NOPOS ? "" : uint_2_Str(pMapItem->lastPos)) << "\t" << address << "\t" << stake;
    os << cOff;
    pMapItem->lastPos = pos;
    lastPos = pos;
    return os.str();
}
// EXISTING_CODE
