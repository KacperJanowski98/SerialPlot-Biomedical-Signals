#ifndef FFTPLOTMANAGER_H
#define FFTPLOTMANAGER_H

#include <QObject>
#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QList>
#include <QSettings>
#include <QMenu>

#include <qwt_plot_curve.h>
#include "plot.h"
#include "framebufferseries.h"
#include "fftstream.h"
#include "snapshot.h"
#include "plotmenu.h"

class FftPlotManager : public QObject
{
    Q_OBJECT

public:
    explicit FftPlotManager(QWidget* plotArea, PlotMenu* menu,
                         const FftStream* stream = nullptr,
                         QObject *parent = 0);
    explicit FftPlotManager(QWidget* plotArea, PlotMenu* menu,
                         Snapshot* snapshot,
                         QObject *parent = 0);
    ~FftPlotManager();
    /// Add a new curve with title and buffer.
    void addCurve(QString title, const XFrameBuffer* xBuf, const FrameBuffer* yBuf);
    /// Removes curves from the end
    void removeCurves(unsigned number);
    /// Returns current number of curves known by plot manager
    unsigned numOfCurves();
    /// export SVG
    void exportSvg (QString fileName) const;

public slots:
    /// Enable/Disable multiple plot display
    void setMulti(bool enabled);
    /// Update all plot widgets
    void replot();
    /// Enable display of a "DEMO" label on each plot
    void showDemoIndicator(bool show = true);
    /// Set the Y axis
    void setYAxis(bool autoScaled, double yMin = 0, double yMax = 1);
    /// Set the X axis
    void setXAxis(bool asIndex, double xMin = 0 , double xMax = 1);
    /// Display an animation for snapshot
    void flashSnapshotOverlay();
    /// Should be called to update zoom base
    void setNumOfSamples(unsigned value);
    /// Maximum width of X axis (limit of hscroll)
    void setPlotWidth(double width);

private:
    bool isMulti;
    QWidget* _plotArea;
    PlotMenu* _menu;
    QVBoxLayout* layout; ///< layout of the `plotArea`
    QScrollArea* scrollArea;
    QList<QwtPlotCurve*> curves;
    QList<Plot*> plotWidgets;
    Plot* emptyPlot;  ///< for displaying when all channels are hidden
    const FftStream* _stream;       ///< attached stream, can be `nullptr`
    const ChannelInfoModel* infoModel;
    bool isDemoShown;
    bool _autoScaled;
    double _yMin;
    double _yMax;
    bool _xAxisAsIndex;
    double _xMin;
    double _xMax;
    unsigned _numOfSamples;
    double _plotWidth;
    Plot::ShowSymbols showSymbols;
    bool inScaleSync; ///< scaleSync is in progress

    /// Common constructor
    void construct(QWidget* plotArea, PlotMenu* menu);
    /// Setups the layout for multi or single plot
    void setupLayout(bool multiPlot);
    /// Inserts a new plot widget to the current layout.
    Plot* addPlotWidget();
    /// Returns the plot widget that given curve is attached to
    Plot* plotWidget(unsigned curveIndex);
    /// Common part of overloaded `addCurve` functions
    void _addCurve(QwtPlotCurve* curve);
    /// Check and make sure "no visible channels" text is shown
    void checkNoVisChannels();

private slots:
    void showGrid(bool show = true);
    void showMinorGrid(bool show = true);
    void showLegend(bool show = true);
    void setLegendPosition(Qt::AlignmentFlag alignment);
    void unzoom();
    void darkBackground(bool enabled = true);
    void setSymbols(Plot::ShowSymbols shown);

    void onNumChannelsChanged(unsigned value);
    void onChannelInfoChanged(const QModelIndex & topLeft,
                              const QModelIndex & bottomRight,
                              const QVector<int> & roles = QVector<int> ());

    /// Synchronize Y axes to be the same width (so that X axes are in line)
    void syncScales();
};

#endif // FFTPLOTMANAGER_H
