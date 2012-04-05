/**
 * imagegraphicsitem.cpp
 *
 * Toke Høiland-Jørgensen
 * 2012-04-05
 */

#include "imagegraphicsitem.h"
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QtGui/QTransform>
#include <QtGui/QMenu>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QDebug>

ImageGraphicsItem::ImageGraphicsItem(QGraphicsItem *parent)
  :QGraphicsPixmapItem(parent)
{
  init();
}

ImageGraphicsItem::ImageGraphicsItem(const QPixmap &pixmap, QGraphicsItem *parent)
  :QGraphicsPixmapItem(parent)
{
  init();
  setPixmap(pixmap);
}

void ImageGraphicsItem::init()
{
  setFlags(QGraphicsItem::ItemIsSelectable);
  setCursor(Qt::CrossCursor);
}

ImageGraphicsItem::~ImageGraphicsItem()
{
}

void ImageGraphicsItem::setPixmap(const QPixmap &pixmap)
{
  QGraphicsPixmapItem::setPixmap(pixmap);
  QSize s = pixmap.size();
  QPointF p(-s.width()/2.0f, -s.height()/2.0f);
  setPos(p);
  scene()->setSceneRect(sceneBoundingRect());
  foreach(QGraphicsView *view, scene()->views()) {
    // Scale the graphics view to leave 15 pixels of air on each side
    // of the image, and recenter it.
    QRect r = view->frameRect();
    float scale = qMin((r.width()-30.0f)/s.width(), (r.height()-30.0f)/s.height());
    scale = qMax(scale, 0.01f);
    scale = qMin(scale, 4.0f);
    QTransform transform = QTransform::fromScale(scale, scale);
    view->centerOn(this);
    view->setTransform(transform);
  }
}

void ImageGraphicsItem::removePOI(POIItem *poi)
{
  QPoint p = poi->pos().toPoint();
  delete poi;
  emit POIRemoved(p);
}

void ImageGraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  QPoint p = event->pos().toPoint();
  qDebug("Adding POI at (%d,%d)", p.x(), p.y());
  emit newPOI(p);

  POIItem * poi = new POIItem(this);
  poi->setPos(p);
}

void ImageGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  event->ignore();
}

void ImageGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  scene()->clearSelection();
  event->ignore();
}