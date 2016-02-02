					/************************************************************************/
					/*      һЩ���������ļ�������ʱ�亯������д�ļ�����������ת������      */
					/************************************************************************/
#include "MdSpi.h"
#include "Custom_Methods.h"
using namespace std;

extern string  local_host;
extern string  user_name;
extern string  user_pwd;
extern string  data_base;
extern string  table;
extern string  data_port;
extern string  database_driver;
extern int	  DATA_TIME_INTERVAL;
/************************************************************************/
/*							����ʱ��ת������					       */
/************************************************************************/
//1����11:45:32��ð��ȥ��  11:45:32->114532
extern int method_TimeToInt(char *tick_UpdateTime){
	try
	{
		int result = 0;
		char p[10] = {0};
		char *buf=tick_UpdateTime;
		int i=0,j=0;
		while (buf[i]!=NULL)
		{
			if( buf[i]!=':') {
				p[j] = buf[i];
				j++;
			}
			i++;
		}
		result=atoi(p);

		return result;
	}
	catch (std::exception& e)
	{
		std::cerr <<"exception caught:methodWriteFile2 "<< e.what() <<'\n';
	}
	
};
//2��ת��60���������ʱ���뻻������ӣ�����Ϊ114316,����������11*3600+43*60+16
extern int method_TimeToInt_60(int itime){

	return (itime/10000)*3600+((itime%10000)/100)*60+(itime%100);
};
//3�����������ʱ���뻻�������ת���ɹ����ʱ���뻻������� 102460 -> 102500
int method_Template_TimeInt(int ntime){
	int h,m,s;
	h = ntime/10000;
	m = ntime%10000/100;
	s = ntime%100;
	if (s>=60)
	{
		s =s%60;
		m =m+1;
		if (m>=60)
		{
			m=m%60;
			h=h+1;
		}
		return ((h*10000)+(m*100)+s) % 240000;

	}else{
		return ntime % 240000;            //(ҹ��24����Ϊ0)
	}

}
//4����ʱ���뻻�������    ����int method_TimeToInt_60(int itime)
int method_TimeToInt_60(){

	SYSTEMTIME st; 
	GetLocalTime(&st);  
	unsigned int h =st.wHour;
	unsigned int m =st.wMinute;
	unsigned int s =st.wSecond;

	return (h*3600)+(m*60)+(s%100);
};
//5����104316����ʱ������������   ��ϵͳʱ�� ->104316��ʽ
int method_TimeToInt_Int(){

	SYSTEMTIME st; 
	GetLocalTime(&st);  
	unsigned int h =st.wHour;
	unsigned int m =st.wMinute;
	unsigned int s =st.wSecond;
	return (h*10000)+(m*100)+s;
};
//6��ͣ����Ϣʱ��
bool method_Rest_Time(){
	//������ͣ��ʱ��
	int  SuspensionTime=method_TimeToInt_Int();
	if ((SuspensionTime   > CLOSE_EVENING + TIME_DIF	&& SuspensionTime< OPEN_MORNING - TIME_DIF)
		||(SuspensionTime > CLOSE_MORNING + TIME_DIF	&& SuspensionTime< OPEN_NOON - TIME_DIF)
		||(SuspensionTime > CLOSE_NOON  + 2 * TIME_DIF  && SuspensionTime< OPEN_EVENING - TIME_DIF)
		){
			return true;
	}else{
		return false;
	}

};
//7���������Ͼ��۵�һЩ����������
bool filter_Unreasonable_Price(int datetime){

	//������ͣ��ʱ��
	int  local_time = method_TimeToInt_Int();
	//2:30 ~9:00   11:30 ~13:00   15:15 ~ 21:00 
	if ((datetime   > CLOSE_EVENING && datetime< OPEN_MORNING)			//2:30 ~9:00
		||(datetime > CLOSE_MORNING && datetime< OPEN_NOON )				//11:30 ~13:00 
		||(datetime > CLOSE_NOON  +  TIME_DIF  && datetime< OPEN_EVENING)	// 15:15 ~ 21:00 
		){
			return true;
	}
	//���˿�����ʱ��һ���׶����һ������  ����12:54:23�����򣬻ᷢ��11:30:00������
	if (local_time - datetime > 500 )
	{
		return true;
	}

	return false;

};
/************************************************************************/
/*                          д���ļ���������                             */
/************************************************************************/
//д���ļ�

void methodWriteFile(CThostFtdcDepthMarketDataField_Custom &data,string filename){
	bool flag = (data.UpdateTime>=235700&&data.UpdateTime<=240000)
		         ||(data.UpdateTime>=0&&data.UpdateTime<=300)
				 ||(data.UpdateTime>=112700&&data.UpdateTime<=113000)
				 ||(data.UpdateTime>=145700&&data.UpdateTime<=150000)
				 ||(data.UpdateTime>=151400&&data.UpdateTime<=151500)
				 ||(data.UpdateTime>=210000&&data.UpdateTime<=210300);	
	if (!flag)
	{
		return;
	}

	try
	{
		FILE * fw;
		string name = ".\\datas2\\"+filename;
		fw = _fsopen(name.c_str(),"ab+",_SH_DENYWR);//D:\\�������\\�������\\������ϴ\\testMdUserApi.doc
		while (1)
		{
			if (fw!=NULL)
			{
				LockFile(fw,0,0,0,0);
				fwrite(&data,sizeof(data),1,fw);
				UnlockFile(fw,0,0,0,0);
				break;
			}else{
				continue;
			}
		}
		fclose(fw);
	}

	catch (std::exception& e)
	{
		std::cerr <<"exception caught:methodWriteFile "<< e.what() <<'\n';
	}
		
};
void methodWriteFile2(CThostFtdcDepthMarketDataField_Custom_Other &data,string filename){	
	bool flag = (data.UpdateTime>=235700&&data.UpdateTime<=240000)
		||(data.UpdateTime>=0&&data.UpdateTime<=300)
		||(data.UpdateTime>=112700&&data.UpdateTime<=113000)
		||(data.UpdateTime>=145700&&data.UpdateTime<=150000)
		||(data.UpdateTime>=151400&&data.UpdateTime<=151500)
		||(data.UpdateTime>=210000&&data.UpdateTime<=210300);	
	if (!flag)
	{
		return;
	}
	try
	{
		FILE * fw;
		string name = ".\\datas2\\"+filename;
		fw = _fsopen(name.c_str(),"ab+",_SH_DENYWR);//D:\\�������\\�������\\������ϴ\\testMdUserApi.doc
		while (1)
		{
			if (fw!=NULL)
			{
				LockFile(fw,0,0,0,0);
				fwrite(&data,sizeof(data),1,fw);
				UnlockFile(fw,0,0,0,0);
				break;
			}else{
				continue;
			}
		}

		fclose(fw);
	}

	catch (std::exception& e)
	{
		std::cerr <<"exception caught:methodWriteFile2 "<< e.what() <<'\n';
	}


};

//����׼��Tick����ת�����Զ����Tick����---OperatorData.cpp�õ�  ת������
CThostFtdcDepthMarketDataField_Custom_Tick methodChangeData(CThostFtdcDepthMarketDataField_Custom &original_data){
	//CThostFtdcDepthMarketDataField_Custom  ->  CThostFtdcDepthMarketDataField_Custom_Tick
	try
	{
		CThostFtdcDepthMarketDataField_Custom_Tick custom_tick;
		//strcpy_s(custom_tick.ExchangeID,sizeof(custom_tick.ExchangeID),original_data.ExchangeID);
		//strcpy_s(custom_tick.InstrumentID,sizeof(custom_tick.InstrumentID),original_data.InstrumentID);
		//strncpy_s(b,_countof(b),a,strlen(a));

		strncpy_s(custom_tick.ExchangeID,_countof(custom_tick.ExchangeID),original_data.ExchangeID,strlen(original_data.ExchangeID));
		strncpy_s(custom_tick.InstrumentID,_countof(custom_tick.InstrumentID),original_data.InstrumentID,strlen(original_data.InstrumentID));
		custom_tick.TradingDay= original_data.TradingDay;

		custom_tick.LastPrice=original_data.LastPrice;

		custom_tick.Volume=original_data.Volume;
		custom_tick.Turnover=original_data.Turnover;
		custom_tick.OpenInterest=original_data.OpenInterest;
		custom_tick.UpdateTime=original_data.UpdateTime;
		custom_tick.BidPrice1=original_data.BidPrice1;
		custom_tick.BidVolume1=original_data.BidVolume1;
		custom_tick.AskPrice1=original_data.AskPrice1;
		custom_tick.AskVolume1=original_data.AskVolume1;
		custom_tick.OpenInterestRange = 0;

		return custom_tick;
	}
	catch (std::exception& e)
	{
		std::cerr <<"exception caught:methodChangeData "<< e.what() <<'\n';
	}
	
};
/************************************************************************/
/*                          ��ӡ���ݲ���                                */
/************************************************************************/

//��ȡ�������ļ��ķ���                             

void readdata1()
{

	FILE* fr,*fw;
	char fn[255];
	try
	{
		fr=_fsopen(".\\datas2\\tick","rb",_SH_DENYWR);  //tick quotation TESTtest222 D:\\�������\\�������\\testMdUserApi.doc\\datas2\\ag1412
		sprintf_s(fn,".\\datas2\\%s.txt","tick");
		fw=_fsopen(fn,"wb",_SH_DENYWR);
		CThostFtdcDepthMarketDataField_Custom tb;
		//CThostFtdcDepthMarketDataField tb;
		//CThostFtdcDepthMarketDataField_Custom_Tick tb;
		//CThostFtdcDepthMarketDataField_Custom_Other tb;
		int cccccc =0;
		//fseek(fr,5000000*sizeof(tb),0);
		while (fread(&tb,sizeof(tb),1,fr)>0/*&&cccccc<100000*/)
		{
			//if (tb.UpdateTime>112800&&tb.UpdateTime<134000)
			//if ((tb.UpdateTime>130000))
			{
				try
				{   
					fprintf(fw,"%s,%d,%s,%d,%d,%.2f,%ld\r\n",tb.ExchangeID,tb.TradingDay,tb.InstrumentID,tb.UpdateTime,tb.UpdateMillisec,tb.LastPrice,tb.ReceiveTime);
					
					//fprintf(fw,"%s,%s,%d,%d,%.2f,%ld\r\n",tb.ExchangeID,tb.InstrumentID,tb.UpdateTime,tb.UpdateMillisec,tb.LastPrice,tb.ReceiveTime);
					//fprintf(fw,"%s,%s,%s,%.2f,%.2f\r\n",tb.InstrumentID,tb.TradingDay,tb.UpdateTime,tb.OpenPrice,tb.LastPrice);
					//fprintf(fw,"%s,%d,%.2f,%.2f,%.2f,%.2f\r\n",tb.InstrumentID,tb.UpdateTime,tb.LastPrice,tb.Volume,tb.OpenInterest,tb.OpenInterestRange);
					//fprintf(fw,"%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",tb.UpdateTime,tb.OpenPrice,tb.HighestPrice,tb.LowestPrice,tb.ClosePrice,tb.Volume,tb.OpenInterest,tb.OpenInterestRange);
				}
				catch (exception)
				{
					printf("%s","�Ҳ����ļ�����ļ�ʧ��");
				}
			}

			cccccc++;
		}
		// 		fseek(fr,-500000*sizeof(CThostFtdcDepthMarketDataField_Custom),2);
		// 		while (fread(&tb,sizeof(tb),1,fr)>0/*&&cccccc<100000*/)
		// 		{
		// 			try
		// 			{   
		// 				fprintf(fw,"%s,%s,%d,%d,%.2f,%ld\r\n",tb.ExchangeID,tb.InstrumentID,tb.UpdateTime,tb.UpdateMillisec,tb.LastPrice,tb.ReceiveTime);
		// 				//fprintf(fw,"%s,%s,%s,%.2f,%.2f\r\n",tb.InstrumentID,tb.TradingDay,tb.UpdateTime,tb.OpenPrice,tb.LastPrice);
		// 				//fprintf(fw,"%s,%d,%.2f,%.2f,%.2f,%.2f\r\n",tb.InstrumentID,tb.UpdateTime,tb.LastPrice,tb.Volume,tb.OpenInterest,tb.OpenInterestRange);
		// 				//fprintf(fw,"%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",tb.UpdateTime,tb.OpenPrice,tb.HighestPrice,tb.LowestPrice,tb.ClosePrice,tb.Volume,tb.OpenInterest,tb.OpenInterestRange);
		// 			}
		// 			catch (exception)
		// 			{
		// 				printf("%s","�Ҳ����ļ�����ļ�ʧ��");
		// 			}
		// 			//cccccc++;
		// 		}
		fclose(fw);
		fclose(fr);
	}
	catch (exception* e)
	{

		printf("%s","�Ҳ����ļ�����ļ�ʧ��");
		throw(e);
	}

}
void readdata2()
{

	FILE* fr,*fw;
	char fn[255];
	try
	{
		fr=_fsopen(".\\datas2\\original","rb",_SH_DENYWR);  //TESTtest222 D:\\�������\\�������\\testMdUserApi.doc\\datas2\\ag1412
		sprintf_s(fn,".\\datas2\\%s.txt","original");
		fw=_fsopen(fn,"wb",_SH_DENYWR);
		CThostFtdcDepthMarketDataField_Custom tb;
		//CThostFtdcDepthMarketDataField tb;
		//CThostFtdcDepthMarketDataField_Custom_Tick tb;
		//CThostFtdcDepthMarketDataField_Custom_Other tb;
		int cccccc =0;
		//fseek(fr,1000000*sizeof(tb),0);

		while (fread(&tb,sizeof(tb),1,fr)>0/*&&cccccc<100000*/)
		{
			//if (tb.UpdateTime>112800&&tb.UpdateTime<134000)
			//if (tb.UpdateTime>235500||tb.TradingDay>20151012)
			//if ((tb.UpdateTime>130000&&tb.UpdateTime<210000))
			{
				try
				{   
					fprintf(fw,"%s,%d,%s,%d,%d,%.2f,%ld\r\n",tb.ExchangeID,tb.TradingDay,tb.InstrumentID,tb.UpdateTime,tb.UpdateMillisec,tb.LastPrice,tb.ReceiveTime);

					//fprintf(fw,"%s,%s,%d,%d,%.2f,%ld\r\n",tb.ExchangeID,tb.InstrumentID,tb.UpdateTime,tb.UpdateMillisec,tb.LastPrice,tb.ReceiveTime);
					//fprintf(fw,"%s,%s,%s,%.2f,%.2f\r\n",tb.InstrumentID,tb.TradingDay,tb.UpdateTime,tb.OpenPrice,tb.LastPrice);
					//fprintf(fw,"%s,%d,%.2f,%.2f,%.2f,%.2f\r\n",tb.InstrumentID,tb.UpdateTime,tb.LastPrice,tb.Volume,tb.OpenInterest,tb.OpenInterestRange);
					//fprintf(fw,"%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",tb.UpdateTime,tb.OpenPrice,tb.HighestPrice,tb.LowestPrice,tb.ClosePrice,tb.Volume,tb.OpenInterest,tb.OpenInterestRange);
				}
				catch (exception)
				{
					printf("%s","�Ҳ����ļ�����ļ�ʧ��");
				}
			}

			cccccc++;
		}
		// 		fseek(fr,-500000*sizeof(CThostFtdcDepthMarketDataField_Custom),2);
		// 		while (fread(&tb,sizeof(tb),1,fr)>0/*&&cccccc<100000*/)
		// 		{
		// 			try
		// 			{   
		// 				fprintf(fw,"%s,%s,%d,%d,%.2f,%ld\r\n",tb.ExchangeID,tb.InstrumentID,tb.UpdateTime,tb.UpdateMillisec,tb.LastPrice,tb.ReceiveTime);
		// 				//fprintf(fw,"%s,%s,%s,%.2f,%.2f\r\n",tb.InstrumentID,tb.TradingDay,tb.UpdateTime,tb.OpenPrice,tb.LastPrice);
		// 				//fprintf(fw,"%s,%d,%.2f,%.2f,%.2f,%.2f\r\n",tb.InstrumentID,tb.UpdateTime,tb.LastPrice,tb.Volume,tb.OpenInterest,tb.OpenInterestRange);
		// 				//fprintf(fw,"%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",tb.UpdateTime,tb.OpenPrice,tb.HighestPrice,tb.LowestPrice,tb.ClosePrice,tb.Volume,tb.OpenInterest,tb.OpenInterestRange);
		// 			}
		// 			catch (exception)
		// 			{
		// 				printf("%s","�Ҳ����ļ�����ļ�ʧ��");
		// 			}
		// 			//cccccc++;
		// 		}
		fclose(fw);
		fclose(fr);
	}
	catch (exception* e)
	{
		printf("%s","�Ҳ����ļ�����ļ�ʧ��");
		throw(e);
	}

}
void readdata3()
{

	FILE* fr,*fw;
	char fn[255];
	try
	{
		fr=_fsopen(".\\datas2\\60","rb",_SH_DENYWR);  //today-1mintoday-5secTESTtest222 D:\\�������\\�������\\testMdUserApi.doc\\datas2\\ag1412
		sprintf_s(fn,".\\datas2\\%s.txt","60");
		fw=_fsopen(fn,"wb",_SH_DENYWR);
		CThostFtdcDepthMarketDataField_Custom_Other tb;
		//CThostFtdcDepthMarketDataField tb;
		//CThostFtdcDepthMarketDataField_Custom_Tick tb;
		//CThostFtdcDepthMarketDataField_Custom_Other tb;
		int cccccc =0;
		//fseek(fr,1000000*sizeof(tb),0);

		while (fread(&tb,sizeof(tb),1,fr)>0/*&&cccccc<100000*/)
		{
			//if (tb.UpdateTime>112800&&tb.UpdateTime<134000)
			//if ((tb.UpdateTime>105000)&&tb.TradingDay==20151013)
			{
				try
				{   
					fprintf(fw,"%s,%d,%.2f,%.2f,%.2f,%.2f\r\n",tb.InstrumentID,tb.UpdateTime,tb.OpenPrice,tb.LowestPrice,tb.HighestPrice,tb.ClosePrice);
					//fprintf(fw,"%s,%s,%s,%.2f,%.2f\r\n",tb.InstrumentID,tb.TradingDay,tb.UpdateTime,tb.OpenPrice,tb.LastPrice);
					//fprintf(fw,"%s,%d,%.2f,%.2f,%.2f,%.2f\r\n",tb.InstrumentID,tb.UpdateTime,tb.LastPrice,tb.Volume,tb.OpenInterest,tb.OpenInterestRange);
					//fprintf(fw,"%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",tb.UpdateTime,tb.OpenPrice,tb.HighestPrice,tb.LowestPrice,tb.ClosePrice,tb.Volume,tb.OpenInterest,tb.OpenInterestRange);
				}
				catch (exception)
				{
					printf("%s","�Ҳ����ļ�����ļ�ʧ��");
				}
			}

			cccccc++;
		}
		// 		fseek(fr,-500000*sizeof(CThostFtdcDepthMarketDataField_Custom),2);
		// 		while (fread(&tb,sizeof(tb),1,fr)>0/*&&cccccc<100000*/)
		// 		{
		// 			try
		// 			{   
		// 				fprintf(fw,"%s,%s,%d,%d,%.2f,%ld\r\n",tb.ExchangeID,tb.InstrumentID,tb.UpdateTime,tb.UpdateMillisec,tb.LastPrice,tb.ReceiveTime);
		// 				//fprintf(fw,"%s,%s,%s,%.2f,%.2f\r\n",tb.InstrumentID,tb.TradingDay,tb.UpdateTime,tb.OpenPrice,tb.LastPrice);
		// 				//fprintf(fw,"%s,%d,%.2f,%.2f,%.2f,%.2f\r\n",tb.InstrumentID,tb.UpdateTime,tb.LastPrice,tb.Volume,tb.OpenInterest,tb.OpenInterestRange);
		// 				//fprintf(fw,"%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",tb.UpdateTime,tb.OpenPrice,tb.HighestPrice,tb.LowestPrice,tb.ClosePrice,tb.Volume,tb.OpenInterest,tb.OpenInterestRange);
		// 			}
		// 			catch (exception)
		// 			{
		// 				printf("%s","�Ҳ����ļ�����ļ�ʧ��");
		// 			}
		// 			//cccccc++;
		// 		}
		fclose(fw);
		fclose(fr);
	}
	catch (exception* e)
	{
		printf("%s","�Ҳ����ļ�����ļ�ʧ��");
		throw(e);
	}

}
//��ȡ�����ļ�
void ReadConfigFile()
{
	try
	{
 		ifstream infile("config.txt",ios::in);
 		char buff[256];
 		while (infile.getline(buff,256))//fgets(buf,50,fr)
 		{
			
 			//cout<<buff<<endl;
 			int month_sum =0;
 			//�ָ�������
			char *p[3]={0};
			//p[0] = new char[20];
			//p[1] = new char[20];
 			char *b=buff;
 			while((p[month_sum]=strtok(b,":"))!=NULL) {
 				month_sum++;
 				b=NULL;
 			}
			
 			//cout<<p[0]<<endl;
 			if (strcmp(p[0],"local_host")==0)
 			{
 				local_host =  p[1];
				
 			}else if (strcmp(p[0],"user_name")==0)
 			{
 				user_name =  p[1];
 			}else if (strcmp(p[0],"user_pwd")==0)
 			{
 				user_pwd =  p[1];
 			}else if (strcmp(p[0],"data_base")==0)
 			{
 				data_base =  p[1];
 			}else if (strcmp(p[0],"table")==0)
 			{
 				table =  p[1];
 			}else if (strcmp(p[0],"data_port")==0)
 			{
 				data_port = p[1];
			}else if (strcmp(p[0],"database_driver")==0)
			{
				database_driver =  p[1];
			}else if (strcmp(p[0],"DATA_TIME_INTERVAL")==0)
			{
				DATA_TIME_INTERVAL = atoi(p[1]);
			}

			//cout<<p[1]<<endl;
 		}	
 		infile.close();
	
	}
	catch (exception* e)
	{
		printf("%s","�Ҳ����ļ����ߴ��ļ�ʧ�ܣ�");
		throw(e);
	}
}



////�Զ����ɺ�Լ�����                       
void methodAutoContract(){

	//��ȡ������Ϣ
	ReadConfigFile();

	try
	{

		time_t now_time;
		struct tm *t ;
		now_time = time(NULL);
		t= localtime(&now_time);
		//�·ݺ����
		int year = t->tm_year-100;
		int mon = t->tm_mon+1;
		int n =0;
		//ifstream infile(".\\��Լ.txt",ios::in);/*D:\\�������\\�������\\testMdUserApi.doc\\*/
		ifstream infile("��Լ.txt",ios::in);/*D:\\�������\\�������\\testMdUserApi.doc\\*/

		// 		FILE* fr;
		// 		fr=_fsopen(".\\��Լ.txt","r",_SH_DENYWR);
		char buf[4096];
		string exchangecode="shfe";
		int SUM_CONTRACTS=0;
		char temp[CONTRACTCOUNT][20];
		while (infile.getline(buf,4096))//fgets(buf,50,fr)
		{
			if ((string)buf=="dce")
			{
				exchangecode="dce";
			}else if((string)buf=="shfe")
			{
				exchangecode="shfe";
			}else if((string)buf=="czce")
			{
				exchangecode="czce";
			}else if((string)buf=="cffex")
			{
				exchangecode="cffex";
			}
			else
			{
				//����Ҫ��Ϊ��ʼֵ
				year = t->tm_year - 100;
				//�����ܺ�
				int month_sum =0;
				//�ָ�������
				char *p[50]={0};
				char *b=buf;
				//char **t;
				while((p[month_sum]=strtok(b,","))!=NULL) {
					month_sum++;
					b=NULL;
				}


				//��ǰ�·��±�
				int current_month_index;
				for(current_month_index=1;current_month_index<month_sum;current_month_index++)
				{
					if (atoi(p[current_month_index])>=mon)
					{
						for (int a=1;a<current_month_index;a++)
						{	

							p[month_sum]=p[a];
							month_sum++;	
							p[a]=NULL;
						}
						break;
					}
				}
				//20151210��   ��С�ڱ��µ��·���һ��Ҳ������ʾ
				if (current_month_index == month_sum)
				{
				    current_month_index=1;       
					year+=1;
				}
				//�ȵ�һ��
				char tempbuf[10];
				//֣��������ֻ��һλ
				year = (exchangecode=="czce"?year-10:year);
				_itoa(year,tempbuf,10);
				strcpy_s(temp[SUM_CONTRACTS],p[0]);
				strcat_s(temp[SUM_CONTRACTS],tempbuf);
				strcat_s(temp[SUM_CONTRACTS],p[current_month_index]);
				//�ŵ�map��ȥ
				ppInstrumentID[SUM_CONTRACTS] = temp[SUM_CONTRACTS];
				cmdSpi_custom->exchange_map[temp[SUM_CONTRACTS]] = exchangecode; 
				//cout<<ppInstrumentID[SUM_CONTRACTS];
				SUM_CONTRACTS++;	



				/*cout<<temp<<" ";*/
				for(int t=current_month_index+1;t<month_sum;t++)
				{

					if (atoi(p[t])<atoi(p[t-1]))
					{

						//������1
						year++;
						_itoa_s(year,tempbuf,10);   
						strcpy_s(temp[SUM_CONTRACTS],p[0]);
						strcat_s(temp[SUM_CONTRACTS],tempbuf);
						strcat_s(temp[SUM_CONTRACTS],p[t]);
					}else{
						_itoa_s(year,tempbuf,10);
						strcpy_s(temp[SUM_CONTRACTS],p[0]);
						strcat_s(temp[SUM_CONTRACTS],tempbuf);
						strcat_s(temp[SUM_CONTRACTS],p[t]);
					}
					//�ŵ�map��ȥ
					ppInstrumentID[SUM_CONTRACTS] = temp[SUM_CONTRACTS];/*strcpy(ppInstrumentID[SUM_CONTRACTS],temp);	*/
					cmdSpi_custom->exchange_map[temp[SUM_CONTRACTS]] = exchangecode; 
					//cout<<ppInstrumentID[SUM_CONTRACTS];
					SUM_CONTRACTS++;	

					/*cout<<temp<<" ";*/
				}

				//cout<<endl;

			}
		}
		//fclose(fr);
		infile.close();
	}
	catch (exception* e)
	{
		printf("%s","�Ҳ����ļ����ߴ��ļ�ʧ�ܣ�");
		throw(e);
	}
}
//�ַ����ָ��
vector< string> split( string str, string pattern)
{
	vector<string> ret;
	if(pattern.empty()) return ret;
	size_t start=0,index=str.find_first_of(pattern,0);
	while(index!=str.npos)
	{
		if(start!=index)
			ret.push_back(str.substr(start,index-start));
		start=index+1;
		index=str.find_first_of(pattern,start);
	}
	if(!str.substr(start).empty())
		ret.push_back(str.substr(start));
	return ret;
}
////�Զ����ɺ�Լ�����                       
void methodAutoContract2(){

	//��ȡ������Ϣ
	ReadConfigFile();
	int SUM_CONTRACTS=0;   
	try
	{
		ifstream infile("��Լ.txt",ios::in);
		char buff[256];
		while (infile.getline(buff,256))//fgets(buf,50,fr)
		{

			string str =(string)buff;
			string pattern=",";
			vector< string> result=split(str,pattern);
			for(int i=1; i<result.size(); i++)
			{
				char letter[10];
				ppInstrumentID[SUM_CONTRACTS] = new char(20);
				strcpy(ppInstrumentID[SUM_CONTRACTS],result[i].c_str());
				cmdSpi_custom->exchange_map[ppInstrumentID[SUM_CONTRACTS]] =result[0];
				SUM_CONTRACTS++;	
			}

		}	
		infile.close();
	}
	catch (exception* e)
	{
		printf("%s","�Ҳ����ļ����ߴ��ļ�ʧ�ܣ�");
		throw(e);
	}
}
////�Զ����ɺ�Լ�����                       
void methodAutoContract3(){

	//��ȡ������Ϣ
	ReadConfigFile();
	int SUM_CONTRACTS=0;   
	try
	{
		time_t now_time;
		struct tm *t ;
		now_time = time(NULL);
		t= localtime(&now_time);


		//_itoa_s(year,tempbuf,10);  

		//�·ݺ����
		char year[256];
		char year_next[256];
		char year_czce[256];
		char year_czce_next[256];
		char mon[256] ;
		_itoa_s(t->tm_year-100,year,10);
		_itoa_s(t->tm_year-100+1,year_next,10);
		_itoa_s(t->tm_year-100-10,year_czce,10);
		_itoa_s(t->tm_year-100-10+1,year_czce_next,10);
		_itoa_s(t->tm_mon+1,mon,10);

		ifstream infile("��Լ.txt",ios::in);
		char buff[256];
		while (infile.getline(buff,256))//fgets(buf,50,fr)
		{

			string str =(string)buff;
			string pattern=",";
			vector< string> result=split(str,pattern);
			for(int i=2; i<result.size(); i++)
			{
				char letter[10];

				ppInstrumentID[SUM_CONTRACTS] = new char(20);
				string temp="";
				if (result[0]=="czce")
				{
					if (atoi(result[i].c_str()) <atoi(mon))
					{
						//��һ���
						temp =result[1] +(string)year_czce_next + result[i]; 
					}else{
						//�����
						temp =result[1] +(string)year_czce + result[i]; 
					}
				}
				else{
					if (atoi(result[i].c_str()) <atoi(mon))
					{
						//��һ���
						temp =result[1] +(string)year_next + result[i]; 
					}else{
						//�����
						temp =result[1] + (string)year + result[i]; 
					}
				}


				strcpy(ppInstrumentID[SUM_CONTRACTS],temp.c_str());

				cmdSpi_custom->exchange_map[ppInstrumentID[SUM_CONTRACTS]] =result[0];
				SUM_CONTRACTS++;	
			}

		}	
		infile.close();
	}
	catch (exception* e)
	{
		printf("%s","�Ҳ����ļ����ߴ��ļ�ʧ�ܣ�");
		throw(e);
	}
}
///ADO�������ݿ�
void Ado_Connect_Mysql(CThostFtdcDepthMarketDataField_Custom_Other &ctd_data)
{

#pragma region �������ݿ�
	//////////////////////////////////////////////////////////////////////////�������ݿ�
	CoInitialize(NULL); 
	_ConnectionPtr m_pConnection=NULL;
	//����Connection����
	m_pConnection.CreateInstance("ADODB.Connection");//(__uuidof(Connection));   
	try 
	{  
		//��������ʱ��  
		m_pConnection->ConnectionTimeout = 5;
		_bstr_t connstr = database_driver.c_str();
		connstr	+=";server=";
		connstr += local_host.c_str();
		connstr += ";database=";
		connstr += data_base.c_str();
		//�����ݿ�����  
		//HRESULT hr =
		 m_pConnection->Open(connstr,user_name.c_str(),user_pwd.c_str(),adModeUnknown); //adModeUnknown ȱʡ����ǰ�����Ȩδ����
		//m_pConnection->Open("Data Source=adotest;UID=sa;PWD=;","","",adModeUnknown);
	}  
	catch(_com_error &e)  
	{  
		cout<<"���Ӵ���"<<endl;
		return ;  
	}   
	//////////////////////////////////////////////////////////////////////////�������ݿ�


#pragma endregion �������ݿ�

#pragma region ִ��SQL���
	//////////////////////////////////////////////////////////////////////////�������
	//ִ��SELETE���
	_RecordsetPtr m_pRecordset;
	m_pRecordset.CreateInstance("ADODB.Recordset");
	//m_pRecordset.CreateInstance(__uuidof( Recordset )); //����ʵ��
	//һ��Ҫ��
	_bstr_t openRecordset="SELECT  * FROM ";
	openRecordset+= table.c_str();
	openRecordset +=" limit 1";
	m_pRecordset->Open(openRecordset,m_pConnection.GetInterfacePtr
		(),adOpenStatic,adLockOptimistic,adCmdText);// ִ��SQL��䣬�õ���¼��, m_pConnection������֪
	try{
		if (m_pRecordset!=NULL)
		{
			if (ctd_data.TradingDay!=0)
			{
				m_pRecordset->AddNew();	
				m_pRecordset->PutCollect("exchange_id",_variant_t(ctd_data.ExchangeID));
				m_pRecordset->PutCollect("instrument_id",_variant_t(ctd_data.InstrumentID));
				m_pRecordset->PutCollect("trading_day",_variant_t((long)(ctd_data.TradingDay)));
				//ȡ����ȥ��90001�ֵ�1��
				int remainder = DATA_TIME_INTERVAL >=60 ? 100 : DATA_TIME_INTERVAL;							
				m_pRecordset->PutCollect("update_time",_variant_t((long)(ctd_data.UpdateTime-ctd_data.UpdateTime%remainder)));		

				m_pRecordset->PutCollect("open_price",_variant_t(ctd_data.OpenPrice));
				m_pRecordset->PutCollect("close_price",_variant_t(ctd_data.ClosePrice));
				m_pRecordset->PutCollect("highest_price",_variant_t(ctd_data.HighestPrice));
				m_pRecordset->PutCollect("lowest_price",_variant_t(ctd_data.LowestPrice));	
				m_pRecordset->PutCollect("turnover",_variant_t(ctd_data.Turnover));			
				m_pRecordset->PutCollect("open_interest",_variant_t((long)(ctd_data.OpenInterest)));
				m_pRecordset->PutCollect("open_interestrange",_variant_t((long)(ctd_data.OpenInterestRange)));
				m_pRecordset->PutCollect("bid_price1",_variant_t(ctd_data.BidPrice1));
				m_pRecordset->PutCollect("ask_price1",_variant_t(ctd_data.AskPrice1));
				m_pRecordset->PutCollect("volume",_variant_t((long)(ctd_data.Volume)));
				m_pRecordset->PutCollect("bid_volume1",_variant_t(ctd_data.BidVolume1));
				m_pRecordset->PutCollect("ask_volume1",_variant_t(ctd_data.AskVolume1));
				m_pRecordset->Update();     ///һ��Ҫ���浽���У���Ȼ��Ч

			}

		}

	}catch(_com_error &e){
		cout<<e.ErrorMessage()<<endl;
		//continue;
	}

	//////////////////////////////////////////////////////////////////////////�������
#pragma endregion  ִ��SQL���

#pragma region �ر���������            //20151218 �ͷ�ָ��
 	try
 	{
 		//�������
 		//memset(all_contracts_array.All_Contracts_Data, 0x0, all_contracts_array.index_all * sizeof(CThostFtdcDepthMarketDataField_Custom));
 		m_pRecordset->Close(); //�رռ�¼��   
 		m_pConnection->Close();//�ر����ݿ�  
 	
		if (m_pRecordset)
		{
			m_pRecordset.Release();//�ͷż�¼������ָ��  ��"."���� "->"
			m_pRecordset=NULL;			
		}
 		if (m_pConnection)
 		{
			m_pConnection.Release();//�ͷ����Ӷ���ָ�� ��"."���� "->"
 			m_pConnection=NULL; 			 			
		}
		
 	}
 	catch(_com_error &e){
 		cout<<e.ErrorMessage()<<endl;
 		//continue;
 	}
 #pragma endregion �ر���������
 
 	CoUninitialize();
	return ;
}

//���˲�����۸�
bool  priceRange(CThostFtdcDepthMarketDataField_Custom_Other &current_data,CThostFtdcDepthMarketDataField_Custom_Tick &custom_tick)
{
 	double HighestPrice = current_data.HighestPrice;
	double LowestPrice = current_data.LowestPrice;
	double LastPrice = custom_tick.LastPrice;

 	double range=0;
	//�Ƚ���߼ۺ����¼ۼ۲�
	if (HighestPrice >LastPrice)
	{
		range=(HighestPrice-LastPrice)*100.0/LastPrice;
	}else
	{
		range=(LastPrice-HighestPrice)*100.0/HighestPrice;
	}

	if (range >10)
	{
 		
 		ofstream out(".\\datas2\\log.txt",ios::app);  
 		if (out.is_open())   
 		{     
 			out <<"ʱ�䣺"<<current_data.UpdateTime <<"   Ʒ�֣�"<<current_data.InstrumentID<<"  ���̼ۣ�"<<current_data.OpenPrice<<"  ��߼ۣ�"<<current_data.HighestPrice<<"  ��ͼۣ�"<<current_data.LowestPrice<<"  ���̼ۣ�"<<current_data.ClosePrice<<"\n";  
 		    out <<"ʱ�䣺"<<custom_tick.UpdateTime <<"   Ʒ�֣�"<<custom_tick.InstrumentID<<"  ���¼ۣ�"<<custom_tick.LastPrice<<"\n";  
 			out <<"----------------------------------"<<"\n";
 			out.close();  
 		} 
		
		return true;
	}
	//�Ƚ���ͼۺ����¼ۼ۲�
	if (LowestPrice >LastPrice)
	{
		range=(LowestPrice-LastPrice)*100.0/LastPrice;
	}else
	{
		range=(LastPrice-LowestPrice)*100.0/LowestPrice;
	}

	if (range >10)
	{

 		ofstream out(".\\datas2\\log.txt",ios::app);  
 		if (out.is_open())   
 		{     
 			out <<"ʱ�䣺"<<current_data.UpdateTime <<"   Ʒ�֣�"<<current_data.InstrumentID<<"  ���̼ۣ�"<<current_data.OpenPrice<<"  ��߼ۣ�"<<current_data.HighestPrice<<"  ��ͼۣ�"<<current_data.LowestPrice<<"  ���̼ۣ�"<<current_data.ClosePrice<<"\n";  
 			out <<"ʱ�䣺"<<custom_tick.UpdateTime <<"   Ʒ�֣�"<<custom_tick.InstrumentID<<"  ���¼ۣ�"<<custom_tick.LastPrice<<"\n";  
 			out <<"-------------------------------------"<<"\n";
 			out.close();  
 		} 

		return true;
	}
 	
	return false;
}

