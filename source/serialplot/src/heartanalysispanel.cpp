#include "heartanalysispanel.h"
#include "ui_heartanalysispanel.h"

#include <QDebug>

HeartAnalysisPanel::HeartAnalysisPanel(FftControl *fftControl, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeartAnalysisPanel)
{
    _fftControl = fftControl;

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

void HeartAnalysisPanel::onButtonAnalyze(bool state)
{
    emit buttonAnalyzePressed();

    PyObject *moduleB, *pythonClassB, *objectB, *argsB, *methodB, *calcB;
    PyObject *moduleF, *pythonClassF, *objectF, *argsF, *methodF, *calcF;

    // ----------------- Create class object for Base signal  -------------------
    // Load the module object
    moduleB = PyImport_ImportModule("python_modules.biosignal_analysis");
    if (moduleB == nullptr) {
        PyErr_Print();
    }
    // Builds the name of a callable class
    pythonClassB = PyObject_GetAttrString(moduleB, "HeartAnalysis");
    if (pythonClassB == nullptr) {
        PyErr_Print();
    }


    argsB  = Py_BuildValue("(fs)", static_cast<float>(_fftControl->getSamplingFreq()), "Basic_signal");
    // Creates an instance of the class
    objectB = PyObject_CallObject(pythonClassB, argsB);
    if (objectB == nullptr) {
        PyErr_Print();
    }

    // --------------- Create class object for Filtered signal  ----------------
    // Load the module object
    moduleF = PyImport_ImportModule("python_modules.biosignal_analysis");
    if (moduleF == nullptr) {
        PyErr_Print();
    }
    // Builds the name of a callable class
    pythonClassF = PyObject_GetAttrString(moduleF, "HeartAnalysis");
    if (pythonClassF == nullptr) {
        PyErr_Print();
    }

    argsF  = Py_BuildValue("(fs)", static_cast<float>(_fftControl->getSamplingFreq()), "Filtered_signal");
    // Creates an instance of the class
    objectF = PyObject_CallObject(pythonClassF, argsF);
    if (objectF == nullptr) {
        PyErr_Print();
    }
    // ----------------- Get Base BPM -------------------
    // Get class method
    methodB = PyObject_GetAttrString(objectB, "get_bpm");
    if (methodB == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcB = PyObject_CallObject(methodB, NULL);
    if (calcB == nullptr) {
        PyErr_Print();
    }
    // Get result
    double bpmBase = PyFloat_AsDouble(calcB);
    // ----------------- Get Base IBI -------------------
    // Get class method
    methodB = PyObject_GetAttrString(objectB, "get_ibi");
    if (methodB == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcB = PyObject_CallObject(methodB, NULL);
    if (calcB == nullptr) {
        PyErr_Print();
    }
    // Get result
    double ibiBase = PyFloat_AsDouble(calcB);
    // ----------------- Get Base SDNN -------------------
    // Get class method
    methodB = PyObject_GetAttrString(objectB, "get_sdnn");
    if (methodB == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcB = PyObject_CallObject(methodB, NULL);
    if (calcB == nullptr) {
        PyErr_Print();
    }
    // Get result
    double sdnnBase = PyFloat_AsDouble(calcB);
    // ----------------- Get Base SDSD -------------------
    // Get class method
    methodB = PyObject_GetAttrString(objectB, "get_sdsd");
    if (methodB == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcB = PyObject_CallObject(methodB, NULL);
    if (calcB == nullptr) {
        PyErr_Print();
    }
    // Get result
    double sdsdBase = PyFloat_AsDouble(calcB);
    // ----------------- Get Base RMSSD -------------------
    // Get class method
    methodB = PyObject_GetAttrString(objectB, "get_rmssd");
    if (methodB == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcB = PyObject_CallObject(methodB, NULL);
    if (calcB == nullptr) {
        PyErr_Print();
    }
    // Get result
    double rmssdBase = PyFloat_AsDouble(calcB);
    // ----------------- Get Base HR_MAD -------------------
    // Get class method
    methodB = PyObject_GetAttrString(objectB, "get_hr_mad");
    if (methodB == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcB = PyObject_CallObject(methodB, NULL);
    if (calcB == nullptr) {
        PyErr_Print();
    }
    // Get result
    double hrMadBase = PyFloat_AsDouble(calcB);
    // ----------------- Get Base Breathingrate -------------------
    // Get class method
    methodB = PyObject_GetAttrString(objectB, "get_breathingrate");
    if (methodB == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcB = PyObject_CallObject(methodB, NULL);
    if (calcB == nullptr) {
        PyErr_Print();
    }
    // Get result
    double breathingrateBase = PyFloat_AsDouble(calcB);

    // ----------------- Get Filtered BPM -------------------
    // Get class method
    methodF = PyObject_GetAttrString(objectF, "get_bpm");
    if (methodF == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcF = PyObject_CallObject(methodF, NULL);
    if (calcF == nullptr) {
        PyErr_Print();
    }
    // Get result
    double bpmFiltered = PyFloat_AsDouble(calcF);
    // ----------------- Get Filtered IBI -------------------
    // Get class method
    methodF = PyObject_GetAttrString(objectF, "get_ibi");
    if (methodF == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcF = PyObject_CallObject(methodF, NULL);
    if (calcF == nullptr) {
        PyErr_Print();
    }
    // Get result
    double ibiFiltered = PyFloat_AsDouble(calcF);
    // ----------------- Get Filtered SDNN -------------------
    // Get class method
    methodF = PyObject_GetAttrString(objectF, "get_sdnn");
    if (methodF == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcF = PyObject_CallObject(methodF, NULL);
    if (calcF == nullptr) {
        PyErr_Print();
    }
    // Get result
    double sdnnFiltered = PyFloat_AsDouble(calcF);
    // ----------------- Get Filtered SDSD -------------------
    // Get class method
    methodF = PyObject_GetAttrString(objectF, "get_sdsd");
    if (methodF == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcF = PyObject_CallObject(methodF, NULL);
    if (calcF == nullptr) {
        PyErr_Print();
    }
    // Get result
    double sdsdFiltered = PyFloat_AsDouble(calcF);
    // ----------------- Get Filtered RMSSD -------------------
    // Get class method
    methodF = PyObject_GetAttrString(objectF, "get_rmssd");
    if (methodF == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcF = PyObject_CallObject(methodF, NULL);
    if (calcF == nullptr) {
        PyErr_Print();
    }
    // Get result
    double rmssdFiltered = PyFloat_AsDouble(calcF);
    // ----------------- Get Filtered HR_MAD -------------------
    // Get class method
    methodF = PyObject_GetAttrString(objectF, "get_hr_mad");
    if (methodF == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcF = PyObject_CallObject(methodF, NULL);
    if (calcF == nullptr) {
        PyErr_Print();
    }
    // Get result
    double hrMadFiltered = PyFloat_AsDouble(calcF);
    // ----------------- Get Filtered Breathingrate -------------------
    // Get class method
    methodF = PyObject_GetAttrString(objectF, "get_breathingrate");
    if (methodF == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcF = PyObject_CallObject(methodF, NULL);
    if (calcF == nullptr) {
        PyErr_Print();
    }
    // Get result
    double breathingrateFiltered = PyFloat_AsDouble(calcF);

    qDebug() << "Base bpm: " << bpmBase;
    qDebug() << "Base ibi: " << ibiBase;
    qDebug() << "Base sdnn: " << sdnnBase;
    qDebug() << "Base sdsd: " << sdsdBase;
    qDebug() << "Base rmssd: " << rmssdBase;
    qDebug() << "Base hr_mad: " << hrMadBase;
    qDebug() << "Base breathingrate: " << breathingrateBase;

    qDebug() << "Filtered bpm: " << bpmFiltered;
    qDebug() << "Filtered ibi: " << ibiFiltered;
    qDebug() << "Filtered sdnn: " << sdnnFiltered;
    qDebug() << "Filtered sdsd: " << sdsdFiltered;
    qDebug() << "Filtered rmssd: " << rmssdFiltered;
    qDebug() << "Filtered hr_mad: " << hrMadFiltered;
    qDebug() << "Filtered breathingrate: " << breathingrateFiltered;

    Py_DECREF(moduleB);
    Py_DECREF(pythonClassB);
    Py_DECREF(argsF);
    Py_DECREF(moduleF);
    Py_DECREF(pythonClassF);
    Py_DECREF(argsB);
    Py_DECREF(calcB);
    Py_DECREF(methodB);
    Py_DECREF(objectB);
    Py_DECREF(calcF);
    Py_DECREF(methodF);
    Py_DECREF(objectF);





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
