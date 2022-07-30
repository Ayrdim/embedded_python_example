#include <python3.10/Python.h>
#include "avp_transformer.h"
#include <string.h>


static const char * SCRIPT_PATH = ".";


static PyObject * callPythonFunctionFromModule(
    char const * const moduleName,
    char const * const functionName,
    char const * const arg);
static PyObject * getPythonFunctionFromModule(
    const char * const moduleName,
    const char * const functionName);
static PyObject * getPythonFunctionFromModule(
    const char * const moduleName,
    const char * const functionName);
static PyObject * importPythonModule(const char * const moduleName);
static int updateValue(
    char * const value,
    unsigned int maxValSz,
    PyObject * pyResult);


int AVP_TransformValue(
    char const * const moduleName,
    char const * const functionName,
    char * const value,
    unsigned int maxValSz)
{
    int isFailed = 1;

    if ((NULL != moduleName) &&
        (NULL != functionName) && 
        (NULL != value))
    {
        PyObject *pyResult = NULL;
        setenv("PYTHONPATH", SCRIPT_PATH, 1);
        Py_Initialize();
        
        if ((pyResult = callPythonFunctionFromModule(moduleName, functionName, value)))
        {
            isFailed = updateValue(value, maxValSz, pyResult);
            Py_CLEAR(pyResult);
        }

        if (0 != Py_FinalizeEx())
        {
            printf("Failed to finalise cpython\n");
        }
    }

    return isFailed;
}

static PyObject * callPythonFunctionFromModule(
    char const * const moduleName,
    char const * const functionName,
    char const * const arg)
{
    char * result = NULL;
    PyObject *pyFunction = NULL;
    PyObject *pyResult = NULL;

    if ((pyFunction = getPythonFunctionFromModule(moduleName, functionName)))
    {
        pyResult = PyObject_CallFunction(pyFunction, "s", arg);
        Py_CLEAR(pyFunction);
    }

    return pyResult;
}

static PyObject * getPythonFunctionFromModule(
    const char * const moduleName,
    const char * const functionName)
{
    PyObject * pyFunction = NULL;
    PyObject * pyModule = NULL;
    
    if ((pyModule = importPythonModule(moduleName)))
    {
        pyFunction = PyObject_GetAttrString(pyModule, functionName);
        Py_CLEAR(pyModule);
    }

    return pyFunction;
}

static PyObject * importPythonModule(const char * const moduleName)
{
    PyObject * pyModule = NULL;
    PyObject *pyModuleName = NULL;

    if ((pyModuleName = PyUnicode_FromString(moduleName)))
    {
        pyModule = PyImport_Import(pyModuleName);
        Py_CLEAR(pyModuleName);
    }

    return pyModule;
}

static int setValue(
    char * const value,
    unsigned int maxValSz,
    char const * const newVal)
{
    int isFailed = 1;
    size_t newValSz = strlen(newVal); 

    if (newValSz < maxValSz)
    {
        strcpy(value, newVal);
        isFailed = 0;
    }
    else
    {
        printf("Transform returned a string larger than what we can hold\n");
    }

    return isFailed;
}

static int updateValue(
    char * const value,
    unsigned int maxValSz,
    PyObject * pyResult)
{
    int isFailed = 1;
    char * newVal = NULL;
    PyObject *pyBytes = NULL;
    
    if ((pyBytes = PyUnicode_AsUTF8String(pyResult)))
    {
        newVal = PyBytes_AsString(pyBytes);
        isFailed = setValue(value, maxValSz, newVal);
        Py_CLEAR(pyBytes);
    }

    return isFailed;
}