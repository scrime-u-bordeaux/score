// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "SlotHandle.hpp"

#include <Process/Style/ScenarioStyle.hpp>
#include <Scenario/Document/Interval/IntervalPresenter.hpp>
#include <Scenario/Document/Interval/IntervalView.hpp>
#include <Scenario/Document/Interval/Temporal/TemporalIntervalPresenter.hpp>

#include <score/graphics/GraphicsItem.hpp>

#include <QApplication>
#include <QCursor>
#include <QDrag>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QGraphicsView>
#include <QMimeData>
#include <QPainter>
#include <QPoint>
#include <qnamespace.h>

namespace Scenario
{
SlotHandle::SlotHandle(
    const IntervalPresenter& slotView,
    int slotIndex,
    QGraphicsItem* parent)
  : QGraphicsItem{parent}
  , m_presenter{slotView}
  , m_width{slotView.view()->boundingRect().width()}
  , m_slotIndex{slotIndex}
{
  this->setCacheMode(QGraphicsItem::NoCache);
  this->setCursor(Qt::SizeVerCursor);
}

int SlotHandle::slotIndex() const
{
  return m_slotIndex;
}

void SlotHandle::setSlotIndex(int v)
{
  m_slotIndex = v;
}

QRectF SlotHandle::boundingRect() const
{
  return {-0.5, 0., m_width - 1., handleHeight()};
}

void SlotHandle::paint(
    QPainter* painter,
    const QStyleOptionGraphicsItem* option,
    QWidget* widget)
{
  const auto& style = Process::Style::instance();
  //
  //painter->setPen(Qt::red);
  //painter->setBrush(Qt::blue);
  //painter->drawRect(boundingRect());
  painter->fillRect(boundingRect(), style.ProcessViewBorder.getBrush());
}

void SlotHandle::setWidth(qreal width)
{
  prepareGeometryChange();
  m_width = width;
  update();
}

void SlotHandle::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
  m_presenter.pressed(event->scenePos());
  event->accept();
}

void SlotHandle::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
  static bool moving = false;

  if (!moving)
  {
    moving = true;
    auto p = event->scenePos();
    m_presenter.moved(p);

    auto view = getView(*this);
    if (view)
      view->ensureVisible(p.x(), p.y(), 1, 1);
    moving = false;
  }
  event->accept();
}

void SlotHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
  m_presenter.released(event->scenePos());
  event->accept();
}
}
