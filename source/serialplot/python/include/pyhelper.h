#ifndef PYHELPER_HPP
#define PYHELPER_HPP
#pragma once

#pragma push_macro("slots")
#undef slots
#include "Python.h"
#pragma pop_macro("slots")

class CPyInstance
{
public:
	CPyInstance()
	{
		Py_Initialize();
	}

	~CPyInstance()
	{
		Py_Finalize();
	}
};

class PyHelper
{
public:
	PyHelper(const char* moduleName, const char* className, float sampling, const char* columnName);
	~PyHelper();

	double getPyMethod(const char* method_name);

private:
	PyObject *pModule, *pPythonClass, *pObject, *pArgs, *pMethod, *pCalc;
	const char* mModuleName, * mClassName, *mColumnName;
	float mSampling;
};

#endif // PYHELPER_HPP
