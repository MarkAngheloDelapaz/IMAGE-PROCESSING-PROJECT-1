#ifndef RINSPECTPLANE_H
#define RINSPECTPLANE_H

#include <string>
#include <vector>

#include "R_plane.h"
#include "R_winb.h"


#ifdef QT_CORE_LIB
	#include <QGraphicsScene>
	#include <QThread>
#endif

using std::string;

#ifdef __GNUC__
using std::vector;
#endif

#ifdef QT_CORE_LIB
class RInspectPlain : public QGraphicsScene
#else
class RInspectPlain
#endif
{
public:
	static const int LAYER_GRAY;
	static const int LAYER_BIN;
	static const int LAYER_DRAW;

private:
	struct RqpItem{
		string	name;
		R_PLANE plane;
		int page;

#ifdef QT_CORE_LIB
		QImage image;
#endif

	};

public:
	RInspectPlain(int sizex, int sizey);

	~RInspectPlain();

	void SetGrayImage(string name, R_PLANE gray, int page);
	void SetBinImage(string name, R_PLANE bin, int page);
	void SetDrawPlane();
	unsigned char *GetGrayAddr(string key);
	unsigned char *GetGrayAddr(int index);
	unsigned char *GetBinAddr(string key);
	unsigned char *GetBinAddr(int index);
	string GetGrayName(int index);
	string GetBinName(int index);
	int GetGrayPage(int index);
	int GetBinPage(int index);
	R_PLANE GetGrayPlane(int index);
	R_PLANE GetBinPlane(int index);
	struct winb_t *GetGrayMenu();
	struct winb_t *GetBinMenu();

	int GetGrayCount();
	int GetBinCount();

	int FxSize();
	int FySize();

#ifdef QT_CORE_LIB
	RInspectPlain(QList<QPair<string, R_PLANE> > gray);


	void ShowGrayImage(int index);
	void ShowGrayImage(string key);
	void HideGrayImage();
	void ShowBinImage(int index);
	void ShowBinImage(string key);
	void HideBinImage();
	void ShowDrawImage();
	void HideDrawImage();

	void DrawClear();
	void DrawRect(R_RECT rect);
	void DrawPoint(R_POINT p);
	void DrawPoint(QPoint p);
	void DrawPoint(QPointF p);

	void SetGrayImage(QList<QPair<string, R_PLANE> > gray);

#endif

private:
	std::vector<RqpItem> _gray;
	std::vector<RqpItem> _bin;

	int _fxsize;
	int _fysize;
	struct winb_t *_graymenu;
	struct winb_t *_binmenu;
	char *_graytext;
	char *_bintext;


	int __find_Rqpindex(std::vector<RqpItem> _item, string key);
	void __getWinbMenu(std::vector<RqpItem> list, struct winb_t **menu, char *title, char **buf);

#ifdef QT_CORE_LIB
	RqpItem _draw;
	unsigned char *_draw_data;
	QGraphicsPixmapItem *_dispGray;
	QGraphicsPixmapItem *_dispBin;
	QGraphicsPixmapItem *_dispDraw;
	QPixmap _pxdraw;
	QGraphicsPixmapItem *_gpxdraw;
	Qt::HANDLE _iventloop;				///<	生成されたスレッドのID(GUIスレッド)
#endif
};


#endif // RINSPECTPLANE_H
