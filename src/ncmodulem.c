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
#include <dirent.h>
#include "utoall.h"
#include "pasutl.h"
#include "pasdb.h"
#include "pasutl.h"
#include "utoplt01.h"
#include "ncportal.h"
#include "ncportalweb.h"
#include <iconv.h>



int ncmModule_upload(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead)
{
    char *pConfig;
    char *pFile;
    pasConfList *psConfig;
    utPltDbHead *psDbHead;
    int iReturn;
    psDbHead = utPltInitDb();
    pConfig = (char *)pasGetConfigFileName();
    psConfig = (pasConfList *)pasUtlReadConfig(pConfig);
    if(psConfig == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0880自动备份设定"),ncLang("0427配置文件出错"));
        return 0;
    }

 		utPltPutVar(psDbHead,"StartChkDsclt",pasUtlLookConfig(psConfig,"StartChkDsclt","No"));
    pasUtlFreeConfig(psConfig);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncmWebshow_module.htm");
    return 0;
}
//指定目录下替换脚本
static int ncmExecSrcipt(char tarPath[64],char compname[32],char mname[32],char subname[32],char tlName[32])
{
    char execchar[256]="";	
	  system("rm /home/ncmysql/ncsrv/pexec");
	  system("touch /home/ncmysql/ncsrv/pexec");
	  system("echo \\#\\!/bin/sh > /home/ncmysql/ncsrv/pexec");         //中文支持
	  sprintf(execchar,"echo cd %s >> /home/ncmysql/ncsrv/pexec",tarPath);
	  system(execchar);
	  memset(execchar,0,sizeof(execchar));                                                                                         //   portalImages\/%s  
	  //echo 'grep portal_images * -R | awk -F: '{print $1}' | sort | uniq | xargs sed -i 's/portal_images/images\/atest/g' '>> /home/ncmysql/ncm/pexec
	  sprintf(execchar,"echo -e \"grep portal_images * -R | awk -F: \'{print \\$1}\' | sort | uniq | xargs sed -i \'s/portal_images/images\\/portalImages\\/%s\\/%s\\/%s\\/%s/g\' \">> /home/ncmysql/ncsrv/pexec",compname,mname,tlName,subname);
	  //替换页面图片路径
	  system(execchar);
	  system("chmod 777 /home/ncmysql/ncsrv/pexec");
	  system("/home/ncmysql/ncsrv/pexec"); 	
    
    return 1;	
}
//拷贝图片到指定目录
static int ncmCpImgSrcipt(char imgName[64],char tmpexec[128],char imgpath[128])
{
    char tmp1[256]="",tmp2[256]="";	
	 
	  if (strstr(imgName,".png"))
		{
				        	memset(tmp1,0,sizeof(tmp1));
				        	memset(tmp2,0,sizeof(tmp2));
				        	sprintf(tmp1,"%s/%s",tmpexec,imgName);
				        	sprintf(tmp2,"yes | cp %s %s",tmp1,imgpath);
				        	system(tmp2);
		}
		if (strstr(imgName,".gif"))
		{
				        	memset(tmp1,0,sizeof(tmp1));
				        	memset(tmp2,0,sizeof(tmp2));
				        	sprintf(tmp1,"%s/%s",tmpexec,imgName);
				        	sprintf(tmp2,"yes | cp %s %s",tmp1,imgpath);
				        	system(tmp2);
		}
		if (strstr(imgName,".jpg"))
		{
				        	memset(tmp1,0,sizeof(tmp1));
				        	memset(tmp2,0,sizeof(tmp2));
				        	sprintf(tmp1,"%s/%s",tmpexec,imgName);
				        	sprintf(tmp2,"yes | cp %s %s",tmp1,imgpath);
				        	system(tmp2);
		}
	 
    return 1;	
}
/*
*功能生成图片存放的终端类型和语言文件夹同时生成验证类型文件夹
*temname：厂商名
*authtype：验证类型
*caCompany：厂商
*mname:模板名称
*/
static int ncmCreateTemLangExec(char temname[64],char authtype[16],char caCompany[64],char mname[64])
{
    char imgpath[256]="";                                             //厂商名   模板名  终端类型
    sprintf(imgpath,"mkdir %s/images/portalImages/%s/%s/%s",MODULEPATH,caCompany,mname,temname);
	  system(imgpath);
	  memset(imgpath,0,sizeof(imgpath));                                                           //"login_main"
	  sprintf(imgpath,"mkdir %s/images/portalImages/%s/%s/%s/%s",MODULEPATH,caCompany,mname,temname,authtype);
//	  printf("******模板语言执行2 imgpath=%s  \n",imgpath);
	  system(imgpath);
	  return 1;
}
//根据终端类型 语言生成图片文件夹 
static char *ncmCreateTemLangSrcipt(char temtype[64],char lanflags[8],char caCompany[64],char mname[64],char authtype[16],char *temlanName)
{
    char tmp1[256]="";	
	  if (strcmp(temtype,"PC") == 0)
	  {
	  		if (strcmp(lanflags,"中文") == 0)
			  {
			  	  ncmCreateTemLangExec("pc",authtype,caCompany,mname); 
					  sprintf(tmp1,"%s/images/portalImages/%s/%s/pc/%s",MODULEPATH,caCompany,mname,authtype);
	  				strcpy(temlanName,"pc");			
			  }
			  else            //英文
			  {
			  	 ncmCreateTemLangExec("pe",authtype,caCompany,mname);  
			  	 sprintf(tmp1,"%s/images/portalImages/%s/%s/pe/%s",MODULEPATH,caCompany,mname,authtype);
			  	 strcpy(temlanName,"pe");			
			  }	
	  }
	  else if (strcmp(temtype,"平板") == 0)
	  {
	  		if (strcmp(lanflags,"中文") == 0)
			  {
			  		ncmCreateTemLangExec("pad",authtype,caCompany,mname);  
			  		sprintf(tmp1,"%s/images/portalImages/%s/%s/pad/%s",MODULEPATH,caCompany,mname,authtype);	
			  		strcpy(temlanName,"pad");			
			  }
			  else            
			  {
			  		ncmCreateTemLangExec("padeng",authtype,caCompany,mname);  
			  		sprintf(tmp1,"%s/images/portalImages/%s/%s/padeng/%s",MODULEPATH,caCompany,mname,authtype);		  
			  		strcpy(temlanName,"padeng");			
			  }	
	  }
	  else if (strcmp(temtype,"手机") == 0)
	  {
	  		if (strcmp(lanflags,"中文") == 0)
			  {
			  		ncmCreateTemLangExec("sms",authtype,caCompany,mname);  
			  		sprintf(tmp1,"%s/images/portalImages/%s/%s/sms/%s",MODULEPATH,caCompany,mname,authtype);	
			  		strcpy(temlanName,"sms");					
			  }
			  else            
			  {
			  		ncmCreateTemLangExec("smseng",authtype,caCompany,mname);  
			  		sprintf(tmp1,"%s/images/portalImages/%s/%s/smseng/%s",MODULEPATH,caCompany,mname,authtype);	
			  		strcpy(temlanName,"smseng");				  
			  }	
	  }
	  
    return tmp1;	
}
//拷贝到目标目录并改名
static int ncmModifyStandardRe(char srcPath[256],char srcName[64],char endPath[256],char mname[64])
{
    char tmp2char[512]=""; 
						  		 
		sprintf(tmp2char,"mv %s/%s %s/%s",srcPath,srcName,endPath,mname);    
		printf("tmp2char=%s \n",tmp2char);
		system(tmp2char);
		//执行替换脚本用
		memset(tmp2char,0,sizeof(tmp2char));
		sprintf(tmp2char,"cp %s/%s %s/%s",endPath,mname,endPath,"login_test.htm"); 
		system(tmp2char);
		return 1;
}
//制作模板
int ncmModuleCreateDef(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{     
	 pasDbCursor *psCur;
   char caTemp[400];
   FILE *fp;
   int i,iNum,iReturn;
   long lSid,lCount,lFlags;
   utPltDbHead *psDbHead;
 	 char caFile[256];
 	 char imp_file[128]="";
 	 char temp_file[128]="",filename[64]="";
 	 char sqlbuf[1024]="";
 	 int  m=0,n=0,findflag=0,lcou=0,maxmid=0;
 	 char caLocal[128],caRemote[128],caType[128];
 	 char caText[256],caBuf[16002];
   unsigned groupid=0,imgflag=0;
   char groupname[20],ap_groupid[30]="",tmpPath[128]="",lanflags[12]="",temtype[12]="",tmpexec[256]="",tmp2char[128]="";
   char tmp1[96]="",tmp2[96]="",subfname[64]="",fname[64]="",mtype[28]="",endname[64]="",mname[64]="";
 	 char *p,p1;
 	 DIR *dp;
 	 struct dirent *dirp;
 	 
 	 char caUsername[32] = "",caCompany[64] = "",htmlpath[128] = "",imgpath[128] = "",caTemp_d[256] = "";
 	 char *tlName;
 	 
#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
#endif
	 psDbHead = utPltInitDb();       
	 
	 dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername); 
	 printf("caUsername=%s \n",caUsername);
	 if (strlen(caUsername) > 0)
	 {
	 		sprintf(caTemp,"select company from dsuser where dispname = '%s' ",caUsername);
	    pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(caCompany)-1,caCompany);      	
	 }
	 //厂商名称获取      
   if(!utFileIsExist(MODULEUPPATH)) {
        if(mkdir(MODULEUPPATH,555)!=0){
	        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","导入","打开文件出错");
	        return 0;
        }
	 }
	
	 sprintf(tmpPath,"chmod +777 %s",MODULEUPPATH);
	 system(tmpPath);
	 utMsgPrintMsg(psMsgHead);
	 utMsgGetSomeNVar(psMsgHead,6,"filefield",UT_TYPE_STRING,  sizeof(imp_file)-1,   imp_file,
	                              "groupid",  UT_TYPE_STRING,  sizeof(ap_groupid)-1, ap_groupid,
	                              "lanflags", UT_TYPE_STRING,  sizeof(lanflags)-1,   lanflags,
	                              "mtype",    UT_TYPE_STRING,  sizeof(mtype)-1,      mtype,
	                              "mname",    UT_TYPE_STRING,  sizeof(mtype)-1,      mname,
	                              "temtype",  UT_TYPE_STRING,  sizeof(temtype)-1,    temtype); 
                       
	 //验证重复  根据操作员所属厂商，模板类型，模板名称   
//	 sprintf(caTemp,"select count(*) from ncmodule where m_comp='%s' and m_type='%s' and m_name='%s' ",caCompany,mtype,mname);
	 
	 sprintf(caTemp,"select count(*) from ncmodule where m_lan='%s' and m_type='%s' and m_name='%s' ",lanflags,mtype,mname);
	 
	 
   printf("caTemp=%s  \n",caTemp);
   pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lcou);                            
	 if (lcou == 0)
	 {
	 	  sprintf(caTemp,"select max(mid)+1 from ncmodule");
	 	  printf("lenmid=%d \n",sizeof(maxmid));
  	  pasDbOneRecord(caTemp,0,UT_TYPE_LONG,sizeof(maxmid),&maxmid);   
  	  if (maxmid == 0)
  	  {
  	  	maxmid = 1;	
  	  }
	 	  //若不存在 插入数据库   并执行存放策略      用户管理更新后，此处如何同步
	 		iReturn = pasDbExecSqlF("insert into ncmodule(mid,m_name,m_type,m_temtype,m_lan,m_comp,m_systime) values(%lu,'%s','%s','%s','%s','%s',%lu)",maxmid,mname,mtype,temtype,lanflags,caCompany,time(0));	
	    
	    //加一个主键  用户模板定义增量查询显示
	    
	 }   
	 else          //若存在替换原来的
	 {
	 		//执行存放策略	
	 		
	 }                              
   printf("imp_file=%s\n",imp_file);                    
   if (strlen(imp_file) != 0)
   {
	  	p = utStrSkipSpaces(imp_file);
	  	p = utStrGetWord(p,filename,200,";\n");	//取文件名
	  	p = utStrGetWord(p,temp_file,200,";\n");
	  	if ((*p) == ';')
	  		p = utStrGetWord(p+1,temp_file,200,";\n");     //取带路径的文件名
	  	
	  	printf("  temp_file=%s  filename=%s\n",temp_file,filename);               
	  	
	  	//创建文件夹
	  	memset(tmpexec,0,sizeof(tmpexec));
	  	sprintf(tmpexec,"mkdir %s/pmodule",MODULEUPPATH);
	  	system(tmpexec);
	
	  	//文件夹加权限
	  	memset(tmpexec,0,sizeof(tmpexec));
	  	sprintf(tmpexec,"%s/pmodule",MODULEUPPATH);
	  	sprintf(tmp2char,"chmod 777  %s",tmpexec);
	  	system(tmp2char);
	  	
	  	//拷贝上传文件至临时处理文件夹
	  	memset(tmp2char,0,sizeof(tmp2char));
	  	sprintf(tmp2char,"mv %s %s",temp_file,tmpexec);
	  	printf("cp tmp2char=%s \n",tmp2char);
	  	system(tmp2char);
	  	strcpy(fname,filename);
	  	printf("filename=%s fname=%s\n",filename,fname);
	  	p1=strtok(fname,".");
	    if(p1) 
	    {
	    	 strcpy(subfname,fname);	
	    }
	    
	  	printf("subfname=%s fname=%s\n",subfname,fname);
	  	//解压强制覆盖
	  	memset(tmpexec,0,sizeof(tmpexec));
	  	sprintf(tmpexec,"unzip -xo %s/pmodule/%s -d %s/pmodule",MODULEUPPATH,filename,MODULEUPPATH);      //解压到当前目录
	  	printf(" tmpexec=%s\n",tmpexec);
	  	system(tmpexec);
	  	//读取指定目录下文件
	  	memset(tmpexec,0,sizeof(tmpexec));
	  	sprintf(tmpexec,"%s/pmodule",MODULEUPPATH);
	  	printf("tmpexec=%s \n",tmpexec);
	  	
	  	if((dp=opendir(tmpexec)) == NULL)
	        printf("can't open %s", tmpexec);
	  	while(((dirp = readdir(dp)) != NULL) && (n <= 500)){
	//        if(n % 1 == 0) printf("\n");
	        n++;
	        printf("%10s \n", dirp->d_name);
	        //查找是否有html文件，若无跳出，添加解压目录，在打开    若有直接 按规则拷贝指定目录。
	        if (strstr(dirp->d_name,".htm"))
	        {
	        	findflag=1;	
	        }
	        //根目录找到图片  否则在portal_images下 拷贝时路径变更
	        if (strstr(dirp->d_name,".jpg") || strstr(dirp->d_name,".png") || strstr(dirp->d_name,".gif"))
	        {
	        	imgflag=1;	
	        }
	    }	
	  	closedir(dp); 
	  	printf("findflag=%d imgflag=%d \n",findflag,imgflag);
	  	
	  	if (strlen(mname) > 0)
	  	{	
	  		//创建模板图片文件夹                                         厂商      模板名
	  		if (strcmp(mtype,"认证页面模板") == 0)
	  		{
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s",MODULEPATH,caCompany);
	  			printf("imgpath=%s  \n",imgpath);
	  			system(imgpath);
	  			
	  			memset(imgpath,0,sizeof(imgpath));
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s/%s",MODULEPATH,caCompany,mname);
	  			printf("11 imgpath=%s  \n",imgpath);
	  			system(imgpath);
//	  			
	  			memset(imgpath,0,sizeof(imgpath));
	  			//终端类型判断  语言判断  中文
	  			strcpy(imgpath,ncmCreateTemLangSrcipt(temtype,lanflags,caCompany,mname,"login_main",tlName));
	  			printf("****** tlName=%s \n",tlName);
	  			
	  		}
	  		else if (strcmp(mtype,"认证通过页面模板") == 0)
	  		{
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s",MODULEPATH,caCompany);
	  			system(imgpath);
	  			memset(imgpath,0,sizeof(imgpath));
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s/%s",MODULEPATH,caCompany,mname);
	  			system(imgpath);

	  			memset(imgpath,0,sizeof(imgpath));
	  			strcpy(imgpath,ncmCreateTemLangSrcipt(temtype,lanflags,caCompany,mname,"login_ok",tlName));
	  			printf("*********imgpath=%s \n",imgpath);
	  		}	
	  		else if (strcmp(mtype,"认证退出页面模板") == 0)
	  		{ 
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s",MODULEPATH,caCompany);
	  			system(imgpath);
	  			memset(imgpath,0,sizeof(imgpath));
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s/%s",MODULEPATH,caCompany,mname);
	  			system(imgpath);
	  			memset(imgpath,0,sizeof(imgpath));
	  			strcpy(imgpath,ncmCreateTemLangSrcipt(temtype,lanflags,caCompany,mname,"logout_ok",tlName));
	  		}	
	  		else 
	  		{ 
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s",MODULEPATH,caCompany);
	  			system(imgpath);
	  			memset(imgpath,0,sizeof(imgpath));
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s/%s",MODULEPATH,caCompany,mname);
	  			system(imgpath);
	  			memset(imgpath,0,sizeof(imgpath));
	  			strcpy(imgpath,ncmCreateTemLangSrcipt(temtype,lanflags,caCompany,mname,"login_main",tlName));
	  		}	
	  		
	  		//创建页面模板文件夹                                      //厂商     模板名
	  		sprintf(htmlpath,"mkdir %s/ncmportal/%s",MODULEPATH,caCompany);
	  		system(htmlpath);
	  		memset(htmlpath,0,sizeof(htmlpath));
	  		sprintf(htmlpath,"mkdir %s/ncmportal/%s/%s",MODULEPATH,caCompany,mname);
	  		system(htmlpath);
	  		memset(htmlpath,0,sizeof(htmlpath));
	  		sprintf(htmlpath,"mkdir %s/ncmportal/%s/%s/%s",MODULEPATH,caCompany,mname,tlName);
	  		printf("3 htmlpath=%s  \n",htmlpath);
	  		system(htmlpath);
	  		
	  		memset(htmlpath,0,sizeof(htmlpath));
	  		sprintf(htmlpath,"%s/ncmportal/%s/%s/%s",MODULEPATH,caCompany,mname,tlName);
	  	}
	  	
	  	n=0;
	  	//页面操作
	  	if (findflag == 1){
	  		 //????
	  		  if((dp=opendir(tmpexec)) == NULL)
	        printf("can't open %s", tmpexec);
	        while(((dirp = readdir(dp)) != NULL) && (n <= 500)){
			        if (strstr(dirp->d_name,".htm"))
			        {
			        	memset(tmp1,0,sizeof(tmp1));
			        	memset(tmp2,0,sizeof(tmp2));
			        	//拷贝页面到指定目录
			        	sprintf(tmp1,"%s/%s",tmpexec,dirp->d_name);
			        	sprintf(tmp2,"cp %s %s",tmp1,htmlpath); 
			        	printf("tmp2=%s \n",tmp2);
			        	system(tmp2);
			        
						  	if ((strlen(mtype) > 0) && (strcmp(mtype,"认证页面模板") == 0))
						  	{
							  	 ncmModifyStandardRe(tmpexec,dirp->d_name,htmlpath,"login_main.htm");
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"login_main",tlName);
							  	 strcpy(endname,"login_main.htm");
						  	}
						  	else if ((strlen(mtype) > 0) && (strcmp(mtype,"认证通过页面模板") == 0))
						  	{
							  	 ncmModifyStandardRe(tmpexec,dirp->d_name,htmlpath,"login_ok.htm");
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"login_ok",tlName);
							  	 strcpy(endname,"login_ok.htm");
						  	}
						  	else if ((strlen(mtype) > 0) && (strcmp(mtype,"认证退出页面模板") == 0))
						  	{  
							  	 ncmModifyStandardRe(tmpexec,dirp->d_name,htmlpath,"logout_ok.htm");
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"logout_ok",tlName);
							  	 strcpy(endname,"logout_ok.htm");
						  	}

			        	else              //默认为认证界面(全套同一文件夹无法区分)
			        	{
						  		 ncmModifyStandardRe(tmpexec,dirp->d_name,htmlpath,"login_main.htm");
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"login_main",tlName);
							  	 strcpy(endname,"login_main.htm");
						  	}
			        }
			        if (imgflag == 1)       //根目录找到
			       	{
			       		ncmCpImgSrcipt(dirp->d_name,tmpexec,imgpath);
			      	}
						  n++;
			    }
			  	closedir(dp);  	
	    }
	    else
	    {
	    	  //添加解压文件夹目录
	    	  sprintf(tmpexec,"%s/pmodule/%s",MODULEUPPATH,subfname);
	    		if((dp=opendir(tmpexec)) == NULL)
	        	printf("can't open %s", tmpexec);
	        n=0;	
			  	while(((dirp = readdir(dp)) != NULL) && (n <= 500)){
			        if (strstr(dirp->d_name,".htm"))
			        {
			        	memset(tmp1,0,sizeof(tmp1));
			        	memset(tmp2,0,sizeof(tmp2));
			        	//拷贝页面到指定目录       必须拷贝，因为目录下有2个含有替换字符串的页面才会替换。
			        	sprintf(tmp1,"%s/%s",tmpexec,dirp->d_name);
			        	sprintf(tmp2,"cp %s %s",tmp1,htmlpath); 
			        	printf("tmp2=%s \n",tmp2);
			        	system(tmp2);
			        
						  	if ((strlen(mtype) > 0) && (strcmp(mtype,"认证页面模板") == 0))
						  	{
							  	 ncmModifyStandardRe(tmpexec,dirp->d_name,htmlpath,"login_main.htm");
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"login_main",tlName);
							  	 strcpy(endname,"login_main.htm");
						  	}
						  	else if ((strlen(mtype) > 0) && (strcmp(mtype,"认证通过页面模板") == 0))
						  	{
							  	 ncmModifyStandardRe(tmpexec,dirp->d_name,htmlpath,"login_ok.htm");
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"login_ok",tlName);
							  	 strcpy(endname,"login_ok.htm");
						  	}
						  	else if ((strlen(mtype) > 0) && (strcmp(mtype,"认证退出成功页面模板") == 0))
						  	{  
							  	 ncmModifyStandardRe(tmpexec,dirp->d_name,htmlpath,"logout_ok.htm");
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"logout_ok",tlName);
							  	 strcpy(endname,"logout_ok.htm");
						  	}
			        	else              //默认为认证界面(全套同一文件夹无法区分)
			        	{
							  	 ncmModifyStandardRe(tmpexec,dirp->d_name,htmlpath,"login_main.htm");
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"login_main",tlName);
							  	 strcpy(endname,"login_main.htm");
						  	}
			        }
			        //解压子目录 如果找到flag=2  
			      	if (strstr(dirp->d_name,".jpg") || strstr(dirp->d_name,".png") || strstr(dirp->d_name,".gif"))
			        {
			        	imgflag=2;	
			        }
			        n++;
			    }
			  	closedir(dp);  	
	    }
	    printf("2 imgflag=%d \n",imgflag);
      if (imgflag == 2) //html目录下找到
			{
				  sprintf(tmpexec,"%s/pmodule/%s",MODULEUPPATH,subfname);
	    		if((dp=opendir(tmpexec)) == NULL)
	        	printf("can't open %s", tmpexec);
	        n=0;
	        while(((dirp = readdir(dp)) != NULL) && (n <= 500)){	
					  ncmCpImgSrcipt(dirp->d_name,tmpexec,imgpath);
					  n++;
					}
				  closedir(dp);  	
			}
			else    //根目录和html子目录都没有
			{
				  sprintf(tmpexec,"%s/pmodule/%s/portal_images",MODULEUPPATH,subfname);
				  printf("2222 tmpexec=%s  imgpath=%s\n",tmpexec,imgpath);
	    		if((dp=opendir(tmpexec)) == NULL)
	        	printf("can't open %s \n", tmpexec);
	        n=0;
			    while(((dirp = readdir(dp)) != NULL) && (n <= 500)){
				    ncmCpImgSrcipt(dirp->d_name,tmpexec,imgpath);
					  n++;
					}
				  closedir(dp);  	
			} 
			 
	  	//最后要删除upload下的临时文件夹，否则新修改的不会覆盖现有的。
	  	memset(tmp2char,0,sizeof(tmp2char));
	  	sprintf(tmp2char,"rm -fr %s/pmodule",MODULEUPPATH);
	  	printf("tmp2char=%s \n",tmp2char);
	  	system(tmp2char);
	  	
	  	//替换html文件的图片路径，拷贝html到指定目录(一定规则(操作员的权限)生成的文件夹)     pe 等通过模板名称区分，模板名称即文件夹名	  	

	  	//http.conf 配置一个相对路径
	 }
   utPltPutVar(psDbHead,"fname",temp_file);
   //中文转码成utf-8
   memset(htmlpath,0,sizeof(htmlpath));
   sprintf(htmlpath,"/ncmportal/%s/%s/",caCompany,mname);
   utPltPutVarF(psDbHead,"module_result","%s%s/%s",htmlpath,tlName,endname);
   
   utPltPutVar(psDbHead,"module_company",caCompany);
   utPltPutVar(psDbHead,"module_temtype",tlName);
   
   char htmlname[256]="";
   strcpy(htmlname,htmlpath);
   strcat(htmlname,tlName);
   strcat(htmlname,"/");
   strcat(htmlname,endname);
   sprintf(caTemp,"select count(*) from ncmodule where m_lan='%s' and m_type='%s' and m_name='%s' ",lanflags,mtype,mname);
   pasDbOneRecord(caTemp,0,UT_TYPE_LONG,sizeof(long),&lcou);    
   printf("lcou=%d  htmlname=%s\n",lcou,htmlname);
   if (lcou > 0)
   {
   		iReturn = pasDbExecSqlF("update ncmodule set htmlpath = '%s' where   m_comp='%s' and m_type='%s' and m_name='%s' ",htmlname,caCompany,mtype,mname);
   }
      
   utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_module_result2.htm");
   return 0;
}

//指定目录下替换脚本
static int ncmExecSrciptNew(char tarPath[64],char platenum[32])
{
    char execchar[256]="";	
	  system("rm /home/ncmysql/ncsrv/pexec");
	  system("touch /home/ncmysql/ncsrv/pexec");
	  system("echo \\#\\!/bin/sh > /home/ncmysql/ncsrv/pexec");         //中文支持
	  sprintf(execchar,"echo cd %s >> /home/ncmysql/ncsrv/pexec",tarPath);
	  system(execchar);
	  memset(execchar,0,sizeof(execchar));                                                                                         //   portalImages\/%s  
	  //echo 'grep portal_images * -R | awk -F: '{print $1}' | sort | uniq | xargs sed -i 's/portal_images/images\/atest/g' '>> /home/ncmysql/ncm/pexec
	  sprintf(execchar,"echo -e \"grep portal_images * -R | awk -F: \'{print \\$1}\' | sort | uniq | xargs sed -i \'s/portal_images/images\\/portalImages\\/%s/g\' \">> /home/ncmysql/ncsrv/pexec",platenum);
	  //替换页面图片路径
	  system(execchar);
	  system("chmod 777 /home/ncmysql/ncsrv/pexec");
	  system("/home/ncmysql/ncsrv/pexec"); 	
    
    return 1;	
}
//模板预览
int ncmWebReplaceOne(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
   	char caTemp[1024];
    utPltDbHead *psDbHead;
   	int iReturn,i;   
    char caPlate[128],module_result[128],platepage[128];
    char caDel[16];

    utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,3,"platepath", UT_TYPE_STRING,128, module_result
	                                        ,"resultpath",UT_TYPE_STRING,100, caPlate
	                                        ,"platepage", UT_TYPE_STRING,128, platepage);

    psDbHead = utPltInitDb();
    
    utPltPutVarF(psDbHead,"module_result","%s",module_result);
    
    if(strlen(platepage) > 10)
    {
    	utPltOutToHtml(iFd,psMsgHead,psDbHead,platepage);
    }
    else
    {
    	if(strlen(caPlate) > 0)
	    {
	       utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
	    }
	    else
	    {
	    	 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_group_checktree.htm");
	    }
    }	
    
    return 0;
}

//制作模板 new
int ncmModuleCreate(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{     
	 pasDbCursor *psCur;
   char caTemp[400];
   FILE *fp;
   int i,iNum,iReturn;
   long lSid,lCount,lFlags;
   utPltDbHead *psDbHead;
 	 char caFile[256];
 	 char imp_file[128]="";
 	 char temp_file[128]="",filename[64]="";
 	 char sqlbuf[1024]="";
 	 int  m=0,n=0,findflag=0,lcou=0,maxmid=0;
 	 char caLocal[128],caRemote[128],caType[128];
 	 char caText[256],caBuf[16002];
   unsigned groupid=0,imgflag=0;
   char groupname[20],ap_groupid[30]="",tmpPath[128]="",lanflags[12]="",temtype[12]="",tmpexec[256]="",tmp2char[128]="";
   char tmp1[96]="",tmp2[96]="",subfname[64]="",fname[64]="",mtype[28]="",endname[64]="",mname[64]="";
 	 char *p,p1;
 	 DIR *dp;
 	 struct dirent *dirp;
 	 
 	 char caUsername[32] = "",caCompany[64] = "",htmlpath[128] = "",imgpath[128] = "",caTemp_d[256] = "";
 	 char *tlName;
 	 
 	 char platepath[256] = "" ,pagemesg[1024] = "" , imagePath[256] = "" , platenum[12] = "";
 	 
 	 int htmlflag1 = 0 ,htmlflag2 = 0 , htmlflag3 = 0,htmlcou = 0;
 	 char cleartmp[256] = "";
#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
#endif
	 psDbHead = utPltInitDb();       
	 
	 dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername); 
	 printf("caUsername=%s \n",caUsername);
	 if (strlen(caUsername) > 0)
	 {
	 		sprintf(caTemp,"select company from dsuser where dispname = '%s' ",caUsername);
	    pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(caCompany)-1,caCompany);      	
	 }
	 //厂商名称获取      
   if(!utFileIsExist(MODULEUPPATH)) {
        if(mkdir(MODULEUPPATH,555)!=0){
	        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","导入","打开文件出错");
	        return 0;
        }
	 }
	
	 sprintf(tmpPath,"chmod +777 %s",MODULEUPPATH);
	 system(tmpPath);
	 utMsgPrintMsg(psMsgHead);
	 utMsgGetSomeNVar(psMsgHead,7,"filefield",UT_TYPE_STRING,  sizeof(imp_file)-1,   imp_file,
	                              "groupid",  UT_TYPE_STRING,  sizeof(ap_groupid)-1, ap_groupid,
	                              "lanflags", UT_TYPE_STRING,  sizeof(lanflags)-1,   lanflags,
	                              "mtype",    UT_TYPE_STRING,  sizeof(mtype)-1,      mtype,
	                              "mname",    UT_TYPE_STRING,  sizeof(mtype)-1,      mname,
	                              "pagemesg", UT_TYPE_STRING,  sizeof(mtype)-1,      pagemesg,
	                              "temtype",  UT_TYPE_STRING,  sizeof(temtype)-1,    temtype); 
                       
	 //验证重复  根据操作员所属厂商，模板类型，模板名称   
//	 sprintf(caTemp,"select count(*) from ncmodule where  m_name='%s' ",mname);
	 sprintf(caTemp,"select count(*) from ncmodule where m_lan='%s' and m_temtype='%s' and m_name='%s' ",lanflags,temtype,mname);
//	 sprintf(caTemp,"select count(*) from ncmodule where m_lan='%s' and m_type='%s' and m_name='%s' ",lanflags,mtype,mname);
	 
	 
   printf("caTemp=%s  \n",caTemp);
   pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lcou);                            
	 if (lcou == 0)
	 {
	 	  sprintf(caTemp,"select max(mid)+1 from ncmodule");
	 	  printf("lenmid=%d \n",sizeof(maxmid));
  	  pasDbOneRecord(caTemp,0,UT_TYPE_LONG,sizeof(maxmid),&maxmid);   
  	  if (maxmid == 0)
  	  {
  	  	maxmid = 1;	
  	  }
  	  sprintf(platepath,"%s/ncmportal/%08d/",MODULEAPPPATH,maxmid);
  	  sprintf(platenum,"%08d",maxmid);
  	  printf("platepath=%s platenum=%s\n",platepath,platenum);
	 	  //若不存在 插入数据库   并执行存放策略      用户管理更新后，此处如何同步
	 		iReturn = pasDbExecSqlF("insert into ncmodule(mid,m_name,m_temtype,m_lan,platepath,mesg,m_systime) values(%lu,'%s','%s','%s','%s','%s',%lu)",maxmid,mname,temtype,lanflags,platepath,pagemesg,time(0));	
	    
	    //加一个主键  用户模板定义增量查询显示
	    
	 }   
	 else          //若存在替换原来的
	 {
	 		//执行存放策略	
	 		sprintf(caTemp,"select max(mid)+1 from ncmodule");
	 	  printf("lenmid=%d \n",sizeof(maxmid));
  	  pasDbOneRecord(caTemp,0,UT_TYPE_LONG,sizeof(maxmid),&maxmid);   
  	  if (maxmid == 0)
  	  {
  	  	maxmid = 1;	
  	  }
  	  sprintf(platepath,"%s/ncmportal/%08d/",MODULEAPPPATH,maxmid);
  	  sprintf(platenum,"%08d",maxmid);
  	  printf("platepath=%s platenum=%s\n",platepath,platenum);
	 }                              
   printf("imp_file=%s\n",imp_file);                    
   if (strlen(imp_file) != 0)
   {
	  	p = utStrSkipSpaces(imp_file);
	  	p = utStrGetWord(p,filename,200,";\n");	//取文件名
	  	p = utStrGetWord(p,temp_file,200,";\n");
	  	if ((*p) == ';')
	  		p = utStrGetWord(p+1,temp_file,200,";\n");     //取带路径的文件名
	  	
	  	printf("  temp_file=%s  filename=%s\n",temp_file,filename);               
	  	
	  	//创建文件夹
	  	memset(tmpexec,0,sizeof(tmpexec));
	  	sprintf(tmpexec,"mkdir %s/pmodule",MODULEUPPATH);
	  	system(tmpexec);
	
	  	//文件夹加权限
	  	memset(tmpexec,0,sizeof(tmpexec));
	  	sprintf(tmpexec,"%s/pmodule",MODULEUPPATH);
	  	sprintf(tmp2char,"chmod 777  %s",tmpexec);
	  	system(tmp2char);
	  	
	  	//拷贝上传文件至临时处理文件夹
	  	memset(tmp2char,0,sizeof(tmp2char));
	  	sprintf(tmp2char,"mv %s %s",temp_file,tmpexec);
	  	printf("cp tmp2char=%s \n",tmp2char);
	  	system(tmp2char);
	  	strcpy(fname,filename);
	  	printf("filename=%s fname=%s\n",filename,fname);
	  	p1=strtok(fname,".");
	    if(p1) 
	    {
	    	 strcpy(subfname,fname);	
	    }
	    
	  	printf("subfname=%s fname=%s\n",subfname,fname);
	  	//解压强制覆盖
	  	memset(tmpexec,0,sizeof(tmpexec));
	  	sprintf(tmpexec,"unzip -xo %s/pmodule/%s -d %s/pmodule",MODULEUPPATH,filename,MODULEUPPATH);      //解压到当前目录
	  	printf(" tmpexec=%s\n",tmpexec);
	  	system(tmpexec);
	  	//读取指定目录下文件
	  	memset(tmpexec,0,sizeof(tmpexec));
	  	sprintf(tmpexec,"%s/pmodule",MODULEUPPATH);
	  	printf("tmpexec=%s \n",tmpexec);
	  	
	  	if((dp=opendir(tmpexec)) == NULL)
	        printf("can't open %s", tmpexec);
	  	while(((dirp = readdir(dp)) != NULL) && (n <= 500)){
	//        if(n % 1 == 0) printf("\n");
	        n++;
	        printf("******** %10s \n", dirp->d_name);
	        //查找是否有html文件，若无跳出，添加解压目录，在打开    若有直接 按规则拷贝指定目录。
	       
	        if (strstr(dirp->d_name,".htm"))
	        {
	        	findflag=1;	
	        }
	        
	        
	        //根目录找到图片  否则在portal_images下 拷贝时路径变更
	        if (strstr(dirp->d_name,".jpg") || strstr(dirp->d_name,".png") || strstr(dirp->d_name,".gif"))
	        {
	        	imgflag=1;	
	        }
	    }
	  	closedir(dp); 
	  		
	  	
	  	
	  	
	  	printf("findflag=%d imgflag=%d \n",findflag,imgflag);
	  	//2 创建plate下文件夹
	  	
	  	if (strlen(mname) > 0)
	  	{	
	  		/*
	  		//创建模板图片文件夹                                         厂商      模板名
	  		if (strcmp(mtype,"认证页面模板") == 0)
	  		{
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s",MODULEPATH,caCompany);
	  			printf("imgpath=%s  \n",imgpath);
	  			system(imgpath);
	  			
	  			memset(imgpath,0,sizeof(imgpath));
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s/%s",MODULEPATH,caCompany,mname);
	  			printf("11 imgpath=%s  \n",imgpath);
	  			system(imgpath);
//	  			
	  			memset(imgpath,0,sizeof(imgpath));
	  			//终端类型判断  语言判断  中文
	  			strcpy(imgpath,ncmCreateTemLangSrcipt(temtype,lanflags,caCompany,mname,"login_main",tlName));
	  			printf("****** tlName=%s \n",tlName);
	  			
	  		}
	  		else if (strcmp(mtype,"认证通过页面模板") == 0)
	  		{
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s",MODULEPATH,caCompany);
	  			system(imgpath);
	  			memset(imgpath,0,sizeof(imgpath));
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s/%s",MODULEPATH,caCompany,mname);
	  			system(imgpath);

	  			memset(imgpath,0,sizeof(imgpath));
	  			strcpy(imgpath,ncmCreateTemLangSrcipt(temtype,lanflags,caCompany,mname,"login_ok",tlName));
	  			printf("*********imgpath=%s \n",imgpath);
	  		}	
	  		else if (strcmp(mtype,"认证退出页面模板") == 0)
	  		{ 
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s",MODULEPATH,caCompany);
	  			system(imgpath);
	  			memset(imgpath,0,sizeof(imgpath));
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s/%s",MODULEPATH,caCompany,mname);
	  			system(imgpath);
	  			memset(imgpath,0,sizeof(imgpath));
	  			strcpy(imgpath,ncmCreateTemLangSrcipt(temtype,lanflags,caCompany,mname,"logout_ok",tlName));
	  		}	
	  		else 
	  		{ 
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s",MODULEPATH,caCompany);
	  			system(imgpath);
	  			memset(imgpath,0,sizeof(imgpath));
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s/%s",MODULEPATH,caCompany,mname);
	  			system(imgpath);
	  			memset(imgpath,0,sizeof(imgpath));
	  			strcpy(imgpath,ncmCreateTemLangSrcipt(temtype,lanflags,caCompany,mname,"login_main",tlName));
	  		}	
	  		
	  		//创建页面模板文件夹                                      //厂商     模板名
	  		sprintf(htmlpath,"mkdir %s/ncmportal/%s",MODULEPATH,caCompany);
	  		system(htmlpath);
	  		memset(htmlpath,0,sizeof(htmlpath));
	  		
	  		sprintf(htmlpath,"mkdir %s/ncmportal/%s/%s",MODULEPATH,caCompany,mname);
	  		system(htmlpath);
	  		memset(htmlpath,0,sizeof(htmlpath));
	  		sprintf(htmlpath,"mkdir %s/ncmportal/%s/%s/%s",MODULEPATH,caCompany,mname,tlName);
	  		printf("3 htmlpath=%s  \n",htmlpath);
	  		system(htmlpath);
	  		*/
	  		//删除以前的页面目录和图片目录
	  		sprintf(cleartmp,"rm -rf %s/ncmportal/%s",MODULEAPPPATH,platenum);
	      printf("~~~~~~~~~0~~~~~~cleartmp =%s  \n",cleartmp);
	      system(cleartmp);
	      
	      memset(cleartmp,0,sizeof(cleartmp));
	      sprintf(cleartmp,"rm -rf %s/images/portalImages/%s",MODULEPATH,platenum);
	      printf("~~~~~~~~~1~~~~~~cleartmp =%s  \n",cleartmp);
	      system(cleartmp);
	  		
	  		//创建image目录
	  		memset(imagePath,0,sizeof(imagePath));
	  		sprintf(imagePath,"mkdir %s/images/portalImages/%s",MODULEPATH,platenum);
	  		printf("3 imagePath=%s  \n",imagePath);
	  		system(imagePath);
	  		memset(imagePath,0,sizeof(imagePath));
	  		sprintf(imagePath,"chmod 755 %s/images/portalImages/%s",MODULEPATH,platenum);
	  		system(imagePath);
	  		//创建html目录
	  		memset(htmlpath,0,sizeof(htmlpath));
	  		sprintf(htmlpath,"mkdir %s/ncmportal/%s",MODULEAPPPATH,platenum);
	  		printf("3 htmlpath=%s  \n",htmlpath);
	  		system(htmlpath);
	  		memset(htmlpath,0,sizeof(htmlpath));
	  		sprintf(htmlpath,"chmod 755 %s/ncmportal/%s",MODULEAPPPATH,platenum);
	  		system(htmlpath);
	  		
	  		memset(imagePath,0,sizeof(imagePath));
	  		sprintf(imagePath,"%s/images/portalImages/%s",MODULEPATH,platenum);
	  		memset(htmlpath,0,sizeof(htmlpath));
	  		sprintf(htmlpath,"%s/ncmportal/%s",MODULEAPPPATH,platenum);
	  	}
	  	
	  	n=0;
	  	//页面操作
	  	if (findflag == 1){
	  		  if((dp=opendir(tmpexec)) == NULL)
	        printf("can't open %s", tmpexec);
	  		 
	        while(((dirp = readdir(dp)) != NULL) && (n <= 500)){
			        if (strstr(dirp->d_name,".htm"))
			        {
			        	memset(tmp1,0,sizeof(tmp1));
			        	memset(tmp2,0,sizeof(tmp2));
			        	//拷贝页面到指定目录
			        	sprintf(tmp1,"%s/%s",tmpexec,dirp->d_name);
			        	sprintf(tmp2,"cp %s %s",tmp1,htmlpath); 
			        	printf("tmp2=%s \n",tmp2);
			        	system(tmp2);
			        
			        
			          //替换界面里的图片路径
			          ncmExecSrciptNew(htmlpath,platenum); 
			        /*
						  	if ((strlen(mtype) > 0) && (strcmp(mtype,"认证页面模板") == 0))
						  	{
							  	 ncmModifyStandardRe(tmpexec,dirp->d_name,htmlpath,"login_main.htm");
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"login_main",tlName);
							  	 strcpy(endname,"login_main.htm");
						  	}
						  	else if ((strlen(mtype) > 0) && (strcmp(mtype,"认证通过页面模板") == 0))
						  	{
							  	 ncmModifyStandardRe(tmpexec,dirp->d_name,htmlpath,"login_ok.htm");
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"login_ok",tlName);
							  	 strcpy(endname,"login_ok.htm");
						  	}
						  	else if ((strlen(mtype) > 0) && (strcmp(mtype,"认证退出页面模板") == 0))
						  	{  
							  	 ncmModifyStandardRe(tmpexec,dirp->d_name,htmlpath,"logout_ok.htm");
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"logout_ok",tlName);
							  	 strcpy(endname,"logout_ok.htm");
						  	}
			        	else              //默认为认证界面(全套同一文件夹无法区分)
			        	{
						  		 ncmModifyStandardRe(tmpexec,dirp->d_name,htmlpath,"login_main.htm");
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"login_main",tlName);
							  	 strcpy(endname,"login_main.htm");
						  	}
						  	*/
						  	//zms
				        htmlcou++;
				       
				        if (strstr(dirp->d_name,"login_main.htm"))
				        {
				        	printf("******1111*******\n");
				        	htmlflag1 = 1;
				        }
				        if (strstr(dirp->d_name,"login_ok.htm"))
				        {
				        	htmlflag2 = 1;
				        }
				        if (strstr(dirp->d_name,"logout_ok.htm"))
				        {
				        	htmlflag3 = 1;
				        }
			        }
			        if (imgflag == 1)       //根目录找到
			       	{
			       		ncmCpImgSrcipt(dirp->d_name,tmpexec,imagePath);
			      	}
						  n++;
			    }
			  	closedir(dp);  	
	    }
	    else
	    {
	    	  //添加解压文件夹目录
	    	  sprintf(tmpexec,"%s/pmodule/%s",MODULEUPPATH,subfname);
	    		if((dp=opendir(tmpexec)) == NULL)
	        	printf("can't open %s", tmpexec);
	        n=0;	
	        
//	        sprintf(cleartmp,"rm -rf %s/",htmlpath);
//	        printf("~~~~~~~~~1~~~~~~cleartmp =%s  \n",cleartmp);
//	        system(cleartmp);
			  	while(((dirp = readdir(dp)) != NULL) && (n <= 500)){
			        if (strstr(dirp->d_name,".htm"))
			        {
			        	memset(tmp1,0,sizeof(tmp1));
			        	memset(tmp2,0,sizeof(tmp2));
			        	//拷贝页面到指定目录      不需要改名上面已做名字验证
			        	sprintf(tmp1,"%s/%s",tmpexec,dirp->d_name);
			        	sprintf(tmp2,"cp %s %s",tmp1,htmlpath); 
			        	printf("tmp2=%s \n",tmp2);
			        	system(tmp2);
			          
			          //替换界面里的图片路径
			          ncmExecSrciptNew(htmlpath,platenum);
			          
			          /*
						  	if ((strlen(mtype) > 0) && (strcmp(mtype,"认证页面模板") == 0))
						  	{
							  	 ncmModifyStandardRe(tmpexec,dirp->d_name,htmlpath,"login_main.htm");
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"login_main",tlName);
							  	 strcpy(endname,"login_main.htm");
						  	}
						  	else if ((strlen(mtype) > 0) && (strcmp(mtype,"认证通过页面模板") == 0))
						  	{
							  	 ncmModifyStandardRe(tmpexec,dirp->d_name,htmlpath,"login_ok.htm");
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"login_ok",tlName);
							  	 strcpy(endname,"login_ok.htm");
						  	}
						  	else if ((strlen(mtype) > 0) && (strcmp(mtype,"认证退出成功页面模板") == 0))
						  	{  
							  	 ncmModifyStandardRe(tmpexec,dirp->d_name,htmlpath,"logout_ok.htm");
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"logout_ok",tlName);
							  	 strcpy(endname,"logout_ok.htm");
						  	}
			        	else              //默认为认证界面(全套同一文件夹无法区分)
			        	{
							  	 ncmModifyStandardRe(tmpexec,dirp->d_name,htmlpath,"login_main.htm");
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"login_main",tlName);
							  	 strcpy(endname,"login_main.htm");
						  	}
						  	*/
						  	//zms
						  	//zms
				        htmlcou++;
				       printf("***htmlcou =%d \n",htmlcou);
				        if (strstr(dirp->d_name,"login_main.htm"))
				        {
				        	printf("******1111*******\n");
				        	htmlflag1 = 1;
				        }
				        if (strstr(dirp->d_name,"login_ok.htm"))
				        {
				        	htmlflag2 = 1;
				        }
				        if (strstr(dirp->d_name,"logout_ok.htm"))
				        {
				        	htmlflag3 = 1;
				        }
						  	
			        }
			        //解压子目录 如果找到flag=2  
			      	if (strstr(dirp->d_name,".jpg") || strstr(dirp->d_name,".png") || strstr(dirp->d_name,".gif"))
			        {
			        	imgflag=2;	
			        }
			        n++;
			    }
			  	closedir(dp);  	
	    }
	    
	    
	    
	    printf("2 imgflag=%d \n",imgflag);
      if (imgflag == 2) //html目录下找到
			{
				  sprintf(tmpexec,"%s/pmodule/%s",MODULEUPPATH,subfname);
	    		if((dp=opendir(tmpexec)) == NULL)
	        	printf("can't open %s", tmpexec);
	        n=0;
	        while(((dirp = readdir(dp)) != NULL) && (n <= 500)){	
					  ncmCpImgSrcipt(dirp->d_name,tmpexec,imagePath);
					  n++;
					}
				  closedir(dp);  	
			}
			else    //根目录和html子目录都没有
			{
				  sprintf(tmpexec,"%s/pmodule/%s/portal_images",MODULEUPPATH,subfname);
				  printf("2222 tmpexec=%s  imgpath=%s\n",tmpexec,imgpath);
	    		if((dp=opendir(tmpexec)) == NULL)
	        	printf("can't open %s \n", tmpexec);
	        n=0;
			    while(((dirp = readdir(dp)) != NULL) && (n <= 500)){
				    ncmCpImgSrcipt(dirp->d_name,tmpexec,imagePath);
					  n++;
					}
				  closedir(dp);  	
			} 
			
			//zip文件拷贝到指定目录  下载用
			memset(caTemp,0,sizeof(caTemp));
			sprintf(caTemp,"yes | cp %s/pmodule/%s %s/ncmportal/%s",MODULEUPPATH,filename,MODULEAPPPATH,platenum);
			printf(" &&&****caTemp  = %s \n",caTemp);
			system(caTemp);
			
			//最后要删除upload下的临时文件夹，否则新修改的不会覆盖现有的。
	  	memset(tmp2char,0,sizeof(tmp2char));
	  	sprintf(tmp2char,"rm -fr %s/pmodule",MODULEUPPATH);
	  	printf("tmp2char=%s \n",tmp2char);
	  	system(tmp2char);
			
			//zms
	    printf("htmlflag1=%d  htmlflag2=%d  htmlflag3=%d htmlcou=%d \n",htmlflag1,htmlflag2,htmlflag3,htmlcou);
	  	char tmpout[64] = "";
	  	if (htmlflag1 == 0)
	  	{
	  		  sprintf(tmpout,"%s 未上传","login_main.htm");	
	  		  utPltPutVar(psDbHead,"modu_status", tmpout);
	  		  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_module_result2.htm");
   				return 0; 
	  	}	
	  	if (htmlflag2 == 0)
	  	{
	  		  sprintf(tmpout,"%s 未上传","login_ok.htm");	
	  		  utPltPutVar(psDbHead,"modu_status", tmpout);
	  		  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_module_result2.htm");
   				return 0; 
	  	}	
	  	if (htmlflag3 == 0)
	  	{
	  		  sprintf(tmpout,"%s 未上传","logout_ok.htm");	
	  		  utPltPutVar(psDbHead,"modu_status", tmpout);
	  		  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_module_result2.htm");
   				return 0; 
	  	}
	  	
	  	if (htmlcou < 3)
	  	{
	  		  utPltPutVar(psDbHead,"modu_status", "模板页面数量不对，请重新上传");
	  	   	utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_module_result2.htm");
   				return 0; 	
	  	}
			 
	  	
	  	
	  	//替换html文件的图片路径，拷贝html到指定目录(一定规则(操作员的权限)生成的文件夹)     pe 等通过模板名称区分，模板名称即文件夹名	  	

	  	//http.conf 配置一个相对路径
	 }
	 printf("**********Come here**********\n");
   utPltPutVar(psDbHead,"fname",temp_file);
   
   
   
   //中文转码成utf-8
   memset(htmlpath,0,sizeof(htmlpath));
   sprintf(htmlpath,"/ncmportal/%s/%s/",caCompany,mname);
//   utPltPutVarF(psDbHead,"module_result","%s%s/%s",htmlpath,tlName,endname);
//   utPltPutVarF(psDbHead,"module_result","%s/%s",htmlpath,endname);
   
   utPltPutVar(psDbHead,"module_company",caCompany);
//   utPltPutVar(psDbHead,"module_temtype",tlName);
   char htmlname[256] = "" ,downname[256] = "";
   strcpy(htmlname,"/ncmportal/");
//   strcat(htmlname,tlName);
   strcat(htmlname,platenum);
   strcat(htmlname,"/");
   
   sprintf(downname,"%s/ncmportal/%s/%s.zip",MODULEAPPPATH,platenum,fname);
//   strcat(htmlname,endname);
   
   utPltPutVarF(psDbHead,"module_result","%s",htmlname);

//   sprintf(caTemp,"select count(*) from ncmodule where m_lan='%s' and m_type='%s' and m_name='%s' ",lanflags,mtype,mname);
   sprintf(caTemp,"select count(*) from ncmodule where m_lan='%s' and m_temtype='%s' and m_name='%s' ",lanflags,temtype,mname);
   pasDbOneRecord(caTemp,0,UT_TYPE_LONG,sizeof(long),&lcou);    
   printf("lcou=%d  htmlname=%s\n",lcou,htmlname);
   if (lcou > 0)
   {
//   		iReturn = pasDbExecSqlF("update ncmodule set htmlpath = '%s' where   m_comp='%s' and m_type='%s' and m_name='%s' ",htmlname,caCompany,mtype,mname);
   		iReturn = pasDbExecSqlF("update ncmodule set platepath = '%s',downname = '%s' where  m_lan='%s' and m_temtype='%s' and m_name='%s' ",htmlname,downname,lanflags,temtype,mname);
   }
      
   utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_module_result2.htm");
   return 0;
}

//应用模板
int ncmModuleApply(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
   FILE *fp;
   int i,iNum,iReturn;
   long lSid,lCount,lFlags;
   utPltDbHead *psDbHead;
 	 char caFile[256];
 	 char imp_file[128]="";
 	 char temp_file[128]="",filename[64]="";
 	 char sqlbuf[1024]="";
 	 int  m=0,n=0,findflag=0,lcou=0,maxmid=0;
 	 char caLocal[128],caRemote[128],caType[128];
 	 char caText[256],caBuf[16002];
   unsigned groupid=0,imgflag=0;
   char groupname[20],ap_groupid[30]="",tmpPath[128]="",lanflags[12]="",temtype[12]="",tmpexec[256]="",tmp2char[128]="";
   char tmp1[96]="",tmp2[96]="",subfname[64]="",fname[64]="",mtype[28]="",endname[64]="",mname[64]="";
 	 char *p,p1;
 	 DIR *dp;
 	 struct dirent *dirp;
 	 
 	 char caUsername[32] = "",caCompany[64] = "",htmlpath[128] = "",imgpath[128] = "",caTemp_d[256] = "";
 	 char *tlName;
 	 char module_result[256] = "", module_company[64] = "" , module_temtype[16] = "";
 	 char caTemp[1024] = "" ;
 	 long existflag=0;
#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
#endif
	 psDbHead = utPltInitDb();       
	 
	 //厂商 终端类型     html路径都传到表单 ，提交获取，拼接字符串   拷贝过去
	 utMsgPrintMsg(psMsgHead);
	 utMsgGetSomeNVar(psMsgHead,3,"module_result",   UT_TYPE_STRING,  sizeof(module_result)-1,   module_result,
	                              "module_company",  UT_TYPE_STRING,  sizeof(module_company)-1,  module_company,
	                              "module_temtype",  UT_TYPE_STRING,  sizeof(module_temtype)-1,  module_temtype);
	  
	  //先判断plate下是否有对应厂家 若有，先备份原来plate下的模板   
	 sprintf(caTemp,"%s/%s",MODULEAPPPATH,module_company);                             
	 printf("caTemp=%s \n",caTemp); 
	 if (utFileIsExist(caTemp))
	 {
	 		memset(caTemp,0,sizeof(caTemp));
	 		sprintf(caTemp,"%s/%s/%s",MODULEAPPPATH,module_company,module_temtype);   printf("caTemp2=%s \n",caTemp);      
	 		if (utFileIsExist(caTemp))
	 		{
	 			//备份
	 			memset(caTemp,0,sizeof(caTemp));
//	 			判断默认目录是否存在
        sprintf(caTemp,"%s/ncmportal/default",MODULEPATH);
        if (utFileIsExist(caTemp))
	 		  {
	 		      	
	 		  }
	 		  else
	 		  {
	 		  	 memset(caTemp,0,sizeof(caTemp));
	 		  	 sprintf(caTemp,"mkdir %s/ncmportal/default",MODULEPATH); printf("caTemp3=%s \n",caTemp);           
	 			   system(caTemp);
	 		  }
	 		  
	 			memset(caTemp,0,sizeof(caTemp));
	 			sprintf(caTemp,"yes | cp %s/%s/%s/* %s/ncmportal/default",MODULEAPPPATH,module_company,module_temtype,MODULEPATH);    
	 			system(caTemp);
	 			printf("caTemp4=%s \n",caTemp);      
	 			//拷贝
		 		memset(caTemp,0,sizeof(caTemp));
		 		sprintf(sqlbuf,"%s/%s/%s",MODULEAPPPATH,module_company,module_temtype);  	
		 		sprintf(caTemp,"yes | cp %s%s %s",MODULEPATH,module_result,sqlbuf);printf("caTemp4=%s \n",caTemp);      
		 		system(caTemp);
	 		}
	 }
	 else   //没有对应厂家  终端类型 创建文件夹  覆盖拷贝过来   
	 {
	 		sprintf(caTemp,"mkdir %s/%s",MODULEAPPPATH,module_company); 
	 		system(caTemp);
	 		memset(caTemp,0,sizeof(caTemp));
	 		sprintf(caTemp,"mkdir %s/%s/%s",MODULEAPPPATH,module_company,module_temtype);  
	 		system(caTemp);
	 		//拷贝
	 		memset(caTemp,0,sizeof(caTemp));
	 		sprintf(sqlbuf,"%s/%s/%s",MODULEAPPPATH,module_company,module_temtype);  	
	 		sprintf(caTemp,"yes | cp %s%s %s",MODULEPATH,module_result,sqlbuf);printf("caTemp21=%s \n",caTemp);     
	 		system(caTemp);
	 }
	                             
	 
	 /*
	 dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername);
	 printf("caUsername=%s \n",caUsername);
	 if (strlen(caUsername) > 0)
	 {
	 		sprintf(caTemp,"select company from dsuser where dispname = '%s' ",caUsername);
	    pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(caCompany)-1,caCompany);      	
	 }
	 */
	 
	 
	 
	 /*
	 
	 //厂商名称获取      
   if(!utFileIsExist(MODULEUPPATH)) {
        if(mkdir(MODULEUPPATH,555)!=0){
	        utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","导入","打开文件出错");
	        return 0;
        }
	 }
	
	 sprintf(tmpPath,"chmod +777 %s",MODULEUPPATH);
	 system(tmpPath);
	 utMsgPrintMsg(psMsgHead);
	 utMsgGetSomeNVar(psMsgHead,6,"filefield",UT_TYPE_STRING,  sizeof(imp_file)-1,   imp_file,
	                              "groupid",  UT_TYPE_STRING,  sizeof(ap_groupid)-1, ap_groupid,
	                              "lanflags", UT_TYPE_STRING,  sizeof(lanflags)-1,   lanflags,
	                              "mtype",    UT_TYPE_STRING,  sizeof(mtype)-1,      mtype,
	                              "mname",    UT_TYPE_STRING,  sizeof(mtype)-1,      mname,
	                              "temtype",  UT_TYPE_STRING,  sizeof(temtype)-1,    temtype); 
                       
	 //验证重复  根据操作员所属厂商，模板类型，模板名称   
	 sprintf(caTemp,"select count(*) from ncmodule where m_comp='%s' and m_type='%s' and m_name='%s' ",caCompany,mtype,mname);
   printf("caTemp=%s  \n",caTemp);
   pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lcou);                            
	 if (lcou == 0)
	 {
	 	  sprintf(caTemp,"select max(mid)+1 from ncmodule");
	 	  printf("lenmid=%d \n",sizeof(maxmid));
  	  pasDbOneRecord(caTemp,0,UT_TYPE_LONG,sizeof(maxmid),&maxmid);   
  	  if (maxmid == 0)
  	  {
  	  	maxmid = 1;	
  	  }
	 	  //若不存在 插入数据库   并执行存放策略      用户管理更新后，此处如何同步
	 		iReturn = pasDbExecSqlF("insert into ncmodule(mid,m_name,m_type,m_temtype,m_lan,m_comp,m_systime) values(%lu,'%s','%s','%s','%s','%s',%lu)",maxmid,mname,mtype,temtype,lanflags,caCompany,time(0));	
	    
	 }   
	 else          //若存在替换原来的
	 {
	 		//执行存放策略	
	 		
	 }                              
   printf("imp_file=%s\n",imp_file);                    
   if (strlen(imp_file) != 0)
   {
	  	p = utStrSkipSpaces(imp_file);
	  	p = utStrGetWord(p,filename,200,";\n");	//取文件名
	  	p = utStrGetWord(p,temp_file,200,";\n");
	  	if ((*p) == ';')
	  		p = utStrGetWord(p+1,temp_file,200,";\n");     //取带路径的文件名
	  	
	  	printf("  temp_file=%s  filename=%s\n",temp_file,filename);               
	  	
	  	//创建文件夹
	  	memset(tmpexec,0,sizeof(tmpexec));
	  	sprintf(tmpexec,"mkdir %s/pmodule",MODULEUPPATH);
	  	system(tmpexec);
	
	  	//文件夹加权限
	  	memset(tmpexec,0,sizeof(tmpexec));
	  	sprintf(tmpexec,"%s/pmodule",MODULEUPPATH);
	  	sprintf(tmp2char,"chmod 777  %s",tmpexec);
	  	system(tmp2char);
	  	
	  	//拷贝上传文件至临时处理文件夹
	  	memset(tmp2char,0,sizeof(tmp2char));
	  	sprintf(tmp2char,"mv %s %s",temp_file,tmpexec);
	  	printf("cp tmp2char=%s \n",tmp2char);
	  	system(tmp2char);
	  	strcpy(fname,filename);
	  	p1=strtok(fname,".");
	    if(p1) 
	    {
	    	 strcpy(subfname,fname);	
	    }
	    
	  	printf("subfname=%s fname=%s\n",subfname,fname);
	  	//解压强制覆盖
	  	memset(tmpexec,0,sizeof(tmpexec));
	  	sprintf(tmpexec,"unzip -xo %s/pmodule/%s -d %s/pmodule",MODULEUPPATH,filename,MODULEUPPATH);      //解压到当前目录
	  	printf(" tmpexec=%s\n",tmpexec);
	  	system(tmpexec);
	  	//读取指定目录下文件
	  	memset(tmpexec,0,sizeof(tmpexec));
	  	sprintf(tmpexec,"%s/pmodule",MODULEUPPATH);
	  	printf("tmpexec=%s \n",tmpexec);
	  	
	  	if((dp=opendir(tmpexec)) == NULL)
	        printf("can't open %s", tmpexec);
	  	while(((dirp = readdir(dp)) != NULL) && (n <= 500)){
	//        if(n % 1 == 0) printf("\n");
	        n++;
	        printf("%10s \n", dirp->d_name);
	        //查找是否有html文件，若无跳出，添加解压目录，在打开    若有直接 按规则拷贝指定目录。
	        if (strstr(dirp->d_name,".htm"))
	        {
	        	findflag=1;	
	        }
	        //根目录找到图片  否则在portal_images下 拷贝时路径变更
	        if (strstr(dirp->d_name,".jpg") || strstr(dirp->d_name,".png") || strstr(dirp->d_name,".gif"))
	        {
	        	imgflag=1;	
	        }
	    }
	  	closedir(dp); 
	  	printf("findflag=%d imgflag=%d \n",findflag,imgflag);
	  	
	  	if (strlen(mname) > 0)
	  	{	
	  		//创建模板图片文件夹                                         厂商      模板名
	  		if (strcmp(mtype,"认证页面模板") == 0)
	  		{
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s",MODULEPATH,caCompany);
	  			printf("imgpath=%s  \n",imgpath);
	  			system(imgpath);
	  			
	  			memset(imgpath,0,sizeof(imgpath));
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s/%s",MODULEPATH,caCompany,mname);
	  			printf("11 imgpath=%s  \n",imgpath);
	  			system(imgpath);
//	  			
	  			memset(imgpath,0,sizeof(imgpath));
	  			//终端类型判断  语言判断  中文
	  			strcpy(imgpath,ncmCreateTemLangSrcipt(temtype,lanflags,caCompany,mname,"login_main",tlName));
	  			printf("****** tlName=%s \n",tlName);
	  			
	  		}
	  		else if (strcmp(mtype,"认证通过页面模板") == 0)
	  		{
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s",MODULEPATH,caCompany);
	  			system(imgpath);
	  			memset(imgpath,0,sizeof(imgpath));
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s/%s",MODULEPATH,caCompany,mname);
	  			system(imgpath);

	  			memset(imgpath,0,sizeof(imgpath));
	  			strcpy(imgpath,ncmCreateTemLangSrcipt(temtype,lanflags,caCompany,mname,"login_ok",tlName));
	  		}	
	  		else if (strcmp(mtype,"认证退出页面模板") == 0)
	  		{ 
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s",MODULEPATH,caCompany);
	  			system(imgpath);
	  			memset(imgpath,0,sizeof(imgpath));
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s/%s",MODULEPATH,caCompany,mname);
	  			system(imgpath);
	  			memset(imgpath,0,sizeof(imgpath));
	  			strcpy(imgpath,ncmCreateTemLangSrcipt(temtype,lanflags,caCompany,mname,"logout_ok",tlName));
	  		}	
	  		else 
	  		{ 
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s",MODULEPATH,caCompany);
	  			system(imgpath);
	  			memset(imgpath,0,sizeof(imgpath));
	  			sprintf(imgpath,"mkdir %s/images/portalImages/%s/%s",MODULEPATH,caCompany,mname);
	  			system(imgpath);
	  			memset(imgpath,0,sizeof(imgpath));
	  			strcpy(imgpath,ncmCreateTemLangSrcipt(temtype,lanflags,caCompany,mname,"login_ok",tlName));
	  		}	
	  		
	  		//创建页面模板文件夹                                      //厂商     模板名
	  		sprintf(htmlpath,"mkdir %s/ncmportal/%s",MODULEPATH,caCompany);
	  		system(htmlpath);
	  		memset(htmlpath,0,sizeof(htmlpath));
	  		sprintf(htmlpath,"mkdir %s/ncmportal/%s/%s",MODULEPATH,caCompany,mname);
	  		system(htmlpath);
	  		memset(htmlpath,0,sizeof(htmlpath));
	  		sprintf(htmlpath,"mkdir %s/ncmportal/%s/%s/%s",MODULEPATH,caCompany,mname,tlName);
	  		printf("3 htmlpath=%s  \n",htmlpath);
	  		system(htmlpath);
	  		
	  		memset(htmlpath,0,sizeof(htmlpath));
	  		sprintf(htmlpath,"%s/ncmportal/%s/%s/%s",MODULEPATH,caCompany,mname,tlName);
	  	}
	  	
	  	n=0;
	  	//页面操作
	  	if (findflag == 1){
	        while(((dirp = readdir(dp)) != NULL) && (n <= 500)){
			        if (strstr(dirp->d_name,".htm"))
			        {
			        	memset(tmp1,0,sizeof(tmp1));
			        	memset(tmp2,0,sizeof(tmp2));
			        	//拷贝页面到指定目录
			        	sprintf(tmp1,"%s/%s",tmpexec,dirp->d_name);
			        	sprintf(tmp2,"cp %s %s",tmp1,htmlpath); 
			        	printf("tmp2=%s \n",tmp2);
			        	system(tmp2);
			        
						  	if ((strlen(mtype) > 0) && (strcmp(mtype,"认证页面模板") == 0))
						  	{
						  		 memset(tmp2char,0,sizeof(tmp2char));
						  		 //atest 为对应操作员所对应的厂商名称
							  	 sprintf(tmp2char,"mv %s/%s %s/%s",tmpexec,dirp->d_name,htmlpath,"login_main.htm");    
							  	 printf("tmp2char=%s \n",tmp2char);
							  	 system(tmp2char);
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"login_main",tlName);
							  	 strcpy(endname,"login_main.htm");
						  	}
						  	else if ((strlen(mtype) > 0) && (strcmp(mtype,"认证通过页面模板") == 0))
						  	{
						  		 memset(tmp2char,0,sizeof(tmp2char));
						  		 sprintf(tmp2char,"mv %s/%s %s/%s",tmpexec,dirp->d_name,htmlpath,"login_ok.htm");  
							  	 system(tmp2char);
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"login_ok",tlName);
							  	 strcpy(endname,"login_ok.htm");
						  	}
						  	else if ((strlen(mtype) > 0) && (strcmp(mtype,"认证退出页面模板") == 0))
						  	{  
						  		 memset(tmp2char,0,sizeof(tmp2char));
						  		 sprintf(tmp2char,"mv %s/%s %s/%s",tmpexec,dirp->d_name,htmlpath,"logout_ok.htm");  
							  	 system(tmp2char);
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"logout_ok",tlName);
							  	 strcpy(endname,"logout_ok.htm");
						  	}

			        	else              //默认为认证界面(全套同一文件夹无法区分)
			        	{
						  		 memset(tmp2char,0,sizeof(tmp2char));
						  		 sprintf(tmp2char,"mv %s/%s %s/%s",tmpexec,dirp->d_name,htmlpath,"login_main.htm");  
							  	 system(tmp2char);
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"login_main",tlName);
							  	 strcpy(endname,"login_main.htm");
						  	}
			        }
			        if (imgflag == 1)       //根目录找到
			       	{
			       		ncmCpImgSrcipt(dirp->d_name,tmpexec,imgpath);
			      	}
						  n++;
			    }
			  	closedir(dp);  	
	    }
	    else
	    {
	    	  //添加解压文件夹目录
	    	  sprintf(tmpexec,"%s/pmodule/%s",MODULEUPPATH,subfname);
	    		if((dp=opendir(tmpexec)) == NULL)
	        	printf("can't open %s", tmpexec);
	        n=0;	
			  	while(((dirp = readdir(dp)) != NULL) && (n <= 500)){
			        if (strstr(dirp->d_name,".htm"))
			        {
			        	memset(tmp1,0,sizeof(tmp1));
			        	memset(tmp2,0,sizeof(tmp2));
			        	//拷贝页面到指定目录       必须拷贝，因为目录下有2个含有替换字符串的页面才会替换。
			        	sprintf(tmp1,"%s/%s",tmpexec,dirp->d_name);
			        	sprintf(tmp2,"cp %s %s",tmp1,htmlpath); 
			        	printf("tmp2=%s \n",tmp2);
			        	system(tmp2);
			        
						  	if ((strlen(mtype) > 0) && (strcmp(mtype,"认证页面模板") == 0))
						  	{
						  		 memset(tmp2char,0,sizeof(tmp2char));
						  		 //atest 为对应操作员所对应的厂商名称     移动到指定文件夹下并更名
							  	 sprintf(tmp2char,"mv %s/%s %s/%s",tmpexec,dirp->d_name,htmlpath,"login_main.htm");    
							  	 system(tmp2char);
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"login_main",tlName);
							  	 strcpy(endname,"login_main.htm");
						  	}
						  	else if ((strlen(mtype) > 0) && (strcmp(mtype,"认证通过页面模板") == 0))
						  	{
						  		 memset(tmp2char,0,sizeof(tmp2char));
						  		 sprintf(tmp2char,"mv %s/%s %s/%s",tmpexec,dirp->d_name,htmlpath,"login_ok.htm");  
							  	 system(tmp2char);
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"login_ok",tlName);
							  	 strcpy(endname,"login_ok.htm");
						  	}
						  	else if ((strlen(mtype) > 0) && (strcmp(mtype,"认证退出成功页面模板") == 0))
						  	{  
						  		 memset(tmp2char,0,sizeof(tmp2char));
						  		 sprintf(tmp2char,"mv %s/%s %s/%s",tmpexec,dirp->d_name,htmlpath,"logout_ok.htm");  
							  	 system(tmp2char);
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"logout_ok",tlName);
							  	 strcpy(endname,"logout_ok.htm");
						  	}
			        	else              //默认为认证界面(全套同一文件夹无法区分)
			        	{
						  		 memset(tmp2char,0,sizeof(tmp2char));
						  		 sprintf(tmp2char,"mv %s/%s %s/%s",tmpexec,dirp->d_name,htmlpath,"login_main.htm");  
							  	 system(tmp2char);
							  	 ncmExecSrcipt(htmlpath,caCompany,mname,"login_main",tlName);
							  	 strcpy(endname,"login_main.htm");
						  	}
			        }
			        //解压子目录 如果找到flag=2  
			      	if (strstr(dirp->d_name,".jpg") || strstr(dirp->d_name,".png") || strstr(dirp->d_name,".gif"))
			        {
			        	imgflag=2;	
			        }
			        n++;
			    }
			  	closedir(dp);  	
	    }
	    printf("2 imgflag=%d \n",imgflag);
      if (imgflag == 2) //html目录下找到
			{
				  sprintf(tmpexec,"%s/pmodule/%s",MODULEUPPATH,subfname);
	    		if((dp=opendir(tmpexec)) == NULL)
	        	printf("can't open %s", tmpexec);
	        n=0;
	        while(((dirp = readdir(dp)) != NULL) && (n <= 500)){	
					  ncmCpImgSrcipt(dirp->d_name,tmpexec,imgpath);
					  n++;
					}
				  closedir(dp);  	
			}
			else    //根目录和html子目录都没有
			{
				  sprintf(tmpexec,"%s/pmodule/%s/portal_images",MODULEUPPATH,subfname);
				  printf("2222 tmpexec=%s  imgpath=%s\n",tmpexec,imgpath);
	    		if((dp=opendir(tmpexec)) == NULL)
	        	printf("can't open %s \n", tmpexec);
	        n=0;
			    while(((dirp = readdir(dp)) != NULL) && (n <= 500)){
				    ncmCpImgSrcipt(dirp->d_name,tmpexec,imgpath);
					  n++;
					}
				  closedir(dp);  	
			} 
			 
	  	//最后要删除upload下的临时文件夹，否则新修改的不会覆盖现有的。
	  	memset(tmp2char,0,sizeof(tmp2char));
	  	sprintf(tmp2char,"rm -fr %s/pmodule",MODULEUPPATH);
	  	printf("tmp2char=%s \n",tmp2char);
	  	system(tmp2char);
	  	
	  	//替换html文件的图片路径，拷贝html到指定目录(一定规则(操作员的权限)生成的文件夹)     pc eng等通过模板名称区分，模板名称即文件夹名	  	

	  	//http.conf 配置一个相对路径
	 }
   utPltPutVar(psDbHead,"fname",temp_file);

   memset(htmlpath,0,sizeof(htmlpath));
   sprintf(htmlpath,"/ncmportal/%s/%s/",caCompany,mname);
   utPltPutVarF(psDbHead,"module_result","%s%s",htmlpath,endname);
   char htmlname[256]="";
   strcpy(htmlname,htmlpath);
   strcat(htmlname,endname);
   printf("lcou=%d  \n",lcou);
   if (lcou > 0)
   {
   		iReturn = pasDbExecSqlF("update ncmodule set htmlpath = '%s' where   m_comp='%s' and m_type='%s' and m_name='%s' ",htmlname,caCompany,mtype,mname);
   }
   */
   //返回页面状态 以及预览文件 
   utPltPutVar(psDbHead,"fname",temp_file);
   utPltPutVar(psDbHead,"module_result",module_result);
   //调整到应用成功失败提示页   
   utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_module_result2.htm");
   return 0;
}

int ncmTreeFac(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
   	char caTemp[1024];
   	char sqlbuf[1024];
   	long lFlag,lKid;
   	int  iNum;
  
    utPltDbHead *psDbHead;
   	int iReturn,i;   
    long lCount;
    char caName[64];
    long lId,lSid;
    char caKeyword[32] = "";
    char caPlate[128];
    char caDel[16],caUpdate[16],caSelsid[1024],caKeyname[64],caSid[16],caTemp_d[64];
    char caCode[16];
    char caFacs[1024]="";	

    utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,8,"keyword",   UT_TYPE_STRING,sizeof(caKeyword)-1, caKeyword,
															             "del",       UT_TYPE_STRING,sizeof(caDel)-1,     caDel,
															             "keyname",   UT_TYPE_STRING,sizeof(caKeyname)-1, caKeyname,
															             "selsid",    UT_TYPE_STRING,sizeof(caSelsid)-1,  caSelsid,
															             "update",    UT_TYPE_STRING,sizeof(caUpdate)-1,  caUpdate,
															             "sid",       UT_TYPE_STRING,sizeof(caSid)-1,     caSid,
															             "del",       UT_TYPE_STRING,sizeof(caDel)-1,     caDel,
															             "plate",     UT_TYPE_STRING,sizeof(caPlate)-1,   caPlate);
    
    trim(caKeyword);
    if(strlen(caKeyword)>0){
				pasCvtGBK(2,caKeyword,caTemp,30);
				strcpy(caKeyword,caTemp);
    }
    strcpy(caFacs,getDsShopids());
    iNum = 0;
    psDbHead = utPltInitDb();

		//sprintf(sqlbuf,"select groupid,name from ncfacgroup where 1=1 and flags = 0 ");
    sprintf(sqlbuf,"select groupid,dispname from ncsrvgroup where 1=1 and flags = 1 ");
//    sprintf(sqlbuf+strlen(sqlbuf)," and length(groupname)<17");
    if(strlen(caKeyword)>0){
    	sprintf(sqlbuf+strlen(sqlbuf)," and dispname like '%%%s%%' ", caKeyword);
    }
    if(strlen(caFacs)>0){
    	sprintf(sqlbuf+strlen(sqlbuf)," and groupid in (%s) ",caFacs);
    }
    sprintf(sqlbuf+strlen(sqlbuf)," order by moditime DESC limit 15");
    printf("sqlbuf = %s \n",sqlbuf);
  	psCur = pasDbOpenSql(sqlbuf,0);
    if(psCur == NULL) {
    	  utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","部门查询","数据库出错");
        return 0;
    }
    memset(caName,0,sizeof(caName));
    memset(caCode,0,sizeof(caCode));
    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caCode,
                                   UT_TYPE_STRING,63,caName);
                     
    while(iReturn == 0||iReturn==1405) {
         if(iNum>0){
         		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
         }
         iNum++;
         utPltSetCvtHtml(1);  		        
			   utPltPutLoopVarF(psDbHead,"groupname",iNum,"%s",caName);					    
         utPltSetCvtHtml(0);  
         utPltPutLoopVarF(psDbHead,"groupid",iNum,"%s",caCode);
         utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
         utPltPutLoopVar(psDbHead,"cls", iNum,"file");
         	
		     memset(caName,0,sizeof(caName));
	       memset(caCode,0,sizeof(caCode));
			   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caCode,
			                                  UT_TYPE_STRING,63,caName);
    }
    pasDbCloseCursor(psCur);

    if(strlen(caPlate)>0){
    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
    }else{
	  	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_group_checktree.htm");
	  }
    return 0;
}
//根据终端类型 语言生成图片文件夹 
static char *ncmCreateTemLangMaintain(char temtype[64],char lanflags[8],char caCompany[64],char mname[64],char authtype[16])
{
    char tmp1[256]="";	
	  if (strcmp(temtype,"PC") == 0)
	  {
	  		if (strcmp(lanflags,"中文") == 0)
			  {
			  	  ncmCreateTemLangExec("pc",authtype,caCompany,mname); 
					  sprintf(tmp1,"%s/images/portalImages/%s/%s/pc/%s",MODULEPATH,caCompany,mname,authtype);
			  }
			  else            //英文
			  {
			  	 ncmCreateTemLangExec("pe",authtype,caCompany,mname);  
			  	 sprintf(tmp1,"%s/images/portalImages/%s/%s/pe/%s",MODULEPATH,caCompany,mname,authtype);
			  }	
	  }
	  else if (strcmp(temtype,"平板") == 0)
	  {
	  		if (strcmp(lanflags,"中文") == 0)
			  {
			  		ncmCreateTemLangExec("pad",authtype,caCompany,mname);  
			  		sprintf(tmp1,"%s/images/portalImages/%s/%s/pad/%s",MODULEPATH,caCompany,mname,authtype);	
			  }
			  else            
			  {
			  		ncmCreateTemLangExec("padeng",authtype,caCompany,mname);  
			  		sprintf(tmp1,"%s/images/portalImages/%s/%s/padeng/%s",MODULEPATH,caCompany,mname,authtype);		
			  }	
	  }
	  else if (strcmp(temtype,"手机") == 0)
	  {
	  		if (strcmp(lanflags,"中文") == 0)
			  {
			  		ncmCreateTemLangExec("sms",authtype,caCompany,mname);  
			  		sprintf(tmp1,"%s/images/portalImages/%s/%s/sms/%s",MODULEPATH,caCompany,mname,authtype);	
			  }
			  else            
			  {
			  		ncmCreateTemLangExec("smseng",authtype,caCompany,mname);  
			  		sprintf(tmp1,"%s/images/portalImages/%s/%s/smseng/%s",MODULEPATH,caCompany,mname,authtype);	
			  }	
	  }
	  
    return tmp1;	
}
/*
*功能生成图片存放的厂商和模板名称文件夹
*temname：厂商名
*authtype：验证类型
*caCompany：厂商
*mname:模板名称
*/
static int ncmUpdateTemLangExec(char caCompany[64],char mname[64])
{
    char imgpath[256]="";                                             //厂商名   模板名  终端类型
    sprintf(imgpath,"mkdir %s/images/portalImages/%s",MODULEPATH,caCompany);
	  system(imgpath);
	  memset(imgpath,0,sizeof(imgpath));                                                          
	  sprintf(imgpath,"mkdir %s/images/portalImages/%s/%s",MODULEPATH,caCompany,mname);
//	  printf("******模板语言执行2 imgpath=%s  \n",imgpath);
	  system(imgpath);
	  return 1;
}
//模板维护更新 重新生成文件夹 
static int ncmUpdateTemLangMaintain(char temtype[64],char lanflags[8],char caCompany[64],char mname[64],char authtype[16])
{
    //生成厂商和模板文件夹
    ncmUpdateTemLangExec(caCompany,mname);
    //生成终端类型和验证类型文件夹
	  if (strcmp(temtype,"PC") == 0)
	  {
	  		if (strcmp(lanflags,"中文") == 0)
			  {
			  	  ncmCreateTemLangExec("pc",authtype,caCompany,mname); 					  
			  }
			  else            //英文
			  {
			  	 ncmCreateTemLangExec("pe",authtype,caCompany,mname);  
			  }	
	  }
	  else if (strcmp(temtype,"平板") == 0)
	  {
	  		if (strcmp(lanflags,"中文") == 0)
			  {
			  		ncmCreateTemLangExec("pad",authtype,caCompany,mname);  
			  }
			  else            
			  {
			  		ncmCreateTemLangExec("padeng",authtype,caCompany,mname);  
			  }	
	  }
	  else if (strcmp(temtype,"手机") == 0)
	  {
	  		if (strcmp(lanflags,"中文") == 0)
			  {
			  		ncmCreateTemLangExec("sms",authtype,caCompany,mname);  	
			  }
			  else            
			  {
			  		ncmCreateTemLangExec("smseng",authtype,caCompany,mname);  
			  }	
	  }
	  
    return 1;	
}
static char *ncmUpdateHtmlfolder(char caCompany[64],char mname[64],char temtype[64])
{
    char imgpath[256]="" ,newPagef[256] = "";                                             //厂商名   模板名  终端类型
    sprintf(imgpath,"mkdir %s/ncmportal/%s",MODULEPATH,caCompany);
	  system(imgpath);
	  memset(imgpath,0,sizeof(imgpath));                                                          
	  sprintf(imgpath,"mkdir %s/ncmportal/%s/%s",MODULEPATH,caCompany,mname);
	  system(imgpath);
	  memset(imgpath,0,sizeof(imgpath));                                                          
	  sprintf(imgpath,"mkdir %s/ncmportal/%s/%s/%s",MODULEPATH,caCompany,mname,temtype);
//	  printf("******模板语言执行2 imgpath=%s  \n",imgpath);
	  system(imgpath);
	  sprintf(newPagef,"%s/ncmportal/%s/%s/%s",MODULEPATH,caCompany,mname,temtype);
	  return newPagef;
}
//模板管理(维护)
int ncmWebModuleSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iNum,iReturn;
    utPltDbHead *psDbHead;
    char *p;
    int i=0;
   	char caGid[16],caDid[16],caAdd[16],caDel[16];
		char caName[32],caModi[16],caApp[16];
		char caDevname[64],caCaptype[10],caFlags[2];
		long sid=0;
		char caSid[16];
		char caTemp[2024];
		long promisc=0,lTotRec=0,tmpcode=0,lSid=0;
	 	pasDbCursor *psCur;
    char captype_desc[32]="",flags_desc[8]="",caPromisc[16]="";
    char caTemp_d[256],caSesid[1024],caMsg[1024],caUsername2[32]="";
    
    long mid=0;
    char m_name[64] = "", m_type[32] = "", m_temtype[32] = "", m_lan[32] = "", m_comp[32] = "" , htmlpath[256] = "";
    char tempflag[16] = "", lan_flag[16] = "",mokeyword[64] = "",sqlbuf[1024] = "" ,m_mesg[256] = "" , downname[256] = "";
    char down_name[256] = "";
    utMsgPrintMsg(psMsgHead);
    dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
	  iReturn = utMsgGetSomeNVar(psMsgHead,16,
	                "m_name",        UT_TYPE_STRING,sizeof(m_name) - 1    ,m_name,
	                "m_type",        UT_TYPE_STRING,sizeof(m_type) - 1    ,m_type,
	                "m_temtype",     UT_TYPE_STRING,sizeof(m_temtype) - 1 ,m_temtype,
	                "m_lan",         UT_TYPE_STRING,sizeof(m_lan) - 1     ,m_lan,
	                "m_comp",        UT_TYPE_STRING,sizeof(m_comp) - 1    ,m_comp,    
	                "tempflag",      UT_TYPE_STRING,sizeof(m_comp) - 1    ,tempflag,      //终端类型
	                "lan_flag",      UT_TYPE_STRING,sizeof(m_comp) - 1    ,lan_flag,         //语言
	                "mokeyword",     UT_TYPE_STRING,sizeof(mokeyword) - 1 ,mokeyword,    
	                "m_downname",    UT_TYPE_STRING,sizeof(down_name) - 1 ,down_name,
	                "mesg",          UT_TYPE_STRING,sizeof(m_mesg) - 1    ,m_mesg,
	                "m_mid",         UT_TYPE_STRING,12,caSid,
	                "add",           UT_TYPE_STRING,12,caAdd,
	                "del",           UT_TYPE_STRING,12,caDel,
	                "selsid",        UT_TYPE_STRING,1000,caSesid,
	                "update",        UT_TYPE_STRING,12,caModi,
	                "app",           UT_TYPE_STRING,11,caApp);
	  
	  trim(mokeyword);
	  
	  if(strlen(down_name)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,down_name,caTemp,sizeof(down_name)-1));
    	strcpy(down_name,caTemp_d);
    	utPltFileDownload(iFd, "application/zip", "", down_name, down_name);
    	return 1;
    }
	  
	  
	  //未操作选择框
	  if(strlen(m_name)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,m_name,caTemp,sizeof(m_name)-1));
    	strcpy(m_name,caTemp_d);
    }
    if(strlen(m_type)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,m_type,caTemp,sizeof(m_type)-1));
    	strcpy(m_type,caTemp_d);
    }
    if(strlen(m_temtype)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,m_temtype,caTemp,sizeof(m_temtype)-1));
    	strcpy(m_temtype,caTemp_d);
    }
    if(strlen(m_lan)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,m_lan,caTemp,sizeof(m_lan)-1 ));
    	strcpy(m_lan,caTemp_d);
    }
    if(strlen(m_comp)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,m_comp,caTemp,sizeof(m_comp)-1));
    	strcpy(m_comp,caTemp_d);
    }
    if(strlen(tempflag)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,tempflag,caTemp,sizeof(tempflag)-1));
    	strcpy(tempflag,caTemp_d);
    }
    if(strlen(lan_flag)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,lan_flag,caTemp,sizeof(lan_flag)-1));
    	strcpy(lan_flag,caTemp_d);
    }
	  if(strlen(mokeyword)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,mokeyword,caTemp,sizeof(mokeyword)-1));
    	strcpy(mokeyword,caTemp_d);
    }
    if(strlen(m_mesg)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,m_mesg,caTemp,sizeof(m_mesg)-1));
    	strcpy(m_mesg,caTemp_d);
    }
	  
	  if(strlen(captype_desc) > 2)
	  {
		  	sprintf(caTemp,"select code from ncwethetype where flag=1 and name='%s'  ",captype_desc);
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&tmpcode);  
	  		memset(caCaptype,0,sizeof(caCaptype));
	  		sprintf(caCaptype,"%d",tmpcode); 
	  }
	  else{
	  		strcpy(caCaptype,captype_desc);	
	  }              
    
    printf("caCaptype=%s  caPromisc=%s  caFlags=%s  mokeyword = %s \n",caCaptype,caPromisc,caFlags,mokeyword);
    
    if(strcmp(caModi,"update")==0){  //增加一个组 
       if(atol(caSid) == 0)
       {  		
//         		unsigned long lSid=pasGetSid("ncnetdev","sid");
         		sprintf(caTemp,"select max(mid)+1 from ncmodule   ");
  					pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSid);  
         		printf("lSid=%d \n",lSid);
         		if(lSid == 0)
         		{
         			lSid=1;	
         		}
            iReturn = pasDbExecSqlF("insert into ncmodule(mid,m_name,m_type,m_temtype,m_lan,m_comp,m_systime) values(%lu,'%s','%s','%s','%s','%s',%lu)",lSid,m_name,m_type,m_temtype,m_lan,m_comp,time(0));
            if(iReturn != 0) {
                utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","模板管理",ncLang("0310记录增加出错"));
                pasDbRollback(NULL);
                return 0;
            }        
            else {
                pasDbCommit(NULL);
            }      
       }else {   //更新
          //删除 创新新的目录
          /*
          char srcPath[256] = "" ,dstPath[256] = "" ,srcImgPath[256] = "" , dstImgPath[256] = "";
          sprintf(sqlbuf,"update ncmodule set m_name='%s',m_type='%s',m_temtype='%s',m_lan='%s',m_comp='%s' where mid = %s ",
                         m_name,m_type,m_temtype,m_lan,m_comp,caSid);
          sprintf(srcPath,"",);
          sprintf(srcImgPath,"",);
          sprintf(caTemp,"select m_name,m_type,m_temtype,m_lan,m_comp,htmlpath from ncmodule where mid = %s",caSid);
          memset(m_name,0,sizeof(m_name));
				  memset(m_type,0,sizeof(m_type));
				  memset(m_temtype,0,sizeof(m_temtype));
				  memset(m_lan,0,sizeof(m_lan));
				  memset(m_comp,0,sizeof(m_comp));
				  memset(htmlpath,0,sizeof(htmlpath));
				  printf("caTemp = %s \n",caTemp);
          pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(m_name)-1    ,m_name,
				                          UT_TYPE_STRING,sizeof(m_type)-1    ,m_type,
				                          UT_TYPE_STRING,sizeof(m_temtype)-1 ,m_temtype,
				                          UT_TYPE_STRING,sizeof(m_lan)-1     ,m_lan,
				                          UT_TYPE_STRING,sizeof(m_comp)-1    ,m_comp,
				                          UT_TYPE_STRING,sizeof(htmlpath)-1  ,htmlpath);
				  //创建新的文件夹(页面和图片)
				  //生成新的图片文件夹
				  
				  if (strcmp(m_type,"认证页面模板") == 0)         
					{
					  	ncmUpdateTemLangMaintain(m_temtype,m_lan,m_comp,m_name,"login_main");
					}
					else if (strcmp(m_type,"认证通过页面模板") == 0)
					{
						  ncmUpdateTemLangMaintain(m_temtype,m_lan,m_comp,m_name,"login_ok");
					}	
					else if (strcmp(m_type,"认证退出页面模板") == 0)
					{   
						  ncmUpdateTemLangMaintain(m_temtype,m_lan,m_comp,m_name,"logout_ok"); 
					}	
					else 
					{ 
					  	ncmUpdateTemLangMaintain(m_temtype,m_lan,m_comp,m_name,"login_main");
					}	
					
		      //生成新的页面文件夹  	  
          if (strcmp(m_temtype,"PC") == 0)
				  {
				  		if (strcmp(m_lan,"中文") == 0)
						  {
						  	  strcpy(dstPath,ncmUpdateHtmlfolder(caCompany,mname,"pc"));
						  }
						  else            //英文
						  {
//						  	 ncmUpdateHtmlfolder(caCompany,mname,"pe");
						  	 strcpy(dstPath,ncmUpdateHtmlfolder(caCompany,mname,"pe"));
						  }	
				  }
				  else if (strcmp(m_temtype,"平板") == 0)
				  {
				  		if (strcmp(m_lan,"中文") == 0)
						  {
						  		strcpy(dstPath,ncmUpdateHtmlfolder(caCompany,mname,"pad"));
						  }
						  else            
						  {
						  		strcpy(dstPath,ncmUpdateHtmlfolder(caCompany,mname,"padeng"));
						  }	
				  }
				  else if (strcmp(m_temtype,"手机") == 0)
				  {
				  		if (strcmp(m_lan,"中文") == 0)
						  {
						  		strcpy(dstPath,ncmUpdateHtmlfolder(caCompany,mname,"sms"));
						  }
						  else            
						  { 
						  		strcpy(dstPath,ncmUpdateHtmlfolder(caCompany,mname,"smseng"));
						  }	
				  }
					else          //默认
					{ 
					  	strcpy(dstPath,ncmUpdateHtmlfolder(caCompany,mname,"default"));
					}			     
				  
				  //拷贝(图片和页面)        记录以前的路径和当前路径
				  
				  memset(caTemp,0,sizeof(caTemp));             
				  sprintf(dstImgPath,"%s/images/portalImages/%s/%s/%s",MODULEPATH,m_comp,m_name,);
				  sprintf(caTemp,"yes | cp -rf %s %s",srcImgPath,dstImgPath);
				  system(caTemp);
				  
				  memset(caTemp,0,sizeof(caTemp));
				  sprintf(srcPath,"%s/ncmportal/%s/%s/%s",MODULEPATH,m_comp,m_name,);
				  sprintf(caTemp,"yes | cp -rf %s %s",srcPath,dstPath);
				  system(caTemp);
				  //删除以前的文件夹(图片和页面)
				  sprintf()
				  */
				  //更新数据库    
				  sprintf(sqlbuf,"update ncmodule set m_name='%s',m_temtype='%s',m_lan='%s',mesg='%s' where mid = %s ",
                         m_name,m_temtype,m_lan,m_mesg,caSid); 
				  printf("******update sqlbuf=%s \n",sqlbuf);                   
				  iReturn = pasDbExecSqlF(sqlbuf);                         

	        
	        if(iReturn != 0) {
	            utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0086Web地址列表"),ncLang("0700修改出错%s"),pasDbErrorMsg(NULL));
	            return 0;
	        }  
        	pasDbCommit(NULL);
    	 }              
       
    }
    else if(!utStrIsSpaces(caDel)) { // 删除一个组  
	    if(strlen(caSesid)>0){
		        //同时删除html下对应页面   删除images下对应图片
		        sprintf(caTemp,"select mid,platepath from ncmodule where mid in(%s)",caSesid);
//		        printf(" 删除图片文件夹 sql = %s  \n",caTemp);
		        psCur=pasDbOpenSql(caTemp,0);
		        if(psCur==NULL){
				      	    utPltFreeDb(psDbHead);
				            utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","模板管理删除查询",ncLang("0174出错 %s"),pasDbErrorMsg(NULL));
				            return 0;
				    }
				    
				
				    memset(htmlpath,0,sizeof(htmlpath));
				    lSid = 0;   
				    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG  ,sizeof(long)        ,&lSid
				                                ,UT_TYPE_STRING,sizeof(htmlpath)-1  ,htmlpath);
				      				  
				    iNum = 0;
				    while(iReturn == 0)
				    {	
		        		sprintf(caTemp,"rm -fr %s%s",MODULEAPPPATH,htmlpath);
		        		//删除页面
		        		if(strlen(caTemp) >= 11)       //确保不删除 rm -fr /home/ncmysql/ncsrv/html/images/portalImages目录
		        		{
			        		system(caTemp);
		        		}
		        		//删除图片文件夹
		        		memset(caTemp,0,sizeof(caTemp));
		        		if (strlen(htmlpath) > 10)
		        		{
		        			sprintf(caTemp,"rm -fr %s/images/portalImages/%08d",MODULEPATH,lSid);
		        			system(caTemp);
		        		}
		        		
				      	memset(caTemp,0,sizeof(caTemp));
				      	memset(htmlpath,0,sizeof(htmlpath));
				        lSid = 0;   
					      
					      iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG  ,sizeof(long)        ,&lSid
				                                    ,UT_TYPE_STRING,sizeof(htmlpath)-1  ,htmlpath);
				  	}  
				    pasDbCloseCursor(psCur);
		        iReturn = pasDbExecSqlF("delete from ncmodule where mid in(%s)",caSesid);
		        if(iReturn != 0) 
		        {
		            pasDbRollback(NULL);
		        }  
		        else{

		        }  
		        pasDbCommit(NULL);
		        sprintf(caMsg,"delete from ncmodule where mid in(%s) ",caSesid);
            if(iReturn==0){
            		ncmWriteSysLog(psMsgHead,caUsername2,"05","成功",caMsg); 
            }
            else{
            		ncmWriteSysLog(psMsgHead,caUsername2,"05","失败",caMsg);
            }
		  }
    }
    else if(!utStrIsSpaces(caApp)) { /* 应用  */
       // By Liyunmng  ncPkgCapApp(psShmHead);
//        ncUtlLoadGroupIpLst(psShmHead);
    }
         
      psDbHead = utPltInitDb();
      sprintf(caTemp,"select count(*) from ncmodule  where 1 = 1 ");
      if (strlen(tempflag) > 0)
      {	
      	sprintf(caTemp+strlen(caTemp)," and m_temtype = '%s' ",tempflag);
    	}
      if (strlen(lan_flag) > 0)
      {	
      	sprintf(caTemp+strlen(caTemp)," and m_lan = '%s' ",lan_flag);
    	}
    	if (strlen(mokeyword) > 0)
      {	
      	sprintf(caTemp+strlen(caTemp)," and (m_name like '%c%s%c' or mesg like '%c%s%c' ) ",'%',mokeyword,'%','%',mokeyword,'%');
    	}
    	printf("caTemp=%s \n",caTemp);
  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lTotRec);   
//      sprintf(caTemp,"select mid,m_name,m_type,m_temtype,m_lan,m_comp,htmlpath from ncmodule where 1 = 1 ");
      
      sprintf(caTemp,"select mid,m_name,m_temtype,m_lan,platepath,mesg,downname from ncmodule where 1 = 1 ");
      
      if (strlen(tempflag) > 0)
      {	
      	sprintf(caTemp+strlen(caTemp)," and m_temtype = '%s' ",tempflag);
    	}
      if (strlen(lan_flag) > 0)
      {	
      	sprintf(caTemp+strlen(caTemp)," and m_lan = '%s' ",lan_flag);
    	}
    	if (strlen(mokeyword) > 0)
      {	
      	sprintf(caTemp+strlen(caTemp)," and (m_name like '%c%s%c' or mesg like '%c%s%c' ) ",'%',mokeyword,'%','%',mokeyword,'%');
    	}
    	psCur=pasDbOpenSql(caTemp,0);
      
      if(psCur==NULL){
      	    utPltFreeDb(psDbHead);
            utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","模板维护查询",ncLang("0174出错 %s"),pasDbErrorMsg(NULL));
            return 0;
      }
      mid=0;	
      memset(m_name,0,sizeof(m_name));
      memset(m_type,0,sizeof(m_type));
      memset(m_temtype,0,sizeof(m_temtype));
      memset(m_lan,0,sizeof(m_lan));
      memset(m_comp,0,sizeof(m_comp));
      memset(htmlpath,0,sizeof(htmlpath));
      memset(m_mesg,0,sizeof(m_mesg));
      memset(downname,0,sizeof(downname)); 
      iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,sizeof(mid),&mid,
                                   UT_TYPE_STRING,sizeof(m_name)-1,m_name,
//                                   UT_TYPE_STRING,sizeof(m_type)-1,m_type,
                                   UT_TYPE_STRING,sizeof(m_temtype)-1,m_temtype,
                                   UT_TYPE_STRING,sizeof(m_lan)-1,m_lan,
//                                   UT_TYPE_STRING,sizeof(m_comp)-1,m_comp,
                                   UT_TYPE_STRING,sizeof(htmlpath)-1,htmlpath,
                                   UT_TYPE_STRING,sizeof(htmlpath)-1,m_mesg,
                                   UT_TYPE_STRING,sizeof(htmlpath)-1,downname);
      				  
      iNum=0;
      while(iReturn==0){
      	if(iNum>0){
        	utPltPutLoopVar(psDbHead,"m_dh",iNum+1,",");
        }
      	iNum++;
	      utPltPutLoopVarF(psDbHead,"m_iNum",iNum,"%lu",iNum);
      	utPltPutLoopVarF(psDbHead,"m_mid" ,iNum,"%lu",mid);
      	utPltPutLoopVar(psDbHead,"m_name" ,iNum,m_name);
      	utPltPutLoopVar(psDbHead,"m_type" ,iNum,m_type);
      	utPltPutLoopVar(psDbHead,"m_lan"  ,iNum,m_lan);
      	utPltPutLoopVar(psDbHead,"m_temtype"  ,iNum,m_temtype);
      	utPltPutLoopVar(psDbHead,"m_comp" ,iNum,m_comp);
      	utPltPutLoopVar(psDbHead,"htmlpath" ,iNum,htmlpath);
        utPltPutLoopVar(psDbHead,"mesg" ,iNum,m_mesg);
        utPltPutLoopVar(psDbHead,"downname" ,iNum,downname);
	      mid=0;		 
	      memset(m_name,0,sizeof(m_name));
	      memset(m_temtype,0,sizeof(m_temtype));
	      memset(m_lan,0,sizeof(m_lan));
	      memset(htmlpath,0,sizeof(htmlpath));
	      memset(m_mesg,0,sizeof(m_mesg));
      	memset(downname,0,sizeof(downname)); 
	      iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,sizeof(mid),&mid,
	                                   UT_TYPE_STRING,sizeof(m_name)-1,m_name,
	                                   UT_TYPE_STRING,sizeof(m_temtype)-1,m_temtype,
	                                   UT_TYPE_STRING,sizeof(m_lan)-1,m_lan,
	                                   UT_TYPE_STRING,sizeof(htmlpath)-1,htmlpath,
	                                   UT_TYPE_STRING,sizeof(htmlpath)-1,m_mesg,
	                                   UT_TYPE_STRING,sizeof(htmlpath)-1,downname);
  		}
       
    pasDbCloseCursor(psCur);
    utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
//    utPltShowDb(psDbHead); 
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_ext_webshowmodule_man.htm");
      
    return 0;
}
//显示接入厂商
int ncmWebModuleManSelect(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
   	int  iNum;
   	long lGroupid,lGroupid0,lCurPg,lTotRec,lRowNum,lStartRec;
    utPltDbHead *psDbHead;
   	int iReturn,i;
    char caCode[16],caName[68],flag[4]="";
    char caTemp[1024];
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead,1,"flag", UT_TYPE_STRING, 4, flag);
    if(strlen(flag) > 0 && atoi(flag) == 1){	
    	sprintf(caTemp,"select groupid,name from ncfacgroup where flags=1 limit 0,29 "); 
  	}else if(strlen(flag) > 0 && atoi(flag) == 2){	
  		sprintf(caTemp,"select groupid,name from ncfacgroup where flags=2 limit 0,29 "); 
  	}else if(strlen(flag) > 0 && atoi(flag) == 3){	
  		sprintf(caTemp,"select groupid,name from ncfacgroup where flags=3 limit 0,29 "); 
  	}else if(strlen(flag) > 0 && atoi(flag) == 4){	
  		sprintf(caTemp,"select groupid,name from ncfacgroup where flags=4 limit 0,29 "); 
  	}
  	else
  	{
  		sprintf(caTemp,"select groupid,name from ncfacgroup where flags=0 limit 0,29 "); 	
  	}
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur){
    	
    	psDbHead = utPltInitDb();
    	
	    memset(caCode,0,sizeof(caCode));
	    memset(caName,0,sizeof(caName));
	  	iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caCode,
	      	                           UT_TYPE_STRING,63,caName);
	    iNum=0;
	    while((iReturn==0)||(iReturn==1405))
	    {
	    		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
	        iNum++;
	        utPltPutLoopVar(psDbHead,"groupname", iNum,caName);
	        utPltPutLoopVar(psDbHead,"groupid",   iNum,caCode);
			    memset(caCode,0,sizeof(caCode));
			    memset(caName,0,sizeof(caName));
			  	iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caCode,
			      	                           UT_TYPE_STRING,63,caName);                              
	    }    
	    pasDbCloseCursor(psCur); 
   
  	}
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_ext_moduleman_select.html");
    return 0;
}

// 显示增加/修改模板的界面  
int ncmWebShowModuleAddModify(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    unsigned long lId,lGroupid,lTime,lTime1,lUserid,lLevel2;
    char caName[32],caGroup[32],caDispname[32],caEmail[64],caUserid[16];
    char caWhere[256],caRoleid[16],caGroupid[16];
    char *p;
    char caResult[16000],caResult1[8192],caSmt[256],fac_desc[64];
    utPltDbHead *psDbHead;
    char *pBuf,*pSelect,caLevel[12];
   	int iReturn,iRetSum,i,j,k,iSum1,iSum2,iNum,iReturn1;
    long lLevel;
//		iReturn = dsCltGetMyInfo(1,"usrlevel",&lLevel);
		lLevel2=lLevel;
		
		char temName[64] = "";
		typedef struct tmpN{
		   char tmpna[64];	
		}tmpN_s;
		tmpN_s tmpnm[30] ;
		tmpN_s lannm[30] ;
		typedef struct tmpse{
		   long mid;
		   char tmpna[64];	
		}tmpse_s;
		tmpse_s mtypenm[50] ;
		long lanNum = 0 ,temNum = 0,mid = 0,tNum = 0,lSid = 0;
		char caTemp[1024];
		char *pBuf1,*pSelect1;
		pasDbCursor *psCur;
		char caMgid[16] = "",mname[64] = "",mtype[64] = "",tmpMname[64] = "";
#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
#endif
//   	if((lLevel != 1)&&(lLevel!=2)) { 
//         utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0664系统管理"),ncLang("0137不好意思，只有系统管理员才可以使用此功能"));
//         return 0;
//   	}   	
    utMsgPrintMsg(psMsgHead);
    psDbHead = utPltInitDb();
   	iReturn = utMsgGetSomeNVar(psMsgHead,1,"mgid",    UT_TYPE_STRING, sizeof(caMgid) - 1, caMgid);
    lUserid = atol(caUserid);
    utPltPutVar(psDbHead, "mgid", caMgid);
    psCur = pasDbOpenSqlF("select name from ncfacgroup where flags = 2");
		if(psCur == NULL) {
		    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm","终端类型",ncLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
		    return 0;
		}

		memset(temName,0,sizeof(temName));
		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(temName) - 1,temName);
		iNum = 0;                                   
		while((iReturn == 0 ||iReturn==1405)) {
		    strcpy(tmpnm[iNum].tmpna,temName);    
		    iNum++;
		    memset(temName,0,sizeof(temName));
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(temName) - 1,temName);
		}
		pasDbCloseCursor(psCur);
    
    psCur = pasDbOpenSqlF("select name from ncfacgroup where flags = 1");
		if(psCur == NULL) {
		    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm","语言",ncLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
		    return 0;
		}

		memset(temName,0,sizeof(temName));
		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(temName) - 1,temName);
		iNum = 0;                                   
		while((iReturn == 0 ||iReturn==1405)) {
		    strcpy(lannm[iNum].tmpna,temName);    
		    iNum++;
		    memset(temName,0,sizeof(temName));
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(temName) - 1,temName);
		}
		pasDbCloseCursor(psCur);
    
    if(!utStrIsSpaces(caMgid)) { // 修改 
        //一组模板统一修改 传模板名称  模板id 模板类型
        sprintf(caTemp,"select count(*) from ncfacgroup where flags = 1");
    	  iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lanNum);
    	  sprintf(caTemp,"select count(*) from ncfacgroup where flags = 2");
    	  iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&temNum);
    	  if (lanNum == 0)
    	  {
    	      lanNum = 1;	
    	  }	
    	  if (temNum == 0)
    	  {
    	      temNum = 1;	
    	  }	
    	  char sTate[1024] = "",tmpchar[1024] = "";
    	  printf("**modi** lanNum = %d  temNum=%d\n",lanNum,temNum);
    	  //取选择类型
    	  psCur = pasDbOpenSqlF("select tid,mname,mtype from nctemplate where mgid = %lu  order by tid asc ",atol(caMgid));
				if(psCur == NULL) {
					        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm","模板编辑",ncLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
					        return 0;
				}
				lSid=0;
				memset(mname,0,sizeof(mname));
				memset(mtype,0,sizeof(mtype));
				iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG ,sizeof(long)     ,&lSid,
				                               UT_TYPE_STRING,sizeof(mname) - 1, mname,
				                               UT_TYPE_STRING,sizeof(mtype) - 1, mtype);
				tNum = 0;                                   
				while((iReturn == 0 ||iReturn==1405)) {
					   mtypenm[tNum].mid = lSid;
					   strcpy(mtypenm[tNum].tmpna,mtype);
					   if (tNum == 0)
					   {
					      strcpy(tmpMname,mname);	
					   }
					   tNum++;
					   lSid=0;
					   memset(mname,0,sizeof(mname));
					   memset(mtype,0,sizeof(mtype));
						 iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG ,sizeof(long)     ,&lSid,
					                                  UT_TYPE_STRING,sizeof(mname) - 1, mname,
					                                  UT_TYPE_STRING,sizeof(mtype) - 1, mtype);
				}
				pasDbCloseCursor(psCur);
				utPltPutVar(psDbHead,"mname",tmpMname); 
    	  tNum = 0;
    	  for (i = 0 ;i < temNum ; i++)
    	  {
    	  	 for(j = 0 ;j < lanNum;j++)
    	  	 {
    	  	 	  utPltPutLoopVar2(psDbHead,"temname",i + 1,j + 1,tmpnm[i].tmpna);    	  	 
    	  	    utPltPutLoopVar2(psDbHead,"lanname",i + 1,j + 1,lannm[j].tmpna); 
    	  	    
					    
    	  	    psCur = pasDbOpenSqlF("select m_name,mid from ncmodule where m_temtype= '%s' and m_lan = '%s' order by find_in_set(m_name,'默认') desc",tmpnm[i].tmpna,lannm[j].tmpna);
//							printf("***** select m_name,mid from ncmodule where m_temtype= '%s' and m_lan = '%s' order by find_in_set(m_name,'默认') desc  \n",tmpnm[i].tmpna,lannm[j].tmpna);
							if(psCur == NULL) 
							{
							    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm","模板类型",ncLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
							    return 0;
							}
							memset(temName,0,sizeof(temName));
							mid = 0;
							iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(temName) - 1,temName
							                              ,UT_TYPE_LONG,  sizeof(long)       ,&mid);
							iNum = 0;     
							pBuf1 = (char *)malloc(sizeof(char)*1024);                              
							while((iReturn == 0 ||iReturn==1405)) 
							{
//							    printf("tmpchar = %s \n",tmpchar);
							    if (iNum == 0)
							    { 
//							    	sprintf(tmpchar,"请选择您需要的模板,0;");
//							    	strcpy(pBuf1,tmpchar);
							    	memset(tmpchar,0,sizeof(tmpchar));
//							    	sprintf(tmpchar,"%s,%d;",temName,mid);
							    	sprintf(tmpchar,"%s,%s;",temName,temName);
							    	strcpy(pBuf1,tmpchar);
//							    	strcat(pBuf1,tmpchar);
							    }
							    else
							    {
//							    	sprintf(tmpchar,"%s,%d;",temName,mid);
							    	sprintf(tmpchar,"%s,%s;",temName,temName);
							    	strcat(pBuf1,tmpchar);
							    }		
							    iNum++;
							    memset(temName,0,sizeof(temName));
							    memset(tmpchar,0,sizeof(tmpchar));
							    mid = 0;
							    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(temName) - 1,temName
							                                  ,UT_TYPE_LONG,  sizeof(long)       ,&mid);
							}
							pasDbCloseCursor(psCur);
    	  	    printf("pBuf1 = %s \n",pBuf1);
					    if(pBuf1) {
                  pSelect1= strUtlCvtInputSelect("mse",mtypenm[tNum].tmpna,pBuf1,"getmoduleValue",230); 
					        printf("pSelect1 = %s \n",pSelect1);
					        free(pBuf1);
					        utPltPutLoopVar2(psDbHead,"mtype" ,i + 1,j + 1,pSelect1); 
					        utPltPutLoopVarF2(psDbHead,"se_mid",i + 1,j + 1,"%lu",mtypenm[tNum].mid); 
					        tNum++;
//					        free(pSelect1);
					    }
					    else
					    {
//					    		free(pBuf1);
					    }	
    	  	 }
    	  }
    }
    else      // 增加新记录 
    { 
    	  //按格式循环正常循环  表格特定区域不输出
    	  sprintf(caTemp,"select count(*) from ncfacgroup where flags = 1");
    	  iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lanNum);
    	  sprintf(caTemp,"select count(*) from ncfacgroup where flags = 2");
    	  iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&temNum);
    	  if (lanNum == 0)
    	  {
    	      lanNum = 1;	
    	  }	
    	  if (temNum == 0)
    	  {
    	      temNum = 1;	
    	  }	
    	  char sTate[1024] = "",tmpchar[1024] = "";
    	  printf(" lanNum = %d  temNum=%d\n",lanNum,temNum);
    	  for (i = 0 ;i < temNum ; i++)
    	  {
    	  	 for(j = 0 ;j < lanNum;j++)
    	  	 {
    	  	 	  utPltPutLoopVar2(psDbHead,"temname",i + 1,j + 1,tmpnm[i].tmpna);    	  	 
    	  	    utPltPutLoopVar2(psDbHead,"lanname",i + 1,j + 1,lannm[j].tmpna); 
    	  	    //按条件输出选择框
    	  	    psCur = pasDbOpenSqlF("select m_name,mid from ncmodule where m_temtype= '%s' and m_lan = '%s' order by find_in_set(m_name,'默认') desc",tmpnm[i].tmpna,lannm[j].tmpna);
//							printf("***** select m_name,mid from ncmodule where m_temtype= '%s' and m_lan = '%s' order by find_in_set(m_name,'默认') desc  \n",tmpnm[i].tmpna,lannm[j].tmpna);
							if(psCur == NULL) {
							    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm","模板类型",ncLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
							    return 0;
							}
							memset(temName,0,sizeof(temName));
							mid = 0;
							iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(temName) - 1,temName
							                              ,UT_TYPE_LONG,  sizeof(long)       ,&mid);
							
							iNum = 0;     
							pBuf1 = (char *)malloc(sizeof(char)*1024);                              
							while((iReturn == 0 ||iReturn==1405)) 
							{
							    if (iNum == 0)
							    { 
							    	memset(tmpchar,0,sizeof(tmpchar));
							    	sprintf(tmpchar,"%s,%s;",temName,temName);
							    	strcpy(pBuf1,tmpchar);
							    }
							    else
							    {
							    	sprintf(tmpchar,"%s,%s;",temName,temName);
							    	strcat(pBuf1,tmpchar);
							    }		
							    iNum++;
							    memset(temName,0,sizeof(temName));
							    memset(tmpchar,0,sizeof(tmpchar));
							    mid = 0;
							    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(temName) - 1,temName
							                                  ,UT_TYPE_LONG,  sizeof(long)       ,&mid);
							}
							pasDbCloseCursor(psCur);
    	  	    
    	  	    printf("pBuf1 = %s \n",pBuf1);
					    if(pBuf1) 
					    {
                  pSelect1= strUtlCvtInputSelect("mid",sTate,pBuf1,"getmoduleValue",230); 
					        printf("pSelect1 = %s \n",pSelect1);
					        free(pBuf1);
					        utPltPutLoopVar2(psDbHead,"mtype",i + 1,j + 1,pSelect1); 
//					        free(pSelect1);
					    }
					    else
					    {
//					    		free(pBuf1);
					    }	
    	  	 }
    	  }
    	  
        utPltPutVarF(psDbHead,"groupid","%lu",0);
        utPltPutVar(psDbHead, "groupname", "\0");
    }
    
    utPltShowDb(psDbHead); 
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncmWebshow_moduledef_addmodi.htm");
    return 0;
}
// 增加或修改模板信息  
int ncmModuleDefInfoSave(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
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
    char caKey2[64],caKey22[64],fac_desc[64]="";
    char *p;
    unsigned long lTime,lUserid,lUsrlevel,lGroupid,lRoleid;
    int iReturn,i,iSum;
    long lLevel;
    
    char caMgid[12] = "",caMname[64] = "" , module_se[4096] = "",temName[64] = "",caTemp[2048] = "";
    typedef struct tmpN{
		   char tmpna[64];	
		}tmpN_s;
		tmpN_s tmpnm[30] ;
		tmpN_s lannm[30] ;
		
		long lanNum = 0 ,temNum = 0,mid = 0,iNum = 0 , j = 0;
		char platepath[256] = "",se_mid[1024] = "";
		pasDbCursor *psCur;
    utPltDbHead *psDbHead;
    typedef struct tmpse{
		   long mid;
		   char tmpna[64];	
		}tmpse_s;
		tmpse_s typenm[50] ;
		char *pBuf1,*pSelect1;
		char mname[64] = "",mtype[64] = "",tmpMname[64] = "";
		long maxTid = 0,maxMgid = 0,tNum = 0,lSid = 0;
		tmpse_s mtypenm[50] ;
	iReturn = dsCltGetMyInfo(2,"usrlevel",&lLevel,
	                           "username",caMyname);
	                            
  dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,16,caMyname);	                            
#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
#endif
    utMsgPrintMsg(psMsgHead);
    lTime = time(0);
   	iReturn = utMsgGetSomeNVar(psMsgHead,13,
   	                "userid",      UT_TYPE_STRING, 10, caUserid,
   	                "name",        UT_TYPE_STRING, 31, caUsername,
   	                "dispname",    UT_TYPE_STRING, 31, caDispname,
   					        "password",    UT_TYPE_STRING, 31, caPassword1,
   					        "password2",   UT_TYPE_STRING, 31, caPassword2,
   						      "key2",        UT_TYPE_STRING, 31, caKey2,
   			        		"key22",       UT_TYPE_STRING, 31, caKey22,
   					        "groupid",     UT_TYPE_STRING, 10, caGroupid,
   					        "usrlevel",    UT_TYPE_STRING, 31, caLevel,
   					        "keyID",       UT_TYPE_STRING, sizeof(se_mid) - 1   , se_mid,
   					        "module_se",   UT_TYPE_STRING, sizeof(module_se) - 1, module_se,
   					        "mname",       UT_TYPE_STRING, sizeof(caMname)   - 1, caMname,
   					        "mgid",        UT_TYPE_STRING, sizeof(caMgid)    - 1, caMgid);
    lUserid = atol(caUserid);
    lGroupid = atol(caGroupid);
    iSum = atol(caSum);
    psDbHead = utPltInitDb();
    iNum = 0;
    for (p = strtok(module_se,"^"); p; p = strtok(NULL,"^"))
    {
        strcpy(typenm[iNum].tmpna,p); 
//        printf("%s \n",p);
        iNum++;	
    }
    iNum = 0;
    printf("se_mid =%s\n",se_mid);
    for (p = strtok(se_mid,"^"); p; p = strtok(NULL,"^"))
    {
         typenm[iNum].mid = atol(p); 
        iNum++;	
    }
    if(lGroupid == 0) {
        strcpy(caGroupname,ncLang("0446全部"));
    }
    else {
        iReturn = pasDbOneRecord("select groupname from ncgroup where groupid = :gid",1,
                            "gid",UT_TYPE_LONG,lGroupid,
                            UT_TYPE_STRING,31,caGroupname);
        if(iReturn != 0) {
             utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncLang("0839增加用户"),ncLang("0900组不存在"));
             return 0;
        }
    }
    lUsrlevel = atol(caLevel);
    
    sprintf(caTemp,"select count(*) from ncfacgroup where flags = 1");
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lanNum);
    sprintf(caTemp,"select count(*) from ncfacgroup where flags = 2");
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&temNum);
    if (lanNum == 0)
    {
    	      lanNum = 1;	
    }	
    if (temNum == 0)
    {
    	      temNum = 1;	
    }	
    char sTate[1024] = "",tmpchar[1024] = "";
    printf("**modi** lanNum = %d  temNum=%d\n",lanNum,temNum);
    
    tNum = 0;
    psCur = pasDbOpenSqlF("select name from ncfacgroup where flags = 2");
		if(psCur == NULL) {
				    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm","终端类型",ncLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
				    return 0;
		}
				
		memset(temName,0,sizeof(temName));
		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(temName) - 1,temName);
		tNum = 0;                                   
		while((iReturn == 0 ||iReturn==1405)) {
				    strcpy(tmpnm[tNum].tmpna,temName);    
				    tNum++;
				    memset(temName,0,sizeof(temName));
				    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(temName) - 1,temName);
		}
		pasDbCloseCursor(psCur);
		    
		psCur = pasDbOpenSqlF("select name from ncfacgroup where flags = 1");
		if(psCur == NULL) {
				    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm","语言",ncLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
				    return 0;
		}
		memset(temName,0,sizeof(temName));
		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(temName) - 1,temName);
		tNum = 0;                                   
		while((iReturn == 0 ||iReturn==1405)) {
				    strcpy(lannm[tNum].tmpna,temName);    
				    tNum++;
				    memset(temName,0,sizeof(temName));
				    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(temName) - 1,temName);
		}
		pasDbCloseCursor(psCur);
    
    if (strlen(caMgid) == 0)               //组id为空，整组插入
    {			
    	  sprintf(caTemp,"select max(mgid)+1 from nctemplate ");
    	  printf("caTemp = %s \n",caTemp);
    	  iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,sizeof(long),&maxMgid);
    	  if (maxMgid == 0)
    	  {
    	     maxMgid = 1;	
    	  }	
    	  sprintf(caMgid,"%d",maxMgid);
    	     printf("**********caMgid = %s\n",caMgid);
    	  for (i = 0 ;i < temNum ; i++)
    	  {
    	  	 for(j = 0 ;j < lanNum;j++)
    	  	 {
    	  	 	  if (strlen(module_se) ==0)
    	  	 	  {
    	  	 	      strcpy(typenm[iNum].tmpna,"默认");	
    	  	 	  }	
    	  	 	  sprintf(caTemp,"select max(tid)+1 from nctemplate ");
    	  	 	  printf("caTemp = %s \n",caTemp);
    	        iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,sizeof(long),&maxTid);
    	        if (maxTid == 0)
    	        {
    	           maxTid = 1;	
    	        }	
    	        
    	        sprintf(caTemp,"select platepath from ncmodule where m_lan='%s' and m_temtype='%s' and m_name='%s'",lannm[j].tmpna,tmpnm[i].tmpna,typenm[iNum].tmpna);
    	        printf("caTemp = %s \n",caTemp);
    	        iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_STRING,sizeof(platepath) - 1,platepath);
    	  	 	  sprintf(caSql,"insert into nctemplate (tid,mname,mtype,mtemtype,mlan,platepath,mgid,mesg,msystime) values \
                           (%lu,'%s','%s','%s','%s','%s',%lu,'%s',%lu)\n",
                           maxTid,caMname,typenm[iNum].tmpna,tmpnm[i].tmpna,lannm[j].tmpna,platepath,maxMgid,"",time(0));
    	  	 	  printf("caSql = %s \n",caSql);
    	  	 	  iReturn=pasDbExecSql(caSql,0); 	
    	  	 	  iNum++;
    	  	 	  if(iReturn == 0) 
    	  	 	  {
	                ncAdmWriteLog(caMyname,caIp,"新增模板",caUsername);
	            }
	            else 
	            {
	               utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm","增加模板","增加模板出错");
	            }
	            maxTid=0;
    	  	 	  memset(platepath,0,sizeof(platepath));
    	  	 	  

    	  	 }
    	  }
    }
    else 
    { // 修改记录  
      for (i = 0;i < iNum; i++)
      {
        sprintf(caSql,"update nctemplate set mtype = '%s' where tid = %d ", typenm[i].tmpna,typenm[i].mid);
        printf("= 1111111111 caSql= %s\n",caSql);
        iReturn=pasDbExecSql(caSql,0); 	
        if(iReturn == 0) 
        {
//        	printf("= 1111111111=\n");
//        		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncmsysman/ncadminuser.htm");
//            ncWebAdminUserList(psShmHead, iFd,psMsgHead);
            ncAdmWriteLog(caMyname,caIp,"修改模板定义类型成功",caUsername);
        }
        else 
        {
           utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm","修改模板","修改模板",iReturn);
        }            
      }
      sprintf(caSql,"update nctemplate set mname = '%s' where mgid = %d ", caMname,atol(caMgid));
	    printf("= 1111111111 caSql= %s\n",caSql);
	    iReturn=pasDbExecSql(caSql,0); 	
    }
    
    
    //取选择类型
    psCur = pasDbOpenSqlF("select tid,mname,mtype from nctemplate where mgid = %lu  order by tid asc ",atol(caMgid));
		if(psCur == NULL) 
		{
					        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm","模板编辑",ncLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
					        return 0;
		}
		lSid=0;
		memset(mname,0,sizeof(mname));
		memset(mtype,0,sizeof(mtype));
		iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG ,sizeof(long)     ,&lSid,
				                           UT_TYPE_STRING,sizeof(mname) - 1, mname,
				                           UT_TYPE_STRING,sizeof(mtype) - 1, mtype);
		tNum = 0;                                   
		while((iReturn == 0 ||iReturn==1405)) {
					   mtypenm[tNum].mid = lSid;
					   strcpy(mtypenm[tNum].tmpna,mtype);
					   if (tNum == 0)
					   {
					      strcpy(tmpMname,mname);	
					   }
					   tNum++;
					   lSid=0;
					   memset(mname,0,sizeof(mname));
					   memset(mtype,0,sizeof(mtype));
						 iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG ,sizeof(long)     ,&lSid,
					                                  UT_TYPE_STRING,sizeof(mname) - 1, mname,
					                                  UT_TYPE_STRING,sizeof(mtype) - 1, mtype);
		}
		pasDbCloseCursor(psCur);
		utPltPutVar(psDbHead,"mname",tmpMname); 
		utPltPutVar(psDbHead,"mgid",caMgid); 
		tNum = 0;
    for (i = 0 ;i < temNum ; i++)
    {
    	  	 for(j = 0 ;j < lanNum;j++)
    	  	 {
    	  	 	  utPltPutLoopVar2(psDbHead,"temname",i + 1,j + 1,tmpnm[i].tmpna);    	  	 
    	  	    utPltPutLoopVar2(psDbHead,"lanname",i + 1,j + 1,lannm[j].tmpna); 
    	  	    
    	  	    psCur = pasDbOpenSqlF("select m_name,mid from ncmodule where m_temtype= '%s' and m_lan = '%s' order by find_in_set(m_name,'默认') desc",tmpnm[i].tmpna,lannm[j].tmpna);
//							printf("**save*** select m_name,mid from ncmodule where m_temtype= '%s' and m_lan = '%s' order by find_in_set(m_name,'默认') desc  \n",tmpnm[i].tmpna,lannm[j].tmpna);
							if(psCur == NULL) 
							{
							    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm","模板类型",ncLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
							    return 0;
							}
							memset(temName,0,sizeof(temName));
							mid = 0;
							iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(temName) - 1,temName
							                              ,UT_TYPE_LONG,  sizeof(long)       ,&mid);
							iNum = 0;     
							pBuf1 = (char *)malloc(sizeof(char)*1024);                              
							while((iReturn == 0 ||iReturn==1405)) 
							{
//							    printf("tmpchar = %s \n",tmpchar);
							    if (iNum == 0)
							    { 
							    	memset(tmpchar,0,sizeof(tmpchar));
							    	sprintf(tmpchar,"%s,%s;",temName,temName);
							    	strcpy(pBuf1,tmpchar);
							    }
							    else
							    {
							    	sprintf(tmpchar,"%s,%s;",temName,temName);
							    	strcat(pBuf1,tmpchar);
							    }		
							    iNum++;
							    memset(temName,0,sizeof(temName));
							    memset(tmpchar,0,sizeof(tmpchar));
							    mid = 0;
							    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(temName) - 1,temName
							                                  ,UT_TYPE_LONG,  sizeof(long)       ,&mid);
							}
							pasDbCloseCursor(psCur);
    	  	    
    	  	    printf("pBuf1 = %s \n",pBuf1);
					    if(pBuf1) {
                  pSelect1= strUtlCvtInputSelect("mse",mtypenm[tNum].tmpna,pBuf1,"getmoduleValue",230); 
					        printf("pSelect1 = %s \n",pSelect1);
					        free(pBuf1);
					        utPltPutLoopVar2(psDbHead,"mtype" ,i + 1,j + 1,pSelect1); 
					        utPltPutLoopVarF2(psDbHead,"se_mid",i + 1,j + 1,"%lu",mtypenm[tNum].mid); 
					        tNum++;
//					        free(pSelect1);
					    }
					    
    	  	 }
    }
    
  	
    utPltShowDb(psDbHead); 
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncmWebshow_moduledef_addmodi.htm");
    return 0;
}
//模板定义
int ncmWebModuleDefine(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iNum,iReturn;
    utPltDbHead *psDbHead;
    char *p;
    int  i=0;
   	char caGid[16],caDid[16],caAdd[16],caDel[16];
		char caName[32],caModi[16],caApp[16];
		char caDevname[64],caCaptype[10],caFlags[2];
		long sid=0;
		char caSid[16];
		char caTemp[2024];
		long promisc=0,lTotRec=0,tmpcode=0,lSid=0;
	 	pasDbCursor *psCur;
    char captype_desc[32]="",flags_desc[8]="",caPromisc[16]="";
    char caTemp_d[256],caSesid[1024],caMsg[1024],caUsername2[32]="";
    
    long mid=0,mgid = 0;
    char m_name[64] = "", m_type[32] = "", m_temtype[32] = "", m_lan[32] = "", m_comp[32] = "" , htmlpath[256] = "";
    char tempflag[16] = "", lan_flag[16] = "",mokeyword[64] = "" ,modtype[32] = "" ,m_modtype[32] = "",m_plate[256] = "";
    
    utMsgPrintMsg(psMsgHead);
    dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
	  iReturn = utMsgGetSomeNVar(psMsgHead,15,
	                "m_name",        UT_TYPE_STRING,sizeof(m_name)-1    ,m_name,
	                "m_type",        UT_TYPE_STRING,sizeof(m_type)-1    ,m_type,
	                "m_temtype",     UT_TYPE_STRING,sizeof(m_temtype)-1 ,m_temtype,
	                "m_lan",         UT_TYPE_STRING,sizeof(m_lan)-1     ,m_lan,
	                "m_modtype",     UT_TYPE_STRING,sizeof(m_modtype)-1 ,m_modtype,    
	                "tempflag",      UT_TYPE_STRING,sizeof(m_comp)-1    ,tempflag,      //终端类型
	                "lan_flag",      UT_TYPE_STRING,sizeof(m_comp)-1    ,lan_flag,         //语言
	                "mokeyword",     UT_TYPE_STRING,sizeof(mokeyword)-1 ,mokeyword,    
	                "m_plate",       UT_TYPE_STRING,sizeof(m_plate)-1   ,m_plate,    
	                "m_mid",         UT_TYPE_STRING,12,caSid,
	                "add",           UT_TYPE_STRING,12,caAdd,
	                "del",           UT_TYPE_STRING,12,caDel,
	                "selsid",        UT_TYPE_STRING,1000,caSesid,
	                "update",        UT_TYPE_STRING,12,caModi,
	                "app",           UT_TYPE_STRING,11,caApp);
	  
	  trim(mokeyword);
	  //未操作选择框
	  if(strlen(m_name)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,m_name,caTemp,sizeof(m_name)-1));
    	strcpy(m_name,caTemp_d);
    }
    if(strlen(m_type)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,m_type,caTemp,sizeof(m_type)-1));
    	strcpy(m_type,caTemp_d);
    }
    if(strlen(m_temtype)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,m_temtype,caTemp,sizeof(m_temtype)-1));
    	strcpy(m_temtype,caTemp_d);
    }
    if(strlen(m_lan)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,m_lan,caTemp,sizeof(m_lan)-1 ));
    	strcpy(m_lan,caTemp_d);
    }
    if(strlen(m_modtype)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,m_modtype,caTemp,sizeof(m_modtype)-1));
    	strcpy(m_modtype,caTemp_d);
    }
    if(strlen(tempflag)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,tempflag,caTemp,sizeof(tempflag)-1));
    	strcpy(tempflag,caTemp_d);
    }
    if(strlen(lan_flag)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,lan_flag,caTemp,sizeof(lan_flag)-1));
    	strcpy(lan_flag,caTemp_d);
    }
	  if(strlen(mokeyword)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,mokeyword,caTemp,sizeof(mokeyword)-1));
    	strcpy(mokeyword,caTemp_d);
    }
	  if(strlen(m_plate)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,m_plate,caTemp,sizeof(m_plate)-1));
    	strcpy(m_plate,caTemp_d);
    }
	  if(strlen(captype_desc) > 2)
	  {
		  	sprintf(caTemp,"select code from ncwethetype where flag=1 and name='%s'  ",captype_desc);
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&tmpcode);  
	  		memset(caCaptype,0,sizeof(caCaptype));
	  		sprintf(caCaptype,"%d",tmpcode); 
	  }
	  else{
	  		strcpy(caCaptype,captype_desc);	
	  }              
    
    printf("caCaptype=%s  caPromisc=%s  caFlags=%s \n",caCaptype,caPromisc,caFlags);
    
    if(strcmp(caModi,"update")==0){  //增加一个组 
       if(atol(caSid) == 0)
       {  		
//         		unsigned long lSid=pasGetSid("ncnetdev","sid");
         		sprintf(caTemp,"select max(tid)+1 from nctemplate   ");
  					pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSid);  
         		printf("lSid=%d \n",lSid);
         		if(lSid == 0)
         		{
         			lSid=1;	
         		}
            iReturn = pasDbExecSqlF("insert into nctemplate(tid,mname,mtype,mtemtype,mlan,platepath,modtype,msystime) values(%lu,'%s','%s','%s','%s','%s','%s',%lu)",lSid,m_name,m_type,m_temtype,m_lan,m_plate,m_modtype,time(0));
            printf("insert into nctemplate(tid,mname,mtype,mtemtype,mlan,platepath,modtype,msystime) values(%lu,'%s','%s','%s','%s','%s','%s',%lu) \n",lSid,m_name,m_type,m_temtype,m_lan,m_plate,m_modtype,time(0));
            if(iReturn != 0) {
                utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","模板定义",ncLang("0310记录增加出错"));
                pasDbRollback(NULL);
                return 0;
            }        
            else {
                pasDbCommit(NULL);
            }      
       }else {   //更新
          iReturn = pasDbExecSqlF("update nctemplate set mname='%s',mtype='%s',mtemtype='%s',mlan='%s',platepath='%s',modtype='%s' where tid = %s ",
                         m_name,m_type,m_temtype,m_lan,m_plate,m_modtype,caSid);
                      printf("update nctemplate set mname='%s',mtype='%s',mtemtype='%s',mlan='%s',platepath='%s',modtype='%s' where tid = %s \n",
                         m_name,m_type,m_temtype,m_lan,m_plate,m_modtype,caSid);   
	        if(iReturn != 0) {
	            utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm",ncLang("0086Web地址列表"),ncLang("0700修改出错%s"),pasDbErrorMsg(NULL));
	            return 0;
	        }  
        	pasDbCommit(NULL);
    	 }              
       
    }
    else if(!utStrIsSpaces(caDel)) { // 删除一个组  
	    if(strlen(caSesid)>0){
//	    	    sprintf(caTemp,"select mgid from nctemplate where tid  in(%s)",caSesid);
//  					pasDbOneRecord(caTemp,0,UT_TYPE_LONG,sizeof(long),&lSid); 
  					psCur = pasDbOpenSqlF("select mgid from nctemplate where tid in(%s) ",caSesid);
						if(psCur == NULL) 
						{
									utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm","模板删除","模板删除出错",pasDbErrorMsg(NULL));
									return 0;
						}
						lSid=0;
						iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG ,sizeof(long)     ,&lSid);
						int tNum = 0;   
						char tmpvar[1024] = "" ,tarId[1024] = "";                                
						while((iReturn == 0 ||iReturn==1405)) {
								sprintf(tmpvar,"%d",lSid);
								if (tNum == 0)
								{
								    strcpy(tarId,tmpvar);	
								}
								else
								{
									strcat(tarId,",");	
									strcat(tarId,tmpvar);	
								}
								tNum++;			  
								lSid=0;
								memset(tmpvar,0,sizeof(tmpvar));
								iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG ,sizeof(long)     ,&lSid);
						}
						pasDbCloseCursor(psCur);
  					
  					
  			
 	 	        iReturn = pasDbExecSqlF("delete from nctemplate where mgid in(%s)",tarId);
		        if(iReturn != 0) {
		            pasDbRollback(NULL);
		        }  
		        else{
//		        	pasDbExecSqlF("delete from ncnetmonip where did=%s ",p);
		        }  
		        pasDbCommit(NULL);
		        sprintf(caMsg,"delete from nctemplate where mgid in(%s) ",tarId);
            if(iReturn==0){
            		ncmWriteSysLog(psMsgHead,caUsername2,"05","成功",caMsg); 
            }
            else{
            		ncmWriteSysLog(psMsgHead,caUsername2,"05","失败",caMsg);
            }
		  }
    }
    else if(!utStrIsSpaces(caApp)) { /* 应用  */
       // By Liyunmng  ncPkgCapApp(psShmHead);
//        ncUtlLoadGroupIpLst(psShmHead);
    }
         
      psDbHead = utPltInitDb();
      sprintf(caTemp,"select count(*) from nctemplate  where 1 = 1 ");
      if (strlen(tempflag) > 0)
      {	
      	sprintf(caTemp+strlen(caTemp)," and mtemtype = '%s' ",tempflag);
    	}
      if (strlen(lan_flag) > 0)
      {	
      	sprintf(caTemp+strlen(caTemp)," and mlan = '%s' ",lan_flag);
    	}
    	if (strlen(mokeyword) > 0)
      {	
      	sprintf(caTemp+strlen(caTemp)," and (mname like '%c%s%c' or modtype like '%c%s%c' ) ",'%',mokeyword,'%','%',mokeyword,'%');
    	}
    	printf("caTemp=%s \n",caTemp);
  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lTotRec);   
      sprintf(caTemp,"select tid,mname,mtype,mtemtype,mlan,platepath,mgid from nctemplate where 1 = 1 ");
      if (strlen(tempflag) > 0)
      {	
      	sprintf(caTemp+strlen(caTemp)," and mtemtype = '%s' ",tempflag);
    	}
      if (strlen(lan_flag) > 0)
      {	
      	sprintf(caTemp+strlen(caTemp)," and mlan = '%s' ",lan_flag);
    	}
    	if (strlen(mokeyword) > 0)
      {	
      	sprintf(caTemp+strlen(caTemp)," and (mname like '%c%s%c' or modtype like '%c%s%c' ) ",'%',mokeyword,'%','%',mokeyword,'%');
    	}
    	psCur=pasDbOpenSql(caTemp,0);

      printf("222caTemp=%s \n",caTemp);
      if(psCur==NULL){
      	    utPltFreeDb(psDbHead);
            utWebDispMsg(iFd,psMsgHead,"ncmsysman/ncmsg_back.htm","模板维护查询",ncLang("0174出错 %s"),pasDbErrorMsg(NULL));
            return 0;
      }
      mid=0;	
      memset(m_name,0,sizeof(m_name));
      memset(m_type,0,sizeof(m_type));
      memset(m_temtype,0,sizeof(m_temtype));
      memset(m_lan,0,sizeof(m_lan));
      memset(modtype,0,sizeof(modtype));
      memset(htmlpath,0,sizeof(htmlpath));
       
      iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,  sizeof(long),&mid,
                                   UT_TYPE_STRING,sizeof(m_name)-1,m_name,
                                   UT_TYPE_STRING,sizeof(m_type)-1,m_type,
                                   UT_TYPE_STRING,sizeof(m_temtype)-1,m_temtype,
                                   UT_TYPE_STRING,sizeof(m_lan)-1,m_lan,
 //                                  UT_TYPE_STRING,sizeof(modtype)-1,modtype,
                                   UT_TYPE_STRING,sizeof(htmlpath)-1,htmlpath,
                                   UT_TYPE_LONG,  sizeof(long),&mgid);
      				  
      iNum=0;
      while(iReturn==0){
      	if(iNum>0){
        	utPltPutLoopVar(psDbHead,"m_dh",iNum+1,",");
        }
      	iNum++;
	      utPltPutLoopVarF(psDbHead,"m_iNum"   ,iNum,"%lu",iNum);
      	utPltPutLoopVarF(psDbHead,"m_mid"    ,iNum,"%lu",mid);
      	utPltPutLoopVar(psDbHead,"m_name"    ,iNum,m_name);
      	utPltPutLoopVar(psDbHead,"m_type"    ,iNum,m_type);
      	utPltPutLoopVar(psDbHead,"m_lan"     ,iNum,m_lan);
      	utPltPutLoopVar(psDbHead,"m_temtype" ,iNum,m_temtype);
//      	utPltPutLoopVar(psDbHead,"m_modtype" ,iNum,modtype);
      	utPltPutLoopVar(psDbHead,"platepath" ,iNum,htmlpath);
        utPltPutLoopVarF(psDbHead,"mgid"     ,iNum,"%lu",mgid);
	      mid=0;		 
	      memset(m_name,0,sizeof(m_name));
	      memset(m_type,0,sizeof(m_type));
	      memset(m_temtype,0,sizeof(m_temtype));
	      memset(m_lan,0,sizeof(m_lan));
//	      memset(modtype,0,sizeof(modtype));
	      memset(htmlpath,0,sizeof(htmlpath));
	      mgid = 0;

	      iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,sizeof(long),&mid,
	                                   UT_TYPE_STRING,sizeof(m_name)-1,m_name,
	                                   UT_TYPE_STRING,sizeof(m_type)-1,m_type,
	                                   UT_TYPE_STRING,sizeof(m_temtype)-1,m_temtype,
	                                   UT_TYPE_STRING,sizeof(m_lan)-1,m_lan,
//	                                   UT_TYPE_STRING,sizeof(modtype)-1,modtype,
	                                   UT_TYPE_STRING,sizeof(htmlpath)-1,htmlpath,
	                                   UT_TYPE_LONG,  sizeof(long),&mgid);
  		}
       
    pasDbCloseCursor(psCur);
    utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
//    utPltShowDb(psDbHead); 
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_ext_webshowmodule_def.htm");
      
    return 0;
}
//模板定义选择
int ncmWebModuleDefineSelect(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
   	char caCurPg[16],caTotRec[16];
   	int  iNum;
   	long lGroupid,lGroupid0,lCurPg,lTotRec,lRowNum,lStartRec;
    utPltDbHead *psDbHead;
   	int iReturn,i;
    char caCode[16],caName[68],flag[4] = "" ,modtype[20] = "";
    char caTemp[1024];
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead,2,"flag",     UT_TYPE_STRING, sizeof(flag) - 1,    flag
                                          ,"pmodtype", UT_TYPE_STRING, sizeof(modtype) - 1, modtype);
    
    if(strlen(flag) > 0 && atoi(flag) == 1)
    {	
    	//选择模板类型， 判断是终端类型还是语言                              
    	sprintf(caTemp,"select distinct m_lan,mid from ncmodule where modtype='%s' group by m_lan limit 0,29 ",modtype); 
    	
  	}else if(strlen(flag) > 0 && atoi(flag) == 2)
  	{	
  		sprintf(caTemp,"select distinct m_temtype,mid from ncmodule where modtype='%s' group by m_temtype limit 0,29 ",modtype); 
  	}
  	else if(strlen(flag) > 0 && atoi(flag) == 3)
  	{	
  		sprintf(caTemp,"select distinct m_type,mid from ncmodule where modtype='%s' group by m_type limit 0,29 ",modtype); 
  	}
  	else
  	{
  		sprintf(caTemp,"select distinct modtype,mid from ncmodule where 1 = 1 and modtype != '' group by modtype limit 0,29 ",modtype); 
  	}
  	printf("caTemp=%s  \n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur){
    	
    	psDbHead = utPltInitDb();
    	
	    memset(caCode,0,sizeof(caCode));
	    memset(caName,0,sizeof(caName));
	  	iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(caName) - 1,caName,
	      	                           UT_TYPE_STRING,sizeof(caCode) - 1,caCode);
	    iNum=0;
	    while((iReturn==0)||(iReturn==1405))
	    {
	    		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
	        iNum++;
	        utPltPutLoopVar(psDbHead,"groupname", iNum,caName);
	        utPltPutLoopVar(psDbHead,"groupid",   iNum,caCode);
			    memset(caCode,0,sizeof(caCode));
			    memset(caName,0,sizeof(caName));
			  	iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,sizeof(caName) - 1,caName,
	      	                               UT_TYPE_STRING,sizeof(caCode) - 1,caCode);                           
	    }    
	    pasDbCloseCursor(psCur); 
   
  	}
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncpmodule/ncm_ext_modulemaintain_select.html");
    return 0;
}
