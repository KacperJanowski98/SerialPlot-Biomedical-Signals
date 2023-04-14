#include "heartanalysispanel.h"
#include "ui_heartanalysispanel.h"

#include <QDebug>

HeartAnalysisPanel::HeartAnalysisPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeartAnalysisPanel)
{
//    _fftControl = fftControl;

    ui->setupUi(this);

//    CPyInstance pyInstance;

//    PyRun_SimpleString("import sys");
//    PyRun_SimpleString("import os");
//    PyRun_SimpleString("sys.path.append(os.getcwd())");

//    pyInstance = new CPyInstance();

    connect(ui->pbAnalyze, SIGNAL(clicked(bool)),
            this, SLOT(onButtonAnalyze(bool)));
}

HeartAnalysisPanel::~HeartAnalysisPanel()
{
    delete ui;
//    delete pyInstance;
}

void HeartAnalysisPanel::createClassObject(PyObject *object, float sampling, const char* columnName)
{
    PyObject *module, *python_class, *args;
    // Load the module object
    module = PyImport_ImportModule("python_modules.biosignal_analysis");
    if (module == nullptr) {
        PyErr_Print();
    }

    // Builds the name of a callable class
    python_class = PyObject_GetAttrString(module, "HeartAnalysis");
    if (python_class == nullptr) {
        PyErr_Print();
    }
    Py_DECREF(module);

    args  = Py_BuildValue("(fs)", sampling, columnName);

    // Creates an instance of the class
    object = PyObject_CallObject(python_class, args);

    Py_DECREF(python_class);
    Py_DECREF(args);
}

double HeartAnalysisPanel::getMethod(PyObject *object, const char* methodName)
{
    PyObject *method, *calc;
    // Get class method
    method = PyObject_GetAttrString(object, "get_bpm");
    if (method == nullptr) {
        PyErr_Print();
    }

    // Execute method
    calc = PyObject_CallObject(method, NULL);
    if (calc == nullptr) {
        PyErr_Print();
    }

    double result = PyFloat_AsDouble(calc);
    Py_DECREF(calc);
    Py_DECREF(method);

    return result;
}

void HeartAnalysisPanel::onButtonAnalyze(bool state)
{
//    emit buttonAnalyzePressed();

    PyObject *module, *python_class, *object, *args, *method, *calc;
//         PyObject *module, *python_class, *object, *args;
//         double result = 0.0;

    // Load the module object
    module = PyImport_ImportModule("python_modules.biosignal_analysis");
    if (module == nullptr) {
        PyErr_Print();
    }

    // Builds the name of a callable class
    python_class = PyObject_GetAttrString(module, "HeartAnalysis");
    if (python_class == nullptr) {
        PyErr_Print();
    }

    args  = Py_BuildValue("(fs)", 300.0f, "Basic_signal");


    // Creates an instance of the class
    if (PyCallable_Check(python_class)) {
        object = PyObject_CallObject(python_class, args);
    } else {
        PyErr_Print();
    }

    // Get class method
    method = PyObject_GetAttrString(object, "get_bpm");
    if (method == nullptr) {
        PyErr_Print();

    }

    // Execute method
    calc = PyObject_CallObject(method, NULL);
    if (calc == nullptr) {
        PyErr_Print();
    }

    // Get result
    double result = PyFloat_AsDouble(calc);
    qDebug() << result;


//    float samplingFreq = _fftControl->getSamplingFreq();
//    PyHelper objBase(
//            "python_modules.biosignal_analysis",
//            "HeartAnalysis",
//            300.0f,
//            "Basic_signal"
//        );
//    PyHelper objFiltered(
//            "python_modules.biosignal_analysis",
//            "HeartAnalysis",
//            samplingFreq,
//            "Filtered_signal"
//        );

//    objBase = std::make_unique<PyHelper>(
//            "python_modules.biosignal_analysis",
//            "HeartAnalysis",
//            300.0f,
//            "Basic_signal"
//        );

//    objFiltered = std::make_unique<PyHelper>(
//            "python_modules.biosignal_analysis",
//            "HeartAnalysis",
//            samplingFreq,
//            "Filtered_signal"
//        );
//    objBase = new PyHelper(
//        "python_modules.biosignal_analysis",
//        "HeartAnalysis",
//        samplingFreq,
//        "Basic_signal"
//        );

//    objFiltered = new PyHelper(
//        "python_modules.biosignal_analysis",
//        "HeartAnalysis",
//        samplingFreq,
//        "Filtered_signal"
//        );

//    delete objBase;
//    delete objFiltered;
}
