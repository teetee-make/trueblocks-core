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
#include "options.h"

bool isERC20(const address_t& addr) {
    return false;
}
bool isERC721(const address_t& addr) {
    return false;
}
//--------------------------------------------------------------------
void COptions::finishClean(CAccountName& name) {
    name.is_prefund = prefundWeiMap[name.address] > 0;
    if (isContractAt(name.address, latestBlock)) {
        name.is_erc20 = isERC20(name.address);
        name.is_erc721 = isERC721(name.address);
        name.type = "Contract";
    } else {
        name.is_erc20 = false;
        name.is_erc721 = false;
        // name.type = "EOA";
    }
}

//--------------------------------------------------------------------
bool COptions::cleanNames(const string_q& sourceIn, const string_q& destIn) {
    string_q source = sourceIn;
    string_q dest = destIn;

    replace(source, "~", getHomeFolder());
    replace(source, "$HOME", getHomeFolder());
    replaceAll(source, "//", "/");
    if (!fileExists(source))
        return false;

    cerr << "Processing names file (" << source << ") into " << dest << endl;

    string_q contents = asciiFileToString(source);
    nextTokenClear(contents, '\n');  // skip header

    CStringArray fields;
    string_q fieldStr = toLower(substitute(substitute(STR_DISPLAY_ACCOUNTNAME, "[{", ""), "}]", ""));
    explode(fields, fieldStr, '\t');

    CAccountName name;
    CAccountNameArray names;
    while (name.parseText(fields, contents)) {
        finishClean(name);
        names.push_back(name);
    }
    sort(names.begin(), names.end());

    ostringstream os;
    os << fieldStr << endl;
    for (auto n : names) {
        os << n.Format(STR_DISPLAY_ACCOUNTNAME) << endl;
    }
    stringToAsciiFile(dest, os.str());

    return true;
}

//--------------------------------------------------------------------
bool COptions::handle_clean(void) {
    ENTER("handle_clean");

    string_q mainSource = getGlobalConfig("ethNames")->getConfigStr("settings", "source", "<UNSET>");
    string_q mainDest = configPath("names/names.tab");
    if (!cleanNames(mainSource, mainDest))
        EXIT_USAGE("Primary names file (" + mainSource + ") not found. Quitting...");

    string_q customSource = getGlobalConfig("ethNames")->getConfigStr("settings", "custom", "<UNSET>");
    string_q customDest = configPath("names/names_custom.tab");
    if (!cleanNames(customSource, customDest))
        EXIT_USAGE("Custom names file (" + customSource + ") not found. Quitting...");

    ::remove(getCachePath("names/names.bin").c_str());
    CAccountName acct;
    getNamedAccount(acct, "0x0");  // reloads

    return false;  // don't proceed
}
