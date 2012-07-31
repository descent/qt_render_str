#include <QtGui/QApplication>
#include <QtGui>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

QImage *text2image(QString str, const QFont &font, u32 fg, u32 bg)
{
  //f.setPointSize(font_size);

  qDebug() << "str: " << str;
  QFontMetrics fm(font);

  QRect text_rect_ = fm.boundingRect(str);
  qDebug() << "text_rect_:" << text_rect_;
  QImage *img = new QImage(text_rect_.width(), text_rect_.height(), QImage::Format_Mono);
  //QImage *img = new QImage(text_rect_.width(), text_rect_.height(), QImage::Format_RGB16);
  img->fill(Qt::black);
  //img->fill(bg);
  //img->fill(text_bg_);

  QPainter painter(img);

  painter.setPen(Qt::white);
  painter.setFont(font);
  painter.drawText(-text_rect_.x(), -text_rect_.y(), str);

  uchar* image_addr= img->bits();
  int w16 = img->bytesPerLine();
  
#if 1
  qDebug("=============================");
  qDebug("w16: %d", w16);
  qDebug("h: %d", img->height());

  for (int y = 0 ; y < img->height() ; ++y)
  {    
    for (int x = 0 ; x < w16 ; ++x)
    { 
      u8 c = *image_addr;
          
#if 0
      printf("%x ", *image_addr);
#else
      for (int i=7 ; i>=0 ; --i)
      {
        if (((c >> i) & 0x1) == 1)
          printf("*");
        else
          printf("|");
      }
#endif
      ++image_addr;
    }
    printf("\n");

  
  } 
#endif

  return img;
}

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  QStringList args = app.arguments();

  QString fp("./mmadplayer.xml");

  if (args.size() > 2)
  {
    if (args.at(1) == "-f")
    {
      fp = args.at(2);
    }

  }
  qDebug() << "fp:" << fp;


  //QString str=QString::fromUtf8("懷藝測試ABCDE");
  QString str=QString::fromUtf8("A");
  
  QImage *image = text2image(str, QFont("FreeMono", 20), Qt::yellow, Qt::black);

  return app.exec();
}
