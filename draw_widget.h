#ifndef DRAW_WIDGET_H
#define DRAW_WIDGET_H

#include <QtGui>

class DrawWidget : public QWidget
{
  Q_OBJECT
  public:
    DrawWidget( QWidget * parent = 0, Qt::WindowFlags f = 0 );
    bool set_image_fp(const QString &fp)
    {
  image_ = new QImage(320, 200, QImage::Format_Indexed8);
  image_->fill(Qt::black);
  if (image_->load(fp))
  {
    qDebug() << fp << ": ok";
  }

  printf("w: %d\n", image_->width());
  printf("h: %d;\n", image_->height());
  printf("color count: %d;\n", image_->colorCount());
  QVector<QRgb> color_table = image_->colorTable();
  for (int i=0 ; i < color_table.size() ; ++i)
    qDebug() << color_table[i];
    }

    void paintEvent ( QPaintEvent * event );
  private:
    QImage *image_;

};


#endif
