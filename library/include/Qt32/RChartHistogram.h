#ifndef RCHARTHISTOGRAM_H
#define RCHARTHISTOGRAM_H

#include <QWidget>
//#include <qwt_plot_curve.h>
#include <qwt_plot_histogram.h>
#include <qwt_plot_marker.h>

#include "R_chart.h"

namespace Ui {
class RChartHistogram;
}

class RChartHistogram : public QWidget
{
	Q_OBJECT

public:
	explicit RChartHistogram(QWidget* parent = 0, Qt::WindowFlags f = 0);
	~RChartHistogram();

private:
	Ui::RChartHistogram *ui;

public slots:
//	int setHistogram(const R_HISTOGRAM* data);
	int setHistogram(const void* data, const char* title);

private:
	const R_HISTOGRAM* m_data;
//	QwtPlotCurve* m_curve;
	QwtPlotHistogram* m_curve;
	QwtPlotMarker* m_markerAverage;
	QwtPlotMarker* m_markerUpperLimit;
	QwtPlotMarker* m_markerLowerLimit;
	QwtPlotMarker* m_markerUpperControl;
	QwtPlotMarker* m_markerLowerControl;
};

#endif // RCHARTHISTOGRAM_H
