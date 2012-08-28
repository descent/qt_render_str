#include "draw_widget.h"

DrawWidget::DrawWidget( QWidget * parent, Qt::WindowFlags f):QWidget(parent, f)
{

}
#if 1

void DrawWidget::paintEvent ( QPaintEvent * event )
{
  qDebug() << "pp";
  QPainter painter(this);
  QRect rect(0,0,160, 100);

  painter.drawImage(rect, *image_);
}
#endif
