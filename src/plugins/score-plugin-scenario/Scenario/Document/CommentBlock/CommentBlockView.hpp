#pragma once
#include <Scenario/Document/ScenarioDocument/ScenarioDocumentViewConstants.hpp>

#include <QGraphicsItem>
#include <QObject>
#include <QPointF>
#include <verdigris>
namespace score
{
class TextItem;
}

namespace Scenario
{
class CommentBlockPresenter;
class CommentBlockView final : public QObject, public QGraphicsItem
{
public:
  CommentBlockView(CommentBlockPresenter& presenter, QGraphicsItem* parent);

  //~TimeSyncView() = default;

  static constexpr int static_type()
  {
    return ItemType::Comment;
  }
  int type() const override { return static_type(); }

  const CommentBlockPresenter& presenter() const { return m_presenter; }

  void paint(
      QPainter* painter,
      const QStyleOptionGraphicsItem* option,
      QWidget* widget) override;

  QRectF boundingRect() const override;

  void setHtmlContent(QString htmlText);

  void setSelected(bool);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* evt) override;

private:
  void focusOnText();
  void focusOut();

  CommentBlockPresenter& m_presenter;

  score::TextItem* m_textItem{};

  QPointF m_clickedPoint{};
  QPointF m_clickedScenePoint{};
  bool m_selected{};
};
}
