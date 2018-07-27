#include <stdint.h>
#include "QCamImgfnc.h"
#include "QCamApi.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//----------------------------------------------------------------------------------------

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


using namespace cv;

//----------------------------------------------------------------------------------------

QCam_SettingsEx   settings;      // Main camera settings for normal mode.
QCam_Handle 	m_handle = 0;
QCam_Frame	frame;
Mat		image;

//----------------------------------------------------------------------------------------

void info(const char *name, QCam_Param p)
{
	unsigned long val;
    unsigned long vmin;
    unsigned long vmax;

	QCam_Err errcode = QCam_GetParam((QCam_Settings *)&settings, p, &val);
    
    
    QCam_GetParamMax((QCam_Settings *)&settings, p, &vmax);
    QCam_GetParamMin((QCam_Settings *)&settings, p, &vmin);

	printf("error = %d, %s = %ld, < %d, %ld>\n", errcode, name, val, vmin, vmax);
}

//----------------------------------------------------------------------------------------

void info1(const char *name, QCam_ParamS32 p)
{
     long val;
     long vmin;
     long vmax;
    
    QCam_Err errcode = QCam_GetParamS32((QCam_Settings *)&settings, p, &val);
    
    
    QCam_GetParamS32Max((QCam_Settings *)&settings, p, &vmax);
    QCam_GetParamS32Min((QCam_Settings *)&settings, p, &vmin);
    
    printf("error = %d, %s = %ld, < %d, %ld>\n", errcode, name, val, vmin, vmax);
}

//----------------------------------------------------------------------------------------

void info2(const char *name, QCam_Param64 p)
{
    UNSIGNED64 val;
    UNSIGNED64 vmin;
    UNSIGNED64 vmax;
    
    QCam_Err errcode = QCam_GetParam64((QCam_Settings *)&settings, p, &val);
    
    
    QCam_GetParam64Max((QCam_Settings *)&settings, p, &vmax);
    QCam_GetParam64Min((QCam_Settings *)&settings, p, &vmin);
    
    printf("error = %d, %s = %lld, < %lld, %lld>\n", errcode, name, val, vmin, vmax);
}

//----------------------------------------------------------------------------------------


void dump()
{
    info("qprmGain", qprmGain);
    info("qprmOffset", qprmOffset);
    info("qprmExposure", qprmExposure);
    info("qprmBinning", qprmBinning);
    info("qprmHorizontalBinning", qprmHorizontalBinning);
    info("qprmVerticalBinning", qprmVerticalBinning);
    info("qprmReadoutSpeed", qprmReadoutSpeed);
    info("qprmTriggerType", qprmTriggerType);
    info("qprmReadoutPort", qprmReadoutPort);

    info("qprmCoolerActive", qprmCoolerActive);
    info("qprmExposureRed", qprmExposureRed);
    info("qprmExposureBlue", qprmExposureBlue);
    info("qprmImageFormat", qprmImageFormat);
    info("qprmRoiX", qprmRoiX);
    info("qprmRoiY", qprmRoiY);
    info("qprmRoiWidth", qprmRoiWidth);
    info("qprmRoiHeight", qprmRoiHeight);
    info("qprmCameraMode", qprmCameraMode);
    
    info("qprmSyncb", qprmSyncb);
    info("qprmCameraMode", qprmCameraMode);
    info("qprmNormalizedGain", qprmNormalizedGain);
    info("qprmDoPostProcessing", qprmDoPostProcessing);
    info("qprmRoiX", qprmRoiX);
    info("qprmBlackoutMode", qprmBlackoutMode);
    info("qprmHighSensitivityMode", qprmHighSensitivityMode);
    info("qprmFan", qprmFan);
    info("qprmPostProcessImageFormat", qprmPostProcessImageFormat);

    
    info("qprmEMGain", qprmEMGain);
    info("qprmHighSensitivityMode", qprmHighSensitivityMode);
    info("qprmEasyEmMode", qprmEasyEmMode);
    info("qprmDoPostProcessing", qprmDoPostProcessing);
    info("qprmLockedGainMode", qprmLockedGainMode);
    info("qprmBlackoutMode", qprmBlackoutMode);
    info("qprmHighSensitivityMode", qprmHighSensitivityMode);
    info("qprmCCDClearingMode", qprmCCDClearingMode);
    info("qprmOverSample", qprmOverSample);

    info1("qprmS32NormalizedGaindB", qprmS32NormalizedGaindB);
    info1("qprmS32AbsoluteOffset", qprmS32AbsoluteOffset);
    info1("qprmS32RegulatedCoolingTemp", qprmS32RegulatedCoolingTemp);


    info2("qprm64Exposure", qprm64Exposure);
    info2("qprm64ExposureRed", qprm64ExposureRed);
    info2("qprm64ExposureBlue", qprm64ExposureBlue);
    info2("qprm64NormIntensGain", qprm64NormIntensGain);


}

//----------------------------------------------------------------------------------------

int init_cam(float gain, float exp)
{
    QCam_Err            errcode;
    QCam_CamListItem    list[ 10 ];
    unsigned long       listlen = 10;
    
    
    QCam_LoadDriver();
    errcode = QCam_ListCameras( list, &listlen );
    
    printf("error %d, count %lu\n", errcode, listlen);
    
    
    if (listlen > 0) {
        
        errcode = QCam_OpenCamera( list[0].cameraId, &m_handle);
        errcode = QCam_SetStreaming( m_handle, 1);
        
        errcode = QCam_CreateCameraSettingsStruct(&settings);
        
        QCam_ReadDefaultSettings(m_handle, (QCam_Settings*)&settings);
        //dump();
        
        
        
        QCam_SetParam((QCam_Settings *)&settings, qprmEMGain, gain);
        QCam_SetParam( (QCam_Settings*) &settings, qprmReadoutSpeed, qcReadout20M);
        QCam_SetParam64( (QCam_Settings*) &settings, qprm64Exposure,exp * 1000000000.0);
        
        printf("%d\n", QCam_SetParamS32( (QCam_Settings*) &settings, qprmS32AbsoluteOffset,800));
        
        
        QCam_SetParam((QCam_Settings *)&settings, qprmImageFormat, qfmtMono16);
        
        
        
        QCam_PreflightSettings(m_handle, (QCam_Settings *)&settings);
        
        errcode = QCam_SendSettingsToCam(m_handle, (QCam_Settings*)&settings);
        return 0;
    }
    else {
        printf("no camera found\n");
        exit(-1);
        return -1;
    }
}


bool match(char *s1, const char *s2)
{
    return(strncmp(s1, s2, strlen(s2)) == 0);
}

//--------------------------------------------------------------------------------------

void help(char **argv)
{
    printf("%s -h        print this help\n", argv[0]);
    printf("exta args\n");
    printf("-gain=value\n");
    printf("-exp=value\n");
    printf("-o=filename\n");
}


//--------------------------------------------------------------------------------------


float  g_gain = 1000.0;
float  g_exp = 0.1;
char   g_fn[512];

//--------------------------------------------------------------------------------------


void params(int argc, char **argv)
{
    int pos = 1;


    while(pos < argc) {
        if (match(argv[pos], "-gain=")) {
            sscanf(strchr(argv[pos], '=') , "=%f",  &g_gain);
            argv[pos][0] = 0;
        }
        if (match(argv[pos], "-exp="))  {
            sscanf(strchr(argv[pos], '=') , "=%f",  &g_exp);
            argv[pos][0] = 0;
        }
        if (match(argv[pos], "-o="))  {
            sscanf(strchr(argv[pos], '=') , "=%s",  (char*)g_fn);
            argv[pos][0] = 0;
        }
	pos++;
    }
}

//--------------------------------------------------------------------------------------

    
int main(int argc, char **argv)
{
    QCam_Err            errcode;
    float               g_offset = 0;
    
 
    
    if (argc == 1 || strcmp(argv[1], "-h") == 0) {
        help(argv);
        return 0;
    }
    
    int mult = 1;
    int offset = 0;
    
    params(argc, argv);
    
    init_cam(g_gain, g_exp);

    image = Mat(Size(1004, 1002), CV_16UC1);
   
    cv::namedWindow( "image", 1 );

    unsigned long sizeInBytes;

    QCam_GetInfo(m_handle, qinfImageSize, &sizeInBytes);


    frame.pBuffer = image.ptr<ushort>(0);
	
    //frame.pBuffer = x;
    frame.bufferSize = sizeInBytes;

	
    FILE *f;
    
    
    char buf[512];
    
    
    sprintf(buf, "%s_%ld.ser", g_fn, time(0));
    
    char has_file = 0;
    
    if (strlen(g_fn) > 0)
        has_file = 1;
    
    
    if (has_file) f = fopen(buf, "wb");

    for (int k = 0; k < 1000000; k++) {
        errcode = QCam_GrabFrame(m_handle, &frame);
        
        float rand_offset = *image.ptr<ushort>(k%1024);
        
        if (g_offset == 0) {
            g_offset = offset;
        }
        else {
            g_offset = g_offset + (0.01 * rand_offset);
            g_offset /= 1.01;
        }
        
        if (has_file) fwrite(frame.pBuffer, 1, sizeInBytes, f);
        printf("%d %d, offset = %f\n", k, *(image.ptr<ushort>(0)), g_offset);

        if (true) {
            offset = g_offset - 500;
            if (offset < 0) offset = 0;
            
            image = image - offset;
            image = image * mult;
            
            imshow("image", image);
            char key = cvWaitKey(1);
      
            printf("key %d\n", key);
            if (key == 'f' || key == 'F') {
                //focus mode
                mult = 4.0;
                offset = 0;
            }
            if (key == 'D' || key == 'd') {
                //focus mode
                mult = 16.0;
                offset = 0;
            }
            
            
            if (key == 0x1b)
              goto out;
        }
    }
    
out:;
    
    if (has_file) fclose(f);

    QCam_CloseCamera(m_handle);
	


    QCam_ReleaseDriver();
	
    return 0;
}
