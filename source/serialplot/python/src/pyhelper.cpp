#include "pyhelper.h"


PyHelper::PyHelper(const char* moduleName, const char* className, float sampling, const char* columnName)
    : mModuleName(moduleName)
    , mClassName(className)
    , mColumnName(columnName)
    , mSampling(sampling)
{
    // Load the module object
    pModule = PyImport_ImportModule(mModuleName);
    if (pModule == nullptr) {
        PyErr_Print();
    }

    // Builds the name of a callable class
    pPythonClass = PyObject_GetAttrString(pModule, mClassName);
    if (pPythonClass == nullptr) {
        PyErr_Print();
    }
    
    pArgs  = Py_BuildValue("(fs)", mSampling, mColumnName);

    // Creates an instance of the class
    if (PyCallable_Check(pPythonClass)) {
        pObject = PyObject_CallObject(pPythonClass, pArgs);
    } else {
        PyErr_Print();
    }    
}

PyHelper::~PyHelper()
{
    Py_XDECREF(pModule);
    Py_XDECREF(pPythonClass);
    Py_XDECREF(pObject);
    Py_DECREF(pArgs);
    Py_DECREF(pMethod);
    Py_DECREF(pCalc);
}

double PyHelper::getPyMethod(const char* method_name)
{
    pMethod = PyObject_GetAttrString(pObject, method_name);
    if (pMethod == nullptr) {
        PyErr_Print();
    }

    // Execute method
    pCalc = PyObject_CallObject(pMethod, NULL);
    if (pCalc == nullptr) {
        PyErr_Print();
    }

    return PyFloat_AsDouble(pCalc); 
}
