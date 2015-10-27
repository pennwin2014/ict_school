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
#include <pcap.h>
#include <errno.h>
#include <iconv.h>
#include <dirent.h>
#include "utoall.h"
#include "utoplt01.h"
#include "pasutl.h"
#include "pasdb.h"
#include "ncportal.h"      
#include "ncportalweb.h"

#define FILEPATH "/home/ncmysql/ncsrv/bin"
#define UPLOADFILEPATH "/home/ncmysql/ncsrv/upload"
static int ncmsepcharbydh(char *in,char caOut[200][256])
{
        char *p,*p2;
        long lCount;
        long m=0;
        lCount=0;
        if(strlen(in)==0) return 0;
        p=in;
        while(*p){
        //      printf("lCount=%d\n",lCount);
                if(*p==','){
                        caOut[lCount][m]='\0';
                        m=0;
                        p++;
                        lCount++;
                        if(lCount>=200) return lCount;
                }
                else{
                        if(m<256){
                        caOut[lCount][m]=*p;
                        p++;
                        m++;
                  }
                }
        }
        return lCount+1;
}

char *getDsShopids(){
        static char caReturn[1024];
        char caIds[1024],caTempid[1024];
        char caOpt[256];
        int iReturn,lId,iNum;
         pasDbCursor *psCur;
         char caObj[200][256];
         long i;
       long num=0;
       memset(caReturn,0,sizeof(caReturn));
      memset(caObj,0,200*256);

    iReturn = dsCltGetMyInfo(1,"Userid",&lId);
    if(iReturn!=0||lId<=0){
         return &caReturn[0];
    }

        memset(caOpt,0,sizeof(caOpt));
            pasDbOneRecord("select company from dsuser where id=:id",1,"id",UT_TYPE_LONG,lId,UT_TYPE_STRING,255,caOpt);
            if(strstr(caOpt,"all")||strlen(caOpt)==0){
                return &caReturn[0];
    }
    num=ncmsepcharbydh(caOpt,caObj);
    for(i=0;i<num;i++){
    if(i==0)
        sprintf(caReturn,"%s",caObj[0]);
        else{
           sprintf(caReturn+strlen(caReturn),",%s",caObj[i]);
        }
     }

     return &caReturn[0];
 }


char *getShopsnameByIds(utShmHead *psShmHead,char *in){
	static char caReturn[1024];
	long lCount=0;
	long i;
	char caOut[200][256];
	unsigned long lGroupid;
	char caTemp[1024];
	long iNum=0;
	ncPortalGroup *psGroup;
	memset(caReturn,0,sizeof(caReturn));
	printf("in=%s\n",in);
	memset(caOut,0,200*256);
	lCount=ncmsepcharbydh(in,caOut);
printf("caOut=%s\n",caOut[0]);
	for(i=0;i<lCount;i++){
		sscanf(caOut[i],"%lu",&lGroupid);

	 psGroup = (ncPortalGroup *) ncSrvGetGroupInfoById(psShmHead,lGroupid);
	 printf("lGroupid=%lu\n",lGroupid);
	 if(psGroup){
	 	   if(iNum==0){
	 	     sprintf(caReturn+strlen(caReturn),"%s",psGroup->caDisp);
	     }
	     else{
	     	 sprintf(caReturn+strlen(caReturn),",%s",psGroup->caDisp);
	    }
	    iNum++;
	  }
	}
	printf("caReturn=%s\n",caReturn);
	return &caReturn[0];
}


//参数说明:第一个参数：sql句柄，第二个参数：商超编码，第三个参数：用户类别
//返回值说明：用户ssid
char *ncmGetSsidByStype(utShmHead *psShmHead, char *stype, int type)
{
		pasDbCursor *psCur = NULL;
    
    long iReturn = 0; // 单记录查询
    char caTemp[4096] = "";
    char caSsid[32] = "";
    unsigned long lStype = 0;
    unsigned long lSsid = 0;
    char temp[32] = "";
    char cnfSsid[32] = "";
    
    if(strlen(stype)>0){
		    sprintf(caTemp, "select ssid, stype from ncmautotype where (shopcode+0)=%d ", atoi(stype));
		    printf("caTemp=%s \n",caTemp);
		  	psCur=pasDbOpenSql(caTemp,0);
		    if(psCur==NULL){
		          return 0;
		    }
		  	iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(caSsid)-1,   caSsid,
		  	                             UT_TYPE_ULONG, sizeof(long),       &lStype);
		    while(iReturn==0||iReturn==1405){
		    	  lSsid = lStype%100%10;
		    	  if(lSsid==type){
		    	  		strcpy(temp, caSsid);	
		    	  }
		    	  
		    	  memset(caSsid, 0, sizeof(caSsid));
		    	  lStype = 0;
		    		iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(caSsid)-1,   caSsid,
		  	                                 UT_TYPE_ULONG, sizeof(long),       &lStype);	
		    }
		    pasDbCloseCursor(psCur);
    }
    // 从配置文件中读取ssid
    if(strlen(temp)>0){
    		strcpy(cnfSsid, temp);	
    }else{
    		strcpy(cnfSsid,utComGetVar_sd(psShmHead,"NcmStarffSSID",  "ICPIC"));	
    }
    //printf("cnfSsid = %s\n", cnfSsid);	
    
    return cnfSsid;
}

int ncTimeToLong(char *stime, unsigned long *lTime);
/*{
	 char ctime[20] = "";
	 
	 snprintf(ctime, sizeof(ctime), "%c%c%c%c%c%c%c%c%c%c%c%c%c%c", stime[0], stime[1], stime[2], stime[3], stime[5], stime[6], stime[8], stime[9], stime[11], stime[12], stime[14], stime[15], stime[17], stime[18]);	
	 *lTime = utTimStrToLong("%Y%m%d%H%M%S", ctime);
	 
	 return 0;
}*/

int ncStrToStr(char *stime, char *etime)
{
	 char ctime[32] = "";
	 snprintf(ctime, sizeof(ctime), "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", stime[16], stime[17], stime[18], stime[19], stime[20], stime[21], stime[22], stime[23], stime[24], stime[25], stime[26], stime[27], stime[28], stime[29], stime[30], stime[31]);	
	 strcpy(etime, ctime);
	 
	 return 0;
}

struct servicedata_s{
 		unsigned long userid;
 		int  onuid_count;   //在线用户数
 		int  endnum;      //终端用户数
 		char caServicecode[16];
 		char caCname[64];
 		long onstate;          //1 在线    2-离线
 		char caIp[16];
 		char caDate[16];
 		char caEmail[64];
 		char caAddress[128];
 		char caHandphone[32];
 		char caTel[64];
 		char caContact[32];
 		char caVersion[16];
 		long lMonflag;
 		long lUseflags;	
 		char caGroupname[32];
 		char caDemo[256];
 		long lGtype;
 		unsigned long groupid;
};
static iSort_corp;
static iSortby_corp;
static int ncsUtlSortOnCorp(char *p1,char *p2)
{
    int i;
    long long l;   
    struct servicedata_s psC1,psC2;
    memcpy(&psC1,p1,sizeof(struct servicedata_s));
    memcpy(&psC2,p2,sizeof(struct servicedata_s));  
    if(iSort_corp==3){  
		if(iSortby_corp==1)
		    return psC2.onstate-psC1.onstate;
		 else
		    return psC1.onstate-psC2.onstate;    
		  }
		  else if(iSort_corp==1){
		  	if(iSortby_corp==1)
		  		return strcmp(psC2.caServicecode,psC1.caServicecode);
		  	else
		  	  return strcmp(psC1.caServicecode,psC2.caServicecode);
		  	}
		  	else if(iSort_corp==2){
		  	if(iSortby_corp==1)
		  		return strcmp(psC2.caCname,psC1.caCname);
		  	else
		  	  return strcmp(psC1.caCname,psC2.caCname);
		  	}
		  	else if(iSort_corp==4){
		  	if(iSortby_corp==1)
		  	  return (psC2.onuid_count-psC1.onuid_count);
		  	else
		  	  return (psC1.onuid_count-psC2.onuid_count);
		  	}
		  	else{
		  	if(iSortby_corp==1)
		  		return strcmp(psC2.caServicecode,psC1.caServicecode);
		  	else
		  	  return strcmp(psC1.caServicecode,psC2.caServicecode);
		  	}
}

//显示系统用户管理
int ncmSysUserlist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  utMsgPrintMsg(psMsgHead);
	  
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  char caContent[64],caKtype[8],caCaseflags[8];
	  long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
	  char caName[64],caNetname[32],caTemp_d[128],caStatus[16];
	  char caTid[16],caKid[16];
	  long lSid,lTid;
	  char caPre[36];
	  long iNum;
	  long lktype,lcaseflags,ltid;
	  char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	  long preid,lId;
	  char caGroupname[32];
	  long lTime,lLevel,i;
	  char opt_desc[1024],caOpt[256]="";
	  char caTelphone[64],caHandphone[32],caGroup[32],caEmail[64],caDispname[32],caGroupid[32];
	  char groupcode[32]="";
	  struct groupdate{
 	    char id[16];
 	  	char name[32];
 	  };
 	  struct groupdate *pGdate;
 	 
 	  struct roledate{
 	    long id;
 	  	char name[512];
 	  };
 	  long lLen;
 	  struct roledate *pRole;
 	  long lRolecount,lGroupcount;
 	  
 	  struct optdate{
 	  		char opt[32];
 	  		char stype[8];	
 	  };
 	  struct optdate *pOpt;
	  char caStype[8] = "";
	  // 得到商家ID
    unsigned long lCid = 0;
    char caStyped[32] = "";
    char caOptd[32] = "";
     char caShopids[1024];
      strcpy(caShopids,getDsShopids());
	  dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
	  dsCltGetSessionValue(1, "opt", UT_TYPE_STRING,sizeof(caOptd)-1, caOptd);
	  dsCltGetSessionValue(1, "stype", UT_TYPE_STRING,sizeof(caStyped)-1, caStyped);
	  printf("lCid = %d\n", lCid);
	  printf("caOptd = %s\n", caOptd);
	  printf("caStyped = %s\n", caStyped);
	  
	  //用户角色        
  	pasDbOneRecord("select count(distinct id) from dsuserrole",0,UT_TYPE_LONG,4,&lCount);
 	  pRole=(struct roledate*)malloc(lCount*sizeof(struct roledate));
 	  if(pRole==NULL){
 	  	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","查询数据出错1 ");
        return 0;
 	  }
 	  
 	  sprintf(caTemp,"select b.id,a.dispname from dsuserrole b,dsrole a where a.id=b.roleid order by b.id");
 	  psCur = pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","查询数据出错2 ");
        return 0;
    }   
    lId=0;
    memset(caGroupname,0,sizeof(caGroupname));
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId,
                                 UT_TYPE_STRING,31,caGroupname);
    lRolecount=0;
    preid=-1;
    
    while(iReturn==0||iReturn==1405){
	    	if(preid!=lId){
		    		pRole[lRolecount].id=lId;
			    	strcpy(pRole[lRolecount].name,caGroupname);
			    	lRolecount++;
			    	preid=lId;
	      }else{
			      if(strlen(pRole[lRolecount-1].name)<490)
			     	sprintf(pRole[lRolecount-1].name+strlen(pRole[lRolecount-1].name),",%s",caGroupname);
	      }
	    	lId=0;
	    	memset(caGroupname,0,sizeof(caGroupname));
	    	iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId,
	                                 UT_TYPE_STRING,31,caGroupname);
    }
    pasDbCloseCursor(psCur);
	 
	  //管理区域
 	  pasDbOneRecord("select count(*) from ncgroup",0,UT_TYPE_LONG,4,&lCount);
 	  pGdate=(struct groupdate*)malloc(lCount*sizeof(struct groupdate));
 	  if(pGdate==NULL){
 	  	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","查询数据出错3 ");
        return 0;
 	  }
 	  
 	  sprintf(caTemp,"select groupid,groupname from ncgroup");
 	  psCur = pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","查询数据出错4 ");
        return 0;
    }   
    lId=0;
    memset(caGroupname,0,sizeof(caGroupname));
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId,
                                 UT_TYPE_STRING,31,caGroupname);
    lGroupcount=0;
    while(iReturn==0||iReturn==1405){
    	sprintf(pGdate[lGroupcount].id,"%d",lId);
    	strcpy(pGdate[lGroupcount].name,caGroupname);
    	lGroupcount++;
    	lId=0;
    	memset(caGroupname,0,sizeof(caGroupname));
    	iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId,
                                   UT_TYPE_STRING,31,caGroupname);
    }   
 	  pasDbCloseCursor(psCur);
	  
	  memset(caPre,0,sizeof(caPre));
	  iReturn = utMsgGetSomeNVar(psMsgHead,10,"groupid1", UT_TYPE_STRING,sizeof(caGroupid)-1,   caGroupid,
	  																				"groupcode",UT_TYPE_STRING,sizeof(groupcode)-1,   groupcode,
															              "limit",    UT_TYPE_STRING,sizeof(caLimit)-1,     caLimit,
															              "page",     UT_TYPE_STRING,sizeof(caPage)-1,      caPage,  
															              "del",      UT_TYPE_STRING,sizeof(caDel)-1,       caDel,
															              "dir",      UT_TYPE_STRING,sizeof(caDir)-1,       caDir,
															              "keyword",  UT_TYPE_STRING,sizeof(caKeyword)-1,   caKeyword,
															              "selsid",   UT_TYPE_STRING,sizeof(caSelsid)-1,    caSelsid,			             
															              "start",    UT_TYPE_STRING,sizeof(caStart)-1,     caStart,
															              "del",      UT_TYPE_STRING,sizeof(caDel)-1,       caDel);  

	  if(strlen(caKeyword)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,63));
	  	strcpy(caKeyword,caTemp_d);
	  }          
	  
	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  psDbHead = utPltInitDb(); 
	  
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  		pasDbExecSqlF("delete from  dsuser  where id in(%s)",caSelsid);
	  }

	  sprintf(caTemp,"select count(*) from dsuser where 1=1 ");
    if(strlen(caGroupid)>0){
    		sprintf(caTemp+strlen(caTemp)," and groupid=%s  ",caGroupid);
    }
    if(strlen(caKeyword)>0){
    		sprintf(caTemp+strlen(caTemp)," and (name like '%c%s%c' or groupname like '%c%s%c' or dispname like '%c%s%c' or email like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
    }
    if(lCid>0){
    		sprintf(caTemp+strlen(caTemp)," and cid=%lu",lCid);	
    }
    if(strlen(caStyped)>0){
    	  sprintf(caTemp+strlen(caTemp)," and stype='%s'",caStyped);
    }
    if(strlen(caOptd)>0){
	  	  sprintf(caTemp+strlen(caTemp)," and opt like '%s%c' ",caOptd,'%');
	  }
	  if(strlen(caShopids)>0){
	  	sprintf(caTemp+strlen(caTemp)," and company in (%s) ",caShopids);
	  }
	  if(!utStrIsSpaces(groupcode))  // 商家权限查询
	  {
		 	  char caOpt[64]="";
		 	  sprintf(caOpt,"%s%s",ncmWebAutoDefGet16(psShmHead, iFd,psMsgHead,caStyped),groupcode);
				snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and opt like '%s%c'", caOpt, '%');
	  }	
    lCount=0;
    printf("caTemp=%s\n",caTemp);
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  pOpt=(struct optdate*)malloc(lCount*sizeof(struct optdate));
	  
	  //sprintf(caTemp,"select a.id,a.name,a.dispname,a.groupname,a.addtime,a.email,a.usrlevel,b.groupname,a.opt,c.aname from dsuser a left join ncgroup b on a.groupid=b.groupid left join ncmarea c on a.opt=cast(c.aid as char(12))  where 1=1 ");	
	  sprintf(caTemp,"select a.id,a.name,a.dispname,a.groupname,a.addtime,a.email,a.usrlevel,b.groupname,a.opt,a.stype from dsuser a left join ncgroup b on a.groupid=b.groupid  where 1=1 ");	
    if(strlen(caGroupid)>0){
    		sprintf(caTemp+strlen(caTemp)," and a.groupid=%s  ",caGroupid);
    }
    if(strlen(caKeyword)>0){
    		sprintf(caTemp+strlen(caTemp)," and (a.name like '%c%s%c' or a.groupname like '%c%s%c' or a.dispname like '%c%s%c' or a.email like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
    }
    if(lCid>0){
    		sprintf(caTemp+strlen(caTemp)," and a.cid=%lu",lCid);	
    }
    if(strlen(caShopids)>0){
	  	sprintf(caTemp+strlen(caTemp)," and company in (%s) ",caShopids);
	  }
    if(strlen(caStyped)>0){
    	  sprintf(caTemp+strlen(caTemp)," and stype='%s'",caStyped);
    }
    if(strlen(caOptd)>0){
	  	  sprintf(caTemp+strlen(caTemp)," and opt like '%s%c' ",caOptd,'%');
	  }
	  if(!utStrIsSpaces(groupcode))  // 商家权限查询
	  {
		 	  char caOpt[64]="";
		 	  sprintf(caOpt,"%s%s",ncmWebAutoDefGet16(psShmHead, iFd,psMsgHead,caStyped),groupcode);
				snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and opt like '%s%c'", caOpt, '%');
	  }	
    sprintf(caTemp+strlen(caTemp)," order by a.id desc limit %d,%d",lStartRec,lRowNum);
  	printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0561数据统计出错"),ncLang("0171查询数据出错"));
        return 0;
    }
                        
   	iReturn=0;
   	iNum=0;
   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {
		    memset(caGroupname,0,sizeof(caGroupname));
		    memset(caHandphone,0,sizeof(caHandphone));
		    memset(caTelphone,0,sizeof(caTelphone));
		    memset(caEmail,0,sizeof(caEmail));
		    memset(caDispname,0,sizeof(caDispname));
		    memset(caOpt,0,sizeof(caOpt));
		    memset(caStype,0,sizeof(caStype));
		    iReturn=pasDbFetchInto(psCur, UT_TYPE_LONG,  sizeof(long),          &lId,
	                                    UT_TYPE_STRING,sizeof(caName)-1,      caName,
	                                    UT_TYPE_STRING,sizeof(caDispname)-1,  caDispname,
	                                    UT_TYPE_STRING,sizeof(caGroup)-1,     caGroup,
	                                    UT_TYPE_LONG,  sizeof(long),          &lTime,
	                                    UT_TYPE_STRING,sizeof(caEmail)-1,     caEmail,
	                                    UT_TYPE_LONG,  sizeof(long),          &lLevel,
	                                    UT_TYPE_STRING,sizeof(caGroupname)-1, caGroupname,
	                                    UT_TYPE_STRING,sizeof(caOpt)-1,       caOpt,
	                                    UT_TYPE_STRING,sizeof(caStype)-1,     caStype);
		      	                              
		   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
        if(iNum>1){
       			utPltPutLoopVar(psDbHead,"dh",iNum,",");
        }
        utPltPutLoopVarF(psDbHead,"num", iNum,"%lu",iNum);
        utPltPutLoopVarF(psDbHead,"userid", iNum,"%lu",lId);
        utPltPutLoopVar(psDbHead,"username",iNum,caName);
        utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
        utPltPutLoopVar(psDbHead,"groupname",iNum,caGroup);
        utPltPutLoopVar(psDbHead,"email",iNum,caEmail);
        switch(lLevel){
        		case 1:	utPltPutLoopVar(psDbHead,"level",iNum,"系统管理员");break;
        		case 2: utPltPutLoopVar(psDbHead,"level",iNum,"普通管理员");break;
        		case 3: utPltPutLoopVar(psDbHead,"level",iNum,"一般操作员");break;
        		default:break;
        }
        utPltPutLoopVar(psDbHead,"addtime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",lTime));
        strcpy(pOpt[iNum-1].opt, caOpt);
        strcpy(pOpt[iNum-1].stype, caStype);
        for(i=0;i<lRolecount;i++){
	        	if(pRole[i].id==lId){
		        		utPltPutLoopVar(psDbHead,"role",iNum,pRole[i].name);
		        		break;
	        	}
        }
    }
    for(i=0; i<lCount; i++){
    	  if(strlen(pOpt[i].stype)>0){
    	  		unsigned long lAutodef = 0;
			  		sprintf(caTemp, "select autodef from ncmshopgtype where groupcode = '%s'", pOpt[i].stype);
			  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lAutodef);	
			  		if(lAutodef > 0){
			  			  char tempcode[32] = "";
			  			  char caTempname[32] = "";
			  			  ncStrToStr(pOpt[i].opt, tempcode);
			  			  sprintf(caTemp, "select groupname from ncmautopri where groupcode = '%s' and shopcode = '%s'", tempcode, pOpt[i].stype);
			  		    pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(caTempname)-1,caTempname);	
			  				utPltPutLoopVar(psDbHead,"opt",i+1,caTempname);
			  		}else{ // 不是自定义
			  			  char caTempname[32] = "";
			  			  sprintf(caTemp, "select aname from ncmarea where aid = '%s'", pOpt[i].opt);
			  		    pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(caTempname)-1,caTempname);	
			  				utPltPutLoopVar(psDbHead,"opt",i+1,caTempname);	
			  		}	
    	  }else{
    	  		char caTempname[32] = "";
	  			  sprintf(caTemp, "select aname from ncmarea where aid = '%s'", pOpt[i].opt);
	  		    pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(caTempname)-1,caTempname);	
	  				utPltPutLoopVar(psDbHead,"opt",i+1,caTempname);	
    	  }
    }
    pasDbCloseCursor(psCur);
	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);

    free(pGdate); 
    free(pRole);
    free(pOpt); 

	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncm_sysuser_list.htm");
	  return 0;
}

//显示角色功能列表列表
int ncmDispRoleFunlist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  utMsgPrintMsg(psMsgHead);
	  
	  pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();
    int iret=0;
    int iReturn,iNum,iSum1,iSum2,k;
    long lStatus,lId=0,i,j;
    char caResult1[16000];
    char caResult2[16000];
    char caResult3[16000];
    char caSmt[256];
    char caSmtd[256];
    long lPasid;
    char caPasid[16],caRoleid[16];
    char caName[32],caDispname[1024],caName2[32],caName3[32];
    char *p,*p2;
    struct dscltdata{
    	  unsigned long id;
    		char mainmenu[32];
    		char submenu[2048];	
    };
    struct dscltdata *dsdata;
    char caTemp[1024]="";
    unsigned long lCount=0;
    char caMainmenu[32] = "";
    char caSubmenu[32] = "";
    
    // 得到商家opt
    char caStyped[32] = "";
    char caOptd[32] = "";
    unsigned long lIdd = 0;
    dsCltGetSessionValue(1, "id", UT_TYPE_LONG, sizeof(long), &lIdd);
	  dsCltGetSessionValue(1, "opt", UT_TYPE_STRING, sizeof(caOptd)-1, caOptd);
	  dsCltGetSessionValue(1, "stype", UT_TYPE_STRING, sizeof(caStyped)-1, caStyped);
	  printf("lIdd = %lu\n", lIdd);
	  printf("caOptd = %s\n", caOptd);
	  printf("caStyped = %s\n", caStyped);
    
    pasDbOneRecord("select id from dsappsrv",0,UT_TYPE_LONG,4,&lPasid);
    /* 根据Roleid和Pasid取相应的权限信息  */
    sprintf(caSmt,"select name from dsrole where id=%d ",atol(caRoleid));
    pasDbOneRecord(caSmt,0,UT_TYPE_STRING,31,caName2);
    
    sprintf(caTemp, "select count(*) from ncminitmenu where 1=1");
    pasDbOneRecord(caTemp,0,UT_TYPE_ULONG,sizeof(long),&lCount);
    dsdata = (struct dscltdata *)malloc(sizeof(struct dscltdata)*lCount + 1);
    for(i=0; i<lCount; i++){
    			dsdata[i].id=0;
    			strcpy(dsdata[i].mainmenu, "");
    			strcpy(dsdata[i].submenu, "");
    }
    
    memset(caTemp, 0, sizeof(caTemp));
    sprintf(caTemp, "select id, mainmenu from ncminitmenu where 1=1");
    psCur = pasDbOpenSql(caTemp, 0);
	  if(psCur==NULL){
	 		 return 0;	
	  }
    
    iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lId,
															  UT_TYPE_STRING, sizeof(caMainmenu)-1,  caMainmenu);
    iNum=0;
    while(0 == iret || 1405 == iret){
    			dsdata[iNum].id = lId;
    			strcpy(dsdata[iNum].mainmenu, caMainmenu);
    			//printf("dsdata[%d].id = %d\n", iNum,dsdata[iNum].id);
    			iNum++;
    			lId=0;
    			memset(caMainmenu, 0, sizeof(caMainmenu));
    			iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lId,
															        UT_TYPE_STRING, sizeof(caMainmenu)-1,  caMainmenu);
    }
    pasDbCloseCursor(psCur);
    
    memset(caTemp, 0, sizeof(caTemp));
    for(i=0; i<lCount; i++){
    		sprintf(caTemp, "select submenu from ncminitsubmenu where pid=%lu", dsdata[i].id);
    		psCur = pasDbOpenSql(caTemp, 0);
			  if(psCur==NULL){
			 		 return 0;	
			  }
			  
			  iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(caSubmenu)-1,  caSubmenu);
		    iNum=0;
		    while(0 == iret || 1405 == iret){
		    	    if(iNum==0){
		    					strcpy(dsdata[i].submenu, dsdata[i].mainmenu);
		    					sprintf(dsdata[i].submenu+strlen(dsdata[i].submenu),",%s",caSubmenu);
		    		  }else{
		    		  		sprintf(dsdata[i].submenu+strlen(dsdata[i].submenu),",%s",caSubmenu);	
		    		  }
		    			//printf("dsdata[%d].submenu = %s\n",i,dsdata[i].submenu);
		    			iNum++;
		    			memset(caSubmenu, 0, sizeof(caSubmenu));
		    			iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(caSubmenu)-1,  caSubmenu);
		    }
		    pasDbCloseCursor(psCur);
    }
    
    if(strlen(caStyped)>0){
		    sprintf(caSmt, "select lright,submenu from dsroleattr where roleid in (select roleid from dsuserrole where id = %lu)", lIdd);	
			  //printf("caSmt=%s  \n",caSmt);
		    iReturn = dsCltSqlRecordSet(caSmt,1,9999,&iSum2,caResult2);
				
		    p = caResult2;
		    iNum = 0;
		    j = 0;
		           
		    for(k=0;k<iSum2;k++) {
		        p = utStrGetRecordValue(p,2,UT_TYPE_STRING,sizeof(caName)-1,caName,
		                    							  UT_TYPE_STRING,sizeof(caDispname)-1,caDispname);
		        iNum++;
		        if(iNum>1){
		     	  	  utPltPutLoopVar(psDbHead,"dh",iNum,",");
		     	  }
		        utPltPutLoopVarF(psDbHead,"vara",iNum,"v%d",k);
		        utPltPutLoopVar(psDbHead,"namea", iNum,caName);
		        utPltPutLoopVar(psDbHead,"dispnamea", iNum,caDispname);
		    }
		    utPltPutVarF(psDbHead,"TotRec","%d",iSum2);
    }else{
    		iNum=0;
    		for(i=0; i<lCount; i++){
    				iNum++;
    				if(iNum>1){
	       	  	  utPltPutLoopVar(psDbHead,"dh",iNum,",");
	       	  }
	       	  utPltPutLoopVarF(psDbHead,"vara",iNum,"v%d",i);
            utPltPutLoopVar(psDbHead,"namea", iNum,dsdata[i].mainmenu);
            utPltPutLoopVar(psDbHead,"dispnamea", iNum,dsdata[i].submenu);
    		}
    		utPltPutVarF(psDbHead,"TotRec","%d",lCount);
    }
		
    utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncm_rolefun_list.htm");
    return 0;
}

int ncmDispRoleFunlist_v5(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead;
    int iReturn,iNum,iSum1,iSum2,k;
    long lStatus,lId,j;
    char caResult1[16000];
    char caResult2[16000];
    char caSmt[256];
    long lPasid;
    char caPasid[16],caRoleid[16];
    char caName[32],caDispname[32],caName2[32];
    char *p,*p2;
    utMsgPrintMsg(psMsgHead);

    pasDbOneRecord("select id from dsappsrv",0,UT_TYPE_LONG,4,&lPasid);
    /* 根据Roleid和Pasid取相应的权限信息  */
    sprintf(caSmt,"select name from dsrole where id=%d ",atol(caRoleid));
    pasDbOneRecord(caSmt,0,UT_TYPE_STRING,31,caName2);
    
    sprintf(caSmt,"select lright from dsroleattr where roleid=%s and pasid=%d",caRoleid,lPasid);
    iReturn = dsCltSqlRecordSet(caSmt,1,9999,&iSum1,caResult1);
    if(iReturn != 0 && iReturn != 1403 ) { 
        iSum1 = 0;
        strcpy(caResult1,"\0");
    }
    
    sprintf(caSmt,"select name,dispname from dscltright where pasid=%d",lPasid);
		printf("caSmt=%s  \n",caSmt);
    iReturn = dsCltSqlRecordSet(caSmt,1,9999,&iSum2,caResult2);
		
    psDbHead = utPltInitDb();
    p = caResult2;
    iNum = 0;
    j = 0;
           
    for(k=0;k<iSum2;k++) {
        p = utStrGetRecordValue(p,2,UT_TYPE_STRING,31,caName,
                    							  UT_TYPE_STRING,31,caDispname);
            iNum++;
	          if(iNum>1){
	       	  	  utPltPutLoopVar(psDbHead,"dh",iNum,",");
	       	  }
            utPltPutLoopVarF(psDbHead,"vara",iNum,"v%d",k);
            utPltPutLoopVar(psDbHead,"namea", iNum,caName);
            utPltPutLoopVar(psDbHead,"dispnamea", iNum,caDispname);
            if(iSum1>0 && utStrIsExistRecord(caResult1,1,1,caName)) {
                utPltPutLoopVar(psDbHead,"checka", iNum,"checked");
            }else{
           			utPltPutLoopVar(psDbHead,"checka", iNum,"checked");
            }  
    }
    utPltShowDb(psDbHead);
    utPltPutVarF(psDbHead,"TotRec","%d",iSum2);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncm_rolefun_list.htm");
    return 0;
}

//显示角色列表
int ncmRolelist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
		pasDbCursor *psCur;
		utPltDbHead *psDbHead;
		long iReturn,lCount;
		char caStart[16],caTemp[2048],caCurPg[16];
		char caContent[64],caKtype[8],caCaseflags[8];
		long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
		char caName[64],caNetname[32],caTemp_d[2048],caStatus[16];
		char caRightcheck[2048],caRolename[64];
		char caTid[16],caKid[16];
		long lSid,lTid;
		char caPre[36];
		long iNum;
		long lktype,lcaseflags,ltid;
		char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
		long preid,lId;
		char caGroupname[2048];
		long lTime,lLevel,i;
		char groupcode[32]="";
		long lPasid;
		pasLHashInfo sHashInfo;
		char *pHash;
		struct rData_s{
				unsigned long id;  
				char caFun[2048];
				char caUser[2048];
		};
		struct rData_s *psData;
		char caOut[200][2048];
		long lSum;
		char caRoleid[16],caUsername2[32]="";
		memset(caOut,0,200*2048);
		utMsgPrintMsg(psMsgHead);
		char caOutMain[200][2048];
		memset(caOutMain,0,200*2048);
		char mainmenu[200][36];
		memset(mainmenu,0,200*36);
		
		// 得到商家opt
    char caOptd[32] = "";
    unsigned long lCid = 0;
    char caStyped[32] = "";
	  dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
	  dsCltGetSessionValue(1, "opt", UT_TYPE_STRING, sizeof(caOptd)-1, caOptd);
	  dsCltGetSessionValue(1, "stype", UT_TYPE_STRING, sizeof(caStyped)-1, caStyped);
	  printf("lCid = %lu\n", lCid);
	  printf("caOptd = %s\n", caOptd);
	  printf("caStyped = %s\n", caStyped);
		
    /* 根据Roleid和Pasid取相应的权限信息  */
    pasDbOneRecord("select id from dsappsrv",0,UT_TYPE_LONG,4,&lPasid);
    dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
	 

	  iReturn = utMsgGetSomeNVar(psMsgHead,12, "limit",      UT_TYPE_STRING,sizeof(caLimit)-1,      caLimit,
																             "page",       UT_TYPE_STRING,sizeof(caPage)-1,       caPage,  
																             "dir",        UT_TYPE_STRING,sizeof(caDir)-1,        caDir,
																             "keyword",    UT_TYPE_STRING,sizeof(caKeyword)-1,    caKeyword,
																             "selsid",     UT_TYPE_STRING,sizeof(caSelsid)-1,     caSelsid,			             
																             "start",      UT_TYPE_STRING,sizeof(caStart)-1,      caStart,
																             "del",        UT_TYPE_STRING,sizeof(caDel)-1,        caDel,
																             "update",     UT_TYPE_STRING,sizeof(caUpdate)-1,     caUpdate,
																             "rolename",   UT_TYPE_STRING,sizeof(caRolename)-1,   caRolename,
																             "rightcheck", UT_TYPE_STRING,sizeof(caRightcheck)-1, caRightcheck,
																             "roleid",     UT_TYPE_STRING,sizeof(caRoleid)-1,     caRoleid,
																             "groupcode",  UT_TYPE_STRING,sizeof(groupcode)-1,    groupcode);  

	  if(strlen(caKeyword)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,63));
	  	strcpy(caKeyword,caTemp_d);
	  }      
	  if(strlen(caRightcheck)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caRightcheck,caTemp,2000));
	  	strcpy(caRightcheck,caTemp_d);
	  }              
	  if(strlen(caRolename)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caRolename,caTemp,63));
	  	strcpy(caRolename,caTemp_d);
	  }


	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  psDbHead = utPltInitDb(); 
	  
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
		    iReturn=pasDbExecSqlF("delete from dsuserrole where roleid  in(%s) and roleid!=101 ",caSelsid);
		  	iReturn=pasDbExecSqlF("delete from dsroleattr where roleid  in(%s) and roleid!=101 ",caSelsid);
		  	
		  	if(iReturn==0){
		  			pasDbExecSqlF("delete from dsrole where id  in(%s) and id!=101 ",caSelsid);
		  	}
	  }
	  
	  // 增加/修改
	  printf("caRolename=%s  \n",caRolename);
	  if((strcmp(caUpdate,"update")==0)&&strlen(caRolename)>0){ 
		  	printf("11caRolename=%s  \n",caRolename);
		  	printf("caRightcheck=%s  \n",caRightcheck);
	//	  	lCount=sepcharbydh(caRightcheck,caOut);
		  	char *p;
		  	char *q;
		  	lCount=0;
		  	p=strtok(caRightcheck, "|");
		  	while(p){
		  			strcpy(caOut[lCount],p);
		  			strcpy(caOutMain[lCount], caOut[lCount]);
		  			lCount++;
		  			p=strtok(NULL, "|");
		  	}
		  	printf("lCount=%d\n",lCount);
		  	for(i=0; i<lCount; i++){
		  			q=strtok(caOutMain[i], ",");
		  			if(q){
		  					strcpy(mainmenu[i], q);
		  		  }
		  		  //printf("mainmenu = %s\n",mainmenu);
		  	}
		  	
		  	sprintf(caTemp,"select id from dsrole where name='%s' ",caRolename);
		  	printf("判断角色是否存在caTemp=%s\n",caTemp);
		  	lId=0;
		  	pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lId);
		  	printf("lId = %lu\n", lId);
		  	if(lId==0){  // 增加
			  		lId = pasGetSid("dsrole","id");
			  		if(strlen(caStyped)>0){
			  			 if(strlen(caOptd)>0){
			  			 	   if(strlen(caOptd)>16){
			  			 	   		char optd[32] = "";
			  	  					ncStrToStr(caOptd, optd);
			  	  					iReturn = pasDbExecSqlF("insert into dsrole(id,name,dispname,descr,addtime,moditime,stype,pgroupcode) values (%lu,'%s','%s','%s',%lu,%lu,'%s','%s')",
		                                             lId,caRolename,caRolename,caRolename,time(0),time(0),caStyped,optd);		
			  			 	   }else{
								  		iReturn = pasDbExecSqlF("insert into dsrole(id,name,dispname,descr,addtime,moditime) values (%lu,'%s','%s','%s',%lu,%lu)",
									                                             lId,caRolename,caRolename,caRolename,time(0),time(0)); 
							     }                               
		        	 }else{
		        	 			iReturn = pasDbExecSqlF("insert into dsrole(id,name,dispname,descr,addtime,moditime) values (%lu,'%s','%s','%s',%lu,%lu)",
							                                             lId,caRolename,caRolename,caRolename,time(0),time(0));  	
		        	 }
		        }else{
		        		iReturn = pasDbExecSqlF("insert into dsrole(id,name,dispname,descr,addtime,moditime) values (%lu,'%s','%s','%s',%lu,%lu)",
				                                             lId,caRolename,caRolename,caRolename,time(0),time(0));	
		        }
		        if(iReturn==0){
			        	pasDbExecSqlF("delete from dsroleattr where roleid=%d ",lId);
			        	for(i=0;i<lCount;i++){
			        			if(strlen(caOut[i])>0){
			        				  pasDbExecSqlF("insert into dsroleattr(roleid,pasid,flags,lright,submenu) values(%d,%d,0,'%s','%s')",lId,lPasid,mainmenu[i],caOut[i]);
			        			}
			        	}
		        }                                     
		  	} else { // 修改
		  		  if(strlen(caStyped)>0){
				  		  if(strlen(caOptd)>0){
				  		  		if(strlen(caOptd)>16){
				  		  				char optd[32] = "";
			  	  						ncStrToStr(caOptd, optd);	
			  	  						iReturn = pasDbExecSqlF("update dsrole set name='%s',dispname='%s',descr='%s',moditime=%lu,stype='%s',pgroupcode='%s' where id=%d ",  			
					                                             caRolename,caRolename,caRolename,time(0),caStyped,optd,lId);
				  		  		}else{
				  		  				iReturn = pasDbExecSqlF("update dsrole set name='%s',dispname='%s',descr='%s',moditime=%lu where id=%d ",  			
					                                             caRolename,caRolename,caRolename,time(0),lId);			
				  		  		}
				  		  }else{
				  		  		iReturn = pasDbExecSqlF("update dsrole set name='%s',dispname='%s',descr='%s',moditime=%lu where id=%d ",  			
					                                             caRolename,caRolename,caRolename,time(0),lId);	
				  		  }
			      }else{
			      		iReturn = pasDbExecSqlF("update dsrole set name='%s',dispname='%s',descr='%s',moditime=%lu where id=%d ",  			
					                                             caRolename,caRolename,caRolename,time(0),lId);	
			      }
	          pasDbExecSqlF("delete from dsroleattr where roleid=%d ",lId);
	        	for(i=0;i<lCount;i++){
	        		 if(strlen(caOut[i])>0){
	        		 	   pasDbExecSqlF("insert into dsroleattr(roleid,pasid,flags,lright,submenu) values(%d,%d,0,'%s','%s')",lId,lPasid,mainmenu[i],caOut[i]);	
	        		 }
	        	}                                   
		  	}
	  }
	  //授权  
	  if((strcmp(caUpdate,"grant")==0)&&(strlen(caRoleid)>0)){
	//	  	lCount=sepcharbydh(caRightcheck,caOut);
				char *p;
		  	lCount=0;
		  	p=strtok(caRightcheck, ",");
		  	while(p){
		  			strcpy(caOut[lCount],p);
		  			lCount++;
		  			p=strtok(NULL, ",");
		  	}
		  	iReturn=pasDbExecSqlF("delete from dsuserrole where roleid=%d",atol(caRoleid));
		  	if(lCount>0){
			  		if(iReturn==0){
				  			for(i=0;i<lCount;i++){
				  					pasDbExecSqlF("insert into dsuserrole(id,roleid,addtime,addby) values(%d,%d,%d,'')",atol(caOut[i]),atol(caRoleid),time(0));
				  			}
			  		}
		  	}
	  }
	  
	  //将权限功能列表放到HASH表中
		lCount=0;
		sprintf(caTemp, "select count(*) from dsrole where 1=1");
	  printf("caTemp0 = %s\n", caTemp);
		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
		if(lCount<100){
				lSum=100;
		} else {
				lSum=lCount*1.2;
		}
		printf("lSum=%d\n",lSum);
		pHash = (unsigned char *)pasLHashInit(lSum,lSum,sizeof(struct rData_s),0,4); 
		if(pHash==NULL){
				printf("init hash error\n");
				return 0;
		}
		
		sprintf(caTemp, "select roleid,submenu from dsroleattr where pasid=%d order by lright",lPasid);
 	  printf("caTemp1 = %s\n", caTemp);
 	  psCur = pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","查询数据出错2");
        return 0;
    }   
    lId=0;
    memset(caGroupname,0,sizeof(caGroupname));
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId,
                                 UT_TYPE_STRING,sizeof(caGroupname)-1,caGroupname);

    while(iReturn==0||iReturn==1405){
	    	psData = (struct rData_s *)pasLHashLookA(pHash,&lId); 
	    	if(psData){
		    		if(strlen(psData->caFun)==0){
		    				strcpy(psData->caFun,caGroupname);
		    		}
		    		else{
		    				sprintf(psData->caFun+strlen(psData->caFun),"|%s",caGroupname);
		    		}
	    	}
	    	
	    	lId=0;
	    	memset(caGroupname,0,sizeof(caGroupname));
	    	iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId,
	                                   UT_TYPE_STRING,sizeof(caGroupname)-1,caGroupname);
    }
    pasDbCloseCursor(psCur);
		    
    //将角色对象放到hash中
	  sprintf(caTemp,"select a.roleid,b.dispname from dsuserrole a,dsuser b where a.id=b.id ");
	  snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " group by b.dispname,a.roleid");
 	  psCur = pasDbOpenSql(caTemp,0);
    printf("caTemp2=%s\n",caTemp);
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","查询数据出错2 ");
        return 0;
    }
    lId=0;
    memset(caGroupname,0,sizeof(caGroupname));
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId,
                                 UT_TYPE_STRING,sizeof(caGroupname)-1,caGroupname);

    while(iReturn==0||iReturn==1405){
	    	psData = (struct rData_s *)pasLHashLookA(pHash,&lId); 
	    	if(psData){
		    		if(strlen(psData->caUser)==0){
		    				strcpy(psData->caUser,caGroupname);
		    		}
		    		else{
		    				sprintf(psData->caUser+strlen(psData->caUser),",%s",caGroupname);
		    		}
	    	}
	    	
	    	lId=0;
	    	memset(caGroupname,0,sizeof(caGroupname));
	    	iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId,
	                                   UT_TYPE_STRING,sizeof(caGroupname)-1,caGroupname);
    }
    pasDbCloseCursor(psCur);
	  printf("psData->caUser = %s\n", psData->caUser);
	  
	  sprintf(caTemp,"select count(*) from dsrole where 1=1 ");
    if(strlen(caKeyword)>0){
    	 sprintf(caTemp+strlen(caTemp)," and (name like '%c%s%c') ",'%',caKeyword,'%');
    }
    if(strlen(caStyped)>0){
    		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and stype = '%s'", caStyped);	
    }
    if(strlen(caOptd)>0){
    		char optd[32] = "";
	  	  ncStrToStr(caOptd, optd);
	  		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and pgroupcode like '%s%c'", optd, '%');			
    }
    if(!utStrIsSpaces(groupcode))  // 商家权限查询
	  {
				snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and pgroupcode like '%s%c'", groupcode, '%');
	  }
    lCount=0;
    printf("caTemp=%s\n",caTemp);
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  sprintf(caTemp,"select id,name from dsrole where 1=1 ");
    if(strlen(caKeyword)>0){
    	 sprintf(caTemp+strlen(caTemp)," and (name like '%c%s%c') ",'%',caKeyword,'%');
    }
    if(strlen(caStyped)>0){
    		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and stype = '%s'", caStyped);	
    }
    if(strlen(caOptd)>0){
    		char optd[32] = "";
	  	  ncStrToStr(caOptd, optd);
	  		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and pgroupcode like '%s%c'", optd, '%');			
    }
    if(!utStrIsSpaces(groupcode))  // 商家权限查询
	  {
				snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp), " and pgroupcode like '%s%c'", groupcode, '%');
	  }
    sprintf(caTemp+strlen(caTemp)," order by name limit %d,%d",lStartRec,lRowNum);
  	printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0561数据统计出错"),ncLang("0171查询数据出错"));
        return 0;
    }                            
   	iReturn=0;
   	iNum=0;
   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {
   	
    lId=0;
    memset(caName,0,sizeof(caName));
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId,
                                 UT_TYPE_STRING,31,caName);
                                      
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
        if(iNum>1){
       			utPltPutLoopVar(psDbHead,"dh",iNum,",");
        }
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
        utPltPutLoopVarF(psDbHead,"userid",iNum,"%lu",lId);
        utPltPutLoopVar(psDbHead,"username",iNum,caName);
          
        psData=pasLHashLook(pHash,&lId); 
        if(psData){
	        	utPltPutLoopVar(psDbHead,"fun",iNum,psData->caFun);
	        	utPltPutLoopVar(psDbHead,"dx",iNum,psData->caUser);
        }
    }
    pasDbCloseCursor(psCur);
	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
    ncmWriteSysLog(psMsgHead,caUsername2,"06","成功","查看系统权限角色列表信息");

 		free(pHash); 
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncm_role_list.htm");
	  return 0;
}
// 显示增加/修改用户的界面  
int ncmWebAdminDispUserForm(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    unsigned long lId,lGroupid,lTime,lTime1,lUserid,lLevel2;
    char caName[32],caGroup[32],caDispname[32],caEmail[64],caUserid[16];
    char caWhere[256],caRoleid[16],caGroupid[16];
    char *p;
    char caResult[16000],caResult1[8192],caSmt[256],fac_desc[64];
    utPltDbHead *psDbHead;
    char *pBuf,*pSelect,caLevel[12];
   	int iReturn,iRetSum,i,j,k,iSum1,iSum2,iNum,iReturn1;
   	char caTemp[1024];
    long lLevel,cid = 0;
    char caPriv[32] = "";
    char caPriv_man[64] = "";
    char caStype[32]="";
    char *qtd = NULL;
		char ptd[32] = "";
    char *pBuf1;
    pBuf1 = (char *)malloc(sizeof(char)*1024);  
		iReturn = dsCltGetMyInfo(1,"usrlevel",&lLevel);
		lLevel2=lLevel;
		
#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
#endif

    // 得到账号ID
    unsigned long lCid = 0;
    char caOptd[255] = "";
    char caStyped[32] = "";
    unsigned long lIdd = 0;
    dsCltGetSessionValue(1, "id", UT_TYPE_LONG, sizeof(long), &lIdd);
	  dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
	  dsCltGetSessionValue(1, "opt", UT_TYPE_STRING, sizeof(caOptd)-1, caOptd);
	  dsCltGetSessionValue(1, "stype", UT_TYPE_STRING, sizeof(caStyped)-1, caStyped);
	  printf("lIdd = %lu\n", lIdd);
	  printf("lCid = %lu\n", lCid);
	  printf("caOptd = %s\n", caOptd);
	  printf("caStyped = %s\n", caStyped);

    psDbHead = utPltInitDb();
   	iReturn = utMsgGetSomeNVar(psMsgHead,1,"userid", UT_TYPE_STRING, 15, caUserid);
    sscanf(caUserid,"%u",&lUserid);
    if(!utStrIsSpaces(caUserid)) { // 修改 
        // lUserid = atol(caUserid);
        sprintf(caWhere,"id=%lu",lUserid);
        iReturn = dsCltUserList(1,1,&iRetSum,&iSum1,
            "id,name,dispname,groupname,groupid,addtime,moditime,email,usrlevel,company,cid,opt,stype",
            caWhere,"id", caResult);
        if(iReturn == 0) {
            p = utStrGetRecordValue(caResult,13,UT_TYPE_LONG,   4,&lUserid,
						                                    UT_TYPE_STRING,31,caName,
						                                    UT_TYPE_STRING,31,caDispname,
						                                    UT_TYPE_STRING,31,caGroup,
						                                    UT_TYPE_LONG,   4,&lGroupid,
						                                    UT_TYPE_LONG,   4,&lTime,
						                                    UT_TYPE_LONG,   4,&lTime1,
						                                    UT_TYPE_STRING,63,caEmail,
						                                    UT_TYPE_LONG,   4,&lLevel,
						                                    UT_TYPE_STRING,63,fac_desc,
						                                    UT_TYPE_LONG,   4,&cid,
						                                    UT_TYPE_STRING,31,caPriv,
						                                    UT_TYPE_STRING,31,caStype);
			  
            if(p) {
                utPltPutVarF(psDbHead,"userid","%lu",lUserid);
                utPltPutVar(psDbHead,"name",caName);
                utPltPutVar(psDbHead,"dispname",caDispname);
                utPltPutVar(psDbHead,"groupname",caGroup);
                utPltPutVarF(psDbHead,"groupid","%lu",lGroupid);
                utPltPutVar(psDbHead,"email",caEmail);
                utPltPutVar(psDbHead,"addtime",utTimFormat("%Y-%m-%d %H:%M:%S",lTime));
                utPltPutVar(psDbHead,"moditime",utTimFormat("%Y-%m-%d %H:%M:%S",lTime1));
                utPltPutVarF(psDbHead,"usrlevel","%lu",lLevel);
                utPltPutVar(psDbHead,"levelname",(char *)dsCltUsrLevel(lLevel));
           //     printf("fac_desc=%s\n",fac_desc);
                if(strlen(fac_desc)>0){
                	  printf("fac_desc=%s\n",fac_desc);
                    utPltPutVar(psDbHead,"fac_desc",getShopsnameByIds(psShmHead,fac_desc));
                }
                utPltPutVar(psDbHead,"fac",fac_desc);
     //           utPltPutVarF(psDbHead,"fac","%lu",cid);
                
                char tempP[32] = "";
                ncStrToStr(caPriv, tempP);
                utPltPutVar(psDbHead,"priv",tempP);
                sprintf(caTemp, "select groupname from ncmautopri where groupcode = '%s'", tempP);
                pasDbOneRecord(caTemp,0,UT_TYPE_STRING,63,caPriv_man);
                utPltPutVar(psDbHead,"priv_man",caPriv_man);
		                
                sprintf(caTemp,"select opt,aname from dsuser,ncmarea where cast(aid as char(12))=opt and id=%d ",lUserid);
                printf("caTemp=%s\n",caTemp);
                char caOpt[36],caOptname[64];
                memset(caOptname,0,sizeof(caOptname));
                memset(caOpt,0,sizeof(caOpt));
                pasDbOneRecord(caTemp,0,UT_TYPE_STRING,30,caOpt,
                                        UT_TYPE_STRING,60,caOptname);
                utPltPutVar(psDbHead,"opt",caOpt);
                utPltPutVar(psDbHead,"optname",caOptname);
                if(strlen(caOptd)>0){
		                if(strlen(caOpt)==2){
		                   	strcpy(pBuf1,"省级,1;地市级,2;区县级,3;"); 
		                   	pSelect=strdup(strUtlCvtInputSelect("rollevel","1",pBuf1,"changservice",74));
		                }else if(strlen(caOpt)==4){
			                  strcpy(pBuf1,"地市级,2;区县级,3;"); 
			                  pSelect=strdup(strUtlCvtInputSelect("rollevel","2",pBuf1,"changservice",74));
		                }else if(strlen(caOpt)==6){
			                  strcpy(pBuf1,"区县级,3;"); 
			                  pSelect=strdup(strUtlCvtInputSelect("rollevel","3",pBuf1,"changservice",74));
		                }else{
			                  strcpy(pBuf1,"请选择, ;省级,1;地市级,2;区县级,3;"); 
			                  pSelect=strdup(strUtlCvtInputSelect("rollevel","",pBuf1,"changservice",74));
		                }
                }else{
                	  strcpy(pBuf1,"请选择, ;省级,1;地市级,2;区县级,3;"); 
                		if(strlen(caOpt)==2){
		                  	pSelect=strdup(strUtlCvtInputSelect("rollevel","1",pBuf1,"changservice",74));
		                }else if(strlen(caOpt)==4){
		                  	pSelect=strdup(strUtlCvtInputSelect("rollevel","2",pBuf1,"changservice",74));
		                }else if(strlen(caOpt)==6){
		                  	pSelect=strdup(strUtlCvtInputSelect("rollevel","3",pBuf1,"changservice",74));
		                }else{
		                  	pSelect=strdup(strUtlCvtInputSelect("rollevel","",pBuf1,"changservice",74));
		                }	
                }
            }else {
 //               utPltFreeDb(psDbHead);
                utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncLang("0839增加用户"),ncLang("0773用户信息不正确"));
                return 0;
            }
        }else {
 //           utPltFreeDb(psDbHead);
            utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncLang("0839增加用户"),ncLang("0743用户不存在 %d"),iReturn);
            return 0;
        }            
    }else { // 增加新记录
        utPltPutVarF(psDbHead,"groupid","%lu",0);
        utPltPutVar(psDbHead, "groupname", "\0");
        utPltPutVar(psDbHead, "dispname", "");
        if(strlen(caOptd)==2){
        	  strcpy(pBuf1,"省级,1;地市级,2;区县级,3;"); 
            pSelect=strdup(strUtlCvtInputSelect("rollevel","1",pBuf1,"changservice",74));
        }else if(strlen(caOptd)==4){
        	  strcpy(pBuf1,"地市级,2;区县级,3;"); 
            pSelect=strdup(strUtlCvtInputSelect("rollevel","2",pBuf1,"changservice",74));
        }else if(strlen(caOptd)==6){
        	  strcpy(pBuf1,"区县级,3;"); 
            pSelect=strdup(strUtlCvtInputSelect("rollevel","3",pBuf1,"changservice",74));
        }else{
        	  strcpy(pBuf1,"请选择, ;省级,1;地市级,2;区县级,3;"); 
            pSelect=strdup(strUtlCvtInputSelect("rollevel"," ",pBuf1,"changservice",74));
        }
    }
    utPltPutVar(psDbHead,"rlevel", pSelect);
    if(pSelect) free(pSelect);
    // 显示组信息  
    /*sprintf(caGroupid,"%lu",lGroupid);
    pBuf = pasUtlCvtSqlSelectA("select groupname,groupid from ncgroup where groupid >0 and level=0 order by groupname",
               ncLang("0451全部,0;"));
    if(pBuf) {
        pSelect= pasUtlCvtInputSelect("groupid",caGroupid,pBuf);
        free(pBuf);
        utPltPutVar(psDbHead,"sgroup", pSelect);
        free(pSelect);
    }*/
    // 显示商超类别
    char *r = NULL;
    char *q = NULL;
    char s[16000] = "";
    char caTemp0[64] = "";
    char caTemp1[64] = "";
    pBuf = pasUtlCvtSqlSelectA("select groupname,groupcode from ncmshopgtype where type=2 order by groupname",
               "全部, ;");
    printf("pBuf = %s\n", pBuf);
    if(strlen(pBuf) != 0){
	  		q =	utStrSkipSpaces(pBuf);
	  		while(q && *q){
	  				q = utStrGetWord(q,caTemp0,64,";");
	  				strcpy(caTemp1, caTemp0);
	  				r = strtok(caTemp1, ",");
	  				if(strcmp(r, "全部")==0){
	  						strcat(s, "全部, ;");	
	  				}else if(strcmp(r, "00") != 0){
	  						strcat(s, caTemp0);
	  						strcat(s, ";");
	  				}
	  				q++;	
	  		}
	  }
    if(pBuf) {
        //pSelect= pasUtlCvtInputSelect("groupcode",caStype,s);
        pSelect=strdup(strUtlCvtInputSelect("groupcode",caStype,s,"checkAutodef",160));
        free(pBuf);
        utPltPutVar(psDbHead,"stype", pSelect);
        free(pSelect);
    }
    // 显示用户级别   
    sprintf(caLevel,"%d",lLevel);
    if(lCid>0){
    		lLevel2 = 2;	
    }
    if(lLevel2 == 2){
    	  //pSelect= pasUtlCvtInputSelect("usrlevel",caLevel,"普通管理员,2;一般操作员,3;");
    	  strcpy(pBuf1,"普通管理员,2;一般操作员,3;");
    	  pSelect=strdup(strUtlCvtInputSelect("usrlevel",caLevel,pBuf1,"",160));
    }else{
        //pSelect= pasUtlCvtInputSelect("usrlevel",caLevel,"系统管理员,1;普通管理员,2;");
        strcpy(pBuf1,"系统管理员,1;普通管理员,2;");
        pSelect=strdup(strUtlCvtInputSelect("usrlevel",caLevel,pBuf1,"",160));
    }
    utPltPutVar(psDbHead,"slevel", pSelect);
    free(pSelect);
    
    // 显示角色信息 
    if(lUserid == 0) {
        iSum1 = 0;
    }else{
        sprintf(caSmt,"select roleid from dsuserrole where id= %lu ",lUserid);
        iReturn = dsCltSqlRecordSet(caSmt,1,9999,&iSum1,caResult1);
        if(iReturn != 0) iSum1 = 0;
        
        if(strlen(caResult1)>0){
						qtd = strtok(caResult1, ",");
						qtd = strtok(NULL, ",");
						strncpy(ptd,qtd,strlen(qtd)-1);
			  }
    }
    
    if(lUserid == lIdd){
    		iSum2 = 0;	
    }else{
		    sprintf(caSmt,"select id,dispname from dsrole where 1=1");
		    if(!utStrIsSpaces(caStyped)){
		    		snprintf(caSmt+strlen(caSmt), sizeof(caSmt)-strlen(caSmt), " and stype = '%s'", caStyped);	
		    }
		    if(!utStrIsSpaces(caOptd)){
		    	  char optd[32] = "";
		    	  ncStrToStr(caOptd, optd);
		    		snprintf(caSmt+strlen(caSmt), sizeof(caSmt)-strlen(caSmt), " and pgroupcode like '%s%c'", optd, '%');	
		    }
		    iReturn = dsCltSqlRecordSet(caSmt,1,9999,&iSum2,caResult);
		    if(iReturn != 0) iSum2 = 0;
		    printf("iSum2 = %d\n", iSum2);
		    printf("caResult = %s\n", caResult);	
    }	
    	
    iNum = 0;
    j = 0;
    p = caResult;
    for(k=0;k<iSum2;k++) {
        p = utStrGetRecordValue(p,2,UT_TYPE_STRING,12,caRoleid,
								                    UT_TYPE_STRING,31,caDispname);
								                    
        if(j==0) {
            iNum++;
            utPltPutLoopVar(psDbHead,"rolea", iNum,caRoleid);
            utPltPutLoopVar(psDbHead,"labela", iNum,caDispname);
            if(iSum1>0 &&  strcmp(ptd, caRoleid)==0) {
                utPltPutLoopVar(psDbHead,"checka", iNum,"checked");
                utPltPutLoopVar(psDbHead,"ckid", iNum,caRoleid);
            }
            j++;
        }
         else if(j==1) {
            utPltPutLoopVar(psDbHead,"roleb", iNum,caRoleid);
            utPltPutLoopVar(psDbHead,"labelb", iNum,caDispname);
            if(iSum1>0 && strcmp(ptd, caRoleid)==0) {
                utPltPutLoopVar(psDbHead,"checkb", iNum,"checked");
                utPltPutLoopVar(psDbHead,"ckid", iNum,caRoleid);
            }
            j=0;
        }
    }
    utPltPutVarF(psDbHead,"sumright","%lu",iSum2);
    if(j==1) {
        utPltPutLoopVar(psDbHead,"roleb", iNum,"noneb");
        utPltPutLoopVar(psDbHead,"dispnameb", iNum,ncLang("0818预留"));
        utPltPutLoopVar(psDbHead,"checkb", iNum,"disabled");
    }
    
//    utPltShowDb(psDbHead);
    
    if(lCid==0 && strlen(caOptd)==0 && strlen(caStyped)==0){
    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncadminuser.htm");	
    }else{
    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncadminuser2.htm");
    }
	  
    return 0;
}
/* 增加或修改记录信息  */
int ncmAdminUserSave(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  char caTemp[4096] = "";
	  char caTemp1[4096] = "";
	  char caTemp2[4096] = "";
    char caUserid[16];
    char caUsername[32];
    char caPassword1[32],caPassword2[32],caIp[16];
    char caGroupname[32],caDispname[32],caGroupid[16];
    char caLevel[16],caMyname[32];
    char caEmail[64];
    char caKey[32],caKey0[64];
    char caSum[16];
    char caSql[8192];
    char caVar[32];
    char caKey2[64],caKey22[64],fac_desc[64]="", fac[64] = "";
    char priv[32] = "";
    char priv_man[64] = "";
    char *p;
    unsigned long lTime,lUserid,lUsrlevel,lGroupid,lRoleid;
    int iReturn,i,iSum,lcid = 0;
    long lLevel;
    char caOpt[32] = "";
    char caStype[32] = "";
    unsigned long lAid = 0;
		char caGcode[32] = "";
		char caChecked[32] = "";
    utPltDbHead *psDbHead;
		iReturn = dsCltGetMyInfo(2,"usrlevel",&lLevel,
		                           "username",caMyname);
  	dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,16,caMyname);	
  	                            
#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
#endif

		// 得到账号ID
    unsigned long lCid = 0;
    char caOptd[32] = "";
    char caStyped[32] = "";
    unsigned long lIdd = 0;
    dsCltGetSessionValue(1, "id", UT_TYPE_LONG, sizeof(long), &lIdd);
	  dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
	  dsCltGetSessionValue(1, "opt", UT_TYPE_STRING, sizeof(caOptd)-1, caOptd);
	  dsCltGetSessionValue(1, "stype", UT_TYPE_STRING, sizeof(caStyped)-1, caStyped);
	  printf("lIdd = %d\n", lIdd);
	  printf("lCid = %d\n", lCid);
	  printf("caOptd = %s\n", caOptd);
	  printf("caStyped = %s\n", caStyped);
	  
    utMsgPrintMsg(psMsgHead);
    lTime = time(0);
   	iReturn = utMsgGetSomeNVar(psMsgHead,19,"userid",      UT_TYPE_STRING, 10, caUserid,
												   	                "name",        UT_TYPE_STRING, 31, caUsername,
												   	                "dispname",    UT_TYPE_STRING, 31, caDispname,
												   					        "password",    UT_TYPE_STRING, 31, caPassword1,
												   					        "password2",   UT_TYPE_STRING, 31, caPassword2,
												   						      "key2",        UT_TYPE_STRING, 31, caKey2,
												   			        		"key22",       UT_TYPE_STRING, 31, caKey22,
												   					        "groupid",     UT_TYPE_STRING, 10, caGroupid,
												   					        "usrlevel",    UT_TYPE_STRING, 31, caLevel,
												   					        "email",       UT_TYPE_STRING, 63, caEmail,
												   					        "fac",         UT_TYPE_STRING, 63, fac,
												   					        "fac_desc",    UT_TYPE_STRING, 63, fac_desc,
												   					        "priv",        UT_TYPE_STRING, sizeof(priv)-1, priv,
												   					        "priv_man",    UT_TYPE_STRING, 63, priv_man,
												   					        "sumright",    UT_TYPE_STRING, 10, caSum,
												   					        "clientip",    UT_TYPE_STRING, 15, caIp,
												   					        "opt",         UT_TYPE_STRING, 30, caOpt,
												   					        "groupcode",   UT_TYPE_STRING, 31, caStype,
												   					        "ckid",        UT_TYPE_STRING, 31, caChecked);
	 printf("fac=%s,fac_desc=%s\n",fac,fac_desc);
    sscanf(caUserid,"%u",&lUserid);
    lGroupid = atol(caGroupid);
    iSum = atol(caSum);
    if(strlen(fac)>0){ 
//    		lcid = atol(fac);
      lcid=0;
    }else{
    	  lcid = 0;
    		strcpy(fac_desc, "");	
    }
    if(strlen(caStyped)>0){
    	  if(strlen(priv)==0){
    	  	  strcpy(caOpt, "");
    	  }else{
    	  		sprintf(caOpt,"%s%s",ncmWebAutoDefGet16(psShmHead, iFd,psMsgHead,caStyped),priv);
    	  }
    		printf("caOpt = %s\n", caOpt);
    }else{
    	  unsigned long lAutodef = 0;
    		sprintf(caTemp, "select autodef from ncmshopgtype where groupcode = '%s'", caStype);
    		pasDbOneRecord(caTemp,0,UT_TYPE_ULONG,sizeof(long),&lAutodef);
    		if(lAutodef>0){
    			   char caGroupcode[32] = "";
    				 sprintf(caTemp, "select groupcode from ncmautopri where shopcode = '%s' and parentcode = ''", caStype);
    				 pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(caGroupcode)-1,caGroupcode);
    				 sprintf(caOpt,"%s%s",ncmWebAutoDefGet16(psShmHead, iFd,psMsgHead,caStype),caGroupcode);
    		}	
    }
    if(strcmp(caStype, " ") == 0){
    		strcpy(caStype, "");	
    }
    printf("caStype = %s\n", caStype);
    
    psDbHead = utPltInitDb();
    if(lGroupid == 0) {
        strcpy(caGroupname,"全部");
    } else {
        iReturn = pasDbOneRecord("select groupname from ncgroup where groupid = :gid",1,"gid",UT_TYPE_LONG,lGroupid, UT_TYPE_STRING,31,caGroupname);
        if(iReturn != 0) {
             utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncLang("0839增加用户"),ncLang("0900组不存在"));
             return 0;
        }
    }
    lUsrlevel = atol(caLevel);
    if(strcmp(caPassword1,caPassword2) !=0 ) {
         utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0839增加用户"),ncLang("0351口令不相等"));
         return 0;
    }
    
    if(strcmp(caKey2,caKey22) !=0 ) {
         utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0839增加用户"),ncLang("1556第二密码不相等 "));
         return 0;
    }
   
    printf("****ab111******\n");
    if(strcasecmp(utComGetVar_sd(psShmHead,"secondpasswd","No"),"YES") == 0){
	  	 printf("*******************开始1************************\n");
	  	 if(utStrIsSpaces(caUserid) || lUserid == 0) { /* 新增记录  */
	        lUserid = dsCltGetSeque();
	        if(lUserid > 0) {
	            if(utStrIsSpaces(caPassword1)) {
	               utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0839增加用户"),ncLang("0349口令不能为空"));
	               return 0;
	            }
	            
	            sprintf(caKey0,"%s%s",caUsername,caPassword1);
	            utMd5Ascii22(caKey0,strlen(caKey0),NULL,caKey);  /* 口令加密  */
	            if(lCid==0 && strlen(caOptd)==0 && strlen(caStyped)==0){
	            		sprintf(caSql,"<DSSQL>insert into dsuser (id,name,dispname,groupid,groupname,addtime,\
	                              moditime,lkey,lasttime,logcount,useflags,status,usrlevel,email,key2,company,cid,stype) values \
	                           (%lu,'%s','%s',%lu,'%s',%lu,%lu,'%s',%lu,0,0,0,%d,'%s','%s','%s',%d,'%s')</DSSQL>\n",
	                           lUserid,caUsername,caDispname,lGroupid,caGroupname,lTime,lTime,
	                           caKey,lTime,lUsrlevel,caEmail,caKey2,fac,lcid,caStype);	
	            }else {  // 判断是否是二级分权
	            		sprintf(caSql,"<DSSQL>insert into dsuser (id,name,dispname,groupid,groupname,addtime,\
	                              moditime,lkey,lasttime,logcount,useflags,status,usrlevel,email,key2,company,cid,stype) values \
	                           (%lu,'%s','%s',%lu,'%s',%lu,%lu,'%s',%lu,0,0,0,%d,'%s','%s','%s',%d,'%s')</DSSQL>\n",
	                           lUserid,caUsername,caDispname,lGroupid,caGroupname,lTime,lTime,
	                           caKey,lTime,lUsrlevel,caEmail,caKey2,fac,lCid,caStyped);	
	            }
	            printf("*******************开始2************************\n");
	            utStrAddF(caSql,"<DSSQL>insert into dsuserrole(id,roleid,addtime,addby) values \
	                              (%lu,%lu,%lu,'%s')</DSSQL>\n",
	                           lUserid,atol(caChecked),lTime,caMyname);
	                           
	            iReturn = dsCltSqlExecSome(caSql);
	            
//	            char *pUsername = NULL; //其它类似
//				 		  char *pPassword = NULL;
//				 		  char *pGroupname = NULL;
//				 		  char *pDispname = NULL;
//				 		  char *pSsid = NULL;
//				      int  iStatus = 0;
//				      unsigned long lMaxMac = 0;
//				      char caMaxmac[11] = "";
//		      	  strcpy(caMaxmac,utComGetVar_sd(psShmHead,"NcmMaxUMac","1"));
//		      		lMaxMac = atol(caMaxmac);	
//				      pUsername = strdup(caUsername);
//				      pPassword = strdup(caPassword1);
//				      if(strlen(caOpt)==0){
//				      	  char optd[32] = "";
//				      		sprintf(optd,"%s%s",ncmWebAutoDefGet16(psShmHead, iFd,psMsgHead,caStype),"");
//				      		printf("optd = %s\n", optd);	
//				      		pGroupname = strdup(optd);
//				      }else{
//				      		pGroupname = strdup(caOpt);	
//				      }
//				      pDispname = strdup(caDispname);
//				      pSsid = strdup(utComGetVar_sd(psShmHead,"NcmMySSID",  "ICPIC"));
//				      ncSrvSynUserInfo(psShmHead,pGroupname,pSsid,pUsername,pPassword,NULL,pDispname,0,lMaxMac,0,&iStatus);
//				      if(pSsid){
//					         free(pSsid);
//					         pSsid = NULL;
//				     }   
//				      if(pUsername){
//					         free(pUsername);
//					         pUsername = NULL;
//				      }
//				      if(pPassword){
//					         free(pPassword);
//					         pPassword = NULL;
//				      }
//				      if(pGroupname){
//					         free(pGroupname);
//					         pGroupname = NULL;
//				      }
//				      if(pDispname){
//					         free(pDispname);
//					         pDispname = NULL;
//				      }
				     
	            if(iReturn == 0) {
	            		utPltPutVar(psDbHead,"cflag","1");
	        				utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncadminuser.htm");
	//              ncWebAdminUserList(psShmHead, iFd,psMsgHead);
	                //ncAdmWriteLog(caMyname,caIp,ncLang("0689新增一个用户 %s"),caUsername);
					ncmWriteSysLog(psMsgHead,caUsername,"02","",ncLang("0689新增一个用户 %s"));
	                if(strlen(caOpt)>0){
	                	 printf("update dsuser set opt='%s' where id=%d \n",caOpt,lUserid);
	                	 pasDbExecSqlF("update dsuser set opt='%s' where id=%d ",caOpt,lUserid);
	                }
	            }else{
	                utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncLang("0839增加用户"),"用户名不能重复");
	            } 
	        } else {
	           utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncLang("0839增加用户"),"用户名不能重复");
	           return 0;
	        }
	     } else { /* 修改记录  */
	        sprintf(caSql,"<DSSQL>update dsuser set name='%s',dispname='%s',groupid=%lu,\
	                            groupname='%s'",
	                            caUsername,caDispname,lGroupid,caGroupname);
	        if(!utStrIsSpaces(caPassword1)) {
	            sprintf(caKey0,"%s%s",caUsername,caPassword1);
	            utMd5Ascii22(caKey0,strlen(caKey0),NULL,caKey);  /* 口令加密  */
	            utStrAddF(caSql,",lkey='%s'",caKey);
	        }
	        
	        if(!utStrIsSpaces(caKey2)) {
	            utStrAddF(caSql,",key2='%s'",caKey2);
	        }
	        
	        if(lCid==0 && strlen(caOptd)==0 && strlen(caStyped)==0){
	        		utStrAddF(caSql,",moditime=%lu,lasttime=%lu,usrlevel=%lu,email='%s',company='%s',cid=%lu,stype='%s' where id=%lu </DSSQL>\n",
	                     lTime,lTime,lUsrlevel,caEmail,fac_desc,lcid,caStype,lUserid);	
	        }else{ // 判断是否是二级分权
	        	  utStrAddF(caSql,",moditime=%lu,lasttime=%lu,usrlevel=%lu,email='%s',company='%s',cid=%lu,stype='%s' where id=%lu </DSSQL>\n",
	                     lTime,lTime,lUsrlevel,caEmail,fac,lCid,caStyped,lUserid);
	        }
	        
	        if(lIdd != lUserid){  
			        utStrAddF(caSql,"<DSSQL>delete from dsuserrole where id = %lu </DSSQL>\n",lUserid);
			        utStrAddF(caSql,"<DSSQL>insert into dsuserrole(id,roleid,addtime,addby) values \
			                          (%lu,%lu,%lu,'%s')</DSSQL>\n",
			                       lUserid,atol(caChecked),lTime,caMyname);
	        }
	                
	        iReturn = dsCltSqlExecSome(caSql);
	        
	        if(iReturn == 0) {
	        		utPltPutVar(psDbHead,"cflag","1");
	        		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncadminuser.htm");
	            //ncAdmWriteLog(caMyname,caIp,ncLang("0706修改用户[%s]的信息"),caUsername);
				char msg[32];
				sprintf(msg,ncLang("0706修改用户[%s]的信息"),caUsername);
				printf("msg = %s\n",msg);
				ncmWriteSysLog(psMsgHead,caMyname,"02","",msg);
	            if(strlen(caOpt)>0){
	            	  pasDbExecSqlF("update dsuser set opt='%s' where id=%d ",caOpt,lUserid);
	            		printf("update dsuser set opt='%s' where id=%d \n",caOpt,lUserid);
	            }
	        }
	        else {
	           utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncLang("0839增加用户"),ncLang("0777用户修改错误%d"),iReturn);
	        }            
	     }
    }
    else{ 
	  	 printf("*******************结束1************************\n");
	     if(utStrIsSpaces(caUserid) || lUserid == 0) { /* 新增记录  */
	        lUserid = dsCltGetSeque();
	        if(lUserid > 0) {
	            if(utStrIsSpaces(caPassword1)) {
	               utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0839增加用户"),ncLang("0349口令不能为空"));
	               return 0;
	            }
	           
	            sprintf(caKey0,"%s%s",caUsername,caPassword1);
	            utMd5Ascii22(caKey0,strlen(caKey0),NULL,caKey);  /* 口令加密  */
	            if(lCid==0 && strlen(caOptd)==0 && strlen(caStyped)==0){
	            		sprintf(caSql,"<DSSQL>insert into dsuser (id,name,dispname,groupid,groupname,addtime,\
	                              moditime,lkey,lasttime,logcount,useflags,status,usrlevel,email,company,cid,stype) values \
	                           (%lu,'%s','%s',%lu,'%s',%lu,%lu,'%s',%lu,0,0,0,%d,'%s','%s',%d,'%s')</DSSQL>\n",
	                           lUserid,caUsername,caDispname,lGroupid,caGroupname,lTime,lTime,
	                           caKey,lTime,lUsrlevel,caEmail,fac,lcid,caStype);	
	            }else{ // 判断是否是二级分权
	            		sprintf(caSql,"<DSSQL>insert into dsuser (id,name,dispname,groupid,groupname,addtime,\
	                              moditime,lkey,lasttime,logcount,useflags,status,usrlevel,email,company,cid,stype) values \
	                           (%lu,'%s','%s',%lu,'%s',%lu,%lu,'%s',%lu,0,0,0,%d,'%s','%s',%d,'%s')</DSSQL>\n",
	                           lUserid,caUsername,caDispname,lGroupid,caGroupname,lTime,lTime,
	                           caKey,lTime,lUsrlevel,caEmail,fac,lCid,caStyped);
	            }
	            printf("*******************结束2************************\n");
	            utStrAddF(caSql,"<DSSQL>insert into dsuserrole(id,roleid,addtime,addby) values \
	                              (%lu,%lu,%lu,'%s')</DSSQL>\n",
	                           lUserid,atol(caChecked),lTime,caMyname);
	            iReturn = dsCltSqlExecSome(caSql);
	            if(iReturn == 0) {
	            		utPltPutVar(psDbHead,"cflag","1");
		        			utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncadminuser.htm");
									//ncAdmWriteLog(caMyname,caIp,ncLang("0689新增一个用户 %s"),caUsername);
									char msg[32];
									sprintf(msg,ncLang("0689新增一个用户 %s"),caUsername);
									printf("msg = %s\n",msg);
									ncmWriteSysLog(psMsgHead,caMyname,"02","",msg);
	                if(strlen(caOpt)>0){
		                	printf("update dsuser set opt='%s' where id=%d \n",caOpt,lUserid);
		                	pasDbExecSqlF("update dsuser set opt='%s' where id=%d ",caOpt,lUserid);
	                }
	            } else {
	                utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncLang("0839增加用户"),"用户名不能重复");
	            }  
	        } else {
	            utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncLang("0839增加用户"),"用户名不能重复");
	            return 0;
	        }
	     } else { /* 修改记录  */
	        sprintf(caSql,"<DSSQL>update dsuser set name='%s',dispname='%s',groupid=%lu,\
	                            groupname='%s'",
	                            caUsername,caDispname,lGroupid,caGroupname);
	        if(!utStrIsSpaces(caPassword1)) {
	            sprintf(caKey0,"%s%s",caUsername,caPassword1);
	            utMd5Ascii22(caKey0,strlen(caKey0),NULL,caKey);  /* 口令加密  */
	            utStrAddF(caSql,",lkey='%s'",caKey);
	        }
	        if(lCid==0 && strlen(caOptd)==0 && strlen(caStyped)==0){
	        		utStrAddF(caSql,",moditime=%lu,lasttime=%lu,usrlevel=%lu,email='%s',company='%s',cid=%lu,stype='%s' where id=%lu </DSSQL>\n",
	                     lTime,lTime,lUsrlevel,caEmail,fac,lcid,caStype,lUserid);	
	        }else{ // 判断是否是二级分权
	        		utStrAddF(caSql,",moditime=%lu,lasttime=%lu,usrlevel=%lu,email='%s',company='%s',cid=%lu,stype='%s' where id=%lu </DSSQL>\n",
	                     lTime,lTime,lUsrlevel,caEmail,fac,lCid,caStyped,lUserid);	
	        }
	        
	        if(lIdd != lUserid){
	        	  printf("111111111111111111\n");
			        utStrAddF(caSql,"<DSSQL>delete from dsuserrole where id = %lu </DSSQL>\n",lUserid);
			        utStrAddF(caSql,"<DSSQL>insert into dsuserrole(id,roleid,addtime,addby) values \
			                          (%lu,%lu,%lu,'%s')</DSSQL>\n",
			                       lUserid,atol(caChecked),lTime,caMyname);
	        }       
	        iReturn = dsCltSqlExecSome(caSql);
	        if(iReturn == 0) { 
		        	utPltPutVar(psDbHead,"cflag","1");
		        	utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncadminuser.htm");
		          if(strlen(caOpt)>0){
		            	printf("update dsuser set opt='%s' where id=%d \n",caOpt,lUserid);
		            	pasDbExecSqlF("update dsuser set opt='%s' where id=%d ",caOpt,lUserid);
		          }
	        }
	        else {
	            utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncLang("0839增加用户"),ncLang("0777用户修改错误%d"),iReturn);
	        }            
	     }
    }
    return 0;
}

//用户访问日志
int ncmWebAdmQyeSysLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
 	  utMsgPrintMsg(psMsgHead);
 	  
 	  pasDbCursor *psCur;
 	  utPltDbHead *psDbHead = utPltInitDb();
 	  
    char caTemp[1024];
    char caTotPg[16],caCurPg[16],caCx[12],caDel[12],caDelall[12];
    int  i,iReturn,iNum;
    char caAction[6];
    char caResult[16];
    char caSdate[20],caEdate[20];
    char caDesc_temp[300];
    char groupcode[32];
    
    char caWhere[256],caIp[16],caDesc[256],caName[36],caSear[64];
    char caPage[16],caLimit[16], start[16] = "";
    long lCurPg,lRowNum,lStartRec,lTotPg,lCount;
    unsigned long lSid;
    
    // 得到账号ID
    unsigned long lCid = 0;
	  dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
	  char opt[32]="";
    dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(opt) - 1,opt);
		printf("opt = %s\n",opt);
    
   	iReturn = utMsgGetSomeNVar(psMsgHead,14,"cx",         UT_TYPE_STRING, 11, caCx,
												   	                "del",        UT_TYPE_STRING, 11, caDel,
												   	                "delall",     UT_TYPE_STRING, 11, caDelall,
												   	                "sear",       UT_TYPE_STRING, 63, caSear,
																						"CurPg",      UT_TYPE_STRING, 12, caCurPg,
																						"TotPg",      UT_TYPE_STRING, 12, caTotPg,
																						"page",       UT_TYPE_STRING, 10, caPage,
												   	                "limit",      UT_TYPE_STRING, 10, caLimit,
												   	                "start",      UT_TYPE_STRING,sizeof(start)-1,start,
																						"where",      UT_TYPE_STRING, 255,caWhere,
																						"sdate",      UT_TYPE_STRING, 19, caSdate,
																						"edate",      UT_TYPE_STRING, 19, caEdate,
																						"groupcode",  UT_TYPE_STRING, sizeof(groupcode)-1,groupcode,
																						"action",     UT_TYPE_STRING, 5,  caAction);
	  
	  if(strlen(caSdate)>0)
	  sprintf(caSdate, "%s 00:00:00", caSdate); 
	  if(strlen(caEdate)>0)
	  sprintf(caEdate, "%s 23:59:59", caEdate); 
	  
		pasCvtGBK(2,caSear,caTemp,32);
		strcpy(caSear,caTemp);
		if(strlen(caPage)>0){
	    	strcpy(caCurPg,caPage);
    }
    lCurPg = atol(caCurPg);
    if(lCurPg == 0 || utMsgGetVar_s(psMsgHead,"search")) {
        lCurPg = 1;
    }
    lCurPg = atol(caCurPg);
    if(strlen(caLimit)>0)
   		lRowNum=atol(caLimit);
    else
	    lRowNum = 32;
    lStartRec = (lCurPg - 1) * lRowNum;
    if(lStartRec <= 0) lStartRec=0;
    /* 删除  */
    if(!utStrIsSpaces(caDel)) { /* 删除选择记录  */
        char caVar[12];
        char *p;
        for(i=0;i<lRowNum;i++) {
            sprintf(caVar,"ch%d",i+1);
            p = utMsgGetVar_s(psMsgHead,caVar);
            if(p) {
                 iReturn = pasDbExecSqlF("delete from ncadminlog where sid = %s",p);
            }
        }
        pasDbCommit(NULL);
    }
    
    if(!utStrIsSpaces(caCx)) {
        if(!utStrIsSpaces(caSear)) {
            char caCond[72];
            utStrDelSpaces(caSear);
            sprintf(caCond,"'%c%s%c'",'%',caSear,'%');
            if(IsInActionName(caSear)==0){
            		sprintf(caWhere,"where 1=1 and action='%s' ",ncsGetCodeByActionName(caSear));
            }
            else{
	            	sprintf(caWhere,"where 1=1 and (sdate like %s or name like %s or ipaddr like %s or descr like %s)",
	            	caCond,caCond,caCond,caCond);
            }
        }
        else {
            strcpy(caWhere,"\0");
        }
    }
		if(strlen(caWhere)>0){
				if(strlen(caSdate)>0){
						sprintf(caWhere+strlen(caWhere)," and sdate>='%s' ",caSdate);
				}
				if(strlen(caEdate)>0){
						sprintf(caWhere+strlen(caWhere)," and sdate<='%s' ",caEdate);
				}
				if(strlen(caAction)>0){
						sprintf(caWhere+strlen(caWhere)," and action='%s' ",caAction);
				}
				if(lCid>0){
						sprintf(caWhere+strlen(caWhere)," and cid=%lu ",lCid);
				}
		}
		else{
			  strcpy(caWhere," where 1=1 ");
				if(strlen(caSdate)>0){
						sprintf(caWhere+strlen(caWhere)," and sdate>='%s' ",caSdate);
				}
				if(strlen(caEdate)>0){
						sprintf(caWhere+strlen(caWhere)," and sdate<='%s' ",caEdate);
				}
				if(strlen(caAction)>0){
						sprintf(caWhere+strlen(caWhere)," and action='%s' ",caAction);
				}
				if(lCid>0){
						sprintf(caWhere+strlen(caWhere)," and cid=%lu ",lCid);
				}
		}

    // 删除
    if(!utStrIsSpaces(caDelall)) { /* 删除全部  */
        iReturn = pasDbExecSqlF("delete from ncadminlog  %s",caWhere);
        pasDbCommit(NULL);
    }
    
    sprintf(caTemp,"select count(*) from ncadminlog  %s",caWhere);
		if(strlen(opt) > 0)//编码
		{
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp)," and groupcode like '%s%c' ",opt,'%');
		}
		if(strlen(groupcode) > 0)//编码
		{
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp)," and groupcode like '%%%s%c' ",groupcode,'%');
		}
	  printf("caTemp==%s\n",caTemp);
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
    if(iReturn != 0) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncLang("0053IP查询"),ncLang("0551数据库出错%s"),pasDbErrorMsg(NULL));
        return 0;
    }
    lTotPg = (lCount -1 ) / lRowNum + 1;

	  sprintf(caTemp,"select  sid,sdate,name,ipaddr,descr,action,result from ncadminlog  %s",caWhere);
		if(strlen(opt) > 0)//编码
		{
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp)," and groupcode like '%s%c' ",opt,'%');
		}
		if(strlen(groupcode) > 0)//编码
		{
			snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp)," and groupcode like '%%%s%c' ",groupcode,'%');
		}
		snprintf(caTemp+strlen(caTemp), sizeof(caTemp)-strlen(caTemp),"  order by sdate desc limit %lu,%lu ",lStartRec,lRowNum);
	  printf("caTemp==[%s]\n",caTemp);
		psCur = pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncLang("0746用户查询"),ncLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }

    utPltPutVar(psDbHead,"sdate",caSdate);
    utPltPutVar(psDbHead,"edate",caEdate);
    utPltPutVar(psDbHead,"action",caAction);
    lSid=0;
    memset(caSdate,0,sizeof(caSdate));
    memset(caName,0,sizeof(caName));
    memset(caIp,0,sizeof(caIp));
    memset(caDesc,0,sizeof(caDesc));
    memset(caAction,0,sizeof(caAction));
    memset(caResult,0,sizeof(caResult));
    iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lSid,
                                   UT_TYPE_STRING,19,caSdate,
                                   UT_TYPE_STRING,35, caName,
                                   UT_TYPE_STRING,15,caIp,
                                   UT_TYPE_STRING,255,caDesc,
                                   UT_TYPE_STRING,4,caAction,
                                   UT_TYPE_STRING,10,caResult);
    iNum = 0;                                   
    while((iReturn == 0 ||iReturn==1405)&& iNum < lRowNum) {
        if(iNum>0){
	        	utPltPutLoopVar(psDbHead,"sysvl_dh",iNum+1,",");
	      }
        iNum++;
        utPltPutLoopVarF(psDbHead,"sysvl_num", iNum,"%lu",iNum);
        utPltPutLoopVarF(psDbHead,"sysvl_sid", iNum,"%lu",lSid);
        utPltPutLoopVar(psDbHead,"sysvl_sdate", iNum,caSdate);
        utPltSetCvtHtml(1);
        utPltPutLoopVar(psDbHead,"sysvl_name",  iNum,caName);
        utPltSetCvtHtml(0);
        utPltPutLoopVar(psDbHead,"sysvl_ipaddr", iNum,caIp);
        memset(caDesc_temp,0,sizeof(caDesc_temp));
        if(strlen(caDesc)>0){
        	 ncStrFixedCol(caDesc,caDesc_temp,50,254);
        }
        utPltPutLoopVar(psDbHead,"sysvl_desc", iNum,caDesc_temp);
        utPltPutLoopVar(psDbHead,"sysvl_action",iNum,ncsGetActionNameByCode(caAction));
        utPltPutLoopVar(psDbHead,"sysvl_result",iNum,caResult);
        
		    lSid=0;
		    memset(caSdate,0,sizeof(caSdate));
		    memset(caName,0,sizeof(caName));
		    memset(caIp,0,sizeof(caIp));
		    memset(caDesc,0,sizeof(caDesc));
		    memset(caAction,0,sizeof(caAction));
		    memset(caResult,0,sizeof(caResult));
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lSid,
		                                   UT_TYPE_STRING,19,caSdate,
		                                   UT_TYPE_STRING,35, caName,
		                                   UT_TYPE_STRING,15,caIp,
		                                   UT_TYPE_STRING,255,caDesc,
		                                   UT_TYPE_STRING,4,caAction,
		                                   UT_TYPE_STRING,10,caResult);
    }
    utPltPutVarF(psDbHead,"TotRec", "%lu", lCount);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
    if(!utStrIsSpaces(caWhere)) {
        utPltPutVar(psDbHead,"where", caWhere);
    }        
    if(!utStrIsSpaces(caSear)) {
        utPltPutVar(psDbHead,"sear", caSear);
    }
    pasDbCloseCursor(psCur);
    i = 0;
    while(iNum < lRowNum + 1) {
        i++;
        iNum++;
        utPltPutLoopVarF(psDbHead,"snum", i,"%d",iNum);
    }
	  printf("ncmWebAdmQyeSysLog end \n");
	  
    //  utPltShowDb(psDbHead); 
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncm_sys_uservisitlog.html");
    return 0;
}

//显示全部组 主要用户组合框选择
int ncmWebGroupComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
   	char caGroupname[32],caGroupid[16];
   	
   	char caName[32];
   	char caWhere[256];
   	char caSql[256];
   	char caCurPg[16],caTotRec[16];
   	char caPid[16];
   	unsigned long lLevel,lPid;
   	int  iNum;
  	char caFilter[256];
  	char caNo[16];
   	long lGroupid,lGroupid0,lCurPg,lTotRec,lRowNum,lStartRec;
    utPltDbHead *psDbHead;
   	int iReturn,i;
   	char caVar[256],caFname[64],caBfname[64];
   	char caQuery[256],caTemp[1024];
   	char caValue[256];
   	char caVar1[32];
   	char caOpt[256];
   	long lId;
   	char *p,*pVar;
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
    
    iReturn = utMsgGetSomeNVar(psMsgHead,1,"query", UT_TYPE_STRING, 127, caQuery);
//				printf("caQuery=%s\n",pasCvtGBK(2,caQuery,caTemp,255));
    iReturn = dsCltGetSessionValue(1,"groupid",UT_TYPE_LONG,sizeof(long),&lGroupid0);
    strcpy(caWhere,"\0");
 
     memset(caOpt,0,sizeof(caOpt)); 
    iReturn = dsCltGetMyInfo(1,"Userid",&lId);
    if(iReturn==0){
 	    pasDbOneRecord("select opt from dsuser where id=:id",1,"id",UT_TYPE_LONG,lId,UT_TYPE_STRING,255,caOpt);
    } 

    lRowNum = utComGetVar_ld(psShmHead,"GroupTreeRow",300);
    lStartRec = (lCurPg - 1) * lRowNum;  
    
    if(!strstr(caOpt,"all")&&(strlen(caOpt)>0)){
     	printf("select groupname,groupid,pid from ncgroup where groupid in (%s) or pid in (%s) or groupid in (select groupid from ncgroup where pid in(%s)) order by pid limit %lu,%lu\n",caOpt,caOpt,caOpt,0,lRowNum);
     	
	    psCur = pasDbOpenSqlF("select groupname,groupid,pid from ncgroup where groupid in (%s) or pid in (%s) or groupid in (select pid from ncgroup where groupid in(%s)) order by pid limit %lu,%lu",caOpt,caOpt,caOpt,0,lRowNum);
    }
    else{
   		printf("select groupname,groupid,pid from ncgroup where 1=1  order by pid limit %lu,%lu\n",0,lRowNum);
  	  psCur = pasDbOpenSqlF("select groupname,groupid,pid from ncgroup where 1=1  order by pid limit %lu,%lu",0,lRowNum);
    }
    if(psCur == NULL) {
    	  
        return 0;
    }
    psDbHead = utPltInitDb();
    memset(caGroupname,0,sizeof(caGroupname));
    lGroupid=0;
    lPid=0;
    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caGroupname,
                                   UT_TYPE_LONG,4,&lGroupid,
                                   UT_TYPE_LONG,4,&lPid);
    if(!utStrIsSpaces(caPlate)) utPltPutVar(psDbHead,"plate",caPlate);
    if(!utStrIsSpaces(caFname)) utPltPutVar(psDbHead,"fname",caFname);
    iNum = 0;
    while(iReturn == 0) {
    		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
        iNum++;
        utPltPutLoopVar(psDbHead,"groupname", iNum,caGroupname);
        utPltPutLoopVarF(psDbHead,"groupid",   iNum,"%ld",lGroupid);
		    memset(caGroupname,0,sizeof(caGroupname));
		    lGroupid=0;
		    lLevel=0;
		    lPid=0;
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caGroupname,
		                                   UT_TYPE_LONG,4,&lGroupid,
		                                   UT_TYPE_LONG,4,&lPid);
    }
    pasDbCloseCursor(psCur);   
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncm_corpclass_list.htm");
      
    return 0;
}

//权限配置文件
int ncmSysUserRoleConfig(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  utMsgPrintMsg(psMsgHead);
    pasDbCursor *psCur = NULL;
   	char caTemp[1024]="";
   	char caTemp2[32]="";
   	int  iNum=0;
    char caMfun[2048]="";
    char caSfun[8192] = "";
    //long lLevel;
    utPltDbHead *psDbHead = utPltInitDb();
   	int iReturn=0;
   	int i=0;
   	int iret=0;   
    unsigned long lCount=0;
    char caMainmenu[1024]="";
    char caSubmenu[2048]="";
    char caSy[64]="";
    char *q = NULL;
    
    unsigned long lId=0;
    dsCltGetSessionValue(1,"id",UT_TYPE_LONG,sizeof(long),&lId);
    printf("lId=%d \n",lId);
    unsigned long lCid = 0;
    char caStyped[32] = "";
    char caOptd[32] = "";
	  dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
	  dsCltGetSessionValue(1, "opt", UT_TYPE_STRING,sizeof(caOptd)-1, caOptd);
	  dsCltGetSessionValue(1, "stype", UT_TYPE_STRING,sizeof(caStyped)-1, caStyped);
	  printf("lCid = %d\n", lCid);
	  printf("caOptd = %s\n", caOptd);
	  printf("caStyped = %s\n", caStyped);
    //sprintf(caTemp,"select usrlevel from dsuser where id=%d ",lId);
    //pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lLevel);
    
    struct dscltdata{
    	  unsigned long id;
    		char mainmenu[32];
    		char submenu[2048];	
    };
    struct dscltdata *dsdata;
    
    strcpy(caSy,"在线信息");
 strcpy(caOptd,"111");   
    if(lCid==0 && strlen(caOptd)==0 && strlen(caStyped)==0){
    		sprintf(caTemp, "select count(*) from ncminitmenu where 1=1");
		    pasDbOneRecord(caTemp,0,UT_TYPE_ULONG,sizeof(long),&lCount);
		    dsdata = (struct dscltdata *)malloc(sizeof(struct dscltdata)*lCount + 1);
		    for(i=0; i<lCount; i++){
		    			dsdata[i].id=0;
		    			strcpy(dsdata[i].mainmenu, "");
		    			strcpy(dsdata[i].submenu, "");
		    }
		    
		    memset(caTemp, 0, sizeof(caTemp));
		    sprintf(caTemp, "select id, mainmenu from ncminitmenu where 1=1");
		    psCur = pasDbOpenSql(caTemp, 0);
			  if(psCur==NULL){
			 		 return 0;	
			  }
		    
		    iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lId,
																	  UT_TYPE_STRING, sizeof(caMainmenu)-1,  caMainmenu);
		    iNum=0;
		    while(0 == iret || 1405 == iret){
		    			dsdata[iNum].id = lId;
		    			strcpy(dsdata[iNum].mainmenu, caMainmenu);
		    			//printf("dsdata[%d].id = %d\n", iNum,dsdata[iNum].id);
		    			iNum++;
		    			lId=0;
		    			memset(caMainmenu, 0, sizeof(caMainmenu));
		    			iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lId,
																	        UT_TYPE_STRING, sizeof(caMainmenu)-1,  caMainmenu);
		    }
		    pasDbCloseCursor(psCur);
		    
		    memset(caTemp, 0, sizeof(caTemp));
		    for(i=0; i<lCount; i++){
		    		sprintf(caTemp, "select submenu from ncminitsubmenu where pid=%lu", dsdata[i].id);
		    		psCur = pasDbOpenSql(caTemp, 0);
					  if(psCur==NULL){
					 		 return 0;	
					  }
					  
					  iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(caSubmenu)-1,  caSubmenu);
				    iNum=0;
				    while(0 == iret || 1405 == iret){
				    	    if(iNum==0){
				    					strcpy(dsdata[i].submenu, dsdata[i].mainmenu);
				    					sprintf(dsdata[i].submenu+strlen(dsdata[i].submenu),",%s",caSubmenu);
				    		  }else{
				    		  		sprintf(dsdata[i].submenu+strlen(dsdata[i].submenu),",%s",caSubmenu);	
				    		  }
				    			//printf("dsdata[%d].submenu = %s\n",i,dsdata[i].submenu);
				    			iNum++;
				    			memset(caSubmenu, 0, sizeof(caSubmenu));
				    			iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(caSubmenu)-1,  caSubmenu);
				    }
				    pasDbCloseCursor(psCur);
		    }
		    
		    for(i=0; i<lCount; i++){
		    		if(i==0){
		    				strcpy(caMfun,dsdata[i].mainmenu);
				        strcpy(caSfun,dsdata[i].submenu);
		    		}else{
		    				sprintf(caMfun+strlen(caMfun),",%s",dsdata[i].mainmenu);
				        sprintf(caSfun+strlen(caSfun),",%s",dsdata[i].submenu);	
		    		}	
		    }
    }else{
		    sprintf(caTemp,"select lright,submenu from dsuser a,dsuserrole b,dsroleattr c where a.id=b.id and b.roleid=c.roleid and a.id=%d group by lright",lId);  
		    printf("caTemp=%s\n",caTemp);
		    psCur = pasDbOpenSqlF(caTemp);
				if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","部门查询","数据库出错");
		        return 0;
				}
				   
			  memset(caMainmenu,0,sizeof(caMainmenu));
			  memset(caSubmenu,0,sizeof(caSubmenu));
				iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(caMainmenu)-1,caMainmenu,
				                               UT_TYPE_STRING,sizeof(caSubmenu)-1, caSubmenu);
				iNum=0;	 
				while(iReturn == 0||iReturn==1405) {		        
				     if(iNum==0){
				         		strcpy(caMfun,caMainmenu);
				         		strcpy(caSfun,caSubmenu);
				     }else{
				         	  sprintf(caMfun+strlen(caMfun),",%s",caMainmenu);
				         	  sprintf(caSfun+strlen(caSfun),",%s",caSubmenu);
				     }
				     iNum++;
				     
				     memset(caMainmenu,0,sizeof(caMainmenu));
			       memset(caSubmenu,0,sizeof(caSubmenu));
						 iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(caMainmenu)-1,caMainmenu,
				                                    UT_TYPE_STRING,sizeof(caSubmenu)-1, caSubmenu);
				}
				pasDbCloseCursor(psCur);
	  }
    
    utPltPutVar(psDbHead,"sy",caSy);
    utPltPutVar(psDbHead,"fun",caMfun);	
    utPltPutVar(psDbHead,"sfun",caSfun);
    printf("caSy=%s \n",caSy);
    printf("caMfun=%s \n",caMfun);
    printf("caSfun=%s \n",caSfun);
//  utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncm_right_config.htm");
    return 0;
}

//首页菜单配置文件
int ncmFirstMenuConfig(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
   	char caTemp[1024];
   	int  iNum;
    char caFun[2048];
    long lLevel;
    utPltDbHead *psDbHead;
   	int iReturn,i;   
    long lCount;
    char caName[64],caSy[64],uname[32]="",fi_mname[32]="",fi_mflag[20]="",urlnum[128]="",tname[32]="",tid[32]="";
    long lId=0,lSid,mid=0,mflag=0,systime=0,bflag=0,tNum=0,oNum=0,j=0,maxoid=0,findflg=0;
    struct menu_s{
    	int id;
    	char mname[32];
    	char tname[32];
    	char tid[32];
    	char urlnum[128];
    	int mflag;
    };
    struct menu_s menu_struc[12];
    struct menu_s menu_ops[12];
    psDbHead = utPltInitDb();
    iReturn = utMsgGetSomeNVar(psMsgHead,2,"mname",    UT_TYPE_STRING, 32, fi_mname,
																					 "fi_mflag",    UT_TYPE_STRING, 20, fi_mflag);
    dsCltGetSessionValue(1,"id",UT_TYPE_LONG,sizeof(long),&lId);
    printf("****************************lId=%d \n",lId);
    utMsgPrintMsg(psMsgHead);
    if(strlen(fi_mname)>0){
			 pasCvtGBK(2,fi_mname,caTemp,64);
			 strcpy(fi_mname,caTemp);
		}
    lId=100;
    sprintf(caTemp,"select name from dsuser where id=%d ",lId);
    printf("***********1*****************caTemp=%s \n",caTemp);
    pasDbOneRecord(caTemp,0,UT_TYPE_STRING,32,uname);
//    strcpy(caSy,ncsGetFirstNameById(lLevel));
    strcpy(caSy,"首页");
    printf("***************2*************uname=%s  fi_mname=%s\n",uname,fi_mname);
    if(strlen(fi_mflag)==0) strcpy(fi_mflag,"0");
    if(strlen(uname)>0&&strlen(fi_mname)>0){
	    sprintf(caTemp,"select count(*) from ncwlanopra where uname='%s' and mname='%s' ",uname,fi_mname);
	    pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	    
	    if(strlen(fi_mflag)==0) strcpy(fi_mflag,"0");
	    if(lCount>0){
	    	sprintf(caTemp,"update ncwlanopra set mflag=%s,systime=%lu where uname='%s' and mname='%s' ",
								fi_mflag,time(0),uname,fi_mname);
//								printf("***************update*************caTemp=%s \n",caTemp);
								iReturn = pasDbExecSqlF(caTemp);
	    }else{
	    	sprintf(caTemp,"select max(id)+1 from ncwlanopra ");
	 		  pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&maxoid); 
	 		  if(maxoid==0) maxoid=1;
	    	sprintf(caTemp,"insert into ncwlanopra(id,uname,mname,mflag,systime) values(%d,'%s','%s',%s,%d) ",
								maxoid,uname,fi_mname,fi_mflag,time(0));
//								printf("***************insert*************caTemp=%s \n",caTemp);
								iReturn = pasDbExecSqlF(caTemp);
	    }
  	}

	  sprintf(caTemp,"select id,mname,mflag,tname,tid,urlnum from ncwlanmenu   ");  
//	    sprintf(caTemp,"select a.id,a.mname,a.mflag,b.systime,a.tname,a.tid,a.urlnum,b.mflag from ncwlanmenu a left join ncwlanopra b on(a.mname=b.mname)   ");  
    printf("***********3*****************caTemp=%s \n",caTemp);
    psCur = pasDbOpenSqlF(caTemp);
		if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","菜单查询","数据库出错");
		//    	  free(pHash);
		        return 0;
		} 
		mid=0;
	  memset(fi_mname,0,sizeof(fi_mname));
	  mflag=0;
	  memset(tname,0,sizeof(tname));
	  memset(tid,0,sizeof(tid));
	  memset(urlnum,0,sizeof(urlnum));
	  bflag=0;
		iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&mid,
 																	 UT_TYPE_STRING,31,caName,
 																	 UT_TYPE_LONG,4,&mflag,
 																	 UT_TYPE_STRING,32,tname,
 																	 UT_TYPE_STRING,32,tid,
 																	 UT_TYPE_STRING,128,urlnum);
		iNum=0;	 
		while(iReturn == 0||iReturn==1405) {	
				 strcpy(menu_struc[iNum].mname,caName);	
				 strcpy(menu_struc[iNum].tname,tname);	
				 strcpy(menu_struc[iNum].tid,tid);	
				 strcpy(menu_struc[iNum].urlnum,urlnum);	
				 menu_struc[iNum].mflag=mflag;
		     iNum++;
		     
		     mid=0;
				 memset(fi_mname,0,sizeof(fi_mname));
				 mflag=0;
				 memset(tname,0,sizeof(tname));
			   memset(tid,0,sizeof(tid));
			   memset(urlnum,0,sizeof(urlnum));
			   bflag=0;
				 iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&mid,
 																	 			UT_TYPE_STRING,31,caName,
 																	 			UT_TYPE_LONG,4,&mflag,
		 																	  UT_TYPE_STRING,32,tname,
		 																	  UT_TYPE_STRING,32,tid,
		 																	  UT_TYPE_STRING,128,urlnum);
		}
		pasDbCloseCursor(psCur);
		
		if(strlen(uname)>0){
	    sprintf(caTemp,"select id,mname,mflag from ncwlanopra where uname='%s'  ",uname);  
//      printf("***********3*****************caTemp=%s \n",caTemp);
	    psCur = pasDbOpenSqlF(caTemp);
			if(psCur == NULL) {
			    	  utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","操作菜单查询","数据库出错");
			        return 0;
			} 
			mid=0;
		  memset(fi_mname,0,sizeof(fi_mname));
		  mflag=0;
			iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&mid,
	 																	 UT_TYPE_STRING,31,caName,
	 																	 UT_TYPE_LONG,4,&mflag);
			oNum=0;	 
			while(iReturn == 0||iReturn==1405) {	
					 strcpy(menu_ops[oNum].mname,caName);	
					 menu_ops[oNum].mflag=mflag;
			     oNum++;
			     mid=0;
					 memset(fi_mname,0,sizeof(fi_mname));
					 mflag=0;
					 iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&mid,
	 																	 			UT_TYPE_STRING,31,caName,
	 																	 			UT_TYPE_LONG,4,&mflag);
			}
			pasDbCloseCursor(psCur);
	}
		
		tNum=0;
		bflag=0;
		for(i=0;i<iNum;i++){
				 tNum++;
				 utPltPutLoopVar(psDbHead,"me_df",tNum,",");
		     utPltPutLoopVar(psDbHead,"me_mname",tNum,menu_struc[i].mname);
		     utPltPutLoopVarF(psDbHead,"me_num",tNum,"%d",mid);
		     utPltPutLoopVar(psDbHead,"me_tname",tNum,menu_struc[i].tname);
		     utPltPutLoopVar(psDbHead,"me_mid",tNum,menu_struc[i].tid);
		     utPltPutLoopVar(psDbHead,"me_urlnum",tNum,menu_struc[i].urlnum);
		     utPltPutLoopVarF(psDbHead,"m_num",tNum,"%d",tNum);
		     //二分查找操作表 找到flag按操作表的，未找到按默认的。
		     for(j=0;j<oNum;j++){
		     	if(strcmp(menu_struc[i].mname,menu_ops[j].mname)==0){
		     		bflag=menu_ops[j].mflag;	printf("mname=%s  opmname=%s\n",menu_struc[i].mname,menu_ops[j].mname);
		     		findflg=1;
		     	}
		     }
				 if(findflg>0)
		     	utPltPutLoopVarF(psDbHead,"me_flag",tNum,"%d",bflag);
		     else
		     	utPltPutLoopVarF(psDbHead,"me_flag",tNum,"%d",menu_struc[i].mflag);
		     bflag=0;
		     findflg=0;
		}
		utPltPutVarF(psDbHead,"me_mCou","%d",iNum+1);
//    utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/index_wire_1_right2.html");
    return 0;
}

int ncmjump_gid(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
   utPltDbHead *psDbHead;
 	 char imp_file[129]="";
 	 char temp_file[129]="";
   char groupname[20],ap_groupid[30]="";
 	 psDbHead = utPltInitDb();             
	utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead,2,"plate",UT_TYPE_STRING,  255,imp_file,
	                             "groupid",  UT_TYPE_STRING,  30, ap_groupid);                  
//  printf("imp_file=%s\n",imp_file);    
  utPltPutVar(psDbHead,"groupid",ap_groupid);
  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncm/ncmapupload.html");
  return 0;
}
int ncmWebDispModiPass(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iReturn;
    long lId;
    char caUsername[32];
    char caTemp[1024];
#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
#endif   
iReturn = dsCltGetMyInfo(1,"Userid",&lId);
// iReturn = dsCltGetMyInfo(1,"Userid",&lId);
              
   	printf("iReturn=%d,lId=%d\n",iReturn,lId);
    if(iReturn < 0) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","口令修改","用户没有登录 ");
        return 0;
    }                               
    else {  
    	sprintf(caTemp,"select  name from dsuser where id=%d ",lId); 
    	pasDbOneRecord(caTemp,0,UT_TYPE_STRING,30,caUsername);       	                           
        utPltDbHead *psDbHead;
        psDbHead = utPltInitDbHead();
        utPltPutVar(psDbHead,"username", caUsername);
        utPltPutVarF(psDbHead,"userid",  "%lu",lId);
        utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/nc_modipass.htm");
        return 0;
   }
}
// 口令修改  
int ncmWebModiPass(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead){
   	char caUsername[33],caPassword1[33],caPassword2[33];
   	char caPass0[33],caOldpass[33],caUserid[33];
    utPltDbHead *psDbHead;
    char caKey[32],caKey0[64];
  	long lId;
   	int iReturn;
   	iReturn = dsCltGetMyInfo(1,"Userid",&lId);
   	if(iReturn < 0) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","用户登录","用户没有登录 ");
        return 0;
    }
   	iReturn = utMsgGetSomeNVar(psMsgHead,3,
   	      "rname",         UT_TYPE_STRING,30,caUsername,
					"password11",    UT_TYPE_STRING, 31, caPassword1,
					"password21",    UT_TYPE_STRING, 31, caPassword2);
    utStrDelSpaces(caPassword2);
    utStrDelSpaces(caPassword1);
    utStrDelSpaces(caUsername);
    sprintf(caKey0,"%s%s",caUsername,caPassword1);
    utMd5Ascii22(caKey0,strlen(caKey0),NULL,caKey);
    iReturn = pasDbExecSqlF("update dsuser set lKey='%s' where id=%d ",caKey,lId);
    if(iReturn != 0) {
        psDbHead = utPltInitDbHead();
        utPltPutVarF(psDbHead,"message", "口令修改出错, %d",iReturn);
        utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncmsg_back.htm");
        return 0;
    }
    else {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","密码修改",
                               "密码修改成功 ");
    }
    return 0;
}
// 往操作日志文件中写日志信息   
int ncmWriteSysLog(utMsgHead *psMsgHead,char *username,char *action,char *result,char *msg)
{
  char caIp[20];
  char caUsername[32];
  char caMsg[1256];
  char caMsg_t[1256];
  char caDate[20];
  int iReturn;
  unsigned long lUserid;
  char opt[32]="";
    dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(opt) - 1,opt);
	printf("opt = %s\n",opt);
  iReturn = utMsgGetSomeNVar(psMsgHead,1,"clientip",  UT_TYPE_STRING, 15, caIp);
  if(strlen(msg)!=0){
     	    strcpy(caDate,utTimFormat("%Y-%m-%d %H:%M:%S",time(0)));
     	    strcpy(caMsg_t,pasDbReplaceQuote1(msg));
          iReturn = pasDbExecSqlF("insert into ncadminlog(sdate,name,ipaddr,descr,action,result,groupcode) values ('%s','%s','%s','%s','%s','%s','%s')",
          caDate,username,caIp,caMsg_t,action,result,opt);
		  printf("insert into ncadminlog(sdate,name,ipaddr,descr,action,result,groupcode) values ('%s','%s','%s','%s','%s','%s','%s')\n",
          caDate,username,caIp,caMsg_t,action,result,opt);
	    if(iReturn == 0) {
	        pasDbCommit(NULL);
	    }
  }
  return 0; 
}
//系统登录
int ncmWebAuth(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  //    utMsgPrintMsg(psMsgHead);
	  utPltDbHead *psDbHead = utPltInitDbHead();
   	char caUsername[33],caPassword[33],caIp[16];
   	char caKey[64],caRight[65];
   	char caModi[24];
   	char caPass0[32];
   	char caContuse[16];
   	char *pProdSn,*pHost,*pDispName;
   	char caCount[8];
   	char caTemp2[1024];
   	unsigned long lIp;
   	unsigned long lTime;
   	char caDispname[32];
   	char caSkintheme[256];
   	int iCount,lAuthcount;
    int iLicense = 0;
    char caTemp[256],caAuthcount[8];
    unsigned long lUsrlevel;
    unsigned long lExpire,lUsers,lStart,lLicenseTime;
    long lUserid;
    long lSkintheme;
    char caSql[1024];
   	int iReturn,lDef,lLogflag,lDefCon,lFlag;
   	char caMbserno[36],screenwidth[15]="";
   	char caScreen[12];
    strcpy(caModi,"\0");
    lTime = time(0);
    strcpy(caAuthcount,utComGetVar_sd(psShmHead,"AuthCount","3"));
    lAuthcount=atol(caAuthcount);
    lLicenseTime = utComGetVar_ld(psShmHead,"LicenseTime",5);
    lUserid=0;
    
   	iReturn = utMsgGetSomeNVar(psMsgHead,7, "ModiPass",    UT_TYPE_STRING, 16, caModi,
																						"username",    UT_TYPE_STRING, 31, caUsername,
																						"password",    UT_TYPE_STRING, 31, caPassword,
																						"clientip",    UT_TYPE_STRING, 15, caIp,
																						"screenwidth", UT_TYPE_STRING, 15, screenwidth,
																						"contuse",     UT_TYPE_STRING, 11, caContuse,
																						"screenheight",UT_TYPE_STRING, 8 , caScreen);
										
		if(strlen(caScreen)==0) strcpy(caScreen,"768");
		if(strlen(screenwidth)==0) strcpy(screenwidth,"1440");
		/*
		if(strlen(caUsername)>0){
			memset(caDispname,0,sizeof(caDispname));		
			sprintf(caTemp,"select id,dispname from dsuser where name='%s' ",caUsername);
			pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lUserid,UT_TYPE_STRING,30,caDispname);
			sprintf(caTemp,"select sid,platepath from skintheme where status=0 and accountname='%s' ",caUsername);
			//printf("caTemp = %s\n",caTemp);
			pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSkintheme,UT_TYPE_STRING,255,caSkintheme);
	  }
	  if(strlen(caDispname)==0) strcpy(caDispname,caUsername);
    // 检查许可证  

    pProdSn = utComGetVar_sd(psShmHead,"ProductSn","\0");
    iLicense = pasInstGetLicense(pProdSn,&lExpire,&lUsers,&lStart);
    printf("*****0*****iLicense = %s\n",utTimFormat("%Y-%m-%d %H:%M:%S",lExpire));
//    iLicense = 1;
    if(iLicense != 1) { // 许可证不正确    
        psDbHead = utPltInitDbHead();
        utPltPutVarF(psDbHead,"prodsn",pProdSn);
        pHost = pasInstGetHostInfo("eth0");
        if(pHost == NULL) {
            utPltPutVarF(psDbHead,"message", "System Error");
            //ncsWriteSysLog(psMsgHead,caUsername,"01","Fail","License Error");
			ncmWriteSysLog(psMsgHead,caDispname,"01","Fail","License Error");
//            ncAdmWriteLog(caUsername,caIp,"License Error");
            utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/login2.htm");
            return 0;
        }
        utPltPutVarF(psDbHead,"hostinfo",pHost);
        pDispName = utComGetVar_sd(psShmHead,"YourDispName","\0");
        utPltPutVarF(psDbHead,"name",pDispName);
        //ncsWriteSysLog(psMsgHead,caDispname,"01","Fail","License Error");
		ncmWriteSysLog(psMsgHead,caDispname,"01","Fail","License Error");
//        ncAdmWriteLog(caUsername,caIp,"License Error");
        utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/main_licenseerror.htm");
    }
    
    if(utStrIsSpaces(caContuse) && lTime > lExpire - lLicenseTime * 86400L) { // License 即将到期  
        psDbHead = utPltInitDbHead();
        utPltPutVarF(psDbHead,"prodsn",pProdSn);
        pHost = pasInstGetHostInfo("eth0");
        if(pHost == NULL) {
            utPltPutVarF(psDbHead,"message", "System Error");
            //ncAdmWriteLog(caDispname,caIp,"System Error");
			ncmWriteSysLog(psMsgHead,caDispname,"02","","System Error");
            utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/login2.htm");
            return 0;
        }
        utPltPutVarF(psDbHead,"hostinfo",pHost);
        pDispName = utComGetVar_sd(psShmHead,"YourDispName","\0");
        utPltPutVarF(psDbHead,"name",pDispName);
        utPltPutVarF(psDbHead,"sdate",utTimFormat("%Y-%m-%d %H:%M:%S",lExpire));
        utPltPutVarF(psDbHead,"username",caUsername);
        utPltPutVarF(psDbHead,"password",caPassword);
        utPltPutVarF(psDbHead,"ModiPass",caModi);
        //ncAdmWriteLog(caUsername,caIp,"License Expire");
		ncmWriteSysLog(psMsgHead,caUsername,"02","","License Expire");
        utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/main_licenseexpire.htm");
        return 0;
    }
    
    lIp = utComHostAddress(caIp);
  
   
    utStrDelSpaces(caUsername);
//    utStrToLower(caUsername);
    utStrDelSpaces(caPassword);
    if((strlen(caUsername)==0)||(strlen(caPassword)==0)){
    	 psDbHead = utPltInitDbHead();
    	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/login2.htm");
    	 return 0;
    }
    */
    iReturn = dsCltUserLogin(caUsername,caPassword,caIp); 
	printf("iReturn = %d \n", iReturn);
	if(iReturn != 0)
	{
		utPltPutVarF(psDbHead,"message", "用户名或密码错误！");
		utPltOutToHtml(iFd, psMsgHead, psDbHead, "../html/enter.html");
		return 0; 
	}
    /*if(iReturn != 0) {
        if(iReturn == (-1)) 
        {
            psDbHead = utPltInitDbHead();
            utPltPutVarF(psDbHead,"message", ncLang("0742用户不存在"));
            sprintf(caTemp2,ncLang("0205第%d次用户不存在"),iCount);
            //ncsWriteSysLog(psMsgHead,caDispname,"01","Fail",caTemp2);
			ncmWriteSysLog(psMsgHead,caDispname,"01","Fail",caTemp2);
            utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/login2.htm");
        }
        else if(iReturn == (-2)) {
            psDbHead = utPltInitDbHead();
            utPltPutVarF(psDbHead,"message", ncLang("0353口令不正确"));
            sprintf(caTemp2,ncLang("0203第%d次口令不正确"),iCount);
            //ncsWriteSysLog(psMsgHead,caDispname,"01","Fail",caTemp2);
			ncmWriteSysLog(psMsgHead,caDispname,"01","Fail",caTemp2);
            utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/login2.htm");
        }
        else {
            psDbHead = utPltInitDbHead();
            sprintf(caTemp2,ncLang("0204第%d次验证不正确"),iCount);
            //ncsWriteSysLog(psMsgHead,caDispname,"01","Fail",caTemp2);
			ncmWriteSysLog(psMsgHead,caDispname,"01","Fail",caTemp2);
            utPltPutVarF(psDbHead,"message", ncLang("0718验证不正确"));
            utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/login2.htm");
        }
        return 0;
    }else {

        psDbHead = utPltInitDbHead();
        utPltPutVarF(psDbHead,"height","%d",atol(caScreen)-200);
        ncAdmSetAdmIp(psShmHead,lIp);

       if(strcasecmp(utComGetVar_sd(psShmHead,"secondpasswd","No"),"YES") == 0){
       	utPltPutVar(psDbHead,"username",caUsername);
       	dsCltSetSessionValue(1,"mbflag",UT_TYPE_LONG,4,10);
       	//双密码验证
       	utPltOutToHtml(iFd,psMsgHead,psDbHead,"ds/ds_secondauth.htm");
       	return 0;
       	
       }

lDefCon = atol(utComGetVar_sd(psShmHead,"MbContDef","0"));

lDef = atol(utComGetVar_sd(psShmHead,"MbLoginDef","0"));
memset(caMbserno,0,sizeof(caMbserno));
sprintf(caSql,"select mbserno,logflag,flag from ncuserbindmb where userid=%d ",lUserid);
pasDbOneRecord(caSql,0,UT_TYPE_STRING,16,caMbserno,
                       UT_TYPE_LONG,4,&lLogflag,
                       UT_TYPE_LONG,4,&lFlag);

if(strlen(caMbserno)>0){
	if(lFlag==0){
		   dsCltSetSessionValue(1,"mbflag",UT_TYPE_LONG,4,10);
		  }
	if(lLogflag==1){
		 utPltPutVar(psDbHead,"fname","ncMbLogin");
		 utPltPutVar(psDbHead,"mbserno",caMbserno);
		 

		 utPltPutVarF(psDbHead,"userid","%d",lUserid);
	   utPltOutToHtml(iFd,psMsgHead,psDbHead,"ds/ds_mbauth.htm");
	   return 0;
	  }
}
else{
		if(lDefCon==0)
		   dsCltSetSessionValue(1,"mbflag",UT_TYPE_LONG,4,10);
	
	if(lDef==1){
		utPltPutVarF(psDbHead,"message", ncLang("1555系统要求动态验证，但该用户没有绑定移动密宝"));
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/login2.htm");
		return 0;
	}
}

      ncmWriteSysLog(psMsgHead,caDispname,"01","Success","成功登录到系统");
    
     sprintf(caTemp,"select id,usrlevel from dsuser where name='%s' ",caUsername);
      pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lUserid,
                              UT_TYPE_LONG,4,&lUsrlevel);
     sprintf(caTemp,"%s/hotel/main_group.htm",utComGetVar_sd(psShmHead,"PlatePath","/home/ncmysql/nc/plate"));

     if(((lUsrlevel==3)||(lUsrlevel==2))&&(strcasecmp(utComGetVar_sd(psShmHead,"HotelVesion","No"),"Yes")==0)){

     }else if(((lUsrlevel==3)||(lUsrlevel==2))&&(utFileIsExist(caTemp)==1)){
       utPltOutToHtml(iFd,psMsgHead,psDbHead,"hotel/main_group.htm");
       return 0;
     }
     else{
     	char caConfFile[64];
     	long lCount=0;
     
     	sprintf(caTemp,"select count(*) from dsrole a,dsuserrole b where a.id=b.roleid and b.id=%d and (a.name='all' or a.name='全部权限') ",lUserid);
     	pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
     	//插入登录日志
			char desc[200]="";
			if(lUsrlevel==1)
				strcpy(desc,"系统管理员:");
			else if(lUsrlevel==3)
				strcpy(desc,"一般操作员员:");
			strcat(desc,caUsername);
			strcat(desc,"于");
			strcat(desc,utTimFormat("%Y/%m/%d %H:%M:%S",time(0)));
			strcat(desc,"登录");
			//printf("== **1** =desc=%s\n",desc);
    	sprintf(caTemp,"insert into ncyq_userlog(yqlog_name,yqlog_ip,yqlog_dept,yqlog_in,yqlog_desc) values('%s','%s','%s','%s','%s')", \
   	 	caUsername,caIp,"",utTimFormat("%Y/%m/%d %H:%M:%S",time(0)),desc);
   		iReturn=pasDbExecSql(caTemp,0); 	
     	
     	if(strcasecmp(utComGetVar_sd(psShmHead,"menubyrole","No"),"Yes")==0){
	     	int roleid=0;
	     	sprintf(caTemp,"select b.roleid from dsrole a,dsuserrole b where a.id=b.roleid and b.id=%d ",lUserid);
	     	pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&roleid);
	     	printf("== **2000** ==  roleid=%d\n",roleid);
	     	utPltPutVarF(psDbHead,"roleid","%d",roleid);
	     	//自定义菜单   把roleid传过去
	     	utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/ncdynamic_jump.html");
    	}else{
	     	if(lCount>0){
	     		utPltPutVar(psDbHead,"screenwidth",screenwidth);
	     		utPltPutVar(psDbHead,"username",caUsername);
	     		utPltPutVar(psDbHead,"skintheme",caSkintheme);
				printf("caSkintheme=%s \n",caSkintheme);
	     		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/main.htm");
	     		return 0;
	     	}
	    }
//根据用户角色动态生成菜单
//     	else if(strcasecmp(utComGetVar_sd(psShmHead,"menubyrole","No"),"Yes")==0){
// 
//     		utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/main_1.htm");
//     		return 0;
//     	}
    }
     	
     	*/
    
    utPltPutVar(psDbHead,"username",caUsername);
	utPltPutVar(psDbHead,"skintheme",caSkintheme);
    utPltPutVar(psDbHead,"screenwidth",screenwidth);
    printf("screenwidth=%s \n",screenwidth);
	printf("caSkintheme=%s \n",caSkintheme); 		 	
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/main.htm");
    return 0;
}
//层次树——供comp选择使用
int ncmTreeGroup_comp_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
   	char caGroupname[32],caGroupid[16];
   	
   	char caName[32];
   	char caWhere[256];
   	char caSql[256];
   	char caCurPg[16],caTotRec[16];
   	char caPid[16];
   	unsigned long lLevel,lPid;
   	int  iNum;
  	char caFilter[256];
  	char caNo[16];
   	unsigned long lGroupid,lGroupid0,lCurPg,lTotRec,lRowNum,lStartRec;
    utPltDbHead *psDbHead;
   	int iReturn,i;
   	char caVar[256],caFname[64],caBfname[64];
   	char caValue[256];
   	char caVar1[32];
   	char caOpt[256];
   	long lId;
   	char *p,*pVar;
   	unsigned char *pHash;
    pasLHashInfo  sHashInfo,sHashInfo2;
    struct stData{
    	unsigned long groupid;
    	unsigned long pid;
    	char caGroupname[32];
    	struct stData * pArea;  //子节点
 		  long lSum;             //子节点个数
    };
    char caTemp[2048];
    long lCount,lSum,lSum2,m,n;
    struct stData *psData,*pAreastatdata,*psData2,*psData3;
    
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);

	printf("ncmTreeGroup_comp_v4\n");
     
    iReturn = dsCltGetSessionValue(1,"groupid",UT_TYPE_LONG,sizeof(long),&lGroupid0);
    strcpy(caWhere,"\0");
    memset(caOpt,0,sizeof(caOpt)); 
    iReturn = dsCltGetMyInfo(1,"Userid",&lId);
    if(iReturn==0){
 	    pasDbOneRecord("select opt from dsuser where id=:id",1,"id",UT_TYPE_LONG,lId,UT_TYPE_STRING,255,caOpt);
    } 



    lRowNum = utComGetVar_ld(psShmHead,"GroupTreeRow",1000);
  
    pHash = (unsigned char *)pasLHashInit(lRowNum,lRowNum,sizeof(struct stData),0,4);
    
    if(pHash == NULL) {
             printf(" Memory HttpHash Error \n");
             return (-1);
    }
//    sprintf(caTemp,"select pid,groupid,groupname from ncsgroup ");
    sprintf(caTemp,"select pid,groupid,groupname from ncgroup ");
   
   if(!strstr(caOpt,"all")&&(strlen(caOpt)>0)){
			sprintf(caTemp+strlen(caTemp)," and (groupid in (%s) or groupid in (select groupid from ncgroup where pid in (%s))) ",caOpt,caOpt);
   }
   printf("caTemp=%s\n",caTemp);
    psCur=pasDbOpenSql(caTemp,0);
  
    if(psCur==NULL){
    	free(pHash);
    	return 0;
    }
    lGroupid=0;
    lPid=0;
    memset(caGroupname,0,sizeof(caGroupname));
    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lPid,
                                   UT_TYPE_LONG,4,&lGroupid,
                                   UT_TYPE_STRING,31,caGroupname);
    while(iReturn == 0||iReturn==1405) { 
    	 psData = (struct stData *)pasLHashLookA(pHash,&lGroupid);    	
       if(psData){       
       	psData->pid=lPid;
       	strcpy(psData->caGroupname,caGroupname);
       }
    lGroupid=0;
    lPid=0;
    memset(caGroupname,0,sizeof(caGroupname));
    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lPid,
                                   UT_TYPE_LONG,4,&lGroupid,
                                   UT_TYPE_STRING,31,caGroupname);
    }
    pasDbCloseCursor(psCur);
    
    
    pAreastatdata=(struct stData*)malloc((1000)*sizeof(struct stData));
    if(pAreastatdata==NULL){
  	    utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;
    }
    memset(pAreastatdata,0,(1000)*sizeof(struct stData));
    lSum=0;

    psData = (struct stData *)pasLHashFirst(pHash,&sHashInfo);

     while(psData && lSum < 1000){
             psData2 = (struct stData *)pasLHashLook(pHash,&psData->pid);
      	 	  //找不到，代表是根
            if(psData2==NULL){
            	pAreastatdata[lSum].pid=psData->pid;
            	pAreastatdata[lSum].groupid=psData->groupid;
            	strcpy(pAreastatdata[lSum].caGroupname,psData->caGroupname);
            	//分配子节点
            	pAreastatdata[lSum].pArea=(struct stData*)malloc((100)*sizeof(struct stData));
            	memset(pAreastatdata[lSum].pArea,0,100*sizeof(struct stData));
            	//获取下个节点的数据
            	 psData3 = (struct stData *)pasLHashFirst(pHash,&sHashInfo2);
            	 lSum2=0;
            	 while(psData3&&lSum2<100){
            	 	 if(pAreastatdata[lSum].groupid==psData3->pid){
            	 	 	 pAreastatdata[lSum].pArea[lSum2].pid=psData3->pid;
            	 	 	 pAreastatdata[lSum].pArea[lSum2].groupid=psData3->groupid;
            	 	 	 strcpy(pAreastatdata[lSum].pArea[lSum2].caGroupname,psData3->caGroupname);
            	 	 	 pAreastatdata[lSum].lSum++;
            	 	 	 lSum2++;
            	 	 }
            	 	 	psData3 = (struct stData *)pasLHashNext(&sHashInfo2);
            	 }
            	
            	lSum++;
            }
     
     	psData = (struct stData *)pasLHashNext(&sHashInfo);
     }
    iNum=0;
     psDbHead = utPltInitDb();
    
     for(i=0;i<lSum;i++){
     	if(iNum>0){
     		   		utPltPutLoopVar(psDbHead,"dhf",iNum+1,",");
   	  }
   	  utPltPutLoopVar(psDbHead,"area",iNum+1,pAreastatdata[i].caGroupname);
   	  utPltPutLoopVarF(psDbHead,"groupid",iNum+1,"%d",pAreastatdata[i].groupid);
   	  if(pAreastatdata[i].lSum>0){
   	  	utPltPutLoopVar(psDbHead,"iconCls",iNum+1,"task-folder");
   	  }
   	  else{
   	  	utPltPutLoopVar(psDbHead,"iconCls",iNum+1,"task");
   	  }
   	    //输出第二层节点
   	    for(m=0;m<pAreastatdata[i].lSum;m++){
   	     if(m>0){
    	 	 	 utPltPutLoopVar2(psDbHead,"dhs",iNum+1,m+1,",");
    	 	}      	   
    	 	  utPltPutLoopVarF2(psDbHead,"groupid",iNum+1,m+1,"%d",pAreastatdata[i].pArea[m].groupid);
    	 	  utPltPutLoopVar2(psDbHead,"area",iNum+1,m+1,pAreastatdata[i].pArea[m].caGroupname);
    	 	  //输出第三层节点
    	 	  n=0;
    	 	   psData = (struct stData *)pasLHashFirst(pHash,&sHashInfo);
      
      	   while(psData && n < 100){ 
      	   	if(pAreastatdata[i].pArea[m].groupid==psData->pid){
      	   	if(n>0){
      	 	 		utPltPutLoopVar3(psDbHead,"dht",iNum+1,m+1,n+1,",");
      	 	 	}
      	 	 	utPltPutLoopVar3(psDbHead,"area",iNum+1,m+1,n+1,psData->caGroupname);
      	 	 	utPltPutLoopVarF3(psDbHead,"groupid",iNum+1,m+1,n+1,"%d",psData->groupid);
      	   	n++;
      	   }
      	   	psData = (struct stData *)pasLHashNext(&sHashInfo);
      	   	
      	  }
    	 	  
    	 	  
   	    }
   	  
        iNum++;
      }

      if(pHash)  free(pHash);
	    for(i=0;i<lSum;i++){ 
	        		free(pAreastatdata[i].pArea);
	    }
      free(pAreastatdata);

    utPltPutVar(psDbHead,"rootname",utComGetVar_sd(psShmHead,"rootname","辽阳市"));
//utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncm_group_select_tree.htm");
    return 0;
}

//员工管理
int ncmWebShowUserMan(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  utMsgPrintMsg(psMsgHead);
	  
	  // 初始化数据库
	  char caTemp[4096] = "";
	  char caTemp1[4096] = "";
	  char caTemp2[18] = "";
	  char caTempd[4096] = "";
	  char *q=NULL;
    int iNum=0;
    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();
	 	
	 	char caKey[32] = "";
	 	char userid[32] = "";
	 	char username[32] = "";
	 	char dispname[32] = "";
	 	char groupname[32] = "";
	 	char groupid[32] = "";
	 	char ssid[32] = "";
	 	char proname[32] = "";
	 	char openid[32] = "";
	 	char usermac[1256] = "";
	 	char password[32] = "";
	 	char mark[128] = "";
	 	char addtime[32] = "";
	 	char lasttime[32] = "";
	 	char sexpdate[32] = "";
	 	char sexptime[32] = "";
	 	char expiretime[32] = "";
	 	char usertype[32] = "";
	 	char flags[32] = "";
	 	char maxbindmac[32] = "";
	 	char groupcode[32] = "";
	 	char areaid[32] = "";
	 	unsigned long l_expiretime=0;
    
    unsigned long lUserid=0;
    char caUsername[32]="";
    char caDispname[32]="";
    char caGroupname[32]="";
    unsigned long lGroupid=0;
    char caSsid[32]="";
    char caProname[32]="";
    char caOpenid[32]="";
    char caPassword[32]="";
    char caMark[128]="";
    unsigned long lAddtime=0;
    unsigned long lModitime=0;
    unsigned long lLasttime=0;
    unsigned long lExpiretime=0;
    unsigned long lUsertype=0;
    unsigned long lFlags=0;
    unsigned long lMaxbindmac=0;
    char s_usertype[32]="";
    char s_flags[32]="";
    
    char dUsername[32] = "";
    char dDispname[32] = "";
    char dGroupname[32] = "";
    char dSsid[32] = "";
    char dPassword[32] = "";
    unsigned long dUsertype = 0;
    unsigned long dFlags = 0;
    unsigned long dMaxbindmac = 0;
    // 解除绑定mac地址
    char uUsername[32] = "";
    unsigned long uGroupid = 0;
    
    char caUpdate[16] = "";
	  char caSesid[1024] = "";
	  char caDel[16] = "";
	  char caRemovemac[16] = "";
	  char caRefresh[16] = "";
    char mokeyword[64] = "";
    unsigned long lSid=0;
    char caTemp_d[128] = "";
    
    struct groupdate{
    		char user[32];
    		char dispname[32];
    		char ssid[32];
    		char password[32];
    		int  maxMac;
    		char groupname[32];	
    };
    struct groupdate *pdate;
    
    // 数据表
	  unsigned long lCount1 = 0; // 表中记录的个数
	  unsigned long lCount = 0; // 表中记录的个数
	  long iReturn; // 单记录查询
	  char start[17] = ""; // 默认记录从0开始
	  char limit[17] = ""; // 限定一页中的记录是40条
	  unsigned long iStart = 0; // 转换后的默认开始atoi(start)
	  unsigned long iLimit = 0; // 转换后的限定记录条数atoi(limit)
	 
	  // 翻页
	  unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	  // 导出
	  char caExport[32] = "";
	  FILE *fp = NULL;
	  char caFilename[32] = "";
	  char caFile[128] = "";
	  char caPath[128] = "";
	 
	  // 排序
	  char sort[17] = "";
    char dir[9] = "";
    
    char caOptd[32] = "";
    char users[100][20];
    long lLen,i;
    char caMac[20],caMac_all[1256],mystype[32] = "",caMac_allexp[1256] = "";
    char s_lasttime[32]="";
    char s_expiretime[32]="";
	  dsCltGetSessionValue(1,"stype",UT_TYPE_STRING,30,mystype);
	  dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,30,caOptd);
    printf("mystype = %s\n", mystype);
    printf("caOptd = %s\n", caOptd);
    
    char cnfSsid[32] = "";
    strcpy(cnfSsid, ncmGetSsidByStype(psShmHead, mystype, 1));
    printf("cnfSsid = %s\n", cnfSsid);
    
    char cnfMac[32] = "";
    strcpy(cnfMac,utComGetVar_sd(psShmHead,"MaxBDMac",  "20"));
    printf("cnfMac = %s\n", cnfMac);
    
    // 得到mac的groupid
    unsigned long lStype = 0;
		if (strlen(mystype) > 0){	
		    sprintf(caTemp,"select stype from ncmautotype where (shopcode+0)=%lu and (namecode+0) = 1 ",atoi(mystype));
		    printf("caTemp = %s \n",caTemp);
		    pasDbOneRecord(caTemp,0,UT_TYPE_ULONG,sizeof(long),&lStype);
		    printf("lStype = %d \n",lStype);
	  } else {
	   		lStype = 0;	
	  }
    
	  iReturn = utMsgGetSomeNVar(psMsgHead,30,"key",             UT_TYPE_STRING, sizeof(caKey)-1,      caKey,
	                                          "userid",          UT_TYPE_STRING, sizeof(userid)-1,     userid,
														                "usrname",         UT_TYPE_STRING, sizeof(username)-1,   username,
														                "dispname",        UT_TYPE_STRING, sizeof(dispname)-1,   dispname,
														                "groupid",         UT_TYPE_STRING, sizeof(groupid)-1,    groupid,
														                "ssid",            UT_TYPE_STRING, sizeof(ssid)-1,       ssid,
														                "proname",         UT_TYPE_STRING, sizeof(proname)-1,    proname,
														                "openid",          UT_TYPE_STRING, sizeof(openid)-1,     openid,
														                "usermac",         UT_TYPE_STRING, sizeof(usermac)-1,    usermac,    
														                "password",        UT_TYPE_STRING, sizeof(password)-1,   password,      
														                "mark",            UT_TYPE_STRING, sizeof(mark)-1,       mark,  
														                "addtime",         UT_TYPE_STRING, sizeof(addtime)-1,    addtime, 
														                "lasttime",        UT_TYPE_STRING, sizeof(lasttime)-1,   lasttime,
														                "sexpdate",        UT_TYPE_STRING, sizeof(sexpdate)-1,   sexpdate, 
														                "sexptime",        UT_TYPE_STRING, sizeof(sexptime)-1,   sexptime,          
														                "usertype",        UT_TYPE_STRING, sizeof(usertype)-1,   usertype, 
														                "flags",           UT_TYPE_STRING, sizeof(flags)-1,      flags,
														                "maxbindmac",      UT_TYPE_STRING, sizeof(maxbindmac)-1, maxbindmac,
														                "groupcode",       UT_TYPE_STRING, sizeof(groupcode)-1,  groupcode,
														                "areaid",          UT_TYPE_STRING, sizeof(areaid)-1,     areaid,
														                "mokeyword",       UT_TYPE_STRING, sizeof(mokeyword)-1,  mokeyword,
														                "refresh",	       UT_TYPE_STRING, sizeof(caRefresh)-1,  caRefresh,
														                "removemac",       UT_TYPE_STRING, sizeof(caRemovemac)-1,caRemovemac,
														                "del",             UT_TYPE_STRING, sizeof(caDel)-1,      caDel,
														                "selsid",          UT_TYPE_STRING, sizeof(caSesid)-1,    caSesid,
														                "start",           UT_TYPE_STRING, sizeof(start)-1,      start,
														   	            "limit",           UT_TYPE_STRING, sizeof(limit)-1,      limit,
														   	            "sort",            UT_TYPE_STRING, sizeof(sort)-1,       sort,
																						"dir",             UT_TYPE_STRING, sizeof(dir)-1,        dir,
																						"caExport",        UT_TYPE_STRING, sizeof(caExport)-1,   caExport);
	  
	  trim(mokeyword);
	  
	  //未操作选择框
	  if(strlen(mokeyword)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,mokeyword,caTemp,sizeof(mokeyword)-1));
    	strcpy(mokeyword,caTemp_d);
    }
	  if(strlen(username)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,username,caTemp,sizeof(username)-1));
    	strcpy(username,caTemp_d);
    }
    if(strlen(dispname)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,dispname,caTemp,sizeof(dispname)-1));
    	strcpy(dispname,caTemp_d);
    }
    if(strlen(proname)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,proname,caTemp,sizeof(proname)-1 ));
    	strcpy(proname,caTemp_d);
    }
    if(strlen(usermac)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,usermac,caTemp,sizeof(usermac)-1));
    	strcpy(usermac,caTemp_d);
    }
    if(strlen(ssid)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,ssid,caTemp,sizeof(ssid)-1));
    	strcpy(ssid,caTemp_d);
    }
    if(strlen(password)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,password,caTemp,sizeof(password)-1));
    	strcpy(password,caTemp_d);
    }
    if(strlen(mark)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,mark,caTemp,sizeof(mark)-1));
    	strcpy(mark,caTemp_d);
    }
	  if(strlen(caSesid)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,caSesid,caTemp,sizeof(caSesid)-1));
    	strcpy(caSesid,caTemp_d);
    }
    
    printf("caKey = %s\n", caKey);
    if(!utStrIsSpaces(caKey)){  //增加一个组
    	 //printf("mystype = %s  groupcode = %s\n", mystype,groupcode);
    	 if(strlen(groupcode)==0){
    	 	  if(strlen(caOptd)==0){
    	 	  	  char optd[32] = "";
    	 	  		sprintf(optd,"%s%s",ncmWebAutoDefGet16(psShmHead, iFd,psMsgHead,mystype),"");
    	 	  		printf("optd = %s\n", optd);	
    	 	  		strcpy(groupname, optd);	
    	 	  }else{
    	 	  	  printf("caOptd = %s\n", caOptd);	
    	 	  		strcpy(groupname, caOptd);	
    	 	  }
    	 }else{
//    	 		sprintf(groupname,"%s%s",ncmWebAutoDefGet16(psShmHead, iFd,psMsgHead,mystype),groupcode);
    	 		sprintf(groupname,"%s",groupcode);
    	 }
    	 printf("groupname = %s\n", groupname);
    	 
    	 sprintf(expiretime, "%s %s", sexpdate, sexptime);
    	 printf("expiretime = %s\n", expiretime);
    	 
    	 lSid=utMd5Code(username,strlen(username),"ffff");
   	   if(lSid>2000000000) lSid=lSid-2000000000;
   	   if(lSid>2000000000) lSid=lSid-2000000000;
   	   printf("lSid = %d\n",lSid);
   	   
       if(strcmp(caKey,"add")==0){  
       	    sprintf(caTemp, "select count(*) from ncsrvuser where username='%s'", username);
		    	  pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, sizeof(long), &lCount);
		    	  if(lCount>0){
		    	 			utPltPutVar(psDbHead,"msg","用户已存在！");
				    	 	utPltPutVar(psDbHead,"mesg","数据加载成功");
					 	   	utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_webuserman_dr_list.htm");
					 	   	return 0;
		    	  }

       	    ncTimeToLong(expiretime, &l_expiretime);
		    	  printf("l_expiretime = %lu\n", l_expiretime);
		    	  /*if(l_expiretime<time(0)){
		    	 	    utPltPutVar(psDbHead,"msg","有效时间不能小于当前时间！");
		    	 	    utPltPutVar(psDbHead,"mesg","数据加载成功");
			 	   			utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_webuserman_dr_list.htm");
		    	 			return 0;
		    	  }*/
		    	  
		    	  if(strlen(usermac) != 0){
					  		q =	utStrSkipSpaces(usermac);
					  		while(q && *q){
					  				q = utStrGetWord(q,caTemp2,18,"|");
					  				sprintf(caTempd,"insert into ncsrvusermac (usermac, username, groupid, starttime, lasttime, moditime, flags) values ('%s', '%s', %lu, %lu, %lu, %lu, %lu)", caTemp2,username,lStype,time(0),time(0),time(0),atol(flags));
						 				printf("caTempd = %s\n", caTempd);
								 		pasDbExecSqlF(caTempd);
								 		
								 		char *aUsername = NULL;
								 		char aUsermac[6];
								 		unsigned long aGroupid = 0;
								 		aUsername = strdup(username);
//								 		aUsermac = strdup(caTemp2);
								 		pasCvtMacI((char *)caTemp2,aUsermac);
								 		aGroupid = lStype;
								 		printf("aGroupid = %d, aUsername = %s, aUsermac = %s\n", aGroupid,aUsername,pasCvtMac(aUsermac));
								 		ncSrvAddBindMac(psShmHead,aGroupid,aUsername,aUsermac);
								 		if(aUsername){
								         free(aUsername);
								         aUsername = NULL;
							      }
					  				q++;	
					  		}
					  }
    	 		  
//            sprintf(caTemp, "insert into ncsrvuser(userid,username,dispname,groupname,groupid,ssid,proname,openid,password,mark,addtime,moditime,lasttime,expiretime,usertype,flags) values(%lu,'%s','%s','%s',%lu,'%s','%s','%s','%s','%s',%lu,%lu,%lu,%lu,%lu,%lu)", lSid,username,dispname,groupname,atol(groupid),ssid,proname,openid,password,mark,time(0),time(0),time(0),l_expiretime,atol(usertype),atol(flags));
//       			printf("caTemp = %s\n", caTemp);
//			 			pasDbExecSqlF(caTemp);
			 			
			 		  char *pSsid = NULL;  //其它类似
			 		  char *pUsername = NULL;
			 		  char *pPassword = NULL;
			 		  char *pGroupname = NULL;
			 		  char *pDisp = NULL;
			      int  iStatus = 0;
			      unsigned long lusertype = atol(usertype);
			      unsigned long lflags = atol(flags);
			      unsigned long lMaxMac = 0;
			      if(atol(maxbindmac)>0){
			       		lMaxMac = atol(maxbindmac);
			      }else{
			      	  char caMaxmac[11] = "";
			      	  strcpy(caMaxmac,utComGetVar_sd(psShmHead,"NcmMaxUMac","1"));
			      		lMaxMac = atol(caMaxmac);	
			      }
			      pSsid = strdup(ssid);
			      pUsername = strdup(username);
			      pPassword = strdup(password);
			      pGroupname = strdup(groupname);
			      if(strlen(dispname)==0){
			      		pDisp = strdup(username);	
			      }else{
			      		pDisp = strdup(dispname);	
			      }
			      printf("pSsid = %s, pUsername = %s, pPassword = %s, pGroupname = %s, pDisp = %s, lusertype = %d, lflags = %d, lMaxmac = %d\n", pSsid,pUsername,pPassword,pGroupname,pDisp,lusertype,lflags,lMaxMac);
			      ncPortalUser *result = NULL;
			      result = ncSrvSynUserInfo(psShmHead,pGroupname,pSsid,pUsername,pPassword,NULL,pDisp,lusertype,lMaxMac,lflags,&iStatus);
			      if(result){
			      		printf("status = %d\n", result->cStatus);
			      }
			      if(pSsid){
				         free(pSsid);
				         pSsid = NULL;
			      }
			      if(pUsername){
				         free(pUsername);
				         pUsername = NULL;
			      }
			      if(pPassword){
				         free(pPassword);
				         pPassword = NULL;
			      }
			      if(pGroupname){
				         free(pGroupname);
				         pGroupname = NULL;
			      }
			      if(pDisp){
				         free(pDisp);
				         pDisp = NULL;
			      }
       }else {   //更新
       	    ncTimeToLong(expiretime, &l_expiretime);
		    	 	printf("l_expiretime = %lu\n", l_expiretime);
		    	 	/*if(l_expiretime<atol(lasttime)){
		    	 	    utPltPutVar(psDbHead,"msg","有效时间不能小于最后访问时间！");
		    	 	    utPltPutVar(psDbHead,"mesg","数据加载成功");
			 	   			utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_webuserman_dr_list.htm");
		    	 			return 0;
		    	 	}*/
		    	 	
                    
                    /*
		    	 	pasDbExecSqlF("delete from ncsrvusermac where username='%s'",username);
		    	 	char *cUsername = NULL;
	  				unsigned long cGroupid = 0;
	  				cUsername = strdup(username);
	  				printf("cUsername = %s, cGroupid = %d\n", cUsername, cGroupid);
	  				ncSrvDelUserMacByName(psShmHead,cGroupid,cUsername);
	  				if(cUsername){
				         free(cUsername);
				         cUsername = NULL;
			        }
		    	 	if(strlen(usermac) != 0){
					  		q =	utStrSkipSpaces(usermac);
					  		while(q && *q){
					  				q = utStrGetWord(q,caTemp2,18,"|");
					  				printf("caTemp2 = %s\n", caTemp2);
					  				sprintf(caTempd,"insert into ncsrvusermac (usermac, username, groupid, starttime, lasttime, moditime, flags) values ('%s', '%s', %lu, %lu, %lu, %lu, %lu)", caTemp2,username,atol(groupid),atol(addtime),time(0),time(0),atol(flags));
						 				printf("caTempd = %s\n", caTempd);
								 		pasDbExecSqlF(caTempd);
								 		
								 		char *aUsername = NULL;
								 		char aUsermac[6];
								 		unsigned long aGroupid = 0;
								 		aUsername = strdup(username);
								 		pasCvtMacI((char *)caTemp2,aUsermac);
								 		printf("aGroupid = %d, aUsername = %s, aUsermac = %s\n", aGroupid,aUsername,pasCvtMac(aUsermac));
								 		ncSrvAddBindMac(psShmHead,aGroupid,aUsername,aUsermac);
								 		if(aUsername){
								         free(aUsername);
								         aUsername = NULL;
							      }
					  				q++;	
					  		}
					  }
       	            */
//          	sprintf(caTemp, "update ncsrvuser set userid=%lu,username='%s',dispname='%s',groupname='%s',groupid=%lu,ssid='%s',proname='%s',openid='%s',password='%s',mark='%s',addtime=%lu,moditime=%lu,lasttime=%lu,expiretime=%lu,usertype=%lu,flags=%d where userid='%s'",lSid,username,dispname,groupname,atol(groupid),ssid,proname,openid,password,mark,atol(addtime),time(0),atol(lasttime),l_expiretime,atol(usertype),atol(flags),userid);
//    	 			printf("caTemp = %s\n", caTemp);
//			 			pasDbExecSqlF(caTemp);
			 			//sprintf(caTemp, "update ncsrvuser set flags=%d where userid='%s'",atol(flags),userid);
    	 			//printf("caTemp = %s\n", caTemp);
			 			//pasDbExecSqlF(caTemp);
			 			
			 		  char *pSsid = NULL;  //其它类似
			 		  char *pUsername = NULL;
			 		  char *pPassword = NULL;
			 		  char *pGroupname = NULL;
			 		  char *pDisp = NULL;
			      int  iStatus = 0;
			      unsigned long lusertype = atol(usertype);
			      unsigned long lflags = atol(flags);
			      unsigned long lMaxMac = 0;
			      if(atol(maxbindmac)>0){
			       		lMaxMac = atol(maxbindmac);
			      }else{
			      	  char caMaxmac[11] = "";
			      	  strcpy(caMaxmac,utComGetVar_sd(psShmHead,"NcmMaxUMac","1"));
			      		lMaxMac = atol(caMaxmac);	
			      }
			      pSsid = strdup(ssid);
			      pUsername = strdup(username);
			      pPassword = strdup(password);
			      pGroupname = strdup(groupname);
			      if(strlen(dispname)==0){
			      		pDisp = strdup(username);	
			      }else{
			      		pDisp = strdup(dispname);	
			      }
			      printf("pSsid = %s, pUsername = %s, pPassword = %s, pGroupname = %s, pDisp = %s, lusertype = %d, lflags = %d, lMaxmac = %d\n", pSsid,pUsername,pPassword,pGroupname,pDisp,lusertype,lflags,lMaxMac);
			      ncPortalUser *result = NULL;
			      result = ncSrvSynUserInfo(psShmHead,pGroupname,pSsid,pUsername,pPassword,NULL,pDisp,lusertype,lMaxMac,lflags,&iStatus);
			      if(result){
  			            int iRet;
			      		printf("status = %d\n", result->cStatus);
			      		iRet = ncSrvSynUserMac(psShmHead, result,usermac);
			      }
			      if(pSsid){
				         free(pSsid);
				         pSsid = NULL;
			      }
			      if(pUsername){
				         free(pUsername);
				         pUsername = NULL;
			      }
			      if(pPassword){
				         free(pPassword);
				         pPassword = NULL;
			      }
			      if(pGroupname){
				         free(pGroupname);
				         pGroupname = NULL;
			      }
			      if(pDisp){
				         free(pDisp);
				         pDisp = NULL;
			      }
    	 }
    	 utPltPutVar(psDbHead,"mesg","数据加载成功");
	 	   utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_webuserman_dr_list.htm");
       return 0;     
    }
    
    if(!utStrIsSpaces(caDel)) { // 删除一个组  
		    if(strlen(caSesid)>0){  
		 	      if(strlen(caSesid) != 0){
					  		q =	utStrSkipSpaces(caSesid);
					  		while(q && *q){
					  				q = utStrGetWord(q,caTemp2,18,",");
						 				sprintf(caTempd,"update ncsrvusermac set flags = 9 where username in (select username from ncsrvuser where userid = '%s')", caTemp2);
						 				printf("caTempd = %s\n", caTempd);
								 		pasDbExecSqlF(caTempd);
								 		
								 		sprintf(caTempd,"select username, groupid from ncsrvuser where userid = '%s'", caTemp2);
					  				pasDbOneRecord(caTempd, 0, UT_TYPE_STRING, sizeof(uUsername)-1,   uUsername,
					  				                           UT_TYPE_ULONG,  sizeof(long),          &uGroupid);
					  				
					  				char *pUsername = NULL;
					  				unsigned long pGroupid = 0;
					  				pUsername = strdup(uUsername);
					  				pGroupid = uGroupid;
					  				printf("pUsername = %s, pGroupid = %d\n", pUsername, pGroupid);
					  				ncSrvDelUserMacByName(psShmHead,pGroupid,pUsername);
					  				if(pUsername){
								         free(pUsername);
								         pUsername = NULL;
							      }
								 		
								 		printf("caTemp2 = %s\n", caTemp2);
										if(strlen(caTemp2)>0){
										 		memset(caTempd, 0, sizeof(caTempd));
										 		sprintf(caTempd, "select username, dispname, groupname, ssid, password, usertype, flags, maxbindmac from ncsrvuser where userid = '%s'", caTemp2);
										 		printf("caTempd = %s\n", caTempd);
										 		pasDbOneRecord(caTempd, 0, UT_TYPE_STRING, sizeof(dUsername)-1,   dUsername,
										 		                           UT_TYPE_STRING, sizeof(dDispname)-1,   dDispname,
										 		                           UT_TYPE_STRING, sizeof(dGroupname)-1,  dGroupname,
										 		                           UT_TYPE_STRING, sizeof(dSsid)-1,       dSsid,
										 		                           UT_TYPE_STRING, sizeof(dPassword)-1,   dPassword,
										 		                           UT_TYPE_ULONG,  sizeof(long),          &dUsertype,
										 		                           UT_TYPE_ULONG,  sizeof(long),          &dFlags,
										 		                           UT_TYPE_ULONG,  sizeof(long),          &dMaxbindmac);
										 		
										 		char *pSsid = NULL;  //其它类似
									 		  char *pUsername = NULL;
									 		  char *pPassword = NULL;
									 		  char *pGroupname = NULL;
									 		  char *pDisp = NULL;
									      int  iStatus = 0;
									      unsigned long lusertype = dUsertype;
									      unsigned long lflags = 9;
									      unsigned long lMaxMac = 0;
									      if(dMaxbindmac>0){
									       		lMaxMac = dMaxbindmac;
									      }else{
									      	  char caMaxmac[11] = "";
									      	  strcpy(caMaxmac,utComGetVar_sd(psShmHead,"NcmMaxUMac","1"));
									      		lMaxMac = atol(caMaxmac);	
									      }
									      pSsid = strdup(dSsid);
									      pUsername = strdup(dUsername);
									      pPassword = strdup(dPassword);
									      pGroupname = strdup(dGroupname);
									      pDisp = strdup(dDispname);
									      
									      printf("pSsid = %s\n", pSsid);
									      printf("pUsername = %s\n", pUsername);
									      printf("pPassword = %s\n", pPassword);
									      printf("pGroupname = %s\n", pGroupname);
									      printf("pDisp = %s\n", pDisp);
									      printf("lusertype = %d\n", lusertype);
									      printf("lflags = %d\n", lflags);
									      printf("lMaxMac = %d\n", lMaxMac);
									      
									      ncPortalUser *result = NULL;
									      result = ncSrvSynUserInfo(psShmHead,pGroupname,pSsid,pUsername,pPassword,NULL,pDisp,lusertype,lMaxMac,9,&iStatus);
									      if(result){
									      		printf("status = %d\n", result->cStatus);
									      }
									      if(pSsid){
										         free(pSsid);
										         pSsid = NULL;
									      }
									      if(pUsername){
										         free(pUsername);
										         pUsername = NULL;
									      }
									      if(pPassword){
										         free(pPassword);
										         pPassword = NULL;
									      }
									      if(pGroupname){
										         free(pGroupname);
										         pGroupname = NULL;
									      }
									      if(pDisp){
										         free(pDisp);
										         pDisp = NULL;
									      }
			              }   
					  				q++;	
					  		}
					  }
					  //pasDbExecSqlF("delete from ncsrvuser where userid in(%s)",caSesid);
			  }
    }
    
    if(!utStrIsSpaces(caRemovemac)) { // 解除绑定  
		    if(strlen(caSesid)>0){  
		 	      if(strlen(caSesid) != 0){
					  		q =	utStrSkipSpaces(caSesid);
					  		while(q && *q){
					  				q = utStrGetWord(q,caTemp2,18,",");
					  				sprintf(caTempd,"update ncsrvusermac set flags = 9 where username in (select username from ncsrvuser where userid = '%s')", caTemp2);
						 				printf("caTempd = %s\n", caTempd);
								 		pasDbExecSqlF(caTempd);
					  			
					  				sprintf(caTempd,"select username, groupid from ncsrvuser where userid = '%s'", caTemp2);
					  				pasDbOneRecord(caTempd, 0, UT_TYPE_STRING, sizeof(uUsername)-1,   uUsername,
					  				                           UT_TYPE_ULONG,  sizeof(long),          &uGroupid);
					  				
					  				char *pUsername = NULL;
					  				unsigned long pGroupid = 0;
					  				pUsername = strdup(uUsername);
					  				pGroupid = uGroupid;
					  				printf("pUsername = %s, pGroupid = %d\n", pUsername, pGroupid);
					  				ncSrvDelUserMacByName(psShmHead,pGroupid,pUsername);
					  				if(pUsername){
								         free(pUsername);
								         pUsername = NULL;
							      }
						 				
					  				q++;	
					  		}
					  }
			  }
    }
    
    // 刷新
	  if(!utStrIsSpaces(caRefresh)) {
    		ncSrvSysReset(psShmHead);
	  }
    
    sprintf(caTemp,"select count(*) from ncsrvuser where 1 = 1 ");
    sprintf(caTemp+strlen(caTemp)," and ssid='%s'", cnfSsid);
  	if (strlen(mokeyword) > 0)
    {	
    	sprintf(caTemp+strlen(caTemp)," and (username like '%c%s%c' or dispname like '%c%s%c' or usermac like '%c%s%c' ) ",'%',mokeyword,'%','%',mokeyword,'%','%',mokeyword,'%');
  	}
  	if (strlen(groupid) > 0)
    {	
    	sprintf(caTemp+strlen(caTemp)," and groupid=%lu ", atol(groupid));
  	}
  	if (strlen(areaid) > 0)
    {
    	sprintf(caTemp+strlen(caTemp)," and left(groupname, 6) like '%lu%c' ", atol(areaid),'%');
  	}
  	if (strlen(groupcode) > 0)
    {
    	sprintf(caTemp+strlen(caTemp)," and substring(groupname, 17) like '%s%%' ", groupcode);
  	}
  	if (strlen(flags) > 0)
    {
    	sprintf(caTemp+strlen(caTemp)," and flags = '%s' ", flags);
  	}
  	if(strlen(mystype)>0){
	  		if(strlen(caOptd)>0){
	  			  sprintf(caTemp+strlen(caTemp)," and groupname like '%s%c' ", caOptd,'%');
	  		}else{
	  				sprintf(caTemp+strlen(caTemp)," and substring(groupname, 9, 2) = '%s' ", mystype);
	  		}
  	}
  	printf("caTemp = %s\n", caTemp);
		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lTotRec);
		utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
    pdate=(struct groupdate*)malloc(lTotRec*sizeof(struct groupdate)+1);
    
		iStart = atol(start);
	  iLimit = atol(limit);
	  
	  
	  char tttt[128] = "河南分公司,河南分公司本级,办公室\n";
	  // 导出
	  if(strlen(caExport)>0){
	  	 memset(caFilename,0,sizeof(caFilename));
	 		 sprintf(caFilename, "staffmanager%lu.csv",time(0));
	 		 strcpy(caPath, "/home/ncmysql/ncsrv/html/v8");
	 		 sprintf(caFile, "%s/%s", caPath, caFilename);
	 		 printf("caFile = %s\n", caFile);
	 		 fp = fopen(caFile, "w");
	 		 if(fp == NULL){
	 		 		 pasDbCloseCursor(psCur);
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"登录名,显示名,SSID,密码,Mac,允许Mac,第一级(根),第二级,第三级,第四级,第五级,第六级,第七级,第八级\n");
//			 ncUtlPrintCsv(fp,8,UT_TYPE_STRING,"用户名称",
//			                    UT_TYPE_STRING,"显示名称",
//			                    UT_TYPE_STRING,"单位名称",
//			                    UT_TYPE_STRING,"SSID",
//			                    UT_TYPE_STRING,"Portal名称",
//			                    UT_TYPE_STRING,"密码",
//			                    UT_TYPE_STRING,"有效时间",
//			                    UT_TYPE_STRING,"最后访问时间");
	  }
		
    sprintf(caTemp,"select userid,username,dispname,groupname,groupid,ssid,proname,openid,password,mark,addtime,moditime,lasttime,expiretime,usertype,flags,maxbindmac from ncsrvuser where 1 = 1 ");
    sprintf(caTemp+strlen(caTemp)," and ssid='%s'", cnfSsid);
    if (strlen(mokeyword) > 0)
    {	
    	sprintf(caTemp+strlen(caTemp)," and (username like '%c%s%c' or dispname like '%c%s%c' or usermac like '%c%s%c' ) ",'%',mokeyword,'%','%',mokeyword,'%','%',mokeyword,'%');
  	}
  	if (strlen(groupid) > 0)
    {	
    	sprintf(caTemp+strlen(caTemp)," and groupid=%lu ", atol(groupid));
  	}
  	if (strlen(areaid) > 0)
    {
    	sprintf(caTemp+strlen(caTemp)," and left(groupname, 6) like '%lu%c' ", atol(areaid),'%');
  	}
  	if (strlen(groupcode) > 0)
    {
    	sprintf(caTemp+strlen(caTemp)," and substring(groupname, 17) like '%s%%' ", groupcode);
  	}
  	if (strlen(flags) > 0)
    {
    	sprintf(caTemp+strlen(caTemp)," and flags = '%s' ", flags);
  	}
  	if(strlen(mystype)>0){
	  		if(strlen(caOptd)>0){
	  			  sprintf(caTemp+strlen(caTemp)," and groupname like '%s%c' ", caOptd,'%');
	  		}else{
	  				sprintf(caTemp+strlen(caTemp)," and substring(groupname, 9, 2) = '%s' ", mystype);
	  		}
  	}
  	if(strlen(caExport)==0)
  	sprintf(caTemp+strlen(caTemp)," order by lasttime desc limit %d , %d ",iStart, iLimit < lTotRec ? iLimit : lTotRec);
  	printf("caTemp=%s \n",caTemp);
  	psCur=pasDbOpenSql(caTemp,0);
    if(psCur==NULL){
    	    utPltFreeDb(psDbHead);
          utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","用户管理查询",ncLang("0174出错 %s"),pasDbErrorMsg(NULL));
          return 0;
    }
    
    iReturn=pasDbFetchInto(psCur,UT_TYPE_ULONG, sizeof(long),         &lUserid,
                                 UT_TYPE_STRING,sizeof(caUsername)-1, caUsername,
                                 UT_TYPE_STRING,sizeof(caDispname)-1, caDispname,
                                 UT_TYPE_STRING,sizeof(caGroupname)-1,caGroupname,
                                 UT_TYPE_LONG,  sizeof(long),         &lGroupid,
                                 UT_TYPE_STRING,sizeof(caSsid)-1,     caSsid,
                                 UT_TYPE_STRING,sizeof(caProname)-1,  caProname,
                                 UT_TYPE_STRING,sizeof(caOpenid)-1,   caOpenid,
                                 UT_TYPE_STRING,sizeof(caPassword)-1, caPassword,
                                 UT_TYPE_STRING,sizeof(caMark)-1,     caMark,
                                 UT_TYPE_ULONG, sizeof(long),         &lAddtime,
                                 UT_TYPE_ULONG, sizeof(long),         &lModitime,
                                 UT_TYPE_ULONG, sizeof(long),         &lLasttime,
                                 UT_TYPE_ULONG, sizeof(long),         &lExpiretime,
                                 UT_TYPE_ULONG, sizeof(long),         &lUsertype,
                                 UT_TYPE_ULONG, sizeof(long),         &lFlags,
                                 UT_TYPE_ULONG, sizeof(long),         &lMaxbindmac);
      				  
    iNum=0;
    while(iReturn==0||iReturn==1405){
	    	strcpy(pdate[iNum].user,caUsername);
	    	strcpy(pdate[iNum].dispname,caDispname);
	    	strcpy(pdate[iNum].ssid,caSsid);
	    	strcpy(pdate[iNum].password,caPassword);
	    	pdate[iNum].maxMac = lMaxbindmac;
	    	strcpy(pdate[iNum].groupname,caGroupname);
    	  
      	if(iNum>0){
        	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
        }
      	iNum++;
      	utPltPutLoopVarF(psDbHead,"userid",iNum,"%lu",lUserid);
	      utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu",iNum);
	      utPltSetCvtHtml(1);    
      	utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
      	utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
      	//utPltPutLoopVar(psDbHead,"groupname",iNum,caGroupname);
        utPltSetCvtHtml(0);   	
      	utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",lGroupid);
      	utPltPutLoopVar(psDbHead,"ssid",iNum,caSsid);
      	utPltPutLoopVar(psDbHead,"proname",iNum,caProname);
      	utPltPutLoopVar(psDbHead,"openid",iNum,caOpenid);
      	utPltPutLoopVar(psDbHead,"password",iNum,caPassword);
      	utPltPutLoopVar(psDbHead,"mark",iNum,caMark);
      	if(lAddtime>0){
      	   utPltPutLoopVar(psDbHead,"addtime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",lAddtime));
        }
      	if(lModitime>0){
      	  utPltPutLoopVar(psDbHead,"moditime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",lModitime));
        }
      	if(lLasttime>0){
      	  utPltPutLoopVar(psDbHead,"lasttime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",lLasttime));
      	  strcpy(s_lasttime, utTimFormat("%Y-%m-%d %H:%M:%S",lLasttime));
        }
      	if(lExpiretime>0){
      	   utPltPutLoopVar(psDbHead,"expiretime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",lExpiretime));
      	   strcpy(s_expiretime, utTimFormat("%Y-%m-%d %H:%M:%S",lExpiretime));
        }
      	switch(lUsertype){
      			case 0:strcpy(s_usertype, "固定用户");break;
      			case 1:strcpy(s_usertype, "短消息");break;	
      			case 2:strcpy(s_usertype, "QQ");break;
      			case 3:strcpy(s_usertype, "微信");break;
      			default:break;
      	}
      	utPltPutLoopVar(psDbHead,"usertype",iNum,s_usertype);
      	switch(lFlags){
      			case 0:strcpy(s_flags, "在职");break;
      			case 2:strcpy(s_flags, "待处理");break;
      			case 9:strcpy(s_flags, "离司");break;
      			default:break;	
      	}
        utPltPutLoopVar(psDbHead,"flags",iNum,s_flags);
        utPltPutLoopVarF(psDbHead,"maxbindmac",iNum,"%lu",lMaxbindmac);
//        if(strlen(caExport)>0){
//				 		ncUtlPrintCsv(fp,8,UT_TYPE_STRING,caUsername,
//													 		  UT_TYPE_STRING,caDispname,
//													 		  UT_TYPE_STRING,caGroupname,
//													 		  UT_TYPE_STRING,caSsid,
//													 		  UT_TYPE_STRING,caProname,
//													 		  UT_TYPE_STRING,caPassword,
//		                            UT_TYPE_STRING,s_expiretime,
//		                            UT_TYPE_STRING,s_lasttime);
//			  }
        
	      lUserid=0;
		    memset(caUsername,0,sizeof(caUsername));
		    memset(caDispname,0,sizeof(caDispname));
		    memset(caGroupname,0,sizeof(caGroupname));
		    lGroupid=0;
		    memset(caSsid,0,sizeof(caSsid));
		    memset(caProname,0,sizeof(caProname));
		    memset(caOpenid,0,sizeof(caOpenid));
		    memset(caPassword,0,sizeof(caPassword));
		    memset(caMark,0,sizeof(caMark));
		    lAddtime=0;
		    lModitime=0;
		    lLasttime=0;
		    lExpiretime=0;
		    lUsertype=0;
		    lFlags=0;
		    lMaxbindmac=0;
		    memset(s_usertype,0,sizeof(s_usertype));
		    memset(s_flags,0,sizeof(s_flags));
		    memset(s_lasttime,0,sizeof(s_lasttime));
		    memset(s_expiretime,0,sizeof(s_expiretime));
	      iReturn=pasDbFetchInto(psCur,UT_TYPE_ULONG, sizeof(long),         &lUserid,
		                                 UT_TYPE_STRING,sizeof(caUsername)-1, caUsername,
		                                 UT_TYPE_STRING,sizeof(caDispname)-1, caDispname,
		                                 UT_TYPE_STRING,sizeof(caGroupname)-1,caGroupname,
		                                 UT_TYPE_LONG,  sizeof(long),         &lGroupid,
		                                 UT_TYPE_STRING,sizeof(caSsid)-1,     caSsid,
		                                 UT_TYPE_STRING,sizeof(caProname)-1,  caProname,
		                                 UT_TYPE_STRING,sizeof(caOpenid)-1,   caOpenid,
		                                 UT_TYPE_STRING,sizeof(caPassword)-1, caPassword,
		                                 UT_TYPE_STRING,sizeof(caMark)-1,     caMark,
		                                 UT_TYPE_ULONG, sizeof(long),         &lAddtime,
		                                 UT_TYPE_ULONG, sizeof(long),         &lModitime,
		                                 UT_TYPE_ULONG, sizeof(long),         &lLasttime,
		                                 UT_TYPE_ULONG, sizeof(long),         &lExpiretime,
		                                 UT_TYPE_ULONG, sizeof(long),         &lUsertype,
		                                 UT_TYPE_ULONG, sizeof(long),         &lFlags,
                                     UT_TYPE_ULONG, sizeof(long),         &lMaxbindmac);
  	}
    pasDbCloseCursor(psCur);
    
    for(i=0;i<iNum;i++){
	    	sprintf(caTemp,"select usermac from ncsrvusermac where username='%s' and flags != 9 ",pdate[i].user);
	    	psCur=pasDbOpenSql(caTemp,0);
	    	if(psCur){
		    		long mac_num=0;
		    		memset(caMac,0,sizeof(caMac));
		    		iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(caMac)-1, caMac);
		    		memset(caMac_all,0,sizeof(caMac_all));
		    		memset(caMac_allexp,0,sizeof(caMac_allexp));
		    		while(iReturn==0||iReturn==1405){
			    			 mac_num++;
			    			 if(mac_num>atol(cnfMac)) break;
			    			 sprintf(caMac_all+strlen(caMac_all),"%s,",caMac);
			    			 sprintf(caMac_allexp+strlen(caMac_allexp),"%s|",caMac);
			    			 memset(caMac,0,sizeof(caMac));
			    		   iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(caMac)-1, caMac);
		    		}
		    		pasDbCloseCursor(psCur);
		        utPltPutLoopVarF(psDbHead,"macnum",i+1,"%lu",mac_num);
		    		utPltPutLoopVar(psDbHead,"usermac",i+1,caMac_all);
	    	}
	    	
	    	char tempcode[32] = "";
	    	char tempname[32] = "";
	    	if(strlen(pdate[i].groupname)>16){
	    			//printf("pdate[i].groupname = %s\n",pdate[i].groupname);
			    	ncStrToStr(pdate[i].groupname, tempcode);
			    	//printf("tempcode = %s\n",tempcode);
			    	sprintf(caTemp, "select groupname from ncmautopri where groupcode = '%s' and shopcode = '%s' ", tempcode,mystype);
			    	//printf("caTemp = %s\n", caTemp);
						pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(tempname)-1, tempname);
						if(strlen(tempname)==0){
							 sprintf(caTemp, "select dispname from ncsrvgroup where groupname = '%s' ", pdate[i].groupname);
			     	//printf("caTemp = %s\n", caTemp);
						  pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(tempname)-1, tempname);
						}
						utPltPutLoopVar(psDbHead,"groupname",i+1,tempname);
	    	}else{
	    		  sprintf(caTemp, "select dispname from ncsrvgroup where groupname = '%s' ", pdate[i].groupname);
			    	//printf("caTemp = %s\n", caTemp);
						pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(tempname)-1, tempname);
						utPltPutLoopVar(psDbHead,"groupname",i+1,tempname);
	    	}
	    	
	    	if(strlen(caExport)>0){
	    		char autotmp[512] = "";
	    		strcpy(autotmp,(char *)ncmGetAutoTreeByGroupId(psShmHead,pdate[i].groupname));
	    		fprintf(fp,"%s,%s,%s,%s,%s,%d,%s",pdate[i].user,pdate[i].dispname,pdate[i].ssid,pdate[i].password,caMac_allexp,pdate[i].maxMac,autotmp);
	    	}
    }
    
    if(strlen(caExport)>0){
	 	    fclose(fp);
	      utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
	      remove(caFile);
	      utPltFreeDb(psDbHead);
	      return 0;
	  }
	  
    //utPltShowDb(psDbHead); 
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_ext_webshowuser_man.htm");
      
    return 0;
}

int ncm_webuserman_info(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//#ifdef LDEBUG
   utMsgPrintMsg(psMsgHead); // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
//#endif  
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 char caTemp[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 long iReturn = 0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 char caKey[32] = "";
	 char caUserid[32] = "";
	 char caGroupid[32] = "";
	 char caAreaid[32] = "";
	 char caGroupcode[32] = "";
	 
	 char caUsername[32] = "";
	 char caDispname[32] = "";
	 char caGroupname[32] = "";
	 unsigned long lGroupid = 0;
	 char caSsid[32] = "";
	 char caProname[32] = "";
	 char caOpenid[32] = "";
	 char caUsermac[96] = "";
	 char caPassword[32] = "";
	 char caMark[128] = "";
	 unsigned long lAddtime = 0;
	 unsigned long lLasttime = 0;
	 unsigned long lExpiretime = 0;
	 unsigned long lUsertype = 0;
	 unsigned long lFlags = 0;
	 unsigned long lMaxbindmac = 0;
	 char caMac[20] = "";
	 char caMac_all[256]="";
	 
	 char groupcode[32] = "";
	 char groupcodename[32] = "";
	 
	 char caStyped[64] = "";
	 char caOptd[64] = "";
	 unsigned long lCid = 0;
	 dsCltGetSessionValue(1,"cid",UT_TYPE_ULONG,sizeof(long),&lCid);
	 dsCltGetSessionValue(1,"stype",UT_TYPE_STRING,sizeof(caStyped)-1,caStyped);
   dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(caOptd)-1,caOptd);
   printf("caStyped = %s\n", caStyped);
   printf("caOptd = %s\n", caOptd);
   printf("lCid = %d\n", lCid);
   
   char cnfSsid[32] = "";
   strcpy(cnfSsid, ncmGetSsidByStype(psShmHead, caStyped, 1));
   printf("cnfSsid = %s\n", cnfSsid);
   
   // 从配置文件中取最大mac值
   char cnfMac[32] = "";
   strcpy(cnfMac,utComGetVar_sd(psShmHead,"MaxBDMac",  "20"));
   printf("cnfMac = %s\n", cnfMac);
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 4, "key",       UT_TYPE_STRING,     sizeof(caKey)-1,         caKey,
	                                "groupcode", UT_TYPE_STRING,     sizeof(caGroupcode)-1,   caGroupcode,
	                                "groupid",   UT_TYPE_STRING,     sizeof(caGroupid)-1,     caGroupid,
	                                "userid",    UT_TYPE_STRING,     sizeof(caUserid)-1,      caUserid);
	 
	 if(strcmp(caKey, "update")==0&&strlen(caUserid)>0){
			 sprintf(sql, "select username, dispname, groupname, groupid, ssid, proname, openid, password, mark, addtime, lasttime, expiretime, usertype, flags, maxbindmac from ncsrvuser where userid=%s", caUserid);
			 printf("sql = %s\n", sql);
			 pasDbOneRecord(sql,0,UT_TYPE_STRING,sizeof(caUsername)-1,     caUsername,
			 											UT_TYPE_STRING,sizeof(caDispname)-1,     caDispname,
			 											UT_TYPE_STRING,sizeof(caGroupname)-1,    caGroupname,
			 											UT_TYPE_ULONG, sizeof(long),             &lGroupid,
			 											UT_TYPE_STRING,sizeof(caSsid)-1,         caSsid,
			 											UT_TYPE_STRING,sizeof(caProname)-1,      caProname,
			 											UT_TYPE_STRING,sizeof(caOpenid)-1,       caOpenid,
			 											UT_TYPE_STRING,sizeof(caPassword)-1,     caPassword,
			 											UT_TYPE_STRING,sizeof(caMark)-1,         caMark,
			 											UT_TYPE_ULONG, sizeof(long),             &lAddtime,
			 											UT_TYPE_ULONG, sizeof(long),             &lLasttime,
			 											UT_TYPE_ULONG, sizeof(long),             &lExpiretime,
			 											UT_TYPE_ULONG, sizeof(long),             &lUsertype,
			 											UT_TYPE_ULONG, sizeof(long),             &lFlags,
			 											UT_TYPE_ULONG, sizeof(long),             &lMaxbindmac);
		                        
		    utPltSetCvtHtml(1);
			  utPltPutVar(psDbHead,"userid",caUserid);
				utPltPutVar(psDbHead,"usrname",caUsername);
				utPltPutVar(psDbHead,"dispname",caDispname);
				//utPltPutVar(psDbHead,"groupname",caGroupname);
				utPltPutVarF(psDbHead,"groupid","%lu",lGroupid);
				utPltPutVar(psDbHead,"ssid",caSsid);
				utPltPutVar(psDbHead,"proname",caProname);
				utPltPutVar(psDbHead,"openid",caOpenid);
				utPltPutVar(psDbHead,"password",caPassword);
				utPltPutVar(psDbHead,"mark",caMark);
				utPltSetCvtHtml(0);
				utPltPutVar(psDbHead,"addtime",utTimFormat("%Y/%m/%d %H:%M:%S", lAddtime));
				utPltPutVar(psDbHead,"lasttime",utTimFormat("%Y/%m/%d %H:%M:%S", lLasttime));
				utPltPutVar(psDbHead,"sexpdate",utTimFormat("%Y/%m/%d", lExpiretime));
				utPltPutVar(psDbHead,"sexptime",utTimFormat("%H:%M:%S", lExpiretime));
				
				lUsertype = 0;   //固定用户  员工类别
				utPltPutVarF(psDbHead,"usertype","%lu",lUsertype);
				if(lFlags != 9) lFlags = 0;
				utPltPutVarF(psDbHead,"flags","%lu",lFlags);
				utPltPutVarF(psDbHead,"maxbindmac","%lu",lMaxbindmac);
/*				
				if(strlen(caGroupname)>16){
						ncStrToStr(caGroupname, groupcode);
						utPltPutVar(psDbHead,"groupcode",groupcode);
						printf("groupcode = %s\n", groupcode);
						sprintf(caTemp, "select groupname from ncmautopri where groupcode = '%s' and shopcode = '%s' ", groupcode,caStyped);
						printf("caTemp = %s\n", caTemp);
						pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(groupcodename)-1, groupcodename);
						utPltPutVar(psDbHead,"groupcodename",groupcodename);
						printf("groupcodename = %s\n", groupcodename);
			  }else{
*/
			  //		utPltPutVarF(psDbHead,"groupcode","%lu",lGroupid);
			  		sprintf(caTemp, "select groupname from ncsrvgroup where groupid = '%lu'", lGroupid);
						printf("caTemp = %s\n", caTemp);
						pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(groupcodename)-1, groupcodename);
						utPltPutVar(psDbHead,"groupcodename",groupcodename);
						utPltPutVar(psDbHead,"groupcode",groupcodename);
						printf("groupcodename = %s\n", groupcodename);
	//		  }
			  
				sprintf(caTemp,"select usermac from ncsrvusermac where username='%s' and flags!=9 ",caUsername);
    	  psCur=pasDbOpenSql(caTemp,0);
    	  if(psCur){
		    		long mac_num=0;
		    		memset(caMac,0,sizeof(caMac));
		    		iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,19, caMac);
		    		memset(caMac_all,0,sizeof(caMac_all));
		    		while(iReturn==0||iReturn==1405){
		    			 mac_num++;
		    			 if(mac_num>atol(cnfMac)) break;
		    			 sprintf(caMac_all+strlen(caMac_all),"%s|",caMac);
		    			 memset(caMac,0,sizeof(caMac));
		    		   iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,19, caMac);
		    		}
		    		pasDbCloseCursor(psCur);
    	  }
    	  utPltPutVar(psDbHead,"usermac",caMac_all);
	 }else{
	 			utPltPutVarF(psDbHead,"usertype","%lu",0);
	 			utPltPutVarF(psDbHead,"maxbindmac","%lu",0);
			  if(lFlags != 9) lFlags = 0;
			  utPltPutVarF(psDbHead,"flags","%lu",lFlags);
			  if(strlen(caGroupcode)>0 || strlen(caGroupid)>0){ // 点击左边树
			  	  if(strlen(caGroupcode)>0){ // 是自定义
					  		utPltPutVar(psDbHead,"groupcode",caGroupcode);
							  sprintf(caTemp, "select groupname from ncmautopri where groupcode = '%s' and shopcode = '%s' ", caGroupcode,caStyped);
								printf("caTemp = %s\n", caTemp);
								pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(groupcodename)-1, groupcodename);
								utPltPutVar(psDbHead,"groupcodename",groupcodename);
								printf("groupcodename = %s\n", groupcodename);
					  }else{ // 不是自定义
					  		utPltPutVar(psDbHead,"groupcode",caGroupid);
					  		sprintf(caTemp, "select dispname from ncsrvgroup where groupid = '%s'", caGroupid);
					  		printf("caTemp = %s\n", caTemp);
								pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(groupcodename)-1, groupcodename);
								utPltPutVar(psDbHead,"groupcodename",groupcodename);
								printf("groupcodename = %s\n", groupcodename);	
					  }
			  }else{ // 不点击左边树，默认值
			  		unsigned long lAutodef = 0;
			  		sprintf(caTemp, "select autodef from ncmshopgtype where groupcode = '%s'", caStyped);
			  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lAutodef);	
			  		if(lAutodef > 0){
			  			  if(strlen(caOptd)>0){ // 自定义二级以后
					  			  char tempcode[32] = "";
					  			  char caTempname[32] = "";
					  			  ncStrToStr(caOptd, tempcode);
					  			  utPltPutVar(psDbHead,"groupcode",tempcode);
					  			  sprintf(caTemp, "select groupname from ncmautopri where groupcode = '%s' and shopcode = '%s' ", tempcode,caStyped);
					  		    pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(caTempname)-1,caTempname);	
					  				utPltPutVar(psDbHead,"groupcodename",caTempname);
			  			  }
			  		}else{ // 不是自定义
			  			  char caTempname[32] = "";
			  			  if(strlen(caGroupid)>0){
			  			  		sprintf(caTemp, "select groupname from ncsrvgroup where groupid = '%s'", caGroupid);
					  		    pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(caTempname)-1,caTempname);	
					  				utPltPutVar(psDbHead,"groupcodename",caTempname);
							  		utPltPutVar(psDbHead,"groupcode",caGroupid);	
			  			  }else{
			  			  		sprintf(caTemp, "select groupname from ncsrvgroup where groupid = '%s'", lCid);
					  		    pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(caTempname)-1,caTempname);	
					  				utPltPutVar(psDbHead,"groupcodename",caTempname);
							  		utPltPutVarF(psDbHead,"groupcode","%lu",lCid);	
			  			  }
			  		}
			  }
			  utPltPutVar(psDbHead,"ssid",cnfSsid);
	 }
	 utPltPutVar(psDbHead,"key",caKey);
	 
	 //utPltShowDb(psDbHead);
	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_webuserman_dr.htm");
	 
	 return 0;		
}

int ncm_webuserman_dr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  
	  char caFile2[128] = "";
	  int result = 0;
	  char msg[128] = "";
	  char imp_file[256]="";
    char temp_file[129]="";
    char temp0_file[129]="";
    char temp1_file[129]="";
    char temp2_file[129]="";
    char temp3_file[129]="";
	  char *p=NULL;
    
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif

		psDbHead = utPltInitDb();
    if(!utFileIsExist(UPLOADFILEPATH)) {
        if(mkdir(UPLOADFILEPATH,555)!=0){
        		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncLang("0194导入"),ncLang("0183打开文件出错"));
        		return 0;
        }
    }
    sprintf(temp0_file, "chmod -Rf 777 %s", UPLOADFILEPATH);
    system(temp0_file);
    
    // 建立文件存放路径
  	if(!utFileIsExist(FILEPATH)) {
        if(mkdir(FILEPATH,555)!=0){
        	  utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncLang("0194导入"),ncLang("0183打开文件出错"));
        		return 0;
        }
    }
    sprintf(temp1_file, "chmod -Rf 777 %s", FILEPATH);
    system(temp1_file);
    
	  utMsgGetSomeNVar(psMsgHead,1,"fname", UT_TYPE_STRING, 255, imp_file);
	  
	  if(strlen(imp_file)!=0){
		  	p=utStrSkipSpaces(imp_file);
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
		  	printf("temp_file = %s\n", temp_file);
		  	strcpy(temp3_file, temp_file);
				if(strlen(temp_file)>0){
						sprintf(temp2_file, "mv %s/%s %s/%s", UPLOADFILEPATH, temp_file, FILEPATH, temp_file);
						printf("temp2_file = %s\n", temp2_file);
						sprintf(caFile2, "%s/%s", FILEPATH, temp_file);
				  	system(temp2_file);
				}
		}
		result = ncm_tbuserimpS1(psShmHead,caFile2);
		if(result == 1){
			  strcpy(msg, "上传成功！");
				printf("上传成功！");	
		}else if(result == 2){
			  strcpy(msg, "上传失败！用户名不能为空！");
				printf("上传失败！用户名不能为空！");	
		}else if(result == 3){
			  strcpy(msg, "上传失败！SSID不能为空！");
				printf("上传失败！SSID不能为空！");
		}else{
			  strcpy(msg, "上传失败！");
				printf("上传失败！");		
		}
		utPltPutVar(psDbHead,"msg",msg);
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncm_webusermanager_close.htm");
    return 0;
}

int ncm_webuserman_dload(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  
	  char caFilename[256]="";
    
//#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
//#endif

		psDbHead = utPltInitDb();
    
    strcpy(caFilename, "test.csv");
    utPltFileDownload(iFd,"application/text",FILEPATH,caFilename,caFilename);
    
    utPltFreeDb(psDbHead);
    return 0;
}

//临时用户管理
int ncmWebShowStaffMan(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  // 初始化数据库
	  char caTemp[4096] = "";
	  char caTemp1[4096] = "";
	  char caTemp2[18] = "";
	  char caTempd[4096] = "";
	  char *q=NULL;
    int iNum=0;
    char caShopids[1024];
    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();
	 	
	 	char caKey[32] = "";
	 	char userid[32] = "";
	 	char username[32] = "";
	 	char dispname[32] = "";
	 	char groupname[32] = "";
	 	char groupid[32] = "";
	 	char ssid[32] = "";
	 	char proname[32] = "";
	 	char openid[32] = "";
	 	char usermac[256] = "";
	 	char password[32] = "";
	 	char mark[128] = "";
	 	char addtime[32] = "";
	 	char lasttime[32] = "";
	 	char sexpdate[32] = "";
	 	char sexptime[32] = "";
	 	char expiretime[32] = "";
	 	char usertype[32] = "";
	 	char flags[32] = "";
	 	char maxbindmac[32] = "";
	 	char areaid[32] = "";
	 	unsigned long l_expiretime=0;
     
    unsigned long lUserid=0;
    char caUsername[32]="";
    char caDispname[32]="";
    char caGroupname[32]="";
    unsigned long lGroupid=0;
    char caSsid[32]="";
    char caProname[32]="";
    char caOpenid[32]="";
    char caPassword[32]="";
    char caMark[128]="";
    unsigned long lAddtime=0;
    unsigned long lModitime=0;
    unsigned long lLasttime=0;
    unsigned long lExpiretime=0;
    unsigned long lUsertype=0;
    unsigned long lFlags=0;
    unsigned long lMaxbindmac=0;
    char s_usertype[32]="";
    char s_flags[32]="";
    char s_lasttime[32]="";
    char s_expiretime[32]="";
    
    char caUpdate[16] = "";
	  char caSesid[1024] = "";
	  char caDel[16] = "";
	  char caRemovemac[16] = "";
	  char caRefresh[16] = "";
    char mokeyword[64] = "";
    unsigned long lSid=0;
    char caTemp_d[128] = "";
    char dev[32]="";
    
    struct groupdate{
    		char user[32];
    		char groupname[32];	
    };
    struct groupdate *pdate;
    
    // 解除绑定
    char uUsername[32] = "";
    unsigned long uGroupid = 0;
    
    // 数据表
	  unsigned long lCount1 = 0; // 表中记录的个数
	  unsigned long lCount = 0; // 表中记录的个数
	  long iReturn; // 单记录查询
	  char start[17] = ""; // 默认记录从0开始
	  char limit[17] = ""; // 限定一页中的记录是40条
	  unsigned long iStart = 0; // 转换后的默认开始atoi(start)
	  unsigned long iLimit = 0; // 转换后的限定记录条数atoi(limit)
	 
	  // 翻页
	  unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	  // 导出
	  char caExport[32] = "";
	  FILE *fp = NULL;
	  char caFilename[32] = "";
	  char caFile[128] = "";
	  char caPath[128] = "";
	 
	  // 排序
	  char sort[17] = "";
    char dir[9] = "";
    char caOptd[32] = "";
    char caStyped[32] = "";
    char users[100][20];
    long lLen = 0;
    long i = 0;
    char caMac[20] = "";
    char caMac_all[256] = "";
    char groupcode[32]="";
    char caMove[32],caMgroup[128];
    ncPortalUser *psUser;
    ncPortalGroup *psGroup;
    strcpy(caShopids,getDsShopids());
	  dsCltGetSessionValue(1,"opt",UT_TYPE_STRING,sizeof(caOptd)-1,caOptd);
	  dsCltGetSessionValue(1,"stype",UT_TYPE_STRING,sizeof(caStyped)-1,caStyped);
	  printf("caOptd = %s\n", caOptd);
	  printf("caStyped = %s\n", caStyped);
    
    char cnfSsid[32] = "";
    strcpy(cnfSsid, ncmGetSsidByStype(psShmHead, caStyped, 1));
    
    char cnfMac[32] = "";
    strcpy(cnfMac,utComGetVar_sd(psShmHead,"MaxBDMac",  "20"));
    printf("cnfMac = %s\n", cnfMac);
    
    utMsgPrintMsg(psMsgHead);
    
	  iReturn = utMsgGetSomeNVar(psMsgHead,33,"key",             UT_TYPE_STRING, sizeof(caKey)-1,      caKey,
	                                          "userid",          UT_TYPE_STRING, sizeof(userid)-1,     userid,
														                "usrname",         UT_TYPE_STRING, sizeof(username)-1,    username,
														                "groupname",       UT_TYPE_STRING, sizeof(groupname)-1,    groupname,
														                "dispname",        UT_TYPE_STRING, sizeof(dispname)-1,   dispname,
														                "groupid",         UT_TYPE_STRING, sizeof(groupid)-1,    groupid,
														                "ssid",            UT_TYPE_STRING, sizeof(ssid)-1,       ssid,
														                "proname",         UT_TYPE_STRING, sizeof(proname)-1,    proname,
														                "openid",          UT_TYPE_STRING, sizeof(openid)-1,     openid,
														                "usermac",         UT_TYPE_STRING, sizeof(usermac)-1,    usermac,    
														                "password",        UT_TYPE_STRING, sizeof(password)-1,   password,      
														                "mark",            UT_TYPE_STRING, sizeof(mark)-1,       mark,  
														                "addtime",         UT_TYPE_STRING, sizeof(addtime)-1,    addtime, 
														                "lasttime",        UT_TYPE_STRING, sizeof(lasttime)-1,   lasttime,
														                "sexpdate",        UT_TYPE_STRING, sizeof(sexpdate)-1,   sexpdate, 
														                "sexptime",        UT_TYPE_STRING, sizeof(sexptime)-1,   sexptime,          
														                "usertype",        UT_TYPE_STRING, sizeof(usertype)-1,   usertype, 
														                "flags",           UT_TYPE_STRING, sizeof(flags)-1,      flags,
														                "maxbindmac",      UT_TYPE_STRING, sizeof(maxbindmac)-1, maxbindmac,
														                "areaid",          UT_TYPE_STRING, sizeof(areaid)-1,     areaid,
														                "mokeyword",       UT_TYPE_STRING, sizeof(mokeyword)-1,  mokeyword,
														                "refresh",	       UT_TYPE_STRING, sizeof(caRefresh)-1,  caRefresh,
														                "removemac",       UT_TYPE_STRING, sizeof(caRemovemac)-1,caRemovemac,
														                "groupcode",       UT_TYPE_STRING, sizeof(groupcode)-1,  groupcode,
														                "del",             UT_TYPE_STRING, sizeof(caDel)-1,      caDel,
														                "selsid",          UT_TYPE_STRING, sizeof(caSesid)-1,    caSesid,
														                "start",           UT_TYPE_STRING, sizeof(start)-1,      start,
														   	            "limit",           UT_TYPE_STRING, sizeof(limit)-1,      limit,
														   	            "sort",            UT_TYPE_STRING, sizeof(sort)-1,       sort,
																						"dir",             UT_TYPE_STRING, sizeof(dir)-1,        dir,
																						"caExport",        UT_TYPE_STRING, sizeof(caExport)-1,   caExport,
																						"move",            UT_TYPE_STRING, sizeof(caMove)-1,caMove,
																						"mgroup",          UT_TYPE_STRING,sizeof(caMgroup)-1,caMgroup);
	  
	  trim(mokeyword);
	  
	  //未操作选择框
	  if(strlen(mokeyword)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,mokeyword,caTemp,sizeof(mokeyword)-1));
    	strcpy(mokeyword,caTemp_d);
    }
	  if(strlen(username)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,username,caTemp,sizeof(username)-1));
    	strcpy(username,caTemp_d);
    }
    if(strlen(groupname)>0){
    		strcpy(caTemp_d,pasCvtGBK(2,groupname,caTemp,sizeof(groupname)-1));	
    		strcpy(groupname,caTemp_d);
    	
    }
    if(strlen(dispname)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,dispname,caTemp,sizeof(dispname)-1));
    	strcpy(dispname,caTemp_d);
    }
    if(strlen(proname)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,proname,caTemp,sizeof(proname)-1 ));
    	strcpy(proname,caTemp_d);
    }
    if(strlen(usermac)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,usermac,caTemp,sizeof(usermac)-1));
    	strcpy(usermac,caTemp_d);
    }
    if(strlen(ssid)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,ssid,caTemp,sizeof(ssid)-1));
    	strcpy(ssid,caTemp_d);
    }
    if(strlen(password)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,password,caTemp,sizeof(password)-1));
    	strcpy(password,caTemp_d);
    }
    if(strlen(mark)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,mark,caTemp,sizeof(mark)-1));
    	strcpy(mark,caTemp_d);
    }
	  if(strlen(caSesid)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,caSesid,caTemp,sizeof(caSesid)-1));
    	strcpy(caSesid,caTemp_d);
    }
    if(strlen(caMgroup)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,caMgroup,caTemp,sizeof(caSesid)-1));
    	strcpy(caMgroup,caTemp_d);
    }
    if(!utStrIsSpaces(caRemovemac)) { // 解除绑定  
		    if(strlen(caSesid)>0){  
		 	      if(strlen(caSesid) != 0){
					  		q =	utStrSkipSpaces(caSesid);
					  		while(q && *q){
					  				q = utStrGetWord(q,caTemp2,18,",");
								 		sprintf(caTempd,"update ncsrvusermac set flags = 9 where username in (select username from ncsrvuser where userid = '%s')", caTemp2);
						 				printf("caTempd = %s\n", caTempd);
								 		pasDbExecSqlF(caTempd);
					  				
					  				sprintf(caTempd,"select username, groupid from ncsrvuser where userid = '%s'", caTemp2);
					  				pasDbOneRecord(caTempd, 0, UT_TYPE_STRING, sizeof(uUsername)-1,   uUsername,
					  				                           UT_TYPE_ULONG,  sizeof(long),          &uGroupid);
					  				
					  				char *pUsername = NULL;
					  				unsigned long pGroupid = 0;
					  				pUsername = strdup(uUsername);
					  				pGroupid = uGroupid;
					  				printf("pUsername = %s, pGroupid = %d\n", pUsername, pGroupid);
					  				ncSrvDelUserMacByName(psShmHead,pGroupid,pUsername);
					  				if(pUsername){
								         free(pUsername);
								         pUsername = NULL;
							      }
							      
					  				q++;	
					  		}
					  }
			  }
    }
    
        if(strcmp(caMove,"move")==0) { // 移动部门 
        	sprintf(caTemp,"select groupname,groupid from ncsrvgroup where dispname='%s' ",caMgroup);
     //   	printf("caTemp=%s\n",caTemp);
        	memset(caGroupname,0,sizeof(caGroupname));
        	lGroupid=0;
        	pasDbOneRecord(caTemp,0,UT_TYPE_STRING,31,caGroupname,
        	                        UT_TYPE_LONG,4,&lGroupid);
		    if(strlen(caSesid)>0&&strlen(caGroupname)>0){  
		 	      if(strlen(caSesid) != 0){
					  		q =	utStrSkipSpaces(caSesid);
					  		while(q && *q){
					  				q = utStrGetWord(q,caTemp2,18,",");
								 		sprintf(caTempd,"update ncsrvuser set groupid=%d,groupname='%s' where userid=%s ",lGroupid,caGroupname,caTemp2);
						 				printf("caTempd = %s\n", caTempd);
								 		pasDbExecSqlF(caTempd);
	      	  			  psGroup = (ncPortalGroup *)ncSrvGetGroupInfoByName(psShmHead,caGroupname);
				//	  				 if(psGroup){
					  		//		 	printf("groupname=%s\n",caGroupname);
				//	  				}
					  				sscanf(caTemp2,"%u",&lUserid);
					  		//		printf("lUserid=%d\n",lUserid);
			               psUser = ncSrvGetUserById(psShmHead,lUserid);
			             
			               if(psUser&&psGroup){
			               	printf("lGroupid=%d,grouname=%s\n",lGroupid,caGroupname);
			               	psUser->lGroupid = lGroupid;
			               	strcpy(psUser->caGroup,caGroupname);
                      ncSrvSynUser2Mem(psShmHead,psUser);

                      
			              }
							      
					  				q++;	
					  		}
					  }
			  }
    }
    
    
    
    
    // 刷新
	  if(!utStrIsSpaces(caRefresh)) {
    		ncSrvSysReset(psShmHead);
	  }
      
    sprintf(caTemp,"select count(*) from ncsrvuser as a where  a.username is not null and a.username!=''");
    if(strlen(groupname)>0)
    	{
    		if(strcmp(groupname,"ALL")!=0)
    		{
    			 sprintf(caTemp,"select count(*) from ncsrvuser as a,ncsrvgroup as b where a.groupname = b.groupname and  b.dispname ='%s' and a.username is not null and a.username!=''",groupname);
    				
    		}	
    		if(strcmp(groupname,"未划分单位")==0)
    		{
    				 sprintf(caTemp,"select count(*) from ncsrvuser as a  where  a.groupname='' and a.username is not null and a.username!=''");
    				
    		}		
    	}
    	
    if(strlen(caShopids)>0){
    	sprintf(caTemp+strlen(caTemp)," and a.groupid in (%s) ",caShopids);
    }
  	sprintf(caTemp+strlen(caTemp)," and a.ssid != '%s'", cnfSsid);
  	if (strlen(mokeyword) > 0)
    {	
    	sprintf(caTemp+strlen(caTemp)," and (a.username like '%c%s%c' or a.dispname like '%c%s%c' or a.usermac like '%c%s%c' ) ",'%',mokeyword,'%','%',mokeyword,'%','%',mokeyword,'%');
  	}
  	if (strlen(groupid) > 0)
    {	
    	sprintf(caTemp+strlen(caTemp)," and a.groupid=%lu ", atol(groupid));
  	}
  	if(strlen(caStyped)>0){
	  		if(strlen(caOptd)>0){
	  			  sprintf(caTemp+strlen(caTemp)," and a.groupname like '%s%c' ", caOptd,'%');
	  		}else{
	  				sprintf(caTemp+strlen(caTemp)," and substring(a.groupname, 9, 2) = '%s' ", caStyped);
	  		}
  	}
  	printf("caTemp = %s\n", caTemp);
		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lTotRec);
		utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
		pdate=(struct groupdate*)malloc(lTotRec*sizeof(struct groupdate)+1);
		
		iStart = atol(start);
	  iLimit = atol(limit);
	  
	  // 导出
	  if(strlen(caExport)>0){
	 		 strcpy(caFilename, "usermanager.csv");
	 		 strcpy(caPath, "/home/ncmysql/ncsrv/html/v8");
	 		 sprintf(caFile, "%s/%s", caPath, caFilename);
	 		 printf("caFile = %s\n", caFile);
	 		 fp = fopen(caFile, "w");
	 		 if(fp == NULL){
	 		 		 pasDbCloseCursor(psCur);
		       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncLang("0546数据导出出错"),ncLang("0546数据导出出错"));
		       return 0;	
	 		 }
	 		 
	 		 fprintf(fp,"用户信息\n");
			 ncUtlPrintCsv(fp,8,UT_TYPE_STRING,"用户名称",
			                    UT_TYPE_STRING,"显示名称",
			                    UT_TYPE_STRING,"单位名称",
			                    UT_TYPE_STRING,"SSID",
			                    UT_TYPE_STRING,"Portal名称",
			                    UT_TYPE_STRING,"密码",
			                    UT_TYPE_STRING,"有效时间",
			                    UT_TYPE_STRING,"最后访问时间");
	  }
		
    sprintf(caTemp,"select a.userid,a.username,a.dispname,a.groupname,a.groupid,a.ssid,a.proname,a.openid,a.password,a.mark,a.addtime,a.moditime,a.lasttime,a.expiretime,a.usertype,a.flags,a.maxbindmac,'' from ncsrvuser as a where  a.username is not null and a.username!=''");
   if(strlen(groupname)>0)
   	{
   			if(strcmp(groupname,"ALL")!=0)
   			{
   					sprintf(caTemp,"select a.userid,a.username,a.dispname,a.groupname,a.groupid,a.ssid,a.proname,a.openid,a.password,a.mark,a.addtime,a.moditime,a.lasttime,a.expiretime,a.usertype,a.flags,a.maxbindmac,'' from ncsrvuser as a,ncsrvgroup as b where a.groupname = b.groupname  and b.dispname ='%s' and a.username is not null and a.username!=''",groupname);	
   			}	
   			if(strcmp(groupname,"未划分单位")==0)
   			{
   					sprintf(caTemp,"select a.userid,a.username,a.dispname,a.groupname,a.groupid,a.ssid,a.proname,a.openid,a.password,a.mark,a.addtime,a.moditime,a.lasttime,a.expiretime,a.usertype,a.flags,a.maxbindmac,'' from ncsrvuser as a where  a.groupname='' and a.username is not null and a.username!=''");	
   			}	
   			
   	}
     if(strlen(caShopids)>0){
    	sprintf(caTemp+strlen(caTemp)," and a.groupid in (%s) ",caShopids);
    }
   
    sprintf(caTemp+strlen(caTemp)," and a.ssid!='%s'", cnfSsid);
    if (strlen(mokeyword) > 0)
    {	
    	sprintf(caTemp+strlen(caTemp)," and (a.username like '%c%s%c' or a.dispname like '%c%s%c' or a.usermac like '%c%s%c' ) ",'%',mokeyword,'%','%',mokeyword,'%','%',mokeyword,'%');
  	}
  	if (strlen(groupid) > 0)
    {	
    	sprintf(caTemp+strlen(caTemp)," and a.groupid=%lu ", atol(groupid));
  	}
  	if(strlen(caStyped)>0){
	  		if(strlen(caOptd)>0){
	  			  sprintf(caTemp+strlen(caTemp)," and a.groupname like '%s%c' ", caOptd,'%');
	  		}else{
	  				sprintf(caTemp+strlen(caTemp)," and a.substring(groupname, 9, 2) = '%s' ", caStyped);
	  		}
  	}
  	if(strlen(caExport)==0){
  			sprintf(caTemp+strlen(caTemp)," order by a.lasttime desc limit %d , %d ",iStart, iLimit < lTotRec ? iLimit : lTotRec);
    }
  	printf("caTemp=%s \n",caTemp);
  	psCur=pasDbOpenSql(caTemp,0);
    if(psCur==NULL){
    	    utPltFreeDb(psDbHead);
          utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","用户管理查询",ncLang("0174出错 %s"),pasDbErrorMsg(NULL));
          return 0;
    }
    
    iReturn=pasDbFetchInto(psCur,UT_TYPE_ULONG, sizeof(long),         &lUserid,
                                 UT_TYPE_STRING,sizeof(caUsername)-1, caUsername,
                                 UT_TYPE_STRING,sizeof(caDispname)-1, caDispname,
                                 UT_TYPE_STRING,sizeof(caGroupname)-1,caGroupname,
                                 UT_TYPE_ULONG,  sizeof(long),         &lGroupid,
                                 UT_TYPE_STRING,sizeof(caSsid)-1,     caSsid,
                                 UT_TYPE_STRING,sizeof(caProname)-1,  caProname,
                                 UT_TYPE_STRING,sizeof(caOpenid)-1,   caOpenid,
                                 UT_TYPE_STRING,sizeof(caPassword)-1, caPassword,
                                 UT_TYPE_STRING,sizeof(caMark)-1,     caMark,
                                 UT_TYPE_ULONG, sizeof(long),         &lAddtime,
                                 UT_TYPE_ULONG, sizeof(long),         &lModitime,
                                 UT_TYPE_ULONG, sizeof(long),         &lLasttime,
                                 UT_TYPE_ULONG, sizeof(long),         &lExpiretime,
                                 UT_TYPE_ULONG, sizeof(long),         &lUsertype,
                                 UT_TYPE_ULONG, sizeof(long),         &lFlags,
                                 UT_TYPE_ULONG, sizeof(long),         &lMaxbindmac,
                                 UT_TYPE_STRING,sizeof(dev)-1, dev);
      				  
    iNum=0;
    while(iReturn==0||iReturn==1405){
	    	strcpy(pdate[iNum].user,caUsername);
	    	strcpy(pdate[iNum].groupname,caGroupname);
    	  
      	if(iNum>0){
        	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
        }
      	iNum++;
      	utPltPutLoopVarF(psDbHead,"userid",iNum,"%lu",lUserid);
	      utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu",iNum);
	      utPltSetCvtHtml(1);    
      	utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
      	utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
    
        utPltSetCvtHtml(0);   	
      	utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",lGroupid);
      	utPltPutLoopVar(psDbHead,"ssid",iNum,caSsid);
      	utPltPutLoopVar(psDbHead,"proname",iNum,caProname);
      	utPltPutLoopVar(psDbHead,"openid",iNum,caOpenid);
      	utPltPutLoopVar(psDbHead,"password",iNum,caPassword);
      	utPltPutLoopVar(psDbHead,"mark",iNum,caMark);
      	
      	 psGroup = (ncPortalGroup *)ncSrvGetGroupInfoByName(psShmHead,caGroupname);
      						   if(psGroup){
		                       if(strcmp(psGroup->caDisp,"user")==0){
		                       	 psGroup = (ncPortalGroup *) ncSrvGetGroupInfoById(psShmHead,lGroupid);
		                       	 if(psGroup){
		                       	 	utPltPutLoopVar(psDbHead,"groupname",iNum,psGroup->caDisp);
		                       	}
		                   //    	utPltPutLoopVar(psDbHead,"groupname",iNum,"未划分单位");
		                      }
		                      else{
		                     
					  				 	      utPltPutLoopVar(psDbHead,"groupname",iNum,psGroup->caDisp);
					  				 	    }
					  				}
      	
      	
      	
      	if(lAddtime>0){
      	   utPltPutLoopVar(psDbHead,"addtime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",lAddtime));
        }
      	if(lModitime>0){
      	  utPltPutLoopVar(psDbHead,"moditime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",lModitime));
        }
      	if(lLasttime>0){
      	  utPltPutLoopVar(psDbHead,"lasttime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",lLasttime));
      	  strcpy(s_lasttime, utTimFormat("%Y-%m-%d %H:%M:%S",lLasttime));
        }
      	if(lExpiretime>0){
      	   utPltPutLoopVar(psDbHead,"expiretime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",lExpiretime));
      	   strcpy(s_expiretime, utTimFormat("%Y-%m-%d %H:%M:%S",lExpiretime));
        }
      	switch(lUsertype){
      			case 0:strcpy(s_usertype, "固定用户");break;
      			case 1:strcpy(s_usertype, "短消息");break;	
      			case 2:strcpy(s_usertype, "QQ");break;
      			case 3:strcpy(s_usertype, "微信");break;
      			default:break;
      	}
      	utPltPutLoopVar(psDbHead,"usertype",iNum,s_usertype);
      	switch(lFlags){
      			case 0:strcpy(s_flags, "有效");break;
      			case 9:strcpy(s_flags, "无效");break;
      			default:break;	
      	}
        utPltPutLoopVar(psDbHead,"flags",iNum,s_flags);
        utPltPutLoopVarF(psDbHead,"maxbindmac",iNum,"%lu",lMaxbindmac);
        utPltPutLoopVar(psDbHead,"dev",iNum,dev);
        
        if(strlen(caExport)>0){
				 		ncUtlPrintCsv(fp,8,UT_TYPE_STRING,caUsername,
													 		  UT_TYPE_STRING,caDispname,
													 		  UT_TYPE_STRING,caGroupname,
													 		  UT_TYPE_STRING,caSsid,
													 		  UT_TYPE_STRING,caProname,
													 		  UT_TYPE_STRING,caPassword,
		                            UT_TYPE_STRING,s_expiretime,
		                            UT_TYPE_STRING,s_lasttime);
			  }
        
	      lUserid=0;
		    memset(caUsername,0,sizeof(caUsername));
		    memset(caDispname,0,sizeof(caDispname));
		    memset(caGroupname,0,sizeof(caGroupname));
		    lGroupid=0;
		    memset(caSsid,0,sizeof(caSsid));
		    memset(caProname,0,sizeof(caProname));
		    memset(caOpenid,0,sizeof(caOpenid));
		    memset(caPassword,0,sizeof(caPassword));
		    memset(caMark,0,sizeof(caMark));
		    lAddtime=0;
		    lModitime=0;
		    lLasttime=0;
		    lExpiretime=0;
		    lUsertype=0;
		    lFlags=0;
		    lMaxbindmac=0;
		    memset(s_usertype,0,sizeof(s_usertype));
		    memset(s_flags,0,sizeof(s_flags));
		    memset(s_lasttime,0,sizeof(s_lasttime));
		    memset(s_expiretime,0,sizeof(s_expiretime));
	      iReturn=pasDbFetchInto(psCur,UT_TYPE_ULONG, sizeof(long),         &lUserid,
		                                 UT_TYPE_STRING,sizeof(caUsername)-1, caUsername,
		                                 UT_TYPE_STRING,sizeof(caDispname)-1, caDispname,
		                                 UT_TYPE_STRING,sizeof(caGroupname)-1,caGroupname,
		                                 UT_TYPE_ULONG,  sizeof(long),         &lGroupid,
		                                 UT_TYPE_STRING,sizeof(caSsid)-1,     caSsid,
		                                 UT_TYPE_STRING,sizeof(caProname)-1,  caProname,
		                                 UT_TYPE_STRING,sizeof(caOpenid)-1,   caOpenid,
		                                 UT_TYPE_STRING,sizeof(caPassword)-1, caPassword,
		                                 UT_TYPE_STRING,sizeof(caMark)-1,     caMark,
		                                 UT_TYPE_ULONG, sizeof(long),         &lAddtime,
		                                 UT_TYPE_ULONG, sizeof(long),         &lModitime,
		                                 UT_TYPE_ULONG, sizeof(long),         &lLasttime,
		                                 UT_TYPE_ULONG, sizeof(long),         &lExpiretime,
		                                 UT_TYPE_ULONG, sizeof(long),         &lUsertype,
		                                 UT_TYPE_ULONG, sizeof(long),         &lFlags,
                                     UT_TYPE_ULONG, sizeof(long),         &lMaxbindmac);
  	}
    pasDbCloseCursor(psCur);
    
    for(i=0;i<iNum;i++){
	    	sprintf(caTemp,"select usermac from ncsrvusermac where username='%s' and flags != 9 ",pdate[i].user);
	    	psCur=pasDbOpenSql(caTemp,0);
	    	if(psCur){
		    		long mac_num=0;
		    		memset(caMac,0,sizeof(caMac));
		    		iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,19, caMac);
		    		memset(caMac_all,0,sizeof(caMac_all));
		    		while(iReturn==0||iReturn==1405){
			    			 mac_num++;
			    			 if(mac_num>atol(cnfMac)) break;
			    			 sprintf(caMac_all+strlen(caMac_all),"%s,",caMac);
			    			 memset(caMac,0,sizeof(caMac));
			    		   iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,19, caMac);
		    		}
		    		pasDbCloseCursor(psCur);
		        utPltPutLoopVarF(psDbHead,"macnum",i+1,"%lu",mac_num);
		    		utPltPutLoopVar(psDbHead,"usermac",i+1,caMac_all);
	    	}
	    	
	    	char tempcode[32] = "";
	    	char tempname[32] = "";
	    	/*
	    	if(strlen(pdate[i].groupname)>16){
	    			//printf("pdate[i].groupname = %s\n",pdate[i].groupname);
			    	ncStrToStr(pdate[i].groupname, tempcode);
			    	//printf("tempcode = %s\n",tempcode);
			    	sprintf(caTemp, "select groupname from ncmautopri where groupcode = '%s' and shopcode = '%s' ", tempcode,caStyped);
			    	//printf("caTemp = %s\n", caTemp);
						pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(tempname)-1, tempname);
						utPltPutLoopVar(psDbHead,"groupname",i+1,tempname);
	    	}else{
	    			utPltPutLoopVar(psDbHead,"groupname",i+1,"");
	    	}
	    	*/
    }
    
    if(strlen(caExport)>0){
 	    fclose(fp);
      utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
      remove(caFile);
      utPltFreeDb(psDbHead);
      return 0;
	  }
    
    //utPltShowDb(psDbHead); 
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_ext_webshowuser_man.htm");
      
    return 0;
}

//显示属主
int ncmWebGroupSelectComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    int  iNum;
    utPltDbHead *psDbHead;
    int iReturn,i;
    char groupname[32],dispname[32];
    char caTemp[1024];
#ifdef LDEBUG1
    utMsgPrintMsg(psMsgHead);
#endif

    sprintf(caTemp,"select groupname,dispname from ncsrvgroup limit 0,29 ");
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur){
        psDbHead = utPltInitDb();

		    memset(groupname,0,sizeof(groupname));
		    memset(dispname,0,sizeof(dispname));
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(groupname)-1,groupname,
		                                   UT_TYPE_STRING,sizeof(dispname)-1, dispname);
		    iNum=0;
		    while((iReturn==0)||(iReturn==1405))
		    {
		        utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		        iNum++;
		        utPltPutLoopVar(psDbHead,"groupname", iNum,dispname);
		        utPltPutLoopVar(psDbHead,"groupid",   iNum,groupname);
		        memset(groupname,0,sizeof(groupname));
		        memset(dispname,0,sizeof(dispname));
		        iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(groupname)-1,groupname,
		                                       UT_TYPE_STRING,sizeof(dispname)-1, dispname);
		    }
		    pasDbCloseCursor(psCur);
    }

    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_groupclass_list.htm");
    return 0;
}

//判断商城用户是否在线
int ncmShopUserOnlineFlag(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iReturn,i;
    char clientip[32] = "", tsid[64] = "";
    ncPortalOnline *onlineinfo;
//    ncPortalOnline *ncSrvGetOnlineUserByIp(utShmHead *psShmHead,uint4 lSip);
    iReturn = utMsgGetSomeNVar(psMsgHead,2, "clientip",UT_TYPE_STRING,sizeof(clientip) - 1  ,clientip
                                          , "tsid",    UT_TYPE_STRING,sizeof(tsid) - 1  ,tsid);
//    printf("*******ip = %s    cip=%d, tsid = %s\n",clientip,ipstrtoint(clientip),tsid);
    if(strlen(tsid) > 0)
    {
    	onlineinfo = ncSrvGetOnlineUserByStrTsid(psShmHead,tsid);
    	if (onlineinfo)
    	{	
//    	onlineinfo = ncSrvGetOnlineUserByIp(psShmHead,ipstrtoint(clientip));
//	      printf("*******login = %d \n",onlineinfo->login);
	      if(onlineinfo->login == 1)
	      {
	         return 1;	
	      }
      }
      else 
      {
         return 0;	
      }	
    }	
    else
    {
    	return 0;
    }	
}
//插入商城用户到个人中心
int ncmShopUserOnlineInsert(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iReturn,i,lSid = 0, lCount1 = 0;
    char clientip[32] = "", tsid[64] = "", caTemp[1024] = "";
    ncPortalOnline *onlineinfo;
    
    iReturn = utMsgGetSomeNVar(psMsgHead,1, "tsid",    UT_TYPE_STRING,sizeof(tsid) - 1  ,tsid);
    printf("*******tsid = %s\n",tsid);
    if(strlen(tsid) > 0)
    {
    	onlineinfo = ncSrvGetOnlineUserByStrTsid(psShmHead,tsid);
    	if (onlineinfo)
    	{	
	    	lSid=utMd5Code(onlineinfo->caName,strlen(onlineinfo->caName),"ffff");
	   	  if(lSid>2000000000) lSid=lSid-2000000000;
	   	  if(lSid>2000000000) lSid=lSid-2000000000;
	      printf("*******caName = %s \n",onlineinfo->caName);
	      sprintf(caTemp, "select count(*) from ncmshopuser where username = '%s'", onlineinfo->caName);
		    iReturn = pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &lCount1);
		    if (lCount1 == 0)
		    {	
	          sprintf(caTemp,"insert into ncmshopuser(uid,username) values(%d,'%s')",lSid, onlineinfo->caName);
		 		    printf("caTemp = %s\n", caTemp);
				    pasDbExecSqlF(caTemp);
	      }
	      return 0;	
	      
	    }	
    }
    else
    {
    	return 0;
    }	
}
char *ncmShopGetOnlineMark(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iReturn,i,lSid = 0, lCount1 = 0;
    char caMark[128] = "", tsid[64] = "", caTemp[1024] = "";
    ncPortalOnline *onlineinfo;
    
    iReturn = utMsgGetSomeNVar(psMsgHead,1, "tsid",    UT_TYPE_STRING,sizeof(tsid) - 1  ,tsid);
    printf("*******tsid = %s\n",tsid);
    if(strlen(tsid) > 0)
    {
    	onlineinfo = ncSrvGetOnlineUserByStrTsid(psShmHead,tsid);
    	if(onlineinfo)
    	{
    	    strcpy(caMark,onlineinfo->caMark);
    	}	
    	return caMark;	
    }	
    else
    {
    	return "";
    }	
}

int ncmWebAdmin(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	  // 初始化数据库
	  pasDbCursor *psCur = NULL;
	  utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	  
		// 得到账号ID
    unsigned long lCid = 0;
    char caOptd[255] = "";
    char caStyped[4] = "";
	  dsCltGetSessionValue(1, "cid", UT_TYPE_LONG, sizeof(long), &lCid);
	  dsCltGetSessionValue(1, "opt", UT_TYPE_STRING, sizeof(caOptd)-1, caOptd);
	  dsCltGetSessionValue(1, "stype", UT_TYPE_STRING, sizeof(caStyped)-1, caStyped);
	  printf("caStyped = %s\n",caStyped);
	  printf("lCid = %lu\n", lCid);
	  printf("caOptd = %s\n", caOptd);
	  printf("lCid = %d, opt = %d stype = %d\n", lCid,strlen(caOptd),strlen(caStyped));	 
	  if(lCid==0 && strlen(caOptd)==0 && strlen(caStyped)==0){
	  		utPltPutVar(psDbHead,"username","admin");
	  }
	  
	  if(strlen(caStyped)>0){
	  		if(strlen(caOptd)>18){
	  				utPltPutVar(psDbHead,"isexp","true");	
	  		}else{
	  				utPltPutVar(psDbHead,"isexp","false");		
	  		}
	  }else{
	  		utPltPutVar(psDbHead,"isexp","true");	
	  }
	  
	  char caTemp[256] = "";
	  long lCount1 = 0;
		sprintf(caTemp,"select autodef from ncmshopgtype where type=2 and groupcode = '%s' ",caStyped);
		printf("caTemp = %s\n",caTemp);	
		pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, 4, &lCount1);
		printf("lCount1 = %d\n",lCount1);
		if(lCount1 == 1){
			utPltPutVar(psDbHead,"autodef","true");
		}else{
			utPltPutVar(psDbHead,"autodef","false");
		}  
		
		utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncm_webadmin_list.htm");
	  return 0;	
}

// 用户MAC
int ncm_webusermac_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char preusermac[20] = "";
	 char usermac[32] = "";
	 char username[32] = "";
	 char groupid[20] = "";
	 char sdate[32] = "";
	 char stime[32] = "";
	 char edate[32] = "";
	 char etime[32] = "";
	 char flags[32] = "";
	 	
	 // 存放数据库取出的数据的变量
	 char          	caUsermac[32] = "";
	 char         	caUsername[32] = "";
	 unsigned long	lGroupid = 0;
	 unsigned long  lStarttime = 0;
	 unsigned long  lLasttime = 0;
	 unsigned long  lModitime = 0;
	 unsigned long  lFlags = 0;
	 char           caGroupname[32] = "";
	 char           s_starttime[32] = "";
	 char           s_lasttime[32] = "";
	 char           s_moditime[32] = "";
	 char           s_flags[32] = "";
	 
	 char caUpdate[16] = "";
	 char caSesid[1024] = "";
	 char caDel[16] = "";
	 char caTemp[4096] = "";
	 unsigned long	lMaxid = 0;
	 // 关键字
	 char _keyword[33] = "";
	 
	 // 数据表
	 unsigned long lCount1 = 0; // 表中记录的个数
	 long iReturn; // 单记录查询
	 char start[17] = ""; // 默认记录从0开始
	 char limit[17] = ""; // 限定一页中的记录是40条
	 unsigned long iStart = 0; // 转换后的默认开始atoi(start)
	 unsigned long iLimit = 0; // 转换后的限定记录条数atoi(limit)
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 19,"keyword",   UT_TYPE_STRING, sizeof(_keyword)-1,  _keyword,
	 																"preusermac",UT_TYPE_STRING, sizeof(preusermac)-1,preusermac,
	 																"usermac",   UT_TYPE_STRING, sizeof(usermac)-1,   usermac,
	                                "username",  UT_TYPE_STRING, sizeof(username)-1,  username,
	                                "groupid",   UT_TYPE_STRING, sizeof(groupid)-1,   groupid,
	                                "sdate",     UT_TYPE_STRING, sizeof(sdate)-1,     sdate,
	                                "stime",     UT_TYPE_STRING, sizeof(stime)-1,     stime,
	                                "edate",     UT_TYPE_STRING, sizeof(edate)-1,     edate,
	                                "etime",     UT_TYPE_STRING, sizeof(etime)-1,     etime,
	                                "flags",     UT_TYPE_STRING, sizeof(flags)-1,     flags,
	                                "start",     UT_TYPE_STRING, sizeof(start)-1,     start,
															   	"limit",     UT_TYPE_STRING, sizeof(limit)-1,     limit,
															   	"sort",      UT_TYPE_STRING, sizeof(sort)-1,      sort,
															   	"dir",       UT_TYPE_STRING, sizeof(dir)-1,       dir,
																	"selsid",    UT_TYPE_STRING, sizeof(caSesid)-1,   caSesid,
																	"del",			 UT_TYPE_STRING, sizeof(caDel)-1,     caDel,
																	"update",    UT_TYPE_STRING, sizeof(caUpdate)-1,  caUpdate,
																	"caExport",  UT_TYPE_STRING, sizeof(caExport)-1,  caExport);
	 
	 // 排序
	 if(strlen(sort)==0){
	 	  strcpy(sort,"sid");
	 }
	 // 去除关键字前后的空格
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,32);
			strcpy(_keyword,caTemp);
   }
   if(strlen(usermac)>0){
			pasCvtGBK(2,usermac,caTemp,32);
			strcpy(usermac,caTemp);
   }
   if(strlen(username)>0){
			pasCvtGBK(2,username,caTemp,32);
			strcpy(username,caTemp);
   }
	 
   // 增加/修改
	 if(!utStrIsSpaces(caUpdate)) {
	 	  char starttime[32] = "";
	 	  unsigned long lstarttime = 0;
	 	  char lasttime[32] = "";
	 	  unsigned long llasttime = 0;
	 	  sprintf(starttime, "%s %s", sdate, stime);
	 	  sprintf(lasttime, "%s %s", edate, etime);
	 	  ncTimeToLong(starttime, &lstarttime);
	 	  ncTimeToLong(lasttime, &llasttime);
	 	  if(llasttime < lstarttime){
	 	  		return 0;	
	 	  }
	 	  
	 		if (strcmp(caUpdate, "update")==0){
	 				sprintf(caTemp,"update ncsrvusermac set usermac='%s', username='%s', groupid=%lu, starttime=%lu, lasttime=%lu, moditime=%lu, flags=%lu where usermac='%s'", usermac,username,atol(groupid),lstarttime,llasttime,time(0),atol(flags),preusermac);
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }else if(strcmp(caUpdate, "add")==0){
	 	  	  sprintf(caTemp,"insert into ncsrvusermac (usermac,username,groupid,starttime,lasttime,moditime,flags) values ('%s','%s',%lu,%lu,%lu,%lu,%lu)",usermac,username,atol(groupid),lstarttime,llasttime,time(0),atol(flags));
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 }
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    		  pasDbExecSqlF("delete from ncsrvusermac where usermac in(%s)",caSesid);
    	}
	 }
	 
	 // 查询
	 strcpy(sql, "select count(*) from ncsrvusermac where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (usermac like '%%%s%%')", _keyword);
	 }
	 if(strlen(username) > 0)
   {	
    	sprintf(sql+strlen(sql)," and username='%s' ", username);
   }
	 if(strlen(groupid) > 0)
   {	
    	sprintf(sql+strlen(sql)," and groupid=%lu ", atol(groupid));
   }
   printf("sql = %s\n", sql);
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select u.usermac, u.username, g.dispname, u.groupid, u.starttime, u.lasttime, u.moditime, u.flags from ncsrvusermac u left join ncsrvgroup g on u.groupid=g.groupid where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (u.usermac like '%%%s%%')", _keyword);
	 }
	 if (strlen(username) > 0)
   {	
    	sprintf(sql+strlen(sql)," and u.username='%s' ", username);
   }
	 if (strlen(groupid) > 0)
   {	
    	sprintf(sql+strlen(sql)," and u.groupid=%lu ", atol(groupid));
   }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by u.username DESC, u.%s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(caUsermac)-1,   caUsermac,
															 UT_TYPE_STRING, sizeof(caUsername)-1,  caUsername,
															 UT_TYPE_STRING, sizeof(caGroupname)-1, caGroupname,
													     UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
													     UT_TYPE_ULONG,  sizeof(long),          &lStarttime,
													     UT_TYPE_ULONG,  sizeof(long),          &lLasttime,
													     UT_TYPE_ULONG,  sizeof(long),          &lModitime,
													     UT_TYPE_ULONG,  sizeof(long),          &lFlags);
				
				
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
			 
			 if(lStarttime>0){
			 			strcpy(s_starttime, utTimFormat("%Y/%m/%d %H:%M:%S", lStarttime));
			 }else{
			 		strcpy(s_starttime, "");
			 }
			 if(lLasttime>0){
			 			strcpy(s_lasttime, utTimFormat("%Y/%m/%d %H:%M:%S", lLasttime));
			 }else{
			 		strcpy(s_lasttime, "");
			 }
			 if(lModitime>0){
			 			strcpy(s_moditime, utTimFormat("%Y/%m/%d %H:%M:%S", lModitime));
			 }else{
			 		strcpy(s_moditime, "");
			 }
			 
			 switch(lFlags){
			 		case 0:strcpy(s_flags, "有效");break;	
			 		case 9:strcpy(s_flags, "无效");break;	
			 		default:break;
			 }
			 
			 utPltPutLoopVar(psDbHead,"usermac",iNum,caUsermac);
			 utPltPutLoopVar(psDbHead,"username",iNum,caUsername);
			 utPltPutLoopVar(psDbHead,"groupname",iNum,caGroupname);
			 utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",lGroupid);
			 utPltPutLoopVar(psDbHead,"starttime",iNum,s_starttime);
			 utPltPutLoopVar(psDbHead,"lasttime",iNum,s_lasttime);
			 utPltPutLoopVar(psDbHead,"moditime",iNum,s_moditime);
			 utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",lFlags);
			 utPltPutLoopVar(psDbHead,"s_flags",iNum,s_flags);
			 
			 memset(caUsermac, 0, sizeof(caUsermac));
			 memset(caUsername, 0, sizeof(caUsername));
			 memset(caGroupname, 0, sizeof(caGroupname));
			 lGroupid = 0;
			 lStarttime = 0;
	     lLasttime = 0;
	     lModitime = 0;
	     lFlags = 0;
	     memset(s_starttime, 0, sizeof(s_starttime));
	     memset(s_lasttime, 0, sizeof(s_lasttime));
	     memset(s_moditime, 0, sizeof(s_moditime));
	     memset(s_flags, 0, sizeof(s_flags));
	     iret = pasDbFetchInto(psCur,UT_TYPE_STRING, sizeof(caUsermac)-1,   caUsermac,
																	 UT_TYPE_STRING, sizeof(caUsername)-1,  caUsername,
																	 UT_TYPE_STRING, sizeof(caGroupname)-1, caGroupname,
															     UT_TYPE_ULONG,  sizeof(long),          &lGroupid,
															     UT_TYPE_ULONG,  sizeof(long),          &lStarttime,
															     UT_TYPE_ULONG,  sizeof(long),          &lLasttime,
															     UT_TYPE_ULONG,  sizeof(long),          &lModitime,
															     UT_TYPE_ULONG,  sizeof(long),          &lFlags);	
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncm_webusermac_list.htm");
	  return 0;	
}

char *ncmWebAutoDefGet16(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead,char stype[4])
{
	char caTemp[1024] = "";
	char my16code[20] = "", pronv[64] = "",  cagtype[32] = "", sgroupcode[16] = "";
	int pronvid = 0;
	strcpy(pronv,utComGetVar_sd(psShmHead,"NcmProvince",  "河南省"));
  strcpy(cagtype,utComGetVar_sd(psShmHead,"NcmDeptType",  "商城"));
  sprintf(caTemp, "select aid from ncmarea where aname='%s'", pronv);
	pasDbOneRecord(caTemp, 0, UT_TYPE_ULONG, sizeof(long), &pronvid);
	sprintf(caTemp, "select groupcode from ncmshopgtype where groupname='%s' and type = 1", cagtype);
	pasDbOneRecord(caTemp, 0, UT_TYPE_STRING, sizeof(sgroupcode) - 1, sgroupcode);
	sprintf(my16code,"%02d0000%s%s000000",pronvid,sgroupcode,stype);
	printf("**** my16code = %s \n",my16code);
	return (char *)my16code;
}

int ncm_webautodef_ajax(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
		utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
		
		// 初始化数据库
		char sql[4096] = "";
		pasDbCursor *psCur = NULL;
		utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
		
		// 接收信息变量
		char groupcode[32] = "";
		
		// 存放数据库取出的数据的变量
		unsigned long lAutodef = 0;
		
		// utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
		utMsgGetSomeNVar(psMsgHead, 1,"groupcode", UT_TYPE_STRING, sizeof(groupcode)-1,  groupcode);
		
		sprintf(sql, "select autodef from ncmshopgtype where groupcode='%s'", groupcode);
		printf("sql = %s\n", sql);
		pasDbOneRecord(sql, 0, UT_TYPE_ULONG, sizeof(long), &lAutodef);
		if(lAutodef==0){
				utPltPutVar(psDbHead,"autodef","false");	
		}else{
				utPltPutVar(psDbHead,"autodef","true");		
		}
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncm_webautodef_ajax.htm");
	  return 0;	
}

int ncm_webmenu_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char id[20] = "";
	 char mainmenu[16] = "";
	 char systime[32] = "";
	 char flag[20] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long lId = 0;
 	 char caMainmenu[16] = "";
	 unsigned long lSystime = 0;
	 unsigned long lFlag = 0;
	 char s_systime[32] = "";
	 char s_flag[32] = "";
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	 char caSesid[1024] = "";
	 char caTemp[4096] = "";
	 // 关键字
	 char _keyword[33] = "";
	 
	 // 数据表
	 unsigned long lCount1 = 0; // 表中记录的个数
	 long iReturn; // 单记录查询
	 char start[17] = ""; // 默认记录从0开始
	 char limit[17] = ""; // 限定一页中的记录是40条
	 unsigned long iStart = 0; // 转换后的默认开始atoi(start)
	 unsigned long iLimit = 0; // 转换后的限定记录条数atoi(limit)
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 13,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
															    "id",          UT_TYPE_STRING,  sizeof(id)-1,             id,
																	"mainmenu",    UT_TYPE_STRING,  sizeof(mainmenu)-1,       mainmenu,
																	"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
																	"flag",        UT_TYPE_STRING,  sizeof(flag)-1,           flag,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
																	"del",				 UT_TYPE_STRING,  10,                       caDel,
																	"update",      UT_TYPE_STRING,  10,                       caUpdate,
																	"caExport",    UT_TYPE_STRING,  10,                       caExport);
			
	 // 排序
	 if(strlen(sort)==0){
	 	  strcpy(sort,"id");
	 }
	 // 去除关键字前后的空格
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,32);
			strcpy(_keyword,caTemp);
   }
   if(strlen(mainmenu)>0){
			pasCvtGBK(2,mainmenu,caTemp,16);
			strcpy(mainmenu,caTemp);
   }
   
	 // 增加/修改
	 if(!utStrIsSpaces(caUpdate)) {
	 		if (atol(id)>0){
	 				sprintf(caTemp,"update ncminitmenu set mainmenu='%s', systime=%lu, flag=%lu where id=%lu ", mainmenu,time(0),atol(flag),atol(id));
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  } else {
		    	sprintf(caTemp,"insert into ncminitmenu (mainmenu,systime,flag) values ('%s',%lu,%lu)",mainmenu,time(0),atol(flag));
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 }
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    		  pasDbExecSqlF("delete from ncminitmenu where id in(%s)",caSesid);
    	}
	 }
	 	 
	 // 查询
	 strcpy(sql, "select count(*) from ncminitmenu where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mainmenu like '%%%s%%'", _keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select id, mainmenu, systime, flag from ncminitmenu where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mainmenu like '%%%s%%'", _keyword);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lId,
													  	 UT_TYPE_STRING, sizeof(caMainmenu)-1,  caMainmenu,
															 UT_TYPE_ULONG,  sizeof(long),          &lSystime,
															 UT_TYPE_ULONG,  sizeof(long),          &lFlag);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
			 
			 switch(lFlag){
			 		case 0:strcpy(s_flag, "使用");break;	
			 		case 1:strcpy(s_flag, "不使用");break;	
			 		default:break;		
			 }
			 if(lSystime>0){
			 		strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", lSystime));
			 }else{
			 		strcpy(s_systime, "");
			 }
		   
		   utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",lId);
		   utPltPutLoopVar(psDbHead,"mainmenu",iNum,caMainmenu);
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVarF(psDbHead,"flag",iNum,"%lu",lFlag);
			 utPltPutLoopVar(psDbHead,"s_flag",iNum,s_flag);
			 
			 lId = 0;
		 	 memset(caMainmenu, 0, sizeof(caMainmenu));
			 lSystime = 0;
			 lFlag = 0;
			 memset(s_systime, 0, sizeof(s_systime));
			 memset(s_flag, 0, sizeof(s_flag));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lId,
															  	 UT_TYPE_STRING, sizeof(caMainmenu)-1,  caMainmenu,
																	 UT_TYPE_ULONG,  sizeof(long),          &lSystime,
																	 UT_TYPE_ULONG,  sizeof(long),          &lFlag);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncm_webmenu_list.htm");
	  return 0;	
}

int ncm_websubmenu_list(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
	 
	 // 初始化数据库
	 char sql[4096] = "";
	 pasDbCursor *psCur = NULL;
	 int iret=0;
	 int iNum=0;
	 utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 
	 // 接收信息变量
	 char sid[20] = "";
	 char submenu[32] = "";
	 char systime[32] = "";
	 char pid[32] = "";
	 char flag[20] = "";
	 
	 // 存放数据库取出的数据的变量
	 unsigned long lSid = 0;
 	 char caSubmenu[32] = "";
	 unsigned long lSystime = 0;
	 unsigned long lPid = 0;
	 char caMainmenu[32] = "";
	 unsigned long lFlag = 0;
	 char s_systime[32] = "";
	 char s_flag[32] = "";
	 
	 char caUpdate[16] = "";
	 char caDel[16] = "";
	 char caSesid[1024] = "";
	 char caTemp[4096] = "";
	 // 关键字
	 char _keyword[33] = "";
	 
	 // 数据表
	 unsigned long lCount1 = 0; // 表中记录的个数
	 long iReturn; // 单记录查询
	 char start[17] = ""; // 默认记录从0开始
	 char limit[17] = ""; // 限定一页中的记录是40条
	 unsigned long iStart = 0; // 转换后的默认开始atoi(start)
	 unsigned long iLimit = 0; // 转换后的限定记录条数atoi(limit)
	 
	 // 翻页
	 unsigned long lTotRec = 0; // 记录表中的数据的个数
	 
	 // 导出
	 char caExport[32] = "";
	 
	 // 排序
	 char sort[17] = "";
   char dir[9] = "";
	 
	 // utMsgGetSomeNVar　从消息数据包中取指定数量的变量。按指定的长度返回。
	 utMsgGetSomeNVar(psMsgHead, 14,"keyword",     UT_TYPE_STRING,  sizeof(_keyword)-1,       _keyword,
															    "sid",         UT_TYPE_STRING,  sizeof(sid)-1,            sid,
																	"submenu",     UT_TYPE_STRING,  sizeof(submenu)-1,        submenu,
																	"systime",     UT_TYPE_STRING,  sizeof(systime)-1,        systime,
																	"pid",          UT_TYPE_STRING,  sizeof(pid)-1,           pid,
																	"flag",        UT_TYPE_STRING,  sizeof(flag)-1,           flag,
																	"start",       UT_TYPE_STRING,  sizeof(start)-1,          start,
															   	"limit",       UT_TYPE_STRING,  sizeof(limit)-1,          limit,
															   	"sort",        UT_TYPE_STRING,  sizeof(sort)-1,           sort,
															   	"dir",         UT_TYPE_STRING,  sizeof(dir)-1,            dir,
																	"selsid",      UT_TYPE_STRING,  1000,                     caSesid,
																	"del",				 UT_TYPE_STRING,  10,                       caDel,
																	"update",      UT_TYPE_STRING,  10,                       caUpdate,
																	"caExport",    UT_TYPE_STRING,  10,                       caExport);
			
	 // 排序
	 if(strlen(sort)==0){
	 	  strcpy(sort,"pid");
	 }
	 // 去除关键字前后的空格
	 trim(_keyword);
	 
	 //编码转换
	 if(strlen(_keyword)>0){
			pasCvtGBK(2,_keyword,caTemp,32);
			strcpy(_keyword,caTemp);
   }
   if(strlen(submenu)>0){
			pasCvtGBK(2,submenu,caTemp,32);
			strcpy(submenu,caTemp);
   }
   
	 // 增加/修改
	 if(!utStrIsSpaces(caUpdate)) {
	 		if (atol(sid)>0){
	 				sprintf(caTemp,"update ncminitsubmenu set submenu='%s', systime=%lu, pid=%lu, flag=%lu where sid=%lu ", submenu,time(0),atol(pid),atol(flag),atol(sid));
	 				printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  } else {
		    	sprintf(caTemp,"insert into ncminitsubmenu (submenu,systime,pid,flag) values ('%s',%lu,%lu,%lu)",submenu,time(0),atol(pid),atol(flag));
			 		printf("caTemp = %s\n", caTemp);
			 		pasDbExecSqlF(caTemp);
	 	  }
	 }
	 // 删除
	 if(!utStrIsSpaces(caDel)) {
    	if(strlen(caSesid)>0){
    		  pasDbExecSqlF("delete from ncminitsubmenu where sid in(%s)",caSesid);
    	}
	 }
	 	 
	 // 查询
	 strcpy(sql, "select count(*) from ncminitsubmenu s left join ncminitmenu m on s.pid=m.id where 1=1");
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (s.submenu like '%%%s%%' or m.mainmenu like '%%%s%%')", _keyword,_keyword);
	 }
	 iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
	 lTotRec = lCount1;
	 utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	 printf("lTotRec = %d\n", lTotRec);
	 
	 iStart = atol(start);
	 iLimit = atol(limit);
	 
	 strcpy(sql, "select s.sid, s.submenu, s.systime, s.pid, m.mainmenu, s.flag from ncminitsubmenu s left join ncminitmenu m on s.pid=m.id where 1=1");
	 // utStrIsSpaces: 判断字符串是否为空。
	 if(!utStrIsSpaces(_keyword)) // 根据关键字查询
	 {
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (s.submenu like '%%%s%%' or m.mainmenu like '%%%s%%')", _keyword,_keyword);
	 }
	 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by s.%s %s, s.sid ASC limit %lu, %lu", sort, dir, iStart, iLimit < lCount1 ? iLimit : lCount1);
	 printf("sql = %s\n", sql);
	 psCur = pasDbOpenSql(sql, 0);
	 if(psCur==NULL){
	 		 utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0746用户查询"),ncLang("0171查询数据出错"));
       return 0;	
	 }
	 
	 // pasDbFetchInto取当前记录的内容到输出变量。　0---正确，!=0---错误代码, 具体含义请参见 数据库的有关资料
	 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
													  	 UT_TYPE_STRING, sizeof(caSubmenu)-1,   caSubmenu,
															 UT_TYPE_ULONG,  sizeof(long),          &lSystime,
															 UT_TYPE_ULONG,  sizeof(long),          &lPid,
															 UT_TYPE_STRING, sizeof(caMainmenu)-1,  caMainmenu,
															 UT_TYPE_ULONG,  sizeof(long),          &lFlag);
	 
	  while(0 == iret || 1405 == iret){
			 iNum++;
			 if(iNum > 1)
			 {
				 utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
			 }
			 
			 switch(lFlag){
			 		case 0:strcpy(s_flag, "使用");break;	
			 		case 1:strcpy(s_flag, "不使用");break;	
			 		default:break;		
			 }
			 if(lSystime>0){
			 		strcpy(s_systime, utTimFormat("%Y/%m/%d %H:%M:%S", lSystime));
			 }else{
			 		strcpy(s_systime, "");
			 }
		   
		   utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
		   utPltPutLoopVar(psDbHead,"submenu",iNum,caSubmenu);
			 utPltPutLoopVar(psDbHead,"systime",iNum,s_systime);
			 utPltPutLoopVarF(psDbHead,"pid",iNum,"%lu",lPid);
			 utPltPutLoopVar(psDbHead,"mainmenu",iNum,caMainmenu);
			 utPltPutLoopVarF(psDbHead,"flag",iNum,"%lu",lFlag);
			 utPltPutLoopVar(psDbHead,"s_flag",iNum,s_flag);
			 
			 lSid = 0;
		 	 memset(caSubmenu, 0, sizeof(caSubmenu));
			 lSystime = 0;
			 lPid = 0;
			 memset(caMainmenu, 0, sizeof(caMainmenu));
			 lFlag = 0;
			 memset(s_systime, 0, sizeof(s_systime));
			 memset(s_flag, 0, sizeof(s_flag));
			 iret = pasDbFetchInto(psCur,UT_TYPE_ULONG,  sizeof(long),          &lSid,
															  	 UT_TYPE_STRING, sizeof(caSubmenu)-1,   caSubmenu,
																	 UT_TYPE_ULONG,  sizeof(long),          &lSystime,
																	 UT_TYPE_ULONG,  sizeof(long),          &lPid,
																	 UT_TYPE_STRING, sizeof(caMainmenu)-1,  caMainmenu,
																	 UT_TYPE_ULONG,  sizeof(long),          &lFlag);
	  }
		//utPltShowDb(psDbHead);
	  pasDbCloseCursor(psCur);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncm_websubmenu_list.htm");
	  return 0;	
}

