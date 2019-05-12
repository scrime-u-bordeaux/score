#pragma once
#include <QAction>
#include <QLineEdit>
#include <score_lib_base_export.h>
#include <wobjectdefs.h>

namespace score
{

class SCORE_LIB_BASE_EXPORT SearchLineEdit : public QLineEdit
{
  W_OBJECT(SearchLineEdit)
public:
  SearchLineEdit(QWidget* parent);

  ~SearchLineEdit() override;
  virtual void search() = 0;
};

}