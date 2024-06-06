#ifndef FORMS_H
#define FORMS_H

#include <unordered_map>
#include <string>

#include "value.h"
#include "eval_env.h"

using SpecialFormType = ValuePtr(ValuePtr&, EvalEnv&);

extern std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS;

#endif