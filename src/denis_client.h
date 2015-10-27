#ifndef __DENIS_CLIENT__
#define __DENIS_CLIENT__

int GetVipCardCallback(struct _tempuri__GetVipCard2Response *resp)
{
	if(_tempuri__GetVipCard2Response)
	printf("resp is : %d , msg is : %s \n" , resp->GetVipCard2Result , resp->msg);
}

int RegisterCallback(struct _tempuri__VipRegisterInfoResponse *resp)
{
	
	printf("coming \n");
}

int Denis_GetVipCard2(char *url,struct SOAP_ENV__Header *header,struct _tempuri__GetVipCard2 *in,int (*Callback)(struct _tempuri__GetVipCard2Response *resp))
{
 int status = -1;
 struct soap soap;
 struct _tempuri__GetVipCard2Response out;
  
 soap_init(&soap);
 soap_imode(&soap, SOAP_C_UTFSTRING);
 soap_omode(&soap, SOAP_C_UTFSTRING);
 
 soap.header = header;
 soap.version = 1;
 
 status = soap_call___tempuri__GetVipCard2(&soap,url,NULL,in,&out);
 (*Callback)(&out);
 
 soap_destroy(&soap);
 soap_end(&soap);
 soap_done(&soap);
 
 return status;
}

int Denis_Register(char *url,struct SOAP_ENV__Header *header,struct _tempuri__VipRegisterInfo *in,int (*Callback)(struct _tempuri__VipRegisterInfoResponse *resp))
{
 int status = -1;
 struct soap soap;
 struct _tempuri__VipRegisterInfoResponse out;
  
 soap_init(&soap);
 soap_imode(&soap, SOAP_C_UTFSTRING);
 soap_omode(&soap, SOAP_C_UTFSTRING);
 
 soap.header = header;
 soap.version = 1;
 
 status = soap_call___tempuri__VipRegisterInfo(&soap,url,NULL,in,&out);
 (*Callback)(&out);
 
 soap_destroy(&soap);
 soap_end(&soap);
 soap_done(&soap);
 
 return status;
}

#endif