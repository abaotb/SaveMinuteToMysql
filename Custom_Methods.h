#ifndef CUSTOM_METHODS_H
#define CUSTOM_METHODS_H

#include"OperatorData.h"
using namespace std;
extern CMdSpi_Custom * cmdSpi_custom;	
extern char* ppInstrumentID[];
#pragma once
/************************************************************************/
/*							����ʱ��ת������					       */
/************************************************************************/
//1����11:45:32��ð��ȥ�� 11:45:32->114532
int method_TimeToInt(char *tick_UpdateTime);
//2��ת��60���������ʱ���뻻������ӣ�����Ϊ114316,����������11*3600+43*60+16
extern int method_TimeToInt_60(int itime);
//3�����������ʱ���뻻�������ת���ɹ����ʱ���뻻������� 102460 -> 102500
int method_Template_TimeInt(int ntime);
//4����ʱ���뻻������� ����int method_TimeToInt_60(int itime)
int method_TimeToInt_60();
//5����104316����ʱ������������  ��ϵͳʱ�� ->104316��ʽ
int method_TimeToInt_Int();
//6���Ƿ�ͣ����Ϣʱ��
bool method_Rest_Time();
//7���������Ͼ��۵�һЩ����������
bool filter_Unreasonable_Price(int datetime);
/************************************************************************/
/*                          д���ļ���������                             */
/************************************************************************/
//д���ļ�
//д�յ���ԭʼ��tick���ݣ��ʹ�����tick����
void methodWriteFile(CThostFtdcDepthMarketDataField_Custom &data,string filename);
//д������5�����ݣ�1������
void methodWriteFile2(CThostFtdcDepthMarketDataField_Custom_Other &data,string filename);

//�����յ��ҵ�һ���޸ĵı�׼��Tick����ת�����Զ����Tick����---OperatorData.cpp�õ�
CThostFtdcDepthMarketDataField_Custom_Tick methodChangeData(CThostFtdcDepthMarketDataField_Custom &original_data);


/************************************************************************/
/*                          ��ӡ���ݲ���                                */
/************************************************************************/
void readdata1();   //tick quotation TESTtest222  CThostFtdcDepthMarketDataField_Custom
void readdata2();   //original    CThostFtdcDepthMarketDataField_Custom         fprintf(fw,"%s,%s,%d,%d,%.2f,%ld\r\n",tb.ExchangeID,tb.InstrumentID,tb.UpdateTime,tb.UpdateMillisec,tb.LastPrice,tb.ReceiveTime);
void readdata3();   //today-1min  CThostFtdcDepthMarketDataField_Custom_Other   fprintf(fw,"%s,%d,%.2f,%.2f,%.2f,%.2f\r\n",tb.InstrumentID,tb.UpdateTime,tb.OpenPrice,tb.LowestPrice,tb.HighestPrice,tb.ClosePrice);
//�Զ����ɺ�Լ���������,��C���Ե�strtok�ָ���Զ���������
void methodAutoContract();
//�Զ����ɺ�Լ�������������vector<string>split�ָ�������Զ���������
void methodAutoContract2();
//�Զ����ɺ�Լ�������������vector<string>split�ָ�����Զ���������
void methodAutoContract3();


/************************************************************************/
/*                           �������ݿ�                                  */
/************************************************************************/
//����mysql���ݿ�
void Ado_Connect_Mysql(CThostFtdcDepthMarketDataField_Custom_Other &ctd_data);

bool  priceRange(CThostFtdcDepthMarketDataField_Custom_Other &current_data,CThostFtdcDepthMarketDataField_Custom_Tick &custom_tick);
#endif CUSTOM_METHODS_H