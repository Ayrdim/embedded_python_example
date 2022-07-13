#include <python3.10/Python.h>

int main(int argc, char *argv[])
{
    PyObject *pName = NULL;
    PyObject *pModule = NULL;
    PyObject *pFunc = NULL;
    PyObject *presult = NULL;

    setenv("PYTHONPATH", ".", 1);
    Py_Initialize();

    if ((pName = PyUnicode_FromString("script")))
    {
        if ((pModule = PyImport_Import(pName))) 
        {
            if ((pFunc = PyObject_GetAttrString(pModule, "foo")))
            {
                if ((presult = PyObject_CallObject(pFunc, NULL)))
                {
                    printf("If foo printed to stdout then mission accomplished!\n");
                    Py_XDECREF(presult);
                }
                Py_XDECREF(pFunc);
            }

            if ((pFunc = PyObject_GetAttrString(pModule, "add")))
            {
                if ((presult = PyObject_CallFunction(pFunc, "iis", 8, 4, "HUGE")))
                {
                    printf("Result is %ld\n", PyLong_AsLong(presult));
                }
            }

        }
    }
    if (PyErr_Occurred()) 
    {
        PyErr_Print();
    }

    Py_XDECREF(presult);
    Py_XDECREF(pFunc);
    Py_XDECREF(pModule);
    Py_XDECREF(pName);

    Py_Finalize();
}