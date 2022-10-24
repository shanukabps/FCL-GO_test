//
// Created by SINGER on 02-Sep-22.
//

#ifndef FORMULA_FORMULA_H
#define FORMULA_FORMULA_H

#include <string>
#include "CommonIncludes.h"
#include "ExecutionContext.h"
#include "Entity.h"
#include "json.hpp"

using json = nlohmann::json;

struct EntityS {
    MULONG ul_type;
};

struct ExecutionTemplateS {
    MSTRING s_StartVarName;
    LST_COMMANDPTR lst_Commands;
    PENTITY p_Entity;
    MULONG ul_SpecialCommand;
    MSTRING s_CodeLine;
};

class Formula {

public:
    Formula();

    std::string ProcessLine(std::string sLine);

    json EntityToJson(MULONG e);

    json PEntityToJson(MULONG e);

    json ListOfCommandToJson(LST_COMMANDPTR lc);

    json CommandToJson(Command c);

    std::string ConvertExecutionTemplateToJSON(ExecutionTemplateList *f);

    std::string BuildExecutionTemplateJSONString(MSTRING defFilePath, MSTRING queryString);

    std::string ExecutionTemplateToJSON(ExecutionTemplateList *ec);

    json ExecutionTemplateToJSON(ExecutionTemplate *ec);

};


#endif //FORMULA_FORMULA_H
