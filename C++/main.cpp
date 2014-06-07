#include <iostream>
#include <Windows.h>
#include <CL/cl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>

#define PI 3.14

double getTime()
{

    LARGE_INTEGER CompteurTimer, Frequence;

    double Temps;

    QueryPerformanceFrequency( &Frequence );
    QueryPerformanceCounter( &CompteurTimer );
    Temps = (double)CompteurTimer.QuadPart / (double)Frequence.QuadPart;

    return Temps;
}

size_t shrRoundUp( int group_size, int global_size ) 
{
    int r = global_size % group_size;
    if( r == 0 )
        return global_size;
    return global_size + group_size - r;
}

int convertToString(const char *filename, std::string& s)
{
	size_t size;
	char*  str;
	std::fstream f(filename, (std::fstream::in | std::fstream::binary));

	if(f.is_open())
	{
		size_t fileSize;
		f.seekg(0, std::fstream::end);
		size = fileSize = (size_t)f.tellg();
		f.seekg(0, std::fstream::beg);
		str = new char[size+1];
		if(!str)
		{
			f.close();
			return 0;
		}

		f.read(str, fileSize);
		f.close();
		str[size] = '\0';
		s = str;
		delete[] str;
		return 0;
	}
	std::cout<<"Error: failed to open file\n:"<<filename<<std::endl;
	return -1;
}

class OpenClCompute
{
public:
    int init()
    {
        cl_uint numPlatforms;	//the NO. of platforms
        cl_platform_id platform = NULL;	//the chosen platform
        cl_int	status = clGetPlatformIDs(0, NULL, &numPlatforms);
        if (status != CL_SUCCESS)
        {
            std::cout << "Error: Getting platforms!" << std::endl;
            return -1;
        }

        /*For clarity, choose the first available platform. */
        std::cout << numPlatforms;
        if(numPlatforms > 0)
        {
            cl_platform_id* platforms = (cl_platform_id* )malloc(numPlatforms* sizeof(cl_platform_id));
            status = clGetPlatformIDs(numPlatforms, platforms, NULL);
            platform = platforms[1];
            free(platforms);
        }

        /*Step 2:Query the platform and choose the first GPU device if has one.Otherwise use the CPU as device.*/
        cl_uint				numDevices = 0;
        status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);	
        if (numDevices == 0)	//no GPU available.
        {
            std::cout << "No GPU device available." << std::endl;
            std::cout << "Choose CPU as default device." << std::endl;
            status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 0, NULL, &numDevices);	
            mDevices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
            status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, numDevices, mDevices, NULL);
        }
        else
        {
            mDevices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
            status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, numDevices, mDevices, NULL);
        }

        /*Step 3: Create context.*/
        cl_context mContext = clCreateContext(NULL,1, mDevices,NULL,NULL,NULL);

        /*Step 4: Creating command queue associate with the context.*/
        cl_command_queue  mCommandQueue = clCreateCommandQueue(mContext, mDevices[0], 0, NULL);

        std::string sourceStr;
        status = convertToString("gpu.cl", sourceStr);
        const char *source = sourceStr.c_str();
        size_t sourceSize[] = {strlen(source)};
        //std::cout << source;
        cl_program mProgram = clCreateProgramWithSource(mContext, 1, &source, sourceSize, NULL);

        status=clBuildProgram(mProgram, 1,mDevices,NULL,NULL,NULL);
        char* build_log;
        size_t log_size;
        // First call to know the proper size
        clGetProgramBuildInfo(mProgram, mDevices[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        build_log = new char[log_size+1];
        // Second call to get the log
        clGetProgramBuildInfo(mProgram, mDevices[0], CL_PROGRAM_BUILD_LOG, log_size, build_log, NULL);
        build_log[log_size] = '\0';
        std::cout << build_log << std::endl;
        delete[] build_log;
        mKernel = clCreateKernel(mProgram,"compute_Coefs", NULL);

        //return status;

    /*int loadProgram(const char* filename)
    {
        std::string sourceStr;
        int status = convertToString(filename, sourceStr);
        const char *source = sourceStr.c_str();
        size_t sourceSize[] = {strlen(source)};
        std::cout << source;
        cl_program mProgram = clCreateProgramWithSource(mContext, 1, &source, sourceSize, NULL);

        status=clBuildProgram(mProgram, 1,mDevices,NULL,NULL,NULL);
        mKernel = clCreateKernel(mProgram,"compute_Coefs", NULL);

        return status;
    }*/

    //void runProgram()
    //{

        //allocating memory in GPU
        int pointNumber = 1000000;
        int coefNumber  = 100000;

        cl_int error;
        float* x = new float[pointNumber*6];
        cl_mem xKernel = clCreateBuffer(mContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, pointNumber*6*sizeof(float), 
            x, &error);

        float* coefs = new float[coefNumber];
        cl_mem coefsKernel = clCreateBuffer(mContext, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, coefNumber*sizeof(float), 
            coefs, &error);

        float* func = new float[coefNumber*6];
        cl_mem funcKernel = clCreateBuffer(mContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, coefNumber*6*sizeof(float), 
            func, &error);

        double t1 = getTime();

        float lambdamax[3];
        lambdamax[0] = 1.1;
        lambdamax[1] = 1.2;
        lambdamax[2] = 1.3;
        cl_mem lambdamaxKernel = clCreateBuffer(mContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 3*sizeof(float), 
            lambdamax, &error);

        float anglemax[3];
        anglemax[0] = 1.1;
        anglemax[1] = 1.2;
        anglemax[2] = 1.3;
        cl_mem anglemaxKernel = clCreateBuffer(mContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 3*sizeof(float), 
            anglemax, &error);

        error = clSetKernelArg(mKernel, 0, sizeof(cl_mem), (void *)&xKernel);
        error |= clSetKernelArg(mKernel, 1, sizeof(cl_mem), (void *)&funcKernel);
        error |= clSetKernelArg(mKernel, 2, sizeof(cl_mem), (void *)&coefsKernel);
        error |= clSetKernelArg(mKernel, 3, sizeof(cl_mem), (void *)&lambdamaxKernel);
        error |= clSetKernelArg(mKernel, 4, sizeof(cl_mem), (void *)&anglemaxKernel);
        error |= clSetKernelArg(mKernel, 5, sizeof(int), (void *)&pointNumber);
        error |= clSetKernelArg(mKernel, 6, sizeof(int), (void *)&coefNumber);
        if(error == CL_SUCCESS) std::cout << "OK";
        // Launching kernel
        const size_t local_ws = 64;	// Number of work-items per work-group
        // shrRoundUp returns the smallest multiple of local_ws bigger than size
        const size_t global_ws = shrRoundUp(local_ws, coefNumber);	// Total number of work-items
        error = clEnqueueNDRangeKernel(mCommandQueue, mKernel, 1, NULL, &global_ws, &local_ws, 0, NULL, NULL);
        if(error == CL_SUCCESS) std::cout << "OK";
        std::cout << coefs[0]<<std::endl;
        clEnqueueReadBuffer(mCommandQueue, coefsKernel, CL_TRUE, 0, coefNumber*sizeof(float), coefs, 0, NULL, NULL);
        std::cout << coefs[0];
        std::cout << "computation over\n";
        double t2 = getTime();
        std::cout << "total time" << t2-t1;
        return 0;
    }

    cl_device_id  *mDevices;
    cl_context mContext;
    cl_command_queue mCommandQueue;
    cl_program mProgram;
    cl_kernel mKernel;
};

int main()
{
    int i, j;
    char* value;
    size_t valueSize;
    cl_uint platformCount;
    cl_platform_id* platforms;
    cl_uint deviceCount;
    cl_device_id* devices;
    cl_uint maxComputeUnits;

    // get all platforms
    clGetPlatformIDs(0, NULL, &platformCount);
    platforms = (cl_platform_id*) malloc(sizeof(cl_platform_id) * platformCount);
    clGetPlatformIDs(platformCount, platforms, NULL);

    for (i = 0; i < platformCount; i++) {
        std::cout << "i" << i;
        // get all devices
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount);
        devices = (cl_device_id*) malloc(sizeof(cl_device_id) * deviceCount);
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, deviceCount, devices, NULL);

        // for each device print critical attributes
        for (j = 0; j < deviceCount; j++) {

            // print device name
            clGetDeviceInfo(devices[j], CL_DEVICE_NAME, 0, NULL, &valueSize);
            value = (char*) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DEVICE_NAME, valueSize, value, NULL);
            printf("%d. Device: %s\n", j+1, value);
            free(value);

            // print hardware device version
            clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, 0, NULL, &valueSize);
            value = (char*) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, valueSize, value, NULL);
            printf(" %d.%d Hardware version: %s\n", j+1, 1, value);
            free(value);

            // print software driver version
            clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, 0, NULL, &valueSize);
            value = (char*) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, valueSize, value, NULL);
            printf(" %d.%d Software version: %s\n", j+1, 2, value);
            free(value);

            // print c version supported by compiler for device
            clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, 0, NULL, &valueSize);
            value = (char*) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, valueSize, value, NULL);
            printf(" %d.%d OpenCL C version: %s\n", j+1, 3, value);
            free(value);

            // print parallel compute units
            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS,
                    sizeof(maxComputeUnits), &maxComputeUnits, NULL);
            printf(" %d.%d Parallel compute units: %d\n", j+1, 4, maxComputeUnits);

        }

        free(devices);

    }

    free(platforms);

    OpenClCompute gpu;
    gpu.init();
    //gpu.loadProgram("HelloWorld_Kernel.cl");
//    gpu.runProgram();


    /*unsigned int pointNumber = 100000;
    unsigned int coefNumber  = 100;

    float* x = new float[pointNumber*6];
    //float* v = new float[200000];

    float* coefs = new float[coefNumber];

    float* func = new float[coefNumber*6];

    double t1 = getTime();

    float lambdamax[3];
    lambdamax[0] = 1.1;
    lambdamax[1] = 1.2;
    lambdamax[2] = 1.3;
    float anglemax[3];
    anglemax[0] = 1.1;
    anglemax[1] = 1.2;
    anglemax[2] = 1.3;

    for(int j = 0;j<coefNumber;j++)
    {
        //std::cout << j << "\n";

        float coef1 = 2.*PI*func[j,0]/lambdamax[0];
        float coef2 = 2.*PI*func[j,1]/lambdamax[1];
        float coef3 = 2.*PI*func[j,2]/lambdamax[2];
        float coef4 = 2.*PI*func[j,3]/anglemax[0];
        float coef5 = 2.*PI*func[j,4]/anglemax[1];
        float coef6 = 2.*PI*func[j,5]/anglemax[2];

        for(int i=0;i<pointNumber;i++)
        {
            coefs[j] += exp(coef1*x[i,0])*exp(coef2*x[i,1])*
                exp(coef3*x[i,2])*
                exp(coef4*x[i,3])*exp(coef5*x[i,4])*
                exp(coef6*x[i,5]);
        }


    }
    double t2 = getTime();
    std::cout<< "final Time: "<<t2-t1;*/
    char a;
    std::cin >> a;
}