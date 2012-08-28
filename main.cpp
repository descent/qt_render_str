#include <QApplication>
//#include <QCoreApplication>

#include <QImage>
#include <QFontMetrics>
#include <QPainter>
#include <QTextStream>
#include <QTextCodec>
#include <QFile>
#include <QDebug>
#include <QtGlobal> // for qVersion()

#include <unistd.h>

#include "draw_widget.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

QImage *fp2image(const QString &fp, u32 fg, u32 bg)
{

  QImage *img = new QImage(320, 200, QImage::Format_Indexed8);
  img->fill(bg); 
  if (img->load(fp))
    qDebug() << fp << " load ok";

  return img;
}

QImage *text2image(QString str, const QFont &font, u32 fg, u32 bg)
{
  //f.setPointSize(font_size);

  //printf("str: %s\n", str);
  QFontMetrics fm(font);

  QRect text_rect_ = fm.boundingRect(str);
  qDebug() << "text_rect_:" << text_rect_;
  QImage *img = new QImage(text_rect_.width(), text_rect_.height(), QImage::Format_Mono);
  //QImage *img = new QImage(text_rect_.width(), text_rect_.height(), QImage::Format_RGB16);
  //img->fill(Qt::black); 
  img->fill(bg); 
  //img->fill(text_bg_);

  QPainter painter(img);

  painter.setPen(Qt::white);
  painter.setFont(font);
  painter.drawText(-text_rect_.x(), -text_rect_.y(), str);

#if 0
  uchar* image_addr= img->bits();
  int w16 = img->bytesPerLine();
  int img_w = img->width();
  int cur_x=0;
  
  qDebug("=============================");
  qDebug("w16: %d", w16);
  qDebug("w: %d", img_w);
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
        ++cur_x;
      }
#endif
      ++image_addr;
    }
    next_line:
    cur_x = 0;
    printf("\n");

  
  } 
#endif

  return img;
}
#define RAW_DATA 1
#define RENDER_TEXT 2
void print_mono_img(const QImage *img, int type)
{
  const uchar* image_addr= img->bits();
  int w16 = img->bytesPerLine();
  int img_w = img->width();
  int cur_x=0;
#if 1
  printf("=============================\n");
  printf("w: %d\n", img_w);
  printf("static int wb = %d;\n", w16);
  printf("static int hb = %d;\n", img->height());
  printf("u8 str[]=\n");
  printf("{\n");

  for (int y = 0 ; y < img->height() ; ++y)
  {    
    for (int x = 0 ; x < w16 ; ++x)
    { 
      u8 c = *image_addr;
          
      if (type==RAW_DATA)
        printf("%#5x,", *image_addr);
     else
     {

      for (int i=7 ; i>=0 ; --i)
      {
        if (((c >> i) & 0x1) == 1)
          printf("*");
        else
          printf("|");
        ++cur_x;
      }

      }
      ++image_addr;
    }
    next_line:
    cur_x = 0;
    printf("\n");

  
  } 
#endif
  printf("};\n");
}

void usage(const char* fn)
{
  printf("%s -s font size -t str\n", fn);
  printf("  ex:\n");
  printf("    %s -s 30 -t \"abc\"\n", fn);
  printf("qt version: %s\n", qVersion());
  exit(-1);
}

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  //QCoreApplication app(argc, argv);
  int size=20;
  int opt;
  QString str=QString::fromUtf8("A");
  QString fam = "FreeMono";
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));


  QString fp;
  while ((opt = getopt(argc, argv, "p:f:t:s:h?")) != -1)
  {
    switch (opt)
    {
      case 's':
        //translator_fp = QString(optarg);
        size = strtol(optarg, 0, 10);
        break;
      case 't':
        str=QString::fromUtf8(optarg);
        break;
      case 'p':
      {
        fp = optarg;
        break;
      }
      case 'f':
      {
        //fam = optarg;
        QFile file(optarg); // file path
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
          QTextStream in(&file);
          str=in.readAll();
          qDebug() << "open file:" << optarg << "content: " << str;
        }
        else
        {
          qDebug() << file.errorString();
        }


        break;
      }
      case 'h':
      case '?':
      default:
        usage(argv[0]);
        break;
    }
  }

  //QImage *image = fp2image("./pe256.jpg", Qt::yellow, Qt::black);
  //QString fp = "./pe256.jpg";
  QImage image;
  if (image.load(fp), QImage::Format_Indexed8)
    qDebug() << fp << " load ok";
  print_mono_img(&image, RAW_DATA);

  QVector<QRgb> color_table = image.colorTable();

  for (int i=0 ; i < color_table.size() ; ++i)
  {
    //qDebug() << color_table[i];
    unsigned int c = color_table[i];
    unsigned char r, g, b;
    r = (c >> 16) & 0xff;
    g = (c >> 8) & 0xff;
    b = c & 0xff;
    printf("i: %d ## %08x\n", i, c);
    #if 1
    printf("r: %#02x\n", r);
    printf("g: %#02x\n", g);
    printf("b: %#02x\n", b);
    #else
    printf(".byte %#02x, ", b);
    printf("%#02x, ", g);
    printf("%#02x\n", r);
    #endif
  }

#if 0
  DrawWidget draw_widget;
  draw_widget.set_image_fp(fp);
  draw_widget.show();
#endif
  //QString str=QString::fromUtf8("懷藝測試ABCDE");
  
#if 0
  QImage *image = text2image(str, QFont(fam, size), Qt::yellow, Qt::black);
  print_mono_img(image, RENDER_TEXT);
  print_mono_img(image, RAW_DATA);
#endif

#if 1
  app.exit(0);
#else
  return app.exec();
#endif
}

