#include "MemMan.h"
#include "Node.h"
#include "DefFileReader.h"
#include "Formula.h"
#include "ScriptReader.h"
#include "ExecutionTemplate.h"
#include "json.hpp"
#include <string>
#include "ExecutionContext.h"
#include <list>
#include "Command.h"
#include <vector>
#include "Int.h"
#include "OTPParser.h"
#include "MetaData.h"
#include "NamedPipeOperations.h"

using json = nlohmann::json;

Formula::Formula() {}

std::string Formula::BuildExecutionTemplateJSONString(MSTRING defFilePath, MSTRING queryString) {
    int id = 0;
    DefFileReader dfr;
    // CAUTION: This file path is hardcoded and can cause crashes. You have been warned!
    MetaData *pMD = dfr.Read(defFilePath);
    ScriptReader sr;
    ScriptReaderOutput op;
    std::string bSucc = sr.ProcessScript(pMD, op, queryString);
    if (bSucc != "") {
        std::wcout << "\nFailed to read script\n";
    }
    ExecutionContext ec;
    ec.p_mapFunctions = &op.map_Functions;
    ec.p_MD = pMD;
    Node *pY = MemoryManager::Inst.CreateNode(++id);
    Node *pRESULT = MemoryManager::Inst.CreateNode(++id);
    OTPParser otp;
    // pass default json
    // use empty json
    Node *root = otp.OTPJSONToNodeTree("[]");
    std::string s;
    root->SetValue((char *) s.c_str());
    ec.map_Var["X"] = root;
    ec.map_Var["Y"] = pY;
    ec.map_Var["RESULT"] = pRESULT;
    ExecutionTemplateList *f = op.p_ETL;
    Formula eTemplate;
    return eTemplate.ConvertExecutionTemplateToJSON(f);
}

std::string Formula::ConvertExecutionTemplateToJSON(ExecutionTemplateList *f) {
    json j;
    if (f->size() != 1) {
        j = {{"Error", "The FCL query should be written using one line"}};
        return j.dump();
    }
    ExecutionTemplate *v;
    v = f->at(0);
    return ExecutionTemplateToJSON(v).dump(4);
}

json Formula::EntityToJson(MULONG e) {
    json j;
    j = {{"ul_type", e}};
    return j;
}

json Formula::PEntityToJson(MULONG i) {
    json j;
    j = {{"ul_type", i}};
    return j;
}

json Formula::ListOfCommandToJson(LST_COMMANDPTR lc) {
    json lcJson = {{}};
    std::vector<json> arr;

    for (std::list<PCOMMAND>::iterator it = lc.begin(); it != lc.end(); it++) {
        Command c;
        c = **it;
        json j = {
                {"ul_CommandType",       c.ul_CommandType},
                {"p_Arg",                ExecutionTemplateToJSON(c.p_Arg)},
                {"s_AdditionalFuncName", c.s_AdditionalFuncName},
        };
        if (c.p_EntityArg != PENTITY{}) {
            j["p_EntityArg"] = PEntityToJson(c.p_EntityArg->ul_Type);
        }
        arr.push_back(j);
    }
    for (int i = 0; i < int(lc.size()); ++i) {
        lcJson[i] = arr[i];
    }
    return lcJson;
}

json Formula::ExecutionTemplateToJSON(ExecutionTemplate *et) {
    json j = {
            {"s_StartVarName",    et->s_StartVarName},
            {"p_Entity",          PEntityToJson(ENTITY_TYPE_NULL)},
            {"lst_Commands",      {}},
            {"ul_SpecialCommand", et->ul_SpecialCommand},
            {"s_CodeLine",        et->s_CodeLine},
            {"ul_type",           et->ul_Type},
    };
    if (et->p_Entity != PENTITY{}) {
        j["p_Entity"] = PEntityToJson(et->p_Entity->ul_Type);
    }
    if (et->lst_Commands.size() != 0) {
        j["lst_Commands"] = ListOfCommandToJson(et->lst_Commands);
    }
    return j;
}

std::string Formula::ProcessLine(std::string sLine) {

    DefFileReader dfr;
    ScriptReader sr;
    json j;
    MetaData *pMD = dfr.Read("D:\\Tracified-99x\\FCL-Improvements\\tracified-grammar-release\\Defs.txt");
    auto x = sr.ProcessLine(sLine, pMD);
    std::string val = x.p_ET->GetStartVarName();
    std::string val2 = x.p_ET->GetCodeLine();
    j = {{"startV",   val},
         {"codeLine", val2}};
    return j.dump();
}


