// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "ConditionView.hpp"

#include <Process/Style/ScenarioStyle.hpp>

#include <QColor>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QPolygon>
#include <QSize>
#include <QVector>
#include <QCursor>
#include <qnamespace.h>

#include <wobjectimpl.h>
W_OBJECT_IMPL(Scenario::ConditionView)
namespace Scenario
{
static const QPainterPath conditionTrianglePath{[] {
  QPainterPath p;
  QPainterPathStroker s;
  s.setCapStyle(Qt::RoundCap);
  s.setJoinStyle(Qt::RoundJoin);
  s.setWidth(2);

  p.addPolygon(
      QVector<QPointF>{QPointF(25, 5), QPointF(25, 21), QPointF(32, 14)});
  p.closeSubpath();

  return p + s.createStroke(p);
}()};

ConditionView::ConditionView(QGraphicsItem* parent)
    : QGraphicsItem{parent}
{
  this->setCacheMode(QGraphicsItem::NoCache);
  setFlag(ItemStacksBehindParent, true);
  setCursor(Qt::CursorShape::CrossCursor);
  changeHeight(0);
}

QRectF ConditionView::boundingRect() const
{
  constexpr double penWidth = 0.;
  return QRectF{-penWidth, -penWidth, m_width + penWidth, m_height + m_CHeight + penWidth};
}

void ConditionView::paint(
    QPainter* painter,
    const QStyleOptionGraphicsItem* option,
    QWidget* widget)
{
  auto& style = Process::Style::instance();
  painter->setRenderHint(QPainter::Antialiasing, true);

  const QBrush& col = !m_selected
      ? m_status.conditionStatusColor(style).getBrush()
      : style.IntervalSelected.getBrush();

  style.ConditionPen.setBrush(col);
  style.ConditionTrianglePen.setBrush(col);

  painter->setPen(style.ConditionPen);
  painter->setBrush(style.TransparentBrush);
  painter->drawPath(m_Cpath);

#if !defined(SCORE_IEEE_SKIN)
  painter->setPen(style.ConditionTrianglePen);
  painter->setBrush(col);

  painter->fillPath(conditionTrianglePath, col);
#endif

  painter->setRenderHint(QPainter::Antialiasing, false);
}

void ConditionView::changeHeight(qreal newH)
{
  prepareGeometryChange();
  m_height = newH;

  m_Cpath = QPainterPath();

  QRectF rect(boundingRect().topLeft(), QSize(m_CHeight, m_CHeight));
  QRectF bottomRect(
      QPointF(
          boundingRect().bottomLeft().x(),
          boundingRect().bottomLeft().y() - m_CHeight),
      QSize(m_CHeight, m_CHeight));

  m_Cpath.moveTo(boundingRect().width() / 2., 2.);
  m_Cpath.arcTo(rect, 60., 120.);
  m_Cpath.lineTo(0., m_height + m_CHeight / 2.);
  m_Cpath.arcTo(bottomRect, -180., 120.);

  auto& style = Process::Style::instance();
  QPainterPathStroker stk;
  stk.setWidth(1.);
  stk.setCapStyle(style.ConditionPen.capStyle());
  stk.setJoinStyle(style.ConditionPen.joinStyle());
  m_strokedCpath = stk.createStroke(m_Cpath) + conditionTrianglePath;

  this->update();
}

void ConditionView::setStatus(ExecutionStatus c)
{
  m_status.set(c);
  update();
}

void ConditionView::setSelected(bool selected)
{
  m_selected = selected;
  update();
}

void ConditionView::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
  if (event->button() == Qt::MouseButton::LeftButton && contains(event->pos()))
  {
    event->accept();
    pressed(event->scenePos());
  }
  else
  {
    event->ignore();
  }
}
}

QPainterPath Scenario::ConditionView::shape() const
{
  return m_strokedCpath;
}

bool Scenario::ConditionView::contains(const QPointF& point) const
{
  return m_Cpath.contains(point) || m_strokedCpath.contains(point);
}

QPainterPath Scenario::ConditionView::opaqueArea() const
{
  return m_strokedCpath;
}