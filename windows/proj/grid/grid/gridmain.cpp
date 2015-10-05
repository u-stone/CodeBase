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

	ads_point   orig;                        //ԭ��
	int         grid_width;                  //�������Ŀ��
	int         grid_height;                 //�������ĸ߶�
	//int         line_width;                  //�������ߵĿ��
	//int         line_type;                   //�������ߵ�����
	//int         line_color;                  //�������ߵ���ɫ
	int         line_x_interval;             //��������������֮��ļ��
	int       	line_y_interval;             //����������ˮƽ��֮��ļ��
	//ACHAR       line_layer_name[SIZE];       //�����ڵ�ͼ���ͼ���� 
	int       	mark_x_start_index;          //x�����ϵ�һ����ע�����
	int       	mark_x_interval;             //x�����ϱ�עÿ����������һ����ע
	int       	mark_x_min;                  //x�����ϱ�ע��Сֵ
	int       	mark_y_start_index;          //y�����Ͽ�ʼ��ע�������� 
	int       	mark_y_interval;             //y������ÿ����������һ����ע
	int       	mark_y_min;                  //y�����ϱ�ע��Сֵ
	int       	makr_x_line_end_interval;    //x�������߶˵���ע֮��ľ���
	int       	makr_y_line_end_interval;    //y�������߶˵���ע֮��ľ���
	//ACHAR     	mark_font_name[SIZE];        //��ע������
	ads_real    mark_font_height;            //��ע����߶�
	int       	makrt_font_color;            //��ע������ɫ
	//ACHAR     	mark_font_layer_name[SIZE];  //��ע����ͼ���ͼ����
	int       	mark_type;                   //��עΪ�������꣨X=%d, Y=%d������������꣨A=%d, B=%d��

#undef SIZE
}GRID;

void initGrid(GRID* grid){
	int ret = acedGetPoint(0, L"������ԭ��", grid->orig);
	
	ads_real tmp;
	acedGetReal(L"ˮƽ�ߵĳ���:", &tmp);
	grid->grid_width = (int)tmp;
	acedGetReal(L"���ߵĳ���:", &tmp);
	grid->grid_height = (int)tmp;

	acedGetInt(L"��������֮��ľ���:", &(grid->line_x_interval));
	acedGetInt(L"����ˮƽ�ߵ�֮��ľ���:", &(grid->line_y_interval));


	acedGetInt(L"�ӵڼ���Y�����߿�ʼ��ע:[1, 32767]", &(grid->mark_x_start_index));
	acedGetInt(L"ÿ������Y��������һ����ע:[1, 32767]", &(grid->mark_x_interval));
	acedGetInt(L"ˮƽ�ߵĶ˵����Ӧ��ע֮��ľ���:", &(grid->makr_x_line_end_interval));
	acedGetReal(L"ˮƽ��������С��ע��ֵ:", &tmp);
	grid->mark_x_min = (int)tmp;

	acedGetInt(L"�ӵڼ���X�����߿�ʼ��ע:[1, 32767]", &(grid->mark_y_start_index));
	acedGetInt(L"ÿ������X��������һ����ע:[1, 32767]", &(grid->mark_y_interval));
	acedGetInt(L"���ߵĶ˵����Ӧ��ע֮��ľ���:", &(grid->makr_y_line_end_interval));
	acedGetReal(L"��ֱ��������С��ע��ֵ:", &tmp);
	grid->mark_y_min = (int)tmp;

	grid->mark_font_height = 1.5;
	do {
		acedGetReal(L"��ע����߶�:", &(grid->mark_font_height));
		grid->mark_font_height = abs(grid->mark_font_height);
		if (grid->mark_font_height > 0.0 && grid->mark_font_height <= 150.0)
			break;
	} while (1);
	
	do {
		grid->mark_type = 1;
		acedGetInt(L"��עΪ���������� 1(X, Y)������������� 2(A, B):", &(grid->mark_type));
		if ((grid->mark_type == 1) || (grid->mark_type == 2))
			break;
	} while (1);
}

void debug_print_grid(GRID* grid){
	//acutPrintf(L"ԭ��: (%f, %f, %f)\n", grid->orig[0], grid->orig[1], grid->orig[2]);
	//acutPrintf(L"��������� %d\n", grid->grid_width);
	//acutPrintf(L"�������߶� %d\n", grid->grid_height);
	//acutPrintf(L"���ߵĿ�� %d\n", grid->line_width);
	//acutPrintf(L"�������ߵ����� %d\n", grid->line_type);
	//acutPrintf(L"�������ߵ���ɫ %d\n", grid->line_color);
	//acutPrintf(L"ˮƽ�����ϵļ�� %d\n", grid->line_x_interval);
	//acutPrintf(L"��ֱ�����ϵļ�� %d\n", grid->line_y_interval);
	//acutPrintf(L"���ڵ�ͼ���ͼ���� %s\n", grid->line_layer_name);
	//acutPrintf(L"x�����Ͽ�ʼ��ע�������� %d\n", grid->mark_x_start_index);
	//acutPrintf(L"x�����ϱ�עÿ����������һ����ע %d\n", grid->mark_x_interval);
	//acutPrintf(L"y�����Ͽ�ʼ��ע�������� %d\n", grid->mark_y_start_index);
	//acutPrintf(L"y������ÿ����������һ����ע %d\n", grid->mark_y_interval);
	//acutPrintf(L"x�������߶˵���ע֮��ľ��� %d\n", grid->makr_x_line_end_interval);
	//acutPrintf(L"y�������߶˵���ע֮��ľ��� %d\n", grid->makr_y_line_end_interval);
	//acutPrintf(L"��ע������ %s\n", grid->mark_font_name);
	//acutPrintf(L"��ע����߶� %d\n", grid->mark_font_height);
	//acutPrintf(L"��ע������ɫ %d\n", grid->makrt_font_color);
	//acutPrintf(L"��ע����ͼ���ͼ���� %s\n", grid->mark_font_layer_name);
	//acutPrintf(L"��ע����Ϊ %d\n", grid->mark_type);
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
		acutPrintf(L"\nͼ���Ѵ���");
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
	//�������ͼ��ͱ�עͼ���Ƿ����
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
		acutPrintf(L"���Ʒ�����ʱ���ִ���...\n");
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

