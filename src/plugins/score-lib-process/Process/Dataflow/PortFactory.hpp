#pragma once
#include <Process/Dataflow/Port.hpp>

#include <score/plugins/Interface.hpp>
#include <score/plugins/InterfaceList.hpp>
class QGraphicsItem;
namespace Inspector
{
class Layout;
}
namespace Dataflow
{
class PortItem;
}
namespace Process
{
class SCORE_LIB_PROCESS_EXPORT PortFactory : public score::InterfaceBase
{
  SCORE_INTERFACE(Process::Port, "4d461658-5c27-4a12-ba97-3d9392561ece")
public:
  ~PortFactory() override;

  virtual Process::Port* load(const VisitorVariant&, QObject* parent) = 0;
  virtual Dataflow::PortItem* makeItem(
      Process::Inlet& port,
      const score::DocumentContext& ctx,
      QGraphicsItem* parent,
      QObject* context);
  virtual Dataflow::PortItem* makeItem(
      Process::Outlet& port,
      const score::DocumentContext& ctx,
      QGraphicsItem* parent,
      QObject* context);

  virtual void setupInletInspector(
      Process::Inlet& port,
      const score::DocumentContext& ctx,
      QWidget* parent,
      Inspector::Layout& lay,
      QObject* context);
  virtual void setupOutletInspector(
      Process::Outlet& port,
      const score::DocumentContext& ctx,
      QWidget* parent,
      Inspector::Layout& lay,
      QObject* context);

  virtual QGraphicsItem* makeControlItem(
      Process::ControlInlet& port,
      const score::DocumentContext& ctx,
      QGraphicsItem* parent,
      QObject* context);
  virtual QGraphicsItem* makeControlItem(
      Process::ControlOutlet& port,
      const score::DocumentContext& ctx,
      QGraphicsItem* parent,
      QObject* context);

  virtual QWidget* makeControlWidget(
      Process::ControlInlet& port,
      const score::DocumentContext& ctx,
      QGraphicsItem* parent,
      QObject* context);

};

class SCORE_LIB_PROCESS_EXPORT PortFactoryList final
    : public score::InterfaceList<PortFactory>
{
public:
  using object_type = Process::Port;
  ~PortFactoryList();
  Process::Port* loadMissing(const VisitorVariant& vis, QObject* parent) const;
};

SCORE_LIB_PROCESS_EXPORT
void writeInlets(
    DataStreamWriter& wr,
    const Process::PortFactoryList& pl,
    Process::Inlets& ports,
    QObject* parent);

SCORE_LIB_PROCESS_EXPORT
void writeInlets(
    const QJsonArray& wr,
    const Process::PortFactoryList& pl,
    Process::Inlets& ports,
    QObject* parent);

SCORE_LIB_PROCESS_EXPORT
void writeOutlets(
    DataStreamWriter& wr,
    const Process::PortFactoryList& pl,
    Process::Outlets& ports,
    QObject* parent);
SCORE_LIB_PROCESS_EXPORT
void writeOutlets(
    const QJsonArray& wr,
    const Process::PortFactoryList& pl,
    Process::Outlets& ports,
    QObject* parent);

SCORE_LIB_PROCESS_EXPORT
void readPorts(
    DataStreamReader& wr,
    const Process::Inlets& ins,
    const Process::Outlets& outs);

SCORE_LIB_PROCESS_EXPORT
void readPorts(
    QJsonObject& obj,
    const Process::Inlets& ins,
    const Process::Outlets& outs);

SCORE_LIB_PROCESS_EXPORT
void writePorts(
    DataStreamWriter& wr,
    const Process::PortFactoryList& pl,
    Process::Inlets& ins,
    Process::Outlets& outs,
    QObject* parent);

SCORE_LIB_PROCESS_EXPORT
void writePorts(
    const QJsonObject& obj,
    const Process::PortFactoryList& pl,
    Process::Inlets& ins,
    Process::Outlets& outs,
    QObject* parent);
}
