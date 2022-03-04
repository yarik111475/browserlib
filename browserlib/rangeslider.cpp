#include "rangeslider.h"

#include <QDebug>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QStyleOptionSlider>
#include <QApplication>
#include <QStylePainter>
#include <QStyle>
#include <QToolTip>


// --------------------------------------------------------------------------
RangeSlider::RangeSlider(QWidget* _parent): QSlider(_parent)
{
  init();
}

// --------------------------------------------------------------------------
RangeSlider::RangeSlider( Qt::Orientation o, QWidget* parentObject ):QSlider(o, parentObject)
{
  init();
}
// --------------------------------------------------------------------------
RangeSlider::~RangeSlider()
{
}

// --------------------------------------------------------------------------
int RangeSlider::minimumValue() const
{
  return m_MinimumValue;
}

// --------------------------------------------------------------------------
void RangeSlider::setMinimumValue( int min )
{
  this->setValues( min, qMax(m_MaximumValue,min) );
}

// --------------------------------------------------------------------------
int RangeSlider::maximumValue() const
{
  return m_MaximumValue;
}

// --------------------------------------------------------------------------
void RangeSlider::setMaximumValue( int max )
{
  this->setValues( qMin(m_MinimumValue, max), max );
}

// --------------------------------------------------------------------------
void RangeSlider::setValues(int l, int u)
{
  const int minValue =
    qBound(this->minimum(), qMin(l,u), this->maximum());
  const int maxValue =
    qBound(this->minimum(), qMax(l,u), this->maximum());
  bool emitMinValChanged = (minValue != m_MinimumValue);
  bool emitMaxValChanged = (maxValue != m_MaximumValue);

  m_MinimumValue = minValue;
  m_MaximumValue = maxValue;

  bool emitMinPosChanged =
    (minValue != m_MinimumPosition);
  bool emitMaxPosChanged =
    (maxValue != m_MaximumPosition);
  m_MinimumPosition = minValue;
  m_MaximumPosition = maxValue;

  if (isSliderDown())
    {
    if (emitMinPosChanged || emitMaxPosChanged)
      {
      emit positionsChanged(m_MinimumPosition, m_MaximumPosition);
      }
    if (emitMinPosChanged)
      {
      emit minimumPositionChanged(m_MinimumPosition);
      }
    if (emitMaxPosChanged)
      {
      emit maximumPositionChanged(m_MaximumPosition);
      }
    }
  if (emitMinValChanged || emitMaxValChanged)
    {
    emit valuesChanged(m_MinimumValue, m_MaximumValue);
    }
  if (emitMinValChanged)
    {
    emit minimumValueChanged(m_MinimumValue);
    }
  if (emitMaxValChanged)
    {
    emit maximumValueChanged(m_MaximumValue);
    }
  if (emitMinPosChanged || emitMaxPosChanged ||
      emitMinValChanged || emitMaxValChanged)
    {
    this->update();
    }
}

// --------------------------------------------------------------------------
int RangeSlider::minimumPosition() const
{
  return m_MinimumPosition;
}

// --------------------------------------------------------------------------
int RangeSlider::maximumPosition() const
{
  return m_MaximumPosition;
}

// --------------------------------------------------------------------------
void RangeSlider::setMinimumPosition(int l)
{
  this->setPositions(l, qMax(l, m_MaximumPosition));
}

// --------------------------------------------------------------------------
void RangeSlider::setMaximumPosition(int u)
{
  this->setPositions(qMin(m_MinimumPosition, u), u);
}

// --------------------------------------------------------------------------
void RangeSlider::setPositions(int min, int max)
{
  const int minPosition =
    qBound(this->minimum(), qMin(min, max), this->maximum());
  const int maxPosition =
    qBound(this->minimum(), qMax(min, max), this->maximum());

  bool emitMinPosChanged = (minPosition != m_MinimumPosition);
  bool emitMaxPosChanged = (maxPosition != m_MaximumPosition);

  if (!emitMinPosChanged && !emitMaxPosChanged)
    {
    return;
    }

  m_MinimumPosition = minPosition;
  m_MaximumPosition = maxPosition;

  if (!this->hasTracking())
    {
    this->update();
    }
  if (isSliderDown())
    {
    if (emitMinPosChanged)
      {
      emit minimumPositionChanged(m_MinimumPosition);
      }
    if (emitMaxPosChanged)
      {
      emit maximumPositionChanged(m_MaximumPosition);
      }
    if (emitMinPosChanged || emitMaxPosChanged)
      {
      emit positionsChanged(m_MinimumPosition, m_MaximumPosition);
      }
    }
  if (this->hasTracking())
    {
    this->triggerAction(SliderMove);
    this->setValues(m_MinimumPosition, m_MaximumPosition);
    }
}

// --------------------------------------------------------------------------
void RangeSlider::setSymmetricMoves(bool symmetry)
{
  m_SymmetricMoves = symmetry;
}

// --------------------------------------------------------------------------
bool RangeSlider::symmetricMoves()const
{
  return m_SymmetricMoves;
}

// --------------------------------------------------------------------------
void RangeSlider::onRangeChanged(int _minimum, int _maximum)
{
  Q_UNUSED(_minimum);
  Q_UNUSED(_maximum);
  this->setValues(m_MinimumValue, m_MaximumValue);
}

// --------------------------------------------------------------------------
// Render
void RangeSlider::paintEvent( QPaintEvent* )
{
  QStyleOptionSlider option;
  this->initStyleOption(&option);

  QStylePainter painter(this);
  option.subControls = QStyle::SC_SliderGroove;
  // Move to minimum to not highlight the SliderGroove.
  // On mac style, drawing just the slider groove also draws the handles,
  // therefore we give a negative (outside of view) position.
  option.sliderValue = this->minimum() - this->maximum();
  option.sliderPosition = this->minimum() - this->maximum();
  painter.drawComplexControl(QStyle::CC_Slider, option);

  option.sliderPosition = m_MinimumPosition;
  const QRect lr = style()->subControlRect( QStyle::CC_Slider,
                                            &option,
                                            QStyle::SC_SliderHandle,
                                            this);
  option.sliderPosition = m_MaximumPosition;

  const QRect ur = style()->subControlRect( QStyle::CC_Slider,
                                            &option,
                                            QStyle::SC_SliderHandle,
                                            this);

  QRect sr = style()->subControlRect( QStyle::CC_Slider,
                                      &option,
                                      QStyle::SC_SliderGroove,
                                      this);
  QRect rangeBox;
  if (option.orientation == Qt::Horizontal)
    {
    rangeBox = QRect(
      QPoint(qMin( lr.center().x(), ur.center().x() ), sr.center().y() - 2),
      QPoint(qMax( lr.center().x(), ur.center().x() ), sr.center().y() + 1));
    }
  else
    {
    rangeBox = QRect(
      QPoint(sr.center().x() - 2, qMin( lr.center().y(), ur.center().y() )),
      QPoint(sr.center().x() + 1, qMax( lr.center().y(), ur.center().y() )));
    }

  // -----------------------------
  // Render the range
  //
  QRect groove = this->style()->subControlRect( QStyle::CC_Slider,
                                                &option,
                                                QStyle::SC_SliderGroove,
                                                this );
  groove.adjust(0, 0, -1, 0);

  // Create default colors based on the transfer function.
  //
  QColor highlight = this->palette().color(QPalette::Normal, QPalette::Highlight);
  QLinearGradient gradient;
  if (option.orientation == Qt::Horizontal)
    {
    gradient = QLinearGradient( groove.center().x(), groove.top(),
                                groove.center().x(), groove.bottom());
    }
  else
    {
    gradient = QLinearGradient( groove.left(), groove.center().y(),
                                groove.right(), groove.center().y());
    }

  // TODO: Set this based on the supplied transfer function
  //QColor l = Qt::darkGray;
  //QColor u = Qt::black;

  gradient.setColorAt(0, highlight.darker(120));
  gradient.setColorAt(1, highlight.lighter(160));

  painter.setPen(QPen(highlight.darker(150), 0));
  painter.setBrush(gradient);
  painter.drawRect( rangeBox.intersected(groove) );

  //  -----------------------------------
  // Render the sliders
  //
  if (this->isMinimumSliderDown())
    {
    drawMaximumSlider( &painter );
    drawMinimumSlider( &painter );
    }
  else
    {
    drawMinimumSlider( &painter );
    drawMaximumSlider( &painter );
    }
}

// --------------------------------------------------------------------------
// Standard Qt UI events
void RangeSlider::mousePressEvent(QMouseEvent* mouseEvent)
{
  if (minimum() == maximum() || (mouseEvent->buttons() ^ mouseEvent->button()))
    {
    mouseEvent->ignore();
    return;
    }
  int mepos = this->orientation() == Qt::Horizontal ?
    mouseEvent->pos().x() : mouseEvent->pos().y();

  QStyleOptionSlider option;
  this->initStyleOption( &option );

  QRect handleRect;
  Handle handle_ = handleAtPos(mouseEvent->pos(), handleRect);

  if (handle_ != NoHandle)
    {
    m_SubclassPosition = (handle_ == MinimumHandle)?
      m_MinimumPosition : m_MaximumPosition;

    // save the position of the mouse inside the handle for later
    m_SubclassClickOffset = mepos - (this->orientation() == Qt::Horizontal ?
      handleRect.left() : handleRect.top());

    this->setSliderDown(true);

    if (m_SelectedHandles != handle_)
      {
      m_SelectedHandles = handle_;
      this->update(handleRect);
      }
    // Accept the mouseEvent
    mouseEvent->accept();
    return;
    }

  // if we are here, no handles have been pressed
  // Check if we pressed on the groove between the 2 handles

  QStyle::SubControl control = this->style()->hitTestComplexControl(
    QStyle::CC_Slider, &option, mouseEvent->pos(), this);
  QRect sr = style()->subControlRect(
    QStyle::CC_Slider, &option, QStyle::SC_SliderGroove, this);
  int minCenter = (this->orientation() == Qt::Horizontal ?
    handleRect.left() : handleRect.top());
  int maxCenter = (this->orientation() == Qt::Horizontal ?
    handleRect.right() : handleRect.bottom());
  if (control == QStyle::SC_SliderGroove &&
      mepos > minCenter && mepos < maxCenter)
    {
    // warning lost of precision it might be fatal
    m_SubclassPosition = (m_MinimumPosition + m_MaximumPosition) / 2.;
    m_SubclassClickOffset = mepos - pixelPosFromRangeValue(m_SubclassPosition);
    m_SubclassWidth = (m_MaximumPosition - m_MinimumPosition) / 2;
    qMax(m_SubclassPosition - m_MinimumPosition, m_MaximumPosition - m_SubclassPosition);
    this->setSliderDown(true);
    if (!this->isMinimumSliderDown() || !this->isMaximumSliderDown())
      {
      m_SelectedHandles =
        QFlags<Handle>(MinimumHandle) |
        QFlags<Handle>(MaximumHandle);
      this->update(handleRect.united(sr));
      }
    mouseEvent->accept();
    return;
    }
  mouseEvent->ignore();
}

// --------------------------------------------------------------------------
// Standard Qt UI events
void RangeSlider::mouseMoveEvent(QMouseEvent* mouseEvent)
{
  if (!m_SelectedHandles)
    {
    mouseEvent->ignore();
    return;
    }
  int mepos = this->orientation() == Qt::Horizontal ?
    mouseEvent->pos().x() : mouseEvent->pos().y();

  QStyleOptionSlider option;
  this->initStyleOption(&option);

  const int m = style()->pixelMetric( QStyle::PM_MaximumDragDistance, &option, this );

  int newPosition = pixelPosToRangeValue(mepos - m_SubclassClickOffset);

  if (m >= 0)
    {
    const QRect r = rect().adjusted(-m, -m, m, m);
    if (!r.contains(mouseEvent->pos()))
      {
      newPosition = m_SubclassPosition;
      }
    }

  // Only the lower/left slider is down
  if (this->isMinimumSliderDown() && !this->isMaximumSliderDown())
    {
    double newMinPos = qMin(newPosition,m_MaximumPosition);
    this->setPositions(newMinPos, m_MaximumPosition +
      (m_SymmetricMoves ? m_MinimumPosition - newMinPos : 0));
    }
  // Only the upper/right slider is down
  else if (this->isMaximumSliderDown() && !this->isMinimumSliderDown())
    {
    double newMaxPos = qMax(m_MinimumPosition, newPosition);
    this->setPositions(m_MinimumPosition -
      (m_SymmetricMoves ? newMaxPos - m_MaximumPosition: 0),
      newMaxPos);
    }
  // Both handles are down (the user clicked in between the handles)
  else if (this->isMinimumSliderDown() && this->isMaximumSliderDown())
    {
    this->setPositions(newPosition - m_SubclassWidth,
                       newPosition + m_SubclassWidth );
    }
  mouseEvent->accept();
}

// --------------------------------------------------------------------------
// Standard Qt UI mouseEvents
void RangeSlider::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
  this->QSlider::mouseReleaseEvent(mouseEvent);

  setSliderDown(false);
  m_SelectedHandles = NoHandle;

  this->update();
}

// --------------------------------------------------------------------------
bool RangeSlider::isMinimumSliderDown()const
{
  return m_SelectedHandles & MinimumHandle;
}

// --------------------------------------------------------------------------
bool RangeSlider::isMaximumSliderDown()const
{
  return m_SelectedHandles & MaximumHandle;
}

// --------------------------------------------------------------------------
void RangeSlider::initMinimumSliderStyleOption(QStyleOptionSlider* option) const
{
  this->initStyleOption(option);
}

// --------------------------------------------------------------------------
void RangeSlider::initMaximumSliderStyleOption(QStyleOptionSlider* option) const
{
  this->initStyleOption(option);
}

// --------------------------------------------------------------------------
QString RangeSlider::handleToolTip()const
{
  return m_HandleToolTip;
}

// --------------------------------------------------------------------------
void RangeSlider::setHandleToolTip(const QString& _toolTip)
{
  m_HandleToolTip = _toolTip;
}

// --------------------------------------------------------------------------
bool RangeSlider::event(QEvent* _event)
{
  switch(_event->type())
    {
    case QEvent::ToolTip:
      {
      QHelpEvent* helpEvent = static_cast<QHelpEvent*>(_event);
      QStyleOptionSlider opt;
      // Test the MinimumHandle
      opt.sliderPosition = m_MinimumPosition;
      opt.sliderValue = m_MinimumValue;
      this->initStyleOption(&opt);
      QStyle::SubControl hoveredControl =
        this->style()->hitTestComplexControl(
          QStyle::CC_Slider, &opt, helpEvent->pos(), this);
      if (!m_HandleToolTip.isEmpty() &&
          hoveredControl == QStyle::SC_SliderHandle)
        {
        QToolTip::showText(helpEvent->globalPos(), m_HandleToolTip.arg(this->minimumValue()));
        _event->accept();
        return true;
        }
      // Test the MaximumHandle
      opt.sliderPosition = m_MaximumPosition;
      opt.sliderValue = m_MaximumValue;
      this->initStyleOption(&opt);
      hoveredControl = this->style()->hitTestComplexControl(
        QStyle::CC_Slider, &opt, helpEvent->pos(), this);
      if (!m_HandleToolTip.isEmpty() &&
          hoveredControl == QStyle::SC_SliderHandle)
        {
        QToolTip::showText(helpEvent->globalPos(), m_HandleToolTip.arg(this->maximumValue()));
        _event->accept();
        return true;
        }
      }
      break;
    default:
      break;
    }
  return this->Superclass::event(_event);
}

void RangeSlider::init()
{
    m_MinimumValue = 0;
    m_MaximumValue = 100;
    m_MinimumPosition = 0;
    m_MaximumPosition = 100;
    m_MinimumSliderSelected = QStyle::SC_None;
    m_MaximumSliderSelected = QStyle::SC_None;
    m_SubclassClickOffset = 0;
    m_SubclassPosition = 0;
    m_SubclassWidth = 0;
    m_SelectedHandles = NoHandle;
    m_SymmetricMoves = false;
    QObject::connect(this, SIGNAL(rangeChanged(int, int)), this, SLOT(onRangeChanged(int, int)));
}

RangeSlider::Handle RangeSlider::handleAtPos(const QPoint& pos, QRect& handleRect) const
{
    QStyleOptionSlider option;
    initStyleOption(&option);

    // The functinos hitTestComplexControl only know about 1 handle. As we have
    // 2, we change the position of the handle and test if the pos correspond to
    // any of the 2 positions.

    // Test the MinimumHandle
    option.sliderPosition = m_MinimumPosition;
    option.sliderValue = m_MinimumValue;

    QStyle::SubControl minimumControl = style()->hitTestComplexControl(QStyle::CC_Slider, &option, pos, this);
    QRect minimumHandleRect = style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, this);

    // Test if the pos is under the Maximum handle
    option.sliderPosition = m_MaximumPosition;
    option.sliderValue = m_MaximumValue;

    QStyle::SubControl maximumControl = style()->hitTestComplexControl(QStyle::CC_Slider, &option, pos, this);
    QRect maximumHandleRect = style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, this);

    // The pos is above both handles, select the closest handle
    if (minimumControl == QStyle::SC_SliderHandle &&
        maximumControl == QStyle::SC_SliderHandle)
    {
        int minDist = 0;
        int maxDist = 0;
        if (orientation() == Qt::Horizontal)
        {
            minDist = pos.x() - minimumHandleRect.left();
            maxDist = maximumHandleRect.right() - pos.x();
        }
        else //if (q->orientation() == Qt::Vertical)
        {
            minDist = minimumHandleRect.bottom() - pos.y();
            maxDist = pos.y() - maximumHandleRect.top();
        }
        Q_ASSERT(minDist >= 0 && maxDist >= 0);
        minimumControl = minDist < maxDist ? minimumControl : QStyle::SC_None;
    }

    if (minimumControl == QStyle::SC_SliderHandle)
    {
        handleRect = minimumHandleRect;
        return MinimumHandle;
    }
    else if (maximumControl == QStyle::SC_SliderHandle)
    {
        handleRect = maximumHandleRect;
        return MaximumHandle;
    }
    handleRect = minimumHandleRect.united(maximumHandleRect);
    return NoHandle;
}

// --------------------------------------------------------------------------
// Copied verbatim from QSliderPrivate::pixelPosToRangeValue. See QSlider.cpp
//
int RangeSlider::pixelPosToRangeValue(int pos) const
{
    QStyleOptionSlider option;
    initStyleOption(&option);

    QRect gr = style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderGroove, this);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, this);
    int sliderMin, sliderMax, sliderLength;
    if (option.orientation == Qt::Horizontal)
    {
        sliderLength = sr.width();
        sliderMin = gr.x();
        sliderMax = gr.right() - sliderLength + 1;
    }
    else
    {
        sliderLength = sr.height();
        sliderMin = gr.y();
        sliderMax = gr.bottom() - sliderLength + 1;
    }

    return QStyle::sliderValueFromPosition(minimum(),
        maximum(),
        pos - sliderMin,
        sliderMax - sliderMin,
        option.upsideDown);
}

int RangeSlider::pixelPosFromRangeValue(int val) const
{
    QStyleOptionSlider option;
    initStyleOption(&option);

    QRect gr = style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderGroove, this);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, this);
    int sliderMin, sliderMax, sliderLength;
    if (option.orientation == Qt::Horizontal)
    {
        sliderLength = sr.width();
        sliderMin = gr.x();
        sliderMax = gr.right() - sliderLength + 1;
    }
    else
    {
        sliderLength = sr.height();
        sliderMin = gr.y();
        sliderMax = gr.bottom() - sliderLength + 1;
    }

    return QStyle::sliderPositionFromValue(minimum(),
        maximum(),
        val,
        sliderMax - sliderMin,
        option.upsideDown) + sliderMin;
}

//---------------------------------------------------------------------------
// Draw slider at the bottom end of the range
void RangeSlider::drawMinimumSlider(QStylePainter* painter) const
{
    QStyleOptionSlider option;
    initMinimumSliderStyleOption(&option);

    option.subControls = QStyle::SC_SliderHandle;
    option.sliderValue = m_MinimumValue;
    option.sliderPosition = m_MinimumPosition;
    if (isMinimumSliderDown())
    {
        option.activeSubControls = QStyle::SC_SliderHandle;
        option.state |= QStyle::State_Sunken;
    }
#ifdef Q_OS_MAC
    // On mac style, drawing just the handle actually draws also the groove.
    QRect clip = style()->subControlRect(QStyle::CC_Slider, &option,
        QStyle::SC_SliderHandle, q);
    painter->setClipRect(clip);
#endif
    painter->drawComplexControl(QStyle::CC_Slider, option);
}

//---------------------------------------------------------------------------
// Draw slider at the top end of the range
void RangeSlider::drawMaximumSlider(QStylePainter* painter) const
{
    QStyleOptionSlider option;
    initMaximumSliderStyleOption(&option);

    option.subControls = QStyle::SC_SliderHandle;
    option.sliderValue = m_MaximumValue;
    option.sliderPosition = m_MaximumPosition;
    if (isMaximumSliderDown())
    {
        option.activeSubControls = QStyle::SC_SliderHandle;
        option.state |= QStyle::State_Sunken;
    }
#ifdef Q_OS_MAC
    // On mac style, drawing just the handle actually draws also the groove.
    QRect clip = style()->subControlRect(QStyle::CC_Slider, &option,
        QStyle::SC_SliderHandle, q);
    painter->setClipRect(clip);
#endif
    painter->drawComplexControl(QStyle::CC_Slider, option);
}

