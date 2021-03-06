#pragma once
#include <Scenario/Document/TimeSync/TimeSyncModel.hpp>
#include <score/command/PropertyCommand.hpp>
#include <Scenario/Commands/ScenarioCommandFactory.hpp>
#include <State/Expression.hpp>

#include <score/command/Command.hpp>
#include <score/model/path/Path.hpp>

struct DataStreamInput;
struct DataStreamOutput;

namespace Scenario
{
class TimeSyncModel;
namespace Command
{
using TimeSyncModel = ::Scenario::TimeSyncModel;
class SCORE_PLUGIN_SCENARIO_EXPORT SetAutoTrigger final : public score::Command
{
  SCORE_COMMAND_DECL(
      CommandFactoryName(),
      SetAutoTrigger,
      "Change a trigger")
public:
  SetAutoTrigger(const TimeSyncModel& tn, bool t);

  void undo(const score::DocumentContext& ctx) const override;
  void redo(const score::DocumentContext& ctx) const override;

protected:
  void serializeImpl(DataStreamInput&) const override;
  void deserializeImpl(DataStreamOutput&) override;

private:
  Path<TimeSyncModel> m_path;
  bool m_old{}, m_new{};
};
}
}

#if defined(SCORE_MUSICAL)
PROPERTY_COMMAND_T(Scenario::Command, SetTimeSyncTempo, TimeSyncModel::p_tempo, "Set tempo")
PROPERTY_COMMAND_T(Scenario::Command, SetTimeSyncSignature, TimeSyncModel::p_signature, "Set signature")
#endif
