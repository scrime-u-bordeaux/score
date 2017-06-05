#pragma once
#include <Automation/Spline/SplineAutomMetadata.hpp>
#include <Process/Process.hpp>
#include <State/Address.hpp>
#include <State/Unit.hpp>
#include <boost/container/flat_map.hpp>
#include <iscore_plugin_automation_export.h>

namespace Spline
{

struct spline_data
{
    std::size_t segments() const
    {
      return (points.size() - 1) / 3;
    }

    QPointF getP0(std::size_t i) const
    {
      if(i == 0)
      {
        return points[0];
      }
      else
      {
        return points[i*3];
      }
    }
    QPointF getP1(std::size_t i) const
    {
      if(i == 0)
        return points[1];
      return points[i*3+1];
    }
    QPointF getP2(std::size_t i) const
    {
      if(i == 0)
        return points[2];
      return points[i*3+2];
    }
    QPointF getP3(std::size_t i) const
    {
      if(i == 0)
        return points[3];
      return points[i*3+3];
    }

    static bool isRealPoint(std::size_t i)
    {
      return i == 0 || !((i + 3) % 3);
    }

    std::vector<QPointF> points;


    friend bool operator==(const spline_data& lhs, const spline_data& rhs)
    {
      return lhs.points == rhs.points;
    }
    friend bool operator!=(const spline_data& lhs, const spline_data& rhs)
    {
      return lhs.points != rhs.points;
    }
};



class ISCORE_PLUGIN_AUTOMATION_EXPORT ProcessModel final
    : public Process::ProcessModel
{
  ISCORE_SERIALIZE_FRIENDS
  PROCESS_METADATA_IMPL(Spline::ProcessModel)

  Q_OBJECT
  Q_PROPERTY(State::AddressAccessor address READ address WRITE setAddress
                 NOTIFY addressChanged)
  Q_PROPERTY(bool tween READ tween WRITE setTween NOTIFY tweenChanged)
  Q_PROPERTY(State::Unit unit READ unit WRITE setUnit NOTIFY unitChanged)

public:
  ProcessModel(
      const TimeVal& duration,
      const Id<Process::ProcessModel>& id,
      QObject* parent);
  ~ProcessModel();

  template <typename Impl>
  ProcessModel(Impl& vis, QObject* parent)
      : Process::ProcessModel{vis, parent}
  {
    vis.writeTo(*this);
  }

  State::AddressAccessor address() const;
  void setAddress(const State::AddressAccessor& arg);
  State::Unit unit() const;
  void setUnit(const State::Unit&);

  bool tween() const
  {
    return m_tween;
  }
  void setTween(bool tween)
  {
    if (m_tween == tween)
      return;

    m_tween = tween;
    emit tweenChanged(tween);
  }

  QString prettyName() const override;

  const spline_data& spline() const { return m_spline; }
  void setSpline(const spline_data& c) {
    if(m_spline != c)
    {
      m_spline = c;
      emit splineChanged();
    }
  }
signals:
  void addressChanged(const ::State::AddressAccessor&);
  void tweenChanged(bool tween);
  void unitChanged(const State::Unit&);
  void splineChanged();

private:
  //// ProcessModel ////
  void setDurationAndScale(const TimeVal& newDuration) override;
  void setDurationAndGrow(const TimeVal& newDuration) override;
  void setDurationAndShrink(const TimeVal& newDuration) override;

  bool contentHasDuration() const override;
  TimeVal contentDuration() const override;

  ProcessModel(
      const ProcessModel& source,
      const Id<Process::ProcessModel>& id,
      QObject* parent);

  State::AddressAccessor m_address;
  spline_data m_spline;

  bool m_tween = false;
};
}