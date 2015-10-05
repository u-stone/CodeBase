#include "aced.h"
#include "rxregsvc.h"
#include "dbapserv.h"
#include "dbsymtb.h"
#include "dbpl.h"
#include "gepnt2d.h"
#include "dbents.h"


void initApp();
void unloadApp();
void grid();

void initApp(){
	acedRegCmds->addCommand(L"liusuoyang", L"lsyGrid", L"LSYGRID", ACRX_CMD_MODAL, grid);
}

void unloadApp(){
	acedRegCmds->removeGroup(L"liusuoyang");
}

typedef struct _Grid
{
#define SIZE 128

	ads_point   orig;                        //原点
	int         grid_width;                  //方格网的宽度
	int         grid_height;                 //方格网的高度
	//int         line_width;                  //方格网线的宽度
	//int         line_type;                   //方格网线的线型
	//int         line_color;                  //方格网线的线色
	int         line_x_interval;             //方格网相邻竖线之间的间距
	int       	line_y_interval;             //方格网相邻水平线之间的间距
	//ACHAR       line_layer_name[SIZE];       //先所在的图层的图层名 
	int       	mark_x_start_index;          //x方向上第一个标注的序号
	int       	mark_x_interval;             //x方向上标注每多少条线有一个标注
	int       	mark_x_min;                  //x方向上标注最小值
	int       	mark_y_start_index;          //y方向上开始标注的索引号 
	int       	mark_y_interval;             //y方向上每多少条线有一个标注
	int       	mark_y_min;                  //y方向上标注最小值
	int       	makr_x_line_end_interval;    //x方向上线端到标注之间的距离
	int       	makr_y_line_end_interval;    //y方向上线端到标注之间的距离
	//ACHAR     	mark_font_name[SIZE];        //标注字体名
	ads_real    mark_font_height;            //标注字体高度
	int       	makrt_font_color;            //标注字体颜色
	//ACHAR     	mark_font_layer_name[SIZE];  //标注所在图层的图层名
	int       	mark_type;                   //标注为世界坐标（X=%d, Y=%d）还是相对坐标（A=%d, B=%d）

#undef SIZE
}GRID;

void initGrid(GRID* grid){
	int ret = acedGetPoint(0, L"方格网原点", grid->orig);
	
	ads_real tmp;
	acedGetReal(L"水平线的长度:", &tmp);
	grid->grid_width = (int)tmp;
	acedGetReal(L"竖线的长度:", &tmp);
	grid->grid_height = (int)tmp;

	acedGetInt(L"相邻竖线之间的距离:", &(grid->line_x_interval));
	acedGetInt(L"相邻水平线的之间的距离:", &(grid->line_y_interval));


	acedGetInt(L"从第几条Y方向线开始标注:[1, 32767]", &(grid->mark_x_start_index));
	acedGetInt(L"每多少条Y方向线作一个标注:[1, 32767]", &(grid->mark_x_interval));
	acedGetInt(L"水平线的端点与对应标注之间的距离:", &(grid->makr_x_line_end_interval));
	acedGetReal(L"水平方向上最小标注的值:", &tmp);
	grid->mark_x_min = (int)tmp;

	acedGetInt(L"从第几条X方向线开始标注:[1, 32767]", &(grid->mark_y_start_index));
	acedGetInt(L"每多少条X方向线作一个标注:[1, 32767]", &(grid->mark_y_interval));
	acedGetInt(L"竖线的端点与对应标注之间的距离:", &(grid->makr_y_line_end_interval));
	acedGetReal(L"竖直方向上最小标注的值:", &tmp);
	grid->mark_y_min = (int)tmp;

	grid->mark_font_height = 1.5;
	do {
		acedGetReal(L"标注字体高度:", &(grid->mark_font_height));
		grid->mark_font_height = abs(grid->mark_font_height);
		if (grid->mark_font_height > 0.0 && grid->mark_font_height <= 150.0)
			break;
	} while (1);
	
	do {
		grid->mark_type = 1;
		acedGetInt(L"标注为：绝对坐标 1(X, Y)，还是相对坐标 2(A, B):", &(grid->mark_type));
		if ((grid->mark_type == 1) || (grid->mark_type == 2))
			break;
	} while (1);
}

void debug_print_grid(GRID* grid){
	//acutPrintf(L"原点: (%f, %f, %f)\n", grid->orig[0], grid->orig[1], grid->orig[2]);
	//acutPrintf(L"方格网宽度 %d\n", grid->grid_width);
	//acutPrintf(L"方格网高度 %d\n", grid->grid_height);
	//acutPrintf(L"网线的宽度 %d\n", grid->line_width);
	//acutPrintf(L"方格网线的线型 %d\n", grid->line_type);
	//acutPrintf(L"方格网线的线色 %d\n", grid->line_color);
	//acutPrintf(L"水平方向上的间隔 %d\n", grid->line_x_interval);
	//acutPrintf(L"竖直方向上的间隔 %d\n", grid->line_y_interval);
	//acutPrintf(L"所在的图层的图层名 %s\n", grid->line_layer_name);
	//acutPrintf(L"x方向上开始标注的索引号 %d\n", grid->mark_x_start_index);
	//acutPrintf(L"x方向上标注每多少条线有一个标注 %d\n", grid->mark_x_interval);
	//acutPrintf(L"y方向上开始标注的索引号 %d\n", grid->mark_y_start_index);
	//acutPrintf(L"y方向上每多少条线有一个标注 %d\n", grid->mark_y_interval);
	//acutPrintf(L"x方向上线端到标注之间的距离 %d\n", grid->makr_x_line_end_interval);
	//acutPrintf(L"y方向上线端到标注之间的距离 %d\n", grid->makr_y_line_end_interval);
	//acutPrintf(L"标注字体名 %s\n", grid->mark_font_name);
	//acutPrintf(L"标注字体高度 %d\n", grid->mark_font_height);
	//acutPrintf(L"标注字体颜色 %d\n", grid->makrt_font_color);
	//acutPrintf(L"标注所在图层的图层名 %s\n", grid->mark_font_layer_name);
	//acutPrintf(L"标注类型为 %d\n", grid->mark_type);
}

bool createLayerByName(ACHAR* name){
	AcDbLayerTable *pLayerTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getSymbolTable(pLayerTbl, AcDb::kForWrite);
	ACHAR buf[128] = {0};
	acutSPrintf(buf, L"%s", name);
	if (!pLayerTbl->has(buf)) {
		AcDbLayerTableRecord *pLayerTblRcd
			= new AcDbLayerTableRecord;
		pLayerTblRcd->setName(buf);
		pLayerTblRcd->setIsFrozen(0);// layer to THAWED
		pLayerTblRcd->setIsOff(0);   // layer to ON
		pLayerTblRcd->setVPDFLT(0);  // viewport default
		pLayerTblRcd->setIsLocked(0);// un-locked
		AcCmColor color;
		color.setColorIndex(1); // set color to red
		pLayerTblRcd->setColor(color);
		// For linetype, we need to provide the object ID of
		// the linetype record for the linetype we want to
		// use. First, we need to get the object ID.
		//
		AcDbLinetypeTable *pLinetypeTbl;
		AcDbObjectId ltId;
		acdbHostApplicationServices()->workingDatabase()
			->getSymbolTable(pLinetypeTbl, AcDb::kForRead);
		if ((pLinetypeTbl->getAt(L"DASHED", ltId))
			!= Acad::eOk)
		{
			acutPrintf(L"\nUnable to find DASHED"
				L" linetype. Using CONTINUOUS");
			// CONTINUOUS is in every drawing, so use it.
			//
			pLinetypeTbl->getAt(L"CONTINUOUS", ltId);
		}
		pLinetypeTbl->close();
		pLayerTblRcd->setLinetypeObjectId(ltId);
		pLayerTbl->add(pLayerTblRcd);
		pLayerTblRcd->close();
		pLayerTbl->close();
	} else {
		pLayerTbl->close();
		acutPrintf(L"\n图层已存在");
	}
	return true;
}

bool findLayerByName2(ACHAR* name){
	AcDbLinetypeTable *pLinetypeTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getSymbolTable(pLinetypeTbl, AcDb::kForRead);
	// Create a new iterator that starts at table
	// beginning and skips deleted.
	//
	AcDbLinetypeTableIterator *pLtIterator;
	pLinetypeTbl->newIterator(pLtIterator);
	// Walk the table, getting every table record and
	// printing the linetype name.
	//
	AcDbLinetypeTableRecord *pLtTableRcd;
	ACHAR *pLtName;
	for (; !pLtIterator->done(); pLtIterator->step()) {
		pLtIterator->getRecord(pLtTableRcd, AcDb::kForRead);
		pLtTableRcd->getName(pLtName);
		pLtTableRcd->close();
		acutPrintf(L"\nLinetype name is:  %s", pLtName);
		free(pLtName);
	}
	delete pLtIterator;
	pLinetypeTbl->close();
	return true;
}


bool findTextStyleTable(){
	AcDbTextStyleTable *pTextStyleTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getSymbolTable(pTextStyleTbl, AcDb::kForRead);
	// Create a new iterator that starts at table
	// beginning and skips deleted.
	//
	AcDbTextStyleTableIterator *pLtIterator;
	pTextStyleTbl->newIterator(pLtIterator);
	// Walk the table, getting every table record and
	// printing the linetype name.
	//
	AcDbTextStyleTableRecord *pLtTableRcd;
	ACHAR *pLtName;
	for (; !pLtIterator->done(); pLtIterator->step()) {
		pLtIterator->getRecord(pLtTableRcd, AcDb::kForRead);
		pLtTableRcd->getName(pLtName);
		pLtTableRcd->close();
		acutPrintf(L"\nLinetype name is:  %s", pLtName);
		free(pLtName);
	}
	delete pLtIterator;
	pTextStyleTbl->close();
	return true;
}

void createTextStyle(){
	//Standard
	//Annotative
	
}


void createPLine(ACHAR* layerName, GRID* grid){
	AcDbBlockTable *pBlockTable;
	acdbHostApplicationServices()->workingDatabase()
		->getSymbolTable(pBlockTable, AcDb::kForWrite);
	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,
		AcDb::kForWrite);
	pBlockTable->close();
	AcDbObjectId lineId;
	
	int line_x_n = grid->grid_width / grid->line_x_interval;
	int line_y_n = grid->grid_height / grid->line_y_interval;
	if (line_x_n < 1 || line_y_n < 1)
		return ;

	AcGePoint2d ptStart, ptEnd;
	for (int i = 1; i < line_y_n; ++i){
		ptStart.x = grid->orig[0];
		ptEnd.x   = ptStart.x + grid->grid_width;
		ptEnd.y   = ptStart.y = grid->orig[1] + i * grid->line_y_interval;

		AcDbPolyline *pLine = new AcDbPolyline();
		pLine->addVertexAt(0, ptStart);
		pLine->addVertexAt(1, ptEnd);
		pBlockTableRecord->appendAcDbEntity(lineId, pLine);
		pLine->setLayer(layerName);
		pLine->close();
	}

	for (int i = 1; i < line_x_n; ++i){
		ptEnd.x   = ptStart.x = grid->orig[0] + i * grid->line_x_interval;
		ptStart.y = grid->orig[1];
		ptEnd.y   = grid->orig[1] + grid->grid_height;

		AcDbPolyline *pLine = new AcDbPolyline();
		pLine->addVertexAt(0, ptStart);
		pLine->addVertexAt(1, ptEnd);
		pBlockTableRecord->appendAcDbEntity(lineId, pLine);
		pLine->setLayer(layerName);
		pLine->close();
	}

	pBlockTableRecord->close();
}

void createMark(ACHAR* markLayer, GRID* grid){
	AcDbBlockTable *pBlockTable;
	acdbHostApplicationServices()->workingDatabase()
		->getSymbolTable(pBlockTable, AcDb::kForWrite);
	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,
		AcDb::kForWrite);
	pBlockTable->close();
	AcDbObjectId textId;

	int line_x_n = grid->grid_width / grid->line_x_interval;
	int line_y_n = grid->grid_height / grid->line_y_interval;
	if (line_x_n < 1 || line_y_n < 1)
		return ;

	ACHAR buf[20] = {0};
	AcGePoint3d pt;
	pt.z = 0;

	pt.x = grid->orig[0] + grid->grid_width + grid->makr_x_line_end_interval;
	for (int i = grid->mark_y_start_index - 1; i <= line_y_n; i += grid->mark_y_interval){
		int v = grid->mark_y_min + i * grid->line_y_interval;
		if (grid->mark_type == 1)
			acutSPrintf(buf, L"A=%d", v);
		else
			acutSPrintf(buf, L"X=%d", v);

		acutPrintf(buf);

		pt.y = grid->orig[1] + i * grid->line_y_interval;

		AcDbText *pText = new AcDbText();
		pText->setHorizontalMode(AcDb::kTextLeft);
		pText->setVerticalMode(AcDb::kTextVertMid);
		pText->setTextString(buf);
		pText->setAlignmentPoint(pt);
		//pText->setPosition(pt);
		pText->setHeight(grid->mark_font_height);
		pBlockTableRecord->appendAcDbEntity(textId, pText);
		pText->setLayer(markLayer);
		pText->close();
	}
	pt.x = grid->orig[0] - grid->makr_x_line_end_interval;
	for (int i = grid->mark_y_start_index - 1; i <= line_y_n; i += grid->mark_y_interval){
		int v = grid->mark_y_min + i * grid->line_y_interval;
		if (grid->mark_type == 1)
			acutSPrintf(buf, L"A=%d", v);
		else
			acutSPrintf(buf, L"X=%d", v);

		acutPrintf(buf);

		pt.y = grid->orig[1] + i * grid->line_y_interval;

		AcDbText *pText = new AcDbText();
		pText->setHorizontalMode(AcDb::kTextRight);
		pText->setVerticalMode(AcDb::kTextVertMid);
		pText->setTextString(buf);
		pText->setAlignmentPoint(pt);
		//pText->setPosition(pt);
		pText->setHeight(grid->mark_font_height);
		pBlockTableRecord->appendAcDbEntity(textId, pText);
		pText->setLayer(markLayer);
		pText->close();
	}

	pt.y = grid->orig[1] + grid->grid_height + grid->makr_y_line_end_interval;
	for (int i = grid->mark_x_start_index - 1; i <= line_x_n; i += grid->mark_x_interval){
		AcDbText *pText = new AcDbText();
		int v = grid->mark_x_min + i * grid->line_x_interval;
		if (grid->mark_type == 1)
			acutSPrintf(buf, L"B=%d", v);
		else
			acutSPrintf(buf, L"Y=%d", v);

		acutPrintf(buf);


		pt.x = grid->orig[0] + i * grid->line_x_interval;

		pText->setHorizontalMode(AcDb::kTextCenter);
		pText->setVerticalMode(AcDb::kTextBottom);
		pText->setTextString(buf);
		pText->setAlignmentPoint(pt);
		pText->setHeight(grid->mark_font_height);
		//pText->setPosition(pt);
		pBlockTableRecord->appendAcDbEntity(textId, pText);
		pText->setLayer(markLayer);
		pText->close();
	}

	pt.y = grid->orig[1] - grid->makr_y_line_end_interval;
	for (int i = grid->mark_x_start_index - 1; i <= line_x_n; i += grid->mark_x_interval){
		AcDbText *pText = new AcDbText();
		int v = grid->mark_x_min + i * grid->line_x_interval;
		if (grid->mark_type == 1)
			acutSPrintf(buf, L"B=%d", v);
		else
			acutSPrintf(buf, L"Y=%d", v);

		acutPrintf(buf);

		pt.x = grid->orig[0] + i * grid->line_x_interval;

		pText->setHorizontalMode(AcDb::kTextCenter);
		pText->setVerticalMode(AcDb::kTextTop);
		pText->setTextString(buf);
		pText->setAlignmentPoint(pt);
		pText->setHeight(grid->mark_font_height);
		//pText->setPosition(pt);
		pBlockTableRecord->appendAcDbEntity(textId, pText);
		pText->setLayer(markLayer);
		pText->close();
	}

	pBlockTableRecord->close();
}

void addmark(){
	acutPrintf(L"123");

	AcDbBlockTable *pBlockTable;
	acdbHostApplicationServices()->workingDatabase()
		->getSymbolTable(pBlockTable, AcDb::kForWrite);
	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,
		AcDb::kForWrite);
	pBlockTable->close();
	AcDbObjectId textId;
	acutPrintf(L"123--4");

	ACHAR buf[20] = {0};
	AcDbText *pText = new AcDbText();
	int v = 20;
	if (1)
		acutSPrintf(buf, L"B=%d", v);
	else
		acutSPrintf(buf, L"Y=%d", v);
	acutPrintf(L"123 -- 7");

	acutPrintf(buf);

	AcGePoint3d pt;
	pt.x = 100;//grid->orig[0] + i * grid->line_x_interval;
	pt.y = 200;

	pText->setHorizontalMode(AcDb::kTextCenter);
	pText->setVerticalMode(AcDb::kTextTop);
	pText->setTextString(buf);
	pText->setAlignmentPoint(pt);
	pText->setHeight(2.0);
	//pText->setPosition(pt);
	pBlockTableRecord->appendAcDbEntity(textId, pText);
	pText->setLayer(L"lsy_gridmark");
	pText->close();
	pBlockTableRecord->close();

	acutPrintf(L"123 -- 9");

}

void draw(GRID * grid){
	//检查网线图层和标注图层是否存在
	ACHAR lineLayer[] = L"lsy_gridline";
	ACHAR markLayer[] = L"lsy_gridmark";
	createLayerByName(lineLayer);
	createLayerByName(markLayer);
	createPLine(lineLayer, grid);
	createMark(markLayer, grid);
}

void grid(){
	//addmark();
	//findTextStyleTable();
	//return ;
	try {
		GRID grid;
		memset(&grid, 0, sizeof(grid));
		initGrid(&grid);
		debug_print_grid(&grid);
		draw(&grid);
	} catch (...) {
		acutPrintf(L"绘制方格网时出现错误...\n");
	}
	
	acutPrintf(L"\nHello, This is create by UStone\n");
}

extern "C"
	AcRx::AppRetCode acrxEntryPoint(AcRx::AppMsgCode msg, void *pkt){
		switch (msg)
		{
		case AcRx::kInitAppMsg:
			acrxDynamicLinker->unlockApplication(pkt);
			acrxRegisterAppMDIAware(pkt);
			initApp();
			break;

		case AcRx::kUnloadAppMsg:
			unloadApp();
			break;

		default:
			break;
		}
		return AcRx::kRetOK;
}

