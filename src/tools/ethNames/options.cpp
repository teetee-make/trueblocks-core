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
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("terms", "", "list<string>", OPT_REQUIRED | OPT_POSITIONAL, "a space separated list of one or more search terms"),  // NOLINT
    COption("expand", "e", "", OPT_SWITCH, "expand search to include all fields (default searches name, address, and symbol only)"),  // NOLINT
    COption("match_case", "m", "", OPT_SWITCH, "do case-sensitive search"),
    COption("all", "l", "", OPT_SWITCH, "include all accounts in the search"),
    COption("owned", "o", "", OPT_SWITCH, "include personal accounts in the search"),
    COption("custom", "c", "", OPT_SWITCH, "include your custom named accounts"),
    COption("prefund", "p", "", OPT_SWITCH, "include prefund accounts"),
    COption("named", "n", "", OPT_SWITCH, "include well know token and airdrop addresses in the search"),
    COption("other", "t", "", OPT_HIDDEN | OPT_SWITCH, "export other addresses if found"),
    COption("addr", "a", "", OPT_SWITCH, "display only addresses in the results (useful for scripting)"),
    COption("collections", "s", "", OPT_SWITCH, "display collections data"),
    COption("tags", "g", "", OPT_SWITCH, "export the list of tags and subtags only"),
    COption("to_custom", "u", "", OPT_HIDDEN | OPT_SWITCH, "for editCmd only, is the edited name a custom name or not"),
    COption("clean", "C", "", OPT_HIDDEN | OPT_SWITCH, "clean the data (addrs to lower case, sort by addr)"),
    COption("", "", "", OPT_DESCRIPTION, "Query addresses and/or names of well known accounts."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

string_q shortenFormat(const string_q& fmtIn);
string_q getSearchFields(const string_q& fmtIn);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    CStringArray terms;
    bool expand = false;
    bool all = false;
    bool owned = false;
    bool custom = false;
    bool prefund = false;
    bool named = false;
    bool other = false;
    bool addr = false;
    bool to_custom = false;
    bool clean = false;
    // END_CODE_LOCAL_INIT

    string_q format;
    bool deflt = true;
    bool addr_only = false;

    Init();
    blknum_t latest = getLatestBlock_client();
    latestBlock = latest;
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-e" || arg == "--expand") {
            expand = true;

        } else if (arg == "-m" || arg == "--match_case") {
            match_case = true;

        } else if (arg == "-l" || arg == "--all") {
            all = true;

        } else if (arg == "-o" || arg == "--owned") {
            owned = true;

        } else if (arg == "-c" || arg == "--custom") {
            custom = true;

        } else if (arg == "-p" || arg == "--prefund") {
            prefund = true;

        } else if (arg == "-n" || arg == "--named") {
            named = true;

        } else if (arg == "-t" || arg == "--other") {
            other = true;

        } else if (arg == "-a" || arg == "--addr") {
            addr = true;

        } else if (arg == "-s" || arg == "--collections") {
            collections = true;

        } else if (arg == "-g" || arg == "--tags") {
            tags = true;

        } else if (arg == "-u" || arg == "--to_custom") {
            to_custom = true;

        } else if (arg == "-C" || arg == "--clean") {
            clean = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            if (!parseStringList2(this, terms, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    if (clean)
        return handle_clean();

    if (collections) {
        exportCollections(terms);
        return false;
    }

    if (isCrudCommand()) {
        if (!processEditCommand(terms, to_custom))
            return false;
    }
    for (auto term : terms)
        searches.push_back(term);

    CAccountName unused;
    getNamedAccount(unused, "0x0");

    if (expand) {
        searchFields = STR_DISPLAY_ACCOUNTNAME;
        format = searchFields;
    }

    if (all)
        types = ALL;
    if (named) {
        if (deflt) {
            types = 0;
            deflt = false;
        }
        types |= NAMED;
    }
    if (prefund) {
        if (deflt) {
            types = 0;
            deflt = false;
        }
        types |= PREFUND;
    }
    if (custom) {
        if (deflt) {
            types = 0;
            deflt = false;
        }
        types |= CUSTOM;
    }
    if (owned) {
        if (deflt) {
            types = 0;
            deflt = false;
        }
        types |= OWNED;
    }
    if (other) {
        if (deflt) {
            types = 0;
            deflt = false;
        }
        types |= OTHER;
    }

    if (addr) {
        addr_only = true;
        isNoHeader = true;
        format = "[{ADDRESS}]";
        searchFields = "[{ADDRESS}]\t[{NAME}]";
    }

    if (verbose)
        searchFields += "\t[{SOURCE}]";

    if (tags) {
        manageFields("CAccountName:all", false);
        manageFields("CAccountName:tags", true);
        format = "[{TAGS}]";
        addr_only = false;
        types |= NAMED;
        types |= PREFUND;
        types |= CUSTOM;
        types |= OWNED;
        types |= OTHER;
    }

    // Prepare formatting
    string_q str = (format.empty() ? shortenFormat(STR_DISPLAY_ACCOUNTNAME) : format);
    if (verbose && !contains(format, "{SOURCE}"))
        str += "\t[{SOURCE}]";
    string_q meta = ", \"namePath\": \"" + (isTestMode() ? "--" : getCachePath("names/")) + "\"";

    // Display formatting
    configureDisplay("ethNames", "CAccountName", str, meta);
    if (!tags && (expContext().exportFmt == API1 || expContext().exportFmt == JSON1))
        manageFields("CAccountName:" + cleanFmt(STR_DISPLAY_ACCOUNTNAME));
    if (!expand) {
        HIDE_FIELD(CAccountName, "deleted");
        HIDE_FIELD(CAccountName, "is_custom");
        HIDE_FIELD(CAccountName, "is_prefund");
        HIDE_FIELD(CAccountName, "is_erc20");
        HIDE_FIELD(CAccountName, "is_erc721");
        HIDE_FIELD(CAccountName, "type");
    }

    // Collect results for later display
    applyFilter();

    // Data wrangling
    if (addr_only) {
        HIDE_FIELD(CAccountName, "deleted");
        HIDE_FIELD(CAccountName, "tags");
        HIDE_FIELD(CAccountName, "name");
        HIDE_FIELD(CAccountName, "symbol");
        HIDE_FIELD(CAccountName, "description");
        HIDE_FIELD(CAccountName, "source");
        HIDE_FIELD(CAccountName, "decimal");
        HIDE_FIELD(CAccountName, "is_custom");
        HIDE_FIELD(CAccountName, "is_prefund");
        HIDE_FIELD(CAccountName, "is_erc20");
        HIDE_FIELD(CAccountName, "is_erc721");
        HIDE_FIELD(CAccountName, "type");
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_PREFUND | OPT_OUTPUT | OPT_CRUD);

    // BEG_CODE_INIT
    match_case = false;
    collections = false;
    tags = false;
    // END_CODE_INIT

    items.clear();
    searches.clear();
    searchFields = getSearchFields(STR_DISPLAY_ACCOUNTNAME);
    types = NAMED;
    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));

    establishFolder(getCachePath("names/"));
    CAccountName acct;
    getNamedAccount(acct, "0x0");  // loads names database
    Init();
    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("With a single search term, the tool searches both `name` and `address`.");
    notes.push_back("With two search terms, the first term must match the `address` field, and the second term must match the `name` field.");  // NOLINT
    notes.push_back("When there are two search terms, both must match.");
    notes.push_back("The `--match_case` option requires case sensitive matching. It works with all other options.");
    notes.push_back("To customize the list of names add a `custom` section to the config file (see documentation).");
    // clang-format on
    // END_CODE_NOTES
    notes.push_back("Name file: `" + configPathRelative("names/names.tab") + "` (" +
                    uint_2_Str(fileSize(configPath("names/names.tab"))) + ")");

    // BEG_ERROR_MSG
    // END_ERROR_MSG
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//-----------------------------------------------------------------------
bool COptions::addIfUnique(const CAccountName& item) {
    if (isZeroAddr(item.address))
        return false;

    if (isTestMode() && !isCrudCommand()) {
        if (items.size() > 200)
            return true;
        if ((contains(item.tags, "Kickback") || contains(item.tags, "Humanity")))  // don't expose people during testing
            return true;
    }

    if (tags) {
        string_q key = item.tags;
        if (items[key].tags == key)  // already exists
            return false;
        items[key] = item;
        return true;
    }

    address_t key = toLower(item.address);
    if (items[key].address == key) {  // it's already in the map, but we want the last in name to win
        bool empty = item.name.empty();
        bool isDifferent = items[key].name != item.name;
        bool isOwned = startsWith(items[key].name, "Owned_");
        bool isPrefund = startsWith(items[key].name, "Prefund_");
        if (!empty && (isDifferent || isOwned || isPrefund)) {
            items[key].name = item.name;
        }
        return false;
    }

    if (searches.size() == 0) {
        items[key] = item;
        return true;
    }

    if (!match_case) {
        for (size_t i = 0; i < searches.size(); i++) {
            searches[i] = toLower(searches[i]);
        }
    }

    string_q search1 = searches.size() > 0 ? searches[0] : "";
    string_q search2 = searches.size() > 1 ? searches[1] : "";
    string_q search3 = searches.size() > 2 ? searches[2] : "";

    string_q str = item.Format(searchFields);
    if (!match_case)
        str = toLower(str);

    if ((search1.empty() || search1 == "*" || contains(str, search1)) &&
        (search2.empty() || search2 == "*" || contains(str, search2)) &&
        (search3.empty() || search3 == "*" || contains(str, search3))) {
        items[key] = item;
        return true;
    }
    items.erase(key);
    return false;
}

//-----------------------------------------------------------------------
void COptions::applyFilter() {
    //------------------------
    if (types & OWNED) {
        nodeRequired();
        if (isTestMode()) {
            for (uint32_t i = 1; i < 5; i++) {
                CAccountName item;
                item.address = "0x000000000000000000000000000000000000000" + uint_2_Str(i);
                addIfUnique(item);
            }
        } else {
            CStringArray addrs;
            getAccounts(addrs);
            uint32_t cnt = 0;
            for (auto addr : addrs) {
                CAccountName item;
                item.tags = "00-Active";
                item.address = addr;
                item.name = "Owned_" + padNum4(cnt++);
                addIfUnique(item);
            }
        }
    }

    //------------------------
    if (types & CUSTOM) {
        if (isTestMode() && !isCrudCommand()) {
            for (uint32_t i = 1; i < 5; i++) {
                CAccountName item;
                item.tags = "81-Custom";
                item.address = "0x000000000000000000000000000000000000000" + uint_2_Str(i);
                item.name = "Account_" + uint_2_Str(i);
                if (!(i % 2)) {
                    item.symbol = "AC_" + uint_2_Str(i);
                    item.decimals = i;
                }
                item.source = "Testing";
                addIfUnique(item);
            }
        } else {
            for (auto item : namedAccounts) {
                if (item.is_custom)
                    addIfUnique(item);
            }
        }
    }

    //------------------------
    if (types & NAMED) {
        for (auto item : namedAccounts) {
            if (!item.is_custom && !item.is_prefund && !startsWith(item.tags, "81-Other"))
                addIfUnique(item);
        }
    }

    //------------------------
    if (types & PREFUND) {
        for (auto item : namedAccounts) {
            if (item.is_prefund)
                addIfUnique(item);
        }
    }

    //------------------------
    if (!isTestMode() && (types & OTHER)) {
        for (auto item : namedAccounts) {
            if (startsWith(item.tags, "81-Other"))
                addIfUnique(item);
        }
    }
}

//-----------------------------------------------------------------------
string_q shortenFormat(const string_q& fmtIn) {
    string_q ret = toUpper(fmtIn);
    replace(ret, "[{SOURCE}]", "");
    replace(ret, "[{DESCRIPTION}]", "");
    replace(ret, "[{DECIMAL}]", "");
    replace(ret, "[{DELETED}]", "");
    replace(ret, "[{IS_CUSTOM}]", "");
    replace(ret, "[{IS_PREFUND}]", "");
    replace(ret, "[{IS_ERC20}]", "");
    replace(ret, "[{IS_ERC721}]", "");
    replace(ret, "[{TYPE}]", "");
    return trim(ret, '\t');
}

//-----------------------------------------------------------------------
string_q getSearchFields(const string_q& fmtIn) {
    string_q ret = toUpper(fmtIn);
    replace(ret, "[{SOURCE}]", "");
    replace(ret, "[{DESCRIPTION}]", "");
    replace(ret, "[{DECIMAL}]", "");
    replace(ret, "[{DELETED}]", "");
    replace(ret, "[{IS_CUSTOM}]", "");
    replace(ret, "[{IS_PREFUND}]", "");
    replace(ret, "[{IS_ERC20}]", "");
    replace(ret, "[{IS_ERC721}]", "");
    replace(ret, "[{TYPE}]", "");
    return trim(ret, '\t');
}

//-----------------------------------------------------------------------
void pushToOutput(CAccountNameArray& out, const CAccountName& name, bool to_custom) {
    if (to_custom && !name.is_custom)
        return;
    if (!to_custom && name.is_custom)
        return;
    out.push_back(name);
}

//-----------------------------------------------------------------------
bool COptions::processEditCommand(CStringArray& terms, bool to_custom) {
    if (!contains("create|update|delete|undelete|remove", crudCommand))
        return usage("Invalid edit command '" + crudCommand + "'. Quitting...");

    CAccountName target;
    target.address = trim(getEnvStr("TB_NAME_ADDRESS"), '\"');
    if (target.address.empty()) {
        target.address = terms[0];
    }
    target.name = trim(getEnvStr("TB_NAME_NAME"), '\"');
    target.tags = trim(getEnvStr("TB_NAME_TAG"), '\"');
    target.source = trim(getEnvStr("TB_NAME_SOURCE"), '\"');
    target.symbol = trim(getEnvStr("TB_NAME_SYMBOL"), '\"');
    target.decimals = str_2_Uint(trim(getEnvStr("TB_NAME_DECIMALS"), '\"'));
    target.description = trim(getEnvStr("TB_NAME_DESCR"), '\"');
    target.is_custom = str_2_Bool(trim(getEnvStr("TB_NAME_CUSTOM"), '\"')) || to_custom;
    finishClean(target);

    if (!isApiMode() && isTestMode()) {
        cout << string_q(45, '-') << endl;
        cout << target << endl;
        cout << string_q(45, '-') << endl;
    }

    bool isEdit = crudCommand == "create" || crudCommand == "update";
    string_q fmt = isEdit ? "tags\taddress\tname\tsymbol\tsource\tdescription\tdecimals\tdeleted\tis_custom\tis_"
                            "prefund\tis_erc20\tis_erc721\ttype"
                          : "address";
    CStringArray fields;
    explode(fields, fmt, '\t');

    CAccountNameArray outArray;
    outArray.reserve(namedAccounts.size() + 2);

    bool edited = false;
    for (auto name : namedAccounts) {
        if (name.address == target.address) {
            if (crudCommand == "remove") {
                // do nothing (i.e. skip this name)
                LOG4("Removing ", name.address);
            } else if (crudCommand == "delete") {
                name.m_deleted = true;
                pushToOutput(outArray, name, to_custom);
                LOG4("Deleting ", name.address);
            } else if (crudCommand == "undelete") {
                name.m_deleted = false;
                pushToOutput(outArray, name, to_custom);
                LOG4("Undeleting ", name.address);
            } else {
                name = target;
                pushToOutput(outArray, name, to_custom);
                LOG4("Editing ", name.address);
                terms.clear();
                terms.push_back(target.address);  // we only need the address for the search
            }
            edited = true;
        } else {
            pushToOutput(outArray, name, to_custom);
        }
    }

    if (crudCommand == "create" && !edited) {
        pushToOutput(outArray, target, to_custom);
        LOG4("Creating ", target.address);
        terms.clear();
        terms.push_back(target.address);  // we only need the address for the search
    }

    sort(outArray.begin(), outArray.end());
    LOG4("Finished sorting...");

    fmt = STR_DISPLAY_ACCOUNTNAME;
    ostringstream dataStream2;
    string_q fieldStr = toLower(substitute(substitute(fmt, "[{", ""), "}]", ""));
    string_q testMode = getEnvStr("TEST_MODE");
    setenv("TEST_MODE", "false", true);
    dataStream2 << fieldStr << endl;
    for (auto name : outArray) {
        if (!name.is_prefund)  // if the user has customized a prefund, save it as a custom as well
            dataStream2 << name.Format(fmt) << endl;
    }
    setenv("TEST_MODE", testMode.c_str(), true);

    string_q dest = to_custom ? configPath("names/names_custom.tab") : configPath("names/names.tab");
    stringToAsciiFile(dest, dataStream2.str());
    namedAccounts.clear();
    ::remove(getCachePath("names/names.bin").c_str());
    LOG4("Finished writing...");

    return true;
}
