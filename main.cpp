#include <QtGui/QApplication>
#include <QtGui>

#include <unistd.h>


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
  qDebug("=============================");
  qDebug("w: %d", img_w);
  qDebug("static int wb = %d;", w16);
  qDebug("static int hb = %d;", img->height());
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
  exit(-1);
}

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  int size=20;
  char *cstr = "A";
  int opt;
  QString fam = "FreeMono";

  while ((opt = getopt(argc, argv, "f:t:s:h?")) != -1)
  {
    switch (opt)
    {
      case 's':
        //translator_fp = QString(optarg);
        size = strtol(optarg, 0, 10);
        break;
      case 't':
        cstr = optarg;
        break;
      case 'f':
        fam = optarg;
        break;
      case 'h':
      case '?':
      default:
        usage(argv[0]);
        break;
    }
  }


  //QString str=QString::fromUtf8("懷藝測試ABCDE");
  QString str=QString::fromUtf8(cstr);
  
  QImage *image = text2image(str, QFont(fam, size), Qt::yellow, Qt::black);
  print_mono_img(image, RENDER_TEXT);
  print_mono_img(image, RAW_DATA);

  return app.exec();
}

