#pragma once
#include <iscore/command/PropertyCommand.hpp>

class SetCurveMin : public iscore::PropertyCommand
{
        ISCORE_COMMAND_DECL("AutomationControl", "SetCurveMin", "Set curve minimum")
    public:
        ISCORE_PROPERTY_COMMAND_DEFAULT_CTOR(SetCurveMin, "AutomationControl")

        SetCurveMin(Path<AutomationModel>&& path, double newval):
            iscore::PropertyCommand{std::move(path), "min", newval, factoryName(), commandName(), description()}
        {

        }
};
