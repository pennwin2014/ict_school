#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <time.h>
#include "utoall.h"
#include "utoplt01.h"
#include "pasdb.h"
#include <pcap.h>

#include <errno.h>

#include <iconv.h>

#include <dirent.h>
#include "ncportalweb.h"
#include "ncportal.h"
#define COUPONDOWNTIME 60 //下载优惠劵时间间隔

#include "ncportal.h"
int ncStrToStr(char *stime, char *etime);

//楼层信息
int ncmLevels(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  
	// printf("ncmLevels start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char plateid[32] ="";
	char plateName[32] ="";
	char tsid[32] ="";
	utMsgGetSomeNVar(psMsgHead, 3,
					 "tsid",        	UT_TYPE_STRING,  sizeof(tsid)-1,      	 tsid,
					 "plateid",     	UT_TYPE_STRING,  sizeof(plateid)-1,      plateid,
					 "plateName",       UT_TYPE_STRING,  sizeof(plateName)-1,    plateName
					 );
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);
	char caTemp[256];
	sprintf(caTemp,"select groupid, name,detail from ncmmall where flags = 1 ");	
	if(strlen(plateid) >0){
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and plateid = %d",atol(plateid));
	}
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " order by groupid");
  	// printf("caTemp=%s\n",caTemp);
	
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		int iNum=0;
		long id =0;
		char name[24];
		char detail[128];
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,4,&id,
      	                     UT_TYPE_STRING,23,name,
							 UT_TYPE_STRING,127,detail
							 ))  || 1405==iReturn)
		{	
			iNum++;
			utPltPutLoopVarF(psDbHead,"id",iNum,"%lu", id);
			char *nameUTF = convert("GBK", "UTF-8", name);
			utPltPutLoopVarF(psDbHead,"name",iNum,"%s",nameUTF);
			utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu",iNum);
			char *detailUTF = convert("GBK", "UTF-8", detail);
			utPltPutLoopVarF(psDbHead,"detail",iNum,"%s",detailUTF);
		}
		pasDbCloseCursor(psCur);
	}
//	utPltShowDb(psDbHead);
	if(strlen(plateName)>0){
	char palteName1[128]="";
	sprintf(palteName1,"/%s/ncm_levels.htm",plateName);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	}else{
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_levels.htm");
	}
	// printf("ncmLevels  end!!!!\n");     
	return 0;	  
}

//分类信息
int ncmCategories(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmCategories start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char caTemp[256];
	// // utMsgPrintMsg(psMsgHead);
	char plateName[32] ="";
	char tsid[32] ="";
	char plateid[32] ="";
	utMsgGetSomeNVar(psMsgHead, 3,
					 "plateName",      UT_TYPE_STRING,  sizeof(plateName)-1,     plateName,
					 "plateid",        UT_TYPE_STRING,  sizeof(plateid)-1,       plateid,
					 "tsid",           UT_TYPE_STRING,  sizeof(tsid)-1,  	     tsid);
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);
	sprintf(caTemp,"select groupid, name, imgpath from ncmcomgroup ");
	if(strlen(plateid) >0){
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where plateid = %d",atol(plateid));
	}
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " order by groupid");
  	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		int iNum=0;
		long id =0;
		char name[24];
		char imgpath[128];
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,4,&id,
      	                     UT_TYPE_STRING,23,name,
							 UT_TYPE_STRING,127,imgpath))  || 1405==iReturn)
		{	
			// printf("name = %s\n",name);
			iNum++;
			utPltPutLoopVarF(psDbHead,"id",iNum,"%lu", id);
			utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d", iNum);
			char *nameUTF = convert("GBK", "UTF-8", name);
			utPltPutLoopVarF(psDbHead,"name",iNum,"%s",nameUTF);
			utPltPutLoopVarF(psDbHead,"imgpath",iNum,"%s",imgpath);
		}
		pasDbCloseCursor(psCur);
	}
	
	char areaid[32];
	strcpy(areaid,"4101");//默认郑州
	sprintf(caTemp,"select aid, aname from ncmarea ");
	if(strlen(areaid) >0){
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where pid = %d",atol(areaid));
	}
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " order by aid");
  	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		int iNum=0;
		long aid =0;
		char aname[48];
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,sizeof(aid),&aid,
      	                     UT_TYPE_STRING,sizeof(aname),aname))  || 1405==iReturn)
		{	
			// printf("aname = %s\n",aname);
			iNum++;
			utPltPutLoopVarF(psDbHead,"aid",iNum,"%lu", aid);
			utPltPutLoopVarF(psDbHead,"iNum2",iNum,"%d", iNum);
			char *anameUTF = convert("GBK", "UTF-8", aname);
			utPltPutLoopVarF(psDbHead,"aname",iNum,"%s",anameUTF);
		}
		pasDbCloseCursor(psCur);
	}
	
//	utPltShowDb(psDbHead);
	if(strlen(plateName)>0){
	char palteName1[128]="";
	sprintf(palteName1,"/%s/ncm_categories.htm",plateName);
	// printf("palteName1 = %s\n",palteName1);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	}else{
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_categories.htm");
	}
	// printf("ncmCategories  end!!!!\n"); 
	return 0;	  
}

//小类
int ncmCategories_Types(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmCategories_Types start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	// // utMsgPrintMsg(psMsgHead);
	char logoid[32] = "";
	char id[32] = "";
	char limit1 [32] ="";
	char limit2 [32] ="";
	char level[32] ="";
	char tsid[32] ="";
	char plateid[32] ="";
	char plateName[32] ="";
	utMsgGetSomeNVar(psMsgHead, 8,
   				     "logoid",        	UT_TYPE_STRING,  sizeof(logoid)-1,       logoid,
					 "id",       	 	UT_TYPE_STRING,  sizeof(id)-1,      	 id,
					 "level",        	UT_TYPE_STRING,  sizeof(level)-1,        level,
					 "limit1",        	UT_TYPE_STRING,  sizeof(limit1)-1,       limit1,
					 "limit2",        	UT_TYPE_STRING,  sizeof(limit2)-1,       limit2,
					 "tsid",        	UT_TYPE_STRING,  sizeof(tsid)-1,      	 tsid,
					 "plateid",     	UT_TYPE_STRING,  sizeof(plateid)-1,      plateid,
					 "plateName",       UT_TYPE_STRING,  sizeof(plateName)-1,    plateName
					 );
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);
	char caTemp[256];
	sprintf(caTemp,"select sid, name, imgpath from ncmcomtype where flags=0 and groupid = %s ",id);
	if(strlen(plateid) >0){
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and plateid = %d",atol(plateid));
	}
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " order by groupid");	
  	// printf("caTemp=%s\n",caTemp);
	//int getSid[24]={1,2,3,4,5,6,7,8,9,0};
    psCur = pasDbOpenSql(caTemp,0);
	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		int iNum=0;
		long id =0;
		char name[24];
		char img[128];
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,4,&id,
      	                     UT_TYPE_STRING,23,name,
							 UT_TYPE_STRING,127,img))  || 1405==iReturn)
		{	
			iNum++;
			// for()
			// if()
			utPltPutLoopVarF(psDbHead,"id",iNum,"%lu", id);
			utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu", iNum);
			char *nameUTF = convert("GBK", "UTF-8", name);
			utPltPutLoopVarF(psDbHead,"name",iNum,"%s",nameUTF);
			utPltPutLoopVarF(psDbHead,"img",iNum,"%s",img);
		}
		pasDbCloseCursor(psCur);
	}
//	utPltShowDb(psDbHead);
	if(strlen(plateName)>0){
	char palteName1[128]="";
	sprintf(palteName1,"/%s/ncm_categories_types.htm",plateName);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	}else{
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_categories_types.htm");
	}
	// printf("ncmCategories_Types  end!!!!\n");
    return 0;	  
}

//小类商店对照表
int ncm_ShopType(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncm_ShopType start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	// // utMsgPrintMsg(psMsgHead);
	char sgroupid[32] = "";
	char id[32] = "";
	char limit1 [32] ="";
	char limit2 [32] ="";
	char level[32] ="";
	char plateName[32] ="";
	char tsid[32] ="";
	char plateid[32] ="";
	char areaid[32] ="";
	char keyword[32] ="";
	char keyword2[32] ="";
	utMsgGetSomeNVar(psMsgHead, 10,
   				     "sgroupid",         UT_TYPE_STRING,  sizeof(sgroupid)-1,     sgroupid,
					 "id",       		 UT_TYPE_STRING,  sizeof(id)-1,			  id,
					 "level",       	 UT_TYPE_STRING,  sizeof(level)-1,     	  level,
					 "limit1",       	 UT_TYPE_STRING,  sizeof(limit1)-1,       limit1,
					 "limit2",       	 UT_TYPE_STRING,  sizeof(limit2)-1,       limit2,
					 "tsid",       		 UT_TYPE_STRING,  sizeof(tsid)-1,     	  tsid,
					 "plateid",     	 UT_TYPE_STRING,  sizeof(plateid)-1,      plateid,
					 "areaid",     	     UT_TYPE_STRING,  sizeof(areaid)-1,       areaid,
					 "keyword",     	 UT_TYPE_STRING,  sizeof(keyword)-1,      keyword,
					 "plateName",   	 UT_TYPE_STRING,  sizeof(plateName)-1,    plateName
					 );
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);
	int getShopId = 0;
	int getShopTypeId = 0;
	
	//getShopId = 1373740321;
	// for(){
		// if(getShopTypeId = atol(sgroupid)){
		// getShopId = ncSrvGetCid(psShmHead,psMsgHead);
		// }
	// }
	char caTemp[256];
	if(strlen(plateid) >0){
	sprintf(caTemp,"select shopid, shopname, imgpath,sname,fee,discount from ncmshoptype where plateid= %d",atol(plateid));	
	
	if(getShopId !=0){
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp)," and shopid = %d",getShopId);
	}
	if(strlen(sgroupid)>0 && strcmp("undefined",sgroupid) !=0){
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and sgroupid = %d",atol(sgroupid));
	}
	if(strlen(areaid) >0){
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and areaid = %d",atol(areaid));
	}
	if(strlen(keyword) >0){
	pasCvtGBK(2,keyword,keyword2,30);
	strcpy(keyword,keyword2);
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and shopname like '%%%s%%'",keyword);
	}
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " group by shopid order by level asc, systime desc");
	}
  	// printf("caTemp=%s\n",caTemp);
	
    psCur = pasDbOpenSql(caTemp,0);
	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		int iNum=0;
		long shopid =0;
		char shopname[32];
		char sname[32];
		char imgpath[128];
		double fee=0;
		double discount=0;
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,sizeof(shopid),&shopid,
      	                     UT_TYPE_STRING,sizeof(shopname)-1,shopname,
							 UT_TYPE_STRING,sizeof(imgpath)-1,imgpath,
							 UT_TYPE_STRING,sizeof(sname)-1,sname,
							 UT_TYPE_FLOAT,sizeof(fee),&fee,
							 UT_TYPE_FLOAT,sizeof(discount),&discount
							 ))  || 1405==iReturn)
		{	
			
			iNum++;
			utPltPutLoopVarF(psDbHead,"shopid",iNum,"%lu", shopid);
			if(strlen(sname)>0)utPltPutLoopVarF(psDbHead,"fee",iNum,"%0.2f", fee);
			utPltPutLoopVarF(psDbHead,"discount",iNum,"%0.2f", discount);
			utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu", iNum);
			char *nameUTF = convert("GBK", "UTF-8", shopname);
			utPltPutLoopVarF(psDbHead,"shopname",iNum,"%s",nameUTF);
			char *snameUTF = convert("GBK", "UTF-8", sname);
			utPltPutLoopVarF(psDbHead,"sname",iNum,"%s",snameUTF);
			utPltPutLoopVarF(psDbHead,"imgpath",iNum,"%s",imgpath);
		}
		pasDbCloseCursor(psCur);
	}
//	utPltShowDb(psDbHead);
	if(strlen(plateName)>0){
		char palteName1[128]="";
		if(strlen(keyword)>0){
			sprintf(palteName1,"/%s/ncm_shoptype2.htm",plateName);
		}
		else {
			sprintf(palteName1,"/%s/ncm_shoptype.htm",plateName);
		}
		utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	}else{
		if(strlen(keyword)>0){
			utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_shoptype2.htm");
		}
		else{
			utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_shoptype.htm");
		}
	}
	// printf("ncm_ShopType  end!!!!\n");
    return 0;	  
}

int ncmShops_Discounts(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmShops_Discounts start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	
	char logoid[32] = "";
	char shopid[32] = "";
	char limit1 [32] ="";
	char limit2 [32] ="";
	char level [32] ="";
	// // utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 5,
   				     "logoid",        UT_TYPE_STRING,  sizeof(logoid)-1,       logoid,
					 "shopid",        UT_TYPE_STRING,  sizeof(shopid)-1,       shopid,
					 "level",         UT_TYPE_STRING,  sizeof(level)-1,        level,
					 "limit1",        UT_TYPE_STRING,  sizeof(limit1)-1,       limit1,
					 "limit2",        UT_TYPE_STRING,  sizeof(limit2)-1,       limit2);
					  
	char caTemp[256];
	
	snprintf(caTemp,sizeof(caTemp),"select sid, name, imgpath,plevel from ncmcomindex where flags = 0 and type = 3 and plevel !=0" );	
	
	if(strlen(shopid)>0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and shopid = %s ",shopid);
	}
	
	if(strlen(level)>0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and plevel = %s ",level);
	}
	
	if(strlen(limit1) >0 && strlen(limit2)>0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu",atol(limit1),atol(limit2));
	}
	
	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		// printf("0000000000000\n");
		int iReturn=0;
		int iNum=0;
		long id =0;
		char name[32];
		char imgpath[128];
		long plevel =0;
	
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,4,&id,
      	                     UT_TYPE_STRING,31,name,
							 UT_TYPE_STRING,127,imgpath,
							 UT_TYPE_LONG,sizeof(plevel), &plevel))  || 1405==iReturn)
		{	
		// printf("name = %s\n",name);
		iNum++;
		char *nameUTF = convert("GBK", "UTF-8", name);
		char *imgpathUTF = convert("GBK", "UTF-8", imgpath);
		
		if (plevel == 1){
		utPltPutVarF(psDbHead,"name1","%s",nameUTF);
		utPltPutVarF(psDbHead,"id1","%lu", id);
		utPltPutVarF(psDbHead,"imgpath1","%s",imgpathUTF);
		
		}
		if (plevel == 2){
		utPltPutVarF(psDbHead,"name2","%s",nameUTF);
		utPltPutVarF(psDbHead,"id2","%lu", id);
		utPltPutVarF(psDbHead,"imgpath2","%s",imgpathUTF);
		
		}
		if (plevel == 3){
		utPltPutVarF(psDbHead,"name3","%s",nameUTF);
		utPltPutVarF(psDbHead,"id3","%lu", id);
		utPltPutVarF(psDbHead,"imgpath3","%s",imgpathUTF);
		
		}
		if (plevel == 4){
		utPltPutVarF(psDbHead,"name4","%s",nameUTF);
		utPltPutVarF(psDbHead,"id4","%lu", id);
		utPltPutVarF(psDbHead,"imgpath4","%s",imgpathUTF);
		
		}
		if (plevel == 5){
		utPltPutVarF(psDbHead,"name5","%s",nameUTF);
		utPltPutVarF(psDbHead,"id5","%lu", id);
		utPltPutVarF(psDbHead,"imgpath5","%s",imgpathUTF);
		
		}
		if (plevel == 6){
		utPltPutVarF(psDbHead,"name6","%s",nameUTF);
		utPltPutVarF(psDbHead,"id6","%lu", id);
		utPltPutVarF(psDbHead,"imgpath6","%s",imgpathUTF);
		
		}
		if (plevel == 7){
		utPltPutVarF(psDbHead,"name7","%s",nameUTF);
		utPltPutVarF(psDbHead,"id7","%lu", id);
		utPltPutVarF(psDbHead,"imgpath7","%s",imgpathUTF);
		
		}
		if (plevel == 8){
		utPltPutVarF(psDbHead,"name8","%s",nameUTF);
		utPltPutVarF(psDbHead,"id8","%lu", id);
		utPltPutVarF(psDbHead,"imgpath8","%s",imgpathUTF);
		
		}
		if (plevel == 9){
		utPltPutVarF(psDbHead,"name9","%s",nameUTF);
		utPltPutVarF(psDbHead,"id9","%lu", id);
		utPltPutVarF(psDbHead,"imgpath9","%s",imgpathUTF);
		
		}
		if (plevel == 10){
		utPltPutVarF(psDbHead,"name10","%s",nameUTF);
		utPltPutVarF(psDbHead,"id10","%lu", id);
		utPltPutVarF(psDbHead,"imgpath10","%s",imgpathUTF);
		
		}
		if (plevel == 11){
		utPltPutVarF(psDbHead,"name11","%s",nameUTF);
		utPltPutVarF(psDbHead,"id11","%lu", id);
		utPltPutVarF(psDbHead,"imgpath11","%s",imgpathUTF);
		
		}
		if (plevel == 12){
		utPltPutVarF(psDbHead,"name1","%s",nameUTF);
		utPltPutVarF(psDbHead,"id12","%lu", id);
		utPltPutVarF(psDbHead,"imgpath12","%s",imgpathUTF);
		
		}
		
		
		}
		// printf("22222222222\n");
		pasDbCloseCursor(psCur);
	}
//		utPltShowDb(psDbHead);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_levels_logos_shops_discounts.htm");
		// printf("ncmShops_Discounts  end!!!!\n");
      
	  return 0;	  
}

//促销活动
int ncmDiscounts(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmDiscounts start!!!!\n");
	 pasDbCursor *psCur;
	// utPltDbHead *psDbHead;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	
	char logoid[32] = "";
	char shopid[32] = "";
	char limit1 [32] ="";
	char limit2 [32] ="";
	char level [32] ="";
	char tsid [32] ="";
	char plateid [32] ="";
	char plateName [32] ="";
	// // utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 8,
   				     "logoid",        UT_TYPE_STRING,  sizeof(logoid)-1,       logoid,
					 "shopid",        UT_TYPE_STRING,  sizeof(shopid)-1,       shopid,
					 "level",        UT_TYPE_STRING,  sizeof(level)-1,       level,
					 "limit1",        UT_TYPE_STRING,  sizeof(limit1)-1,       limit1,
					 "limit2",        UT_TYPE_STRING,  sizeof(limit2)-1,       limit2,
					 "tsid",        	UT_TYPE_STRING,  sizeof(tsid)-1,      	 tsid,
					 "plateid",     	UT_TYPE_STRING,  sizeof(plateid)-1,      plateid,
					 "plateName",       UT_TYPE_STRING,  sizeof(plateName)-1,    plateName);
					  
	char caTemp[256];
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);
	utPltPutVarF(psDbHead,"limit1","%lu", atol(limit1));
	utPltPutVarF(psDbHead,"limit2","%lu", atol(limit2));
	snprintf(caTemp,sizeof(caTemp),"select id,aname, plevel, xid,imgpath,address,decript,stime,etime,timedesc,activedesc,guests from ncmactive where flags = 0 and type = 1" );	
	if(strlen(shopid) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and shopid = %lu ",atol(shopid));
	}
	if(strlen(plateid) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and plateid = %d",atol(plateid));
	}
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " order by plevel");
	if(strlen(limit1) >0 && strlen(limit2)>0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu",atol(limit1),atol(limit2));
	}
	// printf("caTemp=%s\n",caTemp);
	
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
	// printf("pscur is null\n");
	// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
	int iReturn=0;
   	int iNum=0;
   	int limit=atol(limit1);
	long id =0;
	long plevel =0;
	long xid =0;
	char imgpath[128];
	char aname[255];
	char address[255];
	char timedesc[128];
	char activedesc[1024];
	char guests[255];
	char decript[1024];
	long stime=0;
	long etime=0;
	char _stime[64];
	char _etime[64];
	

	while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,4,&id,
							 UT_TYPE_STRING,sizeof(aname),aname,
      	                     UT_TYPE_LONG,4,&plevel,
							 UT_TYPE_LONG,4,&xid,
							 UT_TYPE_STRING,127,imgpath,
							 UT_TYPE_STRING,sizeof(address),address,
							 UT_TYPE_STRING,sizeof(decript),decript,
							UT_TYPE_LONG,4,&stime,
							UT_TYPE_LONG,4,&etime,
							 UT_TYPE_STRING,sizeof(timedesc),timedesc,
							  UT_TYPE_STRING,sizeof(activedesc),activedesc,
							  UT_TYPE_STRING,sizeof(guests),guests
							))  || 1405==iReturn)
	 {	
		iNum++;
		limit++;
		utPltPutLoopVarF(psDbHead,"id",iNum,"%lu", id);
		utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu", iNum);
		utPltPutLoopVarF(psDbHead,"limit",iNum,"%lu", limit);
		utPltPutLoopVarF(psDbHead,"plevel",iNum,"%lu",plevel);
		utPltPutLoopVarF(psDbHead,"xid",iNum,"%lu",xid);
		strcpy(_stime, utTimFormat("%Y/%m/%d %H:%M:%S", stime));
		strcpy(_etime, utTimFormat("%Y/%m/%d %H:%M:%S", etime));
		utPltPutLoopVarF(psDbHead,"stime",iNum,"%s",_stime);
		utPltPutLoopVarF(psDbHead,"etime",iNum,"%s",_etime);
		char *anameUTF = convert("GBK", "UTF-8", aname);
		utPltPutLoopVarF(psDbHead,"aname",iNum,"%s",anameUTF);
		char *imgpathUTF = convert("GBK", "UTF-8", imgpath);
		utPltPutLoopVarF(psDbHead,"imgpath",iNum,"%s",imgpathUTF);
		
		char *addressUTF = convert("GBK", "UTF-8", address);
		utPltPutLoopVarF(psDbHead,"address",iNum,"%s",addressUTF);
		char *decriptUTF = convert("GBK", "UTF-8", decript);
		utPltPutLoopVarF(psDbHead,"decript",iNum,"%s",decriptUTF);
		char *timedescUTF = convert("GBK", "UTF-8", timedesc);
		utPltPutLoopVarF(psDbHead,"timedesc",iNum,"%s",timedescUTF);
		char *activedescUTF = convert("GBK", "UTF-8", activedesc);
		utPltPutLoopVarF(psDbHead,"activedesc",iNum,"%s",activedescUTF);
		char *guestsUTF = convert("GBK", "UTF-8", guests);
		utPltPutLoopVarF(psDbHead,"guests",iNum,"%s",guestsUTF);
		
		if(plevel<=4){
		utPltPutLoopVarF(psDbHead,"imgslider",iNum,"%s",imgpathUTF);
		}
		}
		pasDbCloseCursor(psCur);
	}
	
		
	//	utPltShowDb(psDbHead);
		if(strlen(plateName)>0){
		char palteName1[128]="";
		sprintf(palteName1,"/%s/ncm_discounts.htm",plateName);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
		}else{
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_discounts.htm");
		}
		// printf("ncmDiscounts  end!!!!\n");
      
	  return 0;	  
}

//促销详情
int ncmDiscounts_Details(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmDiscounts_Details start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	
	char logoid[32] = "";
	char shopid[32] = "";
	char xid[32] = "";
	char limit1 [32] ="";
	char limit2 [32] ="";
	char level [32] ="";
	char tsid [32] ="";
	char plateid [32] ="";
	char plateName [32] ="";
	// // utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 9,
   				"logoid",       UT_TYPE_STRING,  sizeof(logoid)-1,		logoid,
					"shopid",       UT_TYPE_STRING,  sizeof(shopid)-1,		shopid,
					"xid",	        UT_TYPE_STRING,  sizeof(xid)-1,			xid,
					"level",        UT_TYPE_STRING,  sizeof(level)-1,		level,
					"limit1",       UT_TYPE_STRING,  sizeof(limit1)-1,   	limit1,
					"limit2",       UT_TYPE_STRING,  sizeof(limit2)-1,		limit2,
					"tsid",        	UT_TYPE_STRING,  sizeof(tsid)-1,      	 tsid,
					"plateid",     	UT_TYPE_STRING,  sizeof(plateid)-1,      plateid,
					"plateName",       UT_TYPE_STRING,  sizeof(plateName)-1,    plateName);
					  
	char caTemp[256];
	int tReturn = 0;
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);
	tReturn = ncmShopUserOnlineFlag(psShmHead, iFd,psMsgHead);
	// printf("********test******tReturn = %d \n",tReturn);
	utPltPutVarF(psDbHead,"user_online_flag","%d", tReturn);
	
	snprintf(caTemp,sizeof(caTemp),"select id, servicecode, aname,shopid,sid,plevel, xid,imgpath from ncmactive where flags = 0 and type = 2 and xid=%lu group by imgpath",atol(xid) );	
	
	if(strlen(limit1) >0 && strlen(limit2)>0){
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu",atol(limit1),atol(limit2));
	}
	
	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		int iNum=0;
		long id =0;
		long plevel =0;
		long xid =0;
		long sid =0;
		long shopid =0;
		char servicecode[32]="";
		char aname[32]="";
		
		char imgpath[128]="";
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,4,&id,
							 UT_TYPE_STRING,31,servicecode,
							 UT_TYPE_STRING,31,aname,
							 UT_TYPE_LONG,4,&shopid,
							 UT_TYPE_LONG,4,&sid,
      	                     UT_TYPE_LONG,4,&plevel,
							 UT_TYPE_LONG,4,&xid,
							 UT_TYPE_STRING,127,imgpath))  || 1405==iReturn)
		{	
			iNum++;
			utPltPutLoopVarF(psDbHead,"id",iNum,"%lu", id);
			utPltPutLoopVarF(psDbHead,"shopid",iNum,"%lu", shopid);
			utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu", sid);
			utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu", iNum);
			utPltPutLoopVarF(psDbHead,"plevel",iNum,"%lu",plevel);
			utPltPutLoopVarF(psDbHead,"xid",iNum,"%lu",xid);
			char *servicecodeUTF = convert("GBK", "UTF-8", servicecode);
			utPltPutLoopVarF(psDbHead,"servicecode",iNum,"%s",servicecodeUTF);
			char *anameUTF = convert("GBK", "UTF-8", aname);
			utPltPutLoopVarF(psDbHead,"aname",iNum,"%s",anameUTF);
			char *imgpathUTF = convert("GBK", "UTF-8", imgpath);
			utPltPutLoopVarF(psDbHead,"imgpath",iNum,"%s",imgpathUTF);
		}
		pasDbCloseCursor(psCur);
	}
	sprintf(caTemp,"select cid from ncmactive where flags = 0 and type = 2 and xid=%lu group by cid", atol(xid));	
	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		int iNum=0;
		long cid =0;
		char imgpath[128]="";
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,4,&cid))  || 1405==iReturn)
		{	
		iNum++;
		utPltPutLoopVarF(psDbHead,"cid",iNum,"%lu", cid);
		}
		pasDbCloseCursor(psCur);
	}
//	utPltShowDb(psDbHead);
	if(strlen(plateName)>0){
		char palteName1[128]="";
		sprintf(palteName1,"/%s/ncm_discounts_details.htm",plateName);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	}else{
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_discounts_details.htm");
	}
	// printf("ncmDiscounts_Details  end!!!!\n");
    return 0;	  
}

//个人中心
int ncmPersonals(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmPersonals start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char uid[32] = "";
	char plateName[32] = "";
	char tsid[32] = "";
	char plateid[32] = "";
	// // utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 4,
					"uid",        UT_TYPE_STRING,  sizeof(uid)-1,     		uid,
					"tsid",       UT_TYPE_STRING,  sizeof(tsid)-1,    	    tsid,
					"plateid",    UT_TYPE_STRING,  sizeof(plateid)-1,       plateid,
					"plateName",  UT_TYPE_STRING,  sizeof(plateName)-1, 	plateName);
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);
	char caTemp[256];
	ncPortalOnline *onlineinfo;
	onlineinfo = (ncPortalOnline *)malloc(sizeof(ncPortalOnline)+1);
    // printf("*******tsid = %s\n",tsid);
    if(strlen(tsid) > 0)
    {
		onlineinfo = ncSrvGetOnlineUserByStrTsid(psShmHead,tsid);
		// printf("*******caName = %s \n",onlineinfo->caName);
		sprintf(caTemp,"select uid, nickname, membernum, addtime, username,address, imgpath, integral, coupon from ncmshopuser where username='%s'", onlineinfo->caName);	
	}
	if(strlen(plateid) >0){
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and plateid = %d",atol(plateid));
	}
  	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		int iNum=0;
		long id =0;
		char nickname[32];
		char membernum[32];//会员卡
		long addtime=0;//开卡时间
		char username[32];//手机号
		char address[128];
		char img[128];
		long integral=0;//积分
		long coupon=0;//优惠券
		
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,4,&id,
      	                     UT_TYPE_STRING,31,nickname,
							 UT_TYPE_STRING,31,membernum,
							 UT_TYPE_LONG,4,&addtime,
							 UT_TYPE_STRING,31,username, 
							 UT_TYPE_STRING,31,address,
							 UT_TYPE_STRING,127,img,
							 UT_TYPE_LONG,4,&integral,
							 UT_TYPE_LONG,4,&coupon))  || 1405==iReturn)
		{	
			iNum++;
			char _stime[40] = "";
			utPltPutVarF(psDbHead,"id","%lu", id);
			char *nicknameUTF = convert("GBK", "UTF-8", nickname);
			utPltPutVarF(psDbHead,"nickname","%s",nicknameUTF);
			utPltPutVarF(psDbHead,"membernum","%s",membernum);
			utPltPutVarF(psDbHead,"username","%s",username);
			utPltPutVarF(psDbHead,"img","%s",img);
			char *addressUTF = convert("GBK", "UTF-8", address);
			utPltPutVarF(psDbHead,"address","%s",addressUTF);
			strcpy(_stime, utTimFormat("%Y/%m/%d", addtime));
			utPltPutVarF(psDbHead,"addtime","%s", _stime);
			utPltPutVarF(psDbHead,"integral","%lu", integral);
			utPltPutVarF(psDbHead,"coupon","%lu", coupon);
		}
		pasDbCloseCursor(psCur);
	}
//	utPltShowDb(psDbHead);
	if(strlen(plateName)>0){
		char palteName1[128]="";
		sprintf(palteName1,"/%s/ncm_personals.htm",plateName);
		// printf("palteName1 = %s\n",palteName1);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	}
	else{
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_personals.htm");
	}
	// printf("ncmPersonals  end!!!!\n");
	return 0;	  
}
//个人信息
int ncmPersonals_Info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmPersonals_Info start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char uid[32] = "";
	char plateName[32] = "";
	char tsid[32] = "";
	char plateid[32] = "";
	// // utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 4,
   				     "uid",        UT_TYPE_STRING,  sizeof(uid)-1,      	 uid,
   				     "tsid",       UT_TYPE_STRING,  sizeof(tsid)-1,    		 tsid,
   				     "plateid",    UT_TYPE_STRING,  sizeof(plateid)-1,       plateid,
					 "plateName",  UT_TYPE_STRING,  sizeof(plateName)-1,	 plateName);
	utPltPutVarF(psDbHead,"tsid","%s", tsid);				  
	utPltPutVarF(psDbHead,"plateid","%s", plateid);				  
	// printf("uid = %s\n",uid);		
	char caTemp[256];
	ncPortalOnline *onlineinfo;
	onlineinfo = (ncPortalOnline *)malloc(sizeof(ncPortalOnline)+1);
    // printf("*******tsid = %s\n",tsid);
    if(strlen(tsid) > 0)
    {
		onlineinfo = ncSrvGetOnlineUserByStrTsid(psShmHead,tsid);
		// printf("*******caName = %s \n",onlineinfo->caName);
		sprintf(caTemp,"select uid, nickname, membernum, addtime, username,address, imgpath, integral, coupon from ncmshopuser where username='%s'", onlineinfo->caName);	
	}
	if(strlen(plateid) >0){
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and plateid = %d",atol(plateid));
	}
  	// printf("caTemp=%s\n",caTemp);
	
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		int iNum=0;
		long id =0;
		char nickname[32];
		char membernum[32];//会员卡
		long addtime=0;//开卡时间
		char username[32];//手机号
		char address[128];
		char img[128];
		long integral=0;//积分
		long coupon=0;//优惠券
		
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,4,&id,
      	                     UT_TYPE_STRING,31,nickname,
							 UT_TYPE_STRING,31,membernum,
							 UT_TYPE_LONG,4,&addtime,
							 UT_TYPE_STRING,31,username, 
							 UT_TYPE_STRING,31,address,
							 UT_TYPE_STRING,127,img,
							 UT_TYPE_LONG,4,&integral,
							 UT_TYPE_LONG,4,&coupon))  || 1405==iReturn)
		{	
			iNum++;
			char _stime[40] = "";
			utPltPutVarF(psDbHead,"id","%lu", id);
			char *nicknameUTF = convert("GBK", "UTF-8", nickname);
			utPltPutVarF(psDbHead,"nickname","%s",nicknameUTF);
			utPltPutVarF(psDbHead,"membernum","%s",membernum);
			utPltPutVarF(psDbHead,"username","%s",username);
			utPltPutVarF(psDbHead,"img","%s",img);
			char *addressUTF = convert("GBK", "UTF-8", address);
			utPltPutVarF(psDbHead,"address","%s",addressUTF);
			strcpy(_stime, utTimFormat("%Y/%m/%d", addtime));
			utPltPutVarF(psDbHead,"addtime","%s", _stime);
			utPltPutVarF(psDbHead,"integral","%lu", integral);
			utPltPutVarF(psDbHead,"coupon","%lu", coupon);
		}
		pasDbCloseCursor(psCur);
	}
//	utPltShowDb(psDbHead);
	if(strlen(plateName)>0){
		char palteName1[128]="";
		sprintf(palteName1,"/%s/ncm_personals_info.htm",plateName);
		// printf("palteName1 = %s\n",palteName1);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	}
	else{
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_personals_info.htm");
	}
	// printf("ncmPersonals_Info  end!!!!\n");
    return 0;	  
}
//个人积分
int ncmPersonals_Mark(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmPersonals_Mark start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char uid[32] = "";
	char plateName[32] = "";
	char tsid[32] = "";
	char plateid[32] = "";
	// utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 4,
   				     "uid",        UT_TYPE_STRING,  sizeof(uid)-1,    	  	 uid,
   				     "tsid",       UT_TYPE_STRING,  sizeof(tsid)-1,      	 tsid,
   				     "plateid",    UT_TYPE_STRING,  sizeof(plateid)-1,       plateid,
					 "plateName",  UT_TYPE_STRING,  sizeof(plateName)-1, 	 plateName);
	utPltPutVarF(psDbHead,"tsid","%s", tsid);					  
	utPltPutVarF(psDbHead,"plateid","%s", plateid);					  
	// printf("uid = %s\n",uid);		
	char caTemp[256];
	ncPortalOnline *onlineinfo;
	onlineinfo = (ncPortalOnline *)malloc(sizeof(ncPortalOnline)+1);
    // printf("*******tsid = %s\n",tsid);
    if(strlen(tsid) > 0)
    {
		onlineinfo = ncSrvGetOnlineUserByStrTsid(psShmHead,tsid);
		// printf("*******caName = %s \n",onlineinfo->caName);
		sprintf(caTemp,"select uid, nickname, membernum, addtime, username,address, imgpath, integral, coupon from ncmshopuser where username='%s'", onlineinfo->caName);	
	}
	//sprintf(caTemp,"select uid, nickname, membernum, addtime, username,address, imgpath, integral, coupon from ncmshopuser where uid=%s", uid);	
	if(strlen(plateid) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and plateid = %d",atol(plateid));
	}
  	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		int iNum=0;
		long id =0;
		char nickname[32];
		char membernum[32];//会员卡
		long addtime=0;//开卡时间
		char username[32];//手机号
		char address[128];
		char img[128];
		long integral=0;//积分
		long coupon=0;//优惠券
		
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,4,&id,
      	                     UT_TYPE_STRING,31,nickname,
							  UT_TYPE_STRING,31,membernum,
							   UT_TYPE_LONG,4,&addtime,
							  UT_TYPE_STRING,31,username, 
							   UT_TYPE_STRING,127,address,
							 UT_TYPE_STRING,127,img,
							 UT_TYPE_LONG,4,&integral,
							 UT_TYPE_LONG,4,&coupon))  || 1405==iReturn)
		{	
			iNum++;
			char _stime[40] = "";
			utPltPutVarF(psDbHead,"id","%lu", id);
			char *nicknameUTF = convert("GBK", "UTF-8", nickname);
			utPltPutVarF(psDbHead,"nickname","%s",nicknameUTF);
			utPltPutVarF(psDbHead,"membernum","%s",membernum);	
			utPltPutVarF(psDbHead,"username","%s",username);
			utPltPutVarF(psDbHead,"img","%s",img);
			
			char *addressUTF = convert("GBK", "UTF-8", address);
			utPltPutVarF(psDbHead,"address","%s",addressUTF);
			strcpy(_stime, utTimFormat("%Y/%m/%d %H:%M:%S", addtime));
			utPltPutVarF(psDbHead,"addtime","%s", _stime);
			utPltPutVarF(psDbHead,"integral","%lu", integral);
			utPltPutVarF(psDbHead,"coupon","%lu", coupon);
		}
		pasDbCloseCursor(psCur);
	}
//	utPltShowDb(psDbHead);
	if(strlen(plateName)>0){
		char palteName1[128]="";
		sprintf(palteName1,"/%s/ncm_personals_mark.htm",plateName);
		// printf("palteName1 = %s\n",palteName1);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	}
	else{
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_personals_mark.htm");
	}
	// printf("ncmPersonals_Mark  end!!!!\n");
    return 0;	  
}

//个人消费
int ncmPersonals_Consump(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmPersonals_Consump start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char uid[32] = "";
	char plateName[32] = "";
	char tsid[32] = "";
	char plateid[32] = "";
	// utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 4,
   				     "uid",        UT_TYPE_STRING,  sizeof(uid)-1,   	    uid,
   				     "tsid",       UT_TYPE_STRING,  sizeof(tsid)-1,     	tsid,
   				     "plateid",    UT_TYPE_STRING,  sizeof(plateid)-1,      plateid,
					 "plateName",  UT_TYPE_STRING,  sizeof(plateName)-1, 	plateName);
	utPltPutVarF(psDbHead,"tsid","%s", tsid);					  
	utPltPutVarF(psDbHead,"plateid","%s", plateid);					  
	// printf("uid = %s\n",uid);		

	char caTemp[256];
	ncPortalOnline *onlineinfo;
	onlineinfo = (ncPortalOnline *)malloc(sizeof(ncPortalOnline)+1);
    // printf("*******tsid = %s\n",tsid);
    if(strlen(tsid) > 0)
    {
		onlineinfo = ncSrvGetOnlineUserByStrTsid(psShmHead,tsid);
		// printf("*******caName = %s \n",onlineinfo->caName);
		sprintf(caTemp,"select cid, sname, fee, date from ncmconsump where username='%s'", onlineinfo->caName);	
	}
	if(strlen(plateid) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and plateid = %d",atol(plateid));
	}	
  	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		int iNum=0;
		long id =0;
		char sname[32];
		double fee=0;
		long date = 0;
		
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,4,&id,
      	                     UT_TYPE_STRING,31,sname,
							 UT_TYPE_FLOAT,4,&fee,
							 UT_TYPE_LONG,4,&date))  || 1405==iReturn)
		{	
			iNum++;
			char _stime[40] = "";
			utPltPutLoopVarF(psDbHead,"id",iNum,"%lu", id);	
			char *snameUTF = convert("GBK", "UTF-8", sname);
			utPltPutLoopVarF(psDbHead,"sname",iNum,"%s",snameUTF);
			strcpy(_stime, utTimFormat("%Y/%m/%d", date));
			utPltPutLoopVarF(psDbHead,"date",iNum,"%s", _stime);
			utPltPutLoopVarF(psDbHead,"fee",iNum,"%0.2f",fee);
		}
		pasDbCloseCursor(psCur);
	}
//	utPltShowDb(psDbHead);
	if(strlen(plateName)>0){
		char palteName1[128]="";
		sprintf(palteName1,"/%s/ncm_personals_consump.htm",plateName);
		// printf("palteName1 = %s\n",palteName1);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	}
	else{
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_personals_consump.htm");
	}
	// printf("ncmPersonals_Consump  end!!!!\n");
    return 0;	  
}
//优惠劵信息
int ncmDiscounts_Coupons(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char uid2[32], char delimg[16] )
{
	// printf("ncmDiscounts_Coupons start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char uid[32] = "";
	char cid[32] = "";
	char plateName[32] = "";
	char tsid[32] = "";
	char plateid[32] = "";
	// utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 5,
					"cid",        UT_TYPE_STRING,  sizeof(cid)-1,      		cid,
   				    "uid",        UT_TYPE_STRING,  sizeof(uid)-1,       	uid,
   				    "tsid",       UT_TYPE_STRING,  sizeof(tsid)-1,      	tsid,
   				    "plateid",    UT_TYPE_STRING,  sizeof(plateid)-1,       plateid,
					"plateName",  UT_TYPE_STRING,  sizeof(plateName)-1, 	plateName);
	utPltPutVarF(psDbHead,"tsid","%s", tsid);					  
	utPltPutVarF(psDbHead,"plateid","%s", plateid);					  
	if(uid==NULL){
		strcpy(uid,uid2);
		// printf("uid2=%s\n",uid2);
	}
	if(strcmp("del2",delimg)==0){
		utPltPutVarF(psDbHead,"delimgs","%s",delimg);
	}
	// printf("uid = %s\n",uid);	
	char caTemp[256];
	snprintf(caTemp, sizeof(caTemp),"select cid,fee,counum, addtime, effetime, imgpath from ncmcoupon ");	
	if(strlen(cid)> 0 ){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where cid in(%s) ", cid);	
	}
	if(strlen(uid)> 0 ){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " where uid=%lu ", atol(uid));	
	}
	if(strlen(plateid) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and plateid = %d",atol(plateid));
	}	
  	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		int iNum=0;
		long id =0;
		long counum =0;
		long addtime =0;
		double fee=0;
		long effetime = 0;
		char imgpath[128]="";
		while(0==(iReturn = pasDbFetchInto(psCur,
							UT_TYPE_LONG,4,&id,
							UT_TYPE_FLOAT,4,&fee,
							UT_TYPE_LONG,4,&counum,
							UT_TYPE_LONG,4,&addtime,
							UT_TYPE_LONG,4,&effetime,
							UT_TYPE_STRING,127,imgpath))  || 1405==iReturn)
		{	
			iNum++;
			char _stime[40] = "";
			char _addtime[40] = "";
			utPltPutLoopVarF(psDbHead,"id",iNum,"%lu", id);
			utPltPutLoopVarF(psDbHead,"fee",iNum,"%0.2f",fee);
			utPltPutLoopVarF(psDbHead,"counum",iNum,"%lu", counum);
			strcpy(_addtime, utTimFormat("%Y/%m/%d", addtime));
			utPltPutLoopVarF(psDbHead,"addtime",iNum,"%s", _addtime);
			strcpy(_stime, utTimFormat("%Y/%m/%d", effetime));
			utPltPutLoopVarF(psDbHead,"effetime",iNum,"%s", _stime);
			char *imgpathUTF = convert("GBK", "UTF-8", imgpath);
			utPltPutLoopVarF(psDbHead,"imgpath",iNum,"%s",imgpathUTF);
		}
		pasDbCloseCursor(psCur);
	}
//	utPltShowDb(psDbHead);
	if(strlen(plateName)>0){
		char palteName1[128]="";
		sprintf(palteName1,"/%s/ncm_discounts_coupons.htm",plateName);
		// printf("palteName1 = %s\n",palteName1);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	}
	else{
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_discounts_coupons.htm");
	}
	// printf("ncmDiscounts_Coupons  end!!!!\n");
    return 0;	  
}

//个人优惠劵
int ncmPersonals_Coupon(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char uid2[32], char delimg[16] )
{
	// printf("ncmPersonals_Coupon start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char uid[32] = "";
	char cid[32] = "";
	char plateName[32] = "";
	char tsid[32] = "";
	char plateid[32] = "";
	// utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 5,
					"cid",        UT_TYPE_STRING,  sizeof(cid)-1,       	cid,
   				    "uid",        UT_TYPE_STRING,  sizeof(uid)-1,     	  	uid,
   				    "tsid",       UT_TYPE_STRING,  sizeof(tsid)-1,       	tsid,
   				    "plateid",    UT_TYPE_STRING,  sizeof(plateid)-1,       plateid,
					"plateName",  UT_TYPE_STRING,  sizeof(plateName)-1, 	plateName);
	utPltPutVarF(psDbHead,"tsid","%s", tsid);				  
	utPltPutVarF(psDbHead,"plateid","%s", plateid);				  
	if(uid==NULL){
		strcpy(uid,uid2);
		// printf("uid2=%s\n",uid2);
	}
	if(strcmp("del2",delimg)==0){
		utPltPutVarF(psDbHead,"delimgs","%s",delimg);
	}
	// printf("uid = %s\n",uid);
	char caTemp[256];

	snprintf(caTemp, sizeof(caTemp),"select id,imgpath from ncmcouponuse where flags = 0 ");	
	if(strlen(cid)> 0 ){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and cid in(%s) ", cid);	
	}
	ncPortalOnline *onlineinfo;
	onlineinfo = (ncPortalOnline *)malloc(sizeof(ncPortalOnline)+1);
    // printf("*******tsid = %s\n",tsid);
    if(strlen(tsid) > 0)
    {
		onlineinfo = ncSrvGetOnlineUserByStrTsid(psShmHead,tsid);
		// printf("*******caName = %s \n",onlineinfo->caName);
		sprintf(caTemp," and username='%s'", onlineinfo->caName);	
	}
	if(strlen(plateid) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and plateid = %d",atol(plateid));
	}	
  	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		int iNum=0;
		long id =0;
		char imgpath[128]="";
		while(0==(iReturn = pasDbFetchInto(psCur,
							UT_TYPE_LONG,4,&id,
							UT_TYPE_STRING,127,imgpath))  || 1405==iReturn)
		{	
			iNum++;
			utPltPutLoopVarF(psDbHead,"id",iNum,"%lu", id);
			utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu", iNum);
			char *imgpathUTF = convert("GBK", "UTF-8", imgpath);
			utPltPutLoopVarF(psDbHead,"imgpath",iNum,"%s",imgpathUTF);
		}
		pasDbCloseCursor(psCur);
	}
//	utPltShowDb(psDbHead);
	if(strlen(plateName)>0){
		char palteName1[128]="";
		sprintf(palteName1,"/%s/ncm_personals_coupon.htm",plateName);
		// printf("palteName1 = %s\n",palteName1);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	}
	else{
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_personals_coupon.htm");
	}
	// printf("ncmPersonals_Coupon  end!!!!\n");
    return 0;	  
}


//个人关注
int ncmPersonals_Attention2(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmPersonals_Attention start!!!!\n");
	pasDbCursor *psCur;
	// utPltDbHead *psDbHead;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char uid[32] = "";
	char plateName[32] = "";
	char tsid[32] = "";
	char plateid[32] = "";
	// utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 4,
   				     "uid",        UT_TYPE_STRING,  sizeof(uid)-1,      	uid,
   				     "tsid",       UT_TYPE_STRING,  sizeof(tsid)-1,   	    tsid,
   				     "plateid",    UT_TYPE_STRING,  sizeof(plateid)-1,      plateid,
					 "plateName",  UT_TYPE_STRING,  sizeof(plateName)-1, 	plateName);
	utPltPutVarF(psDbHead,"tsid","%s", tsid);				  
	utPltPutVarF(psDbHead,"plateid","%s", plateid);				  
	// printf("uid = %s\n",uid);		
	int iReturn=0;
	char caTemp[256];
	ncPortalOnline *onlineinfo;
	onlineinfo = (ncPortalOnline *)malloc(sizeof(ncPortalOnline)+1);
    // printf("*******tsid = %s\n",tsid);
    if(strlen(tsid) > 0)
    {
		onlineinfo = ncSrvGetOnlineUserByStrTsid(psShmHead,tsid);
		// printf("*******caName = %s \n",onlineinfo->caName);
		sprintf(caTemp,"select decript from ncmshopuser where username='%s'", onlineinfo->caName);	
	}
	//sprintf(caTemp,"select decript from ncmshopuser where uid=%s", uid);	
  	// printf("caTemp=%s\n",caTemp);
	psCur = pasDbOpenSql(caTemp,0);
	char shopids[1024];
		 iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_STRING,sizeof(shopids), shopids);
		// printf("shopids = %s \n",shopids);
		 pasDbCloseCursor(psCur);
		 
		 
	iReturn=0;
	sprintf(caTemp,"select sid, name, shopid, imgpath from ncmcomindex where flags = 0 and type = 2 and sid in (%s)", shopids);
	if(strlen(plateid) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and plateid = %d",atol(plateid));
	}	
	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
	
	if(psCur == NULL){
	// printf("pscur is null\n");
	// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
	// printf("0000000000000\n");
	
   	int iNum=0;
	long id =0;
	char name[32];
	char img[128];
	long shopid=0;
	while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,4,&id,
      	                     UT_TYPE_STRING,31,name,
							 UT_TYPE_LONG,4,&shopid,
							 UT_TYPE_STRING,127,img))  || 1405==iReturn)
	 {	
		iNum++;
       char _stime[40] = "";
		utPltPutLoopVarF(psDbHead,"id",iNum,"%lu", id);
		utPltPutLoopVarF(psDbHead,"shopid",iNum,"%lu", shopid);
		char *nameUTF = convert("GBK", "UTF-8", name);
		utPltPutLoopVarF(psDbHead,"name",iNum,"%s",nameUTF);
		utPltPutLoopVarF(psDbHead,"img",iNum,"%s",img);
		utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum);
		
		}
		pasDbCloseCursor(psCur);
	}
//		utPltShowDb(psDbHead);
	if(strlen(plateName)>0){
		char palteName1[128]="";
		sprintf(palteName1,"/%s/ncm_personals_attention.htm",plateName);
		// printf("palteName1 = %s\n",palteName1);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	}
	else{
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_personals_attention.htm");
	}
		
		// printf("ncmPersonals_Attention  end!!!!\n");
      
	  return 0;	  
}

//个人关注
int ncmPersonals_Attention(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char uid2[32], char delimg[16])
{
	// printf("ncmPersonals_Attention start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char uid[32] = "";
	char delimgs[16] ="del";
	char plateName[32] = "";
	char tsid[32] = "";
	char plateid[32] = "";
	// utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 4,
   				    "uid",        UT_TYPE_STRING,  sizeof(uid)-1,   	    uid,
   				    "tsid",       UT_TYPE_STRING,  sizeof(tsid)-1, 	      	tsid,
   				    "plateid",    UT_TYPE_STRING,  sizeof(plateid)-1,       plateid,
					"plateName",  UT_TYPE_STRING,  sizeof(plateName)-1, 	plateName);
	utPltPutVarF(psDbHead,"tsid","%s", tsid);		
	utPltPutVarF(psDbHead,"plateid","%s", plateid);		
	if(uid==NULL){
		strcpy(uid,uid2);
		// printf("uid2=%s\n",uid2);
	}
	if(strcmp("del2",delimg)==0){
	//strcpy(delimgs,delimg);
	utPltPutVarF(psDbHead,"delimgs","%s",delimg);
	}
	// printf("uid = %s\n",uid);		
	int iReturn=0;
	char caTemp[256];
	ncPortalOnline *onlineinfo;
	onlineinfo = (ncPortalOnline *)malloc(sizeof(ncPortalOnline)+1);
    // printf("*******tsid = %s\n",tsid);
    if(strlen(tsid) > 0)
    {
		onlineinfo = ncSrvGetOnlineUserByStrTsid(psShmHead,tsid);
		// printf("*******caName = %s \n",onlineinfo->caName);
		sprintf(caTemp,"select aid, shopid, sid, imgpath from ncmshopattent where flags = 0 and username='%s'", onlineinfo->caName);	
	}
	//sprintf(caTemp,"select aid, shopid, sid, imgpath from ncmshopattent where flags = 0 and uid = %s ", uid);
	if(strlen(plateid) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and plateid = %d",atol(plateid));
	}		
	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
	
	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iNum=0;
		long aid =0;
		long sid =0;
		char img[128];
		long shopid=0;
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,4,&aid,
							 UT_TYPE_LONG,4,&shopid,
							 UT_TYPE_LONG,4,&sid,
							 UT_TYPE_STRING,127,img))  || 1405==iReturn)
	 {	
		iNum++;
		char _stime[40] = "";
		utPltPutLoopVarF(psDbHead,"aid",iNum,"%lu", aid);
		utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu", sid);
		utPltPutLoopVarF(psDbHead,"shopid",iNum,"%lu", shopid);
		utPltPutLoopVarF(psDbHead,"img",iNum,"%s",img);
		utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum);
		//utPltPutLoopVarF(psDbHead,"delimgs",iNum,"%s",delimgs);
		}
		pasDbCloseCursor(psCur);
	}
//	utPltShowDb(psDbHead);
	if(strlen(plateName)>0){
		char palteName1[128]="";
		sprintf(palteName1,"/%s/ncm_personals_attention.htm",plateName);
		// printf("palteName1 = %s\n",palteName1);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	}
	else{
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_personals_attention.htm");
	}
	// printf("ncmPersonals_Attention  end!!!!\n");
    return 0;	  
}


//个人收藏
int ncmPersonals_Collection(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead, char uid2[32], char delimg[16])
{
	// printf("ncmPersonals_Collection start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char uid[32] = "";
	char plateName[32] = "";
	char tsid[32] = "";
	char plateid[32] = "";
	// utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 4,
   				    "uid",        UT_TYPE_STRING,  sizeof(uid)-1,      		uid,
   				    "tsid",       UT_TYPE_STRING,  sizeof(tsid)-1,     	 	tsid,
   				    "plateid",    UT_TYPE_STRING,  sizeof(plateid)-1,       plateid,
					"plateName",  UT_TYPE_STRING,  sizeof(plateName)-1, 	plateName);
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);
	if(uid ==NULL){
		// printf("uid2=%s\n",uid2);
		strcpy(uid,uid2);
	}				  
	// printf("uid = %s\n",uid);		
	if(strcmp("del2",delimg)==0){
		//strcpy(delimgs,delimg);
		utPltPutVarF(psDbHead,"delimgs","%s",delimg);
	}
	int iReturn=0;
	char caTemp[256];
	ncPortalOnline *onlineinfo;
	onlineinfo = (ncPortalOnline *)malloc(sizeof(ncPortalOnline)+1);
    // printf("*******tsid = %s\n",tsid);
    if(strlen(tsid) > 0)
    {
		onlineinfo = ncSrvGetOnlineUserByStrTsid(psShmHead,tsid);
		// printf("*******caName = %s \n",onlineinfo->caName);
		sprintf(caTemp,"select aid, sid, imgpath from ncmshopcollect where flags = 0 and username='%s'", onlineinfo->caName);	
	}
	//sprintf(caTemp,"select aid, sid, imgpath from ncmshopcollect where flags = 0 and uid = %s ", uid);
	if(strlen(plateid) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and plateid = %d",atol(plateid));
	}	
	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
	
	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iNum=0;
		long aid =0;
		char img[128];
		long sid=0;
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,4,&aid,
							 UT_TYPE_LONG,4,&sid,
							 UT_TYPE_STRING,127,img))  || 1405==iReturn)
		{	
		iNum++;
		char _stime[40] = "";
		if(aid == 0){
			utPltPutLoopVarF(psDbHead,"discount",iNum,"%s", "discount_no");
		}
		else{
			utPltPutLoopVarF(psDbHead,"discount",iNum,"%s", "discount");
		}
		utPltPutLoopVarF(psDbHead,"aid",iNum,"%lu", aid);
		utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu", sid);
		utPltPutLoopVarF(psDbHead,"img",iNum,"%s",img);
		utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum);
		}
		pasDbCloseCursor(psCur);
	}
//	utPltShowDb(psDbHead);
	if(strlen(plateName)>0){
		char palteName1[128]="";
		sprintf(palteName1,"/%s/ncm_personals_collection.htm",plateName);
		// printf("palteName1 = %s\n",palteName1);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	}
	else{
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_personals_collection.htm");
	}
	// printf("ncmPersonals_Collection  end!!!!\n");
    return 0;	  
}
//首页广告商标详情
int ncmLogos_detail(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmLogos_detail start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char logoid[32] = "";
	
	// utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 1,
   				     "logoid",        UT_TYPE_STRING,  sizeof(logoid)-1,       logoid);
					 
					 
	// printf("logoid = %s\n",logoid);			 
	char caTemp[256];
	sprintf(caTemp,"select sid, name,floor, imgpath, shopid,type from ncmcomindex where flags = 0 and serno = %s ", logoid);	
  	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		int iNum=0;
		long id =0;
		char name[32];
		char floor[32];
		char img[128];
		long shopid=0;
		long type=0;
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_LONG,4,&id,
      	                    UT_TYPE_STRING,31,name,
							UT_TYPE_STRING,31,floor,
							UT_TYPE_STRING,127,img,
							UT_TYPE_LONG,4,&shopid,
							UT_TYPE_LONG,sizeof(type),&type))  || 1405==iReturn)
		{	
			iNum++;
			// printf("type=%lu\n",type);
			utPltPutVarF(psDbHead,"id","%lu", id);
			// printf("id=%lu\n",id);
			// printf("type=%lu\n",type);
			char *nameUTF = convert("GBK", "UTF-8", name);
			utPltPutVarF(psDbHead,"name","%s",nameUTF);
			char *floorUTF = convert("GBK", "UTF-8", floor);
			utPltPutVarF(psDbHead,"floor","%s",floorUTF);
			utPltPutVarF(psDbHead,"shopid","%lu",shopid);
		if(type == 4){
			utPltPutVarF(psDbHead,"vedio","%s",img);
		}
		else if(type == 2){
			utPltPutVarF(psDbHead,"img","%s",img);
		}
		
		}
		pasDbCloseCursor(psCur);
	}
//	utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_logos_detail.htm");
	// printf("ncmLogos_detail  end!!!!\n");
    return 0;	  
}

//首页广告
int ncmLogos(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmLogos start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char caTemp[256];
	int tReturn = 0;
	// tReturn = ncmShopUserOnlineFlag(psShmHead, iFd,psMsgHead);
	// printf("********test******tReturn = %d \n",tReturn);
	// utPltPutVarF(psDbHead,"user_online_flag","%d", tReturn);
	//long getShopId =0;
	//long getShopidLevel =0;
	
	//sprintf(caTemp,"select sid, name, imgpath, level,shopid from ncmcomindex where level != 0 and flags = 0 and type = 1 and shopid = %lu order by level ",getShopId);	
	//printf("caTemp=%s\n",caTemp);
   // psCur = pasDbOpenSql(caTemp,0);
	
	sprintf(caTemp,"select sid, name, imgpath, level,shopid from ncmcomindex where level != 0 and flags = 0 and type = 1 order by level ");	
  	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		
		int iReturn=0;
		int iNum=0;
		long id =0;
		char name[32]="";
		char img[128]="";
		long level=0;
		char serno[32]="";
		long shopid =0;
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_LONG,4,&id,
      	                    UT_TYPE_STRING,31,name,
							UT_TYPE_STRING,127,img,
							UT_TYPE_LONG,4,&level,
							UT_TYPE_LONG,4,&shopid))  || 1405==iReturn)
		{	
		
		//printf("name = %s\n",name);
		//printf("level = %lu\n",level);
		iNum++;
		char *nameUTF = convert("GBK", "UTF-8", name);
		
        if(level ==1){
			utPltPutVarF(psDbHead,"img1","%s", img);
			utPltPutVarF(psDbHead,"id1","%lu", id);
			utPltPutVarF(psDbHead,"name1","%s", nameUTF);
			//utPltPutVarF(psDbHead,"serno1","%s", serno);
			utPltPutVarF(psDbHead,"shopid1","%lu", shopid);
		}
		  if(level ==2){
			utPltPutVarF(psDbHead,"img2","%s", img);
			utPltPutVarF(psDbHead,"id2","%lu", id);
			utPltPutVarF(psDbHead,"name2","%s", nameUTF);
			//utPltPutVarF(psDbHead,"serno2","%s", serno);
			utPltPutVarF(psDbHead,"shopid2","%lu", shopid);	
		}
		  if(level ==3){
			utPltPutVarF(psDbHead,"img3","%s", img);
			utPltPutVarF(psDbHead,"id3","%lu", id);
		//utPltPutVarF(psDbHead,"serno3","%s", serno);
			utPltPutVarF(psDbHead,"name3","%s", nameUTF);
			utPltPutVarF(psDbHead,"shopid3","%lu", shopid);
		}
		  if(level ==4){
			utPltPutVarF(psDbHead,"img4","%s", img);
			utPltPutVarF(psDbHead,"id4","%lu", id);
		//	utPltPutVarF(psDbHead,"serno4","%s", serno);
			utPltPutVarF(psDbHead,"name4","%s", nameUTF);
			utPltPutVarF(psDbHead,"shopid4","%lu", shopid);
		}
		  if(level ==5){
		//  utPltPutVarF(psDbHead,"serno5","%s", serno);
			utPltPutVarF(psDbHead,"img5","%s", img);
			utPltPutVarF(psDbHead,"id5","%lu", id);
			utPltPutVarF(psDbHead,"name5","%s", nameUTF);
			utPltPutVarF(psDbHead,"shopid5","%lu", shopid);
		}
		  if(level ==6){
		//  utPltPutVarF(psDbHead,"serno6","%s", serno);
			utPltPutVarF(psDbHead,"img6","%s", img);
			utPltPutVarF(psDbHead,"id6","%lu", id);
			utPltPutVarF(psDbHead,"name6","%s", nameUTF);
			utPltPutVarF(psDbHead,"shopid6","%lu", shopid);
		}
		  if(level ==7){
		//  utPltPutVarF(psDbHead,"serno7","%s", serno);
			utPltPutVarF(psDbHead,"img7","%s", img);
			utPltPutVarF(psDbHead,"id7","%lu", id);
			utPltPutVarF(psDbHead,"name7","%s", nameUTF);
			utPltPutVarF(psDbHead,"shopid7","%lu", shopid);
		}
		  if(level ==8){
		//  utPltPutVarF(psDbHead,"serno8","%s", serno);
			utPltPutVarF(psDbHead,"img8","%s", img);
			utPltPutVarF(psDbHead,"id8","%lu", id);
			utPltPutVarF(psDbHead,"name8","%s", nameUTF);
			utPltPutVarF(psDbHead,"shopid8","%lu", shopid);
		}
		  if(level ==9){
		 // utPltPutVarF(psDbHead,"serno9","%s", serno);
			utPltPutVarF(psDbHead,"img9","%s", img);
			utPltPutVarF(psDbHead,"id9","%lu", id);
			utPltPutVarF(psDbHead,"name9","%s", nameUTF);
			utPltPutVarF(psDbHead,"shopid9","%lu", shopid);
		}
		  if(level ==10){
		//  utPltPutVarF(psDbHead,"serno10","%s", serno);
			utPltPutVarF(psDbHead,"img10","%s", img);
			utPltPutVarF(psDbHead,"id10","%lu", id);
			utPltPutVarF(psDbHead,"name10","%s", nameUTF);
			utPltPutVarF(psDbHead,"shopid10","%lu", shopid);
		} //printf("111111144411\n");
		  if(level ==11){
		//  utPltPutVarF(psDbHead,"serno11","%s", serno);
			utPltPutVarF(psDbHead,"img11","%s", img);
			utPltPutVarF(psDbHead,"id11","%lu", id);
			utPltPutVarF(psDbHead,"name11","%s", nameUTF);
			utPltPutVarF(psDbHead,"shopid11","%lu", shopid);
		}
		if(level ==12){
		//  utPltPutVarF(psDbHead,"serno11","%s", serno);
			utPltPutVarF(psDbHead,"img12","%s", img);
			utPltPutVarF(psDbHead,"id12","%lu", id);
			utPltPutVarF(psDbHead,"name12","%s", nameUTF);
			utPltPutVarF(psDbHead,"shopid12","%lu", shopid);
		}
		if(level == 13){
		// printf("level ==13\n");
		//  utPltPutVarF(psDbHead,"serno11","%s", serno);
			utPltPutVarF(psDbHead,"img13","%s", img);
			utPltPutVarF(psDbHead,"id13","%lu", id);
			utPltPutVarF(psDbHead,"name13","%s", nameUTF);
			utPltPutVarF(psDbHead,"shopid13","%lu", shopid);
		}
		//memset(name,0,sizeof(name));
		//free(nameUTF);
		//nameUTF = NULL;
		}

		pasDbCloseCursor(psCur);
	}
	//	utPltShowDb(psDbHead);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/shop_menus.htm");
		// printf("ncmLogos  end!!!!\n");
      
	  return 0;	  
}

//商品详情
int ncmProducts(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmProducts start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char sid[32] = "";
	char logoid[32] = "";
	char shopid[32] = "";
	char groupid[32] = "";
	char limit1 [32] ="";
	char limit2 [32] ="";
	char level [32] ="";
	char plateName [32] ="";
	char tsid [32] ="";
	char plateid [32] ="";
	// utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 11,
					"sid",       	   UT_TYPE_STRING,  sizeof(sid)-1,	        sid,
   				"logoid",        UT_TYPE_STRING,  sizeof(logoid)-1,       logoid,
					"shopid",        UT_TYPE_STRING,  sizeof(shopid)-1,       shopid,
					"groupid",       UT_TYPE_STRING,  sizeof(groupid)-1,      groupid,
					"level",         UT_TYPE_STRING,  sizeof(level)-1,        level,
					"limit1",        UT_TYPE_STRING,  sizeof(limit1)-1,       limit1,
					"limit2",        UT_TYPE_STRING,  sizeof(limit2)-1,       limit2,
					"tsid",     	   UT_TYPE_STRING,  sizeof(tsid)-1,      	  tsid,
					"plateid",       UT_TYPE_STRING,  sizeof(plateid)-1,      plateid,
					"plateName",     UT_TYPE_STRING,  sizeof(plateName)-1,    plateName
					);
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);
	utPltPutVarF(psDbHead,"limit1","%lu", atol(limit1));
	utPltPutVarF(psDbHead,"limit2","%lu", atol(limit2));
	utPltPutVarF(psDbHead,"shopid","%s", shopid);
		int tReturn = 0;
	tReturn = ncmShopUserOnlineFlag(psShmHead, iFd,psMsgHead);
	// printf("********test******tReturn = %d \n",tReturn);
	utPltPutVarF(psDbHead,"user_online_flag","%d", tReturn);	  
	// printf("logoid = %s\n",logoid);	
	char caTemp[1024];
	char sql[1024] = "";
	long lCount1=0;
	long iReturn=0;
	memset(sql, 0, sizeof(sql));
	snprintf(sql, sizeof(sql), "select count(*) from ncmcomodity where flags = 0 ");
	if(strlen(logoid) >0){
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sgroupid = %d",atol(logoid));
	}
	if(strlen(plateid) >0){
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %d",atol(plateid));
	}
	if(strlen(shopid) >0){
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and shgroupid = %d",atol(shopid));
	}
	if(strlen(groupid) >0 && strcmp(groupid,"undefined")!=0){
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and groupid = %d",atol(groupid));
	}
	if(strlen(sid) >0){
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sid = %d",atol(sid));
	}
 printf("sql = %s \n",sql);
	iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	// printf("iReturn = %d\n",iReturn);
	// printf("lCount1 = %d\n",lCount1);
	if(lCount1 >0){	
	//printf("limit1 = %s\n",limit1);		
	//printf("limit2 = %s\n",limit2);		
	//printf("strlen(limit1) = %d\n",strlen(limit1));
	//printf("strlen(limit2) = %d\n",strlen(limit2));
	
	snprintf(caTemp,sizeof(caTemp),"select sid, name, dispname, servicecode, sgroupid, groupid,shgroupid, imgpath, shopname, shimgpath, factory, type, url, floor, fldetail, fee,discount, code, tel, address, reserve, aid,active, level, descr, detail, systime from ncmcomodity where flags = 0  ");	
	
	if(strlen(shopid)>0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and shgroupid = %s ",shopid);
	}
	if(strlen(sid)>0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and sid = %s ",sid);
	}
	if(strlen(groupid)>0 && strcmp(groupid,"undefined")!=0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and groupid = %s ",groupid);
	}
	if(strlen(plateid) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and plateid = %d",atol(plateid));
	}
	if(strlen(limit1) >0 && strlen(limit2)>0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu",atol(limit1),atol(limit2));
	}
// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		int iNum=0;
		long id =0;
		char name[128]="";
		char dispname[128]="";
		char service_code[16]="";
		long sgroupid=0;
		long shgroupid=0;
		long groupid=0;
		char imgpath[128]="";
		char shopname[32]="";
		char shimgpath[128]="";
		char factory[32]="";
		char type[128]="";
		char url[255]="";
		char floor[32]="";
		char fldetail[255]="";
		double fee=0;
		double discount=0;
		char code[12]="";
		char tel[24]="";
		char address[255];
		long reserve=0;
		long aid=0;
		char active[64]="";
		long level=0;
		long plevel=0;
		char descr[255]="";
		char detail[1024]="";
		long systime=0;
		
		while(0==(iReturn = pasDbFetchInto(psCur,
							UT_TYPE_LONG,4,&id,
      	                    UT_TYPE_STRING,sizeof(name),name,
							UT_TYPE_STRING,sizeof(dispname),dispname,
							UT_TYPE_STRING,15,service_code,
							UT_TYPE_LONG,4,&sgroupid,
							UT_TYPE_LONG,4,&groupid,
							UT_TYPE_LONG,4,&shgroupid,
							UT_TYPE_STRING,127,imgpath,
							UT_TYPE_STRING,31,shopname,
							UT_TYPE_STRING,127,shimgpath,
							UT_TYPE_STRING,31,factory,
							UT_TYPE_STRING,127,type,
							UT_TYPE_STRING,254,url,
							UT_TYPE_STRING,127,floor,
							UT_TYPE_STRING,254,fldetail,
							UT_TYPE_FLOAT,4, &fee,
							UT_TYPE_FLOAT,4, &discount,
							UT_TYPE_STRING,11,code,
							UT_TYPE_STRING,23,tel,
							UT_TYPE_STRING,254,address,
							UT_TYPE_LONG,4,&reserve,
							UT_TYPE_LONG,4,&aid,
							UT_TYPE_STRING,63,active,
							UT_TYPE_LONG,4,&level,
							UT_TYPE_STRING,254,descr,
							UT_TYPE_STRING,1024,detail,
							UT_TYPE_LONG,4,&systime
							 ))  || 1405==iReturn)
		{	
			iNum++;
			char *shopnameUTF = convert("GBK", "UTF-8", shopname);	
			utPltPutLoopVarF(psDbHead,"shopname",iNum,"%s", shopnameUTF);
			char *shimgpathUTF = convert("GBK", "UTF-8", shimgpath);
			utPltPutLoopVarF(psDbHead,"shimgpath",iNum,"%s", shimgpathUTF);
			char *factoryUTF = convert("GBK", "UTF-8", factory);
			utPltPutLoopVarF(psDbHead,"factory",iNum,"%s", factoryUTF);
			char *typeUTF = convert("GBK", "UTF-8", type);
			utPltPutLoopVarF(psDbHead,"type",iNum,"%s", typeUTF);
			utPltPutLoopVarF(psDbHead,"url",iNum,"%s", url);
			char *floorUTF = convert("GBK", "UTF-8", floor);
			utPltPutLoopVarF(psDbHead,"floor",iNum,"%s", floorUTF);
			char *fldetailUTF = convert("GBK", "UTF-8", fldetail);
			utPltPutLoopVarF(psDbHead,"fldetail",iNum,"%s", fldetailUTF);
			utPltPutLoopVarF(psDbHead,"tel",iNum,"%lu", tel);
			char *addressUTF = convert("GBK", "UTF-8", address);
			utPltPutLoopVarF(psDbHead,"address",iNum,"%s", addressUTF);
			char *detailUTF = convert("GBK", "UTF-8", detail);
			utPltPutLoopVarF(psDbHead,"detail",iNum,"%s", detailUTF);
			
			char *nameUTF = convert("GBK", "UTF-8", name);
			utPltPutLoopVarF(psDbHead,"name",iNum,"%s",nameUTF);
			char *dispnameUTF = convert("GBK", "UTF-8", dispname);
			utPltPutLoopVarF(psDbHead,"dispname",iNum,"%s",dispnameUTF);
			char *imgpathUTF = convert("GBK", "UTF-8", imgpath);
			utPltPutLoopVarF(psDbHead,"imgpath",iNum,"%s",imgpathUTF);
			
			char *descrUTF = convert("GBK", "UTF-8", descr);
			utPltPutLoopVarF(psDbHead,"descr",iNum,"%s",descrUTF);
			
			utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",id);
			utPltPutLoopVarF(psDbHead,"sgroupid",iNum,"%lu",sgroupid);
			utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",groupid);
			utPltPutLoopVarF(psDbHead,"shgroupid",iNum,"%lu",shgroupid);
			utPltPutLoopVarF(psDbHead,"service_code",iNum,"%s",service_code);
			utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu",iNum);
			utPltPutLoopVarF(psDbHead,"fee",iNum,"%0.2f",fee);
			
			if(discount != 0){
			utPltPutLoopVarF(psDbHead,"discountclass",iNum,"%s","discountclass2");
			utPltPutLoopVarF(psDbHead,"feeclass",iNum,"%s","feeclass2");
			utPltPutLoopVarF(psDbHead,"discount",iNum,"%0.2f",(fee*discount));
			}
			else{
			utPltPutLoopVarF(psDbHead,"feeclass",iNum,"%s","feeclass");
			utPltPutLoopVarF(psDbHead,"discountclass",iNum,"%s","discountclass");
			utPltPutLoopVarF(psDbHead,"discount",iNum,"%0.2f",fee);
			}
		
			utPltPutLoopVarF(psDbHead,"reserve",iNum,"%lu",reserve);
			utPltPutLoopVarF(psDbHead,"aid",iNum,"%lu",aid);
			utPltPutLoopVarF(psDbHead,"level",iNum,"%lu",level);
			char *activeUTF = convert("GBK", "UTF-8", active);
			utPltPutLoopVarF(psDbHead,"active","%s", activeUTF);
		}
		pasDbCloseCursor(psCur);
	}
	}
//	utPltShowDb(psDbHead);
	if(strlen(plateName)>0){
		char palteName1[128]="";
		if(strlen(sid)>0){
			sprintf(palteName1,"/%s/ncm_levels_logos_shops_product.htm",plateName);
 printf("palteName1 = %s\n",palteName1);
			utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
		}else{
			sprintf(palteName1,"/%s/ncm_levels_logos_shops_products.htm",plateName);
	 printf("palteName1 = %s\n",palteName1);
			utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
		}
	}
	else{
		if(strlen(sid)>0){
			printf("11111\n");
			utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_levels_logos_shops_product.htm");
		}
		else{
			printf("222222\n");
			utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_levels_logos_shops_products.htm");
		}
	}
	// printf("ncmProducts  end!!!!\n");
    return 0;	  
}

//商店详情
int ncmShops(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmShops start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char shopid[32] = "";
	char logoid[32] ="";
	char groupid[32] ="";
	char dir[32] ="";
	char plateName[32] ="";
	char tsid[32] ="";
	char plateid[32] ="";
	char floor[32] ="";
	char url[255] ="";
	char address[255] ="";
//utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 10,
   				    "shopid",        UT_TYPE_STRING,  sizeof(shopid)-1,      	 shopid,
					"logoid",        UT_TYPE_STRING,  sizeof(logoid)-1,     	 logoid,
					"groupid",        UT_TYPE_STRING,  sizeof(groupid)-1,     	 groupid,
					"dir",        UT_TYPE_STRING,  sizeof(dir)-1,     	 dir,
					"tsid",        	 UT_TYPE_STRING,  sizeof(tsid)-1,      		 tsid,
					"plateid",       UT_TYPE_STRING,  sizeof(plateid)-1,     	 plateid,
					"address",       UT_TYPE_STRING,  sizeof(address)-1,     	 address,
					"url",       UT_TYPE_STRING,  sizeof(url)-1,     	 url,
					"floor",       UT_TYPE_STRING,  sizeof(floor)-1,     	 floor,
					"plateName",     UT_TYPE_STRING,  sizeof(plateName)-1,       plateName
					);
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);
	int tReturn = 0;
	tReturn = ncmShopUserOnlineFlag(psShmHead, iFd,psMsgHead);
	// printf("********test******tReturn = %d \n",tReturn);
	utPltPutVarF(psDbHead,"user_online_flag","%d", tReturn);  
	// printf("shopid = %s\n",shopid);	
	int iSum=0;
	int i =0;
	char caTemp[1024];
	ncmshoptype *shoptype;	
	shoptype = (ncmshoptype *)malloc(sizeof(ncmshoptype)*100+1); 
    //商店id      商店对应类别总数                                    
    shoptype = (ncmshoptype *)ncmgetShopgroupidByShopid(psShmHead,atoi(shopid),&iSum);                 //API1取商店中所有商品小类
    if(strlen(plateid) >0){
	snprintf(caTemp, sizeof(caTemp), "select sid, name from ncmcomtype where flags = 0 and plateid = %d and sid in (",  atol(plateid));

	for (i = 0; i < iSum; i++)
    {
		if(i == iSum-1){
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " %d ",shoptype[i].sgroupid);    
		}
		else{
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " %d, ",shoptype[i].sgroupid);    
		}
		// printf(" shopid = %d sgroupid = %d sid = %d \n",shoptype[i].shopid,shoptype[i].sgroupid,shoptype[i].sid);     
    }
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " ) ");  
	// printf("iSum = %d \n",iSum);
// printf("caTemp=%s\n",caTemp);
	psCur = pasDbOpenSql(caTemp,0);
	}
	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		int iNum=0;
		long sgroupid =0;
		char sgroupname[32]="";
	
		
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_LONG,4,&sgroupid,
      	                    UT_TYPE_STRING,31,sgroupname
							 ))  || 1405==iReturn)
		{	
			iNum++;
			utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",sgroupid);
			char *snameUTF = convert("GBK", "UTF-8", sgroupname);
			utPltPutLoopVarF(psDbHead,"sgroupname",iNum,"%s", snameUTF);
		}
		pasDbCloseCursor(psCur);
	}
	
	
	char sql[1024] = "";
	long lCount1=0;
	long iReturn=0;
	memset(sql, 0, sizeof(sql));
	snprintf(sql, sizeof(sql), "select count(*) from ncmshop where flags = 0 ");

	if(strlen(shopid) >0){
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and id = %lu",atol(shopid));
	}
	if(strlen(plateid) >0){
//		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %d",atol(plateid));
	}
	// printf("sql = %s \n",sql);
	iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	// printf("iReturn = %d\n",iReturn);
	// printf("lCount1 = %d\n",lCount1);
	
	snprintf(caTemp,sizeof(caTemp),"select id,name,service_code,sgroupid,imgpath,floor, code,tel, address,level,plevel,type,stype, descr, detail, url from ncmshop where flags = 0 ");
	if(strlen(shopid) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and id = %lu",atol(shopid));
	}
	if(strlen(plateid) >0){
//		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and plateid = %d",atol(plateid));
	}	
	if(strlen(floor) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and floor = '%s'",floor);
	}	
	if(strlen(url) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and url = '%s'",url);
	}
	if(strlen(address) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and address = '%s'",address);
	}
// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
	
	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		int iNum=0;
		long id =0;
		char name[128]="";
		char service_code[16]="";
		long sgroupid=0;
		char imgpath[128]="";
		char floor[32]="";
		char code[12]="";
		char tel[32]="";
		char address[255];
		long level=0;
		long plevel=0;
		long type=0;
		long stype=0;
		char descr[255]="";
		char detail[1024]="";
		char url[255]="";
		
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_LONG,4,&id,
      	                    UT_TYPE_STRING,sizeof(name),name,
							UT_TYPE_STRING,15,service_code,
							UT_TYPE_LONG,4,&sgroupid,
							UT_TYPE_STRING,127,imgpath,
							UT_TYPE_STRING,31,floor,
							UT_TYPE_STRING,11,code,
							UT_TYPE_STRING,31,tel,
							UT_TYPE_STRING,254,address,
							UT_TYPE_LONG,4,&level,
							UT_TYPE_LONG,4,&plevel,
							UT_TYPE_LONG,4,&type,
							UT_TYPE_LONG,4,&stype,
							UT_TYPE_STRING,254,descr,
							UT_TYPE_STRING,254,detail,
							UT_TYPE_STRING,254,url
							 ))  || 1405==iReturn)
		{	
			iNum++;
			utPltPutVarF(psDbHead,"id","%lu",id);
			char *nameUTF = convert("GBK", "UTF-8", name);
			utPltPutVarF(psDbHead,"name","%s", nameUTF);
			utPltPutVarF(psDbHead,"service_code","%s", service_code);
			utPltPutVarF(psDbHead,"sgroupid","%lu",sgroupid);
			char *floorUTF = convert("GBK", "UTF-8", floor);
			utPltPutVarF(psDbHead,"floor","%s", floorUTF);
			char *imgpathUTF = convert("GBK", "UTF-8", imgpath);
			utPltPutVarF(psDbHead,"imgpath","%s",imgpathUTF);
			utPltPutVarF(psDbHead,"code","%s", code);
			utPltPutVarF(psDbHead,"tel","%s", tel);
			char *addressUTF = convert("GBK", "UTF-8", address);
			utPltPutVarF(psDbHead,"address","%s", addressUTF);
			char *detailUTF = convert("GBK", "UTF-8", detail);
			utPltPutVarF(psDbHead,"detail","%s", detailUTF);
			char *descrUTF = convert("GBK", "UTF-8", descr);
			utPltPutVarF(psDbHead,"descr","%s",descrUTF);
			utPltPutVarF(psDbHead,"level","%lu",level);
			utPltPutVarF(psDbHead,"url","%s", url);
			utPltPutVarF(psDbHead,"logoid","%s", logoid);
		}
		pasDbCloseCursor(psCur);
	}
	
	snprintf(caTemp, sizeof(caTemp), "select count(*) from ncmcomodity where shgroupid = %s",  shopid);
	if(strlen(plateid) >0){
//		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and plateid = %d",atol(plateid));
	}
  //	 printf("caTemp=%s\n",caTemp);
	int totalProducts=0;
	pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, sizeof(totalProducts)-1, &totalProducts);
	utPltPutVarF(psDbHead,"totalProducts","%d", totalProducts);
	
//	utPltShowDb(psDbHead);
	if(strlen(plateName)>0){
	char palteName1[128]="";
	sprintf(palteName1,"/%s/ncm_levels_logos_shops.htm",plateName);
//printf("palteName1 = %s\n",palteName1);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	}else{
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_levels_logos_shops.htm");
	}
	// printf("ncmShops  end!!!!\n");
    return 0;	  
}
//添加关注
int ncm_Addattentions(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncm_Addattentions start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char mark[32] = "";
	char shopid[32] = "";
	//char sid[32] ="";
	char service_code[32] ="";
	char uid[32] ="";
	char aid[32] ="";
	char sid[32] ="";
	char integral[32] ="";
	char username[32] ="";
	char tsid[32] ="";
	char plateid[32] ="";
	char plateName[32] ="";
	char imgpath[128] ="";
	char decript[256] ="";
	unsigned long lTime = time(0);
	long flags =0;
//	 utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 13,
					"mark",			UT_TYPE_STRING,  sizeof(mark)-1,		mark,
   				    "shopid",       UT_TYPE_STRING,  sizeof(shopid)-1,      shopid,
					"logoid",       UT_TYPE_STRING,  sizeof(sid)-1,       	sid,
					"service_code",	UT_TYPE_STRING,  sizeof(service_code)-1,service_code,
					"uid",        	UT_TYPE_STRING,  sizeof(uid)-1,       	uid,
					"aid",        	UT_TYPE_STRING,  sizeof(aid)-1,      	aid,
					"integral",     UT_TYPE_STRING,  sizeof(integral)-1,    integral,
					"username",     UT_TYPE_STRING,  sizeof(username)-1,    username,
					"imgpath",      UT_TYPE_STRING,  sizeof(imgpath)-1,     imgpath,
					"tsid",      UT_TYPE_STRING,  sizeof(tsid)-1,     tsid,
					"plateid",      UT_TYPE_STRING,  sizeof(plateid)-1,     plateid,
					"plateName",      UT_TYPE_STRING,  sizeof(plateName)-1,     plateName,
					"decript",      UT_TYPE_STRING,  sizeof(decript)-1,     decript);
					  
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);	
	ncPortalOnline *onlineinfo;
	onlineinfo = (ncPortalOnline *)malloc(sizeof(ncPortalOnline)+1);
     printf("*******tsid = %s\n",tsid);
    if(strlen(tsid) > 0)
    {
     onlineinfo = ncSrvGetOnlineUserByStrTsid(psShmHead,tsid);
      printf("*******caName = %s \n",onlineinfo->caName);
	}
	char caTemp[1024];
	char sql[1024] = "";
	long lCount1=0;
	long iReturn=0;
	
	memset(sql, 0, sizeof(sql));
	snprintf(sql, sizeof(sql), "select count(*) from ncmshopattent where username='%s' and sid = %lu and shopid = %lu and plateid = %lu", onlineinfo->caName, atol(sid),atol(shopid),atol(plateid));
	// printf("sql = %s \n",sql);
	iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	// printf("iReturn = %d\n",iReturn);
	// printf("lCount1 = %d\n",lCount1);
	
	if(strcmp(mark,"del")==0){
		// printf("del\n");
		snprintf(caTemp,sizeof(caTemp),"update ncmshopattent set flags=1, lasttime=%lu where username = '%s' and sid=%lu and shopid=%lu",lTime,onlineinfo->caName,atol(sid),atol(shopid));	
		// printf("caTemp=%s\n",caTemp);	
		psCur = pasDbOpenSql(caTemp,0);
		// printf("del end\n");;
		ncmPersonals_Attention(psShmHead,iFd,psMsgHead,uid,"del2");
		return 0;
	}	
		
	if(lCount1 == 0){
		// printf("add\n");
		snprintf(caTemp,sizeof(caTemp),"insert into ncmshopattent (servicecode,username,uid,aid,sid,shopid,integral,decript,imgpath,addtime,plateid)values('%s','%s',%lu,%lu,%lu,%lu,%lu,'%s','%s',%lu,%lu)", service_code,onlineinfo->caName,atol(uid),atol(aid),atol(sid),atol(shopid),atol(integral),decript,imgpath,lTime,atol(plateid));
		// printf("caTemp=%s\n",caTemp);	
		psCur = pasDbOpenSql(caTemp,0);
		char caT[100];
		 strcpy(caT,"已关注");
		 char *tUTF = convert("GBK", "UTF-8", caT);
		 if(tUTF){
		   utPltPutVarF(psDbHead,"btn","%s", tUTF);
		   free(tUTF);
	   }
	   strcpy(caT,"成功加入关注");
		 tUTF = convert("GBK", "UTF-8", caT);
		 if(tUTF){
		  utPltPutVarF(psDbHead,"msg","%s", tUTF);
		  free(tUTF);
		 }
	}	
	else{
		snprintf(caTemp,sizeof(caTemp),"select flags from ncmshopattent where username = '%s' and shopid=%lu and sid = %lu and plateid=%lu", onlineinfo->caName, atol(shopid), atol(sid),atol(plateid));
		// printf("caTemp=%s\n",caTemp);
		psCur = pasDbOpenSql(caTemp,0);
		if(psCur == NULL){
		// printf("pscur is null\n");
		}
		if(psCur != NULL)
		{
			// printf("update\n");
			int iReturn=0;
			int iNum=0;
			
			while(0==(iReturn = pasDbFetchInto(psCur,
								 UT_TYPE_LONG,4,&flags
								 ))  || 1405==iReturn)
			{	
				// printf("flags=%lu\n",flags);
				iNum++;
			
			}
		}
		pasDbCloseCursor(psCur);
		memset(caTemp, 0, sizeof(caTemp));
		char caT[32];
		if(flags == 0)
		{
			// printf("update del\n");
		 strcpy(caT,"已关注");
		 char *tUTF = convert("GBK", "UTF-8", caT);
		 if(tUTF){
		   utPltPutVarF(psDbHead,"btn","%s", tUTF);
		   free(tUTF);
	   }
	   strcpy(caT,"成功加入关注");
		 tUTF = convert("GBK", "UTF-8", caT);
		 if(tUTF){
		  utPltPutVarF(psDbHead,"msg","%s", tUTF);
		  free(tUTF);
		 }
			if(strlen(plateName)>0){
			char palteName1[128]="";
			sprintf(palteName1,"/%s/ncm_levels_logos_shops_attentions.htm",plateName);
			// printf("palteName1 = %s\n",palteName1);
			utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
			}
			else{
				utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_levels_logos_shops_attentions.htm");
			}
			printf("update del end\n");
			}
		if(flags == 1){
			// printf("update add\n");
			snprintf(caTemp,sizeof(caTemp),"update ncmshopattent set flags=0, lasttime=%lu where username = '%s' and sid=%lu and shopid=%lu",lTime,onlineinfo->caName,atol(sid),atol(shopid));	
			// printf("caTemp=%s\n",caTemp);	
			psCur = pasDbOpenSql(caTemp,0);
		 strcpy(caT,"已关注");
		 char *tUTF = convert("GBK", "UTF-8", caT);
		 if(tUTF){
		   utPltPutVarF(psDbHead,"btn","%s", tUTF);
		   free(tUTF);
	   }
	   strcpy(caT,"成功加入关注");
		 tUTF = convert("GBK", "UTF-8", caT);
		 if(tUTF){
		  utPltPutVarF(psDbHead,"msg","%s", "成功加入关注");
		  free(tUTF);
		 }
			// printf("update add end\n");
		}
	}
	pasDbCloseCursor(psCur);
//	utPltShowDb(psDbHead);
	if(strlen(plateName)>0){
		char palteName1[128]="";
		sprintf(palteName1,"/%s/ncm_levels_logos_shops_attentions.htm",plateName);
		// printf("palteName1 = %s\n",palteName1);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	}
	else{
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_levels_logos_shops_attentions.htm");
	}
	// printf("ncm_Addattentions  end!!!!\n");
    return 0;	  
}

//收藏商品
int ncm_Addcollections(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncm_Addcollections start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char mark[32] = "";
	char nickname[32] = "";
	char membernum[32] = "";
	char service_code[32] ="";
	char uid[32] ="";
	char aid[32] ="";
	char sid[32] ="";
	char integral[32] ="";
	char username[32] ="";
	char plateName[32] ="";
	char imgpath[128] ="";
	char decript[256] ="";
	char tsid[256] ="";
	char plateid[256] ="";
	unsigned long lTime = time(0);
	long flags =0;
	// utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 14,
					"mark",        	UT_TYPE_STRING,  sizeof(mark)-1,       mark,
   				    "nickname",        UT_TYPE_STRING,  sizeof(nickname)-1,       nickname,
					"membernum",        UT_TYPE_STRING,  sizeof(membernum)-1,       membernum,
					"sid",        UT_TYPE_STRING,  sizeof(sid)-1,       sid,
					"service_code",        UT_TYPE_STRING,  sizeof(service_code)-1,   service_code,
					"uid",       	 UT_TYPE_STRING,  sizeof(uid)-1,       uid,
					"aid",       	 UT_TYPE_STRING,  sizeof(aid)-1,       aid,
					"integral",        UT_TYPE_STRING,  sizeof(integral)-1,       integral,
					"username",        UT_TYPE_STRING,  sizeof(username)-1,       username,
					"imgpath",        UT_TYPE_STRING,  sizeof(imgpath)-1,       imgpath,
					"decript",        UT_TYPE_STRING,  sizeof(decript)-1,       decript,
					"tsid",        UT_TYPE_STRING,  sizeof(tsid)-1,       tsid,
					"plateid",        UT_TYPE_STRING,  sizeof(plateid)-1,       plateid,
					"plateName",        UT_TYPE_STRING,  sizeof(plateName)-1,       plateName);
					
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);
	char caTemp[1024];
	char sql[1024] = "";
	long lCount1=0;
	long iReturn=0;
	 ncPortalOnline *onlineinfo;
	 onlineinfo = (ncPortalOnline *)malloc(sizeof(ncPortalOnline)+1);
    printf("*******tsid = %s\n",tsid);
    if(strlen(tsid) > 0)
    {
     onlineinfo = ncSrvGetOnlineUserByStrTsid(psShmHead,tsid);
     printf("*******caName = %s \n",onlineinfo->caName);
	}
	memset(sql, 0, sizeof(sql));
	snprintf(sql, sizeof(sql), "select count(*) from ncmshopcollect where username='%s' and sid = %lu and plateid = %lu", onlineinfo->caName, atol(sid),atol(plateid));
	// printf("sql = %s \n",sql);
	iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	// printf("iReturn = %d\n",iReturn);
	// printf("lCount1 = %d\n",lCount1);
	if(strcmp(mark,"del")==0){
		// printf("del\n");
		snprintf(caTemp,sizeof(caTemp),"update ncmshopcollect set flags=1, lasttime=%lu where username = '%s' and sid=%lu ",lTime,onlineinfo->caName,atol(sid));	
		// printf("caTemp=%s\n",caTemp);	
		psCur = pasDbOpenSql(caTemp,0);
		utPltPutVarF(psDbHead,"msg","%s", "删除成功！");
		pasDbCloseCursor(psCur);
//		utPltShowDb(psDbHead);	
		ncmPersonals_Collection(psShmHead,  iFd,psMsgHead, uid,"del2");
		//utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_personals_collection.htm");
		// printf("ncm_Addcollections  end!!!!\n");
		return 0;
	}	
	
	if(lCount1 == 0){
		// printf("add\n");
		snprintf(caTemp,sizeof(caTemp),"insert into ncmshopcollect (servicecode,username,nickname,membernum,uid,aid,sid,integral,decript,imgpath,addtime,plateid)values('%s','%s','%s','%s',%lu,%lu,%lu,%lu,'%s','%s',%lu,%lu)", service_code,onlineinfo->caName,nickname,membernum,atol(uid),atol(aid),atol(sid),atol(integral),decript,imgpath,lTime,atol(plateid));
		// printf("caTemp=%s\n",caTemp);	
		psCur = pasDbOpenSql(caTemp,0);
		utPltPutVarF(psDbHead,"sid","%lu", atol(sid));
		utPltPutVarF(psDbHead,"btn","%s", "? 已收藏");
		utPltPutVarF(psDbHead,"msg","%s", "? 收藏成功");
	}
	else{
		snprintf(caTemp,sizeof(caTemp),"select flags from ncmshopcollect where username = '%s' and sid= %lu and plateid= %lu", onlineinfo->caName, atol(sid),atol(plateid));
		// printf("caTemp=%s\n",caTemp);
		psCur = pasDbOpenSql(caTemp,0);
		if(psCur == NULL){
			// printf("pscur is null\n");	
		}
		if(psCur != NULL)
		{
			// printf("update\n");
			int iReturn=0;
			int iNum=0;
			
			while(0==(iReturn = pasDbFetchInto(psCur,
								 UT_TYPE_LONG,4,&flags
								 ))  || 1405==iReturn)
			{	
				// printf("flags=%lu\n",flags);
				iNum++;
			}
		}
		pasDbCloseCursor(psCur);
	
		memset(caTemp, 0, sizeof(caTemp));
		if(flags == 0)
		{
			// printf("update del\n");
			utPltPutVarF(psDbHead,"sid","%lu", atol(sid));
			utPltPutVarF(psDbHead,"btn","%s", "? 已收藏");
			utPltPutVarF(psDbHead,"msg","%s", "已经收藏此商品");
			if(strlen(plateName)>0){
				char palteName1[128]="";
				sprintf(palteName1,"/%s/ncm_products_collections.htm",plateName);
				// printf("palteName1 = %s\n",palteName1);
				utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
			}
			else{
				utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_products_collections.htm");
				}
			// printf("update del end\n");
			return 0;
		}
		if(flags == 1){
			// printf("update add\n");
			snprintf(caTemp,sizeof(caTemp),"update ncmshopcollect set flags=0, lasttime=%lu, aid = %lu, imgpath= '%s' where username = '%s' and sid=%lu ",lTime,atol(aid),imgpath,onlineinfo->caName,atol(sid));	
			// printf("caTemp=%s\n",caTemp);	
			psCur = pasDbOpenSql(caTemp,0);
			utPltPutVarF(psDbHead,"sid","%lu", atol(sid));
			utPltPutVarF(psDbHead,"btn","%s", "? 已收藏");
			utPltPutVarF(psDbHead,"msg","%s", "? 收藏成功");
			// printf("update add end\n");
		}
	}
	pasDbCloseCursor(psCur);
//	utPltShowDb(psDbHead);
	if(strlen(plateName)>0){
		char palteName1[128]="";
		sprintf(palteName1,"/%s/ncm_products_collections.htm",plateName);
		// printf("palteName1 = %s\n",palteName1);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	}
	else{
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_products_collections.htm");
	}
	// printf("ncm_Addcollections  end!!!!\n");
    return 0;	  
}


//收藏商品2
int ncm_Addcollections2(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncm_Addcollections2 start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char mark[32] = "";
	char nickname[32] = "";
	char membernum[32] = "";
	char service_code[32] ="";
	char uid[32] ="";
	char aid[32] ="";
	char sid[32] ="";
	char integral[32] ="";
	char username[32] ="";
	char imgpath[128] ="";
	char decript[256] ="";
	unsigned long lTime = time(0);
	long flags =0;
	// utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 11,
					"mark",        	UT_TYPE_STRING,  sizeof(mark)-1,       mark,
   				    "nickname",        UT_TYPE_STRING,  sizeof(nickname)-1,       nickname,
					"membernum",        UT_TYPE_STRING,  sizeof(membernum)-1,       membernum,
					"sid",        UT_TYPE_STRING,  sizeof(sid)-1,       sid,
					"service_code",        UT_TYPE_STRING,  sizeof(service_code)-1,   service_code,
					"uid",       	 UT_TYPE_STRING,  sizeof(uid)-1,       uid,
					"aid",       	 UT_TYPE_STRING,  sizeof(aid)-1,       aid,
					"integral",        UT_TYPE_STRING,  sizeof(integral)-1,       integral,
					"username",        UT_TYPE_STRING,  sizeof(username)-1,       username,
					"imgpath",        UT_TYPE_STRING,  sizeof(imgpath)-1,       imgpath,
					"decript",        UT_TYPE_STRING,  sizeof(decript)-1,       decript);
					  
	char caTemp[1024];
	char sql[1024] = "";
	long lCount1=0;
	long iReturn=0;
	
	memset(sql, 0, sizeof(sql));
	snprintf(sql, sizeof(sql), "select count(*) from ncmshopcollect where uid=%lu and sid =%lu ", atol(uid), atol(sid));
	// printf("sql = %s \n",sql);
	iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	// printf("iReturn = %d\n",iReturn);
	// printf("lCount1 = %d\n",lCount1);
	if(strcmp(mark,"del")==0){
		// printf("del\n");
		snprintf(caTemp,sizeof(caTemp),"update ncmshopcollect set flags=1, lasttime=%lu where uid = %lu and sid=%lu ",lTime,atol(uid),atol(sid));	
		// printf("caTemp=%s\n",caTemp);	
		psCur = pasDbOpenSql(caTemp,0);
		utPltPutVarF(psDbHead,"msg","%s", "删除成功！");
		pasDbCloseCursor(psCur);
//		utPltShowDb(psDbHead);	
		ncmPersonals_Collection(psShmHead,  iFd,psMsgHead, uid,"del2");
		//utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_personals_collection.htm");
		// printf("ncm_Addcollections  end!!!!\n");
		return 0;
	}	
	
	if(lCount1 == 0){
		// printf("add\n");
		snprintf(caTemp,sizeof(caTemp),"insert into ncmshopcollect (servicecode,username,nickname,membernum,uid,aid,sid,integral,decript,imgpath,addtime)values('%s','%s','%s','%s',%lu,%lu,%lu,%lu,'%s','%s',%lu)", service_code,username,nickname,membernum,atol(uid),atol(aid),atol(sid),atol(integral),decript,imgpath,lTime);
		// printf("caTemp=%s\n",caTemp);	
		psCur = pasDbOpenSql(caTemp,0);
		utPltPutVarF(psDbHead,"sid","%lu", atol(sid));
		utPltPutVarF(psDbHead,"btn","%s", "? 收藏");
		utPltPutVarF(psDbHead,"msg","%s", "? 收藏成功");
	}
	else{
		snprintf(caTemp,sizeof(caTemp),"select flags from ncmshopcollect where uid = %lu and sid= %lu", atol(uid), atol(sid));
		// printf("caTemp=%s\n",caTemp);
		psCur = pasDbOpenSql(caTemp,0);
		if(psCur == NULL){
			// printf("pscur is null\n");	
		}
		if(psCur != NULL)
		{
			// printf("update\n");
			int iReturn=0;
			int iNum=0;
			
			while(0==(iReturn = pasDbFetchInto(psCur,
								 UT_TYPE_LONG,4,&flags
								 ))  || 1405==iReturn)
			{	
				// printf("flags=%lu\n",flags);
				iNum++;
			}
		}
		pasDbCloseCursor(psCur);
	
		memset(caTemp, 0, sizeof(caTemp));
		if(flags == 0)
		{
			// printf("update del\n");
			utPltPutVarF(psDbHead,"sid","%lu", atol(sid));
			utPltPutVarF(psDbHead,"btn","%s", "? 收藏");
			utPltPutVarF(psDbHead,"msg","%s", "已经收藏此商品");
			utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_products_collections.htm");
			// printf("update del end\n");
			return 0;
		}
		if(flags == 1){
			// printf("update add\n");
			snprintf(caTemp,sizeof(caTemp),"update ncmshopcollect set flags=0, lasttime=%lu, aid = %lu, imgpath= '%s' where uid = %lu and sid=%lu ",lTime,atol(aid),imgpath,atol(uid),atol(sid));	
			// printf("caTemp=%s\n",caTemp);	
			psCur = pasDbOpenSql(caTemp,0);
			utPltPutVarF(psDbHead,"sid","%lu", atol(sid));
			utPltPutVarF(psDbHead,"btn","%s", "? 收藏");
			utPltPutVarF(psDbHead,"msg","%s", "? 收藏成功");
			// printf("update add end\n");
		}
	}
	pasDbCloseCursor(psCur);
//	utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_products_collections.htm");
	// printf("ncm_Addcollections2  end!!!!\n");
    return 0;	  
}



//收藏优惠券
int ncm_Addcoupons(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncm_Addcoupons start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char mark[32] = "";
	char aname[32] = "";
	char feetime[32] = "";
	char effetime[32] ="";
	char id[32] ="";
	char uid[32] ="";
	char aid[32] ="";
	char cid[32] ="";
	char usermac[32] ="";
	char sid[32] ="";
	char addtime[32] ="";
	char username[32] ="";
	char imgpath[128] ="";
	char sname[64] ="";
	char servicecode[32] ="";
	char serno[32] ="";
	char counum[32] ="";
	
	unsigned long lTime = time(0);
	long flags =0;
	// utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 12,
					"mark",       	UT_TYPE_STRING,  sizeof(mark)-1,		mark,
					"id",     		UT_TYPE_STRING,  sizeof(id)-1,       	id,
					"servicecode",  UT_TYPE_STRING,  sizeof(servicecode)-1, servicecode,
					"username",     UT_TYPE_STRING,  sizeof(username)-1,    username,
					"aid",     		UT_TYPE_STRING,  sizeof(aid)-1,       	aid,
   				    "aname",		UT_TYPE_STRING,  sizeof(aname)-1,		aname,
					"sname",        UT_TYPE_STRING,  sizeof(sname)-1,       sname,
					"uid",     		UT_TYPE_STRING,  sizeof(uid)-1,       	uid,
					"sid",      	UT_TYPE_STRING,  sizeof(sid)-1,			sid,
					"cid",        	UT_TYPE_STRING,  sizeof(cid)-1,			cid,
					"imgpath",      UT_TYPE_STRING,  sizeof(imgpath)-1,     imgpath,
					"counum",      UT_TYPE_STRING,  sizeof(counum)-1,     counum
					
					);
					  
	char caTemp[1024];
	char sql[1024] = "";
	long lCount1=0;
	long iReturn=0;
	
	memset(sql, 0, sizeof(sql));
	snprintf(sql, sizeof(sql), "select count(*) from ncmcouponuse where uid=%lu and cid =%lu ", atol(uid), atol(cid));
	// printf("sql = %s \n",sql);
	iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	// printf("iReturn = %d\n",iReturn);
	// printf("lCount1 = %d\n",lCount1);
	if(strcmp(mark,"del")==0){
		// printf("del\n");
		snprintf(caTemp,sizeof(caTemp),"update ncmcouponuse set flags = 1 where uid = %lu and id=%lu ",atol(uid),atol(id));	
		// printf("caTemp=%s\n",caTemp);	
		psCur = pasDbOpenSql(caTemp,0);
		utPltPutVarF(psDbHead,"msg","%s", "删除成功！");
		pasDbCloseCursor(psCur);
//		utPltShowDb(psDbHead);
		ncmPersonals_Coupon(psShmHead,  iFd,psMsgHead, uid,"del2");
		//utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_personals_collection.htm");
		// printf("ncm_Addcollections  end!!!!\n");
		return 0;
	}	
	
	if(lCount1 == 0){
		if(atol(counum)>0){
			// printf("add\n");
			snprintf(caTemp,sizeof(caTemp),"update ncmcoupon set counum=%lu where cid = %lu",atol(counum)-1,atol(cid));	
			// printf("caTemp=%s\n",caTemp);	
			psCur = pasDbOpenSql(caTemp,0);
			pasDbCloseCursor(psCur);
			snprintf(caTemp,sizeof(caTemp),"insert into ncmcouponuse (servicecode,username,aid,aname,serno,usermac,sname,feetime,uid,sid,cid,imgpath,systime)values('%s','%s',%lu,'%s','%s','%s','%s',%lu,%lu,%lu,%lu,'%s',%lu)",servicecode,username,atol(aid),aname,serno,usermac,sname,feetime,atol(uid),atol(sid),atol(cid),imgpath,lTime);
			// printf("caTemp=%s\n",caTemp);	
			psCur = pasDbOpenSql(caTemp,0);
			utPltPutVarF(psDbHead,"sid","%lu", atol(sid));
			utPltPutVarF(psDbHead,"btn","%s", "? 已收藏");
			utPltPutVarF(psDbHead,"msg","%s", "? 收藏成功");
		}
		else{
		utPltPutVarF(psDbHead,"msg","%s", "此优惠劵已被下载完");
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_levels_logos_shops_attentions.htm");
		// printf("no coupon end\n");
		return 0;
		}

	}
	else{
		snprintf(caTemp,sizeof(caTemp),"select flags from ncmcouponuse where uid = %lu and cid= %lu", atol(uid), atol(cid));
		// printf("caTemp=%s\n",caTemp);
		psCur = pasDbOpenSql(caTemp,0);
		if(psCur == NULL){
		// printf("pscur is null\n");
		}
		if(psCur != NULL)
		{
			// printf("update\n");
			int iReturn=0;
			int iNum=0;
			
			while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,4,&flags
							 ))  || 1405==iReturn)
			{	
				// printf("flags=%lu\n",flags);
				iNum++;
			
			}
		}
		pasDbCloseCursor(psCur);
		memset(caTemp, 0, sizeof(caTemp));
		if(flags == 0)
		{
			// printf("update del\n");
			utPltPutVarF(psDbHead,"sid","%lu", atol(sid));
			utPltPutVarF(psDbHead,"btn","%s", "? 已收藏");
			utPltPutVarF(psDbHead,"msg","%s", "已经收藏此优惠劵");
			utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_levels_logos_shops_attentions.htm");
			// printf("update del end\n");
			return 0;
		}
		if(flags == 1){
			// printf("update add\n");
			char sql[124] = "";
			long systime1=0;
			long iReturn=0;
			memset(sql, 0, sizeof(sql));
			snprintf(sql, sizeof(sql), "select systime from ncmcouponuse where uid = %lu and cid=%lu order by systime desc ",atol(uid),atol(cid));
			// printf("sql = %s \n",sql);
			iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &systime1);
			// printf("iReturn = %d\n",iReturn);
			// printf("lCount1 = %d\n",systime1);
			if(lTime-systime1 > COUPONDOWNTIME ){
				snprintf(caTemp,sizeof(caTemp),"insert into ncmcouponuse (servicecode,username,aid,aname,serno,usermac,sname,feetime,uid,sid,cid,imgpath,systime)values('%s','%s',%lu,'%s','%s','%s','%s',%lu,%lu,%lu,%lu,'%s',%lu)",servicecode,username,atol(aid),aname,serno,usermac,sname,feetime,atol(uid),atol(sid),atol(cid),imgpath,lTime);
				// printf("caTemp=%s\n",caTemp);	
				psCur = pasDbOpenSql(caTemp,0);
				utPltPutVarF(psDbHead,"sid","%lu", atol(sid));
				utPltPutVarF(psDbHead,"btn","%s", "? 已收藏");
				utPltPutVarF(psDbHead,"msg","%s", "? 收藏成功");
			}
			else{
				utPltPutVarF(psDbHead,"msg","%s", "已经收藏过此优惠劵，请等待片刻");
				utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_levels_logos_shops_attentions.htm");
				// printf("wait for download end\n");
				return 0;
			}
			// printf("download another one end\n");
		}
	}
	pasDbCloseCursor(psCur);
//	utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_levels_logos_shops_attentions.htm");
	// printf("ncm_Addcoupons  end!!!!\n");
    return 0;	  
}

//楼层商标
int ncmLevels_Logos(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmLevels_Logos start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	// utMsgPrintMsg(psMsgHead);
	char name[32] = "";
	char tsid[32] = "";
	char plateid[32] = "";
	char plateName[32] = "";
	utMsgGetSomeNVar(psMsgHead, 4,
   				    "name",        UT_TYPE_STRING,  sizeof(name)-1,       name,
					"tsid",        	UT_TYPE_STRING,  sizeof(tsid)-1,      	 tsid,
					"plateid",       UT_TYPE_STRING,  sizeof(plateid)-1,     	 plateid,
					"plateName",     UT_TYPE_STRING,  sizeof(plateName)-1,       plateName);
			
	char *nameGBK = convert("UTF-8", "GBK", name);			
	char caTemp[256] = "";
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);
	sprintf(caTemp,"select sid, name, imgpath, level, shopid, plevel from ncmcomindex where type=2 and floor = '%s' ",nameGBK);
	if(strlen(plateid) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and plateid = %d",atol(plateid));
	}
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " order by level");
  	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn = 0;
		int iNum = 0;
		long id = 0;
		long level = 0;
		char name[32] = "";
		char img[128] = "";
		long shopid = 0;
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_LONG,4,&id,
      	                    UT_TYPE_STRING,31,name,
							UT_TYPE_STRING,127,img,
							UT_TYPE_LONG,4,&level,
							UT_TYPE_LONG,4,&shopid
							  ))  || 1405==iReturn)
		{	
			iNum++;
			char *nameUTF = convert("GBK", "UTF-8", name);
			utPltPutLoopVarF(psDbHead,"img",iNum,"%s", img);
			utPltPutLoopVarF(psDbHead,"id",iNum,"%lu", id);
			utPltPutLoopVarF(psDbHead,"name",iNum,"%s", nameUTF);
			utPltPutLoopVarF(psDbHead,"shopid",iNum,"%lu", shopid);
			utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu", iNum);
		}
		pasDbCloseCursor(psCur);
	}
//	utPltShowDb(psDbHead);
	if(strlen(plateName)>0){
	char palteName1[128]="";
	sprintf(palteName1,"/%s/ncm_levels_logos.htm",plateName);
	//printf("palteName1 = %s\n",palteName1);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	}else{
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_levels_logos.htm");
	}
	// printf("ncmLevels_Logos  end!!!!\n");
    return 0;	  
}

//楼层商标推荐
int ncmLevels_Logos_Recommend(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmLevels_Logos_Recommend start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	// utMsgPrintMsg(psMsgHead);
	char name[32] = "";
	char tsid[32] = "";
	char plateid[32] = "";
	char plateName[32] = "";
	utMsgGetSomeNVar(psMsgHead, 4,
   				    "name",        UT_TYPE_STRING,  sizeof(name)-1,       name,
					"tsid",        	UT_TYPE_STRING,  sizeof(tsid)-1,      	 tsid,
					"plateid",       UT_TYPE_STRING,  sizeof(plateid)-1,     	 plateid,
					"plateName",     UT_TYPE_STRING,  sizeof(plateName)-1,       plateName);
			
	char *nameGBK = convert("UTF-8", "GBK", name);			
	char caTemp[256] = "";
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);
	sprintf(caTemp,"select sid, name, imgpath, level, shopid, plevel from ncmcomindex where type=2 and level != 0 ");
	if(strlen(plateid) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and plateid = %d",atol(plateid));
	}
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " order by level");
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit 20");
  	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn = 0;
		int iNum = 0;
		long id = 0;
		long level = 0;
		char name[32] = "";
		char img[128] = "";
		long shopid = 0;
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_LONG,4,&id,
      	                    UT_TYPE_STRING,31,name,
							UT_TYPE_STRING,127,img,
							UT_TYPE_LONG,4,&level,
							UT_TYPE_LONG,4,&shopid
							  ))  || 1405==iReturn)
		{	
			iNum++;
			char *nameUTF = convert("GBK", "UTF-8", name);
			utPltPutLoopVarF(psDbHead,"img",iNum,"%s", img);
			utPltPutLoopVarF(psDbHead,"id",iNum,"%lu", id);
			utPltPutLoopVarF(psDbHead,"name",iNum,"%s", nameUTF);
			utPltPutLoopVarF(psDbHead,"shopid",iNum,"%lu", shopid);
			utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu", iNum);
		}
		pasDbCloseCursor(psCur);
	}
	
	
	snprintf(caTemp,sizeof(caTemp),"select id, plevel, xid,imgpath from ncmactive where flags = 0 and type = 1" );	

	if(strlen(plateid) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and plateid = %d",atol(plateid));
	}
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " order by plevel");
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit 5");
	// printf("caTemp=%s\n",caTemp);
	
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
	// printf("pscur is null\n");
	// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
	int iReturn=0;
   	int iNum=0;
	long id =0;
	long plevel =0;
	long xid =0;
	char imgpath[128];
	while(0==(iReturn = pasDbFetchInto(psCur,
      	                     UT_TYPE_LONG,4,&id,
      	                     UT_TYPE_LONG,4,&plevel,
							 UT_TYPE_LONG,4,&xid,
							 UT_TYPE_STRING,127,imgpath))  || 1405==iReturn)
	 {	
		iNum++;
		utPltPutLoopVarF(psDbHead,"id",iNum,"%lu", id);
		utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu", iNum);
		utPltPutLoopVarF(psDbHead,"plevel",iNum,"%lu",plevel);
		utPltPutLoopVarF(psDbHead,"xid",iNum,"%lu",xid);
		char *imgpathUTF = convert("GBK", "UTF-8", imgpath);
		utPltPutLoopVarF(psDbHead,"imgpath",iNum,"%s",imgpathUTF);
		}
		pasDbCloseCursor(psCur);
	}
	
	
//	utPltShowDb(psDbHead);
	if(strlen(plateName)>0){
	char palteName1[128]="";
	sprintf(palteName1,"/%s/ncm_levels_logos_recommend.htm",plateName);
	//printf("palteName1 = %s\n",palteName1);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	}else{
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_levels_logos_recommend.htm");
	}
	// printf("ncmLevels_Logos_Recommend  end!!!!\n");
    return 0;	  
}



//豫园首页
int ncm_yuyuan_home(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//	printf("ncm_yuyuan_home start!!!!\n");
	 pasDbCursor *psCur;
	// utPltDbHead *psDbHead;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	
	char logoid[32] = "";

	char limit1 [32] ="";
	char limit2 [32] ="";

	char tsid [32] ="";
	char caplateid [32] ="";
	char title [128] ="";
	char update [32] ="";
	int plateid = 0;
	char getarg[128];
	
	
	  int iReturn=0;
		int iNum=0;
		long id =0;
		char name[32]="";
		char img[128]="";
		long level=0;
		char serno[32]="";
		long shopid =0;
		char label[128]="";
	
	struct portimg_s{
		char name[32];
		char img[128];
		unsigned long level;
		unsigned long shopid;
	};
	struct portimg_s psImg[8];
//	printf("pid=%d\n",getpid());

	memset(&psImg,0,8*sizeof(struct portimg_s));
	
	strcpy(psImg[0].name,"default");
	strcpy(psImg[0].img,"/images/ShopImages/shopimages/default.png");
	psImg[0].level=5;
	
	strcpy(psImg[1].name,"default");
	strcpy(psImg[1].img,"/images/ShopImages/shopimages/default.png");
	psImg[1].level=6;
  strcpy(psImg[2].name,"default");
	strcpy(psImg[2].img,"/images/ShopImages/shopimages/default.png");
	psImg[1].level=7;
	
	

//utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 7,
   				 "logoid",      UT_TYPE_STRING,  sizeof(logoid)-1,     logoid,
					 "limit1",      UT_TYPE_STRING,  sizeof(limit1)-1,     limit1,
					 "limit2",      UT_TYPE_STRING,  sizeof(limit2)-1,     limit2,
					 "plateid",     UT_TYPE_STRING,  sizeof(caplateid)-1,  caplateid,
					 "tsid",      	UT_TYPE_STRING,  sizeof(tsid)-1,       tsid,
					 "update",      	UT_TYPE_STRING,  sizeof(update)-1,    update,
					 "getarg",        UT_TYPE_STRING,sizeof(getarg)-1,getarg );

	char caTemp [256] ="";
	plateid = 4;
	//strcpy(tsid,"1234567");
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	char caIp[24] = "", caPort[10] = "";
	strcpy(caIp,utComGetVar_sd(psShmHead,"AdminIp", "192.168.20.168"));
	strcpy(caPort,utComGetVar_sd(psShmHead,"AdminPort", "9080"));
	utPltPutVarF(psDbHead,"ipurl","http://%s:%s", caIp,caPort);	
	utPltPutVar(psDbHead,"ip",caIp);
	utPltPutVar(psDbHead,"port",caPort);
	utPltPutVar(psDbHead,"getarg",getarg);	
	//是否在线
	int tReturn = 0;
	tReturn = ncmShopUserOnlineFlag(psShmHead, iFd,psMsgHead);
	// printf("********test******tReturn = %d \n",tReturn);
	utPltPutVarF(psDbHead,"user_online_flag","%d", tReturn);
	unsigned int lplateid = ncmGetPlateidByTsid(psShmHead,atoll(tsid));
printf(" lplateid = %d \n", lplateid);

	utPltPutVarF(psDbHead,"plateid","%d", lplateid);	
		
	
	unsigned getShopId =0;
	long getShopidLevel =0;
	int i=0;
	//getShopId=5;
	getShopId = ncSrvGetCid(psShmHead,psMsgHead);
//printf("getShopId = %lu \n",getShopId);


	sprintf(caTemp,"select sid, name, imgpath, level,shopid,label from ncmcomindex where level >4 and level<8  and type = 1 and plateid = %d ",lplateid);
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " group by level order by level limit 0,10");
//	printf("caTemp=%s\n",caTemp);
  psCur = pasDbOpenSql(caTemp,0);
  char *imgpathUTF,*nameUTF;
	if(psCur != NULL)
	{
	
		while(0==(iReturn = pasDbFetchInto(psCur,
      	      UT_TYPE_LONG,4,&id,
      	      UT_TYPE_STRING,31,name,
							UT_TYPE_STRING,127,img,
							UT_TYPE_LONG,4,&level,
							UT_TYPE_LONG,4,&shopid,
							UT_TYPE_STRING,127, label
							))  || 1405==iReturn)
		{	
       if(level==5){
       		 imgpathUTF = convert("GBK", "UTF-8", img);
	         if(imgpathUTF){
	           strcpy(psImg[0].img,imgpathUTF);
	           free(imgpathUTF);
	         }
	         nameUTF = convert("GBK", "UTF-8", name);
	         if(nameUTF){
	           strcpy(psImg[0].name,nameUTF);
	           free(nameUTF);
	         }	         
      }
		
		       if(level==6){
       		 imgpathUTF = convert("GBK", "UTF-8", img);
	         if(imgpathUTF){
	           strcpy(psImg[1].img,imgpathUTF);
	           free(imgpathUTF);
	         }
	         nameUTF = convert("GBK", "UTF-8", name);
	         if(nameUTF){
	           strcpy(psImg[1].name,nameUTF);
	           free(nameUTF);
	         }	         
           }
				   if(level==7){
       		 imgpathUTF = convert("GBK", "UTF-8", img);
	         if(imgpathUTF){
	           strcpy(psImg[2].img,imgpathUTF);
	           free(imgpathUTF);
	         }
	         nameUTF = convert("GBK", "UTF-8", name);
	         if(nameUTF){
	           strcpy(psImg[2].name,nameUTF);
	           free(nameUTF);
	         }	         
           }
		}
		pasDbCloseCursor(psCur);
   }

  iNum=0;

	sprintf(caTemp,"select sid, name, imgpath, level,shopid,label from ncmcomindex where level != 0 and type = 1 and plateid = %d ",lplateid);
	if(getShopId !=0){
		sprintf(caTemp+strlen(caTemp)," and shopid in (select id from ncmshop where sgroupid=%lu)",getShopId);
	}
//	if(strlen(caLabel)>0){
//		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and label = substring('%s',3,5) ",caLabel);
//	}
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " group by level order by level limit 0,10");
	
// printf("caTemp=%s\n",caTemp);
long iNum_f=0;
  psCur = pasDbOpenSql(caTemp,0);
	if(psCur != NULL)
	{
		

		while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_LONG,4,&id,
      	                    UT_TYPE_STRING,31,name,
							UT_TYPE_STRING,127,img,
							UT_TYPE_LONG,4,&level,
							UT_TYPE_LONG,4,&shopid,
							UT_TYPE_STRING,127, label
							))  || 1405==iReturn)
		{	

		iNum++;
      
	
		utPltPutLoopVarF(psDbHead,"id",iNum,"%lu", id);
		utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu", iNum);
		utPltPutLoopVarF(psDbHead,"level",iNum,"%lu",level);
		utPltPutLoopVarF(psDbHead,"shopid",iNum,"%lu",shopid);
		char *imgpathUTF = convert("GBK", "UTF-8", img);
		if(imgpathUTF){
		  utPltPutLoopVarF(psDbHead,"img",iNum,"%s",imgpathUTF);
		
	  }
    nameUTF = convert("GBK", "UTF-8", name);
		if(level<=4){
		   iNum_f++;
		   utPltPutLoopVarF(psDbHead,"imgslider",iNum_f,"%s",imgpathUTF);
		   utPltPutLoopVarF(psDbHead,"iNumslider",iNum_f,"%lu",iNum);		
		}

		
		if(level==5){
			 strcpy(psImg[0].img,imgpathUTF);
			 strcpy(psImg[0].name,nameUTF);
		}
		if(level==6){
			 strcpy(psImg[1].img,imgpathUTF);
			 strcpy(psImg[1].name,nameUTF);
		}
		if(level==7){
			 strcpy(psImg[2].img,imgpathUTF);
			 strcpy(psImg[2].name,nameUTF);
		}
		
		if(imgpathUTF){
		 free(imgpathUTF);
		}
		if(nameUTF){
			free(nameUTF);
		}
		
		else if(level>4&&level<=8){
			
			 if(level==5){
			 	 utPltPutLoopVarF(psDbHead,"imgslider_s",iNum-4,"%s",imgpathUTF);
			 	 utPltPutLoopVarF(psDbHead,"iNumslider_s",iNum-4,"%lu",iNum);	
			 }
			if(iNum>5)
				{
				utPltPutLoopVar(psDbHead,"dh",iNum-4,",");	
				}
			 utPltPutLoopVarF(psDbHead,"imgslider_s",iNum-4,"%s",imgpathUTF);
		   utPltPutLoopVarF(psDbHead,"iNumslider_s",iNum-4,"%lu",iNum);	
		  
		}
	
		}
		utPltPutVarF(psDbHead,"TotRec","%d",iNum);
		pasDbCloseCursor(psCur);
	}
	
	
	for(i=0;i<3;i++){
		if(i>0){
			utPltPutLoopVar(psDbHead,"dh",i+1,",");
		}
		 utPltPutLoopVarF(psDbHead,"imgslider_s",i+1,"%s",psImg[i].img);
		 utPltPutLoopVarF(psDbHead,"iNumslider_s",i+1,"%lu",i+5);
  }
//		utPltShowDb(psDbHead);
			char platepath[128]="";
			sprintf(caTemp,"select platepath from portalplate where shopplateid = %d ",lplateid);

			pasDbOneRecord(caTemp, 0, UT_TYPE_STRING, sizeof(platepath)-1, platepath);
 //    printf("platepath=%s\n",platepath);
  if(strlen(platepath)==0){
//  	  sprintf(caTemp,"select platepath from portplate where status = 0 ");
//			pasDbOneRecord(caTemp, 0, UT_TYPE_STRING, sizeof(platepath)-1, platepath);
      strcpy(platepath,"/genal/shop_menus.htm");
  }
			utPltOutToHtml(iFd,psMsgHead,psDbHead,platepath);

//     printf("\n ncm_yuyuan_home end \n");
	  return 0;	  
}

/*
//豫园首页
int ncm_yuyuan_home(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	printf("ncm_yuyuan_home start!!!!\n");
	 pasDbCursor *psCur;
	// utPltDbHead *psDbHead;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	printf("****ncm_yuyuan_home start!!!!\n");
	char logoid[32] = "";
	char shopid[32] = "";
	char limit1 [32] ="";
	char limit2 [32] ="";
	char level [32] ="";
	char tsid [32] ="";
	char caplateid [32] ="";
	char title [128] ="";
	int plateid = 0;
	utMsgPrintMsg(psMsgHead);printf("***2*ncm_yuyuan_home start!!!!\n");
	utMsgGetSomeNVar(psMsgHead, 7,
   				     "logoid",      UT_TYPE_STRING,  sizeof(logoid)-1,     logoid,
					 "shopid",      UT_TYPE_STRING,  sizeof(shopid)-1,     shopid,
					 "level",       UT_TYPE_STRING,  sizeof(level)-1,      level,
					 "limit1",      UT_TYPE_STRING,  sizeof(limit1)-1,     limit1,
					 "limit2",      UT_TYPE_STRING,  sizeof(limit2)-1,     limit2,
					 "plateid",   UT_TYPE_STRING,  sizeof(caplateid)-1,  caplateid,
					 "tsid",      	UT_TYPE_STRING,  sizeof(tsid)-1,       tsid);
printf("***3*ncm_yuyuan_home start!!!!\n");
	plateid = 4;
//	strcpy(tsid,"1234567");
	utPltPutVarF(psDbHead,"tsid","%s", tsid);				  
	utPltPutVarF(psDbHead,"plateid","%d", plateid);				  
	char caTemp[256];
	
	int tReturn = 0;
//	  tReturn = ncmShopUserOnlineFlag(psShmHead, iFd,psMsgHead);
//	  printf("********test******tReturn = %d \n",tReturn);
	  utPltPutVarF(psDbHead,"user_online_flag","%d", tReturn);
	sprintf(caTemp,"select sid from portplate where status = 0 ");
	pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, sizeof(plateid)-1, &plateid);
	printf("plateid=%d\n",plateid);
	struct selfShop
    {
		int sid;
		char name[32];
		char img[128];
		int shopid;
		int level;
	};
	struct selfShop selfShop[20] = {
	};
	struct selfShop *p;
	p=selfShop;
	int getShopId =0;
	long getShopidLevel =0;
	int i=0;
	//getShopId=5;
	getShopId = ncSrvGetCid(psShmHead,psMsgHead);
	if(getShopId !=0){
	sprintf(caTemp,"select title  from ncmshop where id = %d ",getShopId);
	pasDbOneRecord(caTemp, 0, UT_TYPE_STRING, sizeof(title)-1, title);
	char *titleUTF = convert("GBK", "UTF-8", title);
	//printf("title=%s\n",title);
	utPltPutVarF(psDbHead,"title","%s", titleUTF);
	sprintf(caTemp,"select sid, name, imgpath, level,shopid from ncmcomindex where level != 0 and flags = 0 and type = 1 and shopid = %d and plateid = %d order by level ",getShopId,plateid);	
//	printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
	if(psCur == NULL){
		printf("pscur is null\n");
		printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		int iNum2=0;
		long id =0;
		char name[32]="";
		char img[128]="";
		long level=0;
		char serno[32]="";
		long shopid =0;
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_LONG,4,&id,
      	                    UT_TYPE_STRING,31,name,
							UT_TYPE_STRING,127,img,
							UT_TYPE_LONG,4,&level,
							UT_TYPE_LONG,4,&shopid))  || 1405==iReturn)
		{
		iNum2++;
		p[iNum2-1].sid=id;
		p[iNum2-1].level=level;
		p[iNum2-1].shopid=shopid;
		strcpy(p[iNum2-1].img, img);
		}

		pasDbCloseCursor(psCur);
	}
	}
	

	sprintf(caTemp,"select sid, name, imgpath, level,shopid from ncmcomindex where level != 0 and flags = 0 and type = 1 and plateid = %d group by level order by level ",plateid);	
//  	printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		printf("pscur is null\n");
		printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		
		int iReturn=0;
		int iNum=0;
		long id =0;
		char name[32]="";
		char img[128]="";
		long level=0;
		char serno[32]="";
		long shopid =0;
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_LONG,4,&id,
      	                    UT_TYPE_STRING,31,name,
							UT_TYPE_STRING,127,img,
							UT_TYPE_LONG,4,&level,
							UT_TYPE_LONG,4,&shopid))  || 1405==iReturn)
		{	

		iNum++;
        for(i=0;i<20;i++){
		if(p[i].level == level){
		id = p[i].sid;
		level = p[i].level;
		shopid = p[i].shopid;
		strcpy(img,p[i].img);
		}
		}
		utPltPutLoopVarF(psDbHead,"id",iNum,"%lu", id);
		utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu", iNum);
		utPltPutLoopVarF(psDbHead,"level",iNum,"%lu",level);
		utPltPutLoopVarF(psDbHead,"shopid",iNum,"%lu",shopid);
		char *imgpathUTF = convert("GBK", "UTF-8", img);
		utPltPutLoopVarF(psDbHead,"img",iNum,"%s",imgpathUTF);
		if(level<=4){
		utPltPutLoopVarF(psDbHead,"imgslider",iNum,"%s",imgpathUTF);
		utPltPutLoopVarF(psDbHead,"iNumslider",iNum,"%lu",iNum);
		
		}
		}

		pasDbCloseCursor(psCur);
	}

		utPltShowDb(psDbHead);
		char platepath[128]="";
		sprintf(caTemp,"select platepath from portplate where status = 0 ");
		pasDbOneRecord(caTemp, 0, UT_TYPE_STRING, sizeof(platepath)-1, platepath);
		printf("platepath=%s\n",platepath);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,platepath);
		//utPltOutToHtml(iFd,psMsgHead,psDbHead,"/yuyuan/shouye.html");
		//utPltOutToHtml(iFd,psMsgHead,psDbHead,"/yuyuan2/shouye.html");
		printf("ncm_yuyuan_home  end!!!!\n");
      
	  return 0;	  
}
*/
//模板信息
int ncm_portplate(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncm_portplate start!!!!\n");
	 pasDbCursor *psCur;
	// utPltDbHead *psDbHead;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	// utMsgPrintMsg(psMsgHead);
	char update[32];
	char sid[32];
	
	char caStyped[32] = "";
	dsCltGetSessionValue(1, "stype", UT_TYPE_STRING,sizeof(caStyped)-1, caStyped);
	// printf("caStyped = %s\n", caStyped);
	
	utMsgGetSomeNVar(psMsgHead, 2,
				 "update",        UT_TYPE_STRING,  sizeof(update)-1,       update,
				 "sid",        UT_TYPE_STRING,  sizeof(sid)-1,       sid
				 );				  
	char caTemp[256];
	int total;
	//sprintf(caTemp,"select count(*) from portplate");
	sprintf(caTemp,"select count(*) from portalplate");
	pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, sizeof(total)-1, &total);
	utPltPutVarF(psDbHead,"TotRec","%d",total);
	/*if(strlen(update)>0){
	sprintf(caTemp,"update portplate set status =1");
	//printf("caTemp=%s\n",caTemp);
	pasDbOpenSql(caTemp,0);
	
	sprintf(caTemp,"update portplate set status = 0 where sid = %d",atoi(sid));
//	// printf("caTemp=%s\n",caTemp);
	pasDbOpenSql(caTemp,0);
	}*/
	
	//sprintf(caTemp,"select sid, platename,status from portplate");
	sprintf(caTemp,"select shopplateid, platename from portalplate where 1=1");
	if(strlen(caStyped)>0){
  	  sprintf(caTemp+strlen(caTemp)," and substring(groupcode, 9, 2)='%s'",caStyped);
  }
  psCur = pasDbOpenSql(caTemp,0);
	
	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		
		int iReturn=0;
		int iNum=0;
		long id =0;
		long status =0;
		char name[64]="";
		
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_LONG,sizeof(id),&id,
      	                    UT_TYPE_STRING,64,name
							//UT_TYPE_LONG,sizeof(status),&status
							))  || 1405==iReturn)
		{	
		iNum++;
			if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
		utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu", id);
		//utPltPutLoopVarF(psDbHead,"status",iNum,"%lu", status);
		utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu", iNum);
		//char *nameUTF = convert("GBK", "UTF-8", name);
		utPltPutLoopVarF(psDbHead,"platename",iNum,"%s",name);
		}

		pasDbCloseCursor(psCur);
	}
//		utPltShowDb(psDbHead);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/yuyuan/ncm_portplate.html");
		//utPltOutToHtml(iFd,psMsgHead,psDbHead,"/yuyuan2/shouye.html");
		// printf("ncm_portplate  end!!!!\n");
      
	  return 0;	  
}

//皮肤主题信息
int ncm_skintheme(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncm_skintheme start!!!!\n");
	 pasDbCursor *psCur;
	// utPltDbHead *psDbHead;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	// utMsgPrintMsg(psMsgHead);
	char update[32];
	char sid[32];
	char username[128];
	utMsgGetSomeNVar(psMsgHead, 5,
					 "update",        UT_TYPE_STRING,  sizeof(update)-1,       update,
					 "username",        UT_TYPE_STRING,  sizeof(username)-1,       username,
					 "sid",        UT_TYPE_STRING,  sizeof(sid)-1,       sid
					 );
					  
	char caTemp[256];
	char platepath[256];
	int total;
	long lId;
	int iReturn;
	if(strlen(update)>0){
	iReturn = dsCltGetMyInfo(1,"Userid",&lId);       
   	// printf("iReturn=%d,lId=%d\n",iReturn,lId);
    if(strlen(username)>0) {
      
    	//sprintf(caTemp,"select  name from dsuser where id=%d ",lId); 
    	//pasDbOneRecord(caTemp,0,UT_TYPE_STRING,127,caUsername);    
		
	if(atol(update) == 1){
	strcpy(platepath,"/newver/ext-4.2.1.883/resources/css/ext-all.css");
	}
	else if(atol(update) == 2){
	strcpy(platepath,"/newver/ext-4.2.1.883/resources/css/ext-all-gray.css");
	}
	else if(atol(update) == 3){
	strcpy(platepath,"/newver/ext-4.2.1.883/resources/css/ext-all-access.css");
	}
	else if(atol(update) == 4){
	strcpy(platepath,"/newver/ext-4.2.1.883/resources/css/ext-all-neptune.css");
	}
	sprintf(caTemp,"update skintheme set platepath='%s' where accountname = '%s'",platepath,username);
	}
	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
	}
	pasDbCloseCursor(psCur);
	// printf("ncm_skintheme  end!!!!\n");
	return 0;	  
}

//短信支付管理
int ncm_smspay(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncm_smspay start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	// utMsgPrintMsg(psMsgHead);
	char name[64] = "";
	char groupname[64] = "";
	char sid[32] = "";
	char id[32] = "";
	char desc[128] = "";
	char keyword[128] = "";
	char moneyflag[32] = "";
	char flag[32] = "";
	char count[32] = "";
	char update[32] = "";
	int iReturn = 0;
	utMsgGetSomeNVar(psMsgHead, 9,
   				    "id",        UT_TYPE_STRING,  sizeof(id)-1,       id,
   				    "sid",        UT_TYPE_STRING,  sizeof(sid)-1,       sid,
   				    "name",        UT_TYPE_STRING,  sizeof(name)-1,       name,
   				    "desc",        UT_TYPE_STRING,  sizeof(desc)-1,       desc,
   				    "keyword",        UT_TYPE_STRING,  sizeof(keyword)-1,       keyword,
					"moneyflag",        UT_TYPE_STRING,  sizeof(moneyflag)-1,       moneyflag,
					"flag",        UT_TYPE_STRING,  sizeof(flag)-1,       flag,
					"count",        UT_TYPE_STRING,  sizeof(count)-1,       count,
					"update",        UT_TYPE_STRING,  sizeof(update)-1,       update
					);
			
	char *nameGBK = convert("UTF-8", "GBK", name);			
	char caTemp[256] = "";
	int lShopId=0;
	char *desc_gbk = convert("UTF-8", "GBK", desc);
	char *keyword_gbk = convert("UTF-8", "GBK", keyword);
	dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lShopId);
	// printf("lShopId = %d\n",lShopId);
	sprintf(caTemp,"select groupname from ncsrvgroup where groupid=%d ",lShopId);
	pasDbOneRecord(caTemp, 0, UT_TYPE_STRING, sizeof(groupname)-1, groupname);
	
	// printf("groupname = %s\n",groupname);
	if(strlen(update)>0){
	if(strcmp(update,"add")==0){
		
		//sprintf(caTemp,"insert into ncmsmspay(sid,count,name,description,systime)values(%d,%d,'%s','%s',%d)",lShopId,atoi(count),groupname,desc_gbk,time(0));
		sprintf(caTemp,"insert into ncmsmspay(sid,count,name,description,systime)values(%d,%d,'%s','%s',%d)",atoi(sid),atoi(count),nameGBK,desc_gbk,time(0));
		
	}
	else if(strcmp(update,"del")==0){
		
		sprintf(caTemp,"delete from ncmsmspay where id in (%s) ",id);
		
	}
	else if(strcmp(update,"modi")==0){
		
	//	sprintf(caTemp,"update ncmsmspay set sid=%d, count=%d, name='%s',description='%s',systime=%d,moneyflag=%d where id = %d",lShopId,atoi(count),groupname,desc_gbk,time(0),atoi(moneyflag),atoi(id));
		sprintf(caTemp,"update ncmsmspay set sid=%d, count=%d, name='%s',description='%s',systime=%d,moneyflag=%d where id = %d",atoi(sid),atoi(count),nameGBK,desc_gbk,time(0),atoi(moneyflag),atoi(id));
		
	}
	else if(strcmp(update,"update")==0){
		sprintf(caTemp,"update ncmsmspay set systime=%d,moneyflag=%d where id = %d",time(0),atoi(moneyflag),atoi(id));
	}
	// printf("caTemp=%s\n",caTemp);
    pasDbOpenSql(caTemp,0);
	// printf("ncm_smspay  end!!!!\n");
	return 0;
	}
	else{
		sprintf(caTemp,"select id,sid,count,name,description,systime,moneyflag,flag from ncmsmspay where flag !=1 ");
		if(strlen(name)>0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and name like '%%%s%%' ",nameGBK);
		}
		if(strlen(keyword)>0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and description like '%%%s%%' or  name like '%%%s%%' ",keyword_gbk,keyword_gbk);
		}
		if(lShopId>0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and sid = %d ",lShopId);
		}
	
  	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn = 0;
		int iNum = 0;
		long id = 0;
		long sid = 0;
		long count = 0;
		long moneyflag = 0;
		long flag = 0;
		char name[64] = "";
		char desc[128] = "";
		long systime = 0;
		char _stime[32] = "";
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_LONG,4,&id,
      	                    UT_TYPE_LONG,4,&sid,
      	                    UT_TYPE_LONG,4,&count,
      	                    UT_TYPE_STRING,31,name,
							UT_TYPE_STRING,127,desc,
							UT_TYPE_LONG,4,&systime,
							UT_TYPE_LONG,4,&moneyflag,
							UT_TYPE_LONG,4,&flag
							  ))  || 1405==iReturn)
		{	
			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead,"dh",iNum,",");
			}

			utPltPutLoopVarF(psDbHead,"id",iNum,"%lu", id);
			utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu", sid);
			utPltPutLoopVarF(psDbHead,"count",iNum,"%lu", count);
			//utPltPutLoopVarF(psDbHead,"systime",iNum,"%lu", systime);
			strcpy(_stime, utTimFormat("%Y/%m/%d %H:%M:%S", systime));
			utPltPutLoopVarF(psDbHead,"systime",iNum,"%s", _stime);
			utPltPutLoopVarF(psDbHead,"name",iNum,"%s", name);
			utPltPutLoopVarF(psDbHead,"desc",iNum,"%s", desc);
			utPltPutLoopVarF(psDbHead,"moneyflag",iNum,"%lu", moneyflag);
			utPltPutLoopVarF(psDbHead,"flag",iNum,"%lu", flag);
		}
		utPltPutVarF(psDbHead,"totalCount","%lu", iNum);
		pasDbCloseCursor(psCur);
	}
	}
	
//	utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncmshop/ncm_smspay.htm");
	// printf("ncm_smspay  end!!!!\n");
    return 0;	  
}

//门店信息
int ncm_groupInfo(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead){
	
printf("ncm_groupInfo start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	// utMsgPrintMsg(psMsgHead);
	char keywords[32] = "";
	char start[12]="";
	char limit[12]="";
	char expexcel[12]="";
	char sdate[32]="";
	char edate[32]="";
	char username[32]="";
	unsigned long lsdate;
	unsigned long ledate;
	
	utMsgGetSomeNVar(psMsgHead, 2,
   				    "ip1",        	UT_TYPE_STRING,  sizeof(keywords)-1,    keywords,
					"expexcel",     UT_TYPE_STRING,  sizeof(expexcel)-1,    expexcel
					);
	
	char caTemp[256];
	long lCount1=0;
	int iNum=0;
	sprintf(caTemp,"select groupid, groupname from ncsrvgroup");	

	//snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " order by userid desc");
	
	// printf("caTemp = %s\n",caTemp);	
	psCur = pasDbOpenSql(caTemp,0);
	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
	 
		int iReturn=0;
		iNum=0;
		char username[32];
		int userid=0;

		while(0==(iReturn = pasDbFetchInto(psCur,
							UT_TYPE_LONG,sizeof(userid),&userid,
      	                     UT_TYPE_STRING,sizeof(username),username
      	                     ))  || 1405==iReturn)
		{	

			iNum++;
			if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
			utPltPutLoopVarF(psDbHead,"userid",iNum,"%lu", userid);
			
			utPltPutLoopVarF(psDbHead,"username",iNum,"%s", username);
			utPltPutLoopVarF(psDbHead,"inum",iNum,"%lu", iNum);

		}
		
		pasDbCloseCursor(psCur);
	}
	utPltPutVarF(psDbHead,"TotRec","%lu", iNum);
//	utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncmshop/ncm_storeinfo.htm");
	// printf("ncm_groupInfo  end!!!!\n");
	//free(p);
	return 0;	  
}

#define SHOPIMGPATH "/home/ncmysql/ncsrv/html"
#define UPLOADIMGPATH "/home/ncmysql/ncsrv/upload"
//模板信息
int ncm_uploadimg(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncm_uploadimg start!!!!\n");
	 pasDbCursor *psCur;
	// utPltDbHead *psDbHead;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	// utMsgPrintMsg(psMsgHead);
	char filesize[32];
	char filetype[32];
	char filedim[32];
	char previewimg[1024];
	char width[32];
	char height[32];
	char x1[32];
	char y1[32];
	char x2[32];
	char y2[32];
	char caImgPath[128]="";
	 char imgname[128]="";
	 char caDispImgPath[128]="";
	 char caImgpath1[128]="";
    char img_file[129]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
 		char caTemp[1024]="";
 		char caTemp2[1024]="";
 		unsigned long lSysTime=0;
 		unsigned long lMaxid = 0;
 		char *p;
	utMsgGetSomeNVar(psMsgHead, 11,
				 "filesize",        UT_TYPE_STRING,  sizeof(filesize)-1,       filesize,
				 "filetype",        UT_TYPE_STRING,  sizeof(filetype)-1,       filetype,
				 "filedim",        UT_TYPE_STRING,  sizeof(filedim)-1,       filedim,
				 "previewimg",        UT_TYPE_STRING,  sizeof(previewimg)-1,       previewimg,
				 "w",        UT_TYPE_STRING,  sizeof(width)-1,       width,
				 "h",        UT_TYPE_STRING,  sizeof(height)-1,       height,
				 "x1",        UT_TYPE_STRING,  sizeof(x1)-1,       x1,
				 "y1",        UT_TYPE_STRING,  sizeof(y1)-1,       y1,
				 "x2",        UT_TYPE_STRING,  sizeof(x2)-1,       x2,
				 "y2",        UT_TYPE_STRING,  sizeof(y2)-1,       y2,
				 "image_fil",        UT_TYPE_STRING,  sizeof(img_file)-1,       img_file
				 );				  
	if(!utFileIsExist(UPLOADIMGPATH)) {
        if(mkdir(UPLOADIMGPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncLang("0194导入"),ncLang("0183打开文件出错"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADIMGPATH);
    system(temp0_file);
    
		// 建立图片存放路径
  	sprintf(caImgPath,"%s/images/ShopImages/test", SHOPIMGPATH);
  	if(!utFileIsExist(caImgPath)) {
        if(mkdir(caImgPath,777)!=0){
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", caImgPath);
    system(temp1_file);
	if(strlen(img_file)!=0)
	{
		// printf("img_file = %s\n", img_file);
		  	p=utStrSkipSpaces(img_file);
		  	/*从指定的字符串中取一个词
		  	
		  	  函数原型：
			　　char *utStrGetWord(char *p,char *pOut,
			　　                   short nMaxLen,char *pTerm);
			　　
					参数说明:
			　　p – 输入字符串
			　　pOut --- 输出字符串，必须要事先分配好空间
			　　nMaxLen --- 输出字符传的最大长度
			　　pTerm  ----- 终止条件的字符串，系统在扫描描时遇到该字符串某一个字符时结束。
			　　
			　　返回值：
			　　结束字符的指针。*/
		  	p=utStrGetWord(p,temp_file,200,";\n");
				// printf("temp_file = %s\n", temp_file);
				if(strlen(temp_file)==0){
					  memset(caImgPath, 0, sizeof(caImgPath));
					  memset(caDispImgPath, 0, sizeof(caDispImgPath));
				}else{
						sprintf(imgname, "test_%lu_%s", time(0), temp_file);
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADIMGPATH, temp_file, caImgPath, imgname);
					// printf("temp2_file = %s\n", temp2_file);
				  	system(temp2_file);
				    strcat(caDispImgPath, "/images/ShopImages/test/");
				    strcat(caDispImgPath, imgname);	
					// printf("caDispImgPath = %s\n", caDispImgPath);
				}
	  }
	
	
//		utPltShowDb(psDbHead);
		
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/yuyuan2/shouye.html");
		// printf("ncm_uploadimg  end!!!!\n");
      
	  return 0;	  
}



//坐标信息
int ncm_portallocmark(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead){
	
printf("ncm_portallocmark start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	// utMsgPrintMsg(psMsgHead);
	char update[32] = "";
	char add[32] = "";
	char start[12]="";
	char limit[12]="";
	char tsid[128]="";
	char plateid[64]="";
	char mark[32]="";
	char fname[32]="";
	char sname[32]="";
	char mapid[32]="";
	char position_x[32]="";
	char position_y[32]="";
	char sx[32]="";
	char ex[32]="";
	char sy[32]="";
	char ey[32]="";
	unsigned long lsdate;
	unsigned long ledate;
	
	utMsgGetSomeNVar(psMsgHead, 8,
   				    "update",    UT_TYPE_STRING,  sizeof(update)-1,    update,
   				    "add",    UT_TYPE_STRING,  sizeof(add)-1,    add,
   				    "position_x",    UT_TYPE_STRING,  sizeof(position_x)-1,    position_x,
   				    "position_y",    UT_TYPE_STRING,  sizeof(position_y)-1,    position_y,
   				    "mark",    UT_TYPE_STRING,  sizeof(mark)-1,    mark,
   				    "fname",    UT_TYPE_STRING,  sizeof(fname)-1,    fname,
   				    "sname",    UT_TYPE_STRING,  sizeof(sname)-1,    sname,
					"tsid",     UT_TYPE_STRING,  sizeof(tsid)-1,    tsid
					);
	
	char caTemp[256];
	long lCount1=0;
	int iNum=0;
	
	pasCvtGBK(2,sname,caTemp,32);
	strcpy(sname,caTemp);
				
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);	
	
	//utPltPutVarF(psDbHead,"position_x","%.2f",  350000.25);
	//utPltPutVarF(psDbHead,"position_y","%.2f", -250000.98);
	ncPortalOnline *onlineinfo;
	onlineinfo = (ncPortalOnline *)malloc(sizeof(ncPortalOnline)+1);
	onlineinfo = (ncPortalOnline *)ncSrvGetOnlineUserByStrTsid(psShmHead,tsid);
	if (onlineinfo)
	{ 
		// printf("position_x = %.2f \n",onlineinfo->longitude);
		// printf("position_y = %.2f \n",onlineinfo->latitude);
		
	   utPltPutVarF(psDbHead,"position_x","%.2f", onlineinfo->longitude);
	   utPltPutVarF(psDbHead,"position_y","%.2f", onlineinfo->latitude);
	}
	
	strcpy(mapid,(char *)utComGetVar_sd(psShmHead,"MapId","17_1_0"));
	if(strcmp(mark,"1")==0){
		strcpy(sx,position_x);
		strcpy(sy,position_y);
	}	
	if(strcmp(mark,"2")==0){
		strcpy(ex,position_x);
		strcpy(ey,position_y);
	}
	// printf("add = %s\n",add);
	if(strcmp(add,"add")==0){
	sprintf(caTemp,"select count(*) from portallocmark where mark='%s' and sname='%s'",fname,sname);
	pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &lCount1);
	// printf("lCount1 = %d\n",lCount1);
	if(lCount1>0){
	if(strcmp(mark,"1")==0){
		sprintf(caTemp,"update portallocmark set mapid='%s', sx='%s',sy='%s' where  mark='%s' and sname='%s'",mapid,sx,sy,fname,sname);	
			utPltPutVarF(psDbHead,"msg","%s", "更新成功");
	}
	if(strcmp(mark,"2")==0){
		sprintf(caTemp,"update portallocmark set mapid='%s', ex='%s',ey='%s' where  mark='%s' and sname='%s'",mapid,ex,ey,fname,sname);	
			utPltPutVarF(psDbHead,"msg","%s", "更新成功");
	}
	
	// printf("caTemp = %s\n",caTemp);	
	psCur = pasDbOpenSql(caTemp,0);
	pasDbCloseCursor(psCur);
	
	}
	else{
	
		sprintf(caTemp,"insert into portallocmark(mark,sname,mapid,sx,sy,ex,ey)values('%s','%s','%s','%s','%s','%s','%s')",fname,sname,mapid,sx,sy,ex,ey);	
		// printf("caTemp = %s\n",caTemp);	
		psCur = pasDbOpenSql(caTemp,0);
		pasDbCloseCursor(psCur);
		utPltPutVarF(psDbHead,"msg","%s", "插入成功");
	}
	}
	
//	utPltShowDb(psDbHead);
	if(strlen(update)>0)
	{
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/yuyuannew/xy.htm");
	}
	else{
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/yuyuannew/ncm_portallocmark.htm");
	}
	// printf("ncm_portallocmark  end!!!!\n");
	//free(p);
	return 0;	  
}

//商家联盟
int ncm_shopgroup (utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead){
	
printf("ncm_shopgroup start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	// utMsgPrintMsg(psMsgHead);
	char update[32] = "";
	char add[32] = "";
	char start[32]="";
	char limit[32]="";
	char keyword[32]="";
	char plateid[64]="";
	char groupcode[1024]="";
	char pid[32]="";
	char mapid[32]="";
	char groupname[32]="";
	char curcode[32]="";
	char flag[32]="";
	unsigned long lsdate;
	unsigned long ledate;
	
	utMsgGetSomeNVar(psMsgHead, 10,
   				    "update",    UT_TYPE_STRING,  sizeof(update)-1,    update,
   				    "add",    UT_TYPE_STRING,  sizeof(add)-1,    add,
   				    "groupname",    UT_TYPE_STRING,  sizeof(groupname)-1,    groupname,
   				    "curcode",    UT_TYPE_STRING,  sizeof(curcode)-1,    curcode,
   				    "groupcode",    UT_TYPE_STRING,  sizeof(groupcode)-1,    groupcode,
   				    "pid",    UT_TYPE_STRING,  sizeof(pid)-1,    pid,
   				     "flag",    UT_TYPE_STRING,  sizeof(flag)-1,    flag,
					"start",     UT_TYPE_STRING,  sizeof(start)-1,    start,
					"limit",     UT_TYPE_STRING,  sizeof(limit)-1,    limit,
					"keyword",     UT_TYPE_STRING,  sizeof(keyword)-1,    keyword
					
					);
	
	char caTemp[256];
	long lCount1=0;
	int iNum=0;
	
	pasCvtGBK(2,groupname,caTemp,32);
	strcpy(groupname,caTemp);
	pasCvtGBK(2,keyword,caTemp,32);
	strcpy(keyword,caTemp);
	// printf("add = %s\n",add);
	if(strcmp(update,"add")==0){
	sprintf(caTemp,"select count(*) from ncmshopgroup  where groupcode like '%s%%'",groupcode);
	// printf("caTemp = %s\n",caTemp);	
	pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &lCount1);
	// printf("lCount1 = %d\n",lCount1);
	
	if(lCount1>0){
	char groupcodemax[8];
	char groupcodemax2[8];
	sprintf(caTemp,"select MAX(substring(groupcode,11,4)) from ncmshopgroup  where groupcode like '%s%%'",groupcode);
	// printf("caTemp = %s\n",caTemp);	
	pasDbOneRecord(caTemp, 0, UT_TYPE_STRING, sizeof(groupcodemax), groupcodemax);
	// printf("lCount1=%lu,groupcode = %s,groupcodemax=%s\n",lCount1,groupcode,groupcodemax);
	
	sprintf(groupcodemax2,"%s%02d",groupcode,atoi(groupcodemax)+1);
	// printf("groupcodemax2=%s\n",groupcodemax2);
	
	sprintf(caTemp,"insert into ncmshopgroup(groupcode,groupname,curcode,systime,pid)values('%s','%s','%s',%lu,'%s')",groupcodemax2,groupname,curcode,time(0),pid);	
	}else{
	sprintf(caTemp,"insert into ncmshopgroup(groupcode,groupname,curcode,systime,pid)values('%s01','%s','%s',%lu,'%s')",groupcode,groupname,curcode,time(0),pid);	
	
	}
	// printf("caTemp = %s\n",caTemp);	
	psCur = pasDbOpenSql(caTemp,0);
	pasDbCloseCursor(psCur);
	
//	}
	}
	else if(strcmp(update,"update")==0){
		sprintf(caTemp,"update ncmshopgroup set groupname='%s',curcode='%s',systime=%lu,pid='%s' where  groupcode='%s'",groupname,curcode,time(0),pid,groupcode);	
		// printf("caTemp = %s\n",caTemp);	
		psCur = pasDbOpenSql(caTemp,0);
		pasDbCloseCursor(psCur);
	
	}
	else if(strcmp(update,"del")==0){
		sprintf(caTemp,"delete from ncmshopgroup where groupcode in(%s)",groupcode);	
		// printf("caTemp = %s\n",caTemp);	
		psCur = pasDbOpenSql(caTemp,0);
		pasDbCloseCursor(psCur);
	
	}
	else{
		sprintf(caTemp,"select groupcode,groupname,curcode,systime,pid from ncmshopgroup where 1=1" );
		if(strlen(keyword)>0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and groupname like '%s%%' ",keyword);
		}
		if(strlen(flag)>0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and substring(groupcode, 9, 2) = '%s' ",flag);
		}
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu ",atol(start),atol(limit));
		// printf("caTemp=%s\n",caTemp);
		psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn = 0;
		int iNum = 0;
		char groupname[32] = "";
		char groupcode[32] = "";
		char curcode[32] = "";
		char pid[32] = "";
		long systime = 0;
		char _stime[32] = "";
		while(0==(iReturn = pasDbFetchInto(psCur,
							UT_TYPE_STRING,31,groupcode,
      	                    UT_TYPE_STRING,31,groupname,
      	                    UT_TYPE_STRING,31,curcode,
							UT_TYPE_LONG,4,&systime,
							UT_TYPE_STRING,31,pid
							  ))  || 1405==iReturn)
		{	
			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead,"dh",iNum,",");
			}
			strcpy(_stime, utTimFormat("%Y/%m/%d %H:%M:%S", systime));
			utPltPutLoopVarF(psDbHead,"systime",iNum,"%s", _stime);
			utPltPutLoopVarF(psDbHead,"groupname",iNum,"%s", groupname);
			utPltPutLoopVarF(psDbHead,"groupcode",iNum,"%s", groupcode);
			utPltPutLoopVarF(psDbHead,"curcode",iNum,"%s", curcode);
			utPltPutLoopVarF(psDbHead,"pid",iNum,"%s", pid);
		}
		utPltPutVarF(psDbHead,"totalCount","%lu", iNum);
		pasDbCloseCursor(psCur);
	}
	
	
	}
//	utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncmshop/ncm_shopgroup.htm");
	// printf("ncm_shopgroup  end!!!!\n");
	return 0;	  
}

//单位组管理
int ncm_shopgtype (utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead){
	
printf("ncm_shopgtype start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	// utMsgPrintMsg(psMsgHead);
	char update[32] = "";
	char add[32] = "";
	char start[32]="";
	char limit[32]="";
	char sort[32]="";
	char dir[32]="";
	char keyword[32]="";
	char plateid[64]="";
	char groupcode[1024]="";
	char id[32]="";
	char mapid[32]="";
	char groupname[32]="";
	char type[32]="";
	char autodef[32]="";
	unsigned long lsdate;
	unsigned long ledate;
	
	utMsgGetSomeNVar(psMsgHead, 12,
   				    "update",    UT_TYPE_STRING,  sizeof(update)-1,    update,
   				    "add",    UT_TYPE_STRING,  sizeof(add)-1,    add,
   				    "groupname",    UT_TYPE_STRING,  sizeof(groupname)-1,    groupname,
   				    "type",    UT_TYPE_STRING,  sizeof(type)-1,    type,
   				    "groupcode",    UT_TYPE_STRING,  sizeof(groupcode)-1,    groupcode,
   				    "id",    UT_TYPE_STRING,  sizeof(id)-1,    id,
					"start",     UT_TYPE_STRING,  sizeof(start)-1,    start,
					"limit",     UT_TYPE_STRING,  sizeof(limit)-1,    limit,
					"sort",     UT_TYPE_STRING,  sizeof(sort)-1,    sort,
					"dir",     UT_TYPE_STRING,  sizeof(dir)-1,    dir,
					"keyword",     UT_TYPE_STRING,  sizeof(keyword)-1,    keyword,
					"autodef",     UT_TYPE_STRING,  sizeof(autodef)-1,    autodef
					
					);
	
	char caTemp[256];
	long lCount1=0;
	int iNum=0;
	
	pasCvtGBK(2,groupname,caTemp,32);
	strcpy(groupname,caTemp);
	pasCvtGBK(2,keyword,caTemp,32);
	strcpy(keyword,caTemp);
	// printf("update = %s\n",update);
	if(strcmp(update,"add")==0){
	sprintf(caTemp,"select count(*) from ncmshopgtype  where type = %lu",atol(type));
	// printf("caTemp = %s\n",caTemp);	
	pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &lCount1);
	// printf("lCount1 = %d\n",lCount1);
	char groupcodemax[8];
	char groupcodemax2[8];
	if(lCount1>0){
	
	sprintf(caTemp,"select MAX(groupcode) from ncmshopgtype  where type = %lu",atol(type));
	// printf("caTemp = %s\n",caTemp);	
	pasDbOneRecord(caTemp, 0, UT_TYPE_STRING, sizeof(groupcodemax), groupcodemax);
	// printf("lCount1=%lu,groupcode = %s,groupcodemax=%s\n",lCount1,groupcode,groupcodemax);
	
	sprintf(groupcodemax2,"%s%02d",groupcode,atoi(groupcodemax)+1);
	// printf("groupcodemax2=%s\n",groupcodemax2);
	
	sprintf(caTemp,"insert into ncmshopgtype(groupcode,groupname,systime,type,autodef)values('%s','%s',%lu,%lu,%lu)",groupcodemax2,groupname,time(0),atol(type),atol(autodef));
	
	}else{
	sprintf(caTemp,"insert into ncmshopgtype(groupcode,groupname,systime,type,autodef)values('%s01','%s',%lu,%lu,%lu)",groupcode,groupname,time(0),atol(type),atol(autodef));
	
	
	}
	// printf("caTemp = %s\n",caTemp);	
	psCur = pasDbOpenSql(caTemp,0);
	pasDbCloseCursor(psCur);
	
	if(atol(type) == 2){
	sprintf(caTemp,"insert into ncmdeptmenu(groupcode,menuname,systime,autodef)values('%s','%s',%lu,%lu)",groupcodemax2,groupname,time(0),atol(autodef));
	// printf("caTemp = %s\n",caTemp);	
	psCur = pasDbOpenSql(caTemp,0);
	pasDbCloseCursor(psCur);
	}
//	}
	}
	else if(strcmp(update,"update")==0){
		sprintf(caTemp,"update ncmshopgtype set groupname='%s',systime=%lu,autodef=%lu where id=%lu",groupname,time(0),atol(autodef),atol(id));	
		// printf("caTemp = %s\n",caTemp);	
		psCur = pasDbOpenSql(caTemp,0);
		pasDbCloseCursor(psCur);
	
	}
	else{
		sprintf(caTemp,"select groupcode,groupname,systime,id,type,autodef from ncmshopgtype where groupcode !='00'" );
		if(strlen(keyword)>0){
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and groupname like '%s%%' ",keyword);
		}
		if(strlen(type)>0){
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and type = %lu ",atol(type));
		}
		if(strlen(sort)>0 && strlen(dir)>0){
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " order by %s %s ",sort,dir);
		}
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu ",atol(start),atol(limit));
		// printf("caTemp=%s\n",caTemp);
		psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn = 0;
		int iNum = 0;
		char groupname[32] = "";
		char groupcode[32] = "";
		char curcode[32] = "";
		int id = 0;
		int autodef = 0;
		long systime = 0;
		int type=0;
		char typename[32]="";
		char _stime[32] = "";
		while(0==(iReturn = pasDbFetchInto(psCur,
							UT_TYPE_STRING,31,groupcode,
      	                    UT_TYPE_STRING,31,groupname,
							UT_TYPE_LONG,sizeof(systime),&systime,
							UT_TYPE_LONG,sizeof(id),&id,
							UT_TYPE_LONG,sizeof(type),&type,
							UT_TYPE_LONG,sizeof(autodef),&autodef
							  ))  || 1405==iReturn)
		{	
			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead,"dh",iNum,",");
			}
			strcpy(_stime, utTimFormat("%Y/%m/%d %H:%M:%S", systime));
			utPltPutLoopVarF(psDbHead,"systime",iNum,"%s", _stime);
			utPltPutLoopVarF(psDbHead,"groupname",iNum,"%s", groupname);
			utPltPutLoopVarF(psDbHead,"groupcode",iNum,"%s", groupcode);
			utPltPutLoopVarF(psDbHead,"id",iNum,"%d", id);
			utPltPutLoopVarF(psDbHead,"type",iNum,"%d",type);
			utPltPutLoopVarF(psDbHead,"autodef",iNum,"%d",autodef);
			
		}
		utPltPutVarF(psDbHead,"totalCount","%lu", iNum);
		pasDbCloseCursor(psCur);
	}
	
	}
//	utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_systems/ncm_shopgtype.htm");
	// printf("ncm_shopgtype  end!!!!\n");
	return 0;	  
}

//组菜单权限
int ncmdeptmenu (utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead){
	
printf("ncmdeptmenu start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	// utMsgPrintMsg(psMsgHead);
	char update[32] = "";
	char add[32] = "";
	char start[32]="";
	char limit[32]="";
	char sort[32]="";
	char dir[32]="";
	char keyword[32]="";
	char plateid[64]="";
	char groupcode[1024]="";
	char id[32]="";
	char mapid[32]="";
	char menuname[32]="";
	char submenu[1024]="";
	char type[32]="";
	unsigned long lsdate;
	unsigned long ledate;
	char rolname[64]="";
	char caOut[200][36];
	memset(caOut,0,200*36);
	char menuname1[1024]="";
	
	utMsgGetSomeNVar(psMsgHead, 13,
   				    "update",    UT_TYPE_STRING,  sizeof(update)-1,    update,
   				    "add",    UT_TYPE_STRING,  sizeof(add)-1,    add,
   				    "menuname",    UT_TYPE_STRING,  sizeof(menuname)-1,    menuname,
   				    "submenu",    UT_TYPE_STRING,  sizeof(submenu)-1,    submenu,
   				    "type",    UT_TYPE_STRING,  sizeof(type)-1,    type,
   				    "groupcode",    UT_TYPE_STRING,  sizeof(groupcode)-1,    groupcode,
   				    "rolvalue",    UT_TYPE_STRING,  sizeof(rolname)-1,    rolname,
   				    "id",    UT_TYPE_STRING,  sizeof(id)-1,    id,
					"start",     UT_TYPE_STRING,  sizeof(start)-1,    start,
					"limit",     UT_TYPE_STRING,  sizeof(limit)-1,    limit,
					"sort",     UT_TYPE_STRING,  sizeof(sort)-1,    sort,
					"dir",     UT_TYPE_STRING,  sizeof(dir)-1,    dir,
					"keyword",     UT_TYPE_STRING,  sizeof(keyword)-1,    keyword
					
					);
	
	char caTemp[256];
	long lCount1=0;
	int iNum=0;
	
	pasCvtGBK(2,menuname,caTemp,32);
	strcpy(menuname,caTemp);
	pasCvtGBK(2,submenu,caTemp,1024);
	strcpy(submenu,caTemp);
	pasCvtGBK(2,keyword,caTemp,32);
	strcpy(keyword,caTemp);
	// printf("menuname = %s \n",menuname);
	strcpy(menuname1, menuname);
	
	// printf("update = %s\n",update);
	if(strcmp(update,"update")==0){
	  sprintf(caTemp,"delete from ncmdeptmenu where groupcode='%s'",groupcode);	
		// printf("caTemp = %s\n",caTemp);	
		psCur = pasDbOpenSql(caTemp,0);
		pasDbCloseCursor(psCur);
		}
	else if(strcmp(update,"add")==0){
		//char *p;
		//p=strtok(menuname, ",");
		//printf("p = %s\n",p);
		sprintf(caTemp,"insert into ncmdeptmenu(groupcode,menuname,submenu,systime)values('%s','%s','%s',%lu)",groupcode,menuname,submenu,time(0));
		// printf("caTemp = %s\n",caTemp);	
		psCur = pasDbOpenSql(caTemp,0);
		pasDbCloseCursor(psCur);
		// while(p=strtok(NULL, ",")){
		// printf("p = %s\n",p);
		// sprintf(caTemp,"insert into ncmdeptmenu(groupcode,menuname,submenu,systime)values('%s','%s','%s',%lu)",groupcode,menuname,submenu,time(0));
		// printf("caTemp = %s\n",caTemp);	
		// psCur = pasDbOpenSql(caTemp,0);
		// pasDbCloseCursor(psCur);
		// }
		
	  //ncmRolelist_sww(psShmHead,iFd,psMsgHead,menuname1,rolname,update);
	}
	else{
		sprintf(caTemp,"select groupcode,menuname,submenu,systime,id from ncmdeptmenu where groupcode !='00'" );
		if(strlen(keyword)>0){
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and menuname like '%s%%' ",keyword);
		}
		if(strlen(groupcode)>0){
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and groupcode = '%s' ",groupcode);
		}
		if(strlen(sort)>0 && strlen(dir)>0){
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " order by %s %s ",sort,dir);
		}
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu ",atol(start),atol(limit));
		// printf("caTemp=%s\n",caTemp);
		psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn = 0;
		int iNum = 0;
		char menuname[32] = "";
		char submenu[1024] = "";
		char groupcode[32] = "";
		char curcode[32] = "";
		int id = 0;
		long systime = 0;
		char typename[32]="";
		char _stime[32] = "";
		while(0==(iReturn = pasDbFetchInto(psCur,
							UT_TYPE_STRING,31,groupcode,
      	                    UT_TYPE_STRING,31,menuname,
      	                    UT_TYPE_STRING,sizeof(submenu),submenu,
							UT_TYPE_LONG,sizeof(systime),&systime,
							UT_TYPE_LONG,sizeof(id),&id
							  ))  || 1405==iReturn)
		{	
			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead,"dh",iNum,",");
			}
			strcpy(_stime, utTimFormat("%Y/%m/%d %H:%M:%S", systime));
			utPltPutLoopVarF(psDbHead,"systime",iNum,"%s", _stime);
			utPltPutLoopVarF(psDbHead,"menuname",iNum,"%s", menuname);
			utPltPutLoopVarF(psDbHead,"submenu",iNum,"%s", submenu);
			utPltPutLoopVarF(psDbHead,"groupcode",iNum,"%s", groupcode);
			utPltPutLoopVarF(psDbHead,"id",iNum,"%d", id);
			
		}
		utPltPutVarF(psDbHead,"totalCount","%lu", iNum);
		pasDbCloseCursor(psCur);
	}
	
	}
//	utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_systems/ncm_deptmenu.htm");
	// printf("ncmdeptmenu  end!!!!\n");
	return 0;	  
}



//规则设置
int ncm_srvpar (utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead){
	
printf("ncm_srvpar start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	// utMsgPrintMsg(psMsgHead);
	char update[32] = "";
	char add[32] = "";
	char start[32]="";
	char limit[32]="";
	char sort[32]="";
	char dir[32]="";
	char keyword[32]="";
	char plateid[64]="";
	char sessiontime[32]="";
	char macbindtime[32]="";
	char smslocalnum[32]="";
	char idletime[32]="";
	char smsinternum[32]="";
	char smsusetime[64]="";
	char limittime[32]="";
	char limitflow[32]="";
	char limitband[32]="";
	char limitunit[32]="";
	char passcodetime[32]="";
	char servicetime[64]="";
	char eng_smsmessage[255]="";
	char zh_smsmessage[255]="";
	char autoauth[32]="";
	char macauth[32]="";
	char weixinauth[32]="";
	char maxmac[32]="";
	char errornum[32]="";
	char errorlock[32]="";
	char flags[32]="";
	char id[32]="";
	char mapid[32]="";
	char ptype[32]="";
	char pid[32]="";
	char selsid[32]="";
	char seltype[32]="";
	char paroption[1024]="";
	char isupdate[32] = "";
	unsigned long lsdate;
	unsigned long ledate;
	unsigned long pids[100];
	long i;
	unsigned long  lflow;
	unsigned long  lband;
	char smsname[32];
	
//	utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 34,
   				    "update",    UT_TYPE_STRING,  sizeof(update)-1,    update,
   				    "ptype",    UT_TYPE_STRING,  sizeof(ptype)-1,    ptype,
   				    "pid",    UT_TYPE_STRING,  sizeof(pid)-1,    pid,
   				    "sessiontime",    UT_TYPE_STRING,  sizeof(sessiontime)-1,    sessiontime,
   				    "idletime",    UT_TYPE_STRING,  sizeof(idletime)-1,    idletime,
   				    "macbindtime",    UT_TYPE_STRING,  sizeof(macbindtime)-1,    macbindtime,
   				    "smslocalnum",    UT_TYPE_STRING,  sizeof(smslocalnum)-1,    smslocalnum,
   				    "smsinternum",    UT_TYPE_STRING,  sizeof(smsinternum)-1,    smsinternum,
   				    "smsusetime",    UT_TYPE_STRING,  sizeof(smsusetime)-1,    smsusetime,
   				    "limittime",    UT_TYPE_STRING,  sizeof(limittime)-1,    limittime,
   				    "limitflow",    UT_TYPE_STRING,  sizeof(limitflow)-1,    limitflow,
   				    "limitband",    UT_TYPE_STRING,  sizeof(limitband)-1,    limitband,
   				    "limitunit",    UT_TYPE_STRING,  sizeof(limitunit)-1,    limitunit,
   				    "passcodetime",    UT_TYPE_STRING,  sizeof(passcodetime)-1,    passcodetime,
   				    "servicetime",    UT_TYPE_STRING,  sizeof(servicetime)-1,    servicetime,
   				    "eng_smsmessage",    UT_TYPE_STRING,  sizeof(eng_smsmessage)-1,    eng_smsmessage,
   				    "zh_smsmessage",    UT_TYPE_STRING,  sizeof(zh_smsmessage)-1,    zh_smsmessage,
   				    "autoauth",    UT_TYPE_STRING,  sizeof(autoauth)-1,    autoauth,
   				    "macauth",    UT_TYPE_STRING,  sizeof(macauth)-1,    macauth,
   				    "weixinauth",    UT_TYPE_STRING,  sizeof(weixinauth)-1,    weixinauth,
   				    "maxmac",    UT_TYPE_STRING,  sizeof(maxmac)-1,    maxmac,
   				    "errornum",    UT_TYPE_STRING,  sizeof(errornum)-1,    errornum,
   				    "errorlock",    UT_TYPE_STRING,  sizeof(errorlock)-1,    errorlock,
   				    "paroption",    UT_TYPE_STRING,  sizeof(paroption)-1,    paroption,
   				    "flags",    UT_TYPE_STRING,  sizeof(flags)-1,    flags,
					"start",     UT_TYPE_STRING,  sizeof(start)-1,    start,
					"limit",     UT_TYPE_STRING,  sizeof(limit)-1,    limit,
					"sort",     UT_TYPE_STRING,  sizeof(sort)-1,    sort,
					"dir",     UT_TYPE_STRING,  sizeof(dir)-1,    dir,
					"keyword",     UT_TYPE_STRING,  sizeof(keyword)-1,    keyword,	
					"selsid",     UT_TYPE_STRING,  sizeof(selsid)-1,    selsid,	
					"seltype",     UT_TYPE_STRING,  sizeof(seltype)-1,    seltype,
					"smsname",  UT_TYPE_STRING,sizeof(smsname)-1,smsname,
					"isupdate",  UT_TYPE_STRING,sizeof(isupdate)-1,isupdate
					);
	entertrim(paroption);
	entertrim(eng_smsmessage);
	entertrim(zh_smsmessage);
	
	lflow = atol(limitflow);
	lband = atol(limitband);
	//printf("流量：%lu\n  带宽：%lu\n",lflow,lband);
	
	char caTemp[1024];
	char caTemp2[1024];
	long lCount1=0;
	int iNum=0;
	
	if(strcmp(update,"refresh")==0){
		// printf("ncSrvSysReset\n");
		ncSrvSysReset(psShmHead);
	}
	if(strlen(flags)==0){
		strcpy(flags,"1");
	}
	if(strlen(maxmac)==0){
		strcpy(maxmac,"5");
	}
	if(strlen(errornum)==0){
		strcpy(errornum,"5");
	}
	if(strstr(smsusetime,"undefined") >0){
		strcpy(smsusetime,"");
	}
	if(strstr(servicetime,"undefined") >0){
		strcpy(servicetime,"");
	}
	pasCvtGBK(2,zh_smsmessage,caTemp,255);
	strcpy(zh_smsmessage,pasDbReplaceQuote(caTemp));
	//strcpy(zh_smsmessage,caTemp);
	pasCvtGBK(2,eng_smsmessage,caTemp,255);
	strcpy(eng_smsmessage,pasDbReplaceQuote(caTemp));
	//strcpy(eng_smsmessage,caTemp);
	// printf("paroption = %s\n",paroption);
	pasCvtGBK(2,paroption,caTemp,1024);
	// printf("caTemp = %s\n",caTemp);
	strcpy(paroption,pasDbReplaceQuote(caTemp));
	// printf("paroption = %s\n",paroption);
	//strcpy(paroption,caTemp);
	
	
	
//printf("1111111\n");
			
	//pasCvtGBK(2,keyword,caTemp,32);
	//strcpy(keyword,caTemp);
//	 printf("update = %s\n",update);
	if(strcmp(update,"add")==0){
	sprintf(caTemp,"select count(*) from ncsrvpar  where pid = %s and ptype = %lu",pid,atol(ptype));
 printf("caTemp = %s\n",caTemp);	
	pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &lCount1);
	// printf("lCount1 = %d\n",lCount1);
	if(lCount1 == 0){
	
	 sprintf(caTemp,"insert into ncsrvpar(pid,ptype,sessiontime,idletime,macbindtime,smslocalnum,smsinternum,smsusetime,limittime,limitflow,limitband,limitunit,passcodetime,servicetime,eng_smsmessage,zh_smsmessage,autoauth,macauth,weixinauth,maxmac,errornum,errorlock,addtime,moditime,paroption,flags)values(%s,%lu,%lu,%lu,%lu,%lu,%lu,'%s',%lu,%lu,%lu,%lu,%lu,'%s','%s','%s',%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,'%s',%lu)",pid,atol(ptype),atol(sessiontime),atol(idletime),atol(macbindtime),atol(smslocalnum),atol(smsinternum),smsusetime,atol(limittime),lflow,lband,atol(limitunit),atol(passcodetime),servicetime,eng_smsmessage,zh_smsmessage,atol(autoauth),atol(macauth),atol(weixinauth),atol(maxmac),atol(errornum),atol(errorlock),time(0),time(0),paroption,atol(flags));
	printf("caTemp=%s\n",caTemp);
	 pasDbExecSqlF(caTemp);
	}
//	else{
//	sprintf(caTemp,"update ncsrvpar set sessiontime =%lu,idletime=%lu,macbindtime=%lu,smslocalnum=%lu,smsinternum=%lu,smsusetime='%s',limittime=%lu,limitflow=%lu,limitband=%lu,limitunit=%lu,passcodetime=%lu,servicetime='%s',eng_smsmessage='%s',zh_smsmessage='%s',autoauth=%lu,macauth=%lu,weixinauth=%lu,maxmac=%lu,errornum=%lu,errorlock=%lu,moditime=%lu,paroption='%s',flags=%lu where pid=%s and ptype = %lu",atol(sessiontime),atol(idletime),atol(macbindtime),atol(smslocalnum),atol(smsinternum),smsusetime,atol(limittime),lflow,lband,atol(limitunit),atol(passcodetime),servicetime,eng_smsmessage,zh_smsmessage,atol(autoauth),atol(macauth),atol(weixinauth),atol(maxmac),atol(errornum),atol(errorlock),time(0),paroption,atol(flags),pid,atol(ptype));	
//	}
	// printf("caTemp = %s\n",caTemp);	
//	psCur = pasDbOpenSql(caTemp,0);
//	pasDbCloseCursor(psCur);
//		pasDbExecSqlF("update ncsrvgroup set smsname='%s' where groupid=%s ",smsname,pid);
//	
//printf("2222\n");
	}
	else if(strcmp(update,"update")==0&&strcmp(isupdate,"ys")==0){
		//printf("333\n");
		sprintf(caTemp,"update ncsrvpar set sessiontime =%lu,idletime=%lu,macbindtime=%lu,smslocalnum=%lu,smsinternum=%lu,smsusetime='%s',limittime=%lu,limitflow=%lu,limitband=%lu,limitunit=%lu,passcodetime=%lu,servicetime='%s',eng_smsmessage='%s',zh_smsmessage='%s',autoauth=%lu,macauth=%lu,weixinauth=%lu,maxmac=%lu,errornum=%lu,errorlock=%lu,moditime=%lu,paroption='%s',flags=%lu where pid=%s and ptype = %lu",atol(sessiontime),atol(idletime),atol(macbindtime),atol(smslocalnum),atol(smsinternum),smsusetime,atol(limittime),lflow,lband,atol(limitunit),atol(passcodetime),servicetime,eng_smsmessage,zh_smsmessage,atol(autoauth),atol(macauth),atol(weixinauth),atol(maxmac),atol(errornum),atol(errorlock),time(0),paroption,atol(flags),selsid,atol(seltype));	
		 
		 printf("caTemp = %s\n",caTemp);	
		 pasDbExecSqlF(caTemp);
//		psCur = pasDbOpenSql(caTemp,0);
	//	pasDbCloseCursor(psCur);
		
		
		pasDbExecSqlF("update ncsrvgroup set smsname='%s' where groupid=%s ",smsname,pid);
	
	}else if(strcmp(update,"del")==0){
		sprintf(caTemp,"delete from ncsrvpar where pid in(%s) and ptype = %lu",selsid,atol(seltype));	
		 printf("caTemp = %s\n",caTemp);	
		psCur = pasDbOpenSql(caTemp,0);
		pasDbCloseCursor(psCur);
	
	}
	else{
		sprintf(caTemp,"select pid,ptype,sessiontime,idletime,macbindtime,smslocalnum,smsinternum,smsusetime,limittime,limitflow,limitband,limitunit,passcodetime,servicetime,eng_smsmessage,zh_smsmessage,autoauth,macauth,weixinauth,maxmac,errornum,errorlock,addtime,moditime,paroption,flags from ncsrvpar where 1=1 " );
		if(strlen(keyword)>0){
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and groupname like '%s%%' ",keyword);
		}
		if(strlen(ptype)>0){
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and ptype = %lu ",atol(ptype));
		}
		if(strlen(pid)>0){
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and pid = %s ",pid);
		}
		if(strlen(sort)>0 && strlen(dir)>0){
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " order by %s %s ",sort,dir);
		}
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu ",atol(start),atol(limit));
		// printf("caTemp=%s\n",caTemp);
		psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn = 0;
	
		char servicetime[64] = "";
		char smsusetime[64] = "";
		char eng_smsmessage[255] = "";
		char zh_smsmessage[255] = "";
		char eng_smsmessage2[255] = "";
		char zh_smsmessage2[255] = "";
		unsigned long pid = 0;
		int ptype = 0;
		int sessiontime = 0;
		int idletime = 0;
		int macbindtime = 0;
		int smslocalnum = 0;
		int smsinternum = 0;
		int limittime = 0;
		int limitflow = 0;
		int limitband = 0;
		int limitunit = 0;
		int passcodetime = 0;
		long systime = 0;
		int autoauth=0;
		int macauth=0;
		int weixinauth=0;
		int maxmac=0;
		int errornum=0;
		int errorlock=0;
		int addtime=0;
		int moditime=0;
		int flags=0;
		char paroption[1024]="";
		char paroption2[1024]="";
		char _stime[32] = "";
		char _moditime[32] = "";
	
		while(0==(iReturn = pasDbFetchInto(psCur,
							UT_TYPE_ULONG,sizeof(pid),&pid,
							UT_TYPE_LONG,sizeof(ptype),&ptype,
							UT_TYPE_LONG,sizeof(sessiontime),&sessiontime,
							UT_TYPE_LONG,sizeof(idletime),&idletime,
							UT_TYPE_LONG,sizeof(macbindtime),&macbindtime,
							UT_TYPE_LONG,sizeof(smslocalnum),&smslocalnum,
							UT_TYPE_LONG,sizeof(smsinternum),&smsinternum,
							UT_TYPE_STRING,sizeof(smsusetime),smsusetime,
							UT_TYPE_LONG,sizeof(limittime),&limittime,
							UT_TYPE_LONG,sizeof(limitflow),&limitflow,
							UT_TYPE_LONG,sizeof(limitband),&limitband,
							UT_TYPE_LONG,sizeof(limitunit),&limitunit,
							UT_TYPE_LONG,sizeof(passcodetime),&passcodetime,
							UT_TYPE_STRING,sizeof(servicetime),servicetime,
							UT_TYPE_STRING,sizeof(eng_smsmessage),eng_smsmessage,
							UT_TYPE_STRING,sizeof(zh_smsmessage),zh_smsmessage,
							UT_TYPE_LONG,sizeof(autoauth),&autoauth,
							UT_TYPE_LONG,sizeof(macauth),&macauth,
							UT_TYPE_LONG,sizeof(weixinauth),&weixinauth,
							UT_TYPE_LONG,sizeof(maxmac),&maxmac,
							UT_TYPE_LONG,sizeof(errornum),&errornum,
							UT_TYPE_LONG,sizeof(errorlock),&errorlock,
							UT_TYPE_LONG,sizeof(addtime),&addtime,
							UT_TYPE_LONG,sizeof(moditime),&moditime,
							UT_TYPE_STRING,sizeof(paroption),paroption,
							UT_TYPE_LONG,sizeof(flags),&flags
							  ))  || 1405==iReturn)
		{	
			pids[iNum]=pid;
			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead,"dh",iNum,",");
			}
			strcpy(_stime, utTimFormat("%Y/%m/%d %H:%M:%S", addtime));
			utPltPutLoopVarF(psDbHead,"addtime",iNum,"%s", _stime);
			strcpy(_moditime, utTimFormat("%Y/%m/%d %H:%M:%S", moditime));
			utPltPutLoopVarF(psDbHead,"moditime",iNum,"%s", _moditime);
			
			
			utPltPutLoopVarF(psDbHead,"smsusetime",iNum,"%s", smsusetime);
			utPltPutLoopVarF(psDbHead,"servicetime",iNum,"%s", servicetime);
			
			// utPltSetCvtHtml(1);
			 utPltPutLoopVarF(psDbHead,"eng_smsmessage",iNum,"%s", eng_smsmessage);
			 utPltPutLoopVarF(psDbHead,"zh_smsmessage",iNum,"%s", zh_smsmessage);
			 utPltPutLoopVarF(psDbHead,"paroption",iNum,"%s", paroption);
			// utPltSetCvtHtml(0);
			//trim(paroption)
			
			
			utPltPutLoopVarF(psDbHead,"pid",iNum,"%lu", pid);
			utPltPutLoopVarF(psDbHead,"ptype",iNum,"%d", ptype);
			utPltPutLoopVarF(psDbHead,"sessiontime",iNum,"%d", sessiontime);
			utPltPutLoopVarF(psDbHead,"idletime",iNum,"%d", idletime);
			utPltPutLoopVarF(psDbHead,"macbindtime",iNum,"%d", macbindtime);
			utPltPutLoopVarF(psDbHead,"smslocalnum",iNum,"%d", smslocalnum);
			utPltPutLoopVarF(psDbHead,"smsinternum",iNum,"%d",smsinternum);
			utPltPutLoopVarF(psDbHead,"limittime",iNum,"%d",limittime);
			utPltPutLoopVarF(psDbHead,"limitflow",iNum,"%d",limitflow);
			utPltPutLoopVarF(psDbHead,"limitband",iNum,"%d",limitband);
			utPltPutLoopVarF(psDbHead,"limitunit",iNum,"%d",limitunit);
			utPltPutLoopVarF(psDbHead,"passcodetime",iNum,"%d",passcodetime);
			utPltPutLoopVarF(psDbHead,"autoauth",iNum,"%d",autoauth);
			utPltPutLoopVarF(psDbHead,"macauth",iNum,"%d",macauth);
			utPltPutLoopVarF(psDbHead,"weixinauth",iNum,"%d",weixinauth);
			utPltPutLoopVarF(psDbHead,"maxmac",iNum,"%d",maxmac);
			utPltPutLoopVarF(psDbHead,"errornum",iNum,"%d",errornum);
			utPltPutLoopVarF(psDbHead,"errorlock",iNum,"%d",errorlock);
			utPltPutLoopVarF(psDbHead,"flags",iNum,"%d",flags);

			
		}
		
		
		utPltPutVarF(psDbHead,"totalCount","%lu", iNum);
		pasDbCloseCursor(psCur);
	}
	
	}
	char caSmsname[32];
//	printf("ccccccccccccccccc\n");
   for(i=0;i<iNum;i++){
   	sprintf(caTemp,"select smsname from ncsrvgroup where groupid=%lu ",pids[i]);
 //  	printf("caTemp=%s\n",caTemp);
   	memset(caSmsname,0,sizeof(caSmsname));
   	pasDbOneRecord(caTemp,0,UT_TYPE_STRING,31,caSmsname);
   	utPltPutLoopVar(psDbHead,"smsname",i+1,caSmsname);
  }
//	utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_rules/ncm_srvpar.htm");
	// printf("ncm_srvpar  end!!!!\n");
	return 0;	  
}

//带宽获取
int ncm_limitband(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
		
	printf("ncm_limitband  Start!!!!");
//	utMsgPrintMsg(psMsgHead); 
	utPltDbHead *psDbHead = utPltInitDbHead();
	
	char sql[1024];
	int iReturn;
	char name[64]="";
	unsigned long value =0;
	
	sprintf(sql, "SELECT name, value FROM ncmbandpar");
	
	pasDbCursor *psCur = pasDbOpenSql(sql, 0);
	
	
	int row = 0;
	
			int iret = pasDbFetchInto(psCur, 
					UT_TYPE_STRING,	sizeof(name) - 1,	name,
					UT_TYPE_ULONG,sizeof(value),&value);
					
		while(0 == iret || 1405 == iret)
		{
			

				if(row > 1)
				{
					utPltPutLoopVar(psDbHead,"dh", row, ",");
				}
				
				utPltPutLoopVar(psDbHead,"name", row, name);
				utPltPutLoopVarF(psDbHead, "value", row,"%lu",value);
				row++;
				iret = pasDbFetchInto(psCur, 
															UT_TYPE_STRING,	sizeof(name) - 1,	name,
															UT_TYPE_ULONG,sizeof(value),&value);
		}
		pasDbCloseCursor(psCur);
		utPltOutToHtml(iFd, psMsgHead, psDbHead, "common/limitband.htm");
	

	return 0;
	
	
}


//SSID属性
int ncm_srvgroupssid(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncm_srvgroupssid start!!!!\n");
	 pasDbCursor *psCur;
	// utPltDbHead *psDbHead;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	// utMsgPrintMsg(psMsgHead);
	char update[32];
	char sid[32];
	utMsgGetSomeNVar(psMsgHead, 2,
				 "update",        UT_TYPE_STRING,  sizeof(update)-1,       update,
				 "sid",        UT_TYPE_STRING,  sizeof(sid)-1,       sid
				 );				  
	char caTemp[256];
	int total;
	sprintf(caTemp,"select count(*) from ncsrvgroupssid");
	pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, sizeof(total)-1, &total);
	utPltPutVarF(psDbHead,"TotRec","%d",total);

	
	sprintf(caTemp,"select sid, ssid,groupid from ncsrvgroupssid");
  //	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
	
	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		
		int iReturn=0;
		int iNum=0;
		unsigned long id =0;
		long status =0;
		char name[64]="";
		
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_ULONG,sizeof(id),&id,
      	                    UT_TYPE_STRING,64,name,
							UT_TYPE_LONG,sizeof(status),&status
							))  || 1405==iReturn)
		{	
		iNum++;
			if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
		utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu", id);
		utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu", status);
		utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu", iNum);
		char *nameUTF = convert("GBK", "UTF-8", name);
		utPltPutLoopVarF(psDbHead,"ssid",iNum,"%s",nameUTF);
		}

		pasDbCloseCursor(psCur);
	}
//		utPltShowDb(psDbHead);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_rules/ncm_srvgroupssid.htm");
		//utPltOutToHtml(iFd,psMsgHead,psDbHead,"/yuyuan2/shouye.html");
		// printf("ncm_srvgroupssid  end!!!!\n");
      
	  return 0;	  
}
//用户组
int ncm_srvgroup(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncm_srvgroup start!!!!\n");
	 pasDbCursor *psCur;
	// utPltDbHead *psDbHead;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	// utMsgPrintMsg(psMsgHead);
	char update[32];
	char sid[32];
	utMsgGetSomeNVar(psMsgHead, 2,
				 "update",        UT_TYPE_STRING,  sizeof(update)-1,       update,
				 "sid",        UT_TYPE_STRING,  sizeof(sid)-1,       sid
				 );				  
	char caTemp[256];
	int total;
	sprintf(caTemp,"select count(*) from ncsrvgroup");
	pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, sizeof(total)-1, &total);
	utPltPutVarF(psDbHead,"TotRec","%d",total);

	sprintf(caTemp,"select groupid, dispname,groupname from ncsrvgroup where flags = 1" );
  	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
	
	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		
		int iReturn=0;
		int iNum=0;
		unsigned long id =0;
		long status =0;
		char name[128]="";
		char groupname[32]="";
		
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_ULONG,sizeof(id),&id,
      	                    UT_TYPE_STRING,128,name,
							UT_TYPE_STRING,32,groupname
							))  || 1405==iReturn)
		{	
		iNum++;
			if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
		utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu", id);
		utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu", iNum);
		//char *nameUTF = convert("GBK", "UTF-8", name);
		//char *groupnameUTF = convert("GBK", "UTF-8", groupname);
		utPltPutLoopVarF(psDbHead,"dispname",iNum,"%s",name);
		utPltPutLoopVarF(psDbHead,"groupname",iNum,"%s",groupname);
		}

		pasDbCloseCursor(psCur);
	}
//		utPltShowDb(psDbHead);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_rules/ncm_srvgroup.htm");
		//utPltOutToHtml(iFd,psMsgHead,psDbHead,"/yuyuan2/shouye.html");
		// printf("ncm_srvgroup  end!!!!\n");
      
	  return 0;	  
}

//自定义权限
int ncm_autopri (utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead){
	
printf("ncm_autopri start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	// utMsgPrintMsg(psMsgHead);
	char update[32] = "";
	char flag[32] = "";
	char start[32]="";
	char limit[32]="";
	char sort[32]="";
	char dir[32]="";
	char keyword[32]="";
	char plateid[64]="";
	char groupcode[32]="";
	char groupcode2[32]="";
	char groupcode3[32]="";
	char id[32]="";
	char mapid[32]="";
	char groupname[32]="";
	char shopcode[32]="";
	char node[32]="";
	char parentcode[32]="";
	unsigned long lsdate;
	unsigned long ledate;
	long lSid =0;
	utMsgGetSomeNVar(psMsgHead, 13,
   				    "update",    UT_TYPE_STRING,  sizeof(update)-1,    update,
   				    "flag",    UT_TYPE_STRING,  sizeof(flag)-1,    flag,
   				    "groupname",    UT_TYPE_STRING,  sizeof(groupname)-1,    groupname,
   				    "node",    UT_TYPE_STRING,  sizeof(node)-1,    node,
   				    "groupcode",    UT_TYPE_STRING,  sizeof(groupcode)-1,    groupcode,
   				    "id",    UT_TYPE_STRING,  sizeof(id)-1,    id,
							"start",     UT_TYPE_STRING,  sizeof(start)-1,    start,
							"limit",     UT_TYPE_STRING,  sizeof(limit)-1,    limit,
							"sort",     UT_TYPE_STRING,  sizeof(sort)-1,    sort,
							"dir",     UT_TYPE_STRING,  sizeof(dir)-1,    dir,
							"keyword",     UT_TYPE_STRING,  sizeof(keyword)-1,    keyword,
							"shopcode",     UT_TYPE_STRING,  sizeof(shopcode)-1,    shopcode,
							"parentcode",     UT_TYPE_STRING,  sizeof(parentcode)-1,    parentcode
					
					);
	
	char caTemp[256];
	char caTemp2[256];
	char caTemp3[256];
	char stype[4]="";
	char opt[32]="";
	char opt2[32]="";
	long lCount1=0;
	long lCount3=0;
	int iNum=0;
	if(strcmp(update,"refresh")==0){
		// printf("ncSrvSysReset\n");
		ncSrvSysReset(psShmHead);
	}
	
	dsCltGetSessionValue(1,"stype",UT_TYPE_STRING,sizeof(stype)-1,stype);
	// printf("stype = %s\n",stype);
	if(strlen(stype)>0){
	strcpy(shopcode,stype);
	}
	
	dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(opt)-1,opt);
	// printf("opt = %s\n",opt);
	ncStrToStr(opt,opt2);
	// printf("opt2 = %s\n",opt2);
	char NcmProvince[32]="";
	char NcmDeptType[32]="";
	long provice =0;
	char deptType[4] ="00";
	sprintf(NcmProvince,"%s",utComGetVar_sd(psShmHead,"NcmProvince","河南省"));
	sprintf(NcmDeptType,"%s",utComGetVar_sd(psShmHead,"NcmDeptType","商城"));
	// printf("%s\n",utComGetVar_sd(psShmHead,"NcmProvince","河南省"));
	// printf("%s\n",utComGetVar_sd(psShmHead,"NcmDeptType","商城"));
	sprintf(caTemp,"select aid from ncmarea  where aname = '%s'",NcmProvince);
	// printf("caTemp = %s\n",caTemp);	
	pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &provice);
	// printf("provice = %lu\n",provice);
	
	
	
	
	sprintf(caTemp,"select groupcode from ncmshopgtype  where groupname = '%s' and type = 1",NcmDeptType);
	// printf("caTemp = %s\n",caTemp);	
	pasDbOneRecord(caTemp, 0, UT_TYPE_STRING, sizeof(deptType), deptType);
	// printf("deptType = %s\n",deptType);
	sprintf(groupcode2,"%d0000%s%s000000",provice,deptType,shopcode);//前16位
	// printf("groupcode2 = %s\n",groupcode2);
	
	pasCvtGBK(2,groupname,caTemp,32);
	strcpy(groupname,caTemp);
	pasCvtGBK(2,keyword,caTemp,32);
	strcpy(keyword,caTemp);
	// printf("update = %s\n",update);
	if(strcmp(update,"add")==0){
	if(strlen(parentcode)>=strlen(opt2)){
	sprintf(caTemp,"select count(*) from ncmautopri  where parentcode = '%s' and shopcode = %s ",parentcode,shopcode);
	
	// printf("caTemp = %s\n",caTemp);	
	pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &lCount1);
	// printf("lCount1 = %d\n",lCount1);
	char groupcodemax[32];
	char groupcodemax2[32];
	if(lCount1>0){
	
	sprintf(caTemp,"select MAX(groupcode) from ncmautopri  where parentcode = '%s' and shopcode = %s",parentcode,shopcode);//找同类最大值
	// printf("caTemp = %s\n",caTemp);	
	pasDbOneRecord(caTemp, 0, UT_TYPE_STRING, sizeof(groupcodemax), groupcodemax);
	// printf("lCount1=%lu,groupcode = %s,groupcodemax=%s\n",lCount1,groupcode,groupcodemax);
	
	sprintf(groupcodemax2,"%s%02d",parentcode,(atoi(groupcodemax)+1)%100);
	// printf("groupcodemax2=%s\n",groupcodemax2);
	if(strlen(groupcodemax2)%2 !=0){
		strcpy(groupcodemax,"0");
		strncat(groupcodemax, groupcodemax2, strlen(groupcodemax2));
	}else{
		strcpy(groupcodemax,groupcodemax2);
	}
	// printf("groupcodemax = %s\n",groupcodemax);
	sprintf(caTemp,"insert into ncmautopri(groupcode,groupname,parentcode,shopcode,systime,flag)values('%s','%s','%s','%s',%lu,%lu)",groupcodemax,groupname,parentcode,shopcode,time(0),atol(flag));
	
	sprintf(groupcode3,"%s%s",groupcode2,groupcodemax);
	// printf("groupcode3 = %s\n",groupcode3);
	
	lSid=utMd5Code(groupcode3,strlen(groupcode3),"ffff");
				   	  if(lSid>2000000000) lSid=lSid-2000000000;
				   	  if(lSid>2000000000) lSid=lSid-2000000000;
				   	  printf("lSid = %d\n",lSid);	
	sprintf(caTemp3,"select count(*) from ncsrvgroup  where groupname = '%s'",groupcode3);//查在单位表里是否存在
	// printf("caTemp3 = %s\n",caTemp3);	
	pasDbOneRecord(caTemp3, 0, UT_TYPE_ULONG, 4, &lCount3);
	// printf("lCount3 = %d\n",lCount3);
	if(lCount3 == 0){	
	sprintf(caTemp2,"insert into ncsrvgroup(groupid,groupname,dispname,addtime)values(%lu,'%s','%s',%lu)",lSid,groupcode3,groupname,time(0));//没有，插入
	}else{
	sprintf(caTemp2,"update ncsrvgroup set dispname='%s',moditime=%lu,flags=1 where groupname ='%s'",groupname,time(0),groupcode3);//查在单位表里是否存在，存在更新1
	}
	
	}else{
	sprintf(caTemp,"insert into ncmautopri(groupcode,groupname,parentcode,shopcode,systime)values('%s01','%s','%s','%s',%lu)",parentcode,groupname,parentcode,shopcode,time(0));
	
	sprintf(groupcode3,"%s%s01",groupcode2,parentcode);
	// printf("groupcode3 = %s\n",groupcode3);
	lSid=utMd5Code(groupcode3,strlen(groupcode3),"ffff");
				   	  if(lSid>2000000000) lSid=lSid-2000000000;
				   	  if(lSid>2000000000) lSid=lSid-2000000000;
				   	  printf("lSid = %d\n",lSid);
	sprintf(caTemp3,"select count(*) from ncsrvgroup  where groupname = '%s'",groupcode3);
	// printf("caTemp3 = %s\n",caTemp3);	
	pasDbOneRecord(caTemp3, 0, UT_TYPE_ULONG, 4, &lCount3);
	// printf("lCount3 = %d\n",lCount3);
	if(lCount3 == 0){					  
		sprintf(caTemp2,"insert into ncsrvgroup(groupid,groupname,dispname,addtime)values(%lu,'%s','%s',%lu)",lSid,groupcode3,groupname,time(0));
	}else{
		sprintf(caTemp2,"update ncsrvgroup set dispname='%s',moditime=%lu,flags=1 where groupname ='%s'",groupname,time(0),groupcode3);	
	}
	}
	// printf("caTemp = %s\n",caTemp);	
	psCur = pasDbOpenSql(caTemp,0);
	pasDbCloseCursor(psCur);
	
	
	// printf("caTemp2 = %s\n",caTemp2);	
	psCur = pasDbOpenSql(caTemp2,0);
	pasDbCloseCursor(psCur);
	
	
	}
	}
	else if(strcmp(update,"update")==0){
		if(strcmp(opt2,groupcode)!=0 && strcmp(groupcode,"01") !=0){
			sprintf(caTemp,"update ncmautopri set groupname='%s',systime=%lu where groupcode='%s' and shopcode='%s'",groupname,time(0),groupcode,shopcode);	
			// printf("caTemp = %s\n",caTemp);	
			psCur = pasDbOpenSql(caTemp,0);
			pasDbCloseCursor(psCur);
			
			sprintf(groupcode3,"%s%s",groupcode2,groupcode);
			// printf("groupcode3 = %s\n",groupcode3);
			sprintf(caTemp2,"update ncsrvgroup set dispname='%s',moditime=%lu where groupname ='%s'",groupname,time(0),groupcode3);	
			// printf("caTemp2 = %s\n",caTemp2);	
			psCur = pasDbOpenSql(caTemp2,0);
			pasDbCloseCursor(psCur);
		}
		else{
				// printf("update failed\n");
		}
	}
	else if(strcmp(update,"del")==0){
		//sprintf(caTemp,"select groupcode from ncmautopri where parentcode = '%s' ",groupcode );
		if(strcmp(opt2,groupcode)!=0 && strcmp(groupcode,"01") !=0){//判断跟节点不能删除
			sprintf(caTemp,"delete from ncmautopri where groupcode like '%s%c' and shopcode = '%s' ",groupcode,'%',shopcode);
			if(strlen(opt2)>0 && strlen(groupcode)==0 ){
				snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and groupcode like '%s%%'",opt2);
			}		
			// printf("caTemp = %s\n",caTemp);	
			psCur = pasDbOpenSql(caTemp,0);
			pasDbCloseCursor(psCur);
			
			sprintf(groupcode3,"%s%s",groupcode2,groupcode);
			// printf("groupcode3 = %s\n",groupcode3);
			sprintf(caTemp2,"update ncsrvgroup set flags=9,moditime=%lu where groupname like '%s%c' ",time(0),groupcode3,'%');	
			// printf("caTemp2 = %s\n",caTemp2);	
			psCur = pasDbOpenSql(caTemp2,0);
			pasDbCloseCursor(psCur);
		}
		else{
			// printf("del failed\n");
		}
	}
	else{
		}
		if(strlen(node)>0){
		sprintf(caTemp,"select id,groupname,groupcode,parentcode, shopcode,systime,flag from ncmautopri where parentcode = '%s' ",node );
		
		if(strlen(keyword)>0){
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and groupname like '%%%s%%'",keyword);
		}
		if(strlen(opt2)>0){
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and groupcode like '%s%%'",opt2);
		}
		if(strlen(shopcode)>0){
			 snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and shopcode = '%s' ",shopcode);
		}
		if(strlen(sort)>0 && strlen(dir)>0){
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " order by %s %s ",sort,dir);
		}
		
		}
		else{
		sprintf(caTemp,"select id,groupname,groupcode,parentcode, shopcode,systime,flag from ncmautopri where shopcode = '%s' ",shopcode );
		if(strlen(keyword)>0){
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and groupname like '%%%s%%'",keyword);
		}else if(strlen(opt2)>0){//根节点
				snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and groupcode = '%s'",opt2);
		}
		if(strlen(keyword)==0 && strlen(opt2)==0 ){//根节点
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and parentcode=''  ");
		}
//		if(strlen(opt2)>0){//根节点
//			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and groupcode = '%s'",opt2);
//		}
		if(strlen(sort)>0 && strlen(dir)>0){
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " order by %s %s ",sort,dir);
		}
		//snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " limit %lu, %lu ",atol(start),atol(limit));
		}
//		 printf("caTemp=%s\n",caTemp);
		psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn = 0;
		int iNum = 0;
		char groupname[32] = "";
		char groupcode[32] = "";
		char parentcode[32] = "";
		char shopcode[32] = "";
		int id = 0;
		int autodef = 0;
		long systime = 0;
		long  flag=0;
		char typename[32]="";
		char _stime[32] = "";
		while(0==(iReturn = pasDbFetchInto(psCur,
							UT_TYPE_LONG,sizeof(id),&id,
							UT_TYPE_STRING,31,groupname,
							UT_TYPE_STRING,31,groupcode,
							UT_TYPE_STRING,31,parentcode,
							UT_TYPE_STRING,31,shopcode,
							UT_TYPE_LONG,sizeof(systime),&systime,
							UT_TYPE_LONG,sizeof(flag),&flag
							  ))  || 1405==iReturn)
		{	
			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead,"dh",iNum,",");
			}
			strcpy(_stime, utTimFormat("%Y/%m/%d %H:%M:%S", systime));
			utPltPutLoopVarF(psDbHead,"systime",iNum,"%s", _stime);
			utPltPutLoopVarF(psDbHead,"groupname",iNum,"%s", groupname);
			utPltPutLoopVarF(psDbHead,"groupcode",iNum,"%s", groupcode);
			utPltPutLoopVarF(psDbHead,"parentcode",iNum,"%s", parentcode);
			utPltPutLoopVarF(psDbHead,"shopcode",iNum,"%s", shopcode);
			utPltPutLoopVarF(psDbHead,"id",iNum,"%d", id);
			utPltPutLoopVarF(psDbHead,"flag",iNum,"%d",flag);
			if(strlen(parentcode)>0){
			utPltPutLoopVarF(psDbHead,"leaf",iNum,"%s", "true");
			}else{
			utPltPutLoopVarF(psDbHead,"leaf",iNum,"%s", "false");
			}
		}
		utPltPutVarF(psDbHead,"totalCount","%lu", iNum);
		pasDbCloseCursor(psCur);
	}
	
	
	//utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_systems/ncm_autopri.htm");
	// printf("ncm_autopri  end!!!!\n");
	return 0;	  
}


//主菜单
int ncm_initmenu(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead){
	
printf("ncm_initmenu start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	// utMsgPrintMsg(psMsgHead);
	char keywords[32] = "";
	char start[12]="";
	char limit[12]="";
	char expexcel[12]="";
	char sdate[32]="";
	char edate[32]="";
	char username[32]="";
	unsigned long lsdate;
	unsigned long ledate;
	
	utMsgGetSomeNVar(psMsgHead, 2,
   				    "ip1",        	UT_TYPE_STRING,  sizeof(keywords)-1,    keywords,
					"expexcel",     UT_TYPE_STRING,  sizeof(expexcel)-1,    expexcel
					);
	
	char caTemp[256];
	long lCount1=0;
	int iNum=0;
	sprintf(caTemp,"select id, mainmenu from ncminitmenu where flag != 1");	

	//snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " order by userid desc");
	
	// printf("caTemp = %s\n",caTemp);	
	psCur = pasDbOpenSql(caTemp,0);
	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
	 
		int iReturn=0;
		iNum=0;
		char mainmenu[32];
		int id=0;

		while(0==(iReturn = pasDbFetchInto(psCur,
							UT_TYPE_LONG,sizeof(id),&id,
      	                     UT_TYPE_STRING,sizeof(mainmenu),mainmenu
      	                     ))  || 1405==iReturn)
		{	

			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead,"dh",iNum,",");
			}
			utPltPutLoopVarF(psDbHead,"id",iNum,"%lu", id);	
			utPltPutLoopVarF(psDbHead,"mainmenu",iNum,"%s", mainmenu);
			utPltPutLoopVarF(psDbHead,"inum",iNum,"%lu", iNum);

		}
		
		pasDbCloseCursor(psCur);
	}
	utPltPutVarF(psDbHead,"TotRec","%lu", iNum);
//	utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_systems/ncm_initmenu.htm");
	// printf("ncm_initmenu  end!!!!\n");
	return 0;	  
}


//子菜单
int ncm_initsubmenu(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead){
	
printf("ncm_initsubmenu start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	// utMsgPrintMsg(psMsgHead);
	char keywords[32] = "";
	char start[12]="";
	char limit[12]="";
	char expexcel[12]="";
	char sdate[32]="";
	char edate[32]="";
	char username[32]="";
	unsigned long lsdate;
	unsigned long ledate;
	
	utMsgGetSomeNVar(psMsgHead, 2,
   				    "ip1",        	UT_TYPE_STRING,  sizeof(keywords)-1,    keywords,
					"expexcel",     UT_TYPE_STRING,  sizeof(expexcel)-1,    expexcel
					);
	
	char caTemp[256];
	long lCount1=0;
	int iNum=0;
	sprintf(caTemp,"select sid, submenu,pid from ncminitsubmenu where flag != 1");	

	//snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " order by userid desc");
	
	// printf("caTemp = %s\n",caTemp);	
	psCur = pasDbOpenSql(caTemp,0);
	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
	 
		int iReturn=0;
		iNum=0;
		char submenu[32];
		int id=0;
		int pid=0;

		while(0==(iReturn = pasDbFetchInto(psCur,
							UT_TYPE_LONG,sizeof(id),&id,
      	                     UT_TYPE_STRING,sizeof(submenu),submenu,
							 UT_TYPE_LONG,sizeof(pid),&pid
      	                     ))  || 1405==iReturn)
		{	

			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead,"dh",iNum,",");
			}
			utPltPutLoopVarF(psDbHead,"id",iNum,"%lu", id);	
			utPltPutLoopVarF(psDbHead,"pid",iNum,"%lu", pid);	
			utPltPutLoopVarF(psDbHead,"submenu",iNum,"%s", submenu);
			utPltPutLoopVarF(psDbHead,"inum",iNum,"%lu", iNum);

		}
		
		pasDbCloseCursor(psCur);
	}
	utPltPutVarF(psDbHead,"TotRec","%lu", iNum);
//	utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/ncm_systems/ncm_initsubmenu.htm");
	// printf("ncm_initsubmenu  end!!!!\n");
	return 0;	  
}

//豫园首页
int ncm_yuyuan_home_ajax(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
 //	// printf("ncm_yuyuan_home_ajax start!!!!\n");
	 pasDbCursor *psCur;
	// utPltDbHead *psDbHead;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	
	char logoid[32] = "";
	char shopid[32] = "";
	char limit1 [32] ="";
	char limit2 [32] ="";
	char level [32] ="";
	char tsid [32] ="";
	char caplateid [32] ="";
	char title [128] ="";
	char update [32] ="";
	int plateid = 0;
	// utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 8,
   				     "logoid",      UT_TYPE_STRING,  sizeof(logoid)-1,     logoid,
					 "shopid",      UT_TYPE_STRING,  sizeof(shopid)-1,     shopid,
					 "level",       UT_TYPE_STRING,  sizeof(level)-1,      level,
					 "limit1",      UT_TYPE_STRING,  sizeof(limit1)-1,     limit1,
					 "limit2",      UT_TYPE_STRING,  sizeof(limit2)-1,     limit2,
					 "plateid",   UT_TYPE_STRING,  sizeof(caplateid)-1,  caplateid,
					 "tsid",      	UT_TYPE_STRING,  sizeof(tsid)-1,       tsid,
					 "update",      	UT_TYPE_STRING,  sizeof(update)-1,       update
					 );

	char caTemp [256] ="";
	plateid = 4;
	//strcpy(tsid,"1234567");
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	char caIp[24] = "", caPort[10] = "";
	strcpy(caIp,utComGetVar_sd(psShmHead,"AdminIp", "192.168.20.168"));
	strcpy(caPort,utComGetVar_sd(psShmHead,"AdminPort", "9080"));
	utPltPutVarF(psDbHead,"ipurl","http://%s:%s", caIp,caPort);		
	//是否在线
	int tReturn = 0;
	tReturn = ncmShopUserOnlineFlag(psShmHead, iFd,psMsgHead);
	// printf("********test******tReturn = %d \n",tReturn);
	utPltPutVarF(psDbHead,"user_online_flag","%d", tReturn);
	//地图位置
	char markInfo[32]="";
	char caLabel[128]="";
	char caMark[128]="";
	char *pValue;
	//char *ncmShopGetOnlineMark(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
	strcpy(caMark,(char *)ncmShopGetOnlineMark(psShmHead, iFd,psMsgHead));
	strcpy(markInfo,(char *)utComGetVar_sd(psShmHead,"NcmMark","No"));
	strcpy(caLabel,caMark);
		// printf("caLabel = %s\n",caLabel);
	if(strcasecmp(markInfo,"Yes") && strlen(caMark) > 0)
	{
//		ncSrvGetPortalMark(caMark,NCSRV_MARK_LOCATION,pValue);
//		strcpy(caLabel,pValue);
//   strcpy(caLabel,caMark);
//		printf("caLabel = %s\n",caLabel);
	}
	// if(strlen(update)>0){
	// strcpy(caLabel,"8");
	// }
//	sprintf(caTemp,"select sid from portplate where status = 0 ");
//	pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, sizeof(plateid)-1, &plateid);
	// printf("plateid=%d\n",plateid);
	unsigned int lplateid = ncmGetPlateidByTsid(psShmHead,atoll(tsid));
	// printf(" lplateid = %d \n", lplateid);
	utPltPutVarF(psDbHead,"plateid","%d", lplateid);	
		
	struct selfShop
    {
		int sid;
		char name[32];
		char img[128];
		int shopid;
		int level;
	};
	struct selfShop selfShop[20] = {
	};
	struct selfShop *p;
	p=selfShop;
	int getShopId =0;
	long getShopidLevel =0;
	int i=0;
	//getShopId=5;
	getShopId = ncSrvGetCid(psShmHead,psMsgHead);
	if(getShopId !=0){
	sprintf(caTemp,"select title  from ncmshop where id = %d ",getShopId);
	pasDbOneRecord(caTemp, 0, UT_TYPE_STRING, sizeof(title)-1, title);
	char *titleUTF = convert("GBK", "UTF-8", title);
	//printf("title=%s\n",title);
	utPltPutVarF(psDbHead,"title","%s", titleUTF);
	sprintf(caTemp,"select sid, name, imgpath, level,shopid from ncmcomindex where level != 0 and type = 1 and shopid = %d and plateid = %d ",getShopId,lplateid);
	if(strlen(caLabel)>0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and label = substring('%s',3,5) ",caLabel);
	}
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " order by level");
// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		int iNum2=0;
		long id =0;
		char name[32]="";
		char img[128]="";
		long level=0;
		char serno[32]="";
		long shopid =0;
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_LONG,4,&id,
      	                    UT_TYPE_STRING,31,name,
							UT_TYPE_STRING,127,img,
							UT_TYPE_LONG,4,&level,
							UT_TYPE_LONG,4,&shopid))  || 1405==iReturn)
		{
		iNum2++;
		p[iNum2-1].sid=id;
		p[iNum2-1].level=level;
		p[iNum2-1].shopid=shopid;
		strcpy(p[iNum2-1].img, img);
		}

		pasDbCloseCursor(psCur);
	}
	}
	

	sprintf(caTemp,"select sid, name, imgpath, level,shopid,label from ncmcomindex where level != 0 and type = 1 and plateid = %d ",lplateid);
	if(strlen(caLabel)>0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and substring('%s',3,5) ",caLabel);
	}
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " group by level order by level");
	
	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);

	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		
		int iReturn=0;
		int iNum=0;
		long id =0;
		char name[32]="";
		char img[128]="";
		long level=0;
		char serno[32]="";
		long shopid =0;
		char label[128]="";
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_LONG,4,&id,
      	                    UT_TYPE_STRING,31,name,
							UT_TYPE_STRING,127,img,
							UT_TYPE_LONG,4,&level,
							UT_TYPE_LONG,4,&shopid,
							UT_TYPE_STRING,127, label
							))  || 1405==iReturn)
		{	

		iNum++;
        for(i=0;i<20;i++){
		if(p[i].level == level){
		id = p[i].sid;
		level = p[i].level;
		shopid = p[i].shopid;
		strcpy(img,p[i].img);
		}
		}
		utPltPutLoopVarF(psDbHead,"id",iNum,"%lu", id);
		utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu", iNum);
		utPltPutLoopVarF(psDbHead,"level",iNum,"%lu",level);
		utPltPutLoopVarF(psDbHead,"shopid",iNum,"%lu",shopid);
		char *imgpathUTF = convert("GBK", "UTF-8", img);
		utPltPutLoopVarF(psDbHead,"img",iNum,"%s",imgpathUTF);
		char *labelUTF = convert("GBK", "UTF-8", label);
		utPltPutLoopVarF(psDbHead,"label",iNum,"%s",labelUTF);
		if(level<=4){
		utPltPutLoopVarF(psDbHead,"imgslider",iNum,"%s",imgpathUTF);
		utPltPutLoopVarF(psDbHead,"iNumslider",iNum,"%lu",iNum);
		
		}
		}
		utPltPutVarF(psDbHead,"TotRec","%d",iNum);
		pasDbCloseCursor(psCur);
	}
	
//		utPltShowDb(psDbHead);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncminit_ajax.htm");
		// printf("ncm_yuyuan_home_ajax  end!!!!\n");
      
	  return 0;	  
}
//系统刷新
int ncm_system_reset(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
// printf("ncm_system_reset  start!!!!\n");
utPltDbHead *psDbHead;
psDbHead= utPltInitDb();
// printf("ncSrvSysReset\n");
ncSrvSysReset(psShmHead);
//utPltPutLoopVarF(psDbHead,"success",iNum,"%s",true);
sleep(3);
utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm_systems/ncm_system_reset.htm");
// printf("ncm_system_reset  end!!!!\n");
return 0;
}


//调查问卷
int ncm_questions(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncm_questions start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char groupcode[32] = "";
	char mac[32] = "";
	//char sid[32] ="";
	char age[32] ="";
	char edu[32] ="";
	char readtype1[64] ="";
	char readtype2[64] ="";
	char readtype3[64] ="";
	char readtype4[64] ="";
	char readtype5[64] ="";
	char readtype6[64] ="";
	char readtype7[64] ="";
	char readtype8[64] ="";
	char readtype[64] ="";
	char getbook[64] ="";
	char getbook1[64] ="";
	char getbook2[64] ="";
	char getbook3[64] ="";
	char getbook4[64] ="";
	char bookmon[32] ="";
	char sex[32] ="";
	char readtime[32] ="";
	char username[32] ="";
	char tsid[32] ="";
	char plateid[32] ="";
	char plateName[32] ="";
	char flag[32] ="";
	char booketype[64] ="";
	char booketype1[64] ="";
	char booketype2[64] ="";
	char decript[256] ="";
	unsigned long lTime = time(0);
	long flags =0;
	// // utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 27,
					"groupcode",			UT_TYPE_STRING,  sizeof(groupcode)-1,		groupcode,
   				    "mac",       UT_TYPE_STRING,  sizeof(mac)-1,      mac,
					"sex",       UT_TYPE_STRING,  sizeof(sex)-1,       	sex,
					"age",	UT_TYPE_STRING,  sizeof(age)-1,age,
					"edu",        	UT_TYPE_STRING,  sizeof(edu)-1,       	edu,
					"readtype1",        	UT_TYPE_STRING,  sizeof(readtype1)-1,      	readtype1,
					"readtype2",        	UT_TYPE_STRING,  sizeof(readtype2)-1,      	readtype2,
					"readtype3",        	UT_TYPE_STRING,  sizeof(readtype3)-1,      	readtype3,
					"readtype4",        	UT_TYPE_STRING,  sizeof(readtype4)-1,      	readtype4,
					"readtype5",        	UT_TYPE_STRING,  sizeof(readtype5)-1,      	readtype5,
					"readtype6",        	UT_TYPE_STRING,  sizeof(readtype6)-1,      	readtype6,
					"readtype7",        	UT_TYPE_STRING,  sizeof(readtype7)-1,      	readtype7,
					"readtype8",        	UT_TYPE_STRING,  sizeof(readtype8)-1,      	readtype8,
					"readtime",     UT_TYPE_STRING,  sizeof(readtime)-1,    readtime,
					"username",     UT_TYPE_STRING,  sizeof(username)-1,    username,
					"booketype1",      UT_TYPE_STRING,  sizeof(booketype1)-1,     booketype1,
					"booketype2",      UT_TYPE_STRING,  sizeof(booketype2)-1,     booketype2,
					"getbook1",      UT_TYPE_STRING,  sizeof(getbook1)-1,     getbook1,
					"getbook2",      UT_TYPE_STRING,  sizeof(getbook2)-1,     getbook2,
					"getbook3",      UT_TYPE_STRING,  sizeof(getbook3)-1,     getbook3,
					"getbook4",      UT_TYPE_STRING,  sizeof(getbook4)-1,     getbook4,
					"bookmon",      UT_TYPE_STRING,  sizeof(bookmon)-1,     bookmon,
					"tsid",      UT_TYPE_STRING,  sizeof(tsid)-1,     tsid,
					"plateid",      UT_TYPE_STRING,  sizeof(plateid)-1,     plateid,
					"plateName",      UT_TYPE_STRING,  sizeof(plateName)-1,     plateName,
					"decript",      UT_TYPE_STRING,  sizeof(decript)-1,     decript,
					"flag",      UT_TYPE_STRING,  sizeof(flag)-1,     flag);
					  
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);	
	utPltPutVarF(psDbHead,"plateName","%s", plateName);	
	ncPortalOnline *onlineinfo;
	
    // printf("*******tsid = %s\n",tsid);
	onlineinfo = (ncPortalOnline *)malloc(sizeof(ncPortalOnline)+1);
    if(strlen(tsid) > 0)
    {
     onlineinfo = ncSrvGetOnlineUserByStrTsid(psShmHead,tsid);
     // printf("*******caName = %s \n",onlineinfo->caName);
	 strcpy(mac,(char *)ncmGetMacByTsid(psShmHead, atoll(tsid)));
	 // printf("*******mac = %s \n",mac);
	}
	if(strlen(flag)>0){
		char palteName1[128]="";
		sprintf(palteName1,"/%s/wenjian_diaocha.htm",plateName);
		// printf("palteName1 = %s\n",palteName1);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	
	}else{
	char caTemp[1024];
	char sql[1024] = "";
	long lCount1=0;
	long iReturn=0;
	
	memset(sql, 0, sizeof(sql));
	snprintf(sql, sizeof(sql), "select count(*) from ncmquestion where username='%s' and mac = '%s'", onlineinfo->caName,mac);
	// printf("sql = %s \n",sql);
	iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	// printf("iReturn = %d\n",iReturn);
	// printf("lCount1 = %d\n",lCount1);
	
	if(lCount1 == 0){
		// printf("add\n");
		snprintf(caTemp,sizeof(caTemp),"insert into ncmquestion (groupcode,username,mac,sex,age,edu,readtype,readtime,booketype,getbook,bookmon,systime)values('%s','%s','%s',%lu,%lu,%lu,'%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu',%lu,'%lu,%lu','%lu,%lu,%lu,%lu',%lu,%lu)", groupcode,onlineinfo->caName,mac,atol(sex),atol(age),atol(edu),atol(readtype1),atol(readtype2),atol(readtype3),atol(readtype4),atol(readtype5),atol(readtype6),atol(readtype7),atol(readtype8),atol(readtime),atol(booketype1),atol(booketype2),atol(getbook1),atol(getbook2),atol(getbook3),atol(getbook4),atol(bookmon),time(0));
		// printf("caTemp=%s\n",caTemp);	
		pasDbOpenSql(caTemp,0);
		//utPltPutVarF(psDbHead,"btn","%s", "已关注");
		//utPltPutVarF(psDbHead,"msg","%s", "成功加入关注");
		//pasDbCloseCursor(psCur);
	}else{
		// printf("update\n");
		snprintf(caTemp,sizeof(caTemp),"update ncmquestion set groupcode = '%s',username = '%s',mac='%s',sex=%lu,age=%lu,edu=%lu,readtype='%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu',readtime=%lu,booketype='%lu,%lu',getbook='%lu,%lu,%lu,%lu',bookmon=%lu,systime=%lu where mac = '%s'", groupcode,onlineinfo->caName,mac,atol(sex),atol(age),atol(edu),atol(readtype1),atol(readtype2),atol(readtype3),atol(readtype4),atol(readtype5),atol(readtype6),atol(readtype7),atol(readtype8),atol(readtime),atol(booketype1),atol(booketype2),atol(getbook1),atol(getbook2),atol(getbook3),atol(getbook4),atol(bookmon),time(0),mac);
		// printf("caTemp=%s\n",caTemp);	
		pasDbOpenSql(caTemp,0);
	
	}
	
	// if (onlineinfo)
	// free(onlineinfo);
	//utPltShowDb(psDbHead);
	//printf("11111\n");
	if(strlen(plateName)>0){
		char palteName1[128]="";
		sprintf(palteName1,"/%s/ncm_questions_submit.htm",plateName);
		// printf("palteName1 = %s\n",palteName1);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	}
	else{
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_questions_submit.htm");
	}
	}
	
	// printf("ncm_questions  end!!!!\n");
    return 0;	  
}

//商店详情
int ncmShops_shbook(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	// printf("ncmShops_shbook start!!!!\n");
	pasDbCursor *psCur;
	utPltDbHead *psDbHead;
	psDbHead= utPltInitDb();
	char shopid[32] = "";
	char logoid[32] ="";
	char groupid[32] ="";
	char dir[32] ="";
	char plateName[32] ="";
	char tsid[32] ="";
	char plateid[32] ="";
	char floor[32] ="";
	char url[255] ="";
	char address[255] ="";
	// // utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 10,
   				    "shopid",        UT_TYPE_STRING,  sizeof(shopid)-1,      	 shopid,
					"logoid",        UT_TYPE_STRING,  sizeof(logoid)-1,     	 logoid,
					"groupid",        UT_TYPE_STRING,  sizeof(groupid)-1,     	 groupid,
					"dir",        UT_TYPE_STRING,  sizeof(dir)-1,     	 dir,
					"tsid",        	 UT_TYPE_STRING,  sizeof(tsid)-1,      		 tsid,
					"plateid",       UT_TYPE_STRING,  sizeof(plateid)-1,     	 plateid,
					"address",       UT_TYPE_STRING,  sizeof(address)-1,     	 address,
					"url",       UT_TYPE_STRING,  sizeof(url)-1,     	 url,
					"floor",       UT_TYPE_STRING,  sizeof(floor)-1,     	 floor,
					"plateName",     UT_TYPE_STRING,  sizeof(plateName)-1,       plateName
					);
	char caTemp[1024];
	utPltPutVarF(psDbHead,"tsid","%s", tsid);
	utPltPutVarF(psDbHead,"plateid","%s", plateid);
	utPltPutVarF(psDbHead,"address1","%s", address);
	utPltPutVarF(psDbHead,"url1","%s", url);
	int tReturn = 0;
	tReturn = ncmShopUserOnlineFlag(psShmHead, iFd,psMsgHead);
	// printf("********test******tReturn = %d \n",tReturn);
	utPltPutVarF(psDbHead,"user_online_flag","%d", tReturn);  
	// printf("shopid = %s\n",shopid);	
	pasCvtGBK(2,address,caTemp,sizeof(address));
	strcpy(address,caTemp);
	pasCvtGBK(2,url,caTemp,sizeof(url));
	strcpy(url,caTemp);
	int iSum=0;
	int i =0;
	
	ncmshoptype *shoptype;	
	shoptype = (ncmshoptype *)malloc(sizeof(ncmshoptype)*100+1); 
    //商店id      商店对应类别总数                                    
   shoptype = (ncmshoptype *)ncmgetShopgroupidByShopid(psShmHead,atoi(shopid),&iSum);                 //API1取商店中所有商品小类
    if(strlen(plateid) >0){
	snprintf(caTemp, sizeof(caTemp), "select sid, name from ncmcomtype where flags = 0 and plateid = %d and sid in (",  atol(plateid));

	for (i = 0; i < iSum; i++)
    {
		if(i == iSum-1){
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " %d ",shoptype[i].sgroupid);    
		}
		else{
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " %d, ",shoptype[i].sgroupid);    
		}
		// printf(" shopid = %d sgroupid = %d sid = %d \n",shoptype[i].shopid,shoptype[i].sgroupid,shoptype[i].sid);     
    }
	snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " ) ");  
	// printf("iSum = %d \n",iSum);
	// printf("caTemp=%s\n",caTemp);
	psCur = pasDbOpenSql(caTemp,0);
	}
	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		int iNum=0;
		long sgroupid =0;
		char sgroupname[32]="";
	
		
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_LONG,4,&sgroupid,
      	                    UT_TYPE_STRING,31,sgroupname
							 ))  || 1405==iReturn)
		{	
			iNum++;
			utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",sgroupid);
			char *snameUTF = convert("GBK", "UTF-8", sgroupname);
			utPltPutLoopVarF(psDbHead,"sgroupname",iNum,"%s", snameUTF);
		}
		pasDbCloseCursor(psCur);
	}
	
	
	char sql[1024] = "";
	long lCount1=0;
	long iReturn=0;
	memset(sql, 0, sizeof(sql));
	snprintf(sql, sizeof(sql), "select count(*) from ncmshop where flags = 0");
	
	if(strlen(shopid) >0){
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and id = %lu",atol(shopid));
	}
	if(strlen(plateid) >0){
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and plateid = %d",atol(plateid));
	}
	// printf("sql = %s \n",sql);
	iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	// printf("iReturn = %d\n",iReturn);
	// printf("lCount1 = %d\n",lCount1);
	
	snprintf(caTemp,sizeof(caTemp),"select id,name,service_code,sgroupid,imgpath,floor, code,tel, address,level,plevel,type,stype, descr, detail, url from ncmshop where flags = 0");
	if(strlen(shopid) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and id = %lu",atol(shopid));
	}
	if(strlen(plateid) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and plateid = %d",atol(plateid));
	}	
	if(strlen(floor) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and floor = '%s'",floor);
	}	
	if(strlen(url) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and url = '%s'",url);
	}
	if(strlen(address) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and address = '%s'",address);
	}
	
	if(strlen(address) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " group by url");
	}
	else{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " group by address");
	}
  	// printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
	
	if(psCur == NULL){
		// printf("pscur is null\n");
		// printf("Do SQL Error, sqlcode is %d \n",  pasDbSqlCode(NULL));
	}
	if(psCur != NULL)
	{
		int iReturn=0;
		int iNum=0;
		long id =0;
		char name[128]="";
		char service_code[16]="";
		long sgroupid=0;
		char imgpath[128]="";
		char floor[32]="";
		char code[12]="";
		char tel[32]="";
		char address[255];
		long level=0;
		long plevel=0;
		long type=0;
		long stype=0;
		char descr[255]="";
		char detail[1024]="";
		char url[255]="";
		
		while(0==(iReturn = pasDbFetchInto(psCur,
      	                    UT_TYPE_LONG,4,&id,
      	                    UT_TYPE_STRING,sizeof(name),name,
							UT_TYPE_STRING,15,service_code,
							UT_TYPE_LONG,4,&sgroupid,
							UT_TYPE_STRING,127,imgpath,
							UT_TYPE_STRING,31,floor,
							UT_TYPE_STRING,11,code,
							UT_TYPE_STRING,31,tel,
							UT_TYPE_STRING,254,address,
							UT_TYPE_LONG,4,&level,
							UT_TYPE_LONG,4,&plevel,
							UT_TYPE_LONG,4,&type,
							UT_TYPE_LONG,4,&stype,
							UT_TYPE_STRING,254,descr,
							UT_TYPE_STRING,254,detail,
							UT_TYPE_STRING,254,url
							 ))  || 1405==iReturn)
		{	
			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead,"dh",iNum,",");
			}
			utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",id);
			char *nameUTF = convert("GBK", "UTF-8", name);
			utPltPutLoopVarF(psDbHead,"name",iNum,"%s", nameUTF);
			utPltPutLoopVarF(psDbHead,"service_code",iNum,"%s", service_code);
			utPltPutLoopVarF(psDbHead,"sgroupid",iNum,"%lu",sgroupid);
			char *floorUTF = convert("GBK", "UTF-8", floor);
			utPltPutLoopVarF(psDbHead,"floor",iNum,"%s", floorUTF);
			char *imgpathUTF = convert("GBK", "UTF-8", imgpath);
			utPltPutLoopVarF(psDbHead,"imgpath",iNum,"%s",imgpathUTF);
			utPltPutLoopVarF(psDbHead,"code",iNum,"%s", code);
			utPltPutLoopVarF(psDbHead,"tel",iNum,"%s", tel);
			char *addressUTF = convert("GBK", "UTF-8", address);
			utPltPutLoopVarF(psDbHead,"address",iNum,"%s", addressUTF);
			char *detailUTF = convert("GBK", "UTF-8", detail);
			utPltPutLoopVarF(psDbHead,"detail",iNum,"%s", detailUTF);
			char *descrUTF = convert("GBK", "UTF-8", descr);
			utPltPutLoopVarF(psDbHead,"descr",iNum,"%s",descrUTF);
			utPltPutLoopVarF(psDbHead,"level",iNum,"%lu",level);
			char *urlUTF = convert("GBK", "UTF-8", url);
			utPltPutLoopVarF(psDbHead,"url",iNum,"%s", urlUTF);
			utPltPutLoopVarF(psDbHead,"logoid",iNum,"%s", logoid);
		}
		pasDbCloseCursor(psCur);
	}
	
	snprintf(caTemp, sizeof(caTemp), "select count(*) from ncmcomodity where shgroupid = %s",  shopid);
	if(strlen(plateid) >0){
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and plateid = %d",atol(plateid));
	}
  	// printf("caTemp=%s\n",caTemp);
	int totalProducts=0;
	pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, sizeof(totalProducts)-1, &totalProducts);
	utPltPutVarF(psDbHead,"totalProducts","%d", totalProducts);
	
//	utPltShowDb(psDbHead);
	if(strlen(plateName)>0){
	char palteName1[128]="";
	sprintf(palteName1,"/%s/ncm_levels_logos_shops.htm",plateName);
	// printf("palteName1 = %s\n",palteName1);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,palteName1);
	}else{
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/suning/ncm_levels_logos_shops.htm");
	}
	// printf("ncmShops_shbook  end!!!!\n");
    return 0;	  
}
//单位树——供comp选择使用

int ncmTreeCorp_comp_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caPlate[128];
   	char caGroupname[32],caDispanme[64];
   	
   	char caName[32];
   	char caWhere[256];
   	char caSql[256];
   	char caCurPg[16],caTotRec[16]; 
   	int  iNum;
    char caShopids[1024];
    utPltDbHead *psDbHead;
   	int iReturn,i;
    char caTemp[1024],caDispname[128];
   	strcpy(caShopids,getDsShopids());
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
 //   utMsgPrintMsg(psMsgHead);

 
psDbHead= utPltInitDb();

  
    sprintf(caTemp,"select groupname,dispname from ncsrvgroup where 1=1 ");
    if(strlen(caShopids)>0){
      	sprintf(caTemp+strlen(caTemp)," and groupid in (%s) ",caShopids);
    }
  
//   printf("caTemp=%s\n",caTemp);
    psCur=pasDbOpenSql(caTemp,0);
  
    if(psCur==NULL){
   
    	return 0;
    }
     iNum=0;
    memset(caGroupname,0,sizeof(caGroupname));
    memset(caDispname,0,sizeof(caDispname));  
    iReturn = pasDbFetchInto(psCur,
                                   UT_TYPE_STRING,31,caGroupname,
                                   UT_TYPE_STRING,63,caDispname);
    while(iReturn == 0||iReturn==1405) { 
      if(iNum>0){
     		   		utPltPutLoopVar(psDbHead,"dhf",iNum+1,",");
   	  }
   	  if(strlen(caDispname)>0)
   	  {	
   	  	utPltPutLoopVar(psDbHead,"area",iNum+1,caDispname);
   		}
   		else
   		{
   				utPltPutLoopVar(psDbHead,"area",iNum+1,"未划分单位");
   		}	
   	  utPltPutLoopVar(psDbHead,"code",iNum+1,caGroupname);
   	 
        iNum++;
      memset(caGroupname,0,sizeof(caGroupname));
     memset(caDispname,0,sizeof(caDispname));  
     iReturn = pasDbFetchInto(psCur,
                                   UT_TYPE_STRING,31,caGroupname,
                                   UT_TYPE_STRING,63,caDispname);
    }
    pasDbCloseCursor(psCur);
    
    
  
    utPltPutVar(psDbHead,"rootname",utComGetVar_sd(psShmHead,"rootname","ALL"));
    	
//utPltShowDb(psDbHead);
    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs_group_select_tree.htm");
   

    return 0;
}